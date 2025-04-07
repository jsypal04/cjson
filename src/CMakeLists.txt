cmake_minimum_required(VERSION 3.28)

project(cjson)

add_library(cjson
    lexer.c
    parser.c
    map.c
    load.c
)
