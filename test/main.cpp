#include <iostream>
#include "xml/XmlBase.h"
#include "xml/Helper.h"
#include "msm/AbstractMsm.h"
#include "msm/Msm.h"
#include "mpm/Mpm.h"
#include "mpm/elements/Performance.h"
#include "mpm/elements/metadata/Metadata.h"
#include "mpm/MpmTestUtils.h"

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
        
        // Test MSM creation
        auto msm = msm::Msm::createMsm("Test Composition", "", 720);
        std::cout << "✓ MSM created successfully with title: " << msm->getTitle() << std::endl;
        std::cout << "✓ MSM PPQ: " << msm->getPPQ() << std::endl;
        
        // Test MSM XML output
        std::string msmXml = msm->toXml();
        if (!msmXml.empty()) {
            std::cout << "✓ MSM XML generation successful" << std::endl;
            std::cout << "MSM XML preview (first 200 chars):\n" << msmXml.substr(0, 200) << "..." << std::endl;
        }
        
        // Test MSM cloning
        auto clonedMsm = msm->clone();
        std::cout << "✓ MSM cloning successful: " << clonedMsm->getTitle() << std::endl;
        
        // Test MPM creation
        auto mpm = mpm::Mpm::createMpm();
        std::cout << "✓ MPM created successfully" << std::endl;
        std::cout << "✓ MPM has " << mpm->size() << " performances" << std::endl;
        
        // Test MpmTestUtils
        auto testMsm = test::MpmTestUtils::createSimpleMsm();
        std::cout << "✓ Test MSM created: " << testMsm->getTitle() << std::endl;
        
        // Test multi-part MSM
        auto multiPartMsm = test::MpmTestUtils::createMultiPartMsm();
        std::cout << "✓ Multi-part MSM created: " << multiPartMsm->getTitle() << std::endl;
        
        // Print a test MSM for verification
        test::MpmTestUtils::printMsm(*testMsm, "Test MSM Structure");
        
        std::cout << "\nTesting basic MPM->MSM workflow..." << std::endl;
        auto basicMpm = test::MpmTestUtils::createBasicMpm();
        auto resultMsm = test::MpmTestUtils::applyMpmToMsm(*testMsm, *basicMpm);
        std::cout << "✓ MPM application completed (basic implementation)" << std::endl;
        
        std::cout << "\nAll basic tests passed! C++ port foundation is working." << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}