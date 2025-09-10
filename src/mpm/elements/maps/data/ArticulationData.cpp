#include "mpm/elements/maps/data/ArticulationData.h"
#include "xml/Helper.h"
#include <algorithm>

namespace meico {
namespace mpm {

ArticulationData::ArticulationData() {
    // Initialize with default values as per Java implementation
}

ArticulationData::ArticulationData(const Element& xmlElement) {
    xml = xmlElement;
    date = xml::Helper::parseDouble(xmlElement.attribute("date").value());

    auto nameRef = xmlElement.attribute("name.ref");
    if (nameRef) {
        articulationDefName = nameRef.value();
    }

    auto noteId = xmlElement.attribute("noteid");
    if (noteId) {
        noteid = noteId.value();
    }

    auto absoluteDurationAttr = xmlElement.attribute("absoluteDuration");
    if (absoluteDurationAttr) {
        absoluteDuration = std::make_shared<double>(xml::Helper::parseDouble(absoluteDurationAttr.value()));
    }

    auto absoluteDurationChangeAttr = xmlElement.attribute("absoluteDurationChange");
    if (absoluteDurationChangeAttr) {
        absoluteDurationChange = xml::Helper::parseDouble(absoluteDurationChangeAttr.value());
    }

    auto absoluteDurationMsAttr = xmlElement.attribute("absoluteDurationMs");
    if (absoluteDurationMsAttr) {
        absoluteDurationMs = std::make_shared<double>(xml::Helper::parseDouble(absoluteDurationMsAttr.value()));
    }

    auto absoluteDurationChangeMsAttr = xmlElement.attribute("absoluteDurationChangeMs");
    if (absoluteDurationChangeMsAttr) {
        absoluteDurationChangeMs = xml::Helper::parseDouble(absoluteDurationChangeMsAttr.value());
    }

    auto relativeDurationAttr = xmlElement.attribute("relativeDuration");
    if (relativeDurationAttr) {
        relativeDuration = xml::Helper::parseDouble(relativeDurationAttr.value());
    }

    auto absoluteDelayAttr = xmlElement.attribute("absoluteDelay");
    if (absoluteDelayAttr) {
        absoluteDelay = xml::Helper::parseDouble(absoluteDelayAttr.value());
    }

    auto absoluteDelayMsAttr = xmlElement.attribute("absoluteDelayMs");
    if (absoluteDelayMsAttr) {
        absoluteDelayMs = xml::Helper::parseDouble(absoluteDelayMsAttr.value());
    }

    auto absoluteVelocityAttr = xmlElement.attribute("absoluteVelocity");
    if (absoluteVelocityAttr) {
        absoluteVelocity = std::make_shared<double>(xml::Helper::parseDouble(absoluteVelocityAttr.value()));
    }

    auto absoluteVelocityChangeAttr = xmlElement.attribute("absoluteVelocityChange");
    if (absoluteVelocityChangeAttr) {
        absoluteVelocityChange = xml::Helper::parseDouble(absoluteVelocityChangeAttr.value());
    }

    auto relativeVelocityAttr = xmlElement.attribute("relativeVelocity");
    if (relativeVelocityAttr) {
        relativeVelocity = xml::Helper::parseDouble(relativeVelocityAttr.value());
    }

    auto detuneCentsAttr = xmlElement.attribute("detuneCents");
    if (detuneCentsAttr) {
        detuneCents = xml::Helper::parseDouble(detuneCentsAttr.value());
    }

    auto detuneHzAttr = xmlElement.attribute("detuneHz");
    if (detuneHzAttr) {
        detuneHz = xml::Helper::parseDouble(detuneHzAttr.value());
    }

    auto id = xmlElement.attribute("xml:id");
    if (id) {
        xmlId = id.value();
    }
}

ArticulationData ArticulationData::clone() const {
    ArticulationData clone;
    clone.xml = xml; // Element is copied
    clone.xmlId = xmlId;

    clone.styleName = styleName;
    clone.style = style;
    clone.defaultArticulation = defaultArticulation;
    clone.defaultArticulationDef = defaultArticulationDef;
    clone.articulationDefName = articulationDefName;
    clone.articulationDef = articulationDef;

    clone.date = date;
    clone.noteid = noteid;

    clone.absoluteDuration = absoluteDuration;
    clone.absoluteDurationChange = absoluteDurationChange;
    clone.relativeDuration = relativeDuration;
    clone.absoluteDurationMs = absoluteDurationMs;
    clone.absoluteDurationChangeMs = absoluteDurationChangeMs;
    clone.absoluteVelocityChange = absoluteVelocityChange;
    clone.absoluteVelocity = absoluteVelocity;
    clone.relativeVelocity = relativeVelocity;
    clone.absoluteDelayMs = absoluteDelayMs;
    clone.absoluteDelay = absoluteDelay;
    clone.detuneCents = detuneCents;
    clone.detuneHz = detuneHz;

    return clone;
}

bool ArticulationData::articulateNote(Element& note) const {
    if (!note) {
        return false;
    }

    // First apply the referred articulationDef
    bool dateChanged = false;
    if (articulationDef) {
        // TODO: Implement articulationDef.articulateNote(note) when ArticulationDef is available
        // dateChanged = articulationDef->articulateNote(note);
    }

    // Apply date modifiers
    auto dateAtt = note.attribute("date.perf");
    if (dateAtt) {
        if (absoluteDelay != 0.0) {
            double currentDate = xml::Helper::parseDouble(dateAtt.value());
            dateAtt.set_value(std::to_string(currentDate + absoluteDelay).c_str());
            dateChanged = true;
        }
        if (absoluteDelayMs != 0.0) {
            note.append_attribute("articulation.absoluteDelayMs") = std::to_string(absoluteDelayMs).c_str();
        }
    }

    // Apply duration modifiers
    auto durationAtt = note.attribute("duration.perf");
    if (durationAtt) {
        double duration = xml::Helper::parseDouble(durationAtt.value());
        
        if (absoluteDurationMs) {
            note.append_attribute("articulation.absoluteDurationMs") = std::to_string(*absoluteDurationMs).c_str();
        } else {
            // Apply symbolic duration changes only if no absolute milliseconds duration is specified
            if (absoluteDuration) {
                durationAtt.set_value(std::to_string(*absoluteDuration).c_str());
            }
            if (relativeDuration != 1.0) {
                durationAtt.set_value(std::to_string(duration * relativeDuration).c_str());
            }
            if (absoluteDurationChange != 0.0) {
                double durNew = duration + absoluteDurationChange;
                // Ensure duration doesn't become 0.0 or negative
                for (double reduce = 2.0; durNew <= 0.0; reduce *= 2.0) {
                    durNew = duration + (absoluteDurationChange / reduce);
                }
                durationAtt.set_value(std::to_string(durNew).c_str());
            }
        }
        
        if (absoluteDurationChangeMs != 0.0) {
            note.append_attribute("articulation.absoluteDurationChangeMs") = std::to_string(absoluteDurationChangeMs).c_str();
        }
    }

    // Apply velocity modifiers
    auto velocityAtt = note.attribute("velocity");
    if (velocityAtt) {
        if (absoluteVelocity) {
            velocityAtt.set_value(std::to_string(*absoluteVelocity).c_str());
        }
        if (relativeVelocity != 1.0) {
            double currentVelocity = xml::Helper::parseDouble(velocityAtt.value());
            velocityAtt.set_value(std::to_string(currentVelocity * relativeVelocity).c_str());
        }
        if (absoluteVelocityChange != 0.0) {
            double currentVelocity = xml::Helper::parseDouble(velocityAtt.value());
            velocityAtt.set_value(std::to_string(currentVelocity + absoluteVelocityChange).c_str());
        }
    }

    // Apply detuning
    if (detuneCents != 0.0) {
        note.append_attribute("detuneCents") = std::to_string(detuneCents).c_str();
    }
    if (detuneHz != 0.0) {
        note.append_attribute("detuneHz") = std::to_string(detuneHz).c_str();
    }

    return dateChanged;
}

} // namespace mpm
} // namespace meico