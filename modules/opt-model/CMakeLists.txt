cmake_minimum_required(VERSION 3.8)
project(lpmodel-opt-model)

add_library(opt-model SHARED
        src/control.cpp
        src/utils.cpp)

if(${USE_REMOTE_REPOSITORIES})
    add_dependencies(opt-model dipacus)
else()

endif()

set_target_properties(opt-model PROPERTIES COMPILE_FLAGS -fPIC)


target_include_directories(opt-model PRIVATE
        include
        ${DIPACUS_INCLUDE_DIRS}
        ${DGTAL_INCLUDE_DIRS})

target_link_libraries(opt-model
        -ldipacus-derivates
        -ldipacus-components
        -ldipacus-base
        -lboost_filesystem
        -lboost_system
        ${DGTAL_LIBRARIES})