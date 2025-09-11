#pragma once

#include "xml/AbstractXmlSubtree.h"
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

namespace meico {
namespace mpm {

// Forward declarations
class GenericMap;
class Global;
class Part;

/**
 * This class interfaces MPM's dated environment.
 * Ported from Java Dated class.
 * @author Axel Berndt (original Java), C++ port
 */
class Dated : public xml::AbstractXmlSubtree {
private:
    std::unordered_map<std::string, std::unique_ptr<GenericMap>> maps;  // maps by type like Java HashMap
    Global* global = nullptr;                                           // link to the global environment
    Part* part = nullptr;                                               // link to the local environment, leave null if this is a global dated environment

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
     * @return the added map or nullptr if failed
     */
    GenericMap* addMap(std::unique_ptr<GenericMap> map);

    /**
     * Generate a new map from XML element and add it to the dated environment
     * @param xml the XML element
     * @return the created map or nullptr if failed
     */
    GenericMap* addMap(const Element& xml);

    /**
     * Generate a new empty map with the specified type and add it to the dated environment  
     * @param type the map type
     * @return the created map or nullptr if failed
     */
    GenericMap* addMap(const std::string& type);

    /**
     * Remove the map of the specified type from the dated environment
     * @param type the map type
     */
    void removeMap(const std::string& type);

    /**
     * Remove all maps from dated
     */
    void clear();

    /**
     * Get map by type
     * @param mapType the type of map to get
     * @return map or nullptr if not found
     */
    GenericMap* getMap(const std::string& mapType);
    const GenericMap* getMap(const std::string& mapType) const;

    /**
     * Get all maps (returns a vector for compatibility, but internally uses map)
     * @return vector of all maps
     */
    std::vector<std::unique_ptr<GenericMap>> getAllMaps() const;

    /**
     * Get all maps as the internal hash map
     * @return unordered_map of all maps
     */
    const std::unordered_map<std::string, std::unique_ptr<GenericMap>>& getAllMapsMap() const;

    /**
     * Get the number of maps
     * @return number of maps
     */
    size_t getMapCount() const;

    /**
     * Set the global and part environment of the dated data (like Java)
     * @param globalEnv the global environment
     * @param partEnv the part environment
     */
    void setEnvironment(Global* globalEnv, Part* partEnv);

    /**
     * Get this dated's link to the global environment
     * @return global environment or nullptr
     */
    Global* getGlobal();
    const Global* getGlobal() const;

    /**
     * Get this dated's link to the local environment  
     * @return part environment or nullptr
     */
    Part* getPart();
    const Part* getPart() const;

    /**
     * Parse data from XML element (following Java implementation exactly)
     * @param xmlElement the XML element to parse
     */
    void parseData(const Element& xmlElement) override;

protected:
};

} // namespace mpm
} // namespace meico