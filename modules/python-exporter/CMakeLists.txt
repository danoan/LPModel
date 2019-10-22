cmake_minimum_required(VERSION 3.2)
project(python-exporter)

add_library(python-exporter SHARED src/PythonExporter.cpp )


if(${USE_REMOTE_REPOSITORIES})
    add_dependencies(python-exporter dipacus scabolic initialization constraints linearization lpwriter terms lpmodel-utils)
else()
    add_dependencies(python-exporter initialization constraints linearization lpwriter terms lpmodel-utils)
endif()


target_include_directories(python-exporter PRIVATE
        include
        ${CMAKE_SOURCE_DIR}/modules/constraints/include
        ${CMAKE_SOURCE_DIR}/modules/initialization/include
        ${CMAKE_SOURCE_DIR}/modules/linearization/include
        ${CMAKE_SOURCE_DIR}/modules/lpwriter/include
        ${CMAKE_SOURCE_DIR}/modules/terms/include
        ${CMAKE_SOURCE_DIR}/modules/utils/include
        ${CMAKE_SOURCE_DIR}/modules/types/include
        ${DIPACUS_INCLUDE_DIRS}
        ${SCABOLIC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(python-exporter
        -llpwriter
        -lconstraints
        -lterms
        -linitialization
        -lscabolic-core
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -llpmodel-utils
        -lboost_system
        -lboost_filesystem
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})

