#include <iostream>
#include <cmath>
#include <iomanip>

// Test RubatoMap algorithm with different parameters to match expected results
double computeRubatoTransformation(double date, double startDate, double frameLength, double intensity, double lateStart, double earlyEnd) {
    // Port exact Java algorithm from lines 335-338 of RubatoMap.java
    double localDate = fmod(date - startDate, frameLength);
    double d = (std::pow(localDate / frameLength, intensity) * (earlyEnd - lateStart) + lateStart) * frameLength;
    
    // Java formula: return date + d - localDate;
    return date + d - localDate;
}

int main() {
    std::cout << "RubatoMap Algorithm Parameter Testing" << std::endl;
    std::cout << "Expected results: 120→120.754, 240→215.146, 360→319.897\n" << std::endl;
    
    // Test different parameter combinations
    struct TestCase {
        double frameLength;
        double intensity;
        double lateStart;
        double earlyEnd;
        std::string description;
    };
    
    TestCase tests[] = {
        {480.0, 1.0, 0.0, 1.0, "intensity=1.0, full range"},
        {480.0, 2.0, 0.0, 1.0, "intensity=2.0, full range"},
        {480.0, 0.5, 0.0, 1.0, "intensity=0.5, full range"},
        {480.0, 1.0, 0.1, 0.9, "intensity=1.0, partial range"},
        {480.0, 1.5, 0.1, 0.9, "intensity=1.5, partial range"},
        {480.0, 2.0, 0.2, 0.8, "intensity=2.0, centered range"},
        {360.0, 1.0, 0.0, 1.0, "shorter frame, intensity=1.0"},
        {480.0, 1.0, -0.1, 1.1, "intensity=1.0, extended range"},
    };
    
    double testDates[] = {120.0, 240.0, 360.0};
    double expectedResults[] = {120.754, 215.146, 319.897};
    
    for (const auto& test : tests) {
        std::cout << "Test: " << test.description << std::endl;
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
        std::cout << std::endl;
    }
    
    // Let's also test the original comprehensive test parameters
    std::cout << "Original comprehensive test parameters:" << std::endl;
    std::cout << "frameLength=480.0, intensity=0.1, lateStart=0.1, earlyEnd=0.9" << std::endl;
    
    for (int i = 0; i < 3; i++) {
        double result = computeRubatoTransformation(testDates[i], 0.0, 480.0, 0.1, 0.1, 0.9);
        std::cout << "  " << testDates[i] << " → " << std::fixed << std::setprecision(3) 
                  << result << " (expected " << expectedResults[i] << ")" << std::endl;
    }
    
    return 0;
}