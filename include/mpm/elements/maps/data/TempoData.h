#pragma once

#include "xml/XmlBase.h"
#include <memory>
#include <string>

namespace meico {
namespace mpm {

// Forward declarations
class TempoStyle;

/**
 * This class is used to collect all relevant data to compute tempo
 * Ported from Java TempoData class
 */
class TempoData {
public:
    Element xml;
    std::string xmlId;

    std::string styleName;
    std::shared_ptr<TempoStyle> style;

    double startDate = 0.0;                  // the date at which the tempo instruction starts
    double startDateMilliseconds = 0.0;     // to be used during timing computations, not by the application
    double endDate = 0.0;                   // this is set to the date of the subsequent tempo element or to max value

    std::string bpmString;
    double bpm = 0.0;

    std::string transitionToString;
    double transitionTo = 0.0;

    double beatLength = 0.25;               // default beat length

    double meanTempoAt = 0.0;              // for tempo transitions
    double exponent = 0.0;                 // computed exponent for tempo curve

public:
    /**
     * Default constructor
     */
    TempoData();

    /**
     * Constructor from XML element parsing
     * @param xml MPM tempo element
     */
    TempoData(const Element& xml);

    /**
     * Create a copy of this object
     * @return copied TempoData
     */
    std::unique_ptr<TempoData> clone() const;

    /**
     * Check whether this represents a constant tempo instruction
     * @return true if constant tempo (no transition or same bpm and transitionTo)
     */
    bool isConstantTempo() const;
};

} // namespace mpm
} // namespace meico