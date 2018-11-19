cmake_minimum_required(VERSION 3.8)
project(lpmodel-terms)

find_package(DGtal REQUIRED)

add_library(terms SHARED
        src/sqc/control/CTerm.cpp
        src/sqc/control/CConstants.cpp
        src/sqc/control/CPointelContribution.cpp
        src/sqc/control/IteratorManager.cpp
        src/sqc/API.cpp)

add_dependencies(terms initialization)
add_dependencies(terms utils)

target_include_directories(terms PRIVATE
        include
        ../utils/include
        ../initialization/include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})
