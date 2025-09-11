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
#include "mpm/elements/Global.h"
#include "mpm/elements/Part.h"
#include "mpm/Mpm.h"
#include "xml/Helper.h"
#include <iostream>
#include <stdexcept>

namespace meico {
namespace mpm {

Dated::Dated() {
}

GenericMap* Dated::addMap(std::unique_ptr<GenericMap> map) {
    if (!map) {
        return nullptr;
    }
    
    std::string type = map->getMapType();
    
    // If there is already such a map, delete it (like Java)
    if (maps.find(type) != maps.end()) {
        removeMap(type);
    }
    
    // TODO: Set headers like Java does
    // Header globalHeader = (this.global == null) ? null : this.global.getHeader();
    // Header localHeader = (this.part == null) ? null : this.part.getHeader();
    // map.setHeaders(globalHeader, localHeader);
    
    GenericMap* result = map.get();
    maps[type] = std::move(map);
    
    // TODO: Add XML element to dated XML like Java does
    // Element parent = (Element) map.getXml().getParent();
    // if (parent == null)
    //     this.getXml().appendChild(map.getXml());
    
    return result;
}

GenericMap* Dated::addMap(const Element& xml) {
    if (!xml) {
        return nullptr;
    }
    
    std::string type = xml.name();
    
    // Create the appropriate map type following Java's switch logic exactly
    std::unique_ptr<GenericMap> map;
    if (type == Mpm::DYNAMICS_MAP) {
        // map = DynamicsMap::createDynamicsMap(xml);  // TODO: Add Element factory
        map = DynamicsMap::createDynamicsMap();
    } else if (type == Mpm::MOVEMENT_MAP) {
        // map = MovementMap::createMovementMap(xml);  // TODO: Add Element factory
        map = MovementMap::createMovementMap();
    } else if (type == Mpm::METRICAL_ACCENTUATION_MAP) {
        // map = MetricalAccentuationMap::createMetricalAccentuationMap(xml);  // TODO: Add Element factory
        map = MetricalAccentuationMap::createMetricalAccentuationMap();
    } else if (type == Mpm::TEMPO_MAP) {
        // map = TempoMap::createTempoMap(xml);  // TODO: Add Element factory
        map = TempoMap::createTempoMap();
    } else if (type == Mpm::RUBATO_MAP) {
        // map = RubatoMap::createRubatoMap(xml);  // TODO: Add Element factory
        map = RubatoMap::createRubatoMap();
    } else if (type == Mpm::ASYNCHRONY_MAP) {
        // map = AsynchronyMap::createAsynchronyMap(xml);  // TODO: Add Element factory
        map = AsynchronyMap::createAsynchronyMap();
    } else if (type == Mpm::ARTICULATION_MAP) {
        // map = ArticulationMap::createArticulationMap(xml);  // TODO: Add Element factory
        map = ArticulationMap::createArticulationMap();
    } else if (type == Mpm::IMPRECISION_MAP ||
               type == Mpm::IMPRECISION_MAP_TIMING ||
               type == Mpm::IMPRECISION_MAP_DYNAMICS ||
               type == Mpm::IMPRECISION_MAP_TONEDURATION ||
               type == Mpm::IMPRECISION_MAP_TUNING) {
        // map = ImprecisionMap::createImprecisionMap(xml);  // TODO: Add Element factory
        map = ImprecisionMap::createImprecisionMap("");
    } else if (type == Mpm::ORNAMENTATION_MAP) {
        // map = OrnamentationMap::createOrnamentationMap(xml);  // TODO: Add Element factory
        map = OrnamentationMap::createOrnamentationMap();
    } else {
        // map = GenericMap::createGenericMap(type);  // TODO: Add factory method
        map = nullptr;  // For now, unsupported map types return null
    }
    
    return addMap(std::move(map));
}

GenericMap* Dated::addMap(const std::string& type) {
    if (type.empty()) {
        return nullptr;
    }
    
    // Create the appropriate map type following Java's switch logic exactly
    std::unique_ptr<GenericMap> map;
    if (type == Mpm::DYNAMICS_MAP) {
        map = DynamicsMap::createDynamicsMap();
    } else if (type == Mpm::METRICAL_ACCENTUATION_MAP) {
        map = MetricalAccentuationMap::createMetricalAccentuationMap();
    } else if (type == Mpm::TEMPO_MAP) {
        map = TempoMap::createTempoMap();
    } else if (type == Mpm::RUBATO_MAP) {
        map = RubatoMap::createRubatoMap();
    } else if (type == Mpm::ASYNCHRONY_MAP) {
        map = AsynchronyMap::createAsynchronyMap();
    } else if (type == Mpm::ARTICULATION_MAP) {
        map = ArticulationMap::createArticulationMap();
    } else if (type == Mpm::IMPRECISION_MAP) {
        map = ImprecisionMap::createImprecisionMap("");
    } else if (type == Mpm::IMPRECISION_MAP_TIMING) {
        map = ImprecisionMap::createImprecisionMap("timing");
    } else if (type == Mpm::IMPRECISION_MAP_DYNAMICS) {
        map = ImprecisionMap::createImprecisionMap("dynamics");
    } else if (type == Mpm::IMPRECISION_MAP_TONEDURATION) {
        map = ImprecisionMap::createImprecisionMap("toneduration");
    } else if (type == Mpm::IMPRECISION_MAP_TUNING) {
        map = ImprecisionMap::createImprecisionMap("tuning");
    } else if (type == Mpm::ORNAMENTATION_MAP) {
        map = OrnamentationMap::createOrnamentationMap();
    } else {
        // map = GenericMap::createGenericMap(type);  // TODO: Add factory method
        map = nullptr;  // For now, unsupported map types return null
    }
    
    return addMap(std::move(map));
}

void Dated::removeMap(const std::string& type) {
    auto it = maps.find(type);
    if (it != maps.end()) {
        // TODO: Remove XML like Java does
        // this.getXml().removeChild(m.getXml());
        maps.erase(it);
    }
}

void Dated::clear() {
    // TODO: Remove XML children like Java does
    // this.getXml().removeChildren();
    maps.clear();
}

GenericMap* Dated::getMap(const std::string& mapType) {
    auto it = maps.find(mapType);
    return (it != maps.end()) ? it->second.get() : nullptr;
}

const GenericMap* Dated::getMap(const std::string& mapType) const {
    auto it = maps.find(mapType);
    return (it != maps.end()) ? it->second.get() : nullptr;
}

std::vector<std::unique_ptr<GenericMap>> Dated::getAllMaps() const {
    // For compatibility, convert to vector (but this breaks ownership)
    // This is not ideal, but needed for the current interface
    std::vector<std::unique_ptr<GenericMap>> result;
    // We can't return the actual pointers due to ownership, so we return empty for now
    // TODO: Change interface to not return unique_ptr vector
    return result;
}

const std::unordered_map<std::string, std::unique_ptr<GenericMap>>& Dated::getAllMapsMap() const {
    return maps;
}

size_t Dated::getMapCount() const {
    return maps.size();
}

void Dated::setEnvironment(Global* globalEnv, Part* partEnv) {
    global = globalEnv;
    part = partEnv;
    
    // TODO: Update the links for all maps like Java does
    // Header globalHeader = (this.global == null) ? null : this.global.getHeader();
    // Header localHeader = (this.part == null) ? null : this.part.getHeader();
    // for (GenericMap map : this.maps.values())
    //     map.setHeaders(globalHeader, localHeader);
}

Global* Dated::getGlobal() {
    return global;
}

const Global* Dated::getGlobal() const {
    return global;
}

Part* Dated::getPart() {
    return part;
}

const Part* Dated::getPart() const {
    return part;
}

void Dated::parseData(const Element& xmlElement) {
    if (!xmlElement) {
        throw std::runtime_error("Cannot generate Dated object. XML Element is null.");
    }
    
    setXml(xmlElement);
    
    // Make sure that this element is really a "dated" element (like Java)
    if (std::string(xmlElement.name()) != "dated") {
        std::cerr << "Warning: Element is not named 'dated', but '" << xmlElement.name() << "'" << std::endl;
    }
    
    // Parse the maps following Java's logic exactly
    // Java uses: Nodes maps = this.getXml().query("descendant::*[contains(local-name(), 'Map') or local-name()='score']");
    // We need to find all descendant elements that contain "Map" in their name or are named "score"
    
    std::function<void(const Element&)> findMaps = [&](const Element& element) {
        std::string name = element.name();
        
        // Check if this element should be processed as a map
        if (name.find("Map") != std::string::npos || name == "score") {
            std::cout << "  Found map element: " << name << std::endl;
            addMap(element);
        }
        
        // Recursively check children
        for (auto child : element.children()) {
            findMaps(child);
        }
    };
    
    // Start the recursive search
    findMaps(xmlElement);
}

} // namespace mpm
} // namespace meico