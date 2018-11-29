cmake_minimum_required(VERSION 3.8)
project(lpmodel-objective)

find_package(DGtal REQUIRED)

add_library(objective SHARED
        src/Objective.cpp)

target_include_directories(objective PRIVATE
        include
        ${CMAKE_SOURCE_DIR}/modules/initialization/include
        ${CMAKE_SOURCE_DIR}/modules/utils/include
        ${CMAKE_SOURCE_DIR}/modules/terms/include
        ${DGTAL_INCLUDE_DIRS})

