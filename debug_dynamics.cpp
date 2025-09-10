#include <iostream>
#include "mpm/elements/maps/DynamicsMap.h"

using namespace meico;

int main() {
    std::cout << "Testing DynamicsMap directly..." << std::endl;
    
    auto dynamicsMap = mpm::DynamicsMap::createDynamicsMap();
    
    // Add the same dynamics as in our Bach test
    dynamicsMap->addDynamics(0.0, "40", "100", 0.5, 0.0);
    dynamicsMap->addDynamics(960.0, "100", "", 0.0, 0.0);
    
    std::cout << "Added dynamics data" << std::endl;
    
    // Test getDynamicsAt for our note dates
    std::vector<double> testDates = {0.0, 240.0, 480.0, 720.0, 960.0};
    
    for (double date : testDates) {
        double velocity = dynamicsMap->getDynamicsAt(date);
        std::cout << "Date: " << date << ", Velocity: " << velocity << std::endl;
    }
    
    return 0;
}