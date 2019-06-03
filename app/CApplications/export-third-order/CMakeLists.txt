cmake_minimum_required(VERSION 3.2)
project(export-third-order)

add_executable(export-third-order export-third-order.cpp src/InputReader.cpp)
add_dependencies(export-third-order initialization constraints terms lpmodel-utils python-exporter)

target_include_directories(export-third-order PRIVATE
        include
        ${LPMODEL_INCLUDE_DIR}/constraints/include
        ${LPMODEL_INCLUDE_DIR}/initialization/include
        ${LPMODEL_INCLUDE_DIR}/terms/include
        ${LPMODEL_INCLUDE_DIR}/utils/include
        ${LPMODEL_INCLUDE_DIR}/python-exporter/include
        ${DIPACUS_INCLUDE_DIRS}
        ${SCABOLIC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(export-third-order
        -lpython-exporter
        -linitialization
        -lconstraints
        -lterms
        -llpmodel-utils
        -ldipacus-base
        -lscabolic-core
        -lboost_system
        -lboost_filesystem
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})

