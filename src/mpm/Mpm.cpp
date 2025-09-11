#include "mpm/Mpm.h"
#include "mpm/elements/Performance.h"
#include "mpm/elements/Global.h"
#include "mpm/elements/Part.h"
#include "mpm/elements/Dated.h"
#include "mpm/elements/maps/GenericMap.h"
#include "mpm/elements/metadata/Metadata.h"
#include "xml/Helper.h"

namespace meico {
namespace mpm {

// Static constants
const std::string Mpm::MPM_NAMESPACE = "http://www.cemfi.de/mpm/ns/1.0";

const std::string Mpm::ARTICULATION_STYLE = "articulationStyles";
const std::string Mpm::ORNAMENTATION_STYLE = "ornamentationStyles";
const std::string Mpm::DYNAMICS_STYLE = "dynamicsStyles";
const std::string Mpm::METRICAL_ACCENTUATION_STYLE = "metricalAccentuationStyles";
const std::string Mpm::TEMPO_STYLE = "tempoStyles";
const std::string Mpm::RUBATO_STYLE = "rubatoStyles";

const std::string Mpm::ARTICULATION_MAP = "articulationMap";
const std::string Mpm::ORNAMENTATION_MAP = "ornamentationMap";
const std::string Mpm::DYNAMICS_MAP = "dynamicsMap";
const std::string Mpm::MOVEMENT_MAP = "movementMap";
const std::string Mpm::METRICAL_ACCENTUATION_MAP = "metricalAccentuationMap";
const std::string Mpm::TEMPO_MAP = "tempoMap";
const std::string Mpm::RUBATO_MAP = "rubatoMap";
const std::string Mpm::ASYNCHRONY_MAP = "asynchronyMap";
const std::string Mpm::IMPRECISION_MAP = "imprecisionMap";
const std::string Mpm::IMPRECISION_MAP_TIMING = "imprecisionMap.timing";
const std::string Mpm::IMPRECISION_MAP_DYNAMICS = "imprecisionMap.dynamics";
const std::string Mpm::IMPRECISION_MAP_TONEDURATION = "imprecisionMap.toneduration";
const std::string Mpm::IMPRECISION_MAP_TUNING = "imprecisionMap.tuning";

Mpm::Mpm() : msm::AbstractMsm() {
    init();
}

Mpm::Mpm(const Document& mpmDocument) : msm::AbstractMsm(mpmDocument) {
    parseData();
}

Mpm::Mpm(const std::string& filePath) : msm::AbstractMsm(filePath) {
    parseData();
}

Mpm::Mpm(const std::string& filePath, bool validate, const std::string& schema) 
    : msm::AbstractMsm(filePath, validate, schema) {
    parseData();
}

Mpm::Mpm(const std::string& xmlString, bool fromString) 
    : msm::AbstractMsm(xmlString, fromString) {
    parseData();
}

Mpm::Mpm(std::istream& inputStream) : msm::AbstractMsm(inputStream) {
    parseData();
}

std::unique_ptr<Mpm> Mpm::createMpm() {
    return std::make_unique<Mpm>();
}

size_t Mpm::size() const {
    return performances.size();
}

Performance* Mpm::getPerformance(size_t index) {
    if (index < performances.size()) {
        return performances[index].get();
    }
    return nullptr;
}

const Performance* Mpm::getPerformance(size_t index) const {
    if (index < performances.size()) {
        return performances[index].get();
    }
    return nullptr;
}

void Mpm::addPerformance(std::unique_ptr<Performance> performance) {
    performances.push_back(std::move(performance));
}

Metadata* Mpm::getMetadata() {
    return metadata.get();
}

const Metadata* Mpm::getMetadata() const {
    return metadata.get();
}

void Mpm::setMetadata(std::unique_ptr<Metadata> newMetadata) {
    metadata = std::move(newMetadata);
}

void Mpm::init() {
    Document doc;
    Element root = doc.append_child("mpm");
    
    // Set namespace (simplified - pugixml doesn't have full namespace support)
    // In a full implementation, we'd need proper namespace handling
    
    setDocument(doc);
}

void Mpm::parseData() {
    performances.clear();
    metadata.reset();
    
    Element root = getRootElement();
    if (!root) {
        return;
    }
    
    // Parse metadata
    Element metadataElement = xml::Helper::getFirstChildElement(root, "metadata");
    if (metadataElement) {
        // For now, just create an empty metadata object
        // In a full implementation, this would parse the metadata content
        metadata = std::make_unique<Metadata>();
    }
    
    // Parse performances
    std::vector<Element> performanceElements = xml::Helper::getChildElements(root, "performance");
    for (const auto& perfElement : performanceElements) {
        try {
            auto performance = Performance::createPerformance("");
            if (performance) {
                performance->parseData(perfElement);
                performances.push_back(std::move(performance));
            }
        } catch (const std::exception& e) {
            std::cerr << "Error parsing performance: " << e.what() << std::endl;
        }
    }
}

} // namespace mpm
} // namespace meico