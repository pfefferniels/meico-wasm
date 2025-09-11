#pragma once

#include "xml/AbstractXmlSubtree.h"
#include <memory>
#include <string>

namespace meico {
namespace mpm {

// Forward declarations
class Dated;
class Global;
// class Header;  // Commented out for now

/**
 * This class interfaces the part-specific information, as opposed to global information that apply to all parts.
 * Ported from Java Part class.
 * @author Axel Berndt (original Java), C++ port
 */
class Part : public xml::AbstractXmlSubtree {
private:
    Global* global = nullptr;               // a link to the global environment
    // std::unique_ptr<Header> header;         // the header environment (commented out for now)
    std::unique_ptr<Dated> dated;           // the dated environment
    
    std::string name;                       // the name of the part
    int number = 0;                         // the part number
    int midiChannel = 0;                    // the midi channel
    int midiPort = 0;                       // the midi port
    std::string xmlId;                      // the id attribute

public:
    /**
     * Constructor from XML element
     * @param xml the XML element
     */
    explicit Part(const Element& xml);

    /**
     * Virtual destructor
     */
    virtual ~Part() = default;

    /**
     * Factory method to create a part from XML
     * @param xml the XML element
     * @return new Part instance
     */
    static std::unique_ptr<Part> createPart(const Element& xml);

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
     * Get the header container
     * @return header container or nullptr
     */
    // Header* getHeader();
    // const Header* getHeader() const;

    /**
     * Set the global environment link
     * @param globalEnv the global environment
     */
    void setGlobal(Global* globalEnv);

    /**
     * Get the global environment link
     * @return global environment or nullptr
     */
    Global* getGlobal();
    const Global* getGlobal() const;

    /**
     * Parse data from XML element (following Java implementation exactly)
     * @param xmlElement the XML element to parse
     */
    void parseData(const Element& xmlElement) override;

protected:
};

} // namespace mpm
} // namespace meico