#!/bin/sh

input=$1

if [ $input = "-clean" ];
then
    cmake -S src -B build
    cmake -S tests/src -B tests/build
fi

if [ $input = "-help" ];
then
    echo "This is a script to build the entire project"
    echo "options:"
    echo "  ./build.sh -> the project will be build from the cmake cache's"
    echo "  ./build.sh -clean -> new cmake files will be generated then the project will build"
    echo "  ./build.sh -help -> displays this help menu"
    exit 0
fi


cmake --build build
cmake --build tests/build
