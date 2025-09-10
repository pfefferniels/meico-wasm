#include "mpm/elements/maps/data/RubatoData.h"
#include "xml/Helper.h"

namespace meico {
namespace mpm {

RubatoData::RubatoData() 
    : xml(), xmlId(""), styleName(""), style(nullptr), rubatoDefString(""), rubatoDef(nullptr),
      startDate(0.0), endDate(0.0), frameLength(1.0), intensity(1.0), 
      lateStart(0.0), earlyEnd(1.0), loop(false) {
}

RubatoData::RubatoData(const Element& xml) : RubatoData() {
    this->xml = xml;
    
    // Parse date attribute (required)
    if (xml.attribute("date")) {
        startDate = std::stod(xml.attribute("date").value());
    }
    
    // Parse XML ID
    auto idAttr = xml.attribute("xml:id");
    if (idAttr) {
        xmlId = idAttr.value();
    }
    
    // Parse name.ref attribute for rubatoDef reference
    auto nameRefAttr = xml.attribute("name.ref");
    if (nameRefAttr) {
        rubatoDefString = nameRefAttr.value();
    }
    
    // Parse frameLength attribute
    auto frameLengthAttr = xml.attribute("frameLength");
    if (frameLengthAttr) {
        frameLength = std::stod(frameLengthAttr.value());
    }
    
    // Parse intensity attribute
    auto intensityAttr = xml.attribute("intensity");
    if (intensityAttr) {
        intensity = std::stod(intensityAttr.value());
    }
    
    // Parse lateStart attribute
    auto lateStartAttr = xml.attribute("lateStart");
    if (lateStartAttr) {
        lateStart = std::stod(lateStartAttr.value());
    }
    
    // Parse earlyEnd attribute
    auto earlyEndAttr = xml.attribute("earlyEnd");
    if (earlyEndAttr) {
        earlyEnd = std::stod(earlyEndAttr.value());
    }
    
    // Parse loop attribute
    auto loopAttr = xml.attribute("loop");
    if (loopAttr) {
        std::string loopStr = loopAttr.value();
        loop = (loopStr == "true" || loopStr == "1");
    }
}

RubatoData::RubatoData(const RubatoData& other) 
    : xml(other.xml), xmlId(other.xmlId), styleName(other.styleName), 
      style(other.style), rubatoDefString(other.rubatoDefString), rubatoDef(other.rubatoDef),
      startDate(other.startDate), endDate(other.endDate), frameLength(other.frameLength), 
      intensity(other.intensity), lateStart(other.lateStart), earlyEnd(other.earlyEnd), 
      loop(other.loop) {
}

RubatoData& RubatoData::operator=(const RubatoData& other) {
    if (this != &other) {
        xml = other.xml;
        xmlId = other.xmlId;
        styleName = other.styleName;
        style = other.style;
        rubatoDefString = other.rubatoDefString;
        rubatoDef = other.rubatoDef;
        startDate = other.startDate;
        endDate = other.endDate;
        frameLength = other.frameLength;
        intensity = other.intensity;
        lateStart = other.lateStart;
        earlyEnd = other.earlyEnd;
        loop = other.loop;
    }
    return *this;
}

std::unique_ptr<RubatoData> RubatoData::clone() const {
    return std::make_unique<RubatoData>(*this);
}

}  // namespace mpm
}  // namespace meico