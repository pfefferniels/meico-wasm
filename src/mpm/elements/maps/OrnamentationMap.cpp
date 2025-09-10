#include "mpm/elements/maps/OrnamentationMap.h"
#include "mpm/Mpm.h"
#include "xml/Helper.h"
#include <algorithm>
#include <iostream>

namespace meico {
namespace mpm {

OrnamentationMap::OrnamentationMap() : GenericMap(Mpm::ORNAMENTATION_MAP) {
}

std::unique_ptr<OrnamentationMap> OrnamentationMap::createOrnamentationMap() {
    return std::make_unique<OrnamentationMap>();
}

void OrnamentationMap::parseData(const Element& xmlElement) {
    GenericMap::parseData(xmlElement);
    
    // TODO: Parse ornamentation data from XML
    // For now, this is a placeholder
}

int OrnamentationMap::addOrnament(double date, const std::string& nameRef, double scale, 
                                 const std::vector<std::string>& noteOrder, const std::string& id) {
    auto data = std::make_unique<OrnamentData>();
    data->date = date;
    data->ornamentDefName = nameRef;
    data->scale = scale;
    data->noteOrder = noteOrder;
    data->xmlId = id;
    
    return insertOrnamentData(date, std::move(data));
}

int OrnamentationMap::addOrnament(const OrnamentData& data) {
    auto dataCopy = std::make_unique<OrnamentData>(data);
    return insertOrnamentData(data.date, std::move(dataCopy));
}

int OrnamentationMap::insertOrnamentData(double date, std::unique_ptr<OrnamentData> data) {
    // Find the insertion point to keep the vector sorted by date
    auto it = std::lower_bound(ornamentData.begin(), ornamentData.end(), date,
        [](const supplementary::KeyValue<double, std::unique_ptr<OrnamentData>>& kv, double date) {
            return kv.getKey() < date;
        });
    
    // Insert the new element
    auto pos = ornamentData.insert(it, supplementary::KeyValue<double, std::unique_ptr<OrnamentData>>(date, std::move(data)));
    
    // Return the index
    return static_cast<int>(std::distance(ornamentData.begin(), pos));
}

OrnamentData* OrnamentationMap::getOrnamentDataAt(double date) const {
    int index = getElementIndexBeforeAt(date);
    if (index >= 0) {
        return ornamentData[index].getValue().get();
    }
    return nullptr;
}

OrnamentData* OrnamentationMap::getOrnamentDataOf(int index) const {
    if (index >= 0 && index < static_cast<int>(ornamentData.size())) {
        return ornamentData[index].getValue().get();
    }
    return nullptr;
}

size_t OrnamentationMap::size() const {
    return ornamentData.size();
}

bool OrnamentationMap::isEmpty() const {
    return ornamentData.empty();
}

int OrnamentationMap::getElementIndexBeforeAt(double date) const {
    for (int i = static_cast<int>(ornamentData.size()) - 1; i >= 0; --i) {
        if (ornamentData[i].getKey() <= date) {
            return i;
        }
    }
    return -1;
}

bool OrnamentationMap::applyToMsmPart(Element msmPart) {
    if (!msmPart || ornamentData.empty()) {
        return false;
    }
    
    bool modified = false;
    
    // Find all note elements in the part
    auto scoreElement = xml::Helper::getFirstChildElement(msmPart, "dated");
    if (scoreElement) {
        scoreElement = xml::Helper::getFirstChildElement(scoreElement, "score");
        if (scoreElement) {
            // Process each ornament in the map
            for (const auto& ornamentEntry : ornamentData) {
                double ornamentDate = ornamentEntry.getKey();
                const OrnamentData* ornament = ornamentEntry.getValue().get();
                
                // Find notes at this date
                for (auto note : scoreElement.children("note")) {
                    auto dateAttr = note.attribute("date");
                    if (dateAttr) {
                        double noteDate = xml::Helper::parseDouble(dateAttr.value());
                        
                        // Apply ornament if note is at the ornament date
                        if (std::abs(noteDate - ornamentDate) < 1e-6) { // floating point comparison
                            if (applyOrnamentToNote(note, *ornament)) {
                                modified = true;
                            }
                        }
                    }
                }
            }
        }
    }
    
    return modified;
}

bool OrnamentationMap::applyOrnamentToNote(Element note, const OrnamentData& ornamentData) {
    bool modified = false;
    
    // Add ornament attributes to the note
    // For now, we'll add basic ornament markers
    
    // Add ornament.type attribute
    if (!ornamentData.ornamentDefName.empty()) {
        note.append_attribute("ornament.type") = ornamentData.ornamentDefName.c_str();
        modified = true;
    }
    
    // Add ornament.scale attribute if not default
    if (ornamentData.scale != 0.0 && ornamentData.scale != 1.0) {
        note.append_attribute("ornament.scale") = std::to_string(ornamentData.scale).c_str();
        modified = true;
    }
    
    // Add ornament.note.order attribute if specified
    if (!ornamentData.noteOrder.empty()) {
        std::string noteOrderStr;
        for (size_t i = 0; i < ornamentData.noteOrder.size(); ++i) {
            if (i > 0) noteOrderStr += " ";
            noteOrderStr += ornamentData.noteOrder[i];
        }
        note.append_attribute("ornament.note.order") = noteOrderStr.c_str();
        modified = true;
    }
    
    // TODO: Apply more sophisticated ornament transformations
    // This would involve:
    // 1. Looking up the ornament definition from styles
    // 2. Applying dynamics gradients 
    // 3. Applying temporal spreads
    // 4. Adding new notes if required
    
    std::cout << "Applied ornament '" << ornamentData.ornamentDefName 
              << "' to note at date " << ornamentData.date << std::endl;
    
    return modified;
}

} // namespace mpm
} // namespace meico