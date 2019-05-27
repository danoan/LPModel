cmake_minimum_required(VERSION 3.2)
project(export-cvxopt)

add_executable(export-cvxopt export-cvxopt.cpp src/InputReader.cpp)
add_dependencies(export-cvxopt constraints initialization linearization lpwriter solassign terms utils python-exporter)

target_include_directories(export-cvxopt PRIVATE
        include
        ${LPMODEL_INCLUDE_DIR}/constraints/include
        ${LPMODEL_INCLUDE_DIR}/initialization/include
        ${LPMODEL_INCLUDE_DIR}/linearization/include
        ${LPMODEL_INCLUDE_DIR}/lpwriter/include
        ${LPMODEL_INCLUDE_DIR}/terms/include
        ${LPMODEL_INCLUDE_DIR}/utils/include
        ${LPMODEL_INCLUDE_DIR}/types/include
        ${LPMODEL_INCLUDE_DIR}/python-exporter/include
        ${DIPACUS_INCLUDE_DIRS}
        ${SCABOLIC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(export-cvxopt
        -lpython-exporter
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
