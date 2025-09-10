#include "mpm/elements/maps/TempoMap.h"
#include "mpm/Mpm.h"
#include "xml/Helper.h"
#include <algorithm>
#include <cmath>
#include <limits>
#include <iostream>

namespace meico {
namespace mpm {

TempoMap::TempoMap() : GenericMap(Mpm::TEMPO_MAP) {
}

std::unique_ptr<TempoMap> TempoMap::createTempoMap() {
    return std::make_unique<TempoMap>();
}

int TempoMap::addTempo(double date, const std::string& bpm, const std::string& transitionTo, 
                       double beatLength, double meanTempoAt, const std::string& id) {
    auto data = std::make_unique<TempoData>();
    data->startDate = date;
    data->bpmString = bpm;
    data->transitionToString = transitionTo;
    data->beatLength = beatLength;
    data->meanTempoAt = meanTempoAt;
    data->xmlId = id;
    
    // Try to parse numeric values
    try {
        data->bpm = std::stod(bpm);
    } catch (const std::invalid_argument&) {
        // Keep as string
    }
    
    if (!transitionTo.empty()) {
        try {
            data->transitionTo = std::stod(transitionTo);
        } catch (const std::invalid_argument&) {
            // Keep as string
        }
    }
    
    return addTempo(std::move(data));
}

int TempoMap::addTempo(double date, double bpm, double beatLength) {
    auto data = std::make_unique<TempoData>();
    data->startDate = date;
    data->bpm = bpm;
    data->beatLength = beatLength;
    
    return addTempo(std::move(data));
}

int TempoMap::addTempo(std::unique_ptr<TempoData> data) {
    if (!data) {
        std::cerr << "Cannot add null tempo data" << std::endl;
        return -1;
    }
    
    if (data->bpm == 0.0 && data->bpmString.empty()) {
        std::cerr << "Cannot add tempo, bpm not specified." << std::endl;
        return -1;
    }
    
    // Find insertion point
    auto insertPos = std::lower_bound(tempoData.begin(), tempoData.end(), data->startDate,
        [](const supplementary::KeyValue<double, std::unique_ptr<TempoData>>& kv, double date) {
            return kv.getKey() < date;
        });
    
    int index = static_cast<int>(std::distance(tempoData.begin(), insertPos));
    
    supplementary::KeyValue<double, std::unique_ptr<TempoData>> kv(data->startDate, std::move(data));
    tempoData.insert(insertPos, std::move(kv));
    
    return index;
}

TempoData* TempoMap::getTempoDataAt(double date) const {
    // Find the tempo data that applies at the given date
    for (int i = getElementIndexBeforeAt(date); i >= 0; --i) {
        TempoData* td = getTempoDataOf(i);
        if (td != nullptr) {
            return td;
        }
    }
    return nullptr;
}

TempoData* TempoMap::getTempoDataOf(int index) const {
    if (tempoData.empty() || index < 0 || index >= static_cast<int>(tempoData.size())) {
        return nullptr;
    }
    
    TempoData* data = tempoData[index].getValue().get();
    if (data) {
        // Update end date
        data->endDate = getEndDate(index);
    }
    
    return data;
}

double TempoMap::getTempoAt(double date) const {
    TempoData* tempoData = getTempoDataAt(date);
    return getTempoAt(date, tempoData);
}

double TempoMap::getTempoAt(double date, const TempoData* tempoData) {
    if (tempoData == nullptr) {
        return 100.0;  // Default tempo
    }
    
    // Compute constant tempo
    if (tempoData->isConstantTempo()) {
        return tempoData->bpm;
    }
    
    // Compute continuous tempo transition
    if (date == tempoData->endDate) {
        return tempoData->transitionTo;
    }
    
    double result = (date - tempoData->startDate) / (tempoData->endDate - tempoData->startDate);
    
    double exponent = tempoData->exponent;
    if (exponent == 0.0) {
        exponent = (tempoData->meanTempoAt == 0.0) ? 1.0 : computeExponent(tempoData->meanTempoAt);
    }
    
    result = std::pow(result, exponent);
    result = result * (tempoData->transitionTo - tempoData->bpm) + tempoData->bpm;
    
    return result;
}

void TempoMap::renderTempoToMap(GenericMap& map, int ppq) {
    // This would implement the complex timing computation
    // For now, provide a basic implementation
    std::cout << "TempoMap::renderTempoToMap not fully implemented yet" << std::endl;
}

void TempoMap::renderTempoToMap(GenericMap& map, int ppq, TempoMap* tempoMap) {
    if (tempoMap != nullptr) {
        tempoMap->renderTempoToMap(map, ppq);
        return;
    }
    
    // Fallback: no tempo map provided
    std::cout << "No tempo map provided, using default timing" << std::endl;
}

double TempoMap::computeDiffTiming(double date, int ppq, const TempoData* tempoData) {
    // No tempo data
    if (tempoData == nullptr) {
        return computeMillisecondsForNoTempo(date, ppq);
    }
    
    // Constant tempo
    if (tempoData->isConstantTempo()) {
        return computeMillisecondsForConstantTempo(date, ppq, tempoData);
    }
    
    // Continuous tempo transition
    return computeMillisecondsForTempoTransition(date, ppq, tempoData);
}

bool TempoMap::applyToMsmPart(Element msmPart) {
    if (!msmPart || tempoData.empty()) {
        return false;
    }
    
    bool modified = false;
    
    // Find all note elements in the part - try both direct dated and header/dated paths
    Element scoreElement;
    
    // First try: part -> dated -> score (for some MSM structures)
    scoreElement = xml::Helper::getFirstChildElement(msmPart, "dated");
    if (scoreElement) {
        scoreElement = xml::Helper::getFirstChildElement(scoreElement, "score");
    }
    
    // If not found, try: part -> header -> dated -> score (for Bach MSM structure)
    if (!scoreElement) {
        auto headerElement = xml::Helper::getFirstChildElement(msmPart, "header");
        if (headerElement) {
            auto datedElement = xml::Helper::getFirstChildElement(headerElement, "dated");
            if (datedElement) {
                scoreElement = xml::Helper::getFirstChildElement(datedElement, "score");
            }
        }
    }
    
    if (scoreElement) {
        // Process all notes to add timing-related attributes
        for (auto note : scoreElement.children("note")) {
            auto dateAttr = note.attribute("date");
            if (dateAttr) {
                double noteDate = xml::Helper::parseDouble(dateAttr.value());
                double currentTempo = getTempoAt(noteDate);
                
                // Add tempo information as an attribute for downstream processing
                note.append_attribute("tempo") = std::to_string(currentTempo).c_str();
                modified = true;
            }
        }
    }
    
    return modified;
}

void TempoMap::parseData(const Element& xmlElement) {
    GenericMap::parseData(xmlElement);
    
    // Parse tempo elements from XML
    std::vector<Element> tempoElements = xml::Helper::getChildElements(xmlElement, "tempo");
    
    for (const Element& tempoElement : tempoElements) {
        auto data = std::make_unique<TempoData>(tempoElement);
        addTempo(std::move(data));
    }
}

int TempoMap::getElementIndexBeforeAt(double date) const {
    // Find the index of the last tempo element that starts at or before the given date
    for (int i = static_cast<int>(tempoData.size()) - 1; i >= 0; --i) {
        if (tempoData[i].getKey() <= date) {
            return i;
        }
    }
    return -1;
}

double TempoMap::getEndDate(int index) const {
    // Get the date of the subsequent tempo element or max value
    double endDate = std::numeric_limits<double>::max();
    
    for (int j = index + 1; j < static_cast<int>(tempoData.size()); ++j) {
        endDate = tempoData[j].getKey();
        break;
    }
    
    return endDate;
}

double TempoMap::computeExponent(double meanTempoAt) {
    return std::log(0.5) / std::log(meanTempoAt);
}

double TempoMap::computeMillisecondsForNoTempo(double date, int ppq) {
    // Default tempo of 100 bpm and beatLength=0.25
    return (600.0 * date) / ppq;
}

double TempoMap::computeMillisecondsForConstantTempo(double date, int ppq, const TempoData* tempoData) {
    return ((15000.0 * (date - tempoData->startDate)) / (tempoData->bpm * tempoData->beatLength * ppq));
}

double TempoMap::computeMillisecondsForTempoTransition(double date, int ppq, const TempoData* tempoData) {
    // Simpson's rule for numerical integration
    // The number of iterations of the Simpson's rule = N/2; 16th precision; N must be even!
    double N = 2.0 * static_cast<long>((date - tempoData->startDate) / (static_cast<double>(ppq) / 4));
    if (N == 0.0) {
        N = 2.0;
    }
    
    // Compute some often needed values
    double n = N / 2.0;
    double x = (date - tempoData->startDate) / N;
    
    // Compute Simpson's sum
    double resultConst = ((date - tempoData->startDate) * 5000.0) / (N * tempoData->beatLength * ppq);
    double resultSum = 1.0 / tempoData->bpm + 1.0 / getTempoAt(date, tempoData);
    
    for (int k = 1; k < n; ++k) {
        resultSum += 2.0 / getTempoAt(tempoData->startDate + 2 * k * x, tempoData);
    }
    
    for (int k = 1; k <= n; ++k) {
        resultSum += 4.0 / getTempoAt(tempoData->startDate + (2 * k - 1) * x, tempoData);
    }
    
    return resultConst * resultSum;
}

} // namespace mpm
} // namespace meico