#pragma once

#include "mpm/elements/maps/GenericMap.h"
#include "mpm/elements/maps/data/TempoData.h"
#include "supplementary/KeyValue.h"
#include <vector>
#include <memory>

namespace meico {
namespace mpm {

/**
 * This class interfaces MPM's tempoMaps
 * Ported from Java TempoMap class
 */
class TempoMap : public GenericMap {
private:
    std::vector<supplementary::KeyValue<double, std::unique_ptr<TempoData>>> tempoData;

public:
    /**
     * Constructor
     */
    TempoMap();

    /**
     * Virtual destructor
     */
    virtual ~TempoMap() = default;

    /**
     * Factory method to create a tempo map
     * @return new TempoMap instance
     */
    static std::unique_ptr<TempoMap> createTempoMap();

    /**
     * Add a tempo element to the map
     * @param date musical time
     * @param bpm tempo in beats per minute (string or numeric)
     * @param transitionTo target tempo for transitions
     * @param beatLength beat length (default 0.25 for quarter note)
     * @param meanTempoAt parameter for tempo curves (0.0-1.0)
     * @param id XML ID (optional)
     * @return the index at which it was inserted
     */
    int addTempo(double date, const std::string& bpm, const std::string& transitionTo = "", 
                 double beatLength = 0.25, double meanTempoAt = 0.5, const std::string& id = "");

    /**
     * Add a tempo element to the map (numeric BPM)
     * @param date musical time
     * @param bpm tempo in beats per minute
     * @param beatLength beat length (default 0.25 for quarter note)
     * @return the index at which it was inserted
     */
    int addTempo(double date, double bpm, double beatLength = 0.25);

    /**
     * Add a tempo element from TempoData object
     * @param data the tempo data
     * @return the index at which it was inserted
     */
    int addTempo(std::unique_ptr<TempoData> data);

    /**
     * Get tempo data at a specific time (finds the relevant tempo instruction)
     * @param date the musical time
     * @return pointer to TempoData or nullptr if none found
     */
    TempoData* getTempoDataAt(double date) const;

    /**
     * Get tempo data of a specific element by index
     * @param index the index
     * @return pointer to TempoData or nullptr if invalid index
     */
    TempoData* getTempoDataOf(int index) const;

    /**
     * Compute the tempo in bpm at the specified position according to this tempoMap
     * @param date the musical time
     * @return tempo value in BPM
     */
    double getTempoAt(double date) const;

    /**
     * Compute the tempo in bpm from a given TempoData object and a date
     * @param date the musical time (should fall into the scope of the tempoData)
     * @param tempoData the tempo data (application should ensure date is in scope)
     * @return the tempo or 100.0 bpm if date lies out of scope or tempo data is insufficient
     */
    static double getTempoAt(double date, const TempoData* tempoData);

    /**
     * Iterate through the specified map, compute the milliseconds dates for all elements 
     * and add the results as attributes milliseconds.date and milliseconds.date.end
     * @param map An instance of GenericMap, typically an MSM score element
     * @param ppq the pulses per quarter timing resolution
     */
    void renderTempoToMap(GenericMap& map, int ppq);

    /**
     * Static variant of renderTempoToMap with fallback mechanism if no tempoMap is provided
     * @param map an instance of GenericMap, typically an MSM score element
     * @param ppq the pulses per quarter timing resolution
     * @param tempoMap the tempoMap that is the basis of these computations, or nullptr
     */
    static void renderTempoToMap(GenericMap& map, int ppq, TempoMap* tempoMap);

    /**
     * Convenience method for timing computation; computes the milliseconds difference 
     * between the tick date and the date of the tempo instruction
     * @param date musical time
     * @param ppq pulses per quarter timing resolution
     * @param tempoData a TempoData instance or nullptr (if no tempo information is given)
     * @return date in milliseconds
     */
    static double computeDiffTiming(double date, int ppq, const TempoData* tempoData);

    /**
     * Apply this tempo map to modify elements in an MSM part
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
     * Get the index of the tempo element before or at the given date
     * @param date the musical time
     * @return index or -1 if none found
     */
    int getElementIndexBeforeAt(double date) const;

    /**
     * Get the end date for a tempo instruction (date of next instruction or max value)
     * @param index the index of the current tempo instruction
     * @return the end date
     */
    double getEndDate(int index) const;

    /**
     * This method computes the exponent of the tempo curve segment as defined by one tempo instruction.
     * @param meanTempoAt the value of the meanTempoAt attribute should be greater than 0.0 and smaller than 1.0.
     * @return the exponent of a continuous tempo transition function
     */
    static double computeExponent(double meanTempoAt);

    /**
     * Timing computation for the case that no tempo data is given
     * @param date musical time
     * @param ppq pulses per quarter
     * @return date in milliseconds (assumes 100 bpm and beatLength=0.25)
     */
    static double computeMillisecondsForNoTempo(double date, int ppq);

    /**
     * Timing computation for constant tempo
     * @param date musical time
     * @param ppq pulses per quarter
     * @param tempoData tempo data
     * @return the milliseconds difference between tempoData.startDate and date
     */
    static double computeMillisecondsForConstantTempo(double date, int ppq, const TempoData* tempoData);

    /**
     * Timing computation for continuous tempo transition using Simpson's rule for numerical integration
     * @param date musical time
     * @param ppq pulses per quarter
     * @param tempoData tempo data
     * @return the milliseconds difference between tempoData.startDate and date
     */
    static double computeMillisecondsForTempoTransition(double date, int ppq, const TempoData* tempoData);
};

} // namespace mpm
} // namespace meico