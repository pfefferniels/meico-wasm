#include "mpm/elements/Global.h"
#include "mpm/elements/Dated.h"
#include "mpm/elements/maps/GenericMap.h"
#include "xml/Helper.h"

namespace meico {
namespace mpm {

Global::Global() : dated(std::make_unique<Dated>()) {
}

Dated* Global::getDated() {
    return dated.get();
}

const Dated* Global::getDated() const {
    return dated.get();
}

void Global::setDated(std::unique_ptr<Dated> newDated) {
    dated = std::move(newDated);
}

void Global::parseData(const Element& xmlElement) {
    setXml(xmlElement);
    
    // Parse the dated element
    Element datedElement = xml::Helper::getFirstChildElement(xmlElement, "dated");
    if (datedElement && dated) {
        dated->parseData(datedElement);
    }
}

} // namespace mpm
} // namespace meico