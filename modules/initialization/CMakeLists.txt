cmake_minimum_required(VERSION 3.8)
project(lpmodel-initialization)

find_package(DGtal REQUIRED)
find_package(OpenCV REQUIRED)


add_library(initialization SHARED
        src/Shapes.cpp
        src/control/CPixel.cpp
        src/control/CEdge.cpp
        src/control/CLinel.cpp
        src/API.cpp)

target_link_libraries(initialization
        -lscabolic-core
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})

target_include_directories(initialization PRIVATE
        include
        ${DGTAL_INCLUDE_DIRS}
        ${DIPACUS_INCLUDE_DIRS}
        ${SCABOLIC_INCLUDE_DIRS}
        )
