cmake_minimum_required(VERSION 3.8)
project(test-lp-creation)


add_executable(test-lp-creation test-lp-creation.cpp)
target_compile_definitions(test-lp-creation PRIVATE -DPROJECT_DIR=\"${CMAKE_SOURCE_DIR}\")
add_dependencies(test-lp-creation initialization lpmodel-utils terms linearization lpwriter)

target_include_directories(test-lp-creation PRIVATE
        ${CMAKE_SOURCE_DIR}/modules/initialization/include
        ${CMAKE_SOURCE_DIR}/modules/terms/include
        ${CMAKE_SOURCE_DIR}/modules/constraints/include
        ${CMAKE_SOURCE_DIR}/modules/linearization/include
        ${CMAKE_SOURCE_DIR}/modules/lpwriter/include
        ${CMAKE_SOURCE_DIR}/modules/utils/include
        ${CMAKE_SOURCE_DIR}/modules/types/include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(test-lp-creation PRIVATE
        -llpmodel-utils
        -llpwriter
        -lconstraints
        -lterms
        -linitialization
        -lscabolic-core
        -ldipacus-components
        -lboost_system
        -lboost_filesystem
        ${DGTAL_LIBRARIES})

