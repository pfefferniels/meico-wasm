#include <iostream>
#include "xml/XmlBase.h"
#include "xml/Helper.h"
#include "msm/Msm.h"
#include "mpm/Mpm.h"
#include "mpm/elements/Performance.h"
#include "mpm/elements/Global.h"
#include "mpm/elements/Part.h"
#include "mpm/elements/Dated.h"
#include "mpm/elements/maps/GenericMap.h"
#include "mpm/elements/metadata/Metadata.h"

using namespace meico;

int main() {
    std::cout << "Testing with real .mpm and .msm files..." << std::endl;
    
    try {
        // Load the MSM file
        std::cout << "Loading MSM file: test/bwv1007.msm" << std::endl;
        auto msm = std::make_unique<msm::Msm>("test/bwv1007.msm");
        std::cout << "✓ MSM loaded successfully" << std::endl;
        std::cout << "  Title: " << msm->getTitle() << std::endl;
        std::cout << "  PPQ: " << msm->getPPQ() << std::endl;
        
        // Load the MPM file
        std::cout << "\nLoading MPM file: test/bwv1007.mpm" << std::endl;
        auto mpm = std::make_unique<mpm::Mpm>("test/bwv1007.mpm");
        std::cout << "✓ MPM loaded successfully" << std::endl;
        std::cout << "  Number of performances: " << mpm->size() << std::endl;
        
        if (mpm->size() > 0) {
            auto* performance = mpm->getPerformance(0);
            if (performance) {
                std::cout << "  Performance name: " << performance->getName() << std::endl;
                std::cout << "  Performance PPQ: " << performance->getPPQ() << std::endl;
                
                // Apply the performance to the MSM
                std::cout << "\nApplying MPM performance to MSM..." << std::endl;
                auto resultMsm = performance->perform(*msm);
                if (resultMsm) {
                    std::cout << "✓ Performance application completed successfully!" << std::endl;
                    std::cout << "  Result title: " << resultMsm->getTitle() << std::endl;
                    
                    // Write result to file for comparison
                    std::string resultXml = resultMsm->toXml();
                    if (!resultXml.empty()) {
                        std::cout << "✓ Generated result XML successfully" << std::endl;
                        std::cout << "  XML length: " << resultXml.length() << " characters" << std::endl;
                        
                        // Save result to file for comparison with Java implementation
                        std::string outputFile = "test/bwv1007_cpp_result.msm";
                        if (xml::Helper::writeStringToFile(resultXml, outputFile)) {
                            std::cout << "✓ Saved C++ result to: " << outputFile << std::endl;
                        } else {
                            std::cout << "⚠ Could not save C++ result to file" << std::endl;
                        }
                    }
                } else {
                    std::cout << "✗ Performance application failed" << std::endl;
                }
            } else {
                std::cout << "✗ No performance found" << std::endl;
            }
        } else {
            std::cout << "✗ No performances found in MPM" << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}