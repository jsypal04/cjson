#!/bin/sh

input=$1
test_name=$2

if [ $input = "-clean" ];
then
    if [ ! -d "./build" ]; then
        mkdir "./build"
    fi

    if [ ! -d "./bin" ]; then
        mkdir "./bin"
    fi

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
    echo "  ./build.sh -test -> builds the project and runs all unit tests"
    echo "  ./build.sh -test <test_executable> -> builds the project and runs the unit test specified"
    exit 0
fi


cmake --build build
cmake --build tests/build

if [ $input = "-test" ];
then
    if [ $# -eq 1 ]; then
        cd tests && ./test.sh -all && cd ..
    else
        cd tests && ./test.sh -$test_name && cd ..
    fi
fi
