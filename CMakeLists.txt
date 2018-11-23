cmake_minimum_required(VERSION 3.8)
project(LPModel)

find_package(DGtal REQUIRED)
find_package(OpenCV REQUIRED)

link_directories(cmake-build-debug/modules/constraints
        cmake-build-debug/modules/initialization
        cmake-build-debug/modules/linearization
        cmake-build-debug/modules/lpwriter
        cmake-build-debug/modules/objective
        cmake-build-debug/modules/solassign
        cmake-build-debug/modules/terms
        ${CMAKE_PREFIX_PATH}
        ${DIPACUS_LIBS_DIR})

add_subdirectory(modules/constraints)
add_subdirectory(modules/initialization)
add_subdirectory(modules/linearization)
add_subdirectory(modules/lpwriter)
add_subdirectory(modules/objective)
add_subdirectory(modules/solassign)
add_subdirectory(modules/terms)
add_subdirectory(modules/utils)

add_subdirectory(lab)


add_executable(export-lp export-lp.cpp)
add_dependencies(export-lp constraints initialization linearization lpwriter objective solassign terms utils)

target_link_libraries(export-lp
        -lobjective
        -llpwriter
        -lconstraints
        -lterms
        -linitialization
        -lscabolic-core
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lboost_system
        -lboost_filesystem
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})

target_include_directories(export-lp PRIVATE
        ext/include
        modules/constraints/include
        modules/initialization/include
        modules/linearization/include
        modules/lpwriter/include
        modules/objective/include
        modules/terms/include
        modules/utils/include
        ${DGTAL_INCLUDE_DIRS})



add_executable(generate-solution generate-solution.cpp)
add_dependencies(generate-solution initialization solassign utils)

target_link_libraries(generate-solution
        -lsolassign
        -linitialization
        -ldipacus-base
        ${DGTAL_LIBRARIES})

target_include_directories(generate-solution PRIVATE
        ext/include
        modules/solassign/include
        modules/initialization/include
        modules/utils/include
        ${DGTAL_INCLUDE_DIRS})


add_executable(generate-initial-shapes generate-initial-shapes.cpp )
add_dependencies(generate-initial-shapes initialization)

target_link_libraries(generate-initial-shapes
        -linitialization
        -ldipacus-base
        ${DGTAL_LIBRARIES})

target_include_directories(generate-initial-shapes PRIVATE
        ext/include
        modules/initialization/include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})
