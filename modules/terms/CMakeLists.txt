cmake_minimum_required(VERSION 3.8)
project(lpmodel-terms)

find_package(DGtal REQUIRED)
find_package(OpenCV REQUIRED)

add_library(terms SHARED
        src/sqc/adapters/linel/control/CLinelContribution.cpp
        src/sqc/adapters/linel/control/IteratorManager.cpp
        src/sqc/adapters/linel/handle/BinaryHandle.cpp
        src/sqc/adapters/linel/handle/TernaryHandle.cpp
        src/sqc/control/CTerm.cpp
        src/sqc/control/CConstants.cpp
        src/sqc/CSqc.cpp
        src/data/CData.cpp
        src/API.cpp)

add_dependencies(terms initialization)

target_include_directories(terms PRIVATE
        include
        ${CMAKE_SOURCE_DIR}/modules/initialization/include
        ${SCABOLIC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})
