#include "mpm/elements/Part.h"
#include "mpm/elements/Dated.h"
#include "mpm/elements/maps/GenericMap.h"

namespace meico {
namespace mpm {

Part::Part(const std::string& partName, int partNumber, int channel, int port)
    : name(partName), number(partNumber), midiChannel(channel), midiPort(port),
      dated(std::make_unique<Dated>()) {
}

std::unique_ptr<Part> Part::createPart(const std::string& partName, int partNumber, int channel, int port) {
    return std::make_unique<Part>(partName, partNumber, channel, port);
}

const std::string& Part::getName() const {
    return name;
}

int Part::getNumber() const {
    return number;
}

int Part::getMidiChannel() const {
    return midiChannel;
}

int Part::getMidiPort() const {
    return midiPort;
}

Dated* Part::getDated() {
    return dated.get();
}

const Dated* Part::getDated() const {
    return dated.get();
}

void Part::setDated(std::unique_ptr<Dated> newDated) {
    dated = std::move(newDated);
}

void Part::parseData(const Element& xmlElement) {
    setXml(xmlElement);
    // TODO: Parse part data from XML
}

} // namespace mpm
} // namespace meico