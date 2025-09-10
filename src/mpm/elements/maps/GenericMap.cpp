#include "mpm/elements/maps/GenericMap.h"

namespace meico {
namespace mpm {

GenericMap::GenericMap(const std::string& type) : mapType(type) {
}

const std::string& GenericMap::getMapType() const {
    return mapType;
}

void GenericMap::parseData(const Element& xmlElement) {
    // Basic parsing implementation
    setXml(xmlElement);
}

} // namespace mpm
} // namespace meico