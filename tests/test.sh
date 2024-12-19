#!/bin/sh

input=$1
bin="bin"

if [ $input = "-help" ];
then
    echo "This script file runs unit tests."
    echo "option:"
    echo "  test.sh -all -> runs all unit tests"
    echo "  test.sh -<executable name> -> runs the given unit test"
    echo "  test.sh -help -> displays this help menu"
    exit 0
fi

if [ $input = "-all" ];
then
    for test in "$bin"/*
    do
        string="hello world"
        index=$(expr index "$test" "/")
        len=${#test}
        jsonFilename=$(expr substr "$test" $(($index + 1)) $len)
        echo "Running unit test: $test"
        $test "jsonSrc/$jsonFilename.json"
    done
    exit 0
else
    len=${#input}
    test=$(expr substr "$input" 2 $len)
    if [ -f "bin/$test" ];
    then
        echo "Running unit test: bin/$test"
        ./bin/$test "jsonSrc/$test.json"
    else
        echo "ERROR: Unit test bin/$test does not exist (type ./test.sh -help for the help menu)"
    fi
fi
