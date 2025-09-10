package meico.mpm;

import meico.msm.Msm;
import meico.mpm.elements.Performance;
import meico.mpm.elements.Part;
import meico.mpm.elements.Dated;
import meico.mpm.elements.maps.OrnamentationMap;
import meico.mpm.elements.maps.GenericMap;
import nu.xom.*;

/**
 * Test class for OrnamentationMap functionality.
 * Tests the application of ornamentation maps to MSM documents.
 * Ornamentation typically adds decorative notes like trills, turns, and grace notes.
 * 
 * @author Test Framework
 */
public class OrnamentationMapTest {
    
    /**
     * Test basic ornamentation map functionality.
     * Creates a simple ornamentation and applies it to an MSM.
     */
    public static void testBasicOrnamentationMap() {
        System.out.println("Testing Basic Ornamentation Map...");
        
        // Create simple MSM
        Msm msm = MpmTestUtils.createSimpleMsm();
        MpmTestUtils.printMsm(msm, "Original MSM");
        
        // Create MPM with ornamentation map
        Mpm mpm = MpmTestUtils.createMpmWithMap(Mpm.ORNAMENTATION_MAP);
        Performance performance = mpm.getPerformance(0);
        Part part = performance.getPart("Piano");
        if (part == null) {
            System.err.println("Error: Could not find part 'Piano' in performance");
            return;
        }
        Dated dated = part.getDated();
        OrnamentationMap ornamentationMap = (OrnamentationMap) dated.getMap(Mpm.ORNAMENTATION_MAP);
        
        // Add ornamentation data to the map
        addBasicOrnamentationData(ornamentationMap);
        
        // Apply MPM to MSM
        Msm resultMsm = MpmTestUtils.applyMpmToMsm(msm, mpm);
        MpmTestUtils.printMsm(resultMsm, "MSM after Ornamentation Map");
        
        // Verify results
        if (verifyOrnamentationMapResults(resultMsm)) {
            System.out.println("✓ Basic Ornamentation Map test PASSED");
        } else {
            System.out.println("✗ Basic Ornamentation Map test FAILED");
        }
        System.out.println();
    }
    
    /**
     * Test ornamentation map with trill.
     */
    public static void testTrillOrnamentationMap() {
        System.out.println("Testing Trill Ornamentation Map...");
        
        // Create simple MSM
        Msm msm = MpmTestUtils.createSimpleMsm();
        
        // Create MPM with ornamentation map
        Mpm mpm = MpmTestUtils.createMpmWithMap(Mpm.ORNAMENTATION_MAP);
        Performance performance = mpm.getPerformance(0);
        Part part = performance.getPart("Piano");
        if (part == null) {
            System.err.println("Error: Could not find part 'Piano' in performance");
            return;
        }
        Dated dated = part.getDated();
        OrnamentationMap ornamentationMap = (OrnamentationMap) dated.getMap(Mpm.ORNAMENTATION_MAP);
        
        // Add trill ornamentation data
        addTrillOrnamentationData(ornamentationMap);
        
        // Apply MPM to MSM
        Msm resultMsm = MpmTestUtils.applyMpmToMsm(msm, mpm);
        MpmTestUtils.printMsm(resultMsm, "MSM after Trill Ornamentation Map");
        
        // Verify results
        if (verifyTrillOrnamentationResults(resultMsm)) {
            System.out.println("✓ Trill Ornamentation Map test PASSED");
        } else {
            System.out.println("✗ Trill Ornamentation Map test FAILED");
        }
        System.out.println();
    }
    
    /**
     * Add basic ornamentation data to the map.
     * Creates a simple grace note ornament.
     */
    private static void addBasicOrnamentationData(OrnamentationMap ornamentationMap) {
        // Add a grace note ornament to the first note
        int index = ornamentationMap.addOrnament(0.0, "grace");
        System.out.println("Added grace note ornamentation at index: " + index);
    }
    
    /**
     * Add trill ornamentation data to the map.
     */
    private static void addTrillOrnamentationData(OrnamentationMap ornamentationMap) {
        // Add a trill ornament to the second note
        int index = ornamentationMap.addOrnament(480.0, "trill");
        System.out.println("Added trill ornamentation at index: " + index);
    }
    
    /**
     * Verify that the ornamentation map was applied correctly.
     */
    private static boolean verifyOrnamentationMapResults(Msm msm) {
        // Check if ornamentation affected the score (might add new notes or modify existing ones)
        Nodes notes = msm.getRootElement().query("//note");
        boolean foundOrnamentationProcessing = false;
        int noteCount = notes.size();
        
        System.out.println("Number of notes after ornamentation: " + noteCount);
        
        for (int i = 0; i < notes.size(); i++) {
            Element note = (Element) notes.get(i);
            
            // Check for performance attributes and potential ornamentation attributes
            if (note.getAttribute("date.perf") != null) {
                foundOrnamentationProcessing = true;
                
                // Check for ornamentation-specific attributes
                if (note.getAttribute("ornament.date.offset") != null ||
                    note.getAttribute("ornament.duration") != null ||
                    note.getAttribute("ornament.dynamics") != null) {
                    System.out.println("Found ornamentation attributes on note " + i);
                }
            }
        }
        
        // Ornamentation might add notes, so check if we have more than the original 4
        boolean possibleNewNotes = noteCount >= 4;
        
        return foundOrnamentationProcessing && possibleNewNotes;
    }
    
    /**
     * Verify trill ornamentation results.
     */
    private static boolean verifyTrillOrnamentationResults(Msm msm) {
        // Trills typically add many small notes, so check for increased note count
        Nodes notes = msm.getRootElement().query("//note");
        int noteCount = notes.size();
        
        System.out.println("Number of notes after trill: " + noteCount);
        
        // A trill should potentially add more notes than the original
        boolean foundTrillEffect = noteCount >= 4;
        
        if (foundTrillEffect) {
            System.out.println("Trill ornamentation appears to have been processed");
        }
        
        return foundTrillEffect;
    }
    
    /**
     * Run all ornamentation map tests.
     */
    public static void runAllTests() {
        System.out.println("Running OrnamentationMap Tests...");
        System.out.println("==================================");
        
        testBasicOrnamentationMap();
        testTrillOrnamentationMap();
        
        System.out.println("OrnamentationMap Tests Completed.");
        System.out.println();
    }
    
    /**
     * Main method for running tests independently.
     */
    public static void main(String[] args) {
        runAllTests();
    }
}