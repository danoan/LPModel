cmake_minimum_required(VERSION 3.2)
project(nonlinopt)

add_executable(nonlinopt
        src/Minimizer.cpp
        src/MinimizerAR.cpp
        nonlinopt.cpp)

add_dependencies(nonlinopt constraints initialization solassign terms lpwriter)

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
        ${SCABOLIC_INCLUDE_DIRS}
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS}
        )

target_link_libraries( nonlinopt
        -ladept
        -linitialization
        -llpwriter
        -lterms
        -lconstraints
        -lsolassign
        -ldipacus-base
        -lboost_filesystem
        -lboost_system
        ${DGTAL_LIBRARIES})

