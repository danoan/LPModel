cmake_minimum_required(VERSION 3.2)
project(export-third-order)

add_executable(export-third-order export-third-order.cpp src/InputReader.cpp)
add_dependencies(export-third-order initialization${LINEL} constraints terms${LINEL} lpmodel-utils python-exporter)

target_include_directories(export-third-order PRIVATE
        include
        ${CMAKE_SOURCE_DIR}/modules/constraints/include
        ${CMAKE_SOURCE_DIR}/modules/initialization${LINEL}/include
        ${CMAKE_SOURCE_DIR}/modules/terms/include
        ${CMAKE_SOURCE_DIR}/modules/utils/include
        ${CMAKE_SOURCE_DIR}/modules/python-exporter/include
        ${DIPACUS_INCLUDE_DIRS}
        ${SCABOLIC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(export-third-order
        -lpython-exporter
        -linitialization${LINEL}
        -lconstraints
        -lterms${LINEL}
        -llpmodel-utils
        -ldipacus-base
        -lscabolic-core
        -lboost_system
        -lboost_filesystem
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})

