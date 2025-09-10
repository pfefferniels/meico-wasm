#include "mpm/elements/Dated.h"
#include "mpm/elements/maps/GenericMap.h"

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
    // TODO: Parse maps from XML
}

} // namespace mpm
} // namespace meico