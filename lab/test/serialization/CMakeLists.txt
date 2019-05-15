cmake_minimum_required(VERSION 3.8)
project(test-serialization)


add_executable(test-serialization test-serialization.cpp)
target_compile_definitions(test-serialization PRIVATE -DPROJECT_DIR=\"${CMAKE_SOURCE_DIR}\")

add_dependencies(test-serialization initialization)

target_include_directories(test-serialization PRIVATE
        ${CMAKE_SOURCE_DIR}/modules/initialization/include
        ${CMAKE_SOURCE_DIR}/modules/utils/include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(test-serialization PRIVATE
        -linitialization
        -lutils
        -lscabolic-core
        -ldipacus-components
        -ldipacus-base
        -lboost_system
        -lboost_filesystem
        ${DGTAL_LIBRARIES})
