cmake_minimum_required(VERSION 3.8)
project(lpmodel-utils)

find_package(DGtal REQUIRED)

add_library(utils SHARED
        src/dispUtils.cpp)

add_dependencies(utils initialization)

target_include_directories(utils PRIVATE
        include
        ${CMAKE_SOURCE_DIR}/modules/initialization/include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(utils
        -lscabolic-core
        ${DGTAL_LIBRARIES}
        )