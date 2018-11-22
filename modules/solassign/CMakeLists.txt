cmake_minimum_required(VERSION 3.8)
project(lpmodel-solassign)

find_package(DGtal REQUIRED)

add_library(solassign SHARED
        src/SolutionAssignment.cpp)

add_dependencies(solassign initialization)

target_include_directories(solassign PRIVATE
        include
        ../initialization/include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(solassign PRIVATE
        -linitialization)