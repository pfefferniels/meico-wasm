#pragma once

#include "xml/XmlBase.h"

namespace meico {
namespace msm {

/**
 * This class is a primitive for Msm and Mpm.
 * Ported from Java AbstractMsm class.
 * @author Axel Berndt (original Java), C++ port
 */
class AbstractMsm : public xml::XmlBase {
public:
    /**
     * Default constructor
     */
    AbstractMsm();

    /**
     * Constructor with XML document
     * @param document the data as XML Document
     */
    explicit AbstractMsm(const Document& document);

    /**
     * Constructor with file path
     * @param filePath the data file to be read
     */
    explicit AbstractMsm(const std::string& filePath);

    /**
     * Constructor with file path and validation
     * @param filePath the file path
     * @param validate whether to validate
     * @param schema schema file path
     */
    AbstractMsm(const std::string& filePath, bool validate, const std::string& schema);

    /**
     * Constructor with XML string
     * @param xmlString xml code as UTF8 String
     */
    explicit AbstractMsm(const std::string& xmlString, bool fromString);

    /**
     * Constructor with input stream
     * @param inputStream input stream to read from
     */
    explicit AbstractMsm(std::istream& inputStream);

    /**
     * Virtual destructor
     */
    virtual ~AbstractMsm() = default;
};

} // namespace msm
} // namespace meico