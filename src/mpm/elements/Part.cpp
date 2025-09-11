#include "mpm/elements/Part.h"
#include "mpm/elements/Dated.h"
#include "mpm/elements/Global.h"
#include "mpm/elements/maps/GenericMap.h"  // Include for complete type
// #include "mpm/elements/Header.h"  // Commented out for now
#include "xml/Helper.h"
#include <stdexcept>
#include <iostream>

namespace meico {
namespace mpm {

Part::Part(const Element& xml) : dated(std::make_unique<Dated>()) {
    parseData(xml);
}

std::unique_ptr<Part> Part::createPart(const Element& xml) {
    try {
        return std::make_unique<Part>(xml);
    } catch (const std::exception& e) {
        std::cerr << "Error creating Part: " << e.what() << std::endl;
        return nullptr;
    }
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

/*
Header* Part::getHeader() {
    // return header.get();
    return nullptr;  // Commented out for now
}

const Header* Part::getHeader() const {
    // return header.get();
    return nullptr;  // Commented out for now
}
*/

void Part::setGlobal(Global* globalEnv) {
    global = globalEnv;
    // Link the global and local environment in dated (like Java)
    if (dated) {
        dated->setEnvironment(global, this);
    }
}

Global* Part::getGlobal() {
    return global;
}

const Global* Part::getGlobal() const {
    return global;
}

void Part::parseData(const Element& xmlElement) {
    if (!xmlElement) {
        throw std::runtime_error("Cannot generate Part object. XML Element is null.");
    }
    
    // Parse name attribute (can be empty in Java)
    auto nameAttr = xmlElement.attribute("name");
    if (!nameAttr) {
        name = "";  // Java generates an empty name attribute if missing
        std::cerr << "Warning: Part has no name attribute, using empty string" << std::endl;
    } else {
        name = nameAttr.value();
    }
    
    // Parse number attribute - required
    auto numberAttr = xmlElement.attribute("number");
    if (!numberAttr || std::string(numberAttr.value()).empty()) {
        throw std::runtime_error("Cannot generate Part object. Attribute number is missing or empty.");
    }
    
    // Parse midi.channel attribute - required
    auto midiChannelAttr = xmlElement.attribute("midi.channel");
    if (!midiChannelAttr || std::string(midiChannelAttr.value()).empty()) {
        throw std::runtime_error("Cannot generate Part object. Attribute midi.channel is missing or empty.");
    }
    
    // Parse midi.port attribute - required
    auto midiPortAttr = xmlElement.attribute("midi.port");
    if (!midiPortAttr || std::string(midiPortAttr.value()).empty()) {
        throw std::runtime_error("Cannot generate Part object. Attribute midi.port is missing or empty.");
    }
    
    setXml(xmlElement);
    
    try {
        number = std::stoi(numberAttr.value());
        midiChannel = std::stoi(midiChannelAttr.value());
        midiPort = std::stoi(midiPortAttr.value());
    } catch (const std::exception& e) {
        throw std::runtime_error("Cannot parse Part attributes: invalid number format");
    }
    
    // Parse ID attribute
    auto idAttr = xmlElement.attribute("id");
    if (idAttr) {
        xmlId = idAttr.value();
    }
    
    // Make sure that this element is really a "part" element (Java does this)
    if (std::string(xmlElement.name()) != "part") {
        std::cerr << "Warning: Element is not named 'part', but '" << xmlElement.name() << "'" << std::endl;
    }
    
    // Make sure there is a header environment (Java does this but we'll skip for now since Header is not implemented)
    Element headerElement = xml::Helper::getFirstChildElement(xmlElement, "header");
    if (!headerElement) {
        // Java would create a header here, but we'll skip for now
        std::cerr << "Info: No header element found in part" << std::endl;
    }
    
    // Make sure there is a dated environment
    Element datedElement = xml::Helper::getFirstChildElement(xmlElement, "dated");
    if (!datedElement) {
        // Java creates a dated element if none exists
        if (!dated) {
            dated = std::make_unique<Dated>();
        }
        std::cerr << "Warning: No dated element found in part, creating empty dated environment" << std::endl;
    } else {
        if (!dated) {
            dated = std::make_unique<Dated>();
        }
        dated->parseData(datedElement);
    }
    
    if (!dated) {
        throw std::runtime_error("Cannot generate Part object. Failed to generate Dated object.");
    }
    
    // Link the global and local environment (done later when setGlobal is called)
}

} // namespace mpm
} // namespace meico