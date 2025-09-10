#include <iostream>
#include <cmath>

int main() {
    // Test the rubato algorithm with the same values as the comprehensive test
    double date = 120.0;
    double startDate = 0.0;
    double frameLength = 480.0;
    double intensity = 2.0;
    double lateStart = 0.0;  // corrected to 0.0
    double earlyEnd = 1.0;   // corrected to 1.0
    
    std::cout << "Rubato transformation debug:" << std::endl;
    std::cout << "Input: date=" << date << ", startDate=" << startDate << ", frameLength=" << frameLength << std::endl;
    std::cout << "Parameters: intensity=" << intensity << ", lateStart=" << lateStart << ", earlyEnd=" << earlyEnd << std::endl;
    
    double localDate = fmod(date - startDate, frameLength);
    std::cout << "localDate = fmod(" << date << " - " << startDate << ", " << frameLength << ") = " << localDate << std::endl;
    
    double t = localDate / frameLength;
    std::cout << "t = " << localDate << " / " << frameLength << " = " << t << std::endl;
    
    double transformedT = std::pow(t, intensity);
    std::cout << "transformedT = pow(" << t << ", " << intensity << ") = " << transformedT << std::endl;
    
    double transformedLocalDate = (transformedT * (earlyEnd - lateStart) + lateStart) * frameLength;
    std::cout << "transformedLocalDate = (" << transformedT << " * (" << earlyEnd << " - " << lateStart << ") + " << lateStart << ") * " << frameLength << " = " << transformedLocalDate << std::endl;
    
    double result = startDate + transformedLocalDate;
    std::cout << "result = " << startDate << " + " << transformedLocalDate << " = " << result << std::endl;
    
    std::cout << "\nExpected: 120 → 120.754" << std::endl;
    std::cout << "Actual: " << date << " → " << result << std::endl;
    
    return 0;
}