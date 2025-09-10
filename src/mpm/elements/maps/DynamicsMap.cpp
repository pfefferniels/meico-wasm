#include "mpm/elements/maps/DynamicsMap.h"
#include "mpm/Mpm.h"
#include "xml/Helper.h"
#include <algorithm>

namespace meico {
namespace mpm {

DynamicsMap::DynamicsMap() : GenericMap(Mpm::DYNAMICS_MAP) {
}

std::unique_ptr<DynamicsMap> DynamicsMap::createDynamicsMap() {
    return std::make_unique<DynamicsMap>();
}

void DynamicsMap::addDynamics(double date, double dynamics) {
    dynamicsData.emplace_back(date, dynamics);
    
    // Keep data sorted by date for efficient lookup
    std::sort(dynamicsData.begin(), dynamicsData.end(), 
              [](const auto& a, const auto& b) { return a.getKey() < b.getKey(); });
}

double DynamicsMap::getDynamicsAt(double date) const {
    if (dynamicsData.empty()) {
        return 100.0; // Default velocity
    }
    
    // Find the appropriate dynamics value
    for (size_t i = 0; i < dynamicsData.size(); ++i) {
        if (dynamicsData[i].getKey() >= date) {
            if (i == 0) {
                return dynamicsData[i].getValue();
            }
            
            // Interpolate between previous and current
            const auto& prev = dynamicsData[i - 1];
            const auto& curr = dynamicsData[i];
            return interpolate(prev.getKey(), prev.getValue(), 
                             curr.getKey(), curr.getValue(), date);
        }
    }
    
    // Use last value if date is beyond all entries
    return dynamicsData.back().getValue();
}

bool DynamicsMap::applyToMsmPart(Element msmPart) {
    if (!msmPart || dynamicsData.empty()) {
        return false;
    }
    
    bool modified = false;
    
    // Find all note elements in the part
    auto scoreElement = xml::Helper::getFirstChildElement(msmPart, "dated");
    if (scoreElement) {
        scoreElement = xml::Helper::getFirstChildElement(scoreElement, "score");
        if (scoreElement) {
            // Process all notes
            for (auto note : scoreElement.children("note")) {
                auto dateAttr = note.attribute("date");
                if (dateAttr) {
                    double noteDate = xml::Helper::parseDouble(dateAttr.value());
                    double newVelocity = getDynamicsAt(noteDate);
                    
                    // Update or add velocity attribute
                    auto velocityAttr = note.attribute("velocity");
                    if (velocityAttr) {
                        velocityAttr.set_value(std::to_string(newVelocity).c_str());
                    } else {
                        note.append_attribute("velocity") = std::to_string(newVelocity).c_str();
                    }
                    modified = true;
                }
            }
        }
    }
    
    return modified;
}

void DynamicsMap::parseData(const Element& xmlElement) {
    GenericMap::parseData(xmlElement);
    
    // Parse dynamics entries from XML (simplified implementation)
    for (auto child : xmlElement.children()) {
        if (std::string(child.name()) == "dynamics") {
            auto dateAttr = child.attribute("date");
            auto valueAttr = child.attribute("value");
            if (dateAttr && valueAttr) {
                double date = xml::Helper::parseDouble(dateAttr.value());
                double value = xml::Helper::parseDouble(valueAttr.value());
                addDynamics(date, value);
            }
        }
    }
}

double DynamicsMap::interpolate(double x0, double y0, double x1, double y1, double x) const {
    if (x1 == x0) {
        return y0;
    }
    return y0 + (y1 - y0) * (x - x0) / (x1 - x0);
}

} // namespace mpm
} // namespace meico