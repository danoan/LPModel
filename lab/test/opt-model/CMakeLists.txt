cmake_minimum_required(VERSION 3.8)
project(test-opt-model)

add_executable(test-opt-model
        test-opt-model.cpp)

add_dependencies(test-opt-model opt-model)

target_include_directories(test-opt-model PRIVATE
        include
        ${CMAKE_SOURCE_DIR}/modules/opt-model/include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(test-opt-model
        -lopt-model
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lboost_filesystem
        -lboost_system
        ${DGTAL_LIBRARIES})