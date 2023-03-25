cmake_minimum_required(VERSION 3.10)

project(Fiduccia)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_COMPILER "/usr/bin/clang++-12")
set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O3")

add_executable(${CMAKE_PROJECT_NAME} main.cpp src.cpp)