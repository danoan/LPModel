cmake_minimum_required(VERSION 3.2)
project(export-grid)

add_executable(export-grid export-grid.cpp src/InputReader.cpp)
add_dependencies(export-grid initialization)

target_include_directories(export-grid PRIVATE
        include
        ${LPMODEL_INCLUDE_DIR}/initialization/include
        ${DIPACUS_INCLUDE_DIRS}
        ${SCABOLIC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(export-grid
        -linitialization
        -ldipacus-base
        -lboost_system
        -lboost_filesystem
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})
