#pragma once

#include "mpm/elements/maps/GenericMap.h"
#include "mpm/elements/maps/data/MovementData.h"
#include "supplementary/KeyValue.h"
#include <vector>
#include <memory>

namespace meico {
namespace mpm {

/**
 * This class represents a movement map in MPM.
 * Maps musical time to movement positions for controllers like sustain pedal.
 * Ported from Java MovementMap class
 */
class MovementMap : public GenericMap {
private:
    std::vector<supplementary::KeyValue<double, std::unique_ptr<MovementData>>> movementData;

public:
    /**
     * Constructor
     */
    MovementMap();

    /**
     * Virtual destructor
     */
    virtual ~MovementMap() = default;

    /**
     * Factory method to create a movement map
     * @return new MovementMap instance
     */
    static std::unique_ptr<MovementMap> createMovementMap();

    /**
     * Add a movement entry with full parameters
     * @param date the musical time (in PPQ units)
     * @param controller the controller name (e.g., "sustain")
     * @param position the position value (0.0-1.0)
     * @param transitionTo the transition target (0.0-1.0)
     * @param id the XML ID (optional)
     */
    void addMovement(double date, const std::string& controller, double position, 
                     double transitionTo, const std::string& id = "");

    /**
     * Add a movement entry with curvature and protraction parameters
     * @param date the musical time (in PPQ units)
     * @param controller the controller name (e.g., "sustain")
     * @param position the position value (0.0-1.0)
     * @param transitionTo the transition target (0.0-1.0)
     * @param curvature the curvature parameter (0.0-1.0)
     * @param protraction the protraction parameter (-1.0-1.0)
     * @param id the XML ID (optional)
     */
    void addMovement(double date, const std::string& controller, double position, 
                     double transitionTo, double curvature, double protraction,
                     const std::string& id = "");

    /**
     * Add a movement entry from MovementData object
     * @param data the movement data
     */
    void addMovement(std::unique_ptr<MovementData> data);

    /**
     * Get movement data at a specific time (finds the relevant movement instruction)
     * @param date the musical time
     * @return pointer to MovementData or nullptr if none found
     */
    MovementData* getMovementDataAt(double date) const;

    /**
     * Get movement data of a specific element by index
     * @param index the index
     * @return pointer to MovementData or nullptr if invalid index
     */
    MovementData* getMovementDataOf(int index) const;

    /**
     * Get movement position value at a specific time using original Java algorithm
     * @param date the musical time
     * @return movement position value (computed using Bézier curves)
     */
    double getPositionAt(double date) const;

    /**
     * Apply this movement map to create a position map in MSM
     * @param msmPart the MSM part element to modify
     * @return true if any modifications were made
     */
    bool applyToMsmPart(Element msmPart) override;

    /**
     * Render movement to map - creates a positionMap with movement data
     * @return GenericMap representing the positionMap
     */
    std::unique_ptr<GenericMap> renderMovementToMap();

    /**
     * Static helper method to render movement to map
     * @param movementMap the movement map to render
     * @return GenericMap representing the positionMap or nullptr if input is null
     */
    static std::unique_ptr<GenericMap> renderMovementToMap(MovementMap* movementMap);

protected:
    /**
     * Parse data from XML element
     * @param xmlElement the XML element to parse
     */
    void parseData(const Element& xmlElement) override;

private:
    /**
     * Get the index of the movement element before or at the given date
     * @param date the musical time
     * @return index or -1 if none found
     */
    int getElementIndexBeforeAt(double date) const;

    /**
     * Get the end date for a movement instruction (date of next instruction or max value)
     * @param index the index of the current movement instruction
     * @return the end date
     */
    double getEndDate(int index) const;

    /**
     * Get the previous position value for movement elements without explicit position
     * @param index the index of the current movement instruction
     * @return the previous position value
     */
    double getPreviousPosition(int index) const;

    /**
     * Generate movement events and add them to the position map
     * @param movementData the movement data to process
     * @param positionMap the target map to add position events
     */
    static void generateMovement(MovementData* movementData, GenericMap* positionMap);
};

} // namespace mpm
} // namespace meico