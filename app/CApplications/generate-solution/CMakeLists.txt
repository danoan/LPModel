cmake_minimum_required(VERSION 3.2)
project(generate-solution)

add_executable(generate-solution generate-solution.cpp)
add_dependencies(generate-solution initialization solassign lpmodel-utils)

target_include_directories(generate-solution PRIVATE
        ${CMAKE_SOURCE_DIR}/modules/solassign/include
        ${CMAKE_SOURCE_DIR}/modules/initialization/include
        ${CMAKE_SOURCE_DIR}/modules/utils/include
        ${CMAKE_SOURCE_DIR}/modules/terms/include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(generate-solution
        -lterms
        -lsolassign
        -linitialization
        -ldipacus-base
        ${DGTAL_LIBRARIES})