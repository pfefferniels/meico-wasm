#pragma once

#include "mpm/elements/maps/GenericMap.h"
#include "mpm/elements/maps/data/DistributionData.h"
#include "supplementary/RandomNumberProvider.h"
#include "supplementary/KeyValue.h"
#include <memory>
#include <unordered_map>
#include <vector>

namespace meico {
namespace mpm {

/**
 * This class interfaces MPM's diverse imprecisionMaps
 * @author Axel Berndt (Java), Copilot (C++ port)
 */
class ImprecisionMap : public GenericMap {
private:
    static const int TIMING = 1;
    static const int DYNAMICS = 2;
    static const int TONEDURATION = 3;
    static const int TUNING = 4;

public:
    /**
     * ImprecisionMap factory
     * @param domain "timing", "dynamics", "toneduration", "tuning" or anything; even "" is allowed
     * @return
     */
    static std::unique_ptr<ImprecisionMap> createImprecisionMap(const std::string& domain);

    /**
     * ImprecisionMap factory
     * @param xml
     * @return
     */
    static std::unique_ptr<ImprecisionMap> createImprecisionMap(const Element& xml);

    /**
     * set the imprecision domain
     * @param domain "timing", "dynamics", "toneduration", "tuning" or anything; even "" and null are allowed
     */
    void setDomain(const std::string& domain);

    /**
     * get the domain of this imprecisionMap
     * @return
     */
    std::string getDomain() const;

    /**
     * for a tuning imprecision map, specify the unit
     * @param unit "cents", "Hertz", "Hz"
     */
    void setDetuneUnit(const std::string& unit);

    /**
     * get the unit of the tuning imprecision map
     * @return the unit or an empty string
     */
    std::string getDetuneUnit() const;

    /**
     * add a distribution.uniform element to the map
     * @param date
     * @param lowerLimit
     * @param upperLimit
     * @return the index at which it has been inserted
     */
    int addDistributionUniform(double date, double lowerLimit, double upperLimit);

    /**
     * add a distribution.uniform element to the map
     * @param date
     * @param lowerLimit
     * @param upperLimit
     * @param seed
     * @return the index at which it has been inserted
     */
    int addDistributionUniform(double date, double lowerLimit, double upperLimit, long seed);

    /**
     * add a distribution.gaussian element to the map,
     * this is a gaussian distribution (expectation value for the offset is 0.0; asynchrony should be used to shift it elsewhere)
     * @param date
     * @param standardDeviation
     * @param lowerLimit
     * @param upperLimit
     * @return the index at which it has been inserted
     */
    int addDistributionGaussian(double date, double standardDeviation, double lowerLimit, double upperLimit);

    /**
     * add a distribution.gaussian element to the map,
     * this is a gaussian distribution (expectation value for the offset is 0.0; asynchrony should be used to shift it elsewhere)
     * @param date
     * @param standardDeviation
     * @param lowerLimit
     * @param upperLimit
     * @param seed
     * @return the index at which it has been inserted
     */
    int addDistributionGaussian(double date, double standardDeviation, double lowerLimit, double upperLimit, long seed);

    /**
     * add a distribution.triangular element to the map,
     * this specifies a triangular distribution with lower and upper limit, the peak (i.e., highest probability) at the mode value
     * @param date
     * @param lowerLimit
     * @param upperLimit
     * @param mode the peak of the triangle (i.e., highest probability)
     * @param lowerClip lower clip border
     * @param upperClip upper clip border
     * @return the index at which it has been inserted
     */
    int addDistributionTriangular(double date, double lowerLimit, double upperLimit, double mode, double lowerClip, double upperClip);

    /**
     * add a distribution.triangular element to the map,
     * this specifies a triangular distribution with lower and upper limit, the peak (i.e., highest probability) at the mode value
     * @param date
     * @param lowerLimit
     * @param upperLimit
     * @param mode the peak of the triangle (i.e., highest probability)
     * @param lowerClip lower clip border
     * @param upperClip upper clip border
     * @param seed
     * @return the index at which it has been inserted
     */
    int addDistributionTriangular(double date, double lowerLimit, double upperLimit, double mode, double lowerClip, double upperClip, long seed);

