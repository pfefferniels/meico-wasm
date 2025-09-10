#include <iostream>
#include <memory>
#include <iomanip>
#include "xml/XmlBase.h"
#include "msm/Msm.h"
#include "mpm/Mpm.h"
#include "mpm/elements/Performance.h"
#include "mpm/elements/Global.h"
#include "mpm/elements/Part.h"
#include "mpm/elements/Dated.h"
#include "mpm/elements/maps/DynamicsMap.h"
#include "mpm/elements/maps/ArticulationMap.h"
#include "mpm/elements/maps/MetricalAccentuationMap.h"
#include "mpm/elements/maps/TempoMap.h"
#include "mpm/elements/maps/RubatoMap.h"
#include "mpm/elements/maps/OrnamentationMap.h"
#include "mpm/elements/maps/MovementMap.h"
#include "mpm/elements/maps/AsynchronyMap.h"
#include "mpm/elements/maps/ImprecisionMap.h"
#include "mpm/elements/maps/data/DistributionData.h"
#include "mpm/elements/metadata/Metadata.h"

using namespace meico;

int main() {
    std::cout << "Comprehensive test of C++ implementation against Java behavior..." << std::endl;
    
    try {
        // Create the Bach MSM with exact structure from user comment
        std::string bachMsmXml = R"(<?xml version="1.0"?>
<msm title="J. S. Bach Cello Suite BWV 1007 Minuet No 2" xml:id="ef510daa-73b7-46be-bd64-ce72cf21fae9" pulsesperquarter="480">
    <global>
        <header>
        <dated>
            <timesignaturemap>
                <timesignature date="0.0" numerator="3.0" denominator="4">
            </timesignature></timesignaturemap>
            <keysignaturemap>
                <keysignature date="0.0">
                    <accidental midi.pitch="11.0" pitchname="B" value="-1.0">
                </accidental></keysignature>
            </keysignaturemap>
        </dated>
    </header></global>
    <part name="Cello" number="0" midi.channel="0" midi.port="0">
        <header>
        <dated>
            <score>
                <note date="0.0" midi.pitch="58.0" pitchname="B" accidentals="-1.0" duration="240.0" velocity="80.0" xml:id="meico_e0ce30cc-e00e-4cc1-9c43-fea3725a80ef">
                <note date="240.0" midi.pitch="57.0" pitchname="A" accidentals="0.0" duration="240.0" velocity="80.0" xml:id="meico_4b9aa215-cbc9-4009-a652-93dea391ad0a">
                <note date="480.0" midi.pitch="58.0" pitchname="B" accidentals="-1.0" duration="240.0" velocity="80.0" xml:id="meico_af007aa5-8bfd-42bb-a3cd-4ffbc48895b3">
                <note date="720.0" midi.pitch="50.0" pitchname="D" accidentals="0.0" duration="240.0" velocity="80.0" xml:id="meico_5fd2c151-37b9-44d8-b6b5-6228ef9a56b0">
                <note date="960.0" midi.pitch="51.0" pitchname="E" accidentals="-1.0" duration="240.0" velocity="80.0" xml:id="meico_91cafb28-54b7-44e8-a3b5-9183897224a1">
                

</note></note></note></note></note></score></dated></header></part></msm>)";

        // Parse the MSM
        auto bachMsm = std::make_unique<msm::Msm>(bachMsmXml, true);
        std::cout << "✓ Parsed Bach MSM: " << bachMsm->getTitle() << std::endl;
        std::cout << "  PPQ: " << bachMsm->getPPQ() << std::endl;
        
        // Test 1: DynamicsMap with exact Java algorithm validation
        std::cout << "\n=== TESTING DYNAMICSMAP ALGORITHM ===\n" << std::endl;
        
        auto dynamicsMap = mpm::DynamicsMap::createDynamicsMap();
        // Create crescendo from p (40) to f (100) with curvature 0.5 (S-curve)
        dynamicsMap->addDynamics(0.0, "40", "100", 0.5, 0.0);
        dynamicsMap->addDynamics(960.0, "100", "", 0.0, 0.0);
        
        // Test dynamics at specific points to validate Bézier curve
        std::cout << "Testing getDynamicsAt() method with Bézier curves:" << std::endl;
        std::cout << std::fixed << std::setprecision(6);
        
        double dates[] = {0.0, 240.0, 480.0, 720.0, 960.0};
        for (double date : dates) {
            double dynamics = dynamicsMap->getDynamicsAt(date);
            std::cout << "  Date " << date << ": " << dynamics << std::endl;
        }
        
        // Expected Java behavior (from PR description):
        // 40.0→46.3→70.0→93.7→100.0 (perfect S-curve effect)
        std::cout << "\nExpected Java behavior: 40.0→46.3→70.0→93.7→100.0" << std::endl;
        
        // Test 2: ArticulationMap with comprehensive attributes
        std::cout << "\n=== TESTING ARTICULATIONMAP ALGORITHM ===\n" << std::endl;
        
        auto articulationMap = mpm::ArticulationMap::createArticulationMap();
        
        // Test staccato (duration×0.5, velocity×1.2)
        mpm::ArticulationData staccatoData;
        staccatoData.date = 0.0;
        staccatoData.relativeDuration = 0.5;  // 50% duration
        staccatoData.relativeVelocity = 1.2;  // 120% velocity
        staccatoData.xmlId = "staccato_test";
        articulationMap->addArticulation(staccatoData);
        
        // Test legato (duration×1.2, velocity×0.9)
        mpm::ArticulationData legatoData;
        legatoData.date = 240.0;
        legatoData.relativeDuration = 1.2;  // 120% duration
        legatoData.relativeVelocity = 0.9;  // 90% velocity
        legatoData.xmlId = "legato_test";
        articulationMap->addArticulation(legatoData);
        
        std::cout << "Created ArticulationMap with staccato and legato." << std::endl;
        
        // Test 3: TempoMap with power curve transitions
        std::cout << "\n=== TESTING TEMPOMAP ALGORITHM ===\n" << std::endl;
        
        auto tempoMap = mpm::TempoMap::createTempoMap();
        // Test tempo transition from 120→140 BPM with meanTempoAt=0.5
        tempoMap->addTempo(0.0, 120.0, 0.25);
        tempoMap->addTempo(480.0, "120", "140", 0.25, 0.5, "accel_test");
        tempoMap->addTempo(960.0, 140.0, 0.25);
        
        std::cout << "Testing getTempoAt() method with power curve transitions:" << std::endl;
        for (double date = 0.0; date <= 960.0; date += 120.0) {
            double tempo = tempoMap->getTempoAt(date);
            std::cout << "  Date " << date << ": " << tempo << " BPM" << std::endl;
        }
        
        // Expected Java behavior (from PR description):
        // 120→125→130→135→140 BPM (S-curve progression)
        std::cout << "\nExpected Java behavior: 120→125→130→135→140 BPM" << std::endl;
        
        // Test 4: RubatoMap with power function
        std::cout << "\n=== TESTING RUBATOMAP ALGORITHM ===\n" << std::endl;
        
        auto rubatoMap = mpm::RubatoMap::createRubatoMap();
        // Use the correct parameters that produce expected Java behavior
        rubatoMap->addRubato(0.0, 480.0, 1.2, 0.1, 0.9, false, "rubato_test");
        
        std::cout << "Testing rubato data retrieval:" << std::endl;
        for (double date = 120.0; date < 480.0; date += 120.0) {
            auto rubatoData = rubatoMap->getRubatoDataAt(date);
            if (rubatoData) {
                double transformed = mpm::RubatoMap::computeRubatoTransformation(date, *rubatoData);
                std::cout << "  Date " << date << " → " << transformed << std::endl;
            }
        }
        
        // Expected Java behavior (from PR description):
        // 120→120.754, 240→215.146, 360→319.897
        std::cout << "\nExpected Java behavior: 120→120.754, 240→215.146, 360→319.897" << std::endl;
        
        // Test 5: MovementMap with Bézier curves
        std::cout << "\n=== TESTING MOVEMENTMAP ALGORITHM ===\n" << std::endl;
        
        auto movementMap = mpm::MovementMap::createMovementMap();
        movementMap->addMovement(0.0, "sustain", 0.0, 1.0, 0.5, 0.0, "movement_test");
        
        std::cout << "Testing movement data retrieval:" << std::endl;
        for (double t = 0.25; t < 1.0; t += 0.25) {
            auto movementData = movementMap->getMovementDataAt(t * 960.0);
            if (movementData) {
                std::cout << "  t=" << t << ": Movement data found" << std::endl;
            }
        }
        
        // Expected Java behavior (from PR description):
        // 0.105127→0.5→0.894873 (S-curve progression)
        std::cout << "\nExpected Java behavior: 0.105127→0.5→0.894873" << std::endl;
        
        // Test 6: AsynchronyMap
        std::cout << "\n=== TESTING ASYNCHRONYMAP ALGORITHM ===\n" << std::endl;
        
        auto asynchronyMap = mpm::AsynchronyMap::createAsynchronyMap();
        asynchronyMap->addAsynchrony(0.0, 50.0);
        asynchronyMap->addAsynchrony(240.0, -25.0);
        asynchronyMap->addAsynchrony(480.0, 0.0);
        
        std::cout << "Testing asynchrony offset retrieval:" << std::endl;
        for (double date = 0.0; date <= 480.0; date += 240.0) {
            double offset = asynchronyMap->getAsynchronyAt(date);
            std::cout << "  Date " << date << ": " << offset << "ms" << std::endl;
        }
        
        // Expected Java behavior (from PR description):
        // 50ms, -25ms, 0ms offsets working correctly
        std::cout << "\nExpected Java behavior: 50ms, -25ms, 0ms offsets" << std::endl;
        
        // Test 7: ImprecisionMap with distribution types
        std::cout << "\n=== TESTING IMPRECISIONMAP ALGORITHM ===\n" << std::endl;
        
        auto imprecisionMap = mpm::ImprecisionMap::createImprecisionMap("timing");
        
        // Create DistributionData objects manually
        mpm::DistributionData uniformData;
        uniformData.type = "uniform";
        uniformData.lowerLimit = -10.0;
        uniformData.upperLimit = 10.0;
        uniformData.hasLowerLimit = true;
        uniformData.hasUpperLimit = true;
        uniformData.seed = 12345;
        uniformData.xmlId = "uniform_test";
        
        mpm::DistributionData gaussianData;
        gaussianData.type = "gaussian";
        gaussianData.standardDeviation = 5.0;
        gaussianData.hasStandardDeviation = true;
        gaussianData.seed = 54321;
        gaussianData.xmlId = "gaussian_test";
        
        imprecisionMap->addDistribution(uniformData);
        imprecisionMap->addDistribution(gaussianData);
        
        std::cout << "Testing distribution types:" << std::endl;
        std::cout << "  Uniform distribution: [-10, 10]" << std::endl;
        std::cout << "  Gaussian distribution: σ=5.0" << std::endl;
        
        // Expected Java behavior (from PR description):
        // All 6 distribution types implemented (uniform, gaussian, triangular, brownian, compensating triangle, list)
        std::cout << "\nExpected Java behavior: All 6 distribution types supported" << std::endl;
        
        // Test 8: Combined transformation with full pipeline
        std::cout << "\n=== TESTING COMBINED TRANSFORMATIONS ===\n" << std::endl;
        
        auto combinedMpm = mpm::Mpm::createMpm();
        auto performance = mpm::Performance::createPerformance("BachFullTest");
        performance->setPPQ(bachMsm->getPPQ());
        
        // Add all maps to test combined effects
        auto combDynamicsMap = mpm::DynamicsMap::createDynamicsMap();
        combDynamicsMap->addDynamics(0.0, "40", "100", 0.5, 0.0);
        combDynamicsMap->addDynamics(960.0, "100", "", 0.0, 0.0);
        performance->getGlobal()->getDated()->addMap(std::move(combDynamicsMap));
        
        auto combArticulationMap = mpm::ArticulationMap::createArticulationMap();
        mpm::ArticulationData combStaccato;
        combStaccato.date = 0.0;
        combStaccato.relativeDuration = 0.5;
        combStaccato.relativeVelocity = 1.2;
        combArticulationMap->addArticulation(combStaccato);
        performance->getGlobal()->getDated()->addMap(std::move(combArticulationMap));
        
        combinedMpm->addPerformance(std::move(performance));
        
        // Apply transformations
        auto result = combinedMpm->getPerformance(0)->perform(*bachMsm);
        
        // Check first note results
        Element resultRoot = result->getRootElement();
        if (resultRoot) {
            auto parts = resultRoot.children("part");
            for (auto part : parts) {
                auto header = part.child("header");
                if (header) {
                    auto dated = header.child("dated");
                    if (dated) {
                        auto score = dated.child("score");
                        if (score) {
                            auto firstNote = score.child("note");
                            if (firstNote) {
                                std::cout << "First note combined transformation:" << std::endl;
                                std::cout << "  Duration: " << firstNote.attribute("duration").as_string() << std::endl;
                                std::cout << "  Velocity: " << firstNote.attribute("velocity").as_string() << std::endl;
                                
                                // Expected: duration 120 (240*0.5), velocity 48 (40*1.2)
                                std::cout << "\nExpected: duration 120, velocity 48" << std::endl;
                            }
                        }
                    }
                }
                break;
            }
        }
        
        // Test 9: PPQ preservation check
        std::cout << "\n=== TESTING PPQ PRESERVATION ===\n" << std::endl;
        
        std::cout << "Original MSM PPQ: " << bachMsm->getPPQ() << std::endl;
        std::cout << "Result MSM PPQ: " << result->getPPQ() << std::endl;
        
        // Expected: PPQ 480 maintained
        std::cout << "\nExpected: PPQ 480 preserved" << std::endl;
        
        // Test 10: XML structure validation
        std::cout << "\n=== TESTING XML STRUCTURE ===\n" << std::endl;
        
        std::string resultXml = result->toXml();
        std::cout << "Generated XML length: " << resultXml.length() << " characters" << std::endl;
        
        // Check if we can parse the generated XML
        xml::XmlBase testParse(resultXml, true);
        Element testRoot = testParse.getRootElement();
        if (testRoot) {
            std::cout << "✓ Generated XML is valid and parseable" << std::endl;
            std::cout << "  Title: " << testRoot.attribute("title").as_string() << std::endl;
        } else {
            std::cout << "✗ Generated XML is invalid" << std::endl;
        }
        
        std::cout << "\n🎯 COMPREHENSIVE TEST COMPLETED" << std::endl;
        std::cout << "Compare results above with expected Java behavior." << std::endl;
        std::cout << "Any significant deviations indicate implementation issues." << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}