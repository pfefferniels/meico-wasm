package meico.mpm;

/**
 * Main test runner for all MPM functionality tests.
 * This class runs comprehensive tests for all MPM map types and styles.
 * 
 * @author Test Framework
 */
public class MpmTestRunner {
    
    /**
     * Run all MPM tests.
     */
    public static void runAllTests() {
        System.out.println("====================================================");
        System.out.println("         MPM (Musical Performance Markup) Tests");
        System.out.println("====================================================");
        System.out.println();
        
        // Test individual map types
        ArticulationMapTest.runAllTests();
        DynamicsMapTest.runAllTests();
        TempoMapTest.runAllTests();
        MetricalAccentuationMapTest.runAllTests();
        
        // TODO: Add more map type tests as they are implemented
        // RubatoMapTest.runAllTests();
        // MetricalAccentuationMapTest.runAllTests();
        // OrnamentationMapTest.runAllTests();
        // MovementMapTest.runAllTests();
        // AsynchronyMapTest.runAllTests();
        // ImprecisionMapTest.runAllTests();
        
        // TODO: Add style tests
        // ArticulationStyleTest.runAllTests();
        // DynamicsStyleTest.runAllTests();
        // TempoStyleTest.runAllTests();
        
        // TODO: Add integration tests
        // MpmIntegrationTest.runAllTests();
        
        System.out.println("====================================================");
        System.out.println("           All MPM Tests Completed");
        System.out.println("====================================================");
    }
    
    /**
     * Main method for running all tests.
     */
    public static void main(String[] args) {
        runAllTests();
    }
}