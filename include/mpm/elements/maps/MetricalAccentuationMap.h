#pragma once

#include "mpm/elements/maps/GenericMap.h"
#include "mpm/elements/maps/data/MetricalAccentuationData.h"
#include "supplementary/KeyValue.h"
#include <vector>
#include <memory>
#include <string>

namespace meico {
namespace mpm {

// Forward declarations
class MetricalAccentuationStyle;
class AccentuationPatternDef;

/**
 * This class interfaces MPM's metricalAccentuationMaps.
 * Ported from the original Java MetricalAccentuationMap implementation.
 */
class MetricalAccentuationMap : public GenericMap {
private:
    std::vector<MetricalAccentuationData> accentuationData;

public:
    /**
     * Constructor
     */
    MetricalAccentuationMap();

    /**
     * Virtual destructor
     */
    virtual ~MetricalAccentuationMap() = default;

    /**
     * Factory method to create a metrical accentuation map
     * @return new MetricalAccentuationMap instance
     */
    static std::unique_ptr<MetricalAccentuationMap> createMetricalAccentuationMap();

    /**
     * Add an accentuationPattern element to the map
     * @param date musical time
     * @param accentuationPatternDefName reference to an accentuationPatternDef
     * @param scale scaling factor
     * @param loop whether pattern loops
     * @param stickToMeasures restart pattern with every measure
     * @return the index at which it has been inserted
     */
    int addAccentuationPattern(double date, const std::string& accentuationPatternDefName, 
                              double scale, bool loop, bool stickToMeasures);

    /**
     * Add an accentuationPattern element to the map (simplified version)
     * @param date musical time
     * @param accentuationPatternDefName reference to an accentuationPatternDef
     * @param scale scaling factor
     * @param loop whether pattern loops
     * @return the index at which it has been inserted
     */
    int addAccentuationPattern(double date, const std::string& accentuationPatternDefName, 
                              double scale, bool loop);

    /**
     * Add an accentuationPattern element to the map (basic version)
     * @param date musical time
     * @param accentuationPatternDefName reference to an accentuationPatternDef
     * @param scale scaling factor
     * @return the index at which it has been inserted
     */
    int addAccentuationPattern(double date, const std::string& accentuationPatternDefName, 
                              double scale);

    /**
     * Add an accentuationPattern element from MetricalAccentuationData object
     * @param data the accentuation data to add
     * @return the index at which it has been inserted
     */
    int addAccentuationPattern(const MetricalAccentuationData& data);

    /**
     * Get metrical accentuation data of a specified element in this map
     * @param index element index
     * @return accentuation data or nullptr if invalid
     */
    std::shared_ptr<MetricalAccentuationData> getMetricalAccentuationDataOf(int index);

    /**
     * Render metrical accentuation to map
     * @param map the map to modify (preferably an MSM score)
     * @param timeSignatureMap time signature information
     * @param ppq pulses per quarter note
     */
    void renderMetricalAccentuationToMap(GenericMap& map, GenericMap* timeSignatureMap, int ppq);

    /**
     * Apply this accentuation map to modify notes in an MSM part
     * @param msmPart the MSM part element to modify
     * @return true if any modifications were made
     */
    bool applyToMsmPart(Element msmPart) override;

    /**
     * Get the number of elements in this map
     * @return number of elements
     */
    size_t size() const { return accentuationData.size(); }

protected:
    /**
     * Parse data from XML element
     * @param xmlElement the XML element to parse
     */
    void parseData(const Element& xmlElement) override;

private:
    /**
     * Get the accentuation data for a specified date
     * @param date musical time
     * @return accentuation data or nullptr if none applies
     */
    std::shared_ptr<MetricalAccentuationData> getMetricalAccentuationDataAt(double date);

    /**
     * Helper method to get the end date of an accentuation pattern
     * @param index index of the current accentuationPattern instruction
     * @return end date or Double.MAX_VALUE
     */
    double getEndDate(int index);

    /**
     * Compute accentuation value for a given beat position
     * @param beat the beat position (1.0 = first beat, 2.0 = second beat, etc.)
     * @param data the accentuation data to use
     * @return accentuation value
     */
    double computeAccentuationAt(double beat, const MetricalAccentuationData& data) const;

    /**
     * Simple beat-based accentuation pattern (simplified implementation)
     * @param beat the beat position
     * @return base accentuation value (-1.0 to 1.0)
     */
    double getBasicAccentuationPattern(double beat) const;
};

} // namespace mpm
} // namespace meico