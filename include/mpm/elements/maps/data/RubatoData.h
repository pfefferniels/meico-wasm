#pragma once

#include "xml/XmlBase.h"
#include <memory>
#include <string>

namespace meico {
namespace mpm {

// Forward declarations
class RubatoStyle;
class RubatoDef;

/**
 * This class is used to collect all relevant data to compute rubato
 * Ported from Java RubatoData class
 */
class RubatoData {
public:
    Element xml;
    std::string xmlId;

    std::string styleName;
    std::shared_ptr<RubatoStyle> style;
    std::string rubatoDefString;
    std::shared_ptr<RubatoDef> rubatoDef;

    double startDate = 0.0;                  // the date at which the rubato instruction starts
    double endDate = 0.0;                   // this is set to the date of the subsequent rubato element or to max value

    double frameLength = 1.0;               // length of the rubato frame
    double intensity = 1.0;                 // rubato intensity for transformation
    double lateStart = 0.0;                 // late start parameter (0.0-1.0)
    double earlyEnd = 1.0;                  // early end parameter (0.0-1.0)

    bool loop = false;                      // whether this is a looping or oneshot rubato

public:
    /**
     * Default constructor
     */
    RubatoData();

    /**
     * Constructor from XML element parsing
     * @param xml MPM rubato element
     */
    RubatoData(const Element& xml);

    /**
     * Copy constructor
     */
    RubatoData(const RubatoData& other);

    /**
     * Assignment operator
     */
    RubatoData& operator=(const RubatoData& other);

    /**
     * Virtual destructor
     */
    virtual ~RubatoData() = default;

    /**
     * Create a copy of this object
     * @return clone of this RubatoData
     */
    std::unique_ptr<RubatoData> clone() const;
};

}  // namespace mpm
}  // namespace meico