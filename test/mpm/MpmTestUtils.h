#pragma once

#include "msm/Msm.h"
#include "mpm/Mpm.h"
#include <memory>

namespace meico {
namespace test {

/**
 * Utility class for creating test MSM and MPM documents for testing MPM functionality.
 * This class provides methods to create simple musical structures for testing purposes.
 * Ported from Java MpmTestUtils.
 */
class MpmTestUtils {
public:
    /**
     * Creates a simple MSM document for testing.
     * @return Simple MSM document
     */
    static std::unique_ptr<msm::Msm> createSimpleMsm();

    /**
     * Creates a simple MSM document with multiple parts for testing.
     * @return MSM document with multiple parts
     */
    static std::unique_ptr<msm::Msm> createMultiPartMsm();

    /**
     * Creates a basic MPM document structure.
     * @return Basic MPM document
     */
    static std::unique_ptr<mpm::Mpm> createBasicMpm();

    /**
     * Creates an MPM document with a specific map type for testing.
     * @param mapType The type of map to create
     * @return MPM document with the specified map
     */
    static std::unique_ptr<mpm::Mpm> createMpmWithMap(const std::string& mapType);

    /**
     * Creates an MPM document with articulation map for testing.
     * @return MPM document with articulation map
     */
    static std::unique_ptr<mpm::Mpm> createMpmWithArticulationMap();

    /**
     * Creates an MPM document with metrical accentuation map for testing.
     * @return MPM document with metrical accentuation map
     */
    static std::unique_ptr<mpm::Mpm> createMpmWithMetricalAccentuationMap();

    /**
     * Applies an MPM performance to an MSM and returns the result.
     * @param msm The input MSM
     * @param mpm The MPM to apply
     * @return The modified MSM
     */
    static std::unique_ptr<msm::Msm> applyMpmToMsm(const msm::Msm& msm, const mpm::Mpm& mpm);

    /**
     * Verifies that an MSM contains expected note modifications.
     * @param msm The MSM to check
     * @param expectedModifications What modifications to look for
     * @return true if modifications are found
     */
    static bool verifyMsmModifications(const msm::Msm& msm, const std::vector<std::string>& expectedModifications);

    /**
     * Prints MSM content for debugging purposes.
     * @param msm The MSM to print
     * @param label A label for the output
     */
    static void printMsm(const msm::Msm& msm, const std::string& label);

private:
    /**
     * Helper method to add a note to a score map.
     */
    static void addNoteToScore(Element scoreMap, double date, const std::string& name, 
                              int pitch, double duration, double velocity);

    MpmTestUtils() = delete; // Utility class, no instances
};

} // namespace test
} // namespace meico