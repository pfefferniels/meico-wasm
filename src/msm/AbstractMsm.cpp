#include "msm/AbstractMsm.h"

namespace meico {
namespace msm {

AbstractMsm::AbstractMsm() : xml::XmlBase() {
}

AbstractMsm::AbstractMsm(const Document& document) : xml::XmlBase(document) {
}

AbstractMsm::AbstractMsm(const std::string& filePath) : xml::XmlBase(filePath) {
}

AbstractMsm::AbstractMsm(const std::string& filePath, bool validate, const std::string& schema) 
    : xml::XmlBase(filePath, validate, schema) {
}

AbstractMsm::AbstractMsm(const std::string& xmlString, bool fromString) 
    : xml::XmlBase(xmlString, fromString) {
}

AbstractMsm::AbstractMsm(std::istream& inputStream) : xml::XmlBase(inputStream) {
}

} // namespace msm
} // namespace meico