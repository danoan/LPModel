cmake_minimum_required(VERSION 3.8)
project(test-sum-sqc)

add_executable(test-sum-sqc sum-sqc.cpp)
add_dependencies(test-sum-sqc initialization)

target_include_directories(test-sum-sqc PRIVATE
        ${CMAKE_SOURCE_DIR}/modules/initialization/include
        ${CMAKE_SOURCE_DIR}/ext
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(test-sum-sqc PRIVATE
        -linitialization
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        ${DGTAL_LIBRARIES})