#include "mpm/elements/maps/data/TempoData.h"
#include "xml/Helper.h"
#include <limits>

namespace meico {
namespace mpm {

TempoData::TempoData() {
    startDate = 0.0;
    startDateMilliseconds = 0.0;
    endDate = std::numeric_limits<double>::max();
    bpm = 0.0;
    transitionTo = 0.0;
    beatLength = 0.25;
    meanTempoAt = 0.0;
    exponent = 0.0;
}

TempoData::TempoData(const Element& xml) : TempoData() {
    this->xml = xml;
    
    // Parse start date
    std::string dateStr = xml::Helper::getAttributeValue(xml, "date");
    if (!dateStr.empty()) {
        startDate = std::stod(dateStr);
    }
    
    // Parse beat length
    std::string beatLengthStr = xml::Helper::getAttributeValue(xml, "beatLength");
    if (!beatLengthStr.empty()) {
        beatLength = std::stod(beatLengthStr);
    }
    
    // Parse BPM - can be numeric or string
    std::string bpmStr = xml::Helper::getAttributeValue(xml, "bpm");
    if (!bpmStr.empty()) {
        try {
            bpm = std::stod(bpmStr);
        } catch (const std::invalid_argument&) {
            bpmString = bpmStr;  // Store as string if not numeric
        }
    }
    
    // Parse transition.to - can be numeric or string
    std::string transitionStr = xml::Helper::getAttributeValue(xml, "transition.to");
    if (!transitionStr.empty()) {
        try {
            transitionTo = std::stod(transitionStr);
        } catch (const std::invalid_argument&) {
            transitionToString = transitionStr;  // Store as string if not numeric
        }
    }
    
    // Parse meanTempoAt
    std::string meanTempoAtStr = xml::Helper::getAttributeValue(xml, "meanTempoAt");
    if (!meanTempoAtStr.empty()) {
        meanTempoAt = std::stod(meanTempoAtStr);
    }
    
    // Parse XML ID
    xmlId = xml::Helper::getAttributeValue(xml, "id", "http://www.w3.org/XML/1998/namespace");
}

std::unique_ptr<TempoData> TempoData::clone() const {
    auto clone = std::make_unique<TempoData>();
    
    clone->xml = xml;  // Element should be copyable
    clone->xmlId = xmlId;
    clone->styleName = styleName;
    clone->style = style;
    clone->startDate = startDate;
    clone->startDateMilliseconds = startDateMilliseconds;
    clone->endDate = endDate;
    clone->bpmString = bpmString;
    clone->bpm = bpm;
    clone->transitionToString = transitionToString;
    clone->transitionTo = transitionTo;
    clone->beatLength = beatLength;
    clone->meanTempoAt = meanTempoAt;
    clone->exponent = exponent;
    
    return clone;
}

bool TempoData::isConstantTempo() const {
    // True if no transition target is defined
    if (transitionToString.empty() && transitionTo == 0.0) {
        return true;
    }
    
    // True if transition target equals start tempo
    if (transitionTo != 0.0 && transitionTo == bpm) {
        return true;
    }
    
    // True if both string values are defined and equal
    if (!bpmString.empty() && !transitionToString.empty() && transitionToString == bpmString) {
        return true;
    }
    
    return false;
}

} // namespace mpm
} // namespace meico