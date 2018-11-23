cmake_minimum_required(VERSION 3.8)
project(lpmodel-lab-serialization)

find_package(DGtal REQUIRED)

link_directories(
        ../modules/initialization/lib
        ${CMAKE_PREFIX_PATH}
        ${DIPACUS_LIBS_DIR})

add_executable(test-serialization test-serialization.cpp)
add_dependencies(test-serialization initialization)

target_include_directories(test-serialization PRIVATE
        ../modules/initialization/include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(test-serialization PRIVATE
        -linitialization
        -lscabolic-core
        -ldipacus-base
        ${DIPACUS_LIBS_DIR}
        ${DGTAL_LIBRARIES})


