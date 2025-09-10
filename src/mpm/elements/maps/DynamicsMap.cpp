#include "mpm/elements/maps/DynamicsMap.h"
#include "mpm/elements/maps/data/DynamicsData.h"
#include "mpm/Mpm.h"
#include "xml/Helper.h"
#include <algorithm>
#include <limits>

namespace meico {
namespace mpm {

DynamicsMap::DynamicsMap() : GenericMap(Mpm::DYNAMICS_MAP) {
}

std::unique_ptr<DynamicsMap> DynamicsMap::createDynamicsMap() {
    return std::make_unique<DynamicsMap>();
}

void DynamicsMap::addDynamics(double date, const std::string& volume, const std::string& transitionTo, 
                              double curvature, double protraction, bool subNoteDynamics,
                              const std::string& id) {
    auto data = std::make_unique<DynamicsData>();
    data->startDate = date;
    data->volumeString = volume;
    
    // Try to parse volume as numeric value
    try {
        data->volume = xml::Helper::parseDouble(volume);
    } catch (...) {
        // Keep as string for style resolution
        data->volume = 0.0;
    }
    
    if (!transitionTo.empty()) {
        data->transitionToString = transitionTo;
        try {
            data->transitionTo = xml::Helper::parseDouble(transitionTo);
        } catch (...) {
            // Keep as string for style resolution
            data->transitionTo = 0.0;
        }
    } else {
        // No transition specified - constant dynamics
        data->transitionToString = volume;
        data->transitionTo = data->volume;
    }
    
    data->curvature = std::max(0.0, std::min(1.0, curvature));
    data->protraction = std::max(-1.0, std::min(1.0, protraction));
    data->subNoteDynamics = subNoteDynamics;
    data->xmlId = id;
    
    addDynamics(std::move(data));
}

void DynamicsMap::addDynamics(std::unique_ptr<DynamicsData> data) {
    double date = data->startDate;
    
    // Find insertion point to keep sorted by date
    auto it = std::lower_bound(dynamicsData.begin(), dynamicsData.end(), date,
        [](const auto& item, double d) { return item.getKey() < d; });
    
    // Insert at the correct position
    dynamicsData.emplace(it, date, std::move(data));
    
    // Update end dates for all elements
    for (size_t i = 0; i < dynamicsData.size(); ++i) {
        dynamicsData[i].getValue()->endDate = getEndDate(i);
    }
}

DynamicsData* DynamicsMap::getDynamicsDataAt(double date) const {
    for (int i = getElementIndexBeforeAt(date); i >= 0; --i) {
        DynamicsData* dd = getDynamicsDataOf(i);
        if (dd != nullptr) {
            return dd;
        }
    }
    return nullptr;
}

DynamicsData* DynamicsMap::getDynamicsDataOf(int index) const {
    if (dynamicsData.empty() || (index < 0) || (index >= static_cast<int>(dynamicsData.size()))) {
        return nullptr;
    }
    
    return dynamicsData[index].getValue().get();
}

double DynamicsMap::getDynamicsAt(double date) const {
    DynamicsData* dd = getDynamicsDataAt(date);
    if (dd == nullptr) {
        return 100.0; // Default velocity
    }
    
    return dd->getDynamicsAt(date);
}

bool DynamicsMap::applyToMsmPart(Element msmPart) {
    if (!msmPart || dynamicsData.empty()) {
        return false;
    }
    
    bool modified = false;
    
    // Find all note elements in the part - try both direct dated and header/dated paths
    Element scoreElement;
    
    // First try: part -> dated -> score (for some MSM structures)
    scoreElement = xml::Helper::getFirstChildElement(msmPart, "dated");
    if (scoreElement) {
        scoreElement = xml::Helper::getFirstChildElement(scoreElement, "score");
    }
    
    // If not found, try: part -> header -> dated -> score (for Bach MSM structure)
    if (!scoreElement) {
        auto headerElement = xml::Helper::getFirstChildElement(msmPart, "header");
        if (headerElement) {
            auto datedElement = xml::Helper::getFirstChildElement(headerElement, "dated");
            if (datedElement) {
                scoreElement = xml::Helper::getFirstChildElement(datedElement, "score");
            }
        }
    }
    
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
    
    return modified;
}

void DynamicsMap::parseData(const Element& xmlElement) {
    GenericMap::parseData(xmlElement);
    
    // Parse dynamics entries from XML
    for (auto child : xmlElement.children()) {
        if (std::string(child.name()) == "dynamics") {
            auto data = std::make_unique<DynamicsData>(child);
            addDynamics(std::move(data));
        }
    }
}

int DynamicsMap::getElementIndexBeforeAt(double date) const {
    for (int i = static_cast<int>(dynamicsData.size()) - 1; i >= 0; --i) {
        if (dynamicsData[i].getKey() <= date) {
            return i;
        }
    }
    return -1;
}

double DynamicsMap::getEndDate(int index) const {
    // Get the date of the subsequent dynamics element
    double endDate = std::numeric_limits<double>::max();
    for (int j = index + 1; j < static_cast<int>(dynamicsData.size()); ++j) {
        endDate = dynamicsData[j].getKey();
        break;
    }
    return endDate;
}

} // namespace mpm
} // namespace meico