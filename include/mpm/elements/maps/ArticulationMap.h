#pragma once

#include "mpm/elements/maps/GenericMap.h"
#include "supplementary/KeyValue.h"
#include <vector>
#include <memory>
#include <string>

namespace meico {
namespace mpm {

/**
 * Struct to hold articulation data
 */
struct ArticulationData {
    double date = 0.0;
    std::string xmlId;
    std::string noteid;
    std::string articulationDefName;
    
    // Articulation parameters
    double absoluteDuration = 0.0;
    double absoluteDurationChange = 0.0;
    double relativeDuration = 1.0;
    double absoluteDurationMs = 0.0;
    double absoluteDurationChangeMs = 0.0;
    double absoluteVelocityChange = 0.0;
    double absoluteVelocity = 0.0;
    double relativeVelocity = 1.0;
    double absoluteDelayMs = 0.0;
    double absoluteDelay = 0.0;
    double detuneCents = 0.0;
    double detuneHz = 0.0;
    
    // Style information
    std::string styleName;
    std::string defaultArticulation;
    
    Element xml;
    
    ArticulationData() = default;
};

/**
 * This class represents an articulation map in MPM.
 * Handles performance articulations like staccato, legato, etc.
 */
class ArticulationMap : public GenericMap {
private:
    std::vector<ArticulationData> articulationData;

public:
    /**
     * Constructor
     */
    ArticulationMap();

    /**
     * Virtual destructor
     */
    virtual ~ArticulationMap() = default;

    /**
     * Factory method to create an articulation map
     * @return new ArticulationMap instance
     */
    static std::unique_ptr<ArticulationMap> createArticulationMap();

    /**
     * Add an articulation entry by reference to an articulation definition
     * @param date the musical time (in PPQ units)
     * @param articulationDefName reference to an articulation definition
     * @param noteid optional note ID this articulation applies to
     * @param id optional XML ID for this articulation
     */
    void addArticulation(double date, const std::string& articulationDefName, 
                        const std::string& noteid = "", const std::string& id = "");

    /**
     * Add an articulation entry with explicit parameters
     * @param date the musical time
     * @param absoluteDuration absolute duration value
     * @param relativeDuration relative duration multiplier
     * @param absoluteVelocity absolute velocity value
     * @param relativeVelocity relative velocity multiplier
     * @param noteid optional note ID this articulation applies to
     * @param id optional XML ID for this articulation
     */
    void addArticulation(double date, double absoluteDuration, double relativeDuration,
                        double absoluteVelocity, double relativeVelocity,
                        const std::string& noteid = "", const std::string& id = "");

    /**
     * Add an articulation entry from ArticulationData struct
     * @param data the articulation data to add
     */
    void addArticulation(const ArticulationData& data);

    /**
     * Add a style switch
     * @param date the musical time
     * @param styleName reference to a style definition
     * @param defaultArticulation default articulation for this style
     * @param id optional XML ID
     */
    void addStyleSwitch(double date, const std::string& styleName, 
                       const std::string& defaultArticulation = "", const std::string& id = "");

    /**
     * Get articulation data at a specific time
     * @param date the musical time
     * @return vector of articulation data that applies at this time
     */
    std::vector<ArticulationData> getArticulationDataAt(double date) const;

    /**
     * Apply this articulation map to modify notes in an MSM part
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
     * Apply articulation data to a note element
     * @param note the note element to modify
     * @param data the articulation data to apply
     * @return true if the note was modified
     */
    bool applyArticulationToNote(Element note, const ArticulationData& data) const;

    /**
     * Get the current style that applies at the given index
     * @param index the index in the articulation data
     * @return the style name or empty string if none applies
     */
    std::string findStyleAt(size_t index) const;
};

} // namespace mpm
} // namespace meico