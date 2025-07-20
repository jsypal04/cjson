cmake_minimum_required(VERSION 3.28)

project(tests)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../bin")
set(CMAKE_BUILD_TYPE Debug)

add_executable(lexTest1 lexTest1.c)
target_link_libraries(lexTest1 ${CMAKE_SOURCE_DIR}/../../bin/libcjson.a)

add_executable(parserTest1 parserTest1.c)
target_link_libraries(parserTest1 ${CMAKE_SOURCE_DIR}/../../bin/libcjson.a)

add_executable(mapTest1 mapTest1.c)
target_link_libraries(mapTest1 ${CMAKE_SOURCE_DIR}/../../bin/libcjson.a)

add_executable(mapdupTest mapdupTest.c)
target_link_libraries(mapdupTest ${CMAKE_SOURCE_DIR}/../../bin/libcjson.a)

add_executable(arrTest1 arrTest1.c)
target_link_libraries(arrTest1 ${CMAKE_SOURCE_DIR}/../../bin/libcjson.a)

add_executable(loadTest1 loadTest1.c)
target_link_libraries(loadTest1 ${CMAKE_SOURCE_DIR}/../../bin/libcjson.a)

add_executable(mapTests 
    mapTests.c
    utils/map_utils.c
)
target_link_libraries(mapTests ${CMAKE_SOURCE_DIR}/../../bin/libcjson.a)

add_executable(dumpTests 
    dumpTests.c
    utils/map_utils.c
)
target_link_libraries(dumpTests ${CMAKE_SOURCE_DIR}/../../bin/libcjson.a)
