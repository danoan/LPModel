cmake_minimum_required(VERSION 3.8)
project(lpmodel-terms)

find_package(DGtal REQUIRED)

add_library(terms SHARED
        src/sqc/adapters/pointel/control/CPointelContribution.cpp
        src/sqc/adapters/pointel/control/IteratorManager.cpp
        src/sqc/adapters/pointel/handle/BinaryHandle.cpp
        src/sqc/adapters/pointel/handle/TernaryHandle.cpp
        src/sqc/adapters/linel/control/CLinelContribution.cpp
        src/sqc/adapters/linel/control/IteratorManager.cpp
        src/sqc/adapters/linel/handle/BinaryHandle.cpp
        src/sqc/adapters/linel/handle/TernaryHandle.cpp
        src/sqc/control/CTerm.cpp
        src/sqc/control/CConstants.cpp
        src/sqc/API.cpp)

add_dependencies(terms initialization)
add_dependencies(terms utils)

target_include_directories(terms PRIVATE
        include
        ../utils/include
        ../initialization/include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})
