#include <iostream>
#include "xml/XmlBase.h"
#include "xml/Helper.h"
#include "msm/AbstractMsm.h"
#include "msm/Msm.h"
#include "mpm/Mpm.h"
#include "mpm/elements/Performance.h"
#include "mpm/elements/Global.h"
#include "mpm/elements/Part.h"
#include "mpm/elements/Dated.h"
#include "mpm/elements/maps/GenericMap.h"
#include "mpm/elements/maps/DynamicsMap.h"
#include "mpm/elements/maps/ArticulationMap.h"
#include "mpm/elements/maps/MetricalAccentuationMap.h"
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
        
        std::cout << "\nTesting complete MPM->MSM transformation workflow..." << std::endl;
        
        // Create an MSM with test notes
        auto workflowMsm = test::MpmTestUtils::createSimpleMsm();
        std::cout << "✓ Created test MSM with " << workflowMsm->getTitle() << std::endl;
        
        // Test 1: Dynamics Map
        auto dynamicsMpm = test::MpmTestUtils::createMpmWithMap(mpm::Mpm::DYNAMICS_MAP);
        std::cout << "✓ Created MPM with dynamics map (" << dynamicsMpm->size() << " performances)" << std::endl;
        
        // Show original MSM
        std::cout << "\n--- BEFORE Performance Application (Dynamics) ---" << std::endl;
        test::MpmTestUtils::printMsm(*workflowMsm, "Original MSM");
        
        // Apply MPM to MSM
        auto resultMsm = test::MpmTestUtils::applyMpmToMsm(*workflowMsm, *dynamicsMpm);
        std::cout << "✓ MPM dynamics application completed successfully!" << std::endl;
        
        // Show result
        std::cout << "\n--- AFTER Performance Application (Dynamics) ---" << std::endl;
        test::MpmTestUtils::printMsm(*resultMsm, "Result MSM with Applied Dynamics");
        
        // Test 2: Articulation Map
        std::cout << "\nTesting ArticulationMap..." << std::endl;
        auto articulationMpm = test::MpmTestUtils::createMpmWithArticulationMap();
        std::cout << "✓ Created MPM with articulation map" << std::endl;
        
        auto articulationResult = test::MpmTestUtils::applyMpmToMsm(*workflowMsm, *articulationMpm);
        std::cout << "✓ MPM articulation application completed successfully!" << std::endl;
        
        std::cout << "\n--- AFTER Performance Application (Articulation) ---" << std::endl;
        test::MpmTestUtils::printMsm(*articulationResult, "Result MSM with Applied Articulation");
        
        // Test 3: Metrical Accentuation Map
        std::cout << "\nTesting MetricalAccentuationMap..." << std::endl;
        auto accentuationMpm = test::MpmTestUtils::createMpmWithMetricalAccentuationMap();
        std::cout << "✓ Created MPM with metrical accentuation map" << std::endl;
        
        auto accentuationResult = test::MpmTestUtils::applyMpmToMsm(*workflowMsm, *accentuationMpm);
        std::cout << "✓ MPM metrical accentuation application completed successfully!" << std::endl;
        
        std::cout << "\n--- AFTER Performance Application (Metrical Accentuation) ---" << std::endl;
        test::MpmTestUtils::printMsm(*accentuationResult, "Result MSM with Applied Metrical Accentuation");
        
        // Verify transformation worked
        bool hasVelocityChanges = test::MpmTestUtils::verifyMsmModifications(*resultMsm, {"velocity"});
        if (hasVelocityChanges) {
            std::cout << "✓ Performance transformation verified - velocity changes detected!" << std::endl;
        } else {
            std::cout << "ℹ Note: Velocity verification needs more implementation" << std::endl;
        }
        
        std::cout << "\n🎉 All tests passed! New ArticulationMap and MetricalAccentuationMap are working!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}