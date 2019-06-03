cmake_minimum_required(VERSION 3.2)
project(lpmodel-glpk)

add_library(lpmodel-glpk SHARED
        src/glpk.cpp)

target_include_directories(lpmodel-glpk PRIVATE
        include)

target_link_libraries(lpmodel-glpk
        -lglpk)