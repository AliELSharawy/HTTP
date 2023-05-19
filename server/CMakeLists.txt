cmake_minimum_required(VERSION 3.24)
project(server)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
include_directories(.)

add_executable(server
        functions.h
        main.cpp utility.h)

