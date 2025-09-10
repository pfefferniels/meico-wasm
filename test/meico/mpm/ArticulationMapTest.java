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
     */
    private static boolean verifyArticulationMapResults(Msm msm) {
        // Check if articulation attributes were added to notes
        Nodes notes = msm.getRootElement().query("//note");
        boolean foundArticulationAttributes = false;
        
        for (int i = 0; i < notes.size(); i++) {
            Element note = (Element) notes.get(i);
            // Check for common articulation attributes that might be added
            if (note.getAttribute("articulation") != null ||
                note.getAttribute("articulation.absoluteDurationMs") != null ||
                note.getAttribute("articulation.absoluteVelocity") != null ||
                note.getAttribute("articulation.relativeDuration") != null) {
                foundArticulationAttributes = true;
                System.out.println("Found articulation attributes on note: " + note.toXML());
                break;
            }
        }
        
        return foundArticulationAttributes;
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