cmake_minimum_required(VERSION 3.8)
project(LPModel)

find_package(DGtal REQUIRED)
find_package(OpenCV REQUIRED)

set(LINEL "-linel")

link_directories(${LPMODEL_LIBS_DIR}/constraints
        ${LPMODEL_LIBS_DIR}/initialization${LINEL}
        ${LPMODEL_LIBS_DIR}/linearization
        ${LPMODEL_LIBS_DIR}/lpwriter
        ${LPMODEL_LIBS_DIR}/terms${LINEL}
        ${LPMODEL_LIBS_DIR}/utils
        ${LPMODEL_LIBS_DIR}/solassign
        ${LPMODEL_LIBS_DIR}/python-exporter
        ${LPMODEL_LIBS_DIR}/glpk
        ${CMAKE_PREFIX_PATH}
        ${BOOST_LIBS_DIR}
        ${DIPACUS_LIBS_DIR})

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