    /**
     * add a distribution.correlated.brownianNoise element to the map,
     * this represents Brownian noise, the performance renderer generates this by a random walk algorithm
     * @param date
     * @param maxStepWidth the maximum step width in the random walk algorithm
     * @param lowerLimit
     * @param upperLimit
     * @param millisecondsTimingBasis the timing basis (time steps) of the distribution, changing this value will transpose the distribution
     * @return the index at which it has been inserted
     */
    int addDistributionBrownianNoise(double date, double maxStepWidth, double lowerLimit, double upperLimit, double millisecondsTimingBasis);

    /**
     * add a distribution.correlated.brownianNoise element to the map,
     * this represents Brownian noise, the performance renderer generates this by a random walk algorithm
     * @param date
     * @param maxStepWidth the maximum step width in the random walk algorithm
     * @param lowerLimit
     * @param upperLimit
     * @param millisecondsTimingBasis the timing basis (time steps) of the distribution, changing this value will transpose the distribution
     * @param seed
     * @return the index at which it has been inserted
     */
    int addDistributionBrownianNoise(double date, double maxStepWidth, double lowerLimit, double upperLimit, double millisecondsTimingBasis, long seed);

    /**
     * add a distribution.correlated.compensatingTriangle element to the map,
     * in this variant of the triangular distribution the mode (triangle peak) is wandering with the random values,
     * this method is an alternative to brownianNoise and is experimental
     * @param date
     * @param degreeOfCorrelation Must be >= 0.0. To avoid outliers (beyond the lower and upper limit) this value should be >= 1.0. 1.0 keeps the triangle's left and right edge at the lower and upper limit. The greater this value, the narrower is the triangle while wandering around between the limits.
     * @param lowerLimit
     * @param upperLimit
     * @param lowerClip lower clip border
     * @param upperClip upper clip border
     * @param millisecondsTimingBasis the timing basis (time steps) of the distribution, changing this value will transpose the distribution
     * @return the index at which it has been inserted
     */
    int addDistributionCompensatingTriangle(double date, double degreeOfCorrelation, double lowerLimit, double upperLimit, double lowerClip, double upperClip, double millisecondsTimingBasis);

    /**
     * add a distribution.correlated.compensatingTriangle element to the map,
     * in this variant of the triangular distribution the mode (triangle peak) is wandering with the random values,
     * this method is an alternative to brownianNoise and is experimental
     * @param date
     * @param degreeOfCorrelation Must be >= 0.0. To avoid outliers (beyond the lower and upper limit) this value should be >= 1.0. 1.0 keeps the triangle's left and right edge at the lower and upper limit. The greater this value, the narrower is the triangle while wandering around between the limits.
     * @param lowerLimit
     * @param upperLimit
     * @param lowerClip lower clip border
     * @param upperClip upper clip border
     * @param millisecondsTimingBasis the timing basis (time steps) of the distribution, changing this value will transpose the distribution
     * @param seed
     * @return the index at which it has been inserted
     */
    int addDistributionCompensatingTriangle(double date, double degreeOfCorrelation, double lowerLimit, double upperLimit, double lowerClip, double upperClip, double millisecondsTimingBasis, long seed);

    /**
     * add a distribution.list element to the map, it should already contain all its measurement children,
     * this is a table of values of deviations from 0.0 (perfect value),
     * the values may have been obtained by measurements of human musicians' performances
     * @param date
     * @param list
     * @param millisecondsTimingBasis the timing basis (time steps) of the distribution, changing this value will transpose the distribution
     * @return the index at which it has been inserted
     */
    int addDistributionList(double date, const Element& list, double millisecondsTimingBasis);

    /**
     * add a distribution element to the imprecisionMap
     * @param data xml data will be ignored (use addElement() instead to add an xml representation)
     * @return
     */
    int addDistribution(const DistributionData& data);

    /**
     * collect all distribution data of the index-specified map element
     * @param index
     * @return
     */
    std::unique_ptr<DistributionData> getDistributionDataOf(int index) const;

    /**
     * On the basis of the specified imprecisionMap, apply the corresponding transformations to all elements of the specified map.
     * For correlated distributions, this method includes a handover between subsequent imprecision elements, i.e. the final value of the previous becomes the first of the next.
     * @param map
     * @param shakePolyphonicPart If this map/MSM score is polyphonic all voices would perform the exact same imprecision. By setting this flag true, this is shaken up a little bit.
     */
    void renderImprecisionToMap(GenericMap& map, bool shakePolyphonicPart);

