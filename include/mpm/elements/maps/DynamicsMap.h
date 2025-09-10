#pragma once

#include "mpm/elements/maps/GenericMap.h"
#include "supplementary/KeyValue.h"
#include <vector>
#include <memory>

namespace meico {
namespace mpm {

/**
 * This class represents a dynamics map in MPM.
 * Maps musical time to dynamic levels (velocity).
 */
class DynamicsMap : public GenericMap {
private:
    std::vector<supplementary::KeyValue<double, double>> dynamicsData;

public:
    /**
     * Constructor
     */
    DynamicsMap();

    /**
     * Virtual destructor
     */
    virtual ~DynamicsMap() = default;

    /**
     * Factory method to create a dynamics map
     * @return new DynamicsMap instance
     */
    static std::unique_ptr<DynamicsMap> createDynamicsMap();

    /**
     * Add a dynamics entry
     * @param date the musical time (in PPQ units)
     * @param dynamics the dynamics value (velocity)
     */
    void addDynamics(double date, double dynamics);

    /**
     * Get dynamics value at a specific time
     * @param date the musical time
     * @return dynamics value (interpolated if needed)
     */
    double getDynamicsAt(double date) const;

    /**
     * Apply this dynamics map to modify notes in an MSM part
     * @param msmPart the MSM part element to modify
     * @return true if any modifications were made
     */
    bool applyToMsmPart(Element msmPart) override;

protected:
    /**
     * Parse data from XML element
     * @param xmlElement the XML element to parse
     */
    void parseData(const Element& xmlElement) override;

private:
    /**
     * Linear interpolation between two values
     * @param x0 first x coordinate
     * @param y0 first y coordinate
     * @param x1 second x coordinate
     * @param y1 second y coordinate
     * @param x target x coordinate
     * @return interpolated y value
     */
    double interpolate(double x0, double y0, double x1, double y1, double x) const;
};

} // namespace mpm
} // namespace meico