#pragma once

#include "xml/Helper.h"
#include <string>
#include <memory>

namespace meico {
namespace mpm {

// Forward declarations
class MetricalAccentuationStyle;
class AccentuationPatternDef;

/**
 * This class is used to collect all relevant data to compute metrical accentuation.
 * Ported from the original Java MetricalAccentuationData class.
 */
class MetricalAccentuationData {
public:
    Element xml;
    std::string xmlId;

    std::string styleName;
    std::shared_ptr<MetricalAccentuationStyle> style;

    std::string accentuationPatternDefName;
    std::shared_ptr<AccentuationPatternDef> accentuationPatternDef;

    double startDate = 0.0;
    std::shared_ptr<double> endDate = nullptr;
    double scale = 1.0;
    bool loop = false;
    bool stickToMeasures = true;

    /**
     * Default constructor
     */
    MetricalAccentuationData();

    /**
     * Constructor with XML element parsing
     * @param xml MPM accentuationPattern element
     */
    MetricalAccentuationData(const Element& xml);

    /**
     * Create a copy of this object
     * @return cloned MetricalAccentuationData
     */
    MetricalAccentuationData clone() const;
};

} // namespace mpm
} // namespace meico