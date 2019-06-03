cmake_minimum_required(VERSION 3.2)
project(nonlinopt)

add_executable(nonlinopt
        src/activeSet/ActiveSetSolver.cpp
        src/inputReader/InputData.cpp
        src/utils/utils.cpp
        nonlinopt.cpp)

add_dependencies(nonlinopt constraints initialization solassign terms lpwriter lpmodel-glpk)

target_include_directories(nonlinopt PRIVATE
        include
        ${ADEPT_INCLUDE_DIRS}
        ${LPMODEL_INCLUDE_DIR}/initialization/include
        ${LPMODEL_INCLUDE_DIR}/terms/include
        ${LPMODEL_INCLUDE_DIR}/constraints/include
        ${LPMODEL_INCLUDE_DIR}/lpwriter/include
        ${LPMODEL_INCLUDE_DIR}/linearization/include
        ${LPMODEL_INCLUDE_DIR}/solassign/include
        ${LPMODEL_INCLUDE_DIR}/types/include
        ${LPMODEL_INCLUDE_DIR}/utils/include
        ${LPMODEL_INCLUDE_DIR}/glpk/include
        ${SCABOLIC_INCLUDE_DIRS}
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS}
        )

target_link_libraries( nonlinopt
        -ladept
        -lsolassign
        -llpmodel-utils
        -llpmodel-glpk
        -linitialization
        -llpwriter
        -lterms
        -lconstraints
        -ldipacus-base
        -ldipacus-derivates
        -lboost_filesystem
        -lboost_system
        -lglpk
        ${DGTAL_LIBRARIES})

