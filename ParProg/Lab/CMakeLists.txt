# Enable power
# sudo cpupower frequency-set --governor performance
# Disable
# sudo cpupower frequency-set --governor powersave

cmake_minimum_required (VERSION 3.10)
project (Main VERSION 1.0.0)

set (CMAKE_CXX_COMPILER "/usr/bin/g++")
set (CMAKE_CXX_FLAGS    "-Wall -std=c++2a")

find_package (OpenMP REQUIRED)
find_package(Threads REQUIRED)
find_package (GTest REQUIRED)
find_package (benchmark REQUIRED)
find_package(MPI REQUIRED)

include_directories(SYSTEM ${MPI_INCLUDE_PATH})

message ("Building!")
add_executable (openmp main.cpp)
target_link_libraries (openmp PUBLIC Threads::Threads OpenMP::OpenMP_CXX "${GTEST_BOTH_LIBRARIES}" benchmark::benchmark)