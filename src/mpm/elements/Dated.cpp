#include "mpm/elements/Dated.h"
#include "mpm/elements/maps/GenericMap.h"
#include "mpm/elements/maps/TempoMap.h"
#include "mpm/elements/maps/DynamicsMap.h"
#include "mpm/elements/maps/RubatoMap.h"
#include "mpm/elements/maps/ArticulationMap.h"
#include "mpm/elements/maps/MetricalAccentuationMap.h"
#include "mpm/elements/maps/OrnamentationMap.h"
#include "mpm/elements/maps/MovementMap.h"
#include "mpm/elements/maps/AsynchronyMap.h"
#include "mpm/elements/maps/ImprecisionMap.h"
#include "xml/Helper.h"
#include <iostream>

namespace meico {
namespace mpm {

Dated::Dated() {
}

void Dated::addMap(std::unique_ptr<GenericMap> map) {
    maps.push_back(std::move(map));
}

GenericMap* Dated::getMap(const std::string& mapType) {
    for (auto& map : maps) {
        if (map->getMapType() == mapType) {
            return map.get();
        }
    }
    return nullptr;
}

const GenericMap* Dated::getMap(const std::string& mapType) const {
    for (const auto& map : maps) {
        if (map->getMapType() == mapType) {
            return map.get();
        }
    }
    return nullptr;
}

const std::vector<std::unique_ptr<GenericMap>>& Dated::getAllMaps() const {
    return maps;
}

size_t Dated::getMapCount() const {
    return maps.size();
}

void Dated::parseData(const Element& xmlElement) {
    setXml(xmlElement);
    
    // For now, just check what maps are present and create basic placeholders
    // TODO: Implement full parsing for each map type
    
    if (xml::Helper::getFirstChildElement(xmlElement, "tempoMap")) {
        auto tempoMap = TempoMap::createTempoMap();
        addMap(std::move(tempoMap));
        std::cout << "  Found tempoMap" << std::endl;
    }
    
    if (xml::Helper::getFirstChildElement(xmlElement, "dynamicsMap")) {
        auto dynamicsMap = DynamicsMap::createDynamicsMap();
        addMap(std::move(dynamicsMap));
        std::cout << "  Found dynamicsMap" << std::endl;
    }
    
    if (xml::Helper::getFirstChildElement(xmlElement, "rubatoMap")) {
        auto rubatoMap = RubatoMap::createRubatoMap();
        addMap(std::move(rubatoMap));
        std::cout << "  Found rubatoMap" << std::endl;
    }
    
    if (xml::Helper::getFirstChildElement(xmlElement, "articulationMap")) {
        auto articulationMap = ArticulationMap::createArticulationMap();
        addMap(std::move(articulationMap));
        std::cout << "  Found articulationMap" << std::endl;
    }
    
    if (xml::Helper::getFirstChildElement(xmlElement, "metricalAccentuationMap")) {
        auto accentuationMap = MetricalAccentuationMap::createMetricalAccentuationMap();
        addMap(std::move(accentuationMap));
        std::cout << "  Found metricalAccentuationMap" << std::endl;
    }
    
    if (xml::Helper::getFirstChildElement(xmlElement, "ornamentationMap")) {
        auto ornamentationMap = OrnamentationMap::createOrnamentationMap();
        addMap(std::move(ornamentationMap));
        std::cout << "  Found ornamentationMap" << std::endl;
    }
    
    if (xml::Helper::getFirstChildElement(xmlElement, "movementMap")) {
        auto movementMap = MovementMap::createMovementMap();
        addMap(std::move(movementMap));
        std::cout << "  Found movementMap" << std::endl;
    }
    
    if (xml::Helper::getFirstChildElement(xmlElement, "asynchronyMap")) {
        auto asynchronyMap = AsynchronyMap::createAsynchronyMap();
        addMap(std::move(asynchronyMap));
        std::cout << "  Found asynchronyMap" << std::endl;
    }
    
    if (xml::Helper::getFirstChildElement(xmlElement, "imprecisionMap.timing")) {
        auto imprecisionMap = ImprecisionMap::createImprecisionMap("timing");
        addMap(std::move(imprecisionMap));
        std::cout << "  Found imprecisionMap.timing" << std::endl;
    }
    
    if (xml::Helper::getFirstChildElement(xmlElement, "imprecisionMap.dynamics")) {
        auto imprecisionMap = ImprecisionMap::createImprecisionMap("dynamics");
        addMap(std::move(imprecisionMap));
        std::cout << "  Found imprecisionMap.dynamics" << std::endl;
    }
}

} // namespace mpm
} // namespace meico