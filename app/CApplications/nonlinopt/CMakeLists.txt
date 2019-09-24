cmake_minimum_required(VERSION 3.2)
project(nonlinopt)

add_executable(nonlinopt
        src/activeSet/ActiveSetSolver.cpp
        src/inputReader/InputData.cpp
        src/utils/utils.cpp
        nonlinopt.cpp)

add_dependencies(nonlinopt constraints initialization${LINEL} solassign terms${LINEL} lpwriter lpmodel-glpk lpmodel-utils)

target_include_directories(nonlinopt PRIVATE
        include
        ${ADEPT_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/modules/initialization${LINEL}/include
        ${CMAKE_SOURCE_DIR}/modules/terms${LINEL}/include
        ${CMAKE_SOURCE_DIR}/modules/constraints/include
        ${CMAKE_SOURCE_DIR}/modules/lpwriter/include
        ${CMAKE_SOURCE_DIR}/modules/linearization/include
        ${CMAKE_SOURCE_DIR}/modules/solassign/include
        ${CMAKE_SOURCE_DIR}/modules/types/include
        ${CMAKE_SOURCE_DIR}/modules/utils/include
        ${CMAKE_SOURCE_DIR}/modules/glpk/include
        ${SCABOLIC_INCLUDE_DIRS}
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS}
        )

target_link_libraries( nonlinopt
        -ladept
        -lsolassign
        -llpmodel-utils
        -llpmodel-glpk
        -linitialization${LINEL}
        -llpwriter
        -lterms${LINEL}
        -lconstraints
        -ldipacus-base
        -ldipacus-derivates
        -lboost_filesystem
        -lboost_system
        -lglpk
        ${DGTAL_LIBRARIES})

