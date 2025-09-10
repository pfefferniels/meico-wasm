#include <iostream>
#include <cmath>
#include <iomanip>
#include <cstdlib>

// Test RubatoMap algorithm to find parameters that match expected results
double computeRubatoTransformation(double date, double startDate, double frameLength, double intensity, double lateStart, double earlyEnd) {
    // Port exact Java algorithm
    double localDate = fmod(date - startDate, frameLength);
    double d = (std::pow(localDate / frameLength, intensity) * (earlyEnd - lateStart) + lateStart) * frameLength;
    return date + d - localDate;
}

int main() {
    std::cout << "Finding RubatoMap parameters for expected results" << std::endl;
    std::cout << "Expected: 120→120.754, 240→215.146, 360→319.897\n" << std::endl;
    
    double testDates[] = {120.0, 240.0, 360.0};
    double expectedResults[] = {120.754, 215.146, 319.897};
    
    // Let's systematically try different parameter ranges
    double bestError = 1000000.0;
    double bestFrameLength = 0, bestIntensity = 0, bestLateStart = 0, bestEarlyEnd = 0;
    
    // Try different frame lengths that make sense
    for (double frameLength = 120.0; frameLength <= 960.0; frameLength += 120.0) {
        // Try different intensities
        for (double intensity = 0.1; intensity <= 3.0; intensity += 0.1) {
            // Try different lateStart/earlyEnd combinations
            for (double lateStart = -0.5; lateStart <= 0.5; lateStart += 0.1) {
                for (double earlyEnd = lateStart + 0.1; earlyEnd <= 1.5; earlyEnd += 0.1) {
                    
                    double totalError = 0.0;
                    bool validResults = true;
                    
                    for (int i = 0; i < 3; i++) {
                        double result = computeRubatoTransformation(testDates[i], 0.0, frameLength, intensity, lateStart, earlyEnd);
                        double error = std::abs(result - expectedResults[i]);
                        totalError += error;
                        
                        // Skip if results are wildly off
                        if (error > 100.0) {
                            validResults = false;
                            break;
                        }
                    }
                    
                    if (validResults && totalError < bestError) {
                        bestError = totalError;
                        bestFrameLength = frameLength;
                        bestIntensity = intensity;
                        bestLateStart = lateStart;
                        bestEarlyEnd = earlyEnd;
                    }
                }
            }
        }
    }
    
    std::cout << "Best match found:" << std::endl;
    std::cout << "  frameLength=" << bestFrameLength << ", intensity=" << bestIntensity 
              << ", lateStart=" << bestLateStart << ", earlyEnd=" << bestEarlyEnd << std::endl;
    std::cout << "  Total error: " << bestError << std::endl;
    
    // Show the results with best parameters
    std::cout << "\nResults with best parameters:" << std::endl;
    for (int i = 0; i < 3; i++) {
        double result = computeRubatoTransformation(testDates[i], 0.0, bestFrameLength, bestIntensity, bestLateStart, bestEarlyEnd);
        double error = std::abs(result - expectedResults[i]);
        std::cout << "  " << testDates[i] << " → " << std::fixed << std::setprecision(3) 
                  << result << " (expected " << expectedResults[i] << ", error=" << error << ")" << std::endl;
    }
    
    // Let's also manually test some specific parameter combinations that might make sense
    std::cout << "\nTesting some manual parameter combinations:" << std::endl;
    
    struct TestCase {
        double frameLength;
        double intensity;
        double lateStart;
        double earlyEnd;
        std::string description;
    };
    
    TestCase manualTests[] = {
        {480.0, 1.0, 0.5, 0.5, "frameLength=480, intensity=1, neutral point"},
        {360.0, 0.5, 0.2, 0.8, "frameLength=360, mild intensity"},
        {960.0, 1.5, 0.1, 0.7, "frameLength=960, higher intensity"},
        {240.0, 2.0, 0.0, 0.6, "frameLength=240, strong intensity"},
    };
    
    for (const auto& test : manualTests) {
        std::cout << "\n" << test.description << ":" << std::endl;
        std::cout << "  frameLength=" << test.frameLength << ", intensity=" << test.intensity 
                  << ", lateStart=" << test.lateStart << ", earlyEnd=" << test.earlyEnd << std::endl;
        
        double totalError = 0.0;
        for (int i = 0; i < 3; i++) {
            double result = computeRubatoTransformation(testDates[i], 0.0, test.frameLength, test.intensity, test.lateStart, test.earlyEnd);
            double error = std::abs(result - expectedResults[i]);
            totalError += error;
            
            std::cout << "    " << testDates[i] << " → " << std::fixed << std::setprecision(3) 
                      << result << " (expected " << expectedResults[i] << ", error=" << error << ")" << std::endl;
        }
        std::cout << "  Total error: " << totalError << std::endl;
    }
    
    return 0;
}