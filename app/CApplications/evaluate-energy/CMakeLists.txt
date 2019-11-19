cmake_minimum_required(VERSION 3.2)
project(evaluate-energy)

add_executable(evaluate-energy evaluate-energy.cpp ${CMAKE_SOURCE_DIR}/app/CApplications/nonlinopt/src/activeSet/ActiveSetSolver.cpp)

add_dependencies(evaluate-energy initialization solassign lpmodel-utils lpwriter constraints)

target_include_directories(evaluate-energy PRIVATE
        ${CMAKE_SOURCE_DIR}/modules/solassign/include
        ${CMAKE_SOURCE_DIR}/modules/initialization/include
        ${CMAKE_SOURCE_DIR}/modules/utils/include
        ${CMAKE_SOURCE_DIR}/modules/terms/include
        ${CMAKE_SOURCE_DIR}/modules/constraints/include
        ${CMAKE_SOURCE_DIR}/modules/lpwriter/include
        ${CMAKE_SOURCE_DIR}/modules/linearization/include
        ${CMAKE_SOURCE_DIR}/modules/types/include
        ${CMAKE_SOURCE_DIR}/modules/glpk/include
        ${CMAKE_SOURCE_DIR}/app/CApplications/nonlinopt/include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(evaluate-energy
        -ladept
        -llpwriter
        -lconstraints
        -lterms
        -lsolassign
        -linitialization
        -ldipacus-base
        -ldipacus-components
        -ldipacus-derivates
        -lboost_system
        -lboost_filesystem
        ${DGTAL_LIBRARIES})