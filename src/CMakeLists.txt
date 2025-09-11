cmake_minimum_required(VERSION 3.28)

project(cjson)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../bin)

set(DEBUG_MEMORY 0) # Set this to 1 to compile debug malloc and free functions.

add_library(cjson
    lexer.c
    parser.c
    map.c
    load.c
    dump.c
    debug.c
)

target_compile_definitions(cjson PRIVATE
    DEBUG_MEMORY=${DEBUG_MEMORY}
)
