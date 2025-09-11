#pragma once

#include "xml/AbstractXmlSubtree.h"
#include <memory>
#include <vector>
#include <string>

namespace meico {
namespace mpm {

// Forward declaration
class GenericMap;

/**
 * This class represents a dated container for maps in MPM.
 * Placeholder implementation for MPM Dated element.
 */
class Dated : public xml::AbstractXmlSubtree {
private:
    std::vector<std::unique_ptr<GenericMap>> maps;

public:
    /**
     * Constructor
     */
    Dated();

    /**
     * Virtual destructor
     */
    virtual ~Dated() = default;

    /**
     * Add a map to this dated container
     * @param map the map to add
     */
    void addMap(std::unique_ptr<GenericMap> map);

    /**
     * Get map by type
     * @param mapType the type of map to get
     * @return map or nullptr if not found
     */
    GenericMap* getMap(const std::string& mapType);
    const GenericMap* getMap(const std::string& mapType) const;

    /**
     * Get all maps
     * @return vector of all maps
     */
    const std::vector<std::unique_ptr<GenericMap>>& getAllMaps() const;

    /**
     * Get the number of maps
     * @return number of maps
     */
    size_t getMapCount() const;

    /**
     * Parse data from XML element
     * @param xmlElement the XML element to parse
     */
    void parseData(const Element& xmlElement) override;

protected:
};

} // namespace mpm
} // namespace meico