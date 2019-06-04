cmake_minimum_required(VERSION 3.2)
project(generate-solution)

add_executable(generate-solution generate-solution.cpp)
add_dependencies(generate-solution initialization${LINEL} solassign lpmodel-utils)

target_include_directories(generate-solution PRIVATE
        ${LPMODEL_INCLUDE_DIR}/solassign/include
        ${LPMODEL_INCLUDE_DIR}/initialization${LINEL}/include
        ${LPMODEL_INCLUDE_DIR}/utils/include
        ${LPMODEL_INCLUDE_DIR}/terms/include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(generate-solution
        -lterms${LINEL}
        -lsolassign
        -linitialization${LINEL}
        -ldipacus-base
        ${DGTAL_LIBRARIES})