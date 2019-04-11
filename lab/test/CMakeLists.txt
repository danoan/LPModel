cmake_minimum_required(VERSION 3.8)
project(lpmodel-lab-test)

find_package(DGtal REQUIRED)

link_directories(
        ${CMAKE_SOURCE_DIR}/modules/initialization/lib
        ${CMAKE_PREFIX_PATH}
        ${DIPACUS_LIBS_DIR})

add_executable(test-serialization test-serialization.cpp)
target_compile_definitions(test-serialization PRIVATE -DPROJECT_DIR=\"${CMAKE_SOURCE_DIR}\")

add_dependencies(test-serialization initialization)

target_include_directories(test-serialization PRIVATE
        ${CMAKE_SOURCE_DIR}/modules/initialization/include
        ${CMAKE_SOURCE_DIR}/modules/utils/include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(test-serialization PRIVATE
        -linitialization
        -lutils
        -lscabolic-core
        -ldipacus-components
        -ldipacus-base
        -lboost_system
        -lboost_filesystem
        ${DIPACUS_LIBS_DIR}
        ${DGTAL_LIBRARIES})



add_executable(test-lp-creation test-lp-creation.cpp)
target_compile_definitions(test-lp-creation PRIVATE -DPROJECT_DIR=\"${CMAKE_SOURCE_DIR}\")
add_dependencies(test-lp-creation initialization utils terms linearization lpwriter)

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
        -lutils
        -llpwriter
        -lconstraints
        -lterms
        -linitialization
        -lscabolic-core
        -ldipacus-components
        -lboost_system
        -lboost_filesystem
        ${DIPACUS_LIBS_DIR}
        ${DGTAL_LIBRARIES})

