cmake_minimum_required(VERSION 3.28)
project(parserTest1)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../bin")

add_executable(parserTest1
    parserTest1.c
)

target_link_libraries(parserTest1
    ${CMAKE_SOURCE_DIR}/../../build/libcjson.a
)
