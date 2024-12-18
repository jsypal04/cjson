cmake_minimum_required(VERSION 3.28)

project(lexTests)

add_executable(test1
    test1.c
)

target_link_libraries(test1
    /home/sypalj/cjson/build/libcjson.a
)
