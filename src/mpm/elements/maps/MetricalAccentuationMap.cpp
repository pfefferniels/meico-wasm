#include "mpm/elements/maps/MetricalAccentuationMap.h"
#include "mpm/Mpm.h"
#include "xml/Helper.h"
#include <algorithm>
#include <cmath>

namespace meico {
namespace mpm {

MetricalAccentuationMap::MetricalAccentuationMap() : GenericMap(Mpm::METRICAL_ACCENTUATION_MAP) {
}

std::unique_ptr<MetricalAccentuationMap> MetricalAccentuationMap::createMetricalAccentuationMap() {
    return std::make_unique<MetricalAccentuationMap>();
}

void MetricalAccentuationMap::addAccentuationPattern(double date, const std::string& accentuationPatternDefName, 
                                                   double scale, bool loop, bool stickToMeasures) {
    MetricalAccentuationData data;
    data.startDate = date;
    data.accentuationPatternDefName = accentuationPatternDefName;
    data.scale = scale;
    data.loop = loop;
    data.stickToMeasures = stickToMeasures;
    
    accentuationData.push_back(data);
    
    // Keep data sorted by date for efficient lookup
    std::sort(accentuationData.begin(), accentuationData.end(), 
              [](const auto& a, const auto& b) { return a.startDate < b.startDate; });
}

const MetricalAccentuationData* MetricalAccentuationMap::getAccentuationDataAt(double date) const {
    if (accentuationData.empty()) {
        return nullptr;
    }
    
    // Find the latest accentuation pattern that applies to this date
    const MetricalAccentuationData* result = nullptr;
    for (const auto& data : accentuationData) {
        if (data.startDate <= date) {
            result = &data;
        } else {
            break;
        }
    }
    
    return result;
}

bool MetricalAccentuationMap::applyToMsmPart(Element msmPart) {
    if (!msmPart || accentuationData.empty()) {
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
                auto velocityAttr = note.attribute("velocity");
                
                if (dateAttr && velocityAttr) {
                    double noteDate = xml::Helper::parseDouble(dateAttr.value());
                    double currentVelocity = xml::Helper::parseDouble(velocityAttr.value());
                    
                    const MetricalAccentuationData* data = getAccentuationDataAt(noteDate);
                    if (data) {
                        // Simplified beat calculation (assumes 4/4 time, PPQ of 480)
                        // In a full implementation, this would use time signature data
                        double ppq = 480.0; // Assume default PPQ
                        double beatsPerMeasure = 4.0;
                        double ticksPerBeat = ppq;
                        double ticksPerMeasure = beatsPerMeasure * ticksPerBeat;
                        
                        double beat;
                        if (data->stickToMeasures) {
                            // Calculate beat position within measure
                            double ticksIntoMeasure = fmod(noteDate, ticksPerMeasure);
                            beat = 1.0 + (ticksIntoMeasure / ticksPerBeat);
                        } else {
                            // Calculate beat position from pattern start
                            double ticksSinceStart = noteDate - data->startDate;
                            beat = 1.0 + (ticksSinceStart / ticksPerBeat);
                        }
                        
                        double accentuation = computeAccentuationAt(beat, *data);
                        double newVelocity = currentVelocity + (accentuation * data->scale);
                        
                        // Clamp velocity to reasonable range
                        newVelocity = std::max(1.0, std::min(127.0, newVelocity));
                        
                        velocityAttr.set_value(std::to_string(newVelocity).c_str());
                        modified = true;
                    }
                }
            }
        }
    }
    
    return modified;
}

void MetricalAccentuationMap::parseData(const Element& xmlElement) {
    GenericMap::parseData(xmlElement);
    
    // Parse accentuation pattern entries from XML
    for (auto child : xmlElement.children()) {
        if (std::string(child.name()) == "accentuationPattern") {
            MetricalAccentuationData data;
            
            auto dateAttr = child.attribute("date");
            auto nameRefAttr = child.attribute("name.ref");
            auto scaleAttr = child.attribute("scale");
            auto loopAttr = child.attribute("loop");
            auto stickToMeasuresAttr = child.attribute("stickToMeasures");
            auto xmlIdAttr = child.attribute("xml:id");
            
            if (dateAttr) {
                data.startDate = xml::Helper::parseDouble(dateAttr.value());
            }
            if (nameRefAttr) {
                data.accentuationPatternDefName = nameRefAttr.value();
            }
            if (scaleAttr) {
                data.scale = xml::Helper::parseDouble(scaleAttr.value());
            }
            if (loopAttr) {
                data.loop = (std::string(loopAttr.value()) == "true");
            }
            if (stickToMeasuresAttr) {
                data.stickToMeasures = (std::string(stickToMeasuresAttr.value()) == "true");
            }
            if (xmlIdAttr) {
                data.xmlId = xmlIdAttr.value();
            }
            
            data.xml = child;
            accentuationData.push_back(data);
        }
    }
    
    // Sort by start date
    std::sort(accentuationData.begin(), accentuationData.end(), 
              [](const auto& a, const auto& b) { return a.startDate < b.startDate; });
}

double MetricalAccentuationMap::computeAccentuationAt(double beat, const MetricalAccentuationData& data) const {
    // Use the basic accentuation pattern based on beat position
    return getBasicAccentuationPattern(beat);
}

double MetricalAccentuationMap::getBasicAccentuationPattern(double beat) const {
    // Simple 4/4 accentuation pattern
    // Beat 1 gets strongest accent, beat 3 gets moderate accent, beats 2 and 4 get weak accents
    double beatInMeasure = fmod(beat - 1.0, 4.0) + 1.0;
    
    if (beatInMeasure >= 1.0 && beatInMeasure < 2.0) {
        return 0.5; // Strong accent on beat 1
    } else if (beatInMeasure >= 3.0 && beatInMeasure < 4.0) {
        return 0.2; // Moderate accent on beat 3
    } else {
        return -0.1; // Weak accents on beats 2 and 4
    }
}

} // namespace mpm
} // namespace meico