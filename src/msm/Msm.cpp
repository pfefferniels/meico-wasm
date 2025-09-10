#include "msm/Msm.h"
#include "xml/Helper.h"
#include <random>
#include <iomanip>
#include <sstream>

namespace meico {
namespace msm {

const int Msm::CONTROL_CHANGE_DENSITY;

Msm::Msm() : AbstractMsm() {
}

Msm::Msm(const Document& msmDocument) : AbstractMsm(msmDocument) {
}

Msm::Msm(const std::string& filePath) : AbstractMsm(filePath) {
}

Msm::Msm(const std::string& filePath, bool validate, const std::string& schema) 
    : AbstractMsm(filePath, validate, schema) {
}

Msm::Msm(const std::string& xmlString, bool fromString) 
    : AbstractMsm(xmlString, fromString) {
}

Msm::Msm(std::istream& inputStream) : AbstractMsm(inputStream) {
}

std::unique_ptr<Msm> Msm::createMsm(const std::string& title, const std::string& id, int ppq) {
    Document doc;
    Element root = doc.append_child("msm");
    
    // Add title attribute
    root.append_attribute("title") = title.c_str();
    
    // Add id attribute (using XML namespace)
    std::string actualId = id.empty() ? generateUUID() : id;
    auto idAttr = root.append_attribute("id");
    idAttr.set_value(actualId.c_str());
    // Note: pugixml doesn't have direct namespace support like Java XOM
    // In a full implementation, we'd need to handle namespaces properly
    
    // Add PPQ attribute
    root.append_attribute("pulsesPerQuarter") = ppq;
    
    // Create global containers
    Element global = root.append_child("global");
    Element dated = global.append_child("dated");
    Element header = global.append_child("header");
    
    // Add global maps to dated
    dated.append_child("timeSignatureMap");
    dated.append_child("keySignatureMap");
    dated.append_child("markerMap");
    dated.append_child("sectionMap");
    dated.append_child("phraseMap");
    dated.append_child("sequencingMap");
    dated.append_child("pedalMap");
    dated.append_child("miscMap");
    
    auto msm = std::make_unique<Msm>();
    msm->setDocument(doc);
    return msm;
}

std::unique_ptr<Msm> Msm::clone() const {
    auto cloned = std::make_unique<Msm>();
    cloned->setDocument(getDocument());
    cloned->setFile(getFile());
    cloned->setIsValid(getIsValid());
    return cloned;
}

std::string Msm::getTitle() const {
    Element root = getRootElement();
    if (root) {
        auto titleAttr = root.attribute("title");
        if (titleAttr) {
            return titleAttr.value();
        }
    }
    return "";
}

int Msm::getPPQ() const {
    Element root = getRootElement();
    if (root) {
        // Try camelCase first (our generated XML)
        auto ppqAttr = root.attribute("pulsesPerQuarter");
        if (ppqAttr) {
            return xml::Helper::parseInt(ppqAttr.value(), 720);
        }
        
        // Try all lowercase (Bach MSM test data format)
        ppqAttr = root.attribute("pulsesperquarter");
        if (ppqAttr) {
            return xml::Helper::parseInt(ppqAttr.value(), 720);
        }
    }
    return 720; // default PPQ
}

void Msm::convertPPQ(int ppq) {
    int currentPPQ = getPPQ();
    if (currentPPQ == ppq) {
        return; // no conversion needed
    }
    
    double scaleFactor = static_cast<double>(ppq) / currentPPQ;
    Element root = getRootElement();
    if (!root) {
        return;
    }
    
    // Update PPQ attribute
    root.attribute("pulsesPerQuarter").set_value(ppq);
    
    // Convert all timing attributes (date, date.end, duration)
    // This is a simplified version - in a full implementation, 
    // we'd need to recursively traverse and update all timing attributes
    convertTimingAttributes(root, scaleFactor);
}

Element Msm::getGlobal() {
    Element root = getRootElement();
    if (root) {
        return xml::Helper::getFirstChildElement(root, "global");
    }
    return Element();
}

const Element Msm::getGlobal() const {
    Element root = getRootElement();
    if (root) {
        return xml::Helper::getFirstChildElement(root, "global");
    }
    return Element();
}

bool Msm::writeMsm() {
    if (getFile().empty()) {
        return false;
    }
    return writeToFile(getFile());
}

bool Msm::writeMsm(const std::string& filename) {
    return writeToFile(filename);
}

void Msm::cleanup(std::vector<std::unique_ptr<Msm>>& msmList) {
    msmList.clear();
}

void Msm::cleanup(std::unique_ptr<Msm>& msm) {
    msm.reset();
}

std::string Msm::generateUUID() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);
    std::uniform_int_distribution<> dis2(8, 11);
    
    std::stringstream ss;
    ss << std::hex;
    
    for (int i = 0; i < 8; i++) {
        ss << dis(gen);
    }
    ss << "-";
    for (int i = 0; i < 4; i++) {
        ss << dis(gen);
    }
    ss << "-4";
    for (int i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    ss << dis2(gen);
    for (int i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    for (int i = 0; i < 12; i++) {
        ss << dis(gen);
    }
    
    return ss.str();
}

void Msm::convertTimingAttributes(Element element, double scaleFactor) {
    if (!element) {
        return;
    }
    
    // Convert timing attributes for this element
    auto dateAttr = element.attribute("date");
    if (dateAttr) {
        double value = xml::Helper::parseDouble(dateAttr.value());
        dateAttr.set_value(std::to_string(value * scaleFactor).c_str());
    }
    
    auto dateEndAttr = element.attribute("date.end");
    if (dateEndAttr) {
        double value = xml::Helper::parseDouble(dateEndAttr.value());
        dateEndAttr.set_value(std::to_string(value * scaleFactor).c_str());
    }
    
    auto durationAttr = element.attribute("duration");
    if (durationAttr) {
        double value = xml::Helper::parseDouble(durationAttr.value());
        durationAttr.set_value(std::to_string(value * scaleFactor).c_str());
    }
    
    // Recursively convert child elements
    for (auto child : element.children()) {
        convertTimingAttributes(child, scaleFactor);
    }
}

} // namespace msm
} // namespace meico