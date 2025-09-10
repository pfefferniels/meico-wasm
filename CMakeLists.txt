cmake_minimum_required(VERSION 3.14)
project(meico-cpp VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)
include_directories(externals/pugixml)

# Source files
set(SOURCES
    externals/pugixml/pugixml.cpp
    src/xml/XmlBase.cpp
    src/xml/AbstractXmlSubtree.cpp
    src/xml/Helper.cpp
    src/msm/AbstractMsm.cpp
    src/msm/Msm.cpp
    src/mpm/Mpm.cpp
    src/mpm/elements/Performance.cpp
    src/mpm/elements/Global.cpp
    src/mpm/elements/Part.cpp
    src/mpm/elements/Dated.cpp
    src/mpm/elements/maps/GenericMap.cpp
    src/mpm/elements/maps/DynamicsMap.cpp
    src/mpm/elements/maps/data/DynamicsData.cpp
    src/mpm/elements/maps/ArticulationMap.cpp
    src/mpm/elements/maps/MetricalAccentuationMap.cpp
    src/supplementary/KeyValue.cpp
)

# Header files
set(HEADERS
    include/xml/XmlBase.h
    include/xml/AbstractXmlSubtree.h
    include/xml/Helper.h
    include/msm/AbstractMsm.h
    include/msm/Msm.h
    include/mpm/Mpm.h
    include/mpm/elements/Performance.h
    include/mpm/elements/Global.h
    include/mpm/elements/Part.h
    include/mpm/elements/Dated.h
    include/mpm/elements/maps/GenericMap.h
    include/mpm/elements/maps/DynamicsMap.h
    include/mpm/elements/maps/data/DynamicsData.h
    include/mpm/elements/maps/ArticulationMap.h
    include/mpm/elements/maps/MetricalAccentuationMap.h
    include/mpm/elements/metadata/Metadata.h
    include/supplementary/KeyValue.h
    include/common/common.h
)

# Create static library
add_library(meico-cpp STATIC ${SOURCES} ${HEADERS})

# Test executable
add_executable(meico-test
    test/main.cpp
    test/mpm/MpmTestUtils.cpp
)

target_link_libraries(meico-test meico-cpp)
target_include_directories(meico-test PRIVATE test)

# Install
install(TARGETS meico-cpp
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY include/ DESTINATION include)