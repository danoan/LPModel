cmake_minimum_required(VERSION 3.8)
project(lpmodel-utils)

find_package(DGtal REQUIRED)

add_library(lpmodel-utils SHARED
        src/dispUtils.cpp
        src/compUtils.cpp)

if(${USE_REMOTE_REPOSITORIES})
    add_dependencies(lpmodel-utils scabolic initialization)
else()
    add_dependencies(lpmodel-utils initialization)
endif()


target_include_directories(lpmodel-utils PRIVATE
        include
        ${CMAKE_SOURCE_DIR}/modules/initialization/include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(lpmodel-utils
        -lscabolic-core
        ${DGTAL_LIBRARIES}
        )