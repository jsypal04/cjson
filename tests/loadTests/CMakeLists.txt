cmake_minimum_required(VERSION 3.28)

project(loadTests)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../bin")

add_executable(loadTest1
    loadTest1.c
)

target_link_libraries(loadTest1
    ${CMAKE_SOURCE_DIR}/../../build/libcjson.a
)
