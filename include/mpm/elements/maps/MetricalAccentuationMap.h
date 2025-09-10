#pragma once

#include "mpm/elements/maps/GenericMap.h"
#include "supplementary/KeyValue.h"
#include <vector>
#include <memory>
#include <string>

namespace meico {
namespace mpm {

/**
 * Simple struct to hold metrical accentuation data
 */
struct MetricalAccentuationData {
    double startDate = 0.0;
    double endDate = 0.0;
    std::string accentuationPatternDefName;
    double scale = 1.0;
    bool loop = true;
    bool stickToMeasures = true;
    std::string xmlId;
    std::string styleName;
    Element xml;
};

/**
 * This class represents a metrical accentuation map in MPM.
 * Applies accentuation patterns to musical beats for dynamics.
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
     * Add an accentuation pattern entry
     * @param date the musical time (in PPQ units)
     * @param accentuationPatternDefName reference to an accentuation pattern definition
     * @param scale scaling factor for the accentuation
     * @param loop whether the pattern loops
     * @param stickToMeasures restart pattern with every measure
     */
    void addAccentuationPattern(double date, const std::string& accentuationPatternDefName, 
                              double scale = 1.0, bool loop = true, bool stickToMeasures = true);

    /**
     * Get accentuation data at a specific time
     * @param date the musical time
     * @return accentuation data or nullptr if none applies
     */
    const MetricalAccentuationData* getAccentuationDataAt(double date) const;

    /**
     * Apply this accentuation map to modify notes in an MSM part
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