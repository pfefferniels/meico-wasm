#include "mpm/elements/maps/data/MovementData.h"
#include "xml/Helper.h"
#include <cmath>
#include <iostream>
#include <algorithm>

namespace meico {
namespace mpm {

MovementData::MovementData() {
    // Initialize default values - all set in header
}

MovementData::MovementData(const Element& xml) 
    : xml(xml), controlPointsComputed(false) {
    
    // Parse date attribute
    auto dateAttr = xml.attribute("date");
    if (dateAttr) {
        startDate = xml::Helper::parseDouble(dateAttr.value());
    }

    // Parse position attribute
    auto positionAttr = xml.attribute("position");
    if (positionAttr) {
        position = xml::Helper::parseDouble(positionAttr.value());
    }

    // Parse transition.to attribute
    auto transitionToAttr = xml.attribute("transition.to");
    if (transitionToAttr) {
        transitionTo = xml::Helper::parseDouble(transitionToAttr.value());
    }

    // Parse curvature attribute
    auto curvatureAttr = xml.attribute("curvature");
    if (curvatureAttr) {
        curvature = xml::Helper::parseDouble(curvatureAttr.value());
    }

    // Parse protraction attribute
    auto protractionAttr = xml.attribute("protraction");
    if (protractionAttr) {
        protraction = xml::Helper::parseDouble(protractionAttr.value());
    }

    // Parse controller attribute
    auto controllerAttr = xml.attribute("controller");
    if (controllerAttr) {
        controller = controllerAttr.value();
    }

    // Parse XML ID
    auto idAttr = xml.attribute("xml:id");
    if (idAttr) {
        xmlId = idAttr.value();
    }
}

std::unique_ptr<MovementData> MovementData::clone() const {
    auto cloned = std::make_unique<MovementData>();
    cloned->xml = xml;
    cloned->xmlId = xmlId;
    cloned->startDate = startDate;
    cloned->endDate = endDate;
    cloned->position = position;
    cloned->transitionTo = transitionTo;
    cloned->controller = controller;
    cloned->curvature = curvature;
    cloned->protraction = protraction;
    cloned->x1 = x1;
    cloned->x2 = x2;
    cloned->controlPointsComputed = controlPointsComputed;
    return cloned;
}

void MovementData::computeInnerControlPointsXPositions() {
    if (protraction == 0.0) {
        x1 = curvature;
        x2 = 1.0 - curvature;
        controlPointsComputed = true;
        return;
    }

    x1 = curvature + ((std::abs(protraction) + protraction) / (2.0 * protraction) - (std::abs(protraction) / protraction) * curvature) * protraction;
    x2 = 1.0 - curvature + ((protraction - std::abs(protraction)) / (2.0 * protraction) + (std::abs(protraction) / protraction) * curvature) * protraction;
    controlPointsComputed = true;
}

double MovementData::getTForDate(double date) {
    if (date == startDate) {
        return 0.0;
    }

    if (date == endDate) {
        return 1.0;
    }

    if (!controlPointsComputed) {
        computeInnerControlPointsXPositions();
    }

    // Values that are often required
    double s = endDate - startDate;
    date = date - startDate;
    double u = (3.0 * x1) - (3.0 * x2) + 1.0;
    double v = (-6.0 * x1) + (3.0 * x2);
    double w = 3.0 * x1;

    // Binary search for the t that is integer precise on the x-axis/time domain
    double t = 0.5;
    double diffX = ((((u * t) + v) * t + w) * t * s) - date;
    for (double tt = 0.25; std::abs(diffX) >= 1.0; tt *= 0.5) { // while the difference in the x-domain is >= 1.0
        if (diffX > 0.0)                                        // if t is too small
            t -= tt;
        else                                                    // if t is too big
            t += tt;
        diffX = ((((u * t) + v) * t + w) * t * s) - date;       // compute difference
    }
    return t;
}

double MovementData::getPositionAt(double date) {
    if (date <= startDate) {
        return position;
    }
    
    if (date >= endDate) {
        return transitionTo;
    }

    double t = getTForDate(date);
    return ((((3.0 - (2.0 * t)) * t * t) * (transitionTo - position)) + position);
}

std::pair<double, double> MovementData::getDatePosition(double t) {
    double x1_3 = 3.0 * x1;
    double x2_3 = 3.0 * x2;
    double u = x1_3 - x2_3 + 1.0;
    double v = (-6.0 * x1) + x2_3;
    
    double frameStart = startDate;
    double frameLength = endDate - startDate;

    double resultDate = ((((u * t) + v) * t + x1_3) * t * frameLength) + frameStart;
    double resultPosition = ((((3.0 - (2.0 * t)) * t * t) * (transitionTo - position)) + position);

    return std::make_pair(resultDate, resultPosition);
}

std::vector<std::pair<double, double>> MovementData::getMovementSegment(double maxStepSize) {
    if (!controlPointsComputed) {
        computeInnerControlPointsXPositions();
    }

    std::vector<double> ts;
    ts.push_back(0.0);
    ts.push_back(1.0);

    std::vector<std::pair<double, double>> series;
    series.push_back(getDatePosition(0.0));  // we start with the first value
    series.push_back(getDatePosition(1.0));  // and end up with the last value

    // Generate further pairs in-between each two adjacent pairs as long as their value difference is greater than maxStepSize
    for (size_t i = 0; i < ts.size() - 1; ++i) {
        while (std::abs(series[i+1].second - series[i].second) > maxStepSize) {
            double t = (ts[i] + ts[i+1]) * 0.5;
            ts.insert(ts.begin() + i + 1, t);
            series.insert(series.begin() + i + 1, getDatePosition(t));
        }
    }

    std::pair<double, double> beginning = std::make_pair(startDate, position);
    series.insert(series.begin(), beginning);

    std::pair<double, double> end = std::make_pair(endDate, transitionTo);
    series.push_back(end);

    // Apply scaling factor for MIDI output (0-127)
    for (auto& tuple : series) {
        tuple.second *= 127.0;
    }

    return series;
}

} // namespace mpm
} // namespace meico