cmake_minimum_required(VERSION 3.10)

project(Fiduccia)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O3")

set (BIN ${CMAKE_PROJECT_NAME})
set (MODS "Ord" "Mod" "Final")

add_executable(${BIN} main.cpp AlgOrd.cpp AlgFinal.cpp)

add_custom_target(copy-${BIN} ALL
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_BINARY_DIR}/${BIN}
    ${CMAKE_SOURCE_DIR}/bin/${BIN}
)
