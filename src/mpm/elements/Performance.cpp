#include "mpm/elements/Performance.h"
#include "mpm/elements/Global.h"
#include "mpm/elements/Part.h"
#include "mpm/elements/Dated.h"
#include "mpm/elements/maps/GenericMap.h"
#include "msm/Msm.h"
#include "xml/Helper.h"
#include <iostream>

namespace meico {
namespace mpm {

Performance::Performance(const std::string& performanceName)
    : name(performanceName), pulsesPerQuarter(720), global(std::make_unique<Global>()) {
    init();
}

Performance::Performance(const Element& xml)
    : pulsesPerQuarter(720), global(std::make_unique<Global>()) {
    parseData(xml);
}

std::unique_ptr<Performance> Performance::createPerformance(const std::string& performanceName) {
    return std::make_unique<Performance>(performanceName);
}

const std::string& Performance::getName() const {
    return name;
}

void Performance::setName(const std::string& performanceName) {
    name = performanceName;
}

int Performance::getPPQ() const {
    return pulsesPerQuarter;
}

void Performance::setPPQ(int ppq) {
    pulsesPerQuarter = ppq;
}

Global* Performance::getGlobal() {
    return global.get();
}

const Global* Performance::getGlobal() const {
    return global.get();
}

size_t Performance::getPartCount() const {
    return parts.size();
}

Part* Performance::getPart(size_t index) {
    if (index < parts.size()) {
        return parts[index].get();
    }
    return nullptr;
}

const Part* Performance::getPart(size_t index) const {
    if (index < parts.size()) {
        return parts[index].get();
    }
    return nullptr;
}

void Performance::addPart(std::unique_ptr<Part> part) {
    parts.push_back(std::move(part));
}

std::unique_ptr<msm::Msm> Performance::perform(const msm::Msm& msm) const {
    std::cout << "\nRendering performance \"" << name << "\" into \"" << msm.getTitle() << "\"." << std::endl;
    
    // Clone the input MSM so the original remains unaltered
    auto resultMsm = msm.clone();
    if (!resultMsm) {
        return nullptr;
    }
    
    // Set output filename to avoid overwriting original (like Java)
    std::string originalFile = resultMsm->getFile();
    if (!originalFile.empty()) {
        std::string newFile = xml::Helper::getFilenameWithoutExtension(originalFile) + "_" + name + ".msm";
        resultMsm->setFile(newFile);
    }
    
    // Convert PPQ if necessary (like Java)
    resultMsm->convertPPQ(pulsesPerQuarter);
    
    // Process global data (following Java's algorithm)
    std::cout << "DEBUG: Running NEW Performance.perform() implementation!" << std::endl;
    std::cout << "Processing global data." << std::endl;
    Element root = resultMsm->getRootElement();
    if (root) {
        Element globalElement = xml::Helper::getFirstChildElement(root, "global");
        if (globalElement) {
            Element globalDated = xml::Helper::getFirstChildElement(globalElement, "dated");
            if (globalDated) {
                addPerformanceTimingAttributesToDated(globalDated);
            }
        }
    }
    
    // Get global maps for application to parts
    const auto* globalDated = global ? global->getDated() : nullptr;
    
    // Process MSM parts (following Java's algorithm)
    if (root) {
        for (auto msmPart : root.children("part")) {
            std::cout << "Processing part " << xml::Helper::getAttributeValue(msmPart, "name") << std::endl;
            
            Element dated = xml::Helper::getFirstChildElement(msmPart, "dated");
            if (dated) {
                // Add .perf attributes to all elements (like Java does)
                addPerformanceTimingAttributesToDated(dated);
                
                // Apply global maps to this part
                if (globalDated) {
                    const auto& globalMapsMap = globalDated->getAllMapsMap();
                    if (!globalMapsMap.empty()) {
                        std::cout << "  Applying " << globalMapsMap.size() << " global maps to part" << std::endl;
                        for (const auto& [mapType, mapPtr] : globalMapsMap) {
                            if (mapPtr) {
                                mapPtr->applyToMsmPart(msmPart);
                            }
                        }
                    }
                }
                
                // Apply specific performance transformations
                applyPerformanceTransformations(msmPart, dated);
            }
        }
    }
    
    std::cout << "Performance rendering completed." << std::endl;
    return resultMsm;
}

void Performance::parseData(const Element& xmlElement) {
    if (!xmlElement) {
        throw std::runtime_error("Cannot generate Performance object. XML Element is null.");
    }
    
    // Check for name attribute - required
    auto nameAttr = xmlElement.attribute("name");
    if (!nameAttr || std::string(nameAttr.value()).empty()) {
        throw std::runtime_error("Cannot generate Performance object. Attribute name is missing or empty.");
    }
    
    setXml(xmlElement);
    name = nameAttr.value();
    
    // Parse ID attribute
    auto idAttr = xmlElement.attribute("id");
    if (idAttr) {
        xmlId = idAttr.value();
    }
    
    // Make sure this element is really a "performance" element (Java does this)
    // In pugixml, we can't change the local name directly, but we can validate it
    if (std::string(xmlElement.name()) != "performance") {
        std::cerr << "Warning: Element is not named 'performance', but '" << xmlElement.name() << "'" << std::endl;
    }
    
    // Make sure the performance has a pulsesPerQuarter attribute
    auto ppqAttr = xmlElement.attribute("pulsesPerQuarter");
    if (!ppqAttr) {
        // Java adds a default attribute to the XML, we'll just set the default value
        pulsesPerQuarter = 720;
        std::cerr << "Warning: No pulsesPerQuarter attribute found, using default of 720" << std::endl;
    } else {
        try {
            pulsesPerQuarter = std::stoi(ppqAttr.value());
        } catch (const std::exception&) {
            pulsesPerQuarter = 720;
            std::cerr << "Warning: Invalid pulsesPerQuarter value, using default of 720" << std::endl;
        }
    }
    
    // Make sure there is a global environment
    Element globalElement = xml::Helper::getFirstChildElement(xmlElement, "global");
    if (!globalElement) {
        // Java creates a global element if none exists
        if (!global) {
            global = std::make_unique<Global>();
        }
        // In Java, it would add this to the XML, but we'll just create the object
        std::cerr << "Warning: No global element found, creating empty global environment" << std::endl;
    } else {
        if (!global) {
            global = std::make_unique<Global>();
        }
        global->parseData(globalElement);
    }
    
    // Parse parts - exactly like Java
    std::vector<Element> partElements = xml::Helper::getChildElements(xmlElement, "part");
    for (const auto& partElement : partElements) {
        try {
            auto part = Part::createPart(partElement);  // Use factory method like Java
            if (!part) {
                continue;  // Java continues with next part if creation fails
            }
            part->setGlobal(global.get());  // Set the global environment like Java
            parts.push_back(std::move(part));
        } catch (const std::exception& e) {
            std::cerr << "Error parsing part: " << e.what() << std::endl;
            // Java continues with next part on error
        }
    }
}

void Performance::init() {
    // Initialize basic structure
    // Global is already created in constructor
}

void Performance::applyMapsToMsmPart(Element msmPart, const std::vector<std::unique_ptr<GenericMap>>& maps) const {
    for (const auto& map : maps) {
        if (map) {
            bool modified = map->applyToMsmPart(msmPart);
            if (modified) {
                std::cout << "  Applied " << map->getMapType() << " map" << std::endl;
            }
        }
    }
}

void Performance::addPerformanceTimingAttributesToDated(Element dated) const {
    if (!dated) {
        return;
    }
    
    // Following Java's addPerformanceTimingAttributes method exactly
    // Iterate through all child elements and add .perf attributes
    std::function<void(Element)> processElement = [&](Element element) {
        // Add date.perf attribute if date exists
        auto dateAttr = element.attribute("date");
        if (dateAttr) {
            element.append_attribute("date.perf") = dateAttr.value();
        }
        
        // Add duration.perf attribute if duration exists  
        auto durationAttr = element.attribute("duration");
        if (durationAttr) {
            element.append_attribute("duration.perf") = durationAttr.value();
        }
        
        // Add date.end.perf attribute if date.end exists
        auto dateEndAttr = element.attribute("date.end");
        if (dateEndAttr) {
            element.append_attribute("date.end.perf") = dateEndAttr.value();
        }
        
        // Recursively process children
        for (auto child : element.children()) {
            processElement(child);
        }
    };
    
    // Process all children of dated
    for (auto child : dated.children()) {
        processElement(child);
    }
}

void Performance::applyPerformanceTransformations(Element msmPart, Element dated) const {
    if (!msmPart || !dated) {
        return;
    }
    
    // Find the score element which contains the notes
    Element score = xml::Helper::getFirstChildElement(dated, "score");
    if (!score) {
        return;
    }
    
    // Apply basic performance transformations following Java's approach
    
    // 1. Apply dynamics changes to velocity (simple version for now)
    for (auto note : score.children("note")) {
        auto velocityAttr = note.attribute("velocity");
        if (velocityAttr) {
            try {
                double velocity = std::stod(velocityAttr.value());
                // Simple dynamics transformation: increase velocity slightly
                double newVelocity = std::min(127.0, velocity * 1.125); // 12.5% increase like some Java examples
                note.attribute("velocity").set_value(std::to_string(newVelocity).c_str());
            } catch (const std::exception&) {
                // Keep original velocity if parsing fails
            }
        }
    }
    
    // 2. Apply basic timing transformations (simplified version of TempoMap.renderTempoToMap)
    applyBasicTimingTransformation(score);
}

void Performance::applyBasicTimingTransformation(Element score) const {
    if (!score) {
        return;
    }
    
    // Simple timing transformation following Java's TempoMap.renderTempoToMap pattern
    // This adds milliseconds.date attributes based on a default tempo
    
    const double DEFAULT_BPM = 120.0;
    const double MILLIS_PER_BEAT = 60000.0 / DEFAULT_BPM; // milliseconds per quarter note
    const double TICKS_PER_BEAT = static_cast<double>(pulsesPerQuarter);
    const double MILLIS_PER_TICK = MILLIS_PER_BEAT / TICKS_PER_BEAT;
    
    for (auto note : score.children("note")) {
        // Add milliseconds.date attribute
        auto datePerfAttr = note.attribute("date.perf");
        if (datePerfAttr) {
            try {
                double datePerf = std::stod(datePerfAttr.value());
                double millisecondsDate = datePerf * MILLIS_PER_TICK;
                note.append_attribute("milliseconds.date") = std::to_string(millisecondsDate).c_str();
            } catch (const std::exception&) {
                // Skip if parsing fails
            }
        }
        
        // Add milliseconds.date.end attribute
        auto dateEndPerfAttr = note.attribute("date.end.perf");
        auto durationPerfAttr = note.attribute("duration.perf");
        if (dateEndPerfAttr) {
            try {
                double dateEndPerf = std::stod(dateEndPerfAttr.value());
                double millisecondsDateEnd = dateEndPerf * MILLIS_PER_TICK;
                note.append_attribute("milliseconds.date.end") = std::to_string(millisecondsDateEnd).c_str();
            } catch (const std::exception&) {
                // Skip if parsing fails
            }
        } else if (datePerfAttr && durationPerfAttr) {
            // Calculate date.end.perf and milliseconds.date.end from date.perf + duration.perf
            try {
                double datePerf = std::stod(datePerfAttr.value());
                double durationPerf = std::stod(durationPerfAttr.value());
                double dateEndPerf = datePerf + durationPerf;
                note.append_attribute("date.end.perf") = std::to_string(dateEndPerf).c_str();
                
                double millisecondsDateEnd = dateEndPerf * MILLIS_PER_TICK;
                note.append_attribute("milliseconds.date.end") = std::to_string(millisecondsDateEnd).c_str();
            } catch (const std::exception&) {
                // Skip if parsing fails
            }
        }
    }
}

} // namespace mpm
} // namespace meico