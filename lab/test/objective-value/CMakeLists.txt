cmake_minimum_required(VERSION 3.2)
project(test-objective-value)

add_executable(test-objective-value
        test-objective-value.cpp
        ${CMAKE_SOURCE_DIR}/app/CApplications/nonlinopt/src/activeSet/ActiveSetSolver.cpp)


add_dependencies(test-objective-value initialization${LINEL} constraints terms${LINEL} lpwriter lpmodel-glpk lpmodel-utils)

target_include_directories(test-objective-value PRIVATE
        ${ADEPT_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/modules/initialization${LINEL}/include
        ${CMAKE_SOURCE_DIR}/modules/constraints/include
        ${CMAKE_SOURCE_DIR}/modules/terms/include
        ${CMAKE_SOURCE_DIR}/modules/lpwriter/include
        ${CMAKE_SOURCE_DIR}/modules/types/include
        ${CMAKE_SOURCE_DIR}/modules/linearization/include
        ${CMAKE_SOURCE_DIR}/modules/glpk/include
        ${CMAKE_SOURCE_DIR}/modules/solassign/include
        ${CMAKE_SOURCE_DIR}/modules/utils/include
        ${CMAKE_SOURCE_DIR}/app/CApplications/nonlinopt/include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(test-objective-value
        -ladept
        -llpmodel-glpk
        -llpmodel-utils
        -lsolassign
        -linitialization${LINEL}
        -lconstraints
        -lterms${LINEL}
        -llpwriter
        -lscabolic-core
        -ldipacus-derivates
        -ldipacus-base
        -lboost_system
        -lboost_filesystem
        ${DGTAL_LIBRARIES})