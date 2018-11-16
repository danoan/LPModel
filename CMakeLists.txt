cmake_minimum_required(VERSION 3.8)
project(LPModel)

find_package(DGtal REQUIRED)
find_package(OpenCV REQUIRED)

link_directories(ext/lib)

add_executable(LPModel main.cpp)

target_link_libraries(LPModel
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lscabolic-core
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})

target_include_directories(LPModel PRIVATE
        ext/include
        ${DGTAL_INCLUDE_DIRS})