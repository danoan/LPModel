cmake_minimum_required(VERSION 3.8)
project(lpmodel-constraints)

find_package(DGtal REQUIRED)

add_library(constraints SHARED
        src/ClosedAndConnected.cpp)

add_dependencies(constraints initialization terms)

target_include_directories(constraints PRIVATE
        include
        ../initialization/include
        ../terms/include
        ${DGTAL_INCLUDE_DIRS})
