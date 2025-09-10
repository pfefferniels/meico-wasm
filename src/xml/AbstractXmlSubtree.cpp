#include "xml/AbstractXmlSubtree.h"
#include <sstream>

namespace meico {
namespace xml {

Element AbstractXmlSubtree::getXml() const {
    return xml;
}

std::string AbstractXmlSubtree::toXml() const {
    if (!xml) {
        return "";
    }
    
    std::ostringstream oss;
    xml.print(oss);
    return oss.str();
}

void AbstractXmlSubtree::setXml(const Element& xmlElement) {
    this->xml = xmlElement;
}

} // namespace xml
} // namespace meico