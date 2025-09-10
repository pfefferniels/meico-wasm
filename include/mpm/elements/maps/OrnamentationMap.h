#pragma once

#include "mpm/elements/maps/GenericMap.h"
#include "mpm/elements/maps/data/OrnamentData.h"
#include "supplementary/KeyValue.h"
#include "xml/XmlBase.h"
#include <vector>
#include <string>
#include <memory>

namespace meico {
namespace mpm {

/**
 * This class interfaces MPM's ornamentationMaps
 * Ported from Java meico.mpm.elements.maps.OrnamentationMap
 * @author Axel Berndt (original Java), C++ port
 */
class OrnamentationMap : public GenericMap {
private:
    std::vector<supplementary::KeyValue<double, std::unique_ptr<OrnamentData>>> ornamentData;

public:
    /**
     * Constructor
     */
    OrnamentationMap();
    
    /**
     * Virtual destructor
     */
    virtual ~OrnamentationMap() = default;

    /**
     * OrnamentationMap factory
     * @return new OrnamentationMap instance
     */
    static std::unique_ptr<OrnamentationMap> createOrnamentationMap();
    
    /**
     * Add an ornament element to the ornamentationMap
     * @param date date of the ornament
     * @param nameRef reference to ornament definition
     * @param scale scale factor (1.0 to omit from XML)
     * @param noteOrder vector of note IDs or special values (empty to omit)
     * @param id XML ID (empty to omit)
     * @return the index at which the element has been added
     */
    int addOrnament(double date, const std::string& nameRef, double scale = 1.0, 
                   const std::vector<std::string>& noteOrder = {}, const std::string& id = "");
    
    /**
     * Add an ornament element to the ornamentationMap
     * @param data ornament data 
     * @return the index at which the element has been added
     */
    int addOrnament(const OrnamentData& data);
    
    /**
     * Get ornament data at a specific time
     * @param date the musical time
     * @return pointer to OrnamentData or nullptr if none found
     */
    OrnamentData* getOrnamentDataAt(double date) const;
    
    /**
     * Get ornament data of a specific element by index
     * @param index the index
     * @return pointer to OrnamentData or nullptr if invalid index
     */
    OrnamentData* getOrnamentDataOf(int index) const;
    
    /**
     * Get the number of ornament elements
     * @return number of elements
     */
    size_t size() const;
    
    /**
     * Check if map is empty
     * @return true if empty
     */
    bool isEmpty() const;
    
    /**
     * Apply this ornamentation map to modify notes in an MSM part
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
     * Insert an ornament data element into the sorted vector
     * @param date the date/time
     * @param data the ornament data
     * @return the index where it was inserted
     */
    int insertOrnamentData(double date, std::unique_ptr<OrnamentData> data);
    
    /**
     * Apply ornament modifications to a note element
     * @param note the note element to modify
     * @param ornamentData the ornament data to apply
     * @return true if modifications were made
     */
    bool applyOrnamentToNote(Element note, const OrnamentData& ornamentData);
    
    /**
     * Get the index of the ornament element at or before the given date
     * @param date the musical time
     * @return index or -1 if none found
     */
    int getElementIndexBeforeAt(double date) const;
};

} // namespace mpm
} // namespace meico