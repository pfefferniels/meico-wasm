#include "mpm/elements/Performance.h"
#include "mpm/elements/Global.h"
#include "mpm/elements/Part.h"
#include "mpm/elements/Dated.h"
#include "mpm/elements/maps/GenericMap.h"
#include "msm/Msm.h"
#include "xml/Helper.h"
#include <iostream>

namespace meico {
namespace mpm {

Performance::Performance(const std::string& performanceName)
    : name(performanceName), pulsesPerQuarter(720), global(std::make_unique<Global>()) {
    init();
}

Performance::Performance(const Element& xml)
    : pulsesPerQuarter(720), global(std::make_unique<Global>()) {
    parseData(xml);
}

std::unique_ptr<Performance> Performance::createPerformance(const std::string& performanceName) {
    return std::make_unique<Performance>(performanceName);
}

const std::string& Performance::getName() const {
    return name;
}

void Performance::setName(const std::string& performanceName) {
    name = performanceName;
}

int Performance::getPPQ() const {
    return pulsesPerQuarter;
}

void Performance::setPPQ(int ppq) {
    pulsesPerQuarter = ppq;
}

Global* Performance::getGlobal() {
    return global.get();
}

const Global* Performance::getGlobal() const {
    return global.get();
}

size_t Performance::getPartCount() const {
    return parts.size();
}

Part* Performance::getPart(size_t index) {
    if (index < parts.size()) {
        return parts[index].get();
    }
    return nullptr;
}

const Part* Performance::getPart(size_t index) const {
    if (index < parts.size()) {
        return parts[index].get();
    }
    return nullptr;
}

void Performance::addPart(std::unique_ptr<Part> part) {
    parts.push_back(std::move(part));
}

std::unique_ptr<msm::Msm> Performance::perform(const msm::Msm& msm) const {
    std::cout << "\nRendering performance \"" << name << "\" into \"" << msm.getTitle() << "\"." << std::endl;
    
    // Clone the input MSM so the original remains unaltered
    auto resultMsm = msm.clone();
    if (!resultMsm) {
        return nullptr;
    }
    
    // Set output filename to avoid overwriting original
    std::string originalFile = resultMsm->getFile();
    if (!originalFile.empty()) {
        std::string newFile = xml::Helper::getFilenameWithoutExtension(originalFile) + "_" + name + ".msm";
        resultMsm->setFile(newFile);
    }
    
    // Convert PPQ if necessary
    resultMsm->convertPPQ(pulsesPerQuarter);
    
    std::cout << "Processing performance data." << std::endl;
    
    // Apply global maps to all parts
    if (global && global->getDated()) {
        const auto& globalMaps = global->getDated()->getAllMaps();
        std::cout << "Applying " << globalMaps.size() << " global maps." << std::endl;
        
        // Find all parts in the MSM
        Element root = resultMsm->getRootElement();
        if (root) {
            for (auto part : root.children("part")) {
                applyMapsToMsmPart(part, globalMaps);
            }
        }
    }
    
    // Apply part-specific maps
    Element root = resultMsm->getRootElement();
    if (root) {
        for (auto part : root.children("part")) {
            // Find matching performance part by name or number
            auto partNameAttr = part.attribute("name");
            if (partNameAttr) {
                std::string partName = partNameAttr.value();
                
                for (const auto& perfPart : parts) {
                    if (perfPart->getName() == partName) {
                        const auto& partMaps = perfPart->getDated()->getAllMaps();
                        std::cout << "Applying " << partMaps.size() << " maps to part: " << partName << std::endl;
                        applyMapsToMsmPart(part, partMaps);
                        break;
                    }
                }
            }
        }
    }
    
    std::cout << "Performance rendering completed." << std::endl;
    return resultMsm;
}

void Performance::parseData(const Element& xmlElement) {
    setXml(xmlElement);
    
    // Parse name
    auto nameAttr = xmlElement.attribute("name");
    if (nameAttr) {
        name = nameAttr.value();
    }
    
    // Parse PPQ
    auto ppqAttr = xmlElement.attribute("pulsesPerQuarter");
    if (ppqAttr) {
        pulsesPerQuarter = xml::Helper::parseInt(ppqAttr.value(), 720);
    }
    
    // TODO: Parse global and parts from XML
}

void Performance::init() {
    // Initialize basic structure
    // Global is already created in constructor
}

void Performance::applyMapsToMsmPart(Element msmPart, const std::vector<std::unique_ptr<GenericMap>>& maps) const {
    for (const auto& map : maps) {
        if (map) {
            bool modified = map->applyToMsmPart(msmPart);
            if (modified) {
                std::cout << "  Applied " << map->getMapType() << " map" << std::endl;
            }
        }
    }
}

} // namespace mpm
} // namespace meico