#include "mpm/MpmTestUtils.h"
#include "mpm/elements/Performance.h"
#include "mpm/elements/Global.h"
#include "mpm/elements/Part.h"
#include "mpm/elements/Dated.h"
#include "mpm/elements/maps/DynamicsMap.h"
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
            dynamicsMap->addDynamics(0.0, 60.0);    // piano at start
            dynamicsMap->addDynamics(1440.0, 110.0); // forte later
            
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

} // namespace test
} // namespace meico