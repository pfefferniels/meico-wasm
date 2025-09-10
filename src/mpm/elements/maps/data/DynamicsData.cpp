#include "mpm/elements/maps/data/DynamicsData.h"
#include "xml/Helper.h"
#include <algorithm>
#include <cmath>
#include <limits>

namespace meico {
namespace mpm {

DynamicsData::DynamicsData() 
    : xml(nullptr), startDate(0.0), endDate(std::numeric_limits<double>::max()),
      volume(0.0), transitionTo(0.0), curvature(0.0), protraction(0.0),
      subNoteDynamics(false), x1(0.0), x2(0.0), controlPointsComputed(false) {
}

DynamicsData::DynamicsData(const Element& xmlElement) 
    : xml(xmlElement), startDate(0.0), endDate(std::numeric_limits<double>::max()),
      volume(0.0), transitionTo(0.0), curvature(0.0), protraction(0.0),
      subNoteDynamics(false), x1(0.0), x2(0.0), controlPointsComputed(false) {
    
    // Parse date attribute
    auto dateAttr = xml.attribute("date");
    if (dateAttr) {
        startDate = xml::Helper::parseDouble(dateAttr.value());
    }

    // Parse volume attribute
    auto volumeAttr = xml.attribute("volume");
    if (volumeAttr) {
        volumeString = volumeAttr.value();
        try {
            volume = xml::Helper::parseDouble(volumeString);
        } catch (...) {
            // Keep as string if not numeric - will need style resolution
        }
    }

    // Parse transition.to attribute
    auto transitionToAttr = xml.attribute("transition.to");
    if (transitionToAttr) {
        transitionToString = transitionToAttr.value();
        try {
            transitionTo = xml::Helper::parseDouble(transitionToString);
        } catch (...) {
            // Keep as string if not numeric - will need style resolution
        }
    }

    // Parse curvature attribute
    auto curvatureAttr = xml.attribute("curvature");
    if (curvatureAttr) {
        curvature = xml::Helper::parseDouble(curvatureAttr.value());
        // Ensure boundaries as in Java
        curvature = std::max(0.0, std::min(1.0, curvature));
    }

    // Parse protraction attribute
    auto protractionAttr = xml.attribute("protraction");
    if (protractionAttr) {
        protraction = xml::Helper::parseDouble(protractionAttr.value());
        // Ensure boundaries as in Java
        protraction = std::max(-1.0, std::min(1.0, protraction));
    }

    // Parse subNoteDynamics attribute
    auto subNoteDynamicsAttr = xml.attribute("subNoteDynamics");
    if (subNoteDynamicsAttr) {
        subNoteDynamics = xml::Helper::parseBoolean(subNoteDynamicsAttr.value());
    }

    // Parse xml:id attribute
    auto idAttr = xml.attribute("xml:id");
    if (idAttr) {
        xmlId = idAttr.value();
    }
}

std::unique_ptr<DynamicsData> DynamicsData::clone() const {
    auto cloned = std::make_unique<DynamicsData>();
    cloned->xml = xml; // Copy XML element
    cloned->xmlId = xmlId;
    cloned->styleName = styleName;
    cloned->style = style;
    cloned->dynamicsDefString = dynamicsDefString;
    cloned->dynamicsDef = dynamicsDef;
    cloned->startDate = startDate;
    cloned->endDate = endDate;
    cloned->volumeString = volumeString;
    cloned->volume = volume;
    cloned->transitionToString = transitionToString;
    cloned->transitionTo = transitionTo;
    cloned->curvature = curvature;
    cloned->protraction = protraction;
    cloned->subNoteDynamics = subNoteDynamics;
    cloned->x1 = x1;
    cloned->x2 = x2;
    cloned->controlPointsComputed = controlPointsComputed;
    return cloned;
}

bool DynamicsData::isConstantDynamics() const {
    return (transitionToString.empty() && transitionTo == 0.0) || 
           (volume == transitionTo);
}

void DynamicsData::computeInnerControlPointsXPositions() {
    if (controlPointsComputed) {
        return;
    }

    // Default values as in Java
    double curvatureVal = curvature;
    double protractionVal = protraction;

    if (protractionVal == 0.0) {
        x1 = curvatureVal;
        x2 = 1.0 - curvatureVal;
    } else {
        x1 = curvatureVal + ((std::abs(protractionVal) + protractionVal) / (2.0 * protractionVal) - 
             (std::abs(protractionVal) / protractionVal) * curvatureVal) * protractionVal;
        x2 = 1.0 - curvatureVal + ((protractionVal - std::abs(protractionVal)) / (2.0 * protractionVal) + 
             (std::abs(protractionVal) / protractionVal) * curvatureVal) * protractionVal;
    }

    controlPointsComputed = true;
}

double DynamicsData::getTForDate(double date) {
    // Numerical solution (not exact, however integer-precise and more efficient)
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
    double adjustedDate = date - startDate;
    double u = (3.0 * x1) - (3.0 * x2) + 1.0;
    double v = (-6.0 * x1) + (3.0 * x2);
    double w = 3.0 * x1;

    // Binary search for the t that is integer precise on the x-axis/time domain
    double t = 0.5;
    double diffX = ((((u * t) + v) * t + w) * t * s) - adjustedDate;
    
    for (double tt = 0.25; std::abs(diffX) >= 1.0; tt *= 0.5) { // while the difference in the x-domain is >= 1.0
        if (diffX > 0.0) {        // if t is too small
            t -= tt;
        } else {                  // if t is too big
            t += tt;
        }
        diffX = ((((u * t) + v) * t + w) * t * s) - adjustedDate; // compute difference
    }
    
    return t;
}

double DynamicsData::getDynamicsAt(double date) {
    if ((date < startDate) || isConstantDynamics()) {
        return volume;
    }

    if (date >= endDate) {
        return transitionTo;
    }

    double t = getTForDate(date);
    return ((((3.0 - (2.0 * t)) * t * t) * (transitionTo - volume)) + volume);
}

std::pair<double, double> DynamicsData::getDateDynamics(double t) {
    std::pair<double, double> result;

    double x1_3 = 3.0 * x1;
    double x2_3 = 3.0 * x2;
    double u = x1_3 - x2_3 + 1.0;
    double v = (-6.0 * x1) + x2_3;
    result.first = ((((u * t) + v) * t + x1_3) * t * (endDate - startDate)) + startDate;

    result.second = ((((3.0 - (2.0 * t)) * t * t) * (transitionTo - volume)) + volume);

    return result;
}

std::vector<std::pair<double, double>> DynamicsData::getSubNoteDynamicsSegment(double maxStepSize) {
    if (!controlPointsComputed) {
        computeInnerControlPointsXPositions();
    }

    std::vector<double> ts;
    ts.push_back(0.0);
    ts.push_back(1.0);
    
    std::vector<std::pair<double, double>> series;
    series.push_back(getDateDynamics(0.0));  // we start with the first value
    series.push_back(getDateDynamics(1.0));  // and end up with the last value

    // Generate further pairs in-between each two adjacent pairs as long as their value difference 
    // is greater than maxStepSize; this here is basically a depth-first algorithm
    for (size_t i = 0; i < ts.size() - 1; ++i) {
        while (std::abs(series[i+1].second - series[i].second) > maxStepSize) {
            double t = (ts[i] + ts[i+1]) * 0.5;
            ts.insert(ts.begin() + i + 1, t);
            series.insert(series.begin() + i + 1, getDateDynamics(t));
        }
    }

    return series;
}

} // namespace mpm
} // namespace meico