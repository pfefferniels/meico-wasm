#include "mpm/elements/maps/data/MetricalAccentuationData.h"
#include "xml/Helper.h"

namespace meico {
namespace mpm {

MetricalAccentuationData::MetricalAccentuationData() {
    // Initialize with default values as per Java implementation
}

MetricalAccentuationData::MetricalAccentuationData(const Element& xmlElement) {
    xml = xmlElement;
    startDate = xml::Helper::parseDouble(xmlElement.attribute("date").value());
    accentuationPatternDefName = xmlElement.attribute("name.ref").value();
    scale = xml::Helper::parseDouble(xmlElement.attribute("scale").value());

    auto loopAttr = xmlElement.attribute("loop");
    if (loopAttr) {
        loop = (std::string(loopAttr.value()) == "true");
    }

    auto stickToMeasuresAttr = xmlElement.attribute("stickToMeasures");
    if (stickToMeasuresAttr) {
        stickToMeasures = (std::string(stickToMeasuresAttr.value()) == "true");
    }

    auto id = xmlElement.attribute("xml:id");
    if (id) {
        xmlId = id.value();
    }
}

MetricalAccentuationData MetricalAccentuationData::clone() const {
    MetricalAccentuationData clone;
    clone.xml = xml; // Element is copied
    clone.xmlId = xmlId;
    clone.styleName = styleName;
    clone.style = style;
    clone.startDate = startDate;
    clone.endDate = endDate;
    clone.accentuationPatternDefName = accentuationPatternDefName;
    clone.accentuationPatternDef = accentuationPatternDef;
    clone.scale = scale;
    clone.loop = loop;
    clone.stickToMeasures = stickToMeasures;
    return clone;
}

} // namespace mpm
} // namespace meico