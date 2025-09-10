#pragma once

#include "mpm/elements/maps/GenericMap.h"
#include "mpm/elements/maps/data/DynamicsData.h"
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
    std::vector<supplementary::KeyValue<double, std::unique_ptr<DynamicsData>>> dynamicsData;

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
     * Add a dynamics entry with full parameters
     * @param date the musical time (in PPQ units)
     * @param volume the volume string or value
     * @param transitionTo the transition target (optional)
     * @param curvature the curvature parameter (0.0-1.0)
     * @param protraction the protraction parameter (-1.0-1.0)
     * @param subNoteDynamics whether to enable sub-note dynamics
     * @param id the XML ID (optional)
     */
    void addDynamics(double date, const std::string& volume, const std::string& transitionTo = "", 
                     double curvature = 0.0, double protraction = 0.0, bool subNoteDynamics = false,
                     const std::string& id = "");

    /**
     * Add a dynamics entry from DynamicsData object
     * @param data the dynamics data
     */
    void addDynamics(std::unique_ptr<DynamicsData> data);

    /**
     * Get dynamics data at a specific time (finds the relevant dynamics instruction)
     * @param date the musical time
     * @return pointer to DynamicsData or nullptr if none found
     */
    DynamicsData* getDynamicsDataAt(double date) const;

    /**
     * Get dynamics data of a specific element by index
     * @param index the index
     * @return pointer to DynamicsData or nullptr if invalid index
     */
    DynamicsData* getDynamicsDataOf(int index) const;

    /**
     * Get dynamics value at a specific time using original Java algorithm
     * @param date the musical time
     * @return dynamics value (computed using Bézier curves)
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
     * Get the index of the dynamics element before or at the given date
     * @param date the musical time
     * @return index or -1 if none found
     */
    int getElementIndexBeforeAt(double date) const;

    /**
     * Get the end date for a dynamics instruction (date of next instruction or max value)
     * @param index the index of the current dynamics instruction
     * @return the end date
     */
    double getEndDate(int index) const;
};

} // namespace mpm
} // namespace meico