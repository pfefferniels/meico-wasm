#pragma once

#include "mpm/elements/maps/GenericMap.h"
#include "supplementary/KeyValue.h"
#include <memory>
#include <vector>

namespace meico {
namespace mpm {

/**
 * Simple data structure for asynchrony information
 */
struct AsynchronyData {
    double date;
    double millisecondsOffset;
    
    AsynchronyData(double d, double offset) : date(d), millisecondsOffset(offset) {}
};

/**
 * This class interfaces MPM's asynchronyMaps
 * @author Axel Berndt (Java), Copilot (C++ port)
 */
class AsynchronyMap : public GenericMap {
private:
    std::vector<supplementary::KeyValue<double, std::unique_ptr<AsynchronyData>>> asynchronyData;

public:
    /**
     * Constructor
     */
    AsynchronyMap();

    /**
     * Virtual destructor
     */
    virtual ~AsynchronyMap() = default;

    /**
     * AsynchronyMap factory
     * @return
     */
    static std::unique_ptr<AsynchronyMap> createAsynchronyMap();

    /**
     * AsynchronyMap factory
     * @param xml
     * @return
     */
    static std::unique_ptr<AsynchronyMap> createAsynchronyMap(const Element& xml);

    /**
     * add an asynchrony element to the map
     * @param date
     * @param millisecondsOffset
     * @return the index at which it has been inserted
     */
    int addAsynchrony(double date, double millisecondsOffset);

    /**
     * get the asynchrony, i.e. the milliseconds offset, at the specified date
     * @param date
     * @return
     */
    double getAsynchronyAt(double date) const;

    /**
     * Apply this asynchrony map to modify notes in an MSM part
     * @param msmPart the MSM part element to modify
     * @return true if any modifications were made
     */
    bool applyToMsmPart(Element msmPart) override;

    /**
     * on the basis of this asynchronyMap, add the corresponding offsets to the millisecond.date and millisecond.date.end attributes of each map element
     * @param map This map's elements must have attributes millisecond.date and millisecond.date.end! This means, it must have been processed by TempoMap.addMillisecondsToMap() before it can be processed here.
     */
    void renderAsynchronyToMap(GenericMap& map);

    /**
     * on the basis of the specified asynchronyMap, add the corresponding offsets to the millisecond.date and millisecond.date.end attributes of each map element
     * @param map This map's elements must have attributes millisecond.date and millisecond.date.end! This means, it must have been processed by TempoMap.addMillisecondsToMap() before it can be processed here.
     * @param asynchronyMap
     */
    static void renderAsynchronyToMap(GenericMap& map, AsynchronyMap* asynchronyMap);

protected:
    /**
     * Parse data from XML element
     * @param xmlElement the XML element to parse
     */
    void parseData(const Element& xmlElement) override;

private:
    /**
     * Get the index of the asynchrony element before or at the given date
     * @param date the musical time
     * @return index or -1 if none found
     */
    int getElementIndexBeforeAt(double date) const;
};

} // namespace mpm
} // namespace meico