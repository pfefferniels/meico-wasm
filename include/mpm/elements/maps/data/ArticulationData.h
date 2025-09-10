#pragma once

#include "xml/Helper.h"
#include <string>
#include <memory>

namespace meico {
namespace mpm {

// Forward declarations
class ArticulationStyle;
class ArticulationDef;

/**
 * This class is used to collect all relevant data to compute articulation.
 * Ported from the original Java ArticulationData class.
 */
class ArticulationData {
public:
    Element xml;
    std::string xmlId;

    std::string styleName;
    std::shared_ptr<ArticulationStyle> style;
    std::string defaultArticulation;
    std::shared_ptr<ArticulationDef> defaultArticulationDef;
    std::string articulationDefName;
    std::shared_ptr<ArticulationDef> articulationDef;

    double date = 0.0;                       // the date for which the data is assembled
    std::string noteid;

    std::shared_ptr<double> absoluteDuration = nullptr;
    double absoluteDurationChange = 0.0;
    std::shared_ptr<double> absoluteDurationMs = nullptr;
    double absoluteDurationChangeMs = 0.0;
    double relativeDuration = 1.0;
    double absoluteDelay = 0.0;
    double absoluteDelayMs = 0.0;
    std::shared_ptr<double> absoluteVelocity = nullptr;
    double absoluteVelocityChange = 0.0;
    double relativeVelocity = 1.0;
    double detuneCents = 0.0;
    double detuneHz = 0.0;

    /**
     * Default constructor
     */
    ArticulationData();

    /**
     * Constructor with XML element parsing
     * @param xml MPM articulation element
     */
    ArticulationData(const Element& xml);

    /**
     * Create a copy of this object
     * @return cloned ArticulationData
     */
    ArticulationData clone() const;

    /**
     * Apply this articulationData to the specified MSM note element
     * @param note the note element to modify
     * @return true if the date changed, as this might require the map to be reordered
     */
    bool articulateNote(Element& note) const;
};

} // namespace mpm
} // namespace meico