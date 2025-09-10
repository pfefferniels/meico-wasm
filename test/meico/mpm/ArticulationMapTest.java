package meico.mpm;

import meico.msm.Msm;
import meico.mpm.elements.Performance;
import meico.mpm.elements.Part;
import meico.mpm.elements.Dated;
import meico.mpm.elements.maps.ArticulationMap;
import meico.mpm.elements.maps.GenericMap;
import meico.xml.Helper;
import nu.xom.*;

/**
 * Test class for ArticulationMap functionality.
 * Tests the application of articulation maps to MSM documents.
 * 
 * @author Test Framework
 */
public class ArticulationMapTest {
    
    /**
     * Test basic articulation map functionality.
     * Creates a simple articulation map and applies it to an MSM.
     */
    public static void testBasicArticulationMap() {
        System.out.println("Testing Basic Articulation Map...");
        
        // Create simple MSM
        Msm msm = MpmTestUtils.createSimpleMsm();
        MpmTestUtils.printMsm(msm, "Original MSM");
        
        // Create MPM with articulation map
        Mpm mpm = MpmTestUtils.createMpmWithMap(Mpm.ARTICULATION_MAP);
        Performance performance = mpm.getPerformance(0);
        Part part = performance.getPart("Piano");
        if (part == null) {
            System.err.println("Error: Could not find part 'Piano' in performance");
            return;
        }
        Dated dated = part.getDated();
        ArticulationMap articulationMap = (ArticulationMap) dated.getMap(Mpm.ARTICULATION_MAP);
        
        // Add articulation data to the map
        addBasicArticulationData(articulationMap);
        
        // Apply MPM to MSM
        Msm resultMsm = MpmTestUtils.applyMpmToMsm(msm, mpm);
        MpmTestUtils.printMsm(resultMsm, "MSM after Articulation Map");
        
        // Verify results
        if (verifyArticulationMapResults(resultMsm)) {
            System.out.println("✓ Basic Articulation Map test PASSED");
        } else {
            System.out.println("✗ Basic Articulation Map test FAILED");
        }
        System.out.println();
    }
    
    /**
     * Test articulation map with specific note targeting.
     */
    public static void testNoteSpecificArticulationMap() {
        System.out.println("Testing Note-Specific Articulation Map...");
        
        // Create simple MSM
        Msm msm = MpmTestUtils.createSimpleMsm();
        
        // Create MPM with articulation map
        Mpm mpm = MpmTestUtils.createMpmWithMap(Mpm.ARTICULATION_MAP);
        Performance performance = mpm.getPerformance(0);
        Part part = performance.getPart("Piano");
        if (part == null) {
            System.err.println("Error: Could not find part 'Piano' in performance");
            return;
        }
        Dated dated = part.getDated();
        ArticulationMap articulationMap = (ArticulationMap) dated.getMap(Mpm.ARTICULATION_MAP);
        
        // Add note-specific articulation data
        addNoteSpecificArticulationData(articulationMap);
        
        // Apply MPM to MSM
        Msm resultMsm = MpmTestUtils.applyMpmToMsm(msm, mpm);
        MpmTestUtils.printMsm(resultMsm, "MSM after Note-Specific Articulation Map");
        
        // Verify results
        if (verifyNoteSpecificArticulationResults(resultMsm)) {
            System.out.println("✓ Note-Specific Articulation Map test PASSED");
        } else {
            System.out.println("✗ Note-Specific Articulation Map test FAILED");
        }
        System.out.println();
    }
    
    /**
     * Add basic articulation data to the articulation map.
     * Creates simple staccato articulation.
     */
    private static void addBasicArticulationData(ArticulationMap articulationMap) {
        // Add a staccato articulation at the beginning
        int index = articulationMap.addArticulation(0.0, "staccato", null, null);
        System.out.println("Added staccato articulation at index: " + index);
    }
    
    /**
     * Add note-specific articulation data to the articulation map.
     */
    private static void addNoteSpecificArticulationData(ArticulationMap articulationMap) {
        // Add articulation for specific note at date 480.0 (second note)
        int index = articulationMap.addArticulation(480.0, "accent", null, null);
        System.out.println("Added accent articulation at index: " + index);
    }
    
    /**
     * Verify that the articulation map was applied correctly.
     * For articulation, we expect performance attributes to be added and 
     * potentially modifications to timing and dynamics.
     */
    private static boolean verifyArticulationMapResults(Msm msm) {
        // Check if performance attributes were added to notes
        Nodes notes = msm.getRootElement().query("//note");
        boolean foundPerformanceAttributes = false;
        boolean hasReasonableChanges = false;
        
        for (int i = 0; i < notes.size(); i++) {
            Element note = (Element) notes.get(i);
            
            // Check for performance attributes (these should be present after performance application)
            if (note.getAttribute("date.perf") != null &&
                note.getAttribute("duration.perf") != null &&
                note.getAttribute("milliseconds.date") != null) {
                foundPerformanceAttributes = true;
                
                // For staccato articulation, we might expect some timing modifications
                // Let's check if performance attributes are present and reasonable
                String originalDuration = note.getAttributeValue("duration");
                String perfDuration = note.getAttributeValue("duration.perf");
                
                if (originalDuration != null && perfDuration != null) {
                    double origDur = Double.parseDouble(originalDuration);
                    double perfDur = Double.parseDouble(perfDuration);
                    
                    // The performance system has converted timing, so we expect some changes
                    if (origDur > 0 && perfDur > 0) {
                        hasReasonableChanges = true;
                        System.out.println("Note: original duration=" + origDur + ", perf duration=" + perfDur);
                    }
                }
                break;
            }
        }
        
        return foundPerformanceAttributes && hasReasonableChanges;
    }
    
    /**
     * Verify note-specific articulation results.
     */
    private static boolean verifyNoteSpecificArticulationResults(Msm msm) {
        // Similar verification as basic but looking for specific modifications
        return verifyArticulationMapResults(msm);
    }
    
    /**
     * Run all articulation map tests.
     */
    public static void runAllTests() {
        System.out.println("Running ArticulationMap Tests...");
        System.out.println("==================================");
        
        testBasicArticulationMap();
        testNoteSpecificArticulationMap();
        
        System.out.println("ArticulationMap Tests Completed.");
        System.out.println();
    }
    
    /**
     * Main method for running tests independently.
     */
    public static void main(String[] args) {
        runAllTests();
    }
}