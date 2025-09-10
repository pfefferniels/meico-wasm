#pragma once

#include "xml/AbstractXmlSubtree.h"
#include <string>

namespace meico {
namespace mpm {

/**
 * This class represents a generic map in MPM.
 * Base class for all performance maps.
 */
class GenericMap : public xml::AbstractXmlSubtree {
private:
    std::string mapType;

public:
    /**
     * Constructor
     * @param type the map type
     */
    explicit GenericMap(const std::string& type);

    /**
     * Virtual destructor
     */
    virtual ~GenericMap() = default;

    /**
     * Get the map type
     * @return map type
     */
    const std::string& getMapType() const;

    /**
     * Apply this map to modify notes in an MSM part
     * @param msmPart the MSM part element to modify
     * @return true if any modifications were made
     */
    virtual bool applyToMsmPart(Element msmPart) = 0;

protected:
    /**
     * Parse data from XML element
     * @param xmlElement the XML element to parse
     */
    void parseData(const Element& xmlElement) override;
};

} // namespace mpm
} // namespace meico