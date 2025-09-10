#include "xml/XmlBase.h"
#include <sstream>
#include <functional>

namespace meico {
namespace xml {

XmlBase::XmlBase() : file(""), isValid(false) {
}

XmlBase::XmlBase(const Document& document) : file(""), isValid(false) {
    setDocument(document);
}

XmlBase::XmlBase(const std::string& filePath) : file(filePath), isValid(false) {
    readFromFile(filePath, false, "");
}

XmlBase::XmlBase(const std::string& filePath, bool validate, const std::string& schema) 
    : file(filePath), isValid(false) {
    readFromFile(filePath, validate, schema);
}

XmlBase::XmlBase(const std::string& xmlString, bool fromString) : file(""), isValid(false) {
    if (fromString) {
        readFromString(xmlString, false, "");
    } else {
        // Fallback to file path constructor behavior
        file = xmlString;
        readFromFile(xmlString, false, "");
    }
}

XmlBase::XmlBase(std::istream& inputStream) : file(""), isValid(false) {
    readFromInputStream(inputStream, false, "");
}

void XmlBase::readFromFile(const std::string& filePath, bool validate, const std::string& schema) {
    this->file = filePath;
    
    pugi::xml_parse_result result = data.load_file(filePath.c_str());
    if (!result) {
        throw ParsingException("Failed to parse XML file: " + std::string(result.description()));
    }
    
    parseFromResult(result);
    
    // Note: Schema validation not implemented in this basic version
    // Would need libxml2 or similar for full schema validation
    if (validate && !schema.empty()) {
        std::cout << "Warning: Schema validation not implemented in this basic version\n";
    }
}

void XmlBase::readFromString(const std::string& xmlString, bool validate, const std::string& schema) {
    pugi::xml_parse_result result = data.load_string(xmlString.c_str());
    if (!result) {
        throw ParsingException("Failed to parse XML string: " + std::string(result.description()));
    }
    
    parseFromResult(result);
    
    if (validate && !schema.empty()) {
        std::cout << "Warning: Schema validation not implemented in this basic version\n";
    }
}

void XmlBase::readFromInputStream(std::istream& inputStream, bool validate, const std::string& schema) {
    std::string xmlString((std::istreambuf_iterator<char>(inputStream)), std::istreambuf_iterator<char>());
    readFromString(xmlString, validate, schema);
}

bool XmlBase::isEmpty() const {
    return data.first_child().empty();
}

std::string XmlBase::getFile() const {
    return file;
}

void XmlBase::setFile(const std::string& filePath) {
    this->file = filePath;
}

const Document& XmlBase::getDocument() const {
    return data;
}

Document& XmlBase::getDocument() {
    return data;
}

void XmlBase::setDocument(const Document& document) {
    // Deep copy the document
    data.reset();
    for (auto node : document.children()) {
        data.append_copy(node);
    }
}

Element XmlBase::getRootElement() {
    if (isEmpty()) {
        return Element();
    }
    return data.first_child();
}

const Element XmlBase::getRootElement() const {
    if (isEmpty()) {
        return Element();
    }
    return data.first_child();
}

std::string XmlBase::toXml() const {
    if (isEmpty()) {
        return "";
    }
    
    std::ostringstream oss;
    data.save(oss);
    return oss.str();
}

int XmlBase::removeAllElements(const std::string& localName) {
    int count = 0;
    Element root = getRootElement();
    if (!root) {
        return count;
    }
    
    // Use XPath-like query to find all elements with the given name
    auto nodes = root.select_nodes(("//" + localName).c_str());
    for (auto& node : nodes) {
        if (node.node().parent()) {
            node.node().parent().remove_child(node.node());
            count++;
        }
    }
    
    return count;
}

int XmlBase::removeAllAttributes(const std::string& attributeName) {
    int count = 0;
    Element root = getRootElement();
    if (!root) {
        return count;
    }
    
    // Recursively remove attributes from all nodes
    std::function<void(Element)> removeAttrRecursive = [&](Element node) {
        if (node.remove_attribute(attributeName.c_str())) {
            count++;
        }
        for (auto child : node.children()) {
            removeAttrRecursive(child);
        }
    };
    
    removeAttrRecursive(root);
    return count;
}

bool XmlBase::writeToFile(const std::string& filePath) const {
    if (isEmpty()) {
        return false;
    }
    
    return data.save_file(filePath.c_str());
}

bool XmlBase::getIsValid() const {
    return isValid;
}

void XmlBase::setIsValid(bool valid) {
    this->isValid = valid;
}

void XmlBase::parseFromResult(const pugi::xml_parse_result& result) {
    if (result) {
        isValid = true;
    } else {
        isValid = false;
        throw ParsingException("XML parsing failed: " + std::string(result.description()));
    }
}

} // namespace xml
} // namespace meico