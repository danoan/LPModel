cmake_minimum_required(VERSION 3.2)
project(generate-shapes)


add_executable(generate-shapes generate-shapes.cpp)
add_dependencies(generate-shapes initialization${LINEL})

target_include_directories(generate-shapes PRIVATE
        ${LPMODEL_INCLUDE_DIR}/initialization${LINEL}/include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(generate-shapes
        -linitialization${LINEL}
        -ldipacus-base
        -lboost_system
        -lboost_filesystem
        ${DGTAL_LIBRARIES})

