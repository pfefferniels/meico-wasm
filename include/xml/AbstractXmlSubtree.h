#pragma once

#include "common/common.h"

namespace meico {
namespace xml {

/**
 * This is the prototype for classes that occur within the Mpm data structure.
 * Ported from Java AbstractXmlSubtree class.
 * @author Axel Berndt (original Java), C++ port
 */
class AbstractXmlSubtree {
protected:
    Element xml;   // the actual xml data

public:
    /**
     * Default constructor
     */
    AbstractXmlSubtree() = default;

    /**
     * Virtual destructor
     */
    virtual ~AbstractXmlSubtree() = default;

    /**
     * A getter for the XML Element representation
     * @return the XML element
     */
    Element getXml() const;

    /**
     * Convert to XML string
     * @return String with the XML code
     */
    std::string toXml() const;

protected:
    /**
     * This sets the xml data
     * @param xmlElement the XML element
     */
    void setXml(const Element& xmlElement);

    /**
     * Set the data of this object, this parses the xml element and generates the according data structure
     * @param xmlElement the XML element to parse
     */
    virtual void parseData(const Element& xmlElement) = 0;
};

} // namespace xml
} // namespace meico