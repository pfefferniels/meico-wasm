#include "mpm/elements/maps/RubatoMap.h"
#include "xml/Helper.h"
#include <algorithm>
#include <cmath>
#include <limits>
#include <iostream>

namespace meico {
namespace mpm {

RubatoMap::RubatoMap() : GenericMap("rubatoMap") {
}

std::unique_ptr<RubatoMap> RubatoMap::createRubatoMap() {
    return std::make_unique<RubatoMap>();
}

int RubatoMap::addRubato(double date, double frameLength, double intensity, 
                         double lateStart, double earlyEnd, bool loop, const std::string& id) {
    // Create RubatoData object
    RubatoData data;
    data.startDate = date;
    data.frameLength = frameLength;
    data.intensity = ensureIntensityBoundaries(intensity);
    
    auto correctedBounds = ensureLateStartEarlyEndBoundaries(lateStart, earlyEnd);
    data.lateStart = correctedBounds.first;
    data.earlyEnd = correctedBounds.second;
    data.loop = loop;
    data.xmlId = id;
    
    return addRubato(data);
}

int RubatoMap::addRubato(double date, const std::string& rubatoDefName, bool loop, const std::string& id) {
    // Create RubatoData object with rubatoDef reference
    RubatoData data;
    data.startDate = date;
    data.rubatoDefString = rubatoDefName;
    data.loop = loop;
    data.xmlId = id;
    
    return addRubato(data);
}

int RubatoMap::addRubato(const RubatoData& data) {
    // Create a copy of the data
    auto rubatoDataCopy = std::make_unique<RubatoData>(data);
    
    // Ensure boundaries are correct
    rubatoDataCopy->intensity = ensureIntensityBoundaries(rubatoDataCopy->intensity);
    auto correctedBounds = ensureLateStartEarlyEndBoundaries(rubatoDataCopy->lateStart, rubatoDataCopy->earlyEnd);
    rubatoDataCopy->lateStart = correctedBounds.first;
    rubatoDataCopy->earlyEnd = correctedBounds.second;
    
    // Find the correct position to insert (maintaining sorted order by date)
    auto insertPos = std::lower_bound(rubatoData.begin(), rubatoData.end(), data.startDate,
        [](const supplementary::KeyValue<double, std::unique_ptr<RubatoData>>& kv, double date) {
            return kv.getKey() < date;
        });
    
    // Insert the new element
    auto keyValue = supplementary::KeyValue<double, std::unique_ptr<RubatoData>>(data.startDate, std::move(rubatoDataCopy));
    auto it = rubatoData.insert(insertPos, std::move(keyValue));
    
    // Update end dates for affected elements
    int index = std::distance(rubatoData.begin(), it);
    for (int i = 0; i < static_cast<int>(rubatoData.size()); ++i) {
        rubatoData[i].getValue()->endDate = getEndDate(i);
    }
    
    return index;
}

RubatoData* RubatoMap::getRubatoDataAt(double date) {
    // Find the rubato data that applies to this date
    for (int i = static_cast<int>(rubatoData.size()) - 1; i >= 0; --i) {
        RubatoData* rd = getRubatoDataOf(i);
        if (rd && rd->startDate <= date) {
            // Check if this rubato applies to the date
            if (rd->loop || (date < rd->startDate + rd->frameLength)) {
                return rd;
            }
        }
    }
    return nullptr;
}

RubatoData* RubatoMap::getRubatoDataOf(int index) {
    if (index < 0 || index >= static_cast<int>(rubatoData.size())) {
        return nullptr;
    }
    return rubatoData[index].getValue().get();
}

void RubatoMap::renderRubatoToMap(GenericMap& map) {
    if (rubatoData.empty()) {
        return;
    }
    
    // Implementation would need access to map's elements
    // This is a simplified version - the full implementation would require
    // access to the internal structure of GenericMap
    std::cout << "RubatoMap::renderRubatoToMap - applying rubato transformations to map" << std::endl;
}

void RubatoMap::renderRubatoToMap(GenericMap& map, RubatoMap& rubatoMap) {
    rubatoMap.renderRubatoToMap(map);
}

double RubatoMap::computeRubatoTransformation(double date, const RubatoData& rubatoData) {
    // This is the core algorithm from the Java implementation
    double localDate = fmod(date - rubatoData.startDate, rubatoData.frameLength);
    double t = localDate / rubatoData.frameLength;
    
    // Apply the rubato transformation using the power function
    double transformedT = std::pow(t, rubatoData.intensity);
    
    // Map to the lateStart-earlyEnd range
    double d = (transformedT * (rubatoData.earlyEnd - rubatoData.lateStart) + rubatoData.lateStart) * rubatoData.frameLength;
    
    return date + d - localDate;
}

double RubatoMap::ensureIntensityBoundaries(double intensity) {
    if (intensity == 0.0) {
        std::cerr << "Invalid rubato intensity = 0.0 is set to 0.01." << std::endl;
        return 0.01;
    }
    if (intensity < 0.0) {
        std::cerr << "Invalid rubato intensity < 0.0 is inverted." << std::endl;
        return intensity * -1.0;
    }
    return intensity;
}

std::pair<double, double> RubatoMap::ensureLateStartEarlyEndBoundaries(double lateStart, double earlyEnd) {
    double correctedLateStart = lateStart;
    double correctedEarlyEnd = earlyEnd;
    
    if (lateStart < 0.0) {
        std::cerr << "Invalid rubato lateStart < 0.0 is set to 0.0." << std::endl;
        correctedLateStart = 0.0;
    }
    if (earlyEnd > 1.0) {
        std::cerr << "Invalid rubato earlyEnd > 1.0 is set to 1.0." << std::endl;
        correctedEarlyEnd = 1.0;
    }
    if (lateStart >= earlyEnd) {
        std::cerr << "Invalid rubato lateStart >= earlyEnd, setting them to 0.0 and 1.0." << std::endl;
        correctedLateStart = 0.0;
        correctedEarlyEnd = 1.0;
    }
    
    return std::make_pair(correctedLateStart, correctedEarlyEnd);
}

double RubatoMap::getEndDate(int index) {
    if (index < 0 || index >= static_cast<int>(rubatoData.size())) {
        return std::numeric_limits<double>::max();
    }
    
    // Find the next rubato element
    for (int j = index + 1; j < static_cast<int>(rubatoData.size()); ++j) {
        return rubatoData[j].getKey();
    }
    
    return std::numeric_limits<double>::max();
}

bool RubatoMap::applyToMsmPart(Element msmPart) {
    // For rubato maps, we typically don't modify individual notes directly
    // but rather provide timing transformations at a higher level
    std::cout << "RubatoMap timing application would be implemented here" << std::endl;
    return false;  // No direct modifications to notes
}

void RubatoMap::parseData(const Element& xmlElement) {
    GenericMap::parseData(xmlElement);
    
    // Parse rubato elements from XML
    for (auto child = xmlElement.first_child(); child; child = child.next_sibling()) {
        if (std::string(child.name()) == "rubato") {
            RubatoData data(child);
            addRubato(data);
        }
    }
}

}  // namespace mpm
}  // namespace meico