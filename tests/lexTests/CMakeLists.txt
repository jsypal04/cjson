cmake_minimum_required(VERSION 3.28)

project(lexTests)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../bin")

add_executable(lexTest1
    lexTest1.c
)

target_link_libraries(lexTest1
    ${CMAKE_SOURCE_DIR}/../../build/libcjson.a
)
