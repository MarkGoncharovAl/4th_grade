cmake_minimum_required(VERSION 3.10)

project(Fiduccia)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O3")

set (BIN ${CMAKE_PROJECT_NAME})
set (MODS "Ord" "Mod" "Final")

foreach(X ${MODS})
    add_executable(${BIN}${X} main${X}.cpp Alg${X}.cpp)
endforeach()

foreach(X ${MODS})
add_custom_target(copy-${X} ALL
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_BINARY_DIR}/${BIN}${X}
    ${CMAKE_SOURCE_DIR}/bin/${BIN}${X}
)
endforeach()
