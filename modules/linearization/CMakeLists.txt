cmake_minimum_required(VERSION 3.8)
project(lpmodel-linearization)

add_library(linearization INTERFACE)

target_include_directories(linearization INTERFACE
        include)