cmake_minimum_required(VERSION 3.28)

project(mapTests)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../bin")

add_executable(mapTest1
    mapTest1.c
)

target_link_libraries(mapTest1
    ${CMAKE_SOURCE_DIR}/../../build/libcjson.a
)

add_executable(mapTest2
    mapTest2.c
)

target_link_libraries(mapTest2
    ${CMAKE_SOURCE_DIR}/../../build/libcjson.a
)
