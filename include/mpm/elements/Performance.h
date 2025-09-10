#pragma once

#include "xml/AbstractXmlSubtree.h"

namespace meico {
namespace mpm {

/**
 * Placeholder for Performance class
 */
class Performance : public xml::AbstractXmlSubtree {
public:
    Performance() = default;
    virtual ~Performance() = default;

protected:
    void parseData(const Element& xmlElement) override {
        // TODO: Implement parsing
    }
};

} // namespace mpm
} // namespace meico