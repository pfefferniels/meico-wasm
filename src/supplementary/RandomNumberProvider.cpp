#include "supplementary/RandomNumberProvider.h"
#include <cmath>
#include <algorithm>

namespace meico {
namespace supplementary {

const int RandomNumberProvider::DISTRIBUTION_UNIFORM;
const int RandomNumberProvider::DISTRIBUTION_GAUSSIAN;
const int RandomNumberProvider::DISTRIBUTION_TRIANGULAR;
const int RandomNumberProvider::DISTRIBUTION_CORRELATED_BROWNIANNOISE;
const int RandomNumberProvider::DISTRIBUTION_CORRELATED_COMPENSATING_TRIANGLE;
const int RandomNumberProvider::DISTRIBUTION_LIST;

RandomNumberProvider::RandomNumberProvider(int distributionType)
    : distributionType(distributionType), lowCut(0.0), highCut(0.0), standardDeviation(0.0),
      lowerLimit(0.0), upperLimit(0.0), maxStepWidth(0.0), mode(0.0), degreeOfCorrelation(0.0) {
    std::random_device rd;
    random.seed(rd());
}

std::unique_ptr<RandomNumberProvider> RandomNumberProvider::createRandomNumberProvider_uniformDistribution(double lowerLimit, double upperLimit) {
    auto rand = std::unique_ptr<RandomNumberProvider>(new RandomNumberProvider(DISTRIBUTION_UNIFORM));
    rand->lowerLimit = lowerLimit;
    rand->upperLimit = upperLimit;
    return rand;
}

std::unique_ptr<RandomNumberProvider> RandomNumberProvider::createRandomNumberProvider_gaussianDistribution(double standardDeviation, double lowerLimit, double upperLimit) {
    auto rand = std::unique_ptr<RandomNumberProvider>(new RandomNumberProvider(DISTRIBUTION_GAUSSIAN));
    rand->standardDeviation = standardDeviation;
    rand->lowerLimit = lowerLimit;
    rand->upperLimit = upperLimit;
    return rand;
}

std::unique_ptr<RandomNumberProvider> RandomNumberProvider::createRandomNumberProvider_triangularDistribution(double lowerLimit, double upperLimit, double mode, double lowCut, double highCut) {
    auto rand = std::unique_ptr<RandomNumberProvider>(new RandomNumberProvider(DISTRIBUTION_TRIANGULAR));
    rand->lowerLimit = lowerLimit;
    rand->upperLimit = upperLimit;
    rand->mode = mode;
    rand->lowCut = lowCut;
    rand->highCut = highCut;
    return rand;
}

std::unique_ptr<RandomNumberProvider> RandomNumberProvider::createRandomNumberProvider_brownianNoiseDistribution(double maxStepWidth, double lowerLimit, double upperLimit) {
    auto rand = std::unique_ptr<RandomNumberProvider>(new RandomNumberProvider(DISTRIBUTION_CORRELATED_BROWNIANNOISE));
    rand->maxStepWidth = maxStepWidth;
    rand->lowerLimit = lowerLimit;
    rand->upperLimit = upperLimit;

    // set the first value in the series, this is later required by method compensatingTriangleDistribution()
    std::uniform_real_distribution<> dis(0.0, 1.0);
    double scaleFactor = rand->upperLimit - rand->lowerLimit;
    double firstValue = (dis(rand->random) * scaleFactor) + rand->lowerLimit;
    rand->series.push_back(firstValue);

    return rand;
}

std::unique_ptr<RandomNumberProvider> RandomNumberProvider::createRandomNumberProvider_compensatingTriangleDistribution(double degreeOfCorrelation, double lowerLimit, double upperLimit, double lowCut, double highCut) {
    auto rand = std::unique_ptr<RandomNumberProvider>(new RandomNumberProvider(DISTRIBUTION_CORRELATED_COMPENSATING_TRIANGLE));
    rand->degreeOfCorrelation = degreeOfCorrelation;
    rand->lowerLimit = lowerLimit;
    rand->upperLimit = upperLimit;
    rand->lowCut = lowCut;
    rand->highCut = highCut;

    // set the first value in the series, this is later required by method compensatingTriangleDistribution()
    std::uniform_real_distribution<> dis(0.0, 1.0);
    double scaleFactor = rand->highCut - rand->lowCut;
    double firstValue = (dis(rand->random) * scaleFactor) + rand->lowCut;
    rand->series.push_back(firstValue);

    return rand;
}

std::unique_ptr<RandomNumberProvider> RandomNumberProvider::createRandomNumberProvider_distributionList(const std::vector<double>& list) {
    auto rand = std::unique_ptr<RandomNumberProvider>(new RandomNumberProvider(DISTRIBUTION_LIST));
    rand->series = list;
    return rand;
}

int RandomNumberProvider::getDistributionType() const {
    return this->distributionType;
}

void RandomNumberProvider::setSeed(long seed) {
    this->random.seed(seed);
    this->series.clear();
}

double RandomNumberProvider::getLowCut() const {
    return lowCut;
}

double RandomNumberProvider::getHighCut() const {
    return highCut;
}

double RandomNumberProvider::getStandardDeviation() const {
    return standardDeviation;
}

double RandomNumberProvider::getLowerLimit() const {
    return lowerLimit;
}

double RandomNumberProvider::getUpperLimit() const {
    return upperLimit;
}

double RandomNumberProvider::getValue(double index) {
    int intex = static_cast<int>(index);
    double rest = index - intex;
    
    // Ensure we have enough values in the series
    while (static_cast<int>(this->series.size()) <= intex) {
        switch (this->distributionType) {
            case DISTRIBUTION_UNIFORM:
                this->series.push_back(uniformDistribution());
                break;
            case DISTRIBUTION_GAUSSIAN:
                this->series.push_back(gaussianDistribution());
                break;
            case DISTRIBUTION_TRIANGULAR:
                this->series.push_back(triangularDistribution());
                break;
            case DISTRIBUTION_CORRELATED_BROWNIANNOISE:
                this->series.push_back(brownianNoiseDistribution(this->series.size()));
                break;
            case DISTRIBUTION_CORRELATED_COMPENSATING_TRIANGLE:
                this->series.push_back(compensatingTriangleDistribution(this->series.size()));
                break;
            case DISTRIBUTION_LIST:
                // For list distribution, we cycle through the existing values
                return distributionListValue(index);
        }
    }
    
    double a = this->series[intex];
    
    if (rest <= 0.0) // rest should never be < 0.0, but the check doesn't hurt
        return a;

    // if the index is between two integer indices
    // Ensure we have the next value too
    while (static_cast<int>(this->series.size()) <= intex + 1) {
        switch (this->distributionType) {
            case DISTRIBUTION_UNIFORM:
                this->series.push_back(uniformDistribution());
                break;
            case DISTRIBUTION_GAUSSIAN:
                this->series.push_back(gaussianDistribution());
                break;
            case DISTRIBUTION_TRIANGULAR:
                this->series.push_back(triangularDistribution());
                break;
            case DISTRIBUTION_CORRELATED_BROWNIANNOISE:
                this->series.push_back(brownianNoiseDistribution(this->series.size()));
                break;
            case DISTRIBUTION_CORRELATED_COMPENSATING_TRIANGLE:
                this->series.push_back(compensatingTriangleDistribution(this->series.size()));
                break;
            case DISTRIBUTION_LIST:
                return distributionListValue(index);
        }
    }
    
    double b = this->series[intex + 1]; // get the value of the next integer index
    return a + ((b - a) * rest); // interpolate linearly between the two values
}

void RandomNumberProvider::setInitialValue(double value) {
    this->series.clear();
    this->series.push_back(value);
}

double RandomNumberProvider::uniformDistribution() {
    std::uniform_real_distribution<> dis(0.0, 1.0);
    return (dis(random) * (upperLimit - lowerLimit)) + lowerLimit;
}

double RandomNumberProvider::gaussianDistribution() {
    std::normal_distribution<> dis(0.0, standardDeviation);
    double d;
    do {
        d = dis(random);
    } while (d < lowerLimit || d > upperLimit); // keep generating a new random number while the current value breaks the limits
    return d;
}

double RandomNumberProvider::triangularDistribution() {
    if (upperLimit == lowerLimit) // avoid division by 0.0
        return upperLimit; // the limits allow only one value anyway
    
    double scale = upperLimit - lowerLimit;
    double ca = mode - lowerLimit;
    double F = ca / scale;
    
    std::uniform_real_distribution<> dis(0.0, 1.0);
    double randVal = dis(random);
    
    double result;
    if (randVal < F)
        result = lowerLimit + std::sqrt(randVal * scale * ca);
    else
        result = upperLimit - std::sqrt((1 - randVal) * scale * (upperLimit - mode));
    
    // Clip the result
    if (result > highCut)
        result = highCut;
    if (result < lowCut)
        result = lowCut;
    
    return result;
}

double RandomNumberProvider::brownianNoiseDistribution(double index) {
    if (series.empty())
        return 0.0;
        
    double result;
    std::uniform_real_distribution<> dis(0.0, 1.0);
    
    do {
        result = series.back() + ((dis(random) - 0.5) * 2.0 * maxStepWidth); // compute uniformly distributed step
    } while (result < lowerLimit || result > upperLimit);
    
    return result;
}

double RandomNumberProvider::compensatingTriangleDistribution(double index) {
    if (series.empty())
        return 0.0;
        
    double prevRandomNum = series.back();
    double newLowerLimit = prevRandomNum - ((prevRandomNum - lowerLimit) / degreeOfCorrelation);
    double newUpperLimit = prevRandomNum + ((upperLimit - prevRandomNum) / degreeOfCorrelation);
    
    // Create a temporary triangular distribution
    double scale = newUpperLimit - newLowerLimit;
    double ca = prevRandomNum - newLowerLimit;
    double F = ca / scale;
    
    std::uniform_real_distribution<> dis(0.0, 1.0);
    double randVal = dis(random);
    
    double result;
    if (randVal < F)
        result = newLowerLimit + std::sqrt(randVal * scale * ca);
    else
        result = newUpperLimit - std::sqrt((1 - randVal) * scale * (newUpperLimit - prevRandomNum));
    
    // if 0.0 < degreeOfCorrelation < 1.0, the limits can be broken, here we have to clip the values
    if (result < lowerLimit)
        result = lowerLimit;
    if (result > upperLimit)
        result = upperLimit;
    
    // Clip the result
    if (result > highCut)
        result = highCut;
    if (result < lowCut)
        result = lowCut;
    
    return result;
}

double RandomNumberProvider::distributionListValue(double index) {
    if (series.empty())
        return 0.0;
    return series[static_cast<int>(index) % series.size()]; // read the list value and repeat the list if the index exceeds its length
}

} // namespace supplementary
} // namespace meico