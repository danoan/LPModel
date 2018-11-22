cmake_minimum_required(VERSION 3.8)
project(lpmodel-lpwriter)

add_library(lpwriter
        src/LPWriter.cpp)

target_include_directories(lpwriter PRIVATE
        include
        ../constraints/include
        ../initialization/include
        ${DGTAL_INCLUDE_DIRS})