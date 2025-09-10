#pragma once

#include "msm/AbstractMsm.h"
#include <memory>

namespace meico {
namespace msm {

/**
 * This class holds data in MSM format (Musical Sequence Markup).
 * Ported from Java Msm class.
 * @author Axel Berndt (original Java), C++ port
 */
class Msm : public AbstractMsm {
private:
    static const int CONTROL_CHANGE_DENSITY = 10; // in MPM-to-MIDI export control change density limit

public:
    /**
     * Default constructor
     */
    Msm();

    /**
     * Constructor with XML document
     * @param msmDocument the MSM document
     */
    explicit Msm(const Document& msmDocument);

    /**
     * Constructor with file path
     * @param filePath the MSM file to be read
     */
    explicit Msm(const std::string& filePath);

    /**
     * Constructor with file path and validation
     * @param filePath the file path
     * @param validate whether to validate
     * @param schema schema file path
     */
    Msm(const std::string& filePath, bool validate, const std::string& schema);

    /**
     * Constructor with XML string
     * @param xmlString xml code as UTF8 String
     * @param fromString if true, parse as XML string; if false, treat as file path
     */
    Msm(const std::string& xmlString, bool fromString);

    /**
     * Constructor with input stream
     * @param inputStream input stream to read from
     */
    explicit Msm(std::istream& inputStream);

    /**
     * Virtual destructor
     */
    virtual ~Msm() = default;

    /**
     * Factory method to create an initial MSM instance with empty global maps
     * @param title the title
     * @param id an id string for the root element or empty for random UUID
     * @param ppq pulses per quarter note
     * @return new MSM instance
     */
    static std::unique_ptr<Msm> createMsm(const std::string& title, const std::string& id, int ppq);

    /**
     * Create a copy of this object
     * @return the copy of this MSM object
     */
    std::unique_ptr<Msm> clone() const;

    /**
     * Get the title of the MSM
     * @return title string
     */
    std::string getTitle() const;

    /**
     * Get the PPQ (pulses per quarter note)
     * @return PPQ value
     */
    int getPPQ() const;

    /**
     * Convert PPQ (pulses per quarter note) of this MSM
     * @param ppq the new PPQ value
     */
    void convertPPQ(int ppq);

    /**
     * Get global element
     * @return global element
     */
    Element getGlobal();
    const Element getGlobal() const;

    /**
     * Write MSM to file
     * @return true if successful
     */
    bool writeMsm();

    /**
     * Write MSM to specified file
     * @param filename the output filename
     * @return true if successful
     */
    bool writeMsm(const std::string& filename);

    /**
     * Cleanup - static utility method
     * @param msmList list of MSM objects to cleanup
     */
    static void cleanup(std::vector<std::unique_ptr<Msm>>& msmList);

    /**
     * Cleanup - static utility method
     * @param msm MSM object to cleanup
     */
    static void cleanup(std::unique_ptr<Msm>& msm);

private:
    /**
     * Generate a UUID string
     * @return UUID string
     */
    static std::string generateUUID();

    /**
     * Convert timing attributes recursively
     * @param element the element to process
     * @param scaleFactor the scale factor for conversion
     */
    void convertTimingAttributes(Element element, double scaleFactor);
};

} // namespace msm
} // namespace meico