cmake_minimum_required(VERSION 3.2)
project(export-grid)

add_executable(export-grid export-grid.cpp src/InputReader.cpp)
add_dependencies(export-grid initialization${LINEL})

target_include_directories(export-grid PRIVATE
        include
        ${CMAKE_SOURCE_DIR}/modules/initialization${LINEL}/include
        ${DIPACUS_INCLUDE_DIRS}
        ${SCABOLIC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(export-grid
        -linitialization${LINEL}
        -ldipacus-base
        -lboost_system
        -lboost_filesystem
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})
