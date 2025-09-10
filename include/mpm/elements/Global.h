#pragma once

#include "xml/AbstractXmlSubtree.h"
#include <memory>

namespace meico {
namespace mpm {

// Forward declaration
class Dated;

/**
 * This class represents global performance information.
 * Placeholder implementation for MPM Global element.
 */
class Global : public xml::AbstractXmlSubtree {
private:
    std::unique_ptr<Dated> dated;

public:
    /**
     * Constructor
     */
    Global();

    /**
     * Virtual destructor
     */
    virtual ~Global() = default;

    /**
     * Get the dated container
     * @return dated container or nullptr
     */
    Dated* getDated();
    const Dated* getDated() const;

    /**
     * Set the dated container
     * @param newDated the new dated container
     */
    void setDated(std::unique_ptr<Dated> newDated);

protected:
    /**
     * Parse data from XML element
     * @param xmlElement the XML element to parse
     */
    void parseData(const Element& xmlElement) override;
};

} // namespace mpm
} // namespace meico