#pragma once

#include "msm/AbstractMsm.h"
#include <vector>
#include <memory>

namespace meico {
namespace mpm {

// Forward declarations
class Performance;
class Metadata;

/**
 * This class holds data in MPM format (Music Performance Markup).
 * Ported from Java Mpm class.
 * @author Axel Berndt (original Java), C++ port
 */
class Mpm : public msm::AbstractMsm {
public:
    // Namespace constant
    static const std::string MPM_NAMESPACE;

    // Style type constants
    static const std::string ARTICULATION_STYLE;
    static const std::string ORNAMENTATION_STYLE;
    static const std::string DYNAMICS_STYLE;
    static const std::string METRICAL_ACCENTUATION_STYLE;
    static const std::string TEMPO_STYLE;
    static const std::string RUBATO_STYLE;

    // Map type constants
    static const std::string ARTICULATION_MAP;
    static const std::string ORNAMENTATION_MAP;
    static const std::string DYNAMICS_MAP;
    static const std::string MOVEMENT_MAP;
    static const std::string METRICAL_ACCENTUATION_MAP;
    static const std::string TEMPO_MAP;
    static const std::string RUBATO_MAP;
    static const std::string ASYNCHRONY_MAP;
    static const std::string IMPRECISION_MAP;
    static const std::string IMPRECISION_MAP_TIMING;
    static const std::string IMPRECISION_MAP_DYNAMICS;
    static const std::string IMPRECISION_MAP_TONEDURATION;
    static const std::string IMPRECISION_MAP_TUNING;

private:
    std::unique_ptr<Metadata> metadata;
    std::vector<std::unique_ptr<Performance>> performances;

public:
    /**
     * Constructor. Note: This is not a valid MPM document until a first Performance has been added!
     */
    Mpm();

    /**
     * Constructor with XML document
     * @param mpmDocument the MPM document
     */
    explicit Mpm(const Document& mpmDocument);

    /**
     * Constructor with file path
     * @param filePath the MPM file to be read
     */
    explicit Mpm(const std::string& filePath);

    /**
     * Constructor with file path and validation
     * @param filePath the file path
     * @param validate whether to validate
     * @param schema schema file path
     */
    Mpm(const std::string& filePath, bool validate, const std::string& schema);

    /**
     * Constructor with XML string
     * @param xmlString xml code as UTF8 String
     * @param fromString if true, parse as XML string; if false, treat as file path
     */
    Mpm(const std::string& xmlString, bool fromString);

    /**
     * Constructor with input stream
     * @param inputStream input stream to read from
     */
    explicit Mpm(std::istream& inputStream);

    /**
     * Virtual destructor
     */
    virtual ~Mpm() = default;

    /**
     * Factory method to create an empty MPM
     * @return new MPM instance
     */
    static std::unique_ptr<Mpm> createMpm();

    /**
     * Get the number of performances
     * @return number of performances
     */
    size_t size() const;

    /**
     * Get performance by index
     * @param index the performance index
     * @return performance or nullptr if index out of bounds
     */
    Performance* getPerformance(size_t index);
    const Performance* getPerformance(size_t index) const;

    /**
     * Add a performance
     * @param performance the performance to add
     */
    void addPerformance(std::unique_ptr<Performance> performance);

    /**
     * Get metadata
     * @return metadata or nullptr if not set
     */
    Metadata* getMetadata();
    const Metadata* getMetadata() const;

    /**
     * Set metadata
     * @param metadata the metadata to set
     */
    void setMetadata(std::unique_ptr<Metadata> metadata);

private:
    /**
     * Initialize empty MPM structure
     */
    void init();

    /**
     * Parse data from XML document
     */
    void parseData();
};

} // namespace mpm
} // namespace meico