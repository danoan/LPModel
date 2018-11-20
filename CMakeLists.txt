cmake_minimum_required(VERSION 3.8)
project(LPModel)

find_package(DGtal REQUIRED)
find_package(OpenCV REQUIRED)

link_directories(cmake-build-debug/modules/initialization
        cmake-build-debug/modules/terms
        cmake-build-debug/modules/constraints
        cmake-build-debug/modules/lpwriter
        ${DIPACUS_LIBS_DIR})


add_executable(LPModel main.cpp)


add_subdirectory(modules/utils)
add_subdirectory(modules/initialization)
add_subdirectory(modules/terms)
add_subdirectory(modules/constraints)
add_subdirectory(modules/lpwriter)

add_dependencies(LPModel utils initialization terms constraints lpwriter)

target_link_libraries(LPModel
        -llpwriter
        -lconstraints
        -lterms
        -linitialization
        -lscabolic-core
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})

target_include_directories(LPModel PRIVATE
        ext/include
        modules/initialization/include
        modules/terms/include
        modules/utils/include
        modules/constraints/include
        modules/lpwriter/include
        ${DGTAL_INCLUDE_DIRS})

