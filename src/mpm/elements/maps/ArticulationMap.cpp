#include "mpm/elements/maps/ArticulationMap.h"
#include "mpm/Mpm.h"
#include "xml/Helper.h"
#include <algorithm>

namespace meico {
namespace mpm {

ArticulationMap::ArticulationMap() : GenericMap(Mpm::ARTICULATION_MAP) {
}

std::unique_ptr<ArticulationMap> ArticulationMap::createArticulationMap() {
    return std::make_unique<ArticulationMap>();
}

void ArticulationMap::addArticulation(double date, const std::string& articulationDefName, 
                                    const std::string& noteid, const std::string& id) {
    ArticulationData data;
    data.date = date;
    data.articulationDefName = articulationDefName;
    data.noteid = noteid;
    data.xmlId = id;
    
    articulationData.push_back(data);
    
    // Keep data sorted by date for efficient lookup
    std::sort(articulationData.begin(), articulationData.end(), 
              [](const auto& a, const auto& b) { return a.date < b.date; });
}

void ArticulationMap::addArticulation(double date, double absoluteDuration, double relativeDuration,
                                    double absoluteVelocity, double relativeVelocity,
                                    const std::string& noteid, const std::string& id) {
    ArticulationData data;
    data.date = date;
    data.absoluteDuration = absoluteDuration;
    data.relativeDuration = relativeDuration;
    data.absoluteVelocity = absoluteVelocity;
    data.relativeVelocity = relativeVelocity;
    data.noteid = noteid;
    data.xmlId = id;
    
    articulationData.push_back(data);
    
    // Keep data sorted by date for efficient lookup
    std::sort(articulationData.begin(), articulationData.end(), 
              [](const auto& a, const auto& b) { return a.date < b.date; });
}

void ArticulationMap::addArticulation(const ArticulationData& data) {
    articulationData.push_back(data);
    
    // Keep data sorted by date for efficient lookup
    std::sort(articulationData.begin(), articulationData.end(), 
              [](const auto& a, const auto& b) { return a.date < b.date; });
}

void ArticulationMap::addStyleSwitch(double date, const std::string& styleName, 
                                   const std::string& defaultArticulation, const std::string& id) {
    ArticulationData data;
    data.date = date;
    data.styleName = styleName;
    data.defaultArticulation = defaultArticulation;
    data.xmlId = id;
    
    articulationData.push_back(data);
    
    // Keep data sorted by date for efficient lookup
    std::sort(articulationData.begin(), articulationData.end(), 
              [](const auto& a, const auto& b) { return a.date < b.date; });
}

std::vector<ArticulationData> ArticulationMap::getArticulationDataAt(double date) const {
    std::vector<ArticulationData> result;
    
    // Find all articulations that apply exactly at this date
    for (const auto& data : articulationData) {
        if (std::abs(data.date - date) < 0.001) { // Allow small floating point tolerance
            if (!data.articulationDefName.empty() || data.absoluteVelocity > 0 || data.relativeVelocity != 1.0) {
                result.push_back(data);
            }
        }
    }
    
    return result;
}

bool ArticulationMap::applyToMsmPart(Element msmPart) {
    if (!msmPart || articulationData.empty()) {
        return false;
    }
    
    bool modified = false;
    
    // Find all note elements in the part
    auto scoreElement = xml::Helper::getFirstChildElement(msmPart, "dated");
    if (scoreElement) {
        scoreElement = xml::Helper::getFirstChildElement(scoreElement, "score");
        if (scoreElement) {
            // Process all notes
            for (auto note : scoreElement.children("note")) {
                auto dateAttr = note.attribute("date");
                
                if (dateAttr) {
                    double noteDate = xml::Helper::parseDouble(dateAttr.value());
                    
                    // Check for specific articulations for this note
                    auto noteIdAttr = note.attribute("xml:id");
                    std::string noteId = noteIdAttr ? noteIdAttr.value() : "";
                    
                    // Find articulations that apply to this note
                    bool noteModified = false;
                    for (const auto& data : articulationData) {
                        if (std::abs(data.date - noteDate) < 0.001) { // Same date
                            // Check if this articulation applies to this specific note or all notes
                            if (data.noteid.empty() || data.noteid == noteId) {
                                if (applyArticulationToNote(note, data)) {
                                    noteModified = true;
                                }
                            }
                        }
                    }
                    
                    if (noteModified) {
                        modified = true;
                    }
                }
            }
        }
    }
    
    return modified;
}

void ArticulationMap::parseData(const Element& xmlElement) {
    GenericMap::parseData(xmlElement);
    
    // Parse articulation entries from XML
    for (auto child : xmlElement.children()) {
        if (std::string(child.name()) == "articulation") {
            ArticulationData data;
            
            auto dateAttr = child.attribute("date");
            auto xmlIdAttr = child.attribute("xml:id");
            auto noteidAttr = child.attribute("noteid");
            auto nameRefAttr = child.attribute("name.ref");
            
            if (dateAttr) {
                data.date = xml::Helper::parseDouble(dateAttr.value());
            }
            if (xmlIdAttr) {
                data.xmlId = xmlIdAttr.value();
            }
            if (noteidAttr) {
                data.noteid = noteidAttr.value();
            }
            if (nameRefAttr) {
                data.articulationDefName = nameRefAttr.value();
            }
            
            // Parse articulation parameters
            auto attr = child.attribute("absoluteDuration");
            if (attr) data.absoluteDuration = xml::Helper::parseDouble(attr.value());
            
            attr = child.attribute("absoluteDurationChange");
            if (attr) data.absoluteDurationChange = xml::Helper::parseDouble(attr.value());
            
            attr = child.attribute("relativeDuration");
            if (attr) data.relativeDuration = xml::Helper::parseDouble(attr.value());
            
            attr = child.attribute("absoluteDurationMs");
            if (attr) data.absoluteDurationMs = xml::Helper::parseDouble(attr.value());
            
            attr = child.attribute("absoluteDurationChangeMs");
            if (attr) data.absoluteDurationChangeMs = xml::Helper::parseDouble(attr.value());
            
            attr = child.attribute("absoluteVelocityChange");
            if (attr) data.absoluteVelocityChange = xml::Helper::parseDouble(attr.value());
            
            attr = child.attribute("absoluteVelocity");
            if (attr) data.absoluteVelocity = xml::Helper::parseDouble(attr.value());
            
            attr = child.attribute("relativeVelocity");
            if (attr) data.relativeVelocity = xml::Helper::parseDouble(attr.value());
            
            attr = child.attribute("absoluteDelayMs");
            if (attr) data.absoluteDelayMs = xml::Helper::parseDouble(attr.value());
            
            attr = child.attribute("absoluteDelay");
            if (attr) data.absoluteDelay = xml::Helper::parseDouble(attr.value());
            
            attr = child.attribute("detuneCents");
            if (attr) data.detuneCents = xml::Helper::parseDouble(attr.value());
            
            attr = child.attribute("detuneHz");
            if (attr) data.detuneHz = xml::Helper::parseDouble(attr.value());
            
            data.xml = child;
            articulationData.push_back(data);
            
        } else if (std::string(child.name()) == "style") {
            ArticulationData data;
            
            auto dateAttr = child.attribute("date");
            auto nameRefAttr = child.attribute("name.ref");
            auto defaultArticulationAttr = child.attribute("defaultArticulation");
            auto xmlIdAttr = child.attribute("xml:id");
            
            if (dateAttr) {
                data.date = xml::Helper::parseDouble(dateAttr.value());
            }
            if (nameRefAttr) {
                data.styleName = nameRefAttr.value();
            }
            if (defaultArticulationAttr) {
                data.defaultArticulation = defaultArticulationAttr.value();
            }
            if (xmlIdAttr) {
                data.xmlId = xmlIdAttr.value();
            }
            
            data.xml = child;
            articulationData.push_back(data);
        }
    }
    
    // Sort by date
    std::sort(articulationData.begin(), articulationData.end(), 
              [](const auto& a, const auto& b) { return a.date < b.date; });
}

bool ArticulationMap::applyArticulationToNote(Element note, const ArticulationData& data) const {
    bool modified = false;
    
    // Apply velocity changes
    if (data.absoluteVelocity > 0) {
        auto velocityAttr = note.attribute("velocity");
        if (velocityAttr) {
            velocityAttr.set_value(std::to_string(data.absoluteVelocity).c_str());
        } else {
            note.append_attribute("velocity") = std::to_string(data.absoluteVelocity).c_str();
        }
        modified = true;
    } else if (data.relativeVelocity != 1.0) {
        auto velocityAttr = note.attribute("velocity");
        if (velocityAttr) {
            double currentVelocity = xml::Helper::parseDouble(velocityAttr.value());
            double newVelocity = currentVelocity * data.relativeVelocity;
            newVelocity = std::max(1.0, std::min(127.0, newVelocity)); // Clamp to MIDI range
            velocityAttr.set_value(std::to_string(newVelocity).c_str());
            modified = true;
        }
    } else if (data.absoluteVelocityChange != 0.0) {
        auto velocityAttr = note.attribute("velocity");
        if (velocityAttr) {
            double currentVelocity = xml::Helper::parseDouble(velocityAttr.value());
            double newVelocity = currentVelocity + data.absoluteVelocityChange;
            newVelocity = std::max(1.0, std::min(127.0, newVelocity)); // Clamp to MIDI range
            velocityAttr.set_value(std::to_string(newVelocity).c_str());
            modified = true;
        }
    }
    
    // Apply duration changes
    if (data.absoluteDuration > 0) {
        auto durationAttr = note.attribute("duration");
        if (durationAttr) {
            durationAttr.set_value(std::to_string(data.absoluteDuration).c_str());
        } else {
            note.append_attribute("duration") = std::to_string(data.absoluteDuration).c_str();
        }
        modified = true;
    } else if (data.relativeDuration != 1.0) {
        auto durationAttr = note.attribute("duration");
        if (durationAttr) {
            double currentDuration = xml::Helper::parseDouble(durationAttr.value());
            double newDuration = currentDuration * data.relativeDuration;
            durationAttr.set_value(std::to_string(newDuration).c_str());
            modified = true;
        }
    } else if (data.absoluteDurationChange != 0.0) {
        auto durationAttr = note.attribute("duration");
        if (durationAttr) {
            double currentDuration = xml::Helper::parseDouble(durationAttr.value());
            double newDuration = currentDuration + data.absoluteDurationChange;
            newDuration = std::max(1.0, newDuration); // Ensure positive duration
            durationAttr.set_value(std::to_string(newDuration).c_str());
            modified = true;
        }
    }
    
    return modified;
}

std::string ArticulationMap::findStyleAt(size_t index) const {
    // Find the most recent style switch at or before the given index
    for (int i = static_cast<int>(index); i >= 0; --i) {
        if (!articulationData[i].styleName.empty()) {
            return articulationData[i].styleName;
        }
    }
    return "";
}

} // namespace mpm
} // namespace meico