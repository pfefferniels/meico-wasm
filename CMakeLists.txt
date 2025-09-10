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
)

# Header files
set(HEADERS
    include/xml/XmlBase.h
    include/xml/AbstractXmlSubtree.h
    include/xml/Helper.h
    include/msm/AbstractMsm.h
    include/common/common.h
)

# Create static library
add_library(meico-cpp STATIC ${SOURCES} ${HEADERS})

# Test executable
add_executable(meico-test
    test/main.cpp
)

target_link_libraries(meico-test meico-cpp)

# Install
install(TARGETS meico-cpp
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY include/ DESTINATION include)