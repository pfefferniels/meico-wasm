#pragma once

#include "mpm/elements/maps/GenericMap.h"
#include "mpm/elements/maps/data/ArticulationData.h"
#include "supplementary/KeyValue.h"
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

namespace meico {
namespace mpm {

// Forward declarations
class ArticulationStyle;
class ArticulationDef;

/**
 * This class interfaces MPM's articulationMaps.
 * Ported from the original Java ArticulationMap implementation.
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
     * Add an articulation element to the map
     * @param date musical time (in PPQ units)
     * @param articulationDefName reference to an articulationDef
     * @param noteid optional xml:id reference to the note (should start with #)
     * @param id optional XML ID for this entry
     * @return the index at which it has been inserted
     */
    int addArticulation(double date, const std::string& articulationDefName, 
                       const std::string& noteid = "", const std::string& id = "");

    /**
     * Add an articulation element to the map with all possible attributes
     * @param date musical time
     * @param absoluteDuration numeric value or nullptr
     * @param absoluteDurationChange numeric value
     * @param relativeDuration numeric value
     * @param absoluteDurationMs numeric value or nullptr
     * @param absoluteDurationChangeMs numeric value
     * @param absoluteVelocityChange numeric value
     * @param absoluteVelocity numeric value or nullptr
     * @param relativeVelocity numeric value
     * @param absoluteDelayMs numeric value
     * @param absoluteDelay numeric value
     * @param detuneCents numeric value
     * @param detuneHz numeric value
     * @param noteid optional xml:id reference to the note
     * @param id optional XML ID
     * @return the index at which it has been inserted
     */
    int addArticulation(double date, 
                       std::shared_ptr<double> absoluteDuration, 
                       double absoluteDurationChange, 
                       double relativeDuration,
                       std::shared_ptr<double> absoluteDurationMs, 
                       double absoluteDurationChangeMs,
                       double absoluteVelocityChange, 
                       std::shared_ptr<double> absoluteVelocity, 
                       double relativeVelocity,
                       double absoluteDelayMs, 
                       double absoluteDelay, 
                       double detuneCents, 
                       double detuneHz,
                       const std::string& noteid = "", 
                       const std::string& id = "");

    /**
     * Add an articulation element from ArticulationData object
     * @param data the articulation data to add
     * @return the index at which it has been inserted
     */
    int addArticulation(const ArticulationData& data);

    /**
     * Add a style switch (an MPM style element)
     * @param date musical time
     * @param styleName reference to a styleDef
     * @param defaultArticulation reference to an articulationDef
     * @param id optional XML ID
     * @return the index at which it has been inserted
     */
    int addStyleSwitch(double date, const std::string& styleName, 
                      const std::string& defaultArticulation = "", const std::string& id = "");

    /**
     * Get articulation data of a specified element in this map
     * @param index element index
     * @return articulation data or nullptr if invalid
     */
    std::shared_ptr<ArticulationData> getArticulationDataOf(int index);

    /**
     * Render articulation to map (no millisecond modifiers stage)
     * This method is meant to be applied BEFORE other timing transformations and AFTER dynamics rendering
     * @param map the map to modify
     */
    void renderArticulationToMap_noMillisecondModifiers(GenericMap& map);

    /**
     * Render articulation to map (millisecond modifiers stage)
     * This method is meant to be applied AFTER asynchrony has been added and BEFORE imprecision
     * @param map the map to modify
     */
    void renderArticulationToMap_millisecondModifiers(GenericMap& map);

    /**
     * Apply this articulation map to modify notes in an MSM part
     * @param msmPart the MSM part element to modify
     * @return true if any modifications were made
     */
    bool applyToMsmPart(Element msmPart) override;

    /**
     * Get the number of elements in this map
     * @return number of elements
     */
    size_t size() const { return articulationData.size(); }

protected:
    /**
     * Parse data from XML element
     * @param xmlElement the XML element to parse
     */
    void parseData(const Element& xmlElement) override;

private:
    /**
     * Collect all data needed to compute articulation at the specified date
     * @param date musical time
     * @return vector of all articulations at the specific date
     */
    std::vector<ArticulationData> getArticulationDataAt(double date);

    /**
     * Get the style that applies to the articulation at the specified index
     * @param index the index for which style data is needed
     * @param ad articulation data object to store style information
     */
    void findStyle(int index, ArticulationData& ad);

    /**
     * Apply articulation data to a note element
     * @param note the note element to modify
     * @param data the articulation data to apply
     * @return true if the note was modified
     */
    bool applyArticulationToNote(Element& note, const ArticulationData& data) const;

    /**
     * Get the current style that applies at the given index
     * @param index the index in the articulation data
     * @return the style name or empty string if none applies
     */
    std::string findStyleAt(size_t index) const;
};

} // namespace mpm
} // namespace meico