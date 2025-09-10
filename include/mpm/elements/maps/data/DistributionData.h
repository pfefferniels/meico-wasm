#pragma once

#include "xml/XmlBase.h"
#include "supplementary/KeyValue.h"
#include <string>
#include <vector>
#include <memory>

namespace meico {
namespace mpm {

/**
 * this class is used to collect all relevant data to compute imprecision
 * @author Axel Berndt (Java), Copilot (C++ port)
 */
class DistributionData {
public:
    static const std::string UNIFORM;
    static const std::string GAUSSIAN;
    static const std::string TRIANGULAR;
    static const std::string BROWNIAN;
    static const std::string COMPENSATING_TRIANGLE;
    static const std::string LIST;

    Element xml;
    std::string xmlId;
    double startDate = 0.0;              // the time position of the distribution element
    double endDate = 0.0;

    std::string type;                    // the type of distribution, i.e. the local name of the element

    double standardDeviation = 0.0;
    double maxStepWidth = 0.0;
    double degreeOfCorrelation = 0.0;
    double mode = 0.0;
    double lowerLimit = 0.0;
    double upperLimit = 0.0;
    double lowerClip = 0.0;
    double upperClip = 0.0;
    long seed = 0;
    double millisecondsTimingBasis = 0.0;

    std::vector<double> distributionList;

    // Flags to track which optional values are set
    bool hasStandardDeviation = false;
    bool hasMaxStepWidth = false;
    bool hasDegreeOfCorrelation = false;
    bool hasMode = false;
    bool hasLowerLimit = false;
    bool hasUpperLimit = false;
    bool hasLowerClip = false;
    bool hasUpperClip = false;
    bool hasSeed = false;
    bool hasMillisecondsTimingBasis = false;

    /**
     * default constructor
     */
    DistributionData();

    /**
     * constructor from XML element parsing
     * @param xml MPM distribution element
     */
    explicit DistributionData(const Element& xml);

    /**
     * create a copy of this object
     * @return
     */
    std::unique_ptr<DistributionData> clone() const;

    /**
     * determine the minimum and maximum value in the distribution list
     * @return a KeyValue tuplet (min value, max value)
     */
    supplementary::KeyValue<double, double> getMinAndMaxValueInDistributionList() const;
};

} // namespace mpm
} // namespace meico