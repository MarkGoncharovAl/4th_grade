cmake_minimum_required(VERSION 3.10)

project(Fiduccia)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O3")

set (BIN ${CMAKE_PROJECT_NAME})
set (BIN_MOD ${CMAKE_PROJECT_NAME}Mod)

add_executable(${BIN} main.cpp Alg.cpp)
add_executable(${BIN_MOD} mainMod.cpp AlgMod.cpp)

foreach(FILE IN LISTS BIN BIN_MOD)
add_custom_target(copy-${FILE} ALL
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_BINARY_DIR}/${FILE}
    ${CMAKE_SOURCE_DIR}/bin/${FILE}
)
endforeach(FILE IN LISTS BIN BIN_MOD)
