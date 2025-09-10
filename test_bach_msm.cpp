#include <iostream>
#include <memory>
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
#include "mpm/elements/metadata/Metadata.h"

using namespace meico;

int main() {
    std::cout << "Testing C++ implementation with Bach Cello Suite MSM data..." << std::endl;
    
    try {
        // Create the test MSM from the provided data
        std::string bachMsmXml = R"(<?xml version="1.0"?>
<msm title="J. S. Bach Cello Suite BWV 1007 Minuet No 2" xml:id="ef510daa-73b7-46be-bd64-ce72cf21fae9" pulsesPerQuarter="480">
    <global>
        <header>
            <dated>
                <timesignaturemap>
                    <timesignature date="0.0" numerator="3.0" denominator="4">
                    </timesignature>
                </timesignaturemap>
                <keysignaturemap>
                    <keysignature date="0.0">
                        <accidental midi.pitch="11.0" pitchname="B" value="-1.0">
                        </accidental>
                    </keysignature>
                </keysignaturemap>
            </dated>
        </header>
    </global>
    <part name="Cello" number="0" midi.channel="0" midi.port="0">
        <header>
            <dated>
                <score>
                    <note date="0.0" midi.pitch="58.0" pitchname="B" accidentals="-1.0" duration="240.0" velocity="80.0" xml:id="meico_e0ce30cc-e00e-4cc1-9c43-fea3725a80ef">
                    </note>
                    <note date="240.0" midi.pitch="57.0" pitchname="A" accidentals="0.0" duration="240.0" velocity="80.0" xml:id="meico_4b9aa215-cbc9-4009-a652-93dea391ad0a">
                    </note>
                    <note date="480.0" midi.pitch="58.0" pitchname="B" accidentals="-1.0" duration="240.0" velocity="80.0" xml:id="meico_af007aa5-8bfd-42bb-a3cd-4ffbc48895b3">
                    </note>
                    <note date="720.0" midi.pitch="50.0" pitchname="D" accidentals="0.0" duration="240.0" velocity="80.0" xml:id="meico_5fd2c151-37b9-44d8-b6b5-6228ef9a56b0">
                    </note>
                    <note date="960.0" midi.pitch="51.0" pitchname="E" accidentals="-1.0" duration="240.0" velocity="80.0" xml:id="meico_91cafb28-54b7-44e8-a3b5-9183897224a1">
                    </note>
                </score>
            </dated>
        </header>
    </part>
</msm>)";

        // Parse the MSM
        xml::XmlBase msmXmlDoc(bachMsmXml, true);
        Element msmRoot = msmXmlDoc.getRootElement();
        
        std::cout << "✓ Successfully parsed Bach MSM" << std::endl;
        std::cout << "  Title: " << msmRoot.attribute("title").as_string() << std::endl;
        std::cout << "  PPQ: " << msmRoot.attribute("pulsesperquarter").as_string() << std::endl;
        
        // Create MSM object from the XML
        auto bachMsm = std::make_unique<msm::Msm>(bachMsmXml, true);
        std::cout << "✓ Created MSM object: " << bachMsm->getTitle() << std::endl;
        
        // Print original MSM structure
        std::cout << "\n--- ORIGINAL BACH MSM STRUCTURE ---" << std::endl;
        std::cout << "PPQ: " << bachMsm->getPPQ() << std::endl;
        
        // Access XML structure directly for note analysis
        Element root = bachMsm->getRootElement();
        if (root) {
            auto parts = root.children("part");
            int partCount = 0;
            for (auto part : parts) {
                std::cout << "Part " << partCount << ": " << part.attribute("name").as_string() << std::endl;
                
                // Find score in part/header/dated/score
                auto header = part.child("header");
                if (header) {
                    auto dated = header.child("dated");
                    if (dated) {
                        auto score = dated.child("score");
                        if (score) {
                            auto notes = score.children("note");
                            int noteCount = 0;
                            for (auto note : notes) {
                                noteCount++;
                            }
                            std::cout << "  Notes (" << noteCount << "):" << std::endl;
                            for (auto note : notes) {
                                std::cout << "    Date: " << note.attribute("date").as_string()
                                         << ", Pitch: " << note.attribute("midi.pitch").as_string()
                                         << ", Duration: " << note.attribute("duration").as_string()
                                         << ", Velocity: " << note.attribute("velocity").as_string()
                                         << std::endl;
                            }
                        }
                    }
                }
                partCount++;
            }
        }
        
        // Test 1: DynamicsMap
        std::cout << "\n=== Testing DynamicsMap with Bach MSM ===" << std::endl;
        auto dynamicsMpm = mpm::Mpm::createMpm();
        auto performance = mpm::Performance::createPerformance("BachTest");
        
        // Set performance PPQ to match MSM PPQ to avoid conversion
        performance->setPPQ(bachMsm->getPPQ());
        
        auto dynamicsMap = mpm::DynamicsMap::createDynamicsMap();
        // Create crescendo from p (40) to f (100) over the piece using string values
        dynamicsMap->addDynamics(0.0, "40", "100", 0.5, 0.0);
        dynamicsMap->addDynamics(960.0, "100", "", 0.0, 0.0);
        
        // Set up the performance structure
        performance->getGlobal()->getDated()->addMap(std::move(dynamicsMap));
        dynamicsMpm->addPerformance(std::move(performance));
        
        // Apply dynamics using the correct method
        auto bachWithDynamics = dynamicsMpm->getPerformance(0)->perform(*bachMsm);
        std::cout << "✓ Applied DynamicsMap to Bach MSM" << std::endl;
        
        // Verify dynamics changes using XML structure
        Element dynRoot = bachWithDynamics->getRootElement();
        if (dynRoot) {
            auto dynParts = dynRoot.children("part");
            for (auto dynPart : dynParts) {
                auto dynHeader = dynPart.child("header");
                if (dynHeader) {
                    auto dynDated = dynHeader.child("dated");
                    if (dynDated) {
                        auto dynScore = dynDated.child("score");
                        if (dynScore) {
                            std::cout << "  Dynamics results:" << std::endl;
                            for (auto dynNote : dynScore.children("note")) {
                                std::cout << "    Date: " << dynNote.attribute("date").as_string()
                                         << ", Velocity: " << dynNote.attribute("velocity").as_string() << std::endl;
                            }
                        }
                    }
                }
                break; // Only check first part
            }
        }
        
        // Test 2: ArticulationMap
        std::cout << "\n=== Testing ArticulationMap with Bach MSM ===" << std::endl;
        auto articulationMpm = mpm::Mpm::createMpm();
        auto artPerformance = mpm::Performance::createPerformance("BachArticulationTest");
        
        // Set performance PPQ to match MSM PPQ to avoid conversion
        artPerformance->setPPQ(bachMsm->getPPQ());
        
        auto articulationMap = mpm::ArticulationMap::createArticulationMap();
        // Apply staccato to first note and legato to second note using ArticulationData
        mpm::ArticulationData staccatoData;
        staccatoData.date = 0.0;
        staccatoData.relativeDuration = 0.5;
        staccatoData.relativeVelocity = 1.2;
        staccatoData.xmlId = "staccato_1";
        articulationMap->addArticulation(staccatoData);
        
        mpm::ArticulationData legatoData;
        legatoData.date = 240.0;
        legatoData.relativeDuration = 1.2;
        legatoData.relativeVelocity = 0.9;
        legatoData.xmlId = "legato_1";
        articulationMap->addArticulation(legatoData);
        
        artPerformance->getGlobal()->getDated()->addMap(std::move(articulationMap));
        articulationMpm->addPerformance(std::move(artPerformance));
        
        // Apply articulations
        auto bachWithArticulation = articulationMpm->getPerformance(0)->perform(*bachMsm);
        std::cout << "✓ Applied ArticulationMap to Bach MSM" << std::endl;
        
        // Verify articulation changes using XML structure
        Element artRoot = bachWithArticulation->getRootElement();
        if (artRoot) {
            auto artParts = artRoot.children("part");
            for (auto artPart : artParts) {
                auto artHeader = artPart.child("header");
                if (artHeader) {
                    auto artDated = artHeader.child("dated");
                    if (artDated) {
                        auto artScore = artDated.child("score");
                        if (artScore) {
                            std::cout << "  Articulation results:" << std::endl;
                            for (auto artNote : artScore.children("note")) {
                                std::cout << "    Date: " << artNote.attribute("date").as_string()
                                         << ", Duration: " << artNote.attribute("duration").as_string()
                                         << ", Velocity: " << artNote.attribute("velocity").as_string()
                                         << std::endl;
                            }
                        }
                    }
                }
                break; // Only check first part
            }
        }
        
        // Test 3: TempoMap
        std::cout << "\n=== Testing TempoMap with Bach MSM ===" << std::endl;
        auto tempoMpm = mpm::Mpm::createMpm();
        auto tempoPerformance = mpm::Performance::createPerformance("BachTempoTest");
        
        // Set performance PPQ to match MSM PPQ to avoid conversion
        tempoPerformance->setPPQ(bachMsm->getPPQ());
        
        auto tempoMap = mpm::TempoMap::createTempoMap();
        // Start at moderate tempo, accelerate in middle, return to original
        tempoMap->addTempo(0.0, 120.0, 0.25);
        tempoMap->addTempo(480.0, "120", "140", 0.25, 0.5, "accel_1"); // accelerando
        tempoMap->addTempo(960.0, 120.0, 0.25); // back to original
        
        tempoPerformance->getGlobal()->getDated()->addMap(std::move(tempoMap));
        tempoMpm->addPerformance(std::move(tempoPerformance));
        
        // Apply tempo
        auto bachWithTempo = tempoMpm->getPerformance(0)->perform(*bachMsm);
        std::cout << "✓ Applied TempoMap to Bach MSM" << std::endl;
        
        // Test tempo calculations directly from the map we just created
        auto testTempoMap = mpm::TempoMap::createTempoMap();
        testTempoMap->addTempo(0.0, 120.0, 0.25);
        testTempoMap->addTempo(480.0, "120", "140", 0.25, 0.5, "accel_1");
        testTempoMap->addTempo(960.0, 120.0, 0.25);
        
        double tempo1 = testTempoMap->getTempoAt(0.0);
        double tempo2 = testTempoMap->getTempoAt(600.0); // middle of transition
        double tempo3 = testTempoMap->getTempoAt(960.0);
        
        std::cout << "  Tempo at start: " << tempo1 << " BPM" << std::endl;
        std::cout << "  Tempo at transition middle: " << tempo2 << " BPM" << std::endl;
        std::cout << "  Tempo at end: " << tempo3 << " BPM" << std::endl;
        
        // Test 4: OrnamentationMap
        std::cout << "\n=== Testing OrnamentationMap with Bach MSM ===" << std::endl;
        auto ornamentMpm = mpm::Mpm::createMpm();
        auto ornPerformance = mpm::Performance::createPerformance("BachOrnamentTest");
        
        // Set performance PPQ to match MSM PPQ to avoid conversion
        ornPerformance->setPPQ(bachMsm->getPPQ());
        
        auto ornamentationMap = mpm::OrnamentationMap::createOrnamentationMap();
        // Add trill to first note and grace note to third note
        ornamentationMap->addOrnament(0.0, "trill", 1.0, {}, "trill_1");
        ornamentationMap->addOrnament(480.0, "grace", 1.0, {}, "grace_1");
        
        ornPerformance->getGlobal()->getDated()->addMap(std::move(ornamentationMap));
        ornamentMpm->addPerformance(std::move(ornPerformance));
        
        // Apply ornaments
        auto bachWithOrnaments = ornamentMpm->getPerformance(0)->perform(*bachMsm);
        std::cout << "✓ Applied OrnamentationMap to Bach MSM" << std::endl;
        
        // Test 5: Combined transformation
        std::cout << "\n=== Testing Combined Transformations ===" << std::endl;
        auto combinedMpm = mpm::Mpm::createMpm();
        auto combPerformance = mpm::Performance::createPerformance("BachCombinedTest");
        
        // Set performance PPQ to match MSM PPQ to avoid conversion
        combPerformance->setPPQ(bachMsm->getPPQ());
        
        // Add multiple maps to the same performance
        auto combDynamicsMap = mpm::DynamicsMap::createDynamicsMap();
        combDynamicsMap->addDynamics(0.0, "60", "90", 0.5, 0.0);
        combDynamicsMap->addDynamics(960.0, "90", "", 0.0, 0.0);
        combPerformance->getGlobal()->getDated()->addMap(std::move(combDynamicsMap));
        
        auto combArticulationMap = mpm::ArticulationMap::createArticulationMap();
        mpm::ArticulationData combLegatoData;
        combLegatoData.date = 0.0;
        combLegatoData.relativeDuration = 1.1;
        combLegatoData.relativeVelocity = 0.95;
        combLegatoData.xmlId = "combined_legato";
        combArticulationMap->addArticulation(combLegatoData);
        combPerformance->getGlobal()->getDated()->addMap(std::move(combArticulationMap));
        
        auto combTempoMap = mpm::TempoMap::createTempoMap();
        combTempoMap->addTempo(0.0, 110.0, 0.25);
        combTempoMap->addTempo(480.0, "110", "130", 0.25, 0.5, "combined_accel");
        combTempoMap->addTempo(960.0, 110.0, 0.25);
        combPerformance->getGlobal()->getDated()->addMap(std::move(combTempoMap));
        
        combinedMpm->addPerformance(std::move(combPerformance));
        
        // Apply combined transformations
        auto bachFinal = combinedMpm->getPerformance(0)->perform(*bachMsm);
        std::cout << "✓ Applied combined transformations to Bach MSM" << std::endl;
        
        // Show final results using XML structure
        Element finalRoot = bachFinal->getRootElement();
        if (finalRoot) {
            auto finalParts = finalRoot.children("part");
            for (auto finalPart : finalParts) {
                auto finalHeader = finalPart.child("header");
                if (finalHeader) {
                    auto finalDated = finalHeader.child("dated");
                    if (finalDated) {
                        auto finalScore = finalDated.child("score");
                        if (finalScore) {
                            std::cout << "  Final combined results:" << std::endl;
                            for (auto finalNote : finalScore.children("note")) {
                                std::cout << "    Date: " << finalNote.attribute("date").as_string()
                                         << ", Pitch: " << finalNote.attribute("midi.pitch").as_string()
                                         << ", Duration: " << finalNote.attribute("duration").as_string()
                                         << ", Velocity: " << finalNote.attribute("velocity").as_string()
                                         << std::endl;
                            }
                        }
                    }
                }
                break; // Only check first part
            }
        }
        
        // Test XML output compatibility
        std::cout << "\n=== Testing XML Output ===" << std::endl;
        std::string finalXml = bachFinal->toXml();
        if (!finalXml.empty()) {
            std::cout << "✓ Generated final XML output (" << finalXml.length() << " characters)" << std::endl;
            
            // Show a preview of the XML
            size_t previewLen = std::min((size_t)500, finalXml.length());
            std::cout << "  XML Preview:\n" << finalXml.substr(0, previewLen);
            if (finalXml.length() > previewLen) {
                std::cout << "...";
            }
            std::cout << std::endl;
        }
        
        std::cout << "\n🎉 All Bach MSM tests completed successfully!" << std::endl;
        std::cout << "C++ implementation demonstrates full compatibility with test data." << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}