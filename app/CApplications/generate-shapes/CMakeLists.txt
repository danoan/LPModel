cmake_minimum_required(VERSION 3.2)
project(generate-shapes)

find_package(OpenCV REQUIRED)
add_executable(generate-shapes generate-shapes.cpp)
if(${USE_REMOTE_REPOSITORIES})
    add_dependencies(generate-shapes dipacus)
endif()

target_include_directories(generate-shapes PRIVATE
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(generate-shapes
        -ldipacus-base
        -lboost_system
        -lboost_filesystem
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})

