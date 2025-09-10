#include "mpm/MpmTestUtils.h"
#include "mpm/elements/Performance.h"
#include "mpm/elements/Global.h"
#include "mpm/elements/Part.h"
#include "mpm/elements/Dated.h"
#include "mpm/elements/maps/DynamicsMap.h"
#include "mpm/elements/maps/ArticulationMap.h"
#include "mpm/elements/maps/MetricalAccentuationMap.h"
#include "mpm/elements/maps/TempoMap.h"
#include "mpm/elements/metadata/Metadata.h"
#include "xml/Helper.h"
#include <iostream>

namespace meico {
namespace test {

std::unique_ptr<msm::Msm> MpmTestUtils::createSimpleMsm() {
    // Create a basic MSM with one part and some test notes
    auto msm = msm::Msm::createMsm("Test MSM", "", 720);
    
    Element root = msm->getRootElement();
    if (!root) {
        return msm;
    }
    
    // Add a part
    Element part = root.append_child("part");
    part.append_attribute("name") = "Piano";
    part.append_attribute("number") = "1";
    part.append_attribute("midi.channel") = "0";
    part.append_attribute("midi.port") = "0";
    
    // Add header to part
    Element partHeader = part.append_child("header");
    
    // Add dated element to part
    Element dated = part.append_child("dated");
    
    // Create score map
    Element scoreMap = dated.append_child("score");
    scoreMap.append_attribute("id") = "score";
    
    // Add some test notes
    addNoteToScore(scoreMap, 0.0, "C4", 60, 480.0, 100.0);
    addNoteToScore(scoreMap, 480.0, "D4", 62, 480.0, 100.0);
    addNoteToScore(scoreMap, 960.0, "E4", 64, 480.0, 100.0);
    addNoteToScore(scoreMap, 1440.0, "F4", 65, 480.0, 100.0);
    
    return msm;
}

std::unique_ptr<msm::Msm> MpmTestUtils::createMultiPartMsm() {
    auto msm = createSimpleMsm();
    Element root = msm->getRootElement();
    
    // Add second part
    Element part2 = root.append_child("part");
    part2.append_attribute("name") = "Violin";
    part2.append_attribute("number") = "2";
    part2.append_attribute("midi.channel") = "1";
    part2.append_attribute("midi.port") = "0";
    
    // Add header to part2
    Element partHeader2 = part2.append_child("header");
    
    // Add dated element to part2
    Element dated2 = part2.append_child("dated");
    
    // Create score map for part2
    Element scoreMap2 = dated2.append_child("score");
    scoreMap2.append_attribute("id") = "score";
    
    // Add some test notes to part2 (higher octave)
    addNoteToScore(scoreMap2, 0.0, "C5", 72, 480.0, 90.0);
    addNoteToScore(scoreMap2, 480.0, "D5", 74, 480.0, 90.0);
    addNoteToScore(scoreMap2, 960.0, "E5", 76, 480.0, 90.0);
    addNoteToScore(scoreMap2, 1440.0, "F5", 77, 480.0, 90.0);
    
    return msm;
}

std::unique_ptr<mpm::Mpm> MpmTestUtils::createBasicMpm() {
    auto mpm = mpm::Mpm::createMpm();
    
    // Add a performance
    auto performance = mpm::Performance::createPerformance("Test Performance");
    mpm->addPerformance(std::move(performance));
    
    return mpm;
}

std::unique_ptr<mpm::Mpm> MpmTestUtils::createMpmWithMap(const std::string& mapType) {
    auto mpm = createBasicMpm();
    
    if (mpm->size() > 0) {
        auto performance = mpm->getPerformance(0);
        if (performance && mapType == mpm::Mpm::DYNAMICS_MAP) {
            // Create a dynamics map with some test data
            auto dynamicsMap = mpm::DynamicsMap::createDynamicsMap();
            
            // Add dynamics transition that will span across our notes
            dynamicsMap->addDynamics(0.0, "60", "110", 0.5, 0.0); // piano to forte with curvature from 0 to endDate
            dynamicsMap->addDynamics(1920.0, "110", "", 0.0, 0.0); // end instruction to set end date
            
            performance->getGlobal()->getDated()->addMap(std::move(dynamicsMap));
        }
    }
    
    return mpm;
}

std::unique_ptr<msm::Msm> MpmTestUtils::applyMpmToMsm(const msm::Msm& msm, const mpm::Mpm& mpm) {
    if (mpm.size() == 0) {
        return msm.clone(); // No performances to apply
    }
    
    // Get the first performance and apply it to the MSM
    const auto* performance = mpm.getPerformance(0);
    if (performance) {
        return performance->perform(msm);
    }
    
    return msm.clone();
}

bool MpmTestUtils::verifyMsmModifications(const msm::Msm& msm, const std::vector<std::string>& expectedModifications) {
    // TODO: Implement verification logic
    // This would check for specific modifications in the MSM XML structure
    
    return !expectedModifications.empty(); // Placeholder
}

void MpmTestUtils::printMsm(const msm::Msm& msm, const std::string& label) {
    std::cout << "=== " << label << " ===" << std::endl;
    std::cout << msm.toXml() << std::endl;
    std::cout << std::endl;
}

void MpmTestUtils::addNoteToScore(Element scoreMap, double date, const std::string& name, 
                                 int pitch, double duration, double velocity) {
    Element note = scoreMap.append_child("note");
    note.append_attribute("date") = std::to_string(date).c_str();
    note.append_attribute("name") = name.c_str();
    note.append_attribute("midi.pitch") = std::to_string(pitch).c_str();
    note.append_attribute("duration") = std::to_string(duration).c_str();
    note.append_attribute("velocity") = std::to_string(velocity).c_str();
}

std::unique_ptr<mpm::Mpm> MpmTestUtils::createMpmWithArticulationMap() {
    auto mpm = createBasicMpm();
    
    if (mpm->size() > 0) {
        auto* performance = mpm->getPerformance(0);
        if (performance && performance->getGlobal()) {
            // Create articulation map with some test data
            auto articulationMap = mpm::ArticulationMap::createArticulationMap();
            
            // Add a staccato articulation at date 0 (duration×0.5, velocity×1.2)
            mpm::ArticulationData staccatoData;
            staccatoData.date = 0.0;
            staccatoData.relativeDuration = 0.5;
            staccatoData.relativeVelocity = 1.2;
            staccatoData.xmlId = "staccato_1";
            articulationMap->addArticulation(staccatoData);
            
            // Add a legato articulation at date 1440 (duration×1.2, velocity×0.9)
            mpm::ArticulationData legatoData;
            legatoData.date = 1440.0;
            legatoData.relativeDuration = 1.2;
            legatoData.relativeVelocity = 0.9;
            legatoData.xmlId = "legato_1";
            articulationMap->addArticulation(legatoData);
            
            // Add the map to the performance
            performance->getGlobal()->getDated()->addMap(std::move(articulationMap));
        }
    }
    
    return mpm;
}

std::unique_ptr<mpm::Mpm> MpmTestUtils::createMpmWithMetricalAccentuationMap() {
    auto mpm = createBasicMpm();
    
    if (mpm->size() > 0) {
        auto* performance = mpm->getPerformance(0);
        if (performance && performance->getGlobal()) {
            // Create metrical accentuation map with some test data
            auto accentuationMap = mpm::MetricalAccentuationMap::createMetricalAccentuationMap();
            
            // Add an accentuation pattern starting at date 0
            accentuationMap->addAccentuationPattern(0.0, "basicPattern", 10.0, true, true);
            
            // Add the map to the performance
            performance->getGlobal()->getDated()->addMap(std::move(accentuationMap));
        }
    }
    
    return mpm;
}

std::unique_ptr<mpm::Mpm> MpmTestUtils::createMpmWithTempoMap() {
    auto mpm = createBasicMpm();
    
    if (mpm->size() > 0) {
        auto* performance = mpm->getPerformance(0);
        if (performance && performance->getGlobal()) {
            // Create tempo map with some test data
            auto tempoMap = mpm::TempoMap::createTempoMap();
            
            // Add initial tempo: 120 BPM at date 0
            tempoMap->addTempo(0.0, 120.0, 0.25);
            
            // Add accelerando: transition from 120 to 140 BPM at date 960 (1 beat later)
            tempoMap->addTempo(960.0, "120", "140", 0.25, 0.5, "accel_1");
            
            // Add ritardando: transition from 140 to 100 BPM at date 1920 (2 beats later)
            tempoMap->addTempo(1920.0, "140", "100", 0.25, 0.5, "rit_1");
            
            // Add the map to the performance
            performance->getGlobal()->getDated()->addMap(std::move(tempoMap));
        }
    }
    
    return mpm;
}

} // namespace test
} // namespace meico