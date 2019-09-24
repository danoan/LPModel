cmake_minimum_required(VERSION 3.2)
project(export-cplex-lp)

add_executable(export-cplex-lp export-cplex-lp.cpp src/InputReader.cpp)
add_dependencies(export-cplex-lp constraints initialization${LINEL} linearization lpwriter solassign terms${LINEL} lpmodel-utils)

target_include_directories(export-cplex-lp PRIVATE
        include
        ${CMAKE_SOURCE_DIR}/modules/constraints/include
        ${CMAKE_SOURCE_DIR}/modules/initialization${LINEL}/include
        ${CMAKE_SOURCE_DIR}/modules/linearization/include
        ${CMAKE_SOURCE_DIR}/modules/lpwriter/include
        ${CMAKE_SOURCE_DIR}/modules/terms${LINEL}/include
        ${CMAKE_SOURCE_DIR}/modules/utils/include
        ${CMAKE_SOURCE_DIR}/modules/types/include
        ${DIPACUS_INCLUDE_DIRS}
        ${SCABOLIC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS}
)


target_link_libraries(export-cplex-lp
        -llpwriter
        -lconstraints
        -lterms${LINEL}
        -linitialization${LINEL}
        -lscabolic-core
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -llpmodel-utils
        -lboost_system
        -lboost_filesystem
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})

