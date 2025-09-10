#pragma once

#include "xml/XmlBase.h"
#include <memory>
#include <vector>

namespace meico {
namespace mpm {

// Forward declarations
class MovementStyle;
class MovementDef;

/**
 * This class is used to collect all relevant data to compute movements
 * Ported from Java MovementData class
 */
class MovementData {
public:
    Element xml;
    std::string xmlId;

    double startDate = 0.0;
    double endDate = 0.0;

    double position = 0.0;
    double transitionTo = 1.0;
    std::string controller = "sustain";

    double curvature = 0.4;
    double protraction = 0.0;

private:
    double x1 = 0.0;
    double x2 = 0.0;
    bool controlPointsComputed = false;

public:
    /**
     * Default constructor
     */
    MovementData();

    /**
     * Constructor from XML element parsing
     * @param xml MPM movement element
     */
    MovementData(const Element& xml);

    /**
     * Create a copy of this object
     * @return copied MovementData
     */
    std::unique_ptr<MovementData> clone() const;

    /**
     * Compute the movement position value at the given tick position
     * @param date the time position
     * @return the position value
     */
    double getPositionAt(double date);

    /**
     * This method generates a list of [date, position] pairs that can be rendered 
     * into a sequence of movement events.
     * @param maxStepSize this sets the maximum position step size between two adjacent pairs
     * @return vector of [date, position] pairs
     */
    std::vector<std::pair<double, double>> getMovementSegment(double maxStepSize);

private:
    /**
     * For continuous movement transitions the movement curve is constructed from 
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
     * and returns a pair [date, position]
     * @param t the parameter
     * @return [date, position] pair
     */
    std::pair<double, double> getDatePosition(double t);
};

} // namespace mpm
} // namespace meico