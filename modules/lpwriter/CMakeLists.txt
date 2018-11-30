cmake_minimum_required(VERSION 3.8)
project(lpmodel-lpwriter)

add_library(lpwriter
        src/LPWriter.cpp)

add_dependencies(lpwriter initialization terms constraints objective linearization)

target_include_directories(lpwriter PRIVATE
        include
        ${CMAKE_SOURCE_DIR}/modules/constraints/include
        ${CMAKE_SOURCE_DIR}/modules/initialization/include
        ${CMAKE_SOURCE_DIR}/modules/objective/include
        ${CMAKE_SOURCE_DIR}/modules/linearization/include
        ${CMAKE_SOURCE_DIR}/modules/terms/include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(lpwriter
        -lterms
        -llinearization
        -lobjective
        -lconstraints
        -linitialization
        -lscabolic-core)