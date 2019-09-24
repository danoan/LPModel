cmake_minimum_required(VERSION 3.8)
project(LPModel)

set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install" CACHE PATH "SCaBOliC binaries and libraries install location." )
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib:${BOOST_LIBS_DIR}:${DIPACUS_LIBS_DIR}")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

set(EXTPROJECTS_SOURCE_DIR ${CMAKE_SOURCE_DIR}/ext-projects/source)
if (${CMAKE_BUILD_TYPE} STREQUAL Release)
    set(EXTPROJECTS_BUILD_DIR ${CMAKE_SOURCE_DIR}/ext-projects/cmake-build-release)
else()
    set(EXTPROJECTS_BUILD_DIR ${CMAKE_SOURCE_DIR}/ext-projects/cmake-build-debug)
endif ()

set(BUILD_TESTS OFF CACHE BOOL "Build tests.")
set(CREATE_SHARED_LIBRARIES OFF CACHE BOOL "Create shared libraries. If off, create static libraries.")
set(USE_REMOTE_REPOSITORIES ON CACHE BOOL "Pull external repositories from GitHub.")

set(DIPACUS_INCLUDE_DIRS ${EXTPROJECTS_BUILD_DIR}/include CACHE PATH "DIPaCUS header directories")
set(SCABOLIC_INCLUDE_DIRS ${EXTPROJECTS_BUILD_DIR}/include CACHE PATH "SCaBOliC header directories")
set(GEOC_INCLUDE_DIRS ${EXTPROJECTS_BUILD_DIR}/include CACHE PATH "GEOC header directories")

set(DIPACUS_LIBS_DIR ${EXTPROJECTS_BUILD_DIR}/lib CACHE PATH "DIPaCUS libs directory")
set(SCABOLIC_LIBS_DIR ${EXTPROJECTS_BUILD_DIR}/lib CACHE PATH "SCaBoliC libs directory")

find_package(DGtal REQUIRED)
find_package(OpenCV REQUIRED)

set(LINEL "-linel")

link_directories(${CMAKE_BINARY_DIR}/modules/constraints
        ${CMAKE_BINARY_DIR}/modules/initialization${LINEL}
        ${CMAKE_BINARY_DIR}/modules/linearization
        ${CMAKE_BINARY_DIR}/modules/lpwriter
        ${CMAKE_BINARY_DIR}/modules/terms${LINEL}
        ${CMAKE_BINARY_DIR}/modules/utils
        ${CMAKE_BINARY_DIR}/modules/solassign
        ${CMAKE_BINARY_DIR}/modules/python-exporter
        ${CMAKE_BINARY_DIR}/modules/glpk
        ${CMAKE_PREFIX_PATH}
        ${BOOST_LIBS_DIR}
        ${DIPACUS_LIBS_DIR})


if(${USE_REMOTE_REPOSITORIES})
    include(ext-projects/ExtProjects.cmake)
endif()

add_subdirectory(modules/constraints)
add_subdirectory(modules/initialization${LINEL})
add_subdirectory(modules/linearization)
add_subdirectory(modules/lpwriter)
add_subdirectory(modules/solassign)
add_subdirectory(modules/terms${LINEL})
add_subdirectory(modules/utils)
add_subdirectory(modules/python-exporter)
add_subdirectory(modules/glpk)

add_subdirectory(lab/test)


add_subdirectory(app/CApplications/export-grid)
add_subdirectory(app/CApplications/create-ODR-image)

add_subdirectory(app/CApplications/export-cplex-lp)
add_subdirectory(app/CApplications/export-cplex-qp)
add_subdirectory(app/CApplications/export-cvxopt)
add_subdirectory(app/CApplications/export-third-order)

add_subdirectory(app/CApplications/generate-shapes)
add_subdirectory(app/CApplications/generate-solution)
add_subdirectory(app/CApplications/nonlinopt)