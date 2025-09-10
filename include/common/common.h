#pragma once

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>

// Use pugixml for XML processing
#include <pugixml.hpp>

namespace meico {
    using Element = pugi::xml_node;
    using Document = pugi::xml_document;
    using Attribute = pugi::xml_attribute;

    // Exception classes
    class IOException : public std::runtime_error {
    public:
        IOException(const std::string& msg) : std::runtime_error(msg) {}
    };

    class ParsingException : public std::runtime_error {
    public:
        ParsingException(const std::string& msg) : std::runtime_error(msg) {}
    };

    class SAXException : public std::runtime_error {
    public:
        SAXException(const std::string& msg) : std::runtime_error(msg) {}
    };

    class ParserConfigurationException : public std::runtime_error {
    public:
        ParserConfigurationException(const std::string& msg) : std::runtime_error(msg) {}
    };

    // Helper functions
    inline std::string getFilenameWithoutExtension(const std::string& filename) {
        size_t lastDot = filename.find_last_of('.');
        if (lastDot == std::string::npos || lastDot == 0) {
            return filename;
        }
        return filename.substr(0, lastDot);
    }

    inline bool writeStringToFile(const std::string& content, const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        file << content;
        return file.good();
    }
}