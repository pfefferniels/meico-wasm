package meico.mpm;

import meico.msm.Msm;
import meico.mpm.Mpm;
import meico.mpm.elements.Performance;
import meico.mpm.elements.Part;
import meico.mpm.elements.Header;
import meico.mpm.elements.Global;
import meico.mpm.elements.Dated;
import meico.mpm.elements.maps.*;
import meico.xml.Helper;
import nu.xom.*;

/**
 * Utility class for creating test MSM and MPM documents for testing MPM functionality.
 * This class provides methods to create simple musical structures for testing purposes.
 * 
 * @author Test Framework
 */
public class MpmTestUtils {
    
    /**
     * Creates a simple MSM document with basic musical content for testing.
     * Contains a single part with a few notes at different time positions.
     * 
     * @return Simple MSM document
     */
    public static Msm createSimpleMsm() {
        Msm msm = new Msm();
        
        // Create root element
        Element root = new Element("msm", "http://www.cemfi.de/msm/ns/1.0");
        root.addAttribute(new Attribute("title", "Test MSM"));
        root.addAttribute(new Attribute("pulsesPerQuarter", "480")); // Standard MIDI PPQ
        Document doc = new Document(root);
        msm.setDocument(doc);
        
        // Set a dummy file path
        msm.setFile(new java.io.File("test.msm"));
        
        // Add global element
        Element global = new Element("global");
        root.appendChild(global);
        
        // Add header
        Element header = new Element("header");
        global.appendChild(header);
        
        // Add part
        Element part = new Element("part");
        part.addAttribute(new Attribute("name", "Piano"));
        part.addAttribute(new Attribute("number", "1"));
        part.addAttribute(new Attribute("midi.channel", "0"));
        part.addAttribute(new Attribute("midi.port", "0"));
        root.appendChild(part);
        
        // Add header to part
        Element partHeader = new Element("header");
        part.appendChild(partHeader);
        
        // Add dated element
        Element dated = new Element("dated");
        part.appendChild(dated);
        
        // Create a simple score map with a few notes
        Element scoreMap = new Element("score");
        scoreMap.addAttribute(new Attribute("id", "score"));
        dated.appendChild(scoreMap);
        
        // Add some test notes
        addNoteToScore(scoreMap, 0.0, "C4", 60, 480.0, 100.0);
        addNoteToScore(scoreMap, 480.0, "D4", 62, 480.0, 100.0);
        addNoteToScore(scoreMap, 960.0, "E4", 64, 480.0, 100.0);
        addNoteToScore(scoreMap, 1440.0, "F4", 65, 480.0, 100.0);
        
        return msm;
    }
    
    /**
     * Creates a simple MSM document with multiple parts for testing.
     * 
     * @return MSM document with multiple parts
     */
    public static Msm createMultiPartMsm() {
        Msm msm = createSimpleMsm();
        Element root = msm.getRootElement();
        
        // Add second part
        Element part2 = new Element("part");
        part2.addAttribute(new Attribute("name", "Violin"));
        part2.addAttribute(new Attribute("number", "2"));
        part2.addAttribute(new Attribute("midi.channel", "1"));
        part2.addAttribute(new Attribute("midi.port", "0"));
        root.appendChild(part2);
        
        // Add header to part2
        Element partHeader2 = new Element("header");
        part2.appendChild(partHeader2);
        
        // Add dated element to part2
        Element dated2 = new Element("dated");
        part2.appendChild(dated2);
        
        // Create score map for part2
        Element scoreMap2 = new Element("score");
        scoreMap2.addAttribute(new Attribute("id", "score"));
        dated2.appendChild(scoreMap2);
        
        // Add some test notes to part2 (higher octave)
        addNoteToScore(scoreMap2, 0.0, "C5", 72, 480.0, 90.0);
        addNoteToScore(scoreMap2, 480.0, "D5", 74, 480.0, 90.0);
        addNoteToScore(scoreMap2, 960.0, "E5", 76, 480.0, 90.0);
        addNoteToScore(scoreMap2, 1440.0, "F5", 77, 480.0, 90.0);
        
        return msm;
    }
    
