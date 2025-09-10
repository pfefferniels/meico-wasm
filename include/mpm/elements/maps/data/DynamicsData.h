#pragma once

#include "xml/XmlBase.h"
#include <memory>
#include <vector>

namespace meico {
namespace mpm {

// Forward declarations
class DynamicsStyle;
class DynamicsDef;

/**
 * This class is used to collect all relevant data to compute dynamics
 * Ported from Java DynamicsData class
 */
class DynamicsData {
public:
    Element xml;
    std::string xmlId;

    std::string styleName;
    std::shared_ptr<DynamicsStyle> style;
    std::string dynamicsDefString;
    std::shared_ptr<DynamicsDef> dynamicsDef;

    double startDate = 0.0;
    double endDate = 0.0;

    std::string volumeString;
    double volume = 0.0;

    std::string transitionToString;
    double transitionTo = 0.0;

    double curvature = 0.0;
    double protraction = 0.0;
    bool subNoteDynamics = false;

private:
    double x1 = 0.0;
    double x2 = 0.0;
    bool controlPointsComputed = false;

public:
    /**
     * Default constructor
     */
    DynamicsData();

    /**
     * Constructor from XML element parsing
     * @param xml MPM dynamics element
     */
    DynamicsData(const Element& xml);

    /**
     * Create a copy of this object
     * @return copied DynamicsData
     */
    std::unique_ptr<DynamicsData> clone() const;

    /**
     * Check whether this represents a constant dynamics instruction
     * @return true if constant dynamics
     */
    bool isConstantDynamics() const;

    /**
     * Compute the dynamics value at the given tick position
     * @param date the time position
     * @return the dynamics value
     */
    double getDynamicsAt(double date);

    /**
     * This method generates a list of [date, volume] pairs that can be rendered 
     * into a sequence of channelVolume events.
     * @param maxStepSize this sets the maximum volume step size between two adjacent pairs
     * @return vector of [date, volume] pairs
     */
    std::vector<std::pair<double, double>> getSubNoteDynamicsSegment(double maxStepSize);

private:
    /**
     * For continuous dynamics transitions the dynamics curve is constructed from 
     * a cubic, S-shaped Bézier curve (P0, P1, P2, P3): _/̅
     * This method derives the x-coordinates of the inner two control points from 
     * the values of curvature and protraction. All other coordinates are fixed.
     */
    void computeInnerControlPointsXPositions();

    /**
     * Compute parameter t of the Bézier curve that corresponds to time position date
     * @param date time position
     * @return parameter t
     */
    double getTForDate(double date);

    /**
     * This method works directly with the parameter t to specify a point on the Bézier curve
     * and returns a pair [date, volume]
     * @param t the parameter
     * @return [date, volume] pair
     */
    std::pair<double, double> getDateDynamics(double t);
};

} // namespace mpm
} // namespace meico