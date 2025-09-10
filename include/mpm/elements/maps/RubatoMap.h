#pragma once

#include "mpm/elements/maps/GenericMap.h"
#include "mpm/elements/maps/data/RubatoData.h"
#include "supplementary/KeyValue.h"
#include <vector>
#include <memory>

namespace meico {
namespace mpm {

/**
 * This class interfaces MPM's rubatoMaps
 * Ported from Java RubatoMap class
 */
class RubatoMap : public GenericMap {
private:
    std::vector<supplementary::KeyValue<double, std::unique_ptr<RubatoData>>> rubatoData;

public:
    /**
     * Constructor
     */
    RubatoMap();

    /**
     * Virtual destructor
     */
    virtual ~RubatoMap() = default;

    /**
     * Factory method to create a rubato map
     * @return new RubatoMap instance
     */
    static std::unique_ptr<RubatoMap> createRubatoMap();

    /**
     * Add a rubato element to the map (with direct attributes)
     * @param date musical time
     * @param frameLength length of the rubato frame
     * @param intensity rubato intensity
     * @param lateStart late start parameter (0.0-1.0)
     * @param earlyEnd early end parameter (0.0-1.0)
     * @param loop whether this is a looping rubato
     * @param id XML ID (optional)
     * @return the index at which it was inserted
     */
    int addRubato(double date, double frameLength, double intensity, 
                  double lateStart, double earlyEnd, bool loop, const std::string& id = "");

    /**
     * Add a rubato element to the map (with rubatoDef reference)
     * @param date musical time
     * @param rubatoDefName reference to a rubatoDef
     * @param loop whether this is a looping rubato
     * @param id XML ID (optional)
     * @return the index at which it was inserted
     */
    int addRubato(double date, const std::string& rubatoDefName, bool loop, const std::string& id = "");

    /**
     * Add a rubato element to the map (with RubatoData object)
     * @param data RubatoData object containing all attributes
     * @return the index at which it was inserted
     */
    int addRubato(const RubatoData& data);

    /**
     * Get rubato data at the specified date
     * @param date musical time
     * @return RubatoData pointer or nullptr if no rubato at this date
     */
    RubatoData* getRubatoDataAt(double date);

    /**
     * Get rubato data of a specific element
     * @param index element index
     * @return RubatoData pointer or nullptr if invalid index
     */
    RubatoData* getRubatoDataOf(int index);

    /**
     * Apply rubato transformations to all date and duration attributes of a map
     * @param map target map to transform
     */
    void renderRubatoToMap(GenericMap& map);

    /**
     * Static method to apply rubato transformations to a map
     * @param map target map to transform
     * @param rubatoMap source rubato map
     */
    static void renderRubatoToMap(GenericMap& map, RubatoMap& rubatoMap);

    /**
     * Apply this rubato map to modify elements in an MSM part
     * @param msmPart the MSM part element to modify
     * @return true if any modifications were made
     */
    bool applyToMsmPart(Element msmPart) override;

    /**
     * Compute the rubato transformation for a given date
     * @param date input date
     * @param rubatoData rubato parameters
     * @return transformed date
     */
    static double computeRubatoTransformation(double date, const RubatoData& rubatoData);

protected:
    /**
     * Parse data from XML element
     * @param xmlElement the XML element to parse
     */
    void parseData(const Element& xmlElement) override;

private:
    /**
     * Ensure intensity has a valid value
     * @param intensity input intensity
     * @return corrected intensity
     */
    static double ensureIntensityBoundaries(double intensity);

    /**
     * Ensure validity and consistency of lateStart and earlyEnd
     * @param lateStart late start parameter
     * @param earlyEnd early end parameter
     * @return corrected values
     */
    static std::pair<double, double> ensureLateStartEarlyEndBoundaries(double lateStart, double earlyEnd);

    /**
     * Get the end date for the indexed rubato instruction
     * @param index index of the current rubato instruction
     * @return end date
     */
    double getEndDate(int index);
};

}  // namespace mpm
}  // namespace meico