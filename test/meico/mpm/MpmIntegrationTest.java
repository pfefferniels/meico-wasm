package meico.mpm;

import meico.msm.Msm;
import meico.mpm.elements.Performance;
import meico.mpm.elements.Part;
import meico.mpm.elements.Dated;
import meico.mpm.elements.maps.*;
import nu.xom.*;

/**
 * Integration test class for MPM functionality.
 * Tests the combination of multiple map types applied together.
 * 
 * @author Test Framework
 */
public class MpmIntegrationTest {
    
    /**
     * Test integration of multiple map types working together.
     * Combines dynamics, tempo, and articulation maps.
     */
    public static void testMultipleMapIntegration() {
        System.out.println("Testing Multiple Map Integration...");
        
        // Create simple MSM
        Msm msm = MpmTestUtils.createSimpleMsm();
        MpmTestUtils.printMsm(msm, "Original MSM");
        
        // Create MPM with multiple maps
        Mpm mpm = createMpmWithMultipleMaps();
        
        // Apply MPM to MSM
        Msm resultMsm = MpmTestUtils.applyMpmToMsm(msm, mpm);
        MpmTestUtils.printMsm(resultMsm, "MSM after Multiple Maps Integration");
        
        // Verify results
        if (verifyIntegrationResults(resultMsm)) {
            System.out.println("✓ Multiple Map Integration test PASSED");
        } else {
            System.out.println("✗ Multiple Map Integration test FAILED");
        }
        System.out.println();
    }
    
    /**
     * Test musical phrase with expressive performance.
     * Creates a more realistic musical scenario.
     */
    public static void testExpressiveMusicalPhrase() {
        System.out.println("Testing Expressive Musical Phrase...");
        
        // Create multi-part MSM for more realistic test
        Msm msm = MpmTestUtils.createMultiPartMsm();
        MpmTestUtils.printMsm(msm, "Original Multi-Part MSM");
        
        // Create expressive MPM
        Mpm mpm = createExpressiveMpm();
        
        // Apply MPM to MSM
        Msm resultMsm = MpmTestUtils.applyMpmToMsm(msm, mpm);
        MpmTestUtils.printMsm(resultMsm, "MSM after Expressive Performance");
        
        // Verify results
        if (verifyExpressiveResults(resultMsm)) {
            System.out.println("✓ Expressive Musical Phrase test PASSED");
        } else {
            System.out.println("✗ Expressive Musical Phrase test FAILED");
        }
        System.out.println();
    }
    
    /**
     * Create MPM with multiple map types.
     */
    private static Mpm createMpmWithMultipleMaps() {
        Mpm mpm = MpmTestUtils.createBasicMpm();
        Performance performance = mpm.getPerformance(0);
        
        // Add part
        Part part = Part.createPart("Piano", 1, 0, 0);
        performance.addPart(part);
        Dated dated = part.getDated();
        
        // Add dynamics map
        DynamicsMap dynamicsMap = DynamicsMap.createDynamicsMap();
        dynamicsMap.addDynamics(0.0, "60.0");    // piano
        dynamicsMap.addDynamics(1440.0, "110.0"); // forte
        dated.addMap(dynamicsMap);
        
        // Add tempo map
        TempoMap tempoMap = TempoMap.createTempoMap();
        tempoMap.addTempo(0.0, "80.0", 0.25);   // slow start
        tempoMap.addTempo(960.0, "120.0", 0.25); // speed up
        dated.addMap(tempoMap);
        
        // Add articulation map
        ArticulationMap articulationMap = ArticulationMap.createArticulationMap();
        articulationMap.addArticulation(480.0, "staccato", null, null);
        dated.addMap(articulationMap);
        
        System.out.println("Created MPM with dynamics, tempo, and articulation maps");
        return mpm;
    }
    
