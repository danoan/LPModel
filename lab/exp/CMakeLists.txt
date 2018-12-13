cmake_minimum_required(VERSION 3.8)
project(lpmodel-lab-exp)

find_package(DGtal REQUIRED)

link_directories(
        ${CMAKE_SOURCE_DIR}/modules/initialization/lib
        ${CMAKE_PREFIX_PATH}
        ${DIPACUS_LIBS_DIR})

add_executable(exp-sum-sqc exp-sum-sqc.cpp)
add_dependencies(exp-sum-sqc initialization)

target_include_directories(exp-sum-sqc PRIVATE
        ${CMAKE_SOURCE_DIR}/modules/initialization/include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(exp-sum-sqc PRIVATE
        -linitialization
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        ${DIPACUS_LIBS_DIR}
        ${DGTAL_LIBRARIES})



