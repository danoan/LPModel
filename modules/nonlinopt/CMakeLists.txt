cmake_minimum_required(VERSION 3.2)
project(lpmodel-nonlinopt)

find_package(DGtal REQUIRED)

link_directories(
        ${ADEPT_LIBS_DIR})

add_executable(nonlinopt
        src/Minimizer.cpp
        src/MinimizerAR.cpp
        nonlinopt.cpp)

add_dependencies(nonlinopt constraints initialization solassign terms lpwriter)

target_include_directories(nonlinopt PRIVATE
        include
        ${ADEPT_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/modules/initialization/include
        ${CMAKE_SOURCE_DIR}/modules/terms/include
        ${CMAKE_SOURCE_DIR}/modules/constraints/include
        ${CMAKE_SOURCE_DIR}/modules/lpwriter/include
        ${CMAKE_SOURCE_DIR}/modules/linearization/include
        ${CMAKE_SOURCE_DIR}/modules/solassign/include
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

