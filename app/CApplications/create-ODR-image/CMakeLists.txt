cmake_minimum_required(VERSION 3.2)
project(create-ODR-image)

add_executable(create-ODR-image create-ODR-image.cpp src/InputReader.cpp)
add_dependencies(create-ODR-image initialization${LINEL})

target_include_directories(create-ODR-image PRIVATE
        include
        ${CMAKE_SOURCE_DIR}/modules/initialization${LINEL}/include
        ${DIPACUS_INCLUDE_DIRS}
        ${SCABOLIC_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(create-ODR-image
        -linitialization${LINEL}
        -ldipacus-base
        -lscabolic-core
        -lboost_system
        -lboost_filesystem
        ${DGTAL_LIBRARIES}
        ${OPENCV_LIBS})