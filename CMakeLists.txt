cmake_minimum_required(VERSION 3.8)
project(LPModel)

find_package(DGtal REQUIRED)
find_package(OpenCV REQUIRED)

add_executable(LPModel main.cpp)

target_link_libraries(LPModel
        ${DGTAL_LIBRARIES}
        ${OpenCV_LIBS})

target_include_directories(LPModel PRIVATE
        ${DGTAL_INCLUDE_DIRS})