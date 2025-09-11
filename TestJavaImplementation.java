import meico.msm.Msm;
import meico.mpm.Mpm;
import meico.mpm.elements.Performance;
import meico.xml.Helper;
import java.io.File;

public class TestJavaImplementation {
    public static void main(String[] args) {
        System.out.println("Testing Java MPM->MSM with real files...");
        
        try {
            // Load the MSM file
            System.out.println("Loading MSM file: test/bwv1007.msm");
            Msm msm = new Msm(new File("test/bwv1007.msm"));
            System.out.println("✓ MSM loaded successfully");
            System.out.println("  Title: " + msm.getTitle());
            System.out.println("  PPQ: " + msm.getPPQ());
            
            // Load the MPM file
            System.out.println("\nLoading MPM file: test/bwv1007.mpm");
            Mpm mpm = new Mpm(new File("test/bwv1007.mpm"));
            System.out.println("✓ MPM loaded successfully");
            System.out.println("  Number of performances: " + mpm.size());
            
            if (mpm.size() > 0) {
                Performance performance = mpm.getPerformance(0);
                if (performance != null) {
                    System.out.println("  Performance name: " + performance.getName());
                    System.out.println("  Performance PPQ: " + performance.getPPQ());
                    
                    // Apply the performance to the MSM
                    System.out.println("\nApplying MPM performance to MSM...");
                    Msm resultMsm = performance.perform(msm);
                    if (resultMsm != null) {
                        System.out.println("✓ Performance application completed successfully!");
                        System.out.println("  Result title: " + resultMsm.getTitle());
                        
                        // Write result to file for comparison
                        System.out.println("✓ Generated result MSM successfully");
                        
                        // Save result to file for comparison with C++ implementation
                        String outputFile = "test/bwv1007_java_result.msm";
                        resultMsm.setFile(new File(outputFile));
                        if (resultMsm.writeMsm()) {
                            System.out.println("✓ Saved Java result to: " + outputFile);
                        } else {
                            System.out.println("⚠ Could not save Java result to file");
                        }
                    } else {
                        System.out.println("✗ Performance application failed");
                    }
                } else {
                    System.out.println("✗ No performance found");
                }
            } else {
                System.out.println("✗ No performances found in MPM");
            }
            
        } catch (Exception e) {
            System.err.println("Error: " + e.getMessage());
            e.printStackTrace();
        }
    }
}