cmake_minimum_required(VERSION 3.8)
project(LPModel)

find_package(DGtal REQUIRED)
find_package(OpenCV REQUIRED)

link_directories(cmake-build-debug/modules/initialization
        ${DIPACUS_LIBS_DIR})


add_subdirectory(modules/initialization)

add_executable(LPModel main.cpp)
add_dependencies(LPModel initialization)

target_link_libraries(LPModel
        -linitialization
        -lscabolic-core
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})

target_include_directories(LPModel PRIVATE
        ext/include
        modules/initialization/include
        ${DGTAL_INCLUDE_DIRS})

