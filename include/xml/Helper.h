#pragma once

#include "common/common.h"
#include <regex>
#include <functional>

namespace meico {
namespace xml {

/**
 * This class contains functionality that is useful in XML-related contexts.
 * Ported from Java Helper class.
 * @author Axel Berndt (original Java), C++ port
 */
class Helper {
public:
    /**
     * Get the first child of an XML element with the specified name
     * @param parent the parent element
     * @param childName the name of the child to find
     * @return the first child element with the given name, or empty element if not found
     */
    static Element getFirstChildElement(const Element& parent, const std::string& childName);

    /**
     * Get the next sibling element with the specified name
     * @param siblingName the name of the sibling to find
     * @param current the current element
     * @return the next sibling element with the given name, or empty element if not found
     */
    static Element getNextSiblingElement(const std::string& siblingName, const Element& current);

    /**
     * Get all child elements with the specified name
     * @param parent the parent element
     * @param childName the name of the children to find
     * @return vector of child elements
     */
    static std::vector<Element> getChildElements(const Element& parent, const std::string& childName);

    /**
     * Get filename without extension
     * @param filename filename string including extension (may include the complete path)
     * @return filename/path without extension
     */
    static std::string getFilenameWithoutExtension(const std::string& filename);

    /**
     * Write string to file
     * @param content the string content
     * @param filename the filename string; should include the path and extension
     * @return true if success, false if an error occurred
     */
    static bool writeStringToFile(const std::string& content, const std::string& filename);

    /**
     * Convert string to double with error handling
     * @param str the string to convert
     * @param defaultValue default value if conversion fails
     * @return the converted double value
     */
    static double parseDouble(const std::string& str, double defaultValue = 0.0);

    /**
     * Convert string to int with error handling
     * @param str the string to convert
     * @param defaultValue default value if conversion fails
     * @return the converted int value
     */
    static int parseInt(const std::string& str, int defaultValue = 0);

    /**
     * Convert string to short with error handling
     * @param str the string to convert
     * @param defaultValue default value if conversion fails
     * @return the converted short value
     */
    static short parseShort(const std::string& str, short defaultValue = 0);

    /**
     * Check if a string represents a numeric value
     * @param str the string to check
     * @return true if numeric, false otherwise
     */
    static bool isNumeric(const std::string& str);

    /**
     * Trim whitespace from string
     * @param str the string to trim
     * @return trimmed string
     */
    static std::string trim(const std::string& str);

    /**
     * Get attribute value with default
     * @param element the element
     * @param attributeName the attribute name
     * @param defaultValue default value if attribute doesn't exist
     * @return attribute value or default
     */
    static std::string getAttributeValue(const Element& element, const std::string& attributeName, const std::string& defaultValue = "");

    /**
     * Set attribute value
     * @param element the element
     * @param attributeName the attribute name
     * @param value the value to set
     */
    static void setAttributeValue(Element& element, const std::string& attributeName, const std::string& value);

    /**
     * Remove attribute
     * @param element the element
     * @param attributeName the attribute name
     * @return true if attribute was removed
     */
    static bool removeAttribute(Element& element, const std::string& attributeName);

    /**
     * Check if element has attribute
     * @param element the element
     * @param attributeName the attribute name
     * @return true if attribute exists
     */
    static bool hasAttribute(const Element& element, const std::string& attributeName);

    /**
     * Create element with namespace
     * @param name the element name
     * @param namespaceUri the namespace URI
     * @return the created element
     */
    static Element createElement(const std::string& name, const std::string& namespaceUri = "");

    /**
     * Clone element (deep copy)
     * @param element the element to clone
     * @return cloned element
     */
    static Element cloneElement(const Element& element);

private:
    Helper() = delete; // Utility class, no instances
};

} // namespace xml
} // namespace meico