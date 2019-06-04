cmake_minimum_required(VERSION 3.8)
project(lpmodel-terms-linel)

find_package(DGtal REQUIRED)
find_package(OpenCV REQUIRED)

add_library(terms-linel SHARED
        src/sqc/adapters/linel/control/CLinelContribution.cpp
        src/sqc/adapters/linel/control/IteratorManager.cpp
        src/sqc/adapters/linel/handle/BinaryHandle.cpp
        src/sqc/adapters/linel/handle/TernaryHandle.cpp
        src/sqc/control/CTerm.cpp
        src/sqc/control/CConstants.cpp
        src/sqc/CSqc.cpp
        src/data/CData.cpp
        src/API.cpp)

add_dependencies(terms-linel initialization-linel)

target_include_directories(terms-linel PRIVATE
        include
        ${CMAKE_SOURCE_DIR}/modules/initialization-linel/include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})