    /**
     * on the basis of the specified imprecisionMap, apply the corresponding transformations to all elements of the specified map
     * @param map
     * @param imprecisionMap
     * @param shakePolyphonicPart If this map/MSM score is polyphonic all voices would perform the exact same imprecision. By setting this flag true, this is shaken up a little bit.
     */
    static void renderImprecisionToMap(GenericMap& map, ImprecisionMap* imprecisionMap, bool shakePolyphonicPart);

    /**
     * Apply this imprecision map to modify notes in an MSM part
     * @param msmPart the MSM part element to modify
     * @return true if any modifications were made
     */
    bool applyToMsmPart(Element msmPart) override;

protected:
    /**
     * constructor, generates an empty imprecisionMap,
     * The application should specify the domain before it can be used!
     * @param domain "timing", "dynamics", "toneduration", "tuning" or anything; even is allowed
     */
    explicit ImprecisionMap(const std::string& domain);

    /**
     * constructor, generates an instance from xml code
     * @param xml
     */
    explicit ImprecisionMap(const Element& xml);

    /**
     * set the data of this object, this parses the xml element and generates the according data structure
     * @param xml
     */
    void parseData(const Element& xml) override;

private:
    /**
     * collect all data that is needed to compute the distribution/offset at the specified date
     * @param date
     * @return
     */
    std::unique_ptr<DistributionData> getDistributionDataAt(double date) const;

    /**
     * a helper method for adding offsets to a hashmap of offsets
     * @param offsetsMap
     * @param millisecondsDate
     * @param offsetAndAttribute
     */
    static void addToOffsetsMap(std::unordered_map<double, std::vector<supplementary::KeyValue<double, Element*>>>& offsetsMap, 
                               double millisecondsDate, 
                               const supplementary::KeyValue<double, Element*>& offsetAndAttribute);

    /**
     * a helper method to get the handover value
     * @param randomPrev the random number provider that hands over its last value to the next
     * @param ddPrev the distribution element from which the handover should be done
     * @param ddNext the distribution element to which the handover should be done
     * @return
     */
    static double getHandoverValue(supplementary::RandomNumberProvider* randomPrev, 
                                  const DistributionData* ddPrev, 
                                  const DistributionData* ddNext);

    /**
     * The first value in the random number series of a correlated distribution should be initialized at the last value of the preceding distribution element.
     * If that value is null (because there is no preceding distribution or ...) the first value is set to a random value within a restricted range, i.e. half of the lower and upper limit, as we do not want the imprecision to start with extreme values.
     * This method will cause the RandomNumberProvider to create a totally new series of random numbers; hence, use it only at the beginning before you start working with the values!
     * @param value the last value of the preceding distribution element, or null
     * @param random the RandomNumberProvider to be initialized with the specified value
     */
    static void doHandover(double value, supplementary::RandomNumberProvider* random);

    /**
     * This seeks elements in the specified offsets hashmap with the same milliseconds.date(.end) and shakes their imprecision offsets.
     * Only one randomly chosen element for each date keeps its original offset.
     * @param offsets
     */
    static void shakeOffsets(std::unordered_map<double, std::vector<supplementary::KeyValue<double, Element*>>>& offsets);

    /**
     * When shaking timing offsets we have to take care of collisions,
     * i.e., noteOn and noteOff events with the same pitch and at the same milliseconds date should not be shifted apart.
     * Hence, the timing shaking is a bit extended compared to the usual shakeOffsets() method.
     * @param offsets
     */
    static void shakeTimingOffsets(std::unordered_map<double, std::vector<supplementary::KeyValue<double, Element*>>>& offsets);

    /**
     * A helper method for the shaking mechanisms in methods shakeOffsets() and shakeTimingOffsets().
     * The input offset is reduced by a random amount via triangular distribution. But we keep the direction of the offset.
     * Furthermore, the maximum amount of reduction is limited to half of the offset.
     * So the parameters of the triangular distribution are: (limits are offset and offset/2, mode = offset).
     * @param offset
     * @return
     */
    static double shake(double offset);

    /**
     * While method renderImprecisionToMap() computes the imprecision offsets, this method adds the values to the corresponding attributes.
     * This method is called at the end of renderImprecisionToMap().
     * @param offsets
     */
    static void addOffsetsToAttributes(std::unordered_map<double, std::vector<supplementary::KeyValue<double, Element*>>>& offsets, int domain);
};

} // namespace mpm
} // namespace meico