#include "mpm/elements/maps/data/OrnamentData.h"
#include "xml/Helper.h"
#include <algorithm>
#include <iostream>
#include <sstream>

namespace meico {
namespace mpm {

OrnamentData::OrnamentData() 
    : xml(Element()), xmlId(""), styleName(""), style(nullptr), 
      ornamentDefName(""), ornamentDef(nullptr), date(0.0), scale(0.0) {
}

OrnamentData::OrnamentData(const Element& xml) 
    : xml(xml), xmlId(""), styleName(""), style(nullptr), 
      ornamentDefName(""), ornamentDef(nullptr), date(0.0), scale(0.0) {
    parseFromXml(xml);
}

OrnamentData::OrnamentData(const OrnamentData& other) 
    : xml(other.xml), xmlId(other.xmlId), styleName(other.styleName), 
      style(other.style), ornamentDefName(other.ornamentDefName), 
      ornamentDef(other.ornamentDef), date(other.date), scale(other.scale), 
      noteOrder(other.noteOrder) {
}

OrnamentData& OrnamentData::operator=(const OrnamentData& other) {
    if (this != &other) {
        xml = other.xml;
        xmlId = other.xmlId;
        styleName = other.styleName;
        style = other.style;
        ornamentDefName = other.ornamentDefName;
        ornamentDef = other.ornamentDef;
        date = other.date;
        scale = other.scale;
        noteOrder = other.noteOrder;
    }
    return *this;
}

std::unique_ptr<OrnamentData> OrnamentData::clone() const {
    return std::make_unique<OrnamentData>(*this);
}

void OrnamentData::parseFromXml(const Element& xml) {
    this->xml = xml;
    
    // Parse date attribute (mandatory)
    std::string dateValue = xml::Helper::getAttributeValue(xml, "date");
    if (!dateValue.empty()) {
        this->date = std::stod(dateValue);
    }
    
    // Parse name.ref attribute (mandatory)
    this->ornamentDefName = xml::Helper::getAttributeValue(xml, "name.ref");
    
    // Parse scale attribute (optional)
    std::string scaleValue = xml::Helper::getAttributeValue(xml, "scale");
    if (!scaleValue.empty()) {
        this->scale = std::stod(scaleValue);
    }
    
    // Parse note.order attribute (optional)
    std::string noteOrderValue = xml::Helper::getAttributeValue(xml, "note.order");
    if (!noteOrderValue.empty()) {
        std::string no = noteOrderValue;
        // Trim whitespace
        no.erase(no.find_last_not_of(" \t\n\r\f\v") + 1);
        no.erase(0, no.find_first_not_of(" \t\n\r\f\v"));
        
        this->noteOrder.clear();
        
        if (no == "ascending pitch" || no == "descending pitch") {
            this->noteOrder.push_back(no);
        } else {
            // Parse space-separated note IDs, removing # characters
            std::string cleanStr = no;
            std::replace(cleanStr.begin(), cleanStr.end(), '#', ' ');
            
            std::istringstream iss(cleanStr);
            std::string token;
            while (iss >> token) {
                if (!token.empty()) {
                    this->noteOrder.push_back(token);
                }
            }
        }
    }
    
    // Parse xml:id attribute (optional)
    this->xmlId = xml::Helper::getAttributeValue(xml, "id", "http://www.w3.org/XML/1998/namespace");
}

std::vector<std::vector<Element>> OrnamentData::apply(const std::vector<std::vector<Element>>& chordSequence) {
    std::vector<std::vector<Element>> chordsToAdd;
    
    // If we don't have an ornament definition, return empty
    if (this->ornamentDef == nullptr) {
        return chordsToAdd;
    }
    
    // For now, we'll implement a basic ornament application
    // In the full Java implementation, this would:
    // 1. Apply dynamicsGradient if present
    // 2. Apply temporalSpread if present
    // 3. Generate new notes if required
    
    // TODO: Implement full ornament definition processing
    // For now, just return empty - the ornament will be applied during rendering
    std::cout << "Applying ornament '" << ornamentDefName << "' at date " << date 
              << " with scale " << scale << std::endl;
    
    return chordsToAdd;
}

} // namespace mpm
} // namespace meico