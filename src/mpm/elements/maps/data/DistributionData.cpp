#include "mpm/elements/maps/data/DistributionData.h"
#include "xml/Helper.h"
#include <algorithm>
#include <limits>

namespace meico {
namespace mpm {

const std::string DistributionData::UNIFORM = "distribution.uniform";
const std::string DistributionData::GAUSSIAN = "distribution.gaussian";
const std::string DistributionData::TRIANGULAR = "distribution.triangular";
const std::string DistributionData::BROWNIAN = "distribution.correlated.brownianNoise";
const std::string DistributionData::COMPENSATING_TRIANGLE = "distribution.correlated.compensatingTriangle";
const std::string DistributionData::LIST = "distribution.list";

DistributionData::DistributionData() : xml(xml::Helper::createElement("temp")) {}

DistributionData::DistributionData(const Element& xml) : xml(xml) {
    this->type = xml.name();

    std::string dateStr = xml::Helper::getAttributeValue(xml, "date");
    if (!dateStr.empty())
        this->startDate = std::stod(dateStr);

    std::string xmlIdValue = xml::Helper::getAttributeValue(xml, "xml:id");
    if (!xmlIdValue.empty())
        this->xmlId = xmlIdValue;

    std::string seedStr = xml::Helper::getAttributeValue(xml, "seed");
    if (!seedStr.empty()) {
        this->seed = std::stol(seedStr);
        this->hasSeed = true;
    }

    std::string lowerLimitStr = xml::Helper::getAttributeValue(xml, "limit.lower");
    if (!lowerLimitStr.empty()) {
        this->lowerLimit = std::stod(lowerLimitStr);
        this->hasLowerLimit = true;
    }

    std::string upperLimitStr = xml::Helper::getAttributeValue(xml, "limit.upper");
    if (!upperLimitStr.empty()) {
        this->upperLimit = std::stod(upperLimitStr);
        this->hasUpperLimit = true;
    }

    std::string lowerClipStr = xml::Helper::getAttributeValue(xml, "clip.lower");
    if (!lowerClipStr.empty()) {
        this->lowerClip = std::stod(lowerClipStr);
        this->hasLowerClip = true;
    }

    std::string upperClipStr = xml::Helper::getAttributeValue(xml, "clip.upper");
    if (!upperClipStr.empty()) {
        this->upperClip = std::stod(upperClipStr);
        this->hasUpperClip = true;
    }

    std::string modeStr = xml::Helper::getAttributeValue(xml, "mode");
    if (!modeStr.empty()) {
        this->mode = std::stod(modeStr);
        this->hasMode = true;
    }

    std::string standardDeviationStr = xml::Helper::getAttributeValue(xml, "deviation.standard");
    if (!standardDeviationStr.empty()) {
        this->standardDeviation = std::stod(standardDeviationStr);
        this->hasStandardDeviation = true;
    }

    std::string millisecondsTimingBasisStr = xml::Helper::getAttributeValue(xml, "milliseconds.timingBasis");
    if (!millisecondsTimingBasisStr.empty()) {
        this->millisecondsTimingBasis = std::stod(millisecondsTimingBasisStr);
        this->hasMillisecondsTimingBasis = true;
    }

    std::string degreeOfCorrelationStr = xml::Helper::getAttributeValue(xml, "degreeOfCorrelation");
    if (!degreeOfCorrelationStr.empty()) {
        this->degreeOfCorrelation = std::stod(degreeOfCorrelationStr);
        this->hasDegreeOfCorrelation = true;
    }

    std::string maxStepWidthStr = xml::Helper::getAttributeValue(xml, "stepWidth.max");
    if (!maxStepWidthStr.empty()) {
        this->maxStepWidth = std::stod(maxStepWidthStr);
        this->hasMaxStepWidth = true;
    }

    // Parse measurement children
    for (auto child = xml.first_child(); child; child = child.next_sibling()) {
        if (std::string(child.name()) == "measurement") {
            std::string valueStr = xml::Helper::getAttributeValue(child, "value");
            if (!valueStr.empty()) {
                this->distributionList.push_back(std::stod(valueStr));
            }
        }
    }
}

std::unique_ptr<DistributionData> DistributionData::clone() const {
    auto clone = std::make_unique<DistributionData>();
    clone->xml = this->xml; // shallow copy of XML
    clone->xmlId = this->xmlId;
    clone->startDate = this->startDate;
    clone->endDate = this->endDate;

    clone->type = this->type;
    clone->standardDeviation = this->standardDeviation;
    clone->maxStepWidth = this->maxStepWidth;
    clone->degreeOfCorrelation = this->degreeOfCorrelation;
    clone->mode = this->mode;
    clone->lowerLimit = this->lowerLimit;
    clone->upperLimit = this->upperLimit;
    clone->lowerClip = this->lowerClip;
    clone->upperClip = this->upperClip;
    clone->seed = this->seed;
    clone->millisecondsTimingBasis = this->millisecondsTimingBasis;
    clone->distributionList = this->distributionList;

    // Copy flags
    clone->hasStandardDeviation = this->hasStandardDeviation;
    clone->hasMaxStepWidth = this->hasMaxStepWidth;
    clone->hasDegreeOfCorrelation = this->hasDegreeOfCorrelation;
    clone->hasMode = this->hasMode;
    clone->hasLowerLimit = this->hasLowerLimit;
    clone->hasUpperLimit = this->hasUpperLimit;
    clone->hasLowerClip = this->hasLowerClip;
    clone->hasUpperClip = this->hasUpperClip;
    clone->hasSeed = this->hasSeed;
    clone->hasMillisecondsTimingBasis = this->hasMillisecondsTimingBasis;

    return clone;
}

supplementary::KeyValue<double, double> DistributionData::getMinAndMaxValueInDistributionList() const {
    if (this->distributionList.empty())
        return supplementary::KeyValue<double, double>(0.0, 0.0);

    double min = this->distributionList[0];
    double max = min;

    for (double d : this->distributionList) {
        if (d < min)
            min = d;
        else if (d > max)
            max = d;
    }

    return supplementary::KeyValue<double, double>(min, max);
}

} // namespace mpm
} // namespace meico