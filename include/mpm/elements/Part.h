#pragma once

#include "xml/AbstractXmlSubtree.h"
#include <memory>
#include <string>

namespace meico {
namespace mpm {

// Forward declaration
class Dated;

/**
 * This class represents a part in MPM performance.
 * Placeholder implementation for MPM Part element.
 */
class Part : public xml::AbstractXmlSubtree {
private:
    std::string name;
    int number;
    int midiChannel;
    int midiPort;
    std::unique_ptr<Dated> dated;

public:
    /**
     * Constructor
     * @param partName the name of the part
     * @param partNumber the part number
     * @param channel the MIDI channel
     * @param port the MIDI port
     */
    Part(const std::string& partName, int partNumber, int channel, int port);

    /**
     * Virtual destructor
     */
    virtual ~Part() = default;

    /**
     * Factory method to create a part
     * @param partName the name of the part
     * @param partNumber the part number
     * @param channel the MIDI channel
     * @param port the MIDI port
     * @return new Part instance
     */
    static std::unique_ptr<Part> createPart(const std::string& partName, int partNumber, int channel, int port);

    /**
     * Get the part name
     * @return part name
     */
    const std::string& getName() const;

    /**
     * Get the part number
     * @return part number
     */
    int getNumber() const;

    /**
     * Get the MIDI channel
     * @return MIDI channel
     */
    int getMidiChannel() const;

    /**
     * Get the MIDI port
     * @return MIDI port
     */
    int getMidiPort() const;

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