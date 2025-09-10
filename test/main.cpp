#include <iostream>
#include "xml/XmlBase.h"
#include "xml/Helper.h"
#include "msm/AbstractMsm.h"

using namespace meico;

int main() {
    std::cout << "Testing basic C++ meico classes..." << std::endl;
    
    try {
        // Test XML creation
        xml::XmlBase xmlBase;
        std::cout << "✓ XmlBase created successfully" << std::endl;
        
        // Test simple XML string parsing
        std::string simpleXml = R"(<?xml version="1.0"?>
<root>
    <test attribute="value">Hello World</test>
</root>)";
        
        xml::XmlBase xmlFromString(simpleXml, true);
        std::cout << "✓ XML parsed from string successfully" << std::endl;
        
        // Test root element access
        Element root = xmlFromString.getRootElement();
        if (root) {
            std::cout << "✓ Root element accessed: " << root.name() << std::endl;
        }
        
        // Test helper functions
        std::string filename = "/path/to/file.ext";
        std::string withoutExt = xml::Helper::getFilenameWithoutExtension(filename);
        std::cout << "✓ Helper function test: " << withoutExt << std::endl;
        
        // Test AbstractMsm
        msm::AbstractMsm abstractMsm;
        std::cout << "✓ AbstractMsm created successfully" << std::endl;
        
        std::cout << "\nAll basic tests passed! C++ port foundation is working." << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}