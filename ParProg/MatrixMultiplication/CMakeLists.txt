cmake_minimum_required(VERSION 3.10)
project(Main VERSION 1.0.0)

set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_FLAGS    "-Wall -std=c++2a")

find_package(OpenMP REQUIRED)
find_package(GTest REQUIRED)
include_directories(${SFML_INCLUDE_DIR})

if(OpenMP_CXX_FOUND)
  message("Building!")
  add_executable(main main.cpp)
  target_link_libraries(main PUBLIC OpenMP::OpenMP_CXX ${GTEST_BOTH_LIBRARIES})
else()
  message("Can't found OpenMP!")
endif()