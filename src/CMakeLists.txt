cmake_minimum_required(VERSION 3.28)

project(cjson)
set(CMAKE_BUILD_TYPE Debug)

add_library(cjson
    lexer.c
    parser.c
    map.c
    load.c
)
