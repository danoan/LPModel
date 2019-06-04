cmake_minimum_required(VERSION 3.2)
project(export-cplex-qp)

add_executable(export-cplex-qp export-cplex-qp.cpp src/InputReader.cpp)
add_dependencies(export-cplex-qp constraints initialization${LINEL} linearization lpwriter solassign terms${LINEL} lpmodel-utils)

target_include_directories(export-cplex-qp PRIVATE
        include
        ${LPMODEL_INCLUDE_DIR}/constraints/include
        ${LPMODEL_INCLUDE_DIR}/initialization${LINEL}/include
        ${LPMODEL_INCLUDE_DIR}/linearization/include
        ${LPMODEL_INCLUDE_DIR}/lpwriter/include
        ${LPMODEL_INCLUDE_DIR}/terms/include
        ${LPMODEL_INCLUDE_DIR}/utils/include
        ${LPMODEL_INCLUDE_DIR}/types/include
        ${DIPACUS_INCLUDE_DIRS}
        ${SCABOLIC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(export-cplex-qp
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

