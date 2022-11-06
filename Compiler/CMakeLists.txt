cmake_minimum_required(VERSION 3.10)
project(mcl VERSION 1.0.0)

set(CMAKE_C_COMPILER   "/usr/bin/clang")
set(CMAKE_C_FLAGS      "-Wall -std=c99")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_FLAGS    "-Wall -std=c++2a")

if (NOT DEFINED CMAKE_BUILD_DIRECTORY)
  set (CMAKE_BUILD_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif()

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)
find_package(LLVM REQUIRED CONFIG)
find_package(SFML 2 COMPONENTS network graphics audio REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${SFML_INCLUDE_DIR})

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
message(STATUS "Build directory: ${CMAKE_BUILD_DIRECTORY}")
set(COMPILE_NAME ${PROJECT_NAME})

message("Building flex+bison!")
flex_target(scanner
  Language/scanner.l
  ${CMAKE_BUILD_DIRECTORY}/lexer.cpp
)
bison_target(parser
  Language/compiler.y
  ${CMAKE_BUILD_DIRECTORY}/parser.cpp
  COMPILE_FLAGS "--defines=${CMAKE_BUILD_DIRECTORY}/compiler.cpp.h"
)
add_flex_bison_dependency(scanner parser)

message("Building compiler!")
add_executable(${COMPILE_NAME} 
  Language/driver.cc
  Language/Node.cc
  ${BISON_parser_OUTPUTS}
  ${FLEX_scanner_OUTPUTS}
)
llvm_map_components_to_libnames(llvm_libs support core codegen irreader)
target_compile_features(${COMPILE_NAME} PRIVATE cxx_std_20)
target_include_directories(${COMPILE_NAME} PRIVATE ${CMAKE_BUILD_DIRECTORY})
target_include_directories(${COMPILE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${COMPILE_NAME} PRIVATE ${LLVM_INCLUDE_DIRS})
target_link_libraries(${COMPILE_NAME} ${llvm_libs} Boost::program_options)


message("Building SFML project")
add_executable(main main.c graph.cpp math/math.c)
target_link_libraries(
    main 
    sfml-graphics 
    sfml-network
    sfml-audio 
  )