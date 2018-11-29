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
        ${CMAKE_SOURCE_DIR}/modules/initialization/include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(test-serialization PRIVATE
        -linitialization
        -lscabolic-core
        -ldipacus-components
        -ldipacus-base
        ${DIPACUS_LIBS_DIR}
        ${DGTAL_LIBRARIES})



add_executable(test-levels test-levels.cpp)
add_dependencies(test-levels initialization solassign terms)

target_include_directories(test-levels PRIVATE
        ${CMAKE_SOURCE_DIR}/modules/initialization/include
        ${CMAKE_SOURCE_DIR}/modules/solassign/include
        ${CMAKE_SOURCE_DIR}/modules/terms/include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(test-levels PRIVATE
        -lterms
        -lsolassign
        -linitialization
        -lscabolic-core
        -ldipacus-components
        -ldipacus-base
        ${DIPACUS_LIBS_DIR}
        ${DGTAL_LIBRARIES})


