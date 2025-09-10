#pragma once

#include "common/common.h"
#include <fstream>
#include <filesystem>

namespace meico {
namespace xml {

/**
 * This class is a primitive for all XML-based classes in meico.
 * Ported from Java XmlBase class.
 * @author Axel Berndt (original Java), C++ port
 */
class XmlBase {
protected:
    std::string file;               // the data file path
    Document data;                  // the XML document representation
    bool isValid;                   // indicates whether the input file contained valid data

public:
    /**
     * Default constructor
     */
    XmlBase();

    /**
     * Constructor with XML document
     * @param document the data as XML Document
     */
    explicit XmlBase(const Document& document);

    /**
     * Constructor with file path
     * @param filePath the data file to be read
     */
    explicit XmlBase(const std::string& filePath);

    /**
     * Constructor with file path and validation
     * @param filePath the file path
     * @param validate whether to validate
     * @param schema schema file path
     */
    XmlBase(const std::string& filePath, bool validate, const std::string& schema);

    /**
     * Constructor with XML string
     * @param xmlString xml code as UTF8 String
     * @param fromString if true, parse as XML string; if false, treat as file path
     */
    XmlBase(const std::string& xmlString, bool fromString);

    /**
     * Constructor with input stream
     * @param inputStream input stream to read from
     */
    explicit XmlBase(std::istream& inputStream);

    /**
     * Virtual destructor
     */
    virtual ~XmlBase() = default;

    /**
     * Read from file
     * @param filePath the file path
     * @param validate whether to validate
     * @param schema schema file path
     */
    void readFromFile(const std::string& filePath, bool validate = false, const std::string& schema = "");

    /**
     * Read from string
     * @param xmlString the XML string
     * @param validate whether to validate
     * @param schema schema file path
     */
    void readFromString(const std::string& xmlString, bool validate = false, const std::string& schema = "");

    /**
     * Read from input stream
     * @param inputStream the input stream
     * @param validate whether to validate
     * @param schema schema file path
     */
    void readFromInputStream(std::istream& inputStream, bool validate = false, const std::string& schema = "");

    /**
     * Check if the document is empty
     * @return true if empty
     */
    bool isEmpty() const;

    /**
     * Get the file path
     * @return file path
     */
    std::string getFile() const;

    /**
     * Set the file path
     * @param filePath the file path
     */
    void setFile(const std::string& filePath);

    /**
     * Get the document
     * @return the XML document
     */
    const Document& getDocument() const;
    Document& getDocument();

    /**
     * Set the document
     * @param document the XML document
     */
    void setDocument(const Document& document);

    /**
     * Access the root element of the document
     * @return the root element
     */
    Element getRootElement();
    const Element getRootElement() const;

    /**
     * Convert to XML string
     * @return XML string representation
     */
    std::string toXml() const;

    /**
     * Remove all elements with the specified local name from the xml tree
     * @param localName the elements to be removed
     * @return the number of elements removed
     */
    int removeAllElements(const std::string& localName);

    /**
     * Remove all attributes with the specified name from the xml tree
     * @param attributeName the attribute name
     * @return the number of deletions
     */
    int removeAllAttributes(const std::string& attributeName);

    /**
     * Write to file
     * @param filePath the output file path
     * @return true if successful
     */
    bool writeToFile(const std::string& filePath) const;

    /**
     * Check if valid
     * @return true if valid
     */
    bool getIsValid() const;

    /**
     * Set validation status
     * @param valid validation status
     */
    void setIsValid(bool valid);

protected:
    /**
     * Parse XML from result
     * @param result parse result
     */
    void parseFromResult(const pugi::xml_parse_result& result);
};

} // namespace xml
} // namespace meico