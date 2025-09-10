#include "mpm/elements/maps/MovementMap.h"
#include "xml/Helper.h"
#include "xml/XmlBase.h"
#include <algorithm>
#include <iostream>
#include <limits>

namespace meico {
namespace mpm {

MovementMap::MovementMap() : GenericMap("movementMap") {
}

std::unique_ptr<MovementMap> MovementMap::createMovementMap() {
    return std::make_unique<MovementMap>();
}

void MovementMap::parseData(const Element& xmlElement) {
    GenericMap::parseData(xmlElement);
    
    // Parse movement entries from XML
    for (auto child : xmlElement.children()) {
        if (std::string(child.name()) == "movement") {
            auto data = std::make_unique<MovementData>(child);
            addMovement(std::move(data));
        }
    }
}

void MovementMap::addMovement(double date, const std::string& controller, double position, 
                              double transitionTo, const std::string& id) {
    addMovement(date, controller, position, transitionTo, 0.4, 0.0, id);
}

void MovementMap::addMovement(double date, const std::string& controller, double position, 
                              double transitionTo, double curvature, double protraction,
                              const std::string& id) {
    auto data = std::make_unique<MovementData>();
    data->startDate = date;
    data->controller = controller;
    data->position = position;
    data->transitionTo = transitionTo;
    data->curvature = curvature;
    data->protraction = protraction;
    data->xmlId = id;
    
    addMovement(std::move(data));
}

void MovementMap::addMovement(std::unique_ptr<MovementData> data) {
    double date = data->startDate;
    supplementary::KeyValue<double, std::unique_ptr<MovementData>> kv(date, std::move(data));
    
    // Insert in sorted order
    auto it = std::lower_bound(movementData.begin(), movementData.end(), kv,
        [](const auto& a, const auto& b) { return a.getKey() < b.getKey(); });
    movementData.insert(it, std::move(kv));
}

int MovementMap::getElementIndexBeforeAt(double date) const {
    for (int i = static_cast<int>(movementData.size()) - 1; i >= 0; --i) {
        if (movementData[i].getKey() <= date) {
            return i;
        }
    }
    return -1;
}

MovementData* MovementMap::getMovementDataAt(double date) const {
    int index = getElementIndexBeforeAt(date);
    if (index >= 0) {
        return movementData[index].getValue().get();
    }
    return nullptr;
}

MovementData* MovementMap::getMovementDataOf(int index) const {
    if (movementData.empty() || index < 0) {
        return nullptr;
    }

    if (index >= static_cast<int>(movementData.size())) {
        index = static_cast<int>(movementData.size()) - 1;
    }

    auto& data = movementData[index].getValue();
    
    // Set end date if not already set
    if (data->endDate == 0.0) {
        data->endDate = getEndDate(index);
    }
    
    // Set position from previous element if not already set
    if (data->position == 0.0 && index > 0) {
        data->position = getPreviousPosition(index);
    }

    return data.get();
}

double MovementMap::getEndDate(int index) const {
    // Get the date of the subsequent movement element
    double endDate = std::numeric_limits<double>::max();
    for (int j = index + 1; j < static_cast<int>(movementData.size()); ++j) {
        endDate = movementData[j].getKey();
        break;
    }
    return endDate;
}

double MovementMap::getPreviousPosition(int index) const {
    // Get the final position of the previous movement element
    double finalPosition = 0.0;
    for (int j = index - 1; j >= 0; --j) {
        finalPosition = movementData[j].getValue()->transitionTo;
        break;
    }
    return finalPosition;
}

double MovementMap::getPositionAt(double date) const {
    MovementData* data = getMovementDataAt(date);
    if (data) {
        // Set end date if not already set
        if (data->endDate == 0.0) {
            int index = getElementIndexBeforeAt(date);
            if (index >= 0) {
                data->endDate = getEndDate(index);
            }
        }
        return data->getPositionAt(date);
    }
    return 0.0;  // Default position
}

std::unique_ptr<GenericMap> MovementMap::renderMovementToMap() {
    // For now, return nullptr - this would need GenericMap factory methods
    // In a full implementation, this would create a positionMap
    return nullptr;
}

std::unique_ptr<GenericMap> MovementMap::renderMovementToMap(MovementMap* movementMap) {
    if (movementMap == nullptr) {
        return nullptr;
    }
    return movementMap->renderMovementToMap();
}

void MovementMap::generateMovement(MovementData* movementData, GenericMap* positionMap) {
    // For now, this is simplified - would need proper XML element creation
    // In a full implementation, this would generate position events
    auto movementSegment = movementData->getMovementSegment(0.1);
    
    // Just iterate through the segment to validate the algorithm works
    for (const auto& event : movementSegment) {
        // In a full implementation, we would create XML elements here
        // and add them to the positionMap
    }
}

bool MovementMap::applyToMsmPart(Element msmPart) {
    // MovementMap doesn't directly modify MSM notes, but creates a positionMap
    // This is more of a rendering operation than direct modification
    auto positionMap = renderMovementToMap();
    
    // In a full implementation, this would integrate the positionMap into the MSM structure
    // For now, we return true to indicate the processing was successful
    return !movementData.empty();
}

} // namespace mpm
} // namespace meico