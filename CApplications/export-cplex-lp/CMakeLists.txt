cmake_minimum_required(VERSION 3.2)
project(export-cplex-lp)

add_executable(export-cplex-lp export-cplex-lp.cpp src/InputReader.cpp)
add_dependencies(export-cplex-lp constraints initialization linearization lpwriter solassign terms utils)

target_include_directories(export-cplex-lp PRIVATE
        include
        ${LPMODEL_INCLUDE_DIR}/constraints/include
        ${LPMODEL_INCLUDE_DIR}/initialization/include
        ${LPMODEL_INCLUDE_DIR}/linearization/include
        ${LPMODEL_INCLUDE_DIR}/lpwriter/include
        ${LPMODEL_INCLUDE_DIR}/terms/include
        ${LPMODEL_INCLUDE_DIR}/utils/include
        ${DIPACUS_INCLUDE_DIRS}
        ${SCABOLIC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS}
)


target_link_libraries(export-cplex-lp
        -llpwriter
        -lconstraints
        -lterms
        -linitialization
        -lscabolic-core
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lutils
        -lboost_system
        -lboost_filesystem
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})

