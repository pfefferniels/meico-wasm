#include "xml/Helper.h"
#include <fstream>
#include <algorithm>
#include <cctype>
#include <limits>

namespace meico {
namespace xml {

Element Helper::getFirstChildElement(const Element& parent, const std::string& childName) {
    if (!parent) {
        return Element();
    }
    
    for (auto child : parent.children()) {
        if (child.name() == childName) {
            return child;
        }
    }
    return Element();
}

Element Helper::getNextSiblingElement(const std::string& siblingName, const Element& current) {
    if (!current) {
        return Element();
    }
    
    Element sibling = current.next_sibling();
    while (sibling && sibling.name() != siblingName) {
        sibling = sibling.next_sibling();
    }
    return sibling;
}

std::vector<Element> Helper::getChildElements(const Element& parent, const std::string& childName) {
    std::vector<Element> children;
    if (!parent) {
        return children;
    }
    
    for (auto child : parent.children()) {
        if (child.name() == childName) {
            children.push_back(child);
        }
    }
    return children;
}

std::string Helper::getFilenameWithoutExtension(const std::string& filename) {
    size_t lastDot = filename.find_last_of('.');
    if (lastDot == std::string::npos || lastDot == 0) {
        return filename;
    }
    return filename.substr(0, lastDot);
}

bool Helper::writeStringToFile(const std::string& content, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    file << content;
    return file.good();
}

double Helper::parseDouble(const std::string& str, double defaultValue) {
    try {
        return std::stod(str);
    } catch (const std::exception&) {
        return defaultValue;
    }
}

int Helper::parseInt(const std::string& str, int defaultValue) {
    try {
        return std::stoi(str);
    } catch (const std::exception&) {
        return defaultValue;
    }
}

short Helper::parseShort(const std::string& str, short defaultValue) {
    try {
        int value = std::stoi(str);
        if (value >= std::numeric_limits<short>::min() && value <= std::numeric_limits<short>::max()) {
            return static_cast<short>(value);
        }
        return defaultValue;
    } catch (const std::exception&) {
        return defaultValue;
    }
}

bool Helper::isNumeric(const std::string& str) {
    if (str.empty()) {
        return false;
    }
    
    char* end = nullptr;
    std::strtod(str.c_str(), &end);
    return end == str.c_str() + str.size();
}

std::string Helper::trim(const std::string& str) {
    auto start = str.begin();
    while (start != str.end() && std::isspace(*start)) {
        start++;
    }
    
    auto end = str.end();
    do {
        end--;
    } while (std::distance(start, end) > 0 && std::isspace(*end));
    
    return std::string(start, end + 1);
}

std::string Helper::getAttributeValue(const Element& element, const std::string& attributeName, const std::string& defaultValue) {
    if (!element) {
        return defaultValue;
    }
    
    auto attr = element.attribute(attributeName.c_str());
    if (attr) {
        return attr.value();
    }
    return defaultValue;
}

void Helper::setAttributeValue(Element& element, const std::string& attributeName, const std::string& value) {
    if (element) {
        element.remove_attribute(attributeName.c_str());
        element.append_attribute(attributeName.c_str()) = value.c_str();
    }
}

bool Helper::removeAttribute(Element& element, const std::string& attributeName) {
    if (element) {
        return element.remove_attribute(attributeName.c_str());
    }
    return false;
}

bool Helper::hasAttribute(const Element& element, const std::string& attributeName) {
    if (!element) {
        return false;
    }
    return element.attribute(attributeName.c_str()) != nullptr;
}

Element Helper::createElement(const std::string& name, const std::string& namespaceUri) {
    // Note: pugixml doesn't have direct namespace support like Java XOM
    // For simplicity, we'll just create elements with names
    // In a more complete implementation, we'd need to handle namespaces properly
    pugi::xml_document tempDoc;
    return tempDoc.append_child(name.c_str());
}

Element Helper::cloneElement(const Element& element) {
    pugi::xml_document tempDoc;
    return tempDoc.append_copy(element);
}

} // namespace xml
} // namespace meico