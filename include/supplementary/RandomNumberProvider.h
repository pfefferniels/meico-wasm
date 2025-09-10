#pragma once

#include <vector>
#include <random>
#include <memory>

namespace meico {
namespace supplementary {

/**
 * This class provides random numbers based on the specified distribution.
 * @author Axel Berndt (Java), Copilot (C++ port)
 */
class RandomNumberProvider {
public:
    static const int DISTRIBUTION_UNIFORM = 0;
    static const int DISTRIBUTION_GAUSSIAN = 1;
    static const int DISTRIBUTION_TRIANGULAR = 2;
    static const int DISTRIBUTION_CORRELATED_BROWNIANNOISE = 3;
    static const int DISTRIBUTION_CORRELATED_COMPENSATING_TRIANGLE = 4;
    static const int DISTRIBUTION_LIST = 5;

private:
    std::mt19937 random;                          // the random number generator
    int distributionType;                         // indicates the distribution type which this random number provider uses to generate output
    std::vector<double> series;                   // this is filled with the generated random numbers and allows us to recall them, necessary for correlated random numbers

    double lowCut;
    double highCut;
    double standardDeviation;                     // this is used by Gaussian distribution

    double lowerLimit;
    double upperLimit;
    double maxStepWidth;                          // this is used by Brownian noise distribution
    double mode;                                  // this is used by triangular distribution
    double degreeOfCorrelation;                   // this is used by compensating triangle distribution

    /**
     * constructor
     * @param distributionType the distribution type
     */
    explicit RandomNumberProvider(int distributionType);

public:
    /**
     * a factory for a uniform distribution RandomNumberProvider
     * @param lowerLimit
     * @param upperLimit
     * @return
     */
    static std::unique_ptr<RandomNumberProvider> createRandomNumberProvider_uniformDistribution(double lowerLimit, double upperLimit);

    /**
     * a factory for a Gaussian distribution RandomNumberProvider
     * @param standardDeviation
     * @param lowerLimit
     * @param upperLimit
     * @return
     */
    static std::unique_ptr<RandomNumberProvider> createRandomNumberProvider_gaussianDistribution(double standardDeviation, double lowerLimit, double upperLimit);

    /**
     * a factory for a triangular distribution RandomNumberProvider
     * @param lowerLimit
     * @param upperLimit
     * @param mode
     * @param lowCut
     * @param highCut
     * @return
     */
    static std::unique_ptr<RandomNumberProvider> createRandomNumberProvider_triangularDistribution(double lowerLimit, double upperLimit, double mode, double lowCut, double highCut);

    /**
     * a factory for a Brownian noise distribution RandomNumberProvider
     * @param maxStepWidth
     * @param lowerLimit
     * @param upperLimit
     * @return
     */
    static std::unique_ptr<RandomNumberProvider> createRandomNumberProvider_brownianNoiseDistribution(double maxStepWidth, double lowerLimit, double upperLimit);

    /**
     * a factory for a compensating triangle distribution RandomNumberProvider
     * @param degreeOfCorrelation
     * @param lowerLimit
     * @param upperLimit
     * @param lowCut
     * @param highCut
     * @return
     */
    static std::unique_ptr<RandomNumberProvider> createRandomNumberProvider_compensatingTriangleDistribution(double degreeOfCorrelation, double lowerLimit, double upperLimit, double lowCut, double highCut);

    /**
     * a factory for a RandomNumberProvider with a predefined list of values
     * @param list
     * @return
     */
    static std::unique_ptr<RandomNumberProvider> createRandomNumberProvider_distributionList(const std::vector<double>& list);

    /**
     * query the distribution type with which this random number provider works
     * @return
     */
    int getDistributionType() const;

    /**
     * this can be used to set a specific seed, the series of random numbers so far will be rewritten
     * @param seed
     */
    void setSeed(long seed);

    /**
     * read the lowCut value
     * @return
     */
    double getLowCut() const;

    /**
     * read the highCut value
     * @return
     */
    double getHighCut() const;

    /**
     * read the standardDeviation value
     * @return
     */
    double getStandardDeviation() const;

    /**
     * read the lowerLimit value
     * @return
     */
    double getLowerLimit() const;

    /**
     * read the upperLimit value
     * @return
     */
    double getUpperLimit() const;

    /**
     * get a random value at the specified index
     * @param index
     * @return
     */
    double getValue(double index);

    /**
     * set the initial value for correlated distributions
     * @param value
     */
    void setInitialValue(double value);

private:
    /**
     * generate a uniform distribution value
     * @return
     */
    double uniformDistribution();

    /**
     * generate a Gaussian distribution value
     * @return
     */
    double gaussianDistribution();

    /**
     * generate a triangular distribution value
     * @return
     */
    double triangularDistribution();

    /**
     * generate a Brownian noise distribution value at the specified index
     * @param index
     * @return
     */
    double brownianNoiseDistribution(double index);

    /**
     * generate a compensating triangle distribution value at the specified index
     * @param index
     * @return
     */
    double compensatingTriangleDistribution(double index);

    /**
     * get a value from the distribution list
     * @param index
     * @return
     */
    double distributionListValue(double index);
};

} // namespace supplementary
} // namespace meico