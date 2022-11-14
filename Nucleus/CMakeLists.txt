cmake_minimum_required (VERSION 3.10)
project (nucleus VERSION 1.0.0)

set (CMAKE_CXX_COMPILER "/usr/bin/clang++")
set (CMAKE_CXX_FLAGS    "-Wall -std=c++2a")

find_package (GTest REQUIRED)

add_executable (${PROJECT_NAME} tests.cpp)
target_link_libraries (${PROJECT_NAME} Threads::Threads ${GTEST_BOTH_LIBRARIES})