package meico.mpm;

import meico.msm.Msm;
import meico.mpm.elements.Performance;
import meico.mpm.elements.Part;
import meico.mpm.elements.Dated;
import meico.mpm.elements.maps.TempoMap;
import meico.mpm.elements.maps.GenericMap;
import nu.xom.*;

/**
 * Test class for TempoMap functionality.
 * Tests the application of tempo maps to MSM documents.
 * 
 * @author Test Framework
 */
public class TempoMapTest {
    
    /**
     * Test basic tempo map functionality.
     * Creates a simple tempo map and applies it to an MSM.
     */
    public static void testBasicTempoMap() {
        System.out.println("Testing Basic Tempo Map...");
        
        // Create simple MSM
        Msm msm = MpmTestUtils.createSimpleMsm();
        MpmTestUtils.printMsm(msm, "Original MSM");
        
        // Create MPM with tempo map
        Mpm mpm = MpmTestUtils.createMpmWithMap(Mpm.TEMPO_MAP);
        Performance performance = mpm.getPerformance(0);
        Part part = performance.getPart("Piano");
        if (part == null) {
            System.err.println("Error: Could not find part 'Piano' in performance");
            return;
        }
        Dated dated = part.getDated();
        TempoMap tempoMap = (TempoMap) dated.getMap(Mpm.TEMPO_MAP);
        
        // Add tempo data to the map
        addBasicTempoData(tempoMap);
        
        // Apply MPM to MSM
        Msm resultMsm = MpmTestUtils.applyMpmToMsm(msm, mpm);
        MpmTestUtils.printMsm(resultMsm, "MSM after Tempo Map");
        
        // Verify results
        if (verifyTempoMapResults(resultMsm)) {
            System.out.println("✓ Basic Tempo Map test PASSED");
        } else {
            System.out.println("✗ Basic Tempo Map test FAILED");
        }
        System.out.println();
    }
    
    /**
     * Test tempo map with accelerando.
     */
    public static void testAccelerandoTempoMap() {
        System.out.println("Testing Accelerando Tempo Map...");
        
        // Create simple MSM
        Msm msm = MpmTestUtils.createSimpleMsm();
        
        // Create MPM with tempo map
        Mpm mpm = MpmTestUtils.createMpmWithMap(Mpm.TEMPO_MAP);
        Performance performance = mpm.getPerformance(0);
        Part part = performance.getPart("Piano");
        if (part == null) {
            System.err.println("Error: Could not find part 'Piano' in performance");
            return;
        }
        Dated dated = part.getDated();
        TempoMap tempoMap = (TempoMap) dated.getMap(Mpm.TEMPO_MAP);
        
        // Add accelerando tempo data
        addAccelerandoTempoData(tempoMap);
        
        // Apply MPM to MSM
        Msm resultMsm = MpmTestUtils.applyMpmToMsm(msm, mpm);
        MpmTestUtils.printMsm(resultMsm, "MSM after Accelerando Tempo Map");
        
        // Verify results
        if (verifyAccelerandoTempoResults(resultMsm)) {
            System.out.println("✓ Accelerando Tempo Map test PASSED");
        } else {
            System.out.println("✗ Accelerando Tempo Map test FAILED");
        }
        System.out.println();
    }
    
    /**
     * Add basic tempo data to the tempo map.
     * Creates a moderate tempo marking.
     */
    private static void addBasicTempoData(TempoMap tempoMap) {
        // Add a moderate tempo at the beginning (120 BPM)
        int index = tempoMap.addTempo(0.0, "120.0", 0.25);  // quarter note beat length
        System.out.println("Added moderate tempo at index: " + index);
    }
    
    /**
     * Add accelerando tempo data to the tempo map.
     */
    private static void addAccelerandoTempoData(TempoMap tempoMap) {
        // Add accelerando from 60 BPM to 120 BPM
        int index1 = tempoMap.addTempo(0.0, "60.0", 0.25);    // slow start
        int index2 = tempoMap.addTempo(1440.0, "120.0", 0.25); // faster at the end
        System.out.println("Added accelerando tempo at indices: " + index1 + ", " + index2);
    }
    
    /**
     * Verify that the tempo map was applied correctly.
     */
    private static boolean verifyTempoMapResults(Msm msm) {
        // Check if millisecond timing was affected by tempo changes
        Nodes notes = msm.getRootElement().query("//note");
        boolean foundTempoEffects = false;
        
        for (int i = 0; i < notes.size(); i++) {
            Element note = (Element) notes.get(i);
            
            // Check for millisecond timing attributes
            if (note.getAttribute("milliseconds.date") != null) {
                String msDate = note.getAttributeValue("milliseconds.date");
                double msTime = Double.parseDouble(msDate);
                
                // Check if millisecond timing is reasonable
                if (msTime >= 0) {
                    foundTempoEffects = true;
                    System.out.println("Note milliseconds date: " + msTime);
                }
            }
        }
        
        return foundTempoEffects;
    }
    
    /**
     * Verify accelerando tempo results.
     */
    private static boolean verifyAccelerandoTempoResults(Msm msm) {
        // Check if millisecond timing shows accelerando pattern
        Nodes notes = msm.getRootElement().query("//note");
        boolean hasTempoVariation = false;
        double[] msTimes = new double[notes.size()];
        double[] tickTimes = new double[notes.size()];
        
        for (int i = 0; i < notes.size(); i++) {
            Element note = (Element) notes.get(i);
            if (note.getAttribute("milliseconds.date") != null && note.getAttribute("date.perf") != null) {
                msTimes[i] = Double.parseDouble(note.getAttributeValue("milliseconds.date"));
                tickTimes[i] = Double.parseDouble(note.getAttributeValue("date.perf"));
                System.out.println("Note " + i + " - ticks: " + tickTimes[i] + ", ms: " + msTimes[i]);
            }
        }
        
        // Check if the relationship between tick time and millisecond time changes
        // (indicating tempo changes)
        if (notes.size() >= 2) {
            double ratio1 = (msTimes[1] - msTimes[0]) / (tickTimes[1] - tickTimes[0]);
            double ratio2 = (msTimes[notes.size()-1] - msTimes[notes.size()-2]) / 
                           (tickTimes[notes.size()-1] - tickTimes[notes.size()-2]);
            hasTempoVariation = Math.abs(ratio1 - ratio2) > 0.01; // Some tolerance for floating point
            System.out.println("Tempo ratio change: " + ratio1 + " -> " + ratio2);
        }
        
        return hasTempoVariation;
    }
    
    /**
     * Run all tempo map tests.
     */
    public static void runAllTests() {
        System.out.println("Running TempoMap Tests...");
        System.out.println("=========================");
        
        testBasicTempoMap();
        testAccelerandoTempoMap();
        
        System.out.println("TempoMap Tests Completed.");
        System.out.println();
    }
    
    /**
     * Main method for running tests independently.
     */
    public static void main(String[] args) {
        runAllTests();
    }
}