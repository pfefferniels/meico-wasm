#include "mpm/elements/maps/ArticulationMap.h"
#include "mpm/elements/maps/data/ArticulationData.h"
#include "mpm/Mpm.h"
#include "xml/Helper.h"
#include <algorithm>
#include <map>

namespace meico {
namespace mpm {

ArticulationMap::ArticulationMap() : GenericMap(Mpm::ARTICULATION_MAP) {
}

std::unique_ptr<ArticulationMap> ArticulationMap::createArticulationMap() {
    return std::make_unique<ArticulationMap>();
}

int ArticulationMap::addArticulation(double date, const std::string& articulationDefName, 
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
    
    return static_cast<int>(articulationData.size() - 1);
}

int ArticulationMap::addArticulation(double date, 
                                   std::shared_ptr<double> absoluteDuration,
                                   double absoluteDurationChange,
                                   double relativeDuration,
                                   std::shared_ptr<double> absoluteDurationMs,
                                   double absoluteDurationChangeMs,
                                   double absoluteVelocityChange,
                                   std::shared_ptr<double> absoluteVelocity,
                                   double relativeVelocity,
                                   double absoluteDelayMs,
                                   double absoluteDelay,
                                   double detuneCents,
                                   double detuneHz,
                                   const std::string& noteid,
                                   const std::string& id) {
    ArticulationData data;
    data.date = date;
    data.absoluteDuration = absoluteDuration;
    data.absoluteDurationChange = absoluteDurationChange;
    data.relativeDuration = relativeDuration;
    data.absoluteDurationMs = absoluteDurationMs;
    data.absoluteDurationChangeMs = absoluteDurationChangeMs;
    data.absoluteVelocityChange = absoluteVelocityChange;
    data.absoluteVelocity = absoluteVelocity;
    data.relativeVelocity = relativeVelocity;
    data.absoluteDelayMs = absoluteDelayMs;
    data.absoluteDelay = absoluteDelay;
    data.detuneCents = detuneCents;
    data.detuneHz = detuneHz;
    data.noteid = noteid;
    data.xmlId = id;
    
    articulationData.push_back(data);
    
    // Keep data sorted by date for efficient lookup
    std::sort(articulationData.begin(), articulationData.end(), 
              [](const auto& a, const auto& b) { return a.date < b.date; });
    
    return static_cast<int>(articulationData.size() - 1);
}

int ArticulationMap::addArticulation(const ArticulationData& data) {
    articulationData.push_back(data);
    
    // Keep data sorted by date for efficient lookup
    std::sort(articulationData.begin(), articulationData.end(), 
              [](const auto& a, const auto& b) { return a.date < b.date; });
    
    return static_cast<int>(articulationData.size() - 1);
}

int ArticulationMap::addStyleSwitch(double date, const std::string& styleName, 
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
    
    return static_cast<int>(articulationData.size() - 1);
}

std::shared_ptr<ArticulationData> ArticulationMap::getArticulationDataOf(int index) {
    if (articulationData.empty() || index < 0 || index >= static_cast<int>(articulationData.size())) {
        return nullptr;
    }
    
    // Return a copy as shared_ptr
    return std::make_shared<ArticulationData>(articulationData[index]);
}

std::vector<ArticulationData> ArticulationMap::getArticulationDataAt(double date) {
    std::vector<ArticulationData> ads;
    
    // Find all articulations exactly at this date
    for (int i = static_cast<int>(articulationData.size()) - 1; i >= 0; --i) {
        if (std::abs(articulationData[i].date - date) < 0.001) { // Allow small floating point tolerance
            if (!articulationData[i].articulationDefName.empty() ||
                articulationData[i].absoluteVelocity ||
                articulationData[i].relativeVelocity != 1.0 ||
                articulationData[i].absoluteVelocityChange != 0.0) {
                ads.insert(ads.begin(), articulationData[i]); // Add at front since we're going backwards
            }
        } else if (articulationData[i].date < date) {
            break; // Stop if we've gone past the date
        }
    }
    
    // If no articulation is defined for this particular date, provide style and standard articulation data
    if (ads.empty()) {
        ArticulationData ad;
        ad.date = date;
        // TODO: Find appropriate style when style system is implemented
        // findStyle(index, ad);
        ads.push_back(ad);
    }
    
    return ads;
}

void ArticulationMap::findStyle(int index, ArticulationData& ad) {
    // Get the style that applies to this articulation
    for (int j = index; j >= 0; --j) {
        if (!articulationData[j].styleName.empty()) {
            ad.styleName = articulationData[j].styleName;
            // TODO: Load the actual style when style system is implemented
            // ad.style = getStyle(Mpm::ARTICULATION_STYLE, ad.styleName);
            
            if (!articulationData[j].defaultArticulation.empty()) {
                ad.defaultArticulation = articulationData[j].defaultArticulation;
                // TODO: Load the actual default articulation def when style system is implemented
                // if (ad.style) ad.defaultArticulationDef = ad.style->getDef(ad.defaultArticulation);
            }
            return;
        }
    }
    ad.styleName = "";
}

void ArticulationMap::renderArticulationToMap_noMillisecondModifiers(GenericMap& map) {
    // Make a map using element address as key for notes with specific articulation
    std::map<void*, std::vector<ArticulationData>> noteArtics;
    bool mapTimingChanged = false;
    
    for (size_t articIndex = 0; articIndex < articulationData.size(); ++articIndex) {
        const ArticulationData& ad = articulationData[articIndex];
        
        // Check if this is an actual articulation (not a style switch)
        if (ad.articulationDefName.empty() && !ad.absoluteVelocity && 
            ad.relativeVelocity == 1.0 && ad.absoluteVelocityChange == 0.0) {
            continue; // Skip style switches
        }
        
        if (!ad.noteid.empty()) {
            // This articulation is for a specific note
            // TODO: Find the corresponding note by ID when note lookup is implemented
            // For now, apply to all notes at the same date
        }
        
        // Apply to all map elements at the same date (simplified implementation)
        // TODO: Implement proper note targeting and style resolution
    }
    
    // TODO: Apply default articulation styles
    
    // Correct map order due to timing changes
    if (mapTimingChanged) {
        // TODO: Implement map sorting when GenericMap supports it
    }
}

void ArticulationMap::renderArticulationToMap_millisecondModifiers(GenericMap& map) {
    // TODO: Implement millisecond modifier application
    // This handles attributes like articulation.absoluteDelayMs, etc.
}

bool ArticulationMap::applyToMsmPart(Element msmPart) {
    if (!msmPart || articulationData.empty()) {
        return false;
    }
    
    bool modified = false;
    
    // Find all note elements in the part - try both direct dated and header/dated paths
    Element scoreElement;
    
    // First try: part -> dated -> score (for some MSM structures)
    scoreElement = xml::Helper::getFirstChildElement(msmPart, "dated");
    if (scoreElement) {
        scoreElement = xml::Helper::getFirstChildElement(scoreElement, "score");
    }
    
    // If not found, try: part -> header -> dated -> score (for Bach MSM structure)
    if (!scoreElement) {
        auto headerElement = xml::Helper::getFirstChildElement(msmPart, "header");
        if (headerElement) {
            auto datedElement = xml::Helper::getFirstChildElement(headerElement, "dated");
            if (datedElement) {
                scoreElement = xml::Helper::getFirstChildElement(datedElement, "score");
            }
        }
    }
    
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
    
    return modified;
}

void ArticulationMap::parseData(const Element& xmlElement) {
    GenericMap::parseData(xmlElement);
    
    // Parse articulation entries from XML
    for (auto child : xmlElement.children()) {
        if (std::string(child.name()) == "articulation") {
            ArticulationData data(child);
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

bool ArticulationMap::applyArticulationToNote(Element& note, const ArticulationData& data) const {
    bool modified = false;
    
    // Apply velocity changes
    if (data.absoluteVelocity) {
        auto velocityAttr = note.attribute("velocity");
        if (velocityAttr) {
            velocityAttr.set_value(std::to_string(*data.absoluteVelocity).c_str());
        } else {
            note.append_attribute("velocity") = std::to_string(*data.absoluteVelocity).c_str();
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
    if (data.absoluteDuration) {
        auto durationAttr = note.attribute("duration");
        if (durationAttr) {
            durationAttr.set_value(std::to_string(*data.absoluteDuration).c_str());
        } else {
            note.append_attribute("duration") = std::to_string(*data.absoluteDuration).c_str();
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
    
    // Apply timing changes
    if (data.absoluteDelay != 0.0) {
        auto dateAttr = note.attribute("date");
        if (dateAttr) {
            double currentDate = xml::Helper::parseDouble(dateAttr.value());
            double newDate = currentDate + data.absoluteDelay;
            dateAttr.set_value(std::to_string(newDate).c_str());
            modified = true;
        }
    }
    
    // Apply detuning
    if (data.detuneCents != 0.0) {
        note.append_attribute("detuneCents") = std::to_string(data.detuneCents).c_str();
        modified = true;
    }
    if (data.detuneHz != 0.0) {
        note.append_attribute("detuneHz") = std::to_string(data.detuneHz).c_str();
        modified = true;
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