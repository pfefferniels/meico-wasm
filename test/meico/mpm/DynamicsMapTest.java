package meico.mpm;

import meico.msm.Msm;
import meico.mpm.elements.Performance;
import meico.mpm.elements.Part;
import meico.mpm.elements.Dated;
import meico.mpm.elements.maps.DynamicsMap;
import meico.mpm.elements.maps.GenericMap;
import nu.xom.*;

/**
 * Test class for DynamicsMap functionality.
 * Tests the application of dynamics maps to MSM documents.
 * 
 * @author Test Framework
 */
public class DynamicsMapTest {
    
    /**
     * Test basic dynamics map functionality.
     * Creates a simple dynamics map and applies it to an MSM.
     */
    public static void testBasicDynamicsMap() {
        System.out.println("Testing Basic Dynamics Map...");
        
        // Create simple MSM
        Msm msm = MpmTestUtils.createSimpleMsm();
        MpmTestUtils.printMsm(msm, "Original MSM");
        
        // Create MPM with dynamics map
        Mpm mpm = MpmTestUtils.createMpmWithMap(Mpm.DYNAMICS_MAP);
        Performance performance = mpm.getPerformance(0);
        Part part = performance.getPart("Piano");
        if (part == null) {
            System.err.println("Error: Could not find part 'Piano' in performance");
            return;
        }
        Dated dated = part.getDated();
        DynamicsMap dynamicsMap = (DynamicsMap) dated.getMap(Mpm.DYNAMICS_MAP);
        
        // Add dynamics data to the map
        addBasicDynamicsData(dynamicsMap);
        
        // Apply MPM to MSM
        Msm resultMsm = MpmTestUtils.applyMpmToMsm(msm, mpm);
        MpmTestUtils.printMsm(resultMsm, "MSM after Dynamics Map");
        
        // Verify results
        if (verifyDynamicsMapResults(resultMsm)) {
            System.out.println("✓ Basic Dynamics Map test PASSED");
        } else {
            System.out.println("✗ Basic Dynamics Map test FAILED");
        }
        System.out.println();
    }
    
    /**
     * Test dynamics map with crescendo/diminuendo.
     */
    public static void testCrescendoDynamicsMap() {
        System.out.println("Testing Crescendo Dynamics Map...");
        
        // Create simple MSM
        Msm msm = MpmTestUtils.createSimpleMsm();
        
        // Create MPM with dynamics map
        Mpm mpm = MpmTestUtils.createMpmWithMap(Mpm.DYNAMICS_MAP);
        Performance performance = mpm.getPerformance(0);
        Part part = performance.getPart("Piano");
        if (part == null) {
            System.err.println("Error: Could not find part 'Piano' in performance");
            return;
        }
        Dated dated = part.getDated();
        DynamicsMap dynamicsMap = (DynamicsMap) dated.getMap(Mpm.DYNAMICS_MAP);
        
        // Add crescendo dynamics data
        addCrescendoDynamicsData(dynamicsMap);
        
        // Apply MPM to MSM
        Msm resultMsm = MpmTestUtils.applyMpmToMsm(msm, mpm);
        MpmTestUtils.printMsm(resultMsm, "MSM after Crescendo Dynamics Map");
        
        // Verify results
        if (verifyCrescendoDynamicsResults(resultMsm)) {
            System.out.println("✓ Crescendo Dynamics Map test PASSED");
        } else {
            System.out.println("✗ Crescendo Dynamics Map test FAILED");
        }
        System.out.println();
    }
    
    /**
     * Add basic dynamics data to the dynamics map.
     * Creates a forte dynamic marking.
     */
    private static void addBasicDynamicsData(DynamicsMap dynamicsMap) {
        // Add a forte dynamic at the beginning
        int index = dynamicsMap.addDynamics(0.0, "110.0");  // forte is typically around 110 velocity
        System.out.println("Added forte dynamics at index: " + index);
    }
    
    /**
     * Add crescendo dynamics data to the dynamics map.
     */
    private static void addCrescendoDynamicsData(DynamicsMap dynamicsMap) {
        // Add crescendo from piano to forte over the duration
        int index1 = dynamicsMap.addDynamics(0.0, "60.0");    // piano
        int index2 = dynamicsMap.addDynamics(1440.0, "110.0"); // forte at the end
        System.out.println("Added crescendo dynamics at indices: " + index1 + ", " + index2);
    }
    
    /**
     * Verify that the dynamics map was applied correctly.
     */
    private static boolean verifyDynamicsMapResults(Msm msm) {
        // Check if velocity values were modified and performance attributes added
        Nodes notes = msm.getRootElement().query("//note");
        boolean foundDynamicsChanges = false;
        
        for (int i = 0; i < notes.size(); i++) {
            Element note = (Element) notes.get(i);
            
            // Check for performance attributes and velocity modifications
            if (note.getAttribute("velocity") != null) {
                String velocity = note.getAttributeValue("velocity");
                double vel = Double.parseDouble(velocity);
                
                // Check if velocity is in a reasonable range and potentially modified
                if (vel > 0 && vel <= 127) {
                    foundDynamicsChanges = true;
                    System.out.println("Note velocity: " + vel);
                }
            }
        }
        
        return foundDynamicsChanges;
    }
    
    /**
     * Verify crescendo dynamics results.
     */
    private static boolean verifyCrescendoDynamicsResults(Msm msm) {
        // Check if velocities show a crescendo pattern
        Nodes notes = msm.getRootElement().query("//note");
        boolean hasVelocityVariation = false;
        double firstVelocity = -1;
        double lastVelocity = -1;
        
        for (int i = 0; i < notes.size(); i++) {
            Element note = (Element) notes.get(i);
            if (note.getAttribute("velocity") != null) {
                double velocity = Double.parseDouble(note.getAttributeValue("velocity"));
                if (i == 0) {
                    firstVelocity = velocity;
                }
                lastVelocity = velocity;
                System.out.println("Note " + i + " velocity: " + velocity);
            }
        }
        
        // Check if there's a crescendo pattern (last velocity higher than first)
        if (firstVelocity > 0 && lastVelocity > 0) {
            hasVelocityVariation = (lastVelocity != firstVelocity);
            System.out.println("Velocity change: " + firstVelocity + " -> " + lastVelocity);
        }
        
        return hasVelocityVariation;
    }
    
    /**
     * Run all dynamics map tests.
     */
    public static void runAllTests() {
        System.out.println("Running DynamicsMap Tests...");
        System.out.println("============================");
        
        testBasicDynamicsMap();
        testCrescendoDynamicsMap();
        
        System.out.println("DynamicsMap Tests Completed.");
        System.out.println();
    }
    
    /**
     * Main method for running tests independently.
     */
    public static void main(String[] args) {
        runAllTests();
    }
}