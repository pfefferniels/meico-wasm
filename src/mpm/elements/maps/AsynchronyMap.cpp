#include "mpm/elements/maps/AsynchronyMap.h"
#include "mpm/Mpm.h"
#include "xml/Helper.h"
#include <algorithm>
#include <iostream>

namespace meico {
namespace mpm {

AsynchronyMap::AsynchronyMap() : GenericMap("asynchronyMap") {}

std::unique_ptr<AsynchronyMap> AsynchronyMap::createAsynchronyMap() {
    return std::make_unique<AsynchronyMap>();
}

std::unique_ptr<AsynchronyMap> AsynchronyMap::createAsynchronyMap(const Element& xml) {
    auto map = std::make_unique<AsynchronyMap>();
    map->parseData(xml);
    return map;
}

void AsynchronyMap::parseData(const Element& xmlElement) {
    GenericMap::parseData(xmlElement);
    
    // Parse asynchrony elements from XML
    for (auto child : xmlElement.children("asynchrony")) {
        std::string dateStr = xml::Helper::getAttributeValue(child, "date");
        std::string offsetStr = xml::Helper::getAttributeValue(child, "milliseconds.offset");
        
        if (!dateStr.empty() && !offsetStr.empty()) {
            double date = std::stod(dateStr);
            double offset = std::stod(offsetStr);
            addAsynchrony(date, offset);
        }
    }
}

int AsynchronyMap::addAsynchrony(double date, double millisecondsOffset) {
    auto data = std::make_unique<AsynchronyData>(date, millisecondsOffset);
    supplementary::KeyValue<double, std::unique_ptr<AsynchronyData>> kv(date, std::move(data));
    
    // Insert in sorted order by date
    auto it = std::lower_bound(asynchronyData.begin(), asynchronyData.end(), kv,
        [](const auto& a, const auto& b) { return a.getKey() < b.getKey(); });
    
    int index = static_cast<int>(it - asynchronyData.begin());
    asynchronyData.insert(it, std::move(kv));
    return index;
}

double AsynchronyMap::getAsynchronyAt(double date) const {
    int i = getElementIndexBeforeAt(date);
    if (i < 0 || i >= static_cast<int>(asynchronyData.size()))
        return 0.0;
    
    return asynchronyData[i].getValue()->millisecondsOffset;
}

int AsynchronyMap::getElementIndexBeforeAt(double date) const {
    if (asynchronyData.empty())
        return -1;
    
    // Find the last element with date <= the given date
    for (int i = static_cast<int>(asynchronyData.size()) - 1; i >= 0; --i) {
        if (asynchronyData[i].getKey() <= date)
            return i;
    }
    return -1;
}

void AsynchronyMap::renderAsynchronyToMap(GenericMap& map) {
    // For this simplified implementation, we'll provide a placeholder
    // The full implementation would require integration with other map types
    // which we'll implement when we have the complete system working
}

void AsynchronyMap::renderAsynchronyToMap(GenericMap& map, AsynchronyMap* asynchronyMap) {
    if (asynchronyMap != nullptr)
        asynchronyMap->renderAsynchronyToMap(map);
}

bool AsynchronyMap::applyToMsmPart(Element msmPart) {
    // AsynchronyMap is typically applied to other maps, not directly to MSM parts
    // But we provide a basic implementation for interface compliance
    return false;
}

} // namespace mpm
} // namespace meico