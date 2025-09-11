#pragma once

#include "xml/AbstractXmlSubtree.h"
#include <memory>
#include <vector>
#include <string>

namespace meico {
namespace msm {
    class Msm; // Forward declaration
}

namespace mpm {

// Forward declarations
class Global;
class Part;
class GenericMap;

/**
 * This class represents an MPM performance.
 * One MPM document can hold several performances.
 * Ported from Java Performance class.
 * @author Axel Berndt (original Java), C++ port
 */
class Performance : public xml::AbstractXmlSubtree {
private:
    std::string name;                                    // the name of the performance
    int pulsesPerQuarter;                               // the timing resolution of symbolic time
    std::unique_ptr<Global> global;                     // the global performance information
    std::vector<std::unique_ptr<Part>> parts;          // the local performance information
    std::string xmlId;                                  // the id attribute

public:
    /**
     * Constructor from name
     * @param performanceName the name of the performance
     */
    explicit Performance(const std::string& performanceName);

    /**
     * Constructor from XML element
     * @param xml the XML element
     */
    explicit Performance(const Element& xml);

    /**
     * Virtual destructor
     */
    virtual ~Performance() = default;

    /**
     * Performance factory method
     * @param performanceName the name of the performance
     * @return new Performance instance
     */
    static std::unique_ptr<Performance> createPerformance(const std::string& performanceName);

    /**
     * Get the name of the performance
     * @return performance name
     */
    const std::string& getName() const;

    /**
     * Set the name of the performance
     * @param performanceName the new name
     */
    void setName(const std::string& performanceName);

    /**
     * Get the PPQ (pulses per quarter note)
     * @return PPQ value
     */
    int getPPQ() const;

    /**
     * Set the PPQ (pulses per quarter note)
     * @param ppq the new PPQ value
     */
    void setPPQ(int ppq);

    /**
     * Get the global performance data
     * @return global data or nullptr
     */
    Global* getGlobal();
    const Global* getGlobal() const;

    /**
     * Get the number of parts
     * @return number of parts
     */
    size_t getPartCount() const;

    /**
     * Get part by index
     * @param index the part index
     * @return part or nullptr if index out of bounds
     */
    Part* getPart(size_t index);
    const Part* getPart(size_t index) const;

    /**
     * Add a part
     * @param part the part to add
     */
    void addPart(std::unique_ptr<Part> part);

    /**
     * Apply this performance to an MSM and return the result
     * @param msm the input MSM
     * @return the modified MSM
     */
    std::unique_ptr<msm::Msm> perform(const msm::Msm& msm) const;

    /**
     * Parse data from XML element (from AbstractXmlSubtree)
     * @param xmlElement the XML element to parse
     */
    void parseData(const Element& xmlElement) override;

protected:

private:
    /**
     * Initialize basic performance structure
     */
    void init();

    /**
     * Apply maps to an MSM part
     * @param msmPart the MSM part element
     * @param maps the maps to apply
     */
    void applyMapsToMsmPart(Element msmPart, const std::vector<std::unique_ptr<GenericMap>>& maps) const;
    
    /**
     * Add performance timing attributes to all elements in a dated section (like Java addPerformanceTimingAttributes)
     * @param dated the dated element
     */
    void addPerformanceTimingAttributesToDated(Element dated) const;
    
    /**
     * Apply performance transformations to an MSM part
     * @param msmPart the MSM part element
     * @param dated the dated element within the part
     */
    void applyPerformanceTransformations(Element msmPart, Element dated) const;
    
    /**
     * Apply basic timing transformation to a score element
     * @param score the score element containing notes
     */
    void applyBasicTimingTransformation(Element score) const;
};

} // namespace mpm
} // namespace meico