    /**
     * Create expressive MPM for musical phrase test.
     */
    private static Mpm createExpressiveMpm() {
        Mpm mpm = MpmTestUtils.createBasicMpm();
        Performance performance = mpm.getPerformance(0);
        
        // Add piano part
        Part part1 = Part.createPart("Piano", 1, 0, 0);
        performance.addPart(part1);
        Dated dated1 = part1.getDated();
        
        // Add dynamics for piano
        DynamicsMap dynamicsMap1 = DynamicsMap.createDynamicsMap();
        dynamicsMap1.addDynamics(0.0, "80.0");    // mezzo-piano
        dated1.addMap(dynamicsMap1);
        
        // Add violin part
        Part part2 = Part.createPart("Violin", 2, 1, 0);
        performance.addPart(part2);
        Dated dated2 = part2.getDated();
        
        // Add dynamics for violin with crescendo
        DynamicsMap dynamicsMap2 = DynamicsMap.createDynamicsMap();
        dynamicsMap2.addDynamics(0.0, "70.0");    // start softer
        dynamicsMap2.addDynamics(1440.0, "100.0"); // crescendo to forte
        dated2.addMap(dynamicsMap2);
        
        // Add global tempo map
        TempoMap globalTempoMap = TempoMap.createTempoMap();
        globalTempoMap.addTempo(0.0, "100.0", 0.25);
        performance.getGlobal().getDated().addMap(globalTempoMap);
        
        System.out.println("Created expressive MPM with multi-part dynamics and global tempo");
        return mpm;
    }
    
    /**
     * Verify integration test results.
     */
    private static boolean verifyIntegrationResults(Msm msm) {
        Nodes notes = msm.getRootElement().query("//note");
        boolean foundDynamicsEffect = false;
        boolean foundTempoEffect = false;
        boolean foundArticulationEffect = false;
        
        for (int i = 0; i < notes.size(); i++) {
            Element note = (Element) notes.get(i);
            
            // Check for dynamics (velocity changes)
            if (note.getAttribute("velocity") != null) {
                double velocity = Double.parseDouble(note.getAttributeValue("velocity"));
                if (velocity != 100.0) { // Original was 100, so any change indicates dynamics
                    foundDynamicsEffect = true;
                    System.out.println("Found dynamics effect: velocity = " + velocity);
                }
            }
            
            // Check for tempo effect (millisecond timing)
            if (note.getAttribute("milliseconds.date") != null) {
                foundTempoEffect = true;
            }
            
            // Check for performance processing (indicates articulation was processed)
            if (note.getAttribute("date.perf") != null) {
                foundArticulationEffect = true;
            }
        }
        
        System.out.println("Integration effects - Dynamics: " + foundDynamicsEffect + 
                          ", Tempo: " + foundTempoEffect + 
                          ", Articulation: " + foundArticulationEffect);
        
        return foundDynamicsEffect && foundTempoEffect && foundArticulationEffect;
    }
    
    /**
     * Verify expressive performance results.
     */
    private static boolean verifyExpressiveResults(Msm msm) {
        Nodes notes = msm.getRootElement().query("//note");
        boolean foundMultiPartExpression = false;
        int notesWithVelocityVariation = 0;
        
        for (int i = 0; i < notes.size(); i++) {
            Element note = (Element) notes.get(i);
            
            if (note.getAttribute("velocity") != null) {
                double velocity = Double.parseDouble(note.getAttributeValue("velocity"));
                if (velocity != 100.0) { // Any change from original
                    notesWithVelocityVariation++;
                }
                System.out.println("Note " + i + " velocity: " + velocity);
            }
        }
        
        // Check if we have velocity variation indicating expressive performance
        foundMultiPartExpression = notesWithVelocityVariation > 0;
        
        System.out.println("Notes with velocity variation: " + notesWithVelocityVariation);
        return foundMultiPartExpression;
    }
    
    /**
     * Run all integration tests.
     */
    public static void runAllTests() {
        System.out.println("Running MPM Integration Tests...");
        System.out.println("=================================");
        
        testMultipleMapIntegration();
        testExpressiveMusicalPhrase();
        
        System.out.println("MPM Integration Tests Completed.");
        System.out.println();
    }
    
    /**
     * Main method for running tests independently.
     */
    public static void main(String[] args) {
        runAllTests();
    }
}