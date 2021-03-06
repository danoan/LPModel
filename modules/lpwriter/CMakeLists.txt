cmake_minimum_required(VERSION 3.8)
project(lpmodel-lpwriter)

add_library(lpwriter SHARED
        src/LPWriter.cpp)

if(${USE_REMOTE_REPOSITORIES})
    add_dependencies(lpwriter scabolic initialization terms constraints linearization)
else()
    add_dependencies(lpwriter initialization terms constraints linearization)
endif()



target_include_directories(lpwriter PRIVATE
        include
        ${CMAKE_SOURCE_DIR}/modules/constraints/include
        ${CMAKE_SOURCE_DIR}/modules/initialization/include
        ${CMAKE_SOURCE_DIR}/modules/objective/include
        ${CMAKE_SOURCE_DIR}/modules/linearization/include
        ${CMAKE_SOURCE_DIR}/modules/terms/include
        ${CMAKE_SOURCE_DIR}/modules/types/include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(lpwriter
        -lterms
        -lconstraints
        -linitialization
        -lscabolic-core)