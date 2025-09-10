#pragma once

#include "xml/XmlBase.h"
#include <vector>
#include <string>
#include <memory>

namespace meico {
namespace mpm {

// Forward declarations
class OrnamentationStyle;
class OrnamentDef;

/**
 * This class is used to collect all relevant data to compute ornamentation.
 * Ported from Java meico.mpm.elements.maps.data.OrnamentData
 * @author Axel Berndt (original Java), C++ port
 */
class OrnamentData {
public:
    Element xml;
    std::string xmlId;
    
    std::string styleName;
    OrnamentationStyle* style = nullptr;
    std::string ornamentDefName;
    OrnamentDef* ornamentDef = nullptr;
    
    double date = 0.0;                       // the date for which the data is assembled
    double scale = 0.0;
    std::vector<std::string> noteOrder;
    
    /**
     * Default constructor
     */
    OrnamentData();
    
    /**
     * Constructor from XML element parsing
     * @param xml MPM ornament element
     */
    OrnamentData(const Element& xml);
    
    /**
     * Copy constructor
     */
    OrnamentData(const OrnamentData& other);
    
    /**
     * Assignment operator
     */
    OrnamentData& operator=(const OrnamentData& other);
    
    /**
     * Create a copy of this object
     * @return cloned object
     */
    std::unique_ptr<OrnamentData> clone() const;
    
    /**
     * Apply the ornament to the given chord/note sequence. This will only add
     * corresponding attributes to the notes; their realization in performance
     * attributes is done later during performance rendering. This method will
     * also return new notes to be added to the chordSequence's underlying map.
     * If notes should be deleted from the performance, they are marked by an according attribute.
     * @param chordSequence the sequence of the chords/notes in which the ornament is applied
     * @return sequence of chords/notes to be added to the chordSequence's underlying map
     */
    std::vector<std::vector<Element>> apply(const std::vector<std::vector<Element>>& chordSequence);

private:
    /**
     * Helper method to parse XML attributes
     */
    void parseFromXml(const Element& xml);
};

} // namespace mpm
} // namespace meico