    /**
     * Helper method to add a note to a score map.
     */
    private static void addNoteToScore(Element scoreMap, double date, String name, int pitch, double duration, double velocity) {
        Element note = new Element("note");
        note.addAttribute(new Attribute("date", String.valueOf(date)));
        note.addAttribute(new Attribute("name", name));
        note.addAttribute(new Attribute("midi.pitch", String.valueOf(pitch)));
        note.addAttribute(new Attribute("duration", String.valueOf(duration)));
        note.addAttribute(new Attribute("velocity", String.valueOf(velocity)));
        scoreMap.appendChild(note);
    }
    
    /**
     * Creates a basic MPM document structure.
     * 
     * @return Basic MPM document
     */
    public static Mpm createBasicMpm() {
        Mpm mpm = Mpm.createMpm();
        
        // Add a performance
        Performance performance = Performance.createPerformance("Test Performance");
        mpm.addPerformance(performance);
        
        return mpm;
    }
    
    /**
     * Creates an MPM document with a specific map type for testing.
     * 
     * @param mapType The type of map to create
     * @return MPM document with the specified map
     */
    public static Mpm createMpmWithMap(String mapType) {
        Mpm mpm = createBasicMpm();
        Performance performance = mpm.getPerformance(0);
        
        // Add part
        Part part = Part.createPart("Piano", 1, 0, 0);
        performance.addPart(part);
        
        // Get dated element
        Dated dated = part.getDated();
        
        // Create and add the specified map type using the appropriate factory
        GenericMap map;
        switch (mapType) {
            case Mpm.ARTICULATION_MAP:
                map = ArticulationMap.createArticulationMap();
                break;
            case Mpm.DYNAMICS_MAP:
                map = DynamicsMap.createDynamicsMap();
                break;
            case Mpm.TEMPO_MAP:
                map = TempoMap.createTempoMap();
                break;
            case Mpm.METRICAL_ACCENTUATION_MAP:
                map = MetricalAccentuationMap.createMetricalAccentuationMap();
                break;
            case Mpm.ORNAMENTATION_MAP:
                map = OrnamentationMap.createOrnamentationMap();
                break;
            default:
                map = GenericMap.createGenericMap(mapType);
                break;
        }
        dated.addMap(map);
        
        return mpm;
    }
    
    /**
     * Applies an MPM performance to an MSM and returns the result.
     * 
     * @param msm The input MSM
     * @param mpm The MPM to apply
     * @return The modified MSM
     */
    public static Msm applyMpmToMsm(Msm msm, Mpm mpm) {
        if (mpm.size() == 0) {
            return msm; // No performances to apply
        }
        
        Performance performance = mpm.getPerformance(0);
        return performance.perform(msm);
    }
    
    /**
     * Verifies that an MSM contains expected note modifications.
     * 
     * @param msm The MSM to check
     * @param expectedModifications What modifications to look for
     * @return true if modifications are found
     */
    public static boolean verifyMsmModifications(Msm msm, String... expectedModifications) {
        // Get all note elements from the MSM
        Nodes notes = msm.getRootElement().query("//note");
        
        for (String modification : expectedModifications) {
            boolean found = false;
            for (int i = 0; i < notes.size(); i++) {
                Element note = (Element) notes.get(i);
                if (note.getAttribute(modification) != null) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                return false;
            }
        }
        
        return true;
    }
    
    /**
     * Prints MSM content for debugging purposes.
     * 
     * @param msm The MSM to print
     * @param label A label for the output
     */
    public static void printMsm(Msm msm, String label) {
        System.out.println("=== " + label + " ===");
        System.out.println(msm.toXML());
        System.out.println();
    }
}