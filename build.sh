#!/bin/sh

input=$1
test_name=$2

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
    echo "  ./build.sh -test <test_executable> -> builds the project and runs the unit test specified"
    exit 0
fi


cmake --build build
cmake --build tests/build

if [ $input = "-test" ];
then
    cd tests
    ./test.sh -$test_name
    cd ..
fi
