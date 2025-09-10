#include "mpm/elements/Global.h"
#include "mpm/elements/Dated.h"
#include "mpm/elements/maps/GenericMap.h"

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
    // TODO: Parse global data from XML
}

} // namespace mpm
} // namespace meico