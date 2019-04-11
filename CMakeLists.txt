cmake_minimum_required(VERSION 3.8)
project(LPModel)

find_package(DGtal REQUIRED)
find_package(OpenCV REQUIRED)

link_directories(${LPMODEL_LIBS_DIR}/constraints
        ${LPMODEL_LIBS_DIR}/initialization
        ${LPMODEL_LIBS_DIR}/linearization
        ${LPMODEL_LIBS_DIR}/lpwriter
        ${LPMODEL_LIBS_DIR}/terms
        ${LPMODEL_LIBS_DIR}/utils
        ${LPMODEL_LIBS_DIR}/solassign
        ${CMAKE_PREFIX_PATH}
        ${BOOST_LIBS_DIR}
        ${DIPACUS_LIBS_DIR})

add_subdirectory(modules/constraints)
add_subdirectory(modules/initialization)
add_subdirectory(modules/linearization)
add_subdirectory(modules/lpwriter)
add_subdirectory(modules/solassign)
add_subdirectory(modules/terms)
add_subdirectory(modules/utils)

add_subdirectory(lab/test)
add_subdirectory(lab/exp)


add_subdirectory(CApplications/export-grid)
add_subdirectory(CApplications/create-ODR-image)

add_subdirectory(CApplications/export-cplex-lp)
add_subdirectory(CApplications/export-cplex-qp)
add_subdirectory(CApplications/export-cvxopt)
add_subdirectory(CApplications/generate-shapes)
add_subdirectory(CApplications/generate-solution)
add_subdirectory(CApplications/nonlinopt)