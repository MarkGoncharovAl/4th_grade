cmake_minimum_required(VERSION 3.10)

project(Fiduccia)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
SET (CMAKE_CXX_COMPILER "/usr/bin/clang++-12")

add_executable(${CMAKE_PROJECT_NAME} main.cpp src.cpp)
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE  -O3)

find_package( Boost 1.40 COMPONENTS program_options REQUIRED )
include_directories( ${Boost_INCLUDE_DIR} )
target_link_libraries(${CMAKE_PROJECT_NAME} ${Boost_LIBRARIES})
