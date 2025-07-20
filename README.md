# CJSON

cjson is a json parser written in C for serializing and deserializing json data into a custom ordered map.


## Use

Much to your dismay this project is currently unusable. The idea is it will provide an api with functions that convert json text input to a C ordered map.
Right now I am planning on simply having one header file and one static library to add to a project.

## Roadmap

This is basically a list of things that I want to add.

- Convert C objects to json text
- Build out unit tests
- Do stress testing

## Supported Platforms

I am building this on linux but, out of the goodness of my heart, am considering also making is compatable with Windows.


## File Structure

The file structure of the project is as follows:

- **root**
  - **src**
    - C source files for the project
  - **tests**
    - **bin**
      - binary files for all unit tests (probably will gitignore these at some point)
    - **jsonSrc**
      - json source files for all unit tests
    - **lexTests**
      - C source code for lexer unit tests
    - **parserTests**
      - C source code for parser unit tests
    - **mapTests**
      - C source code for map unit tests
    - test.sh (script to run unit tests)
  - build.sh (script to build the entire project including unit tests) (I never use this).

There are two main directories: src, and tests. src contains the project's source code. tests contains unit tests written in C with a bash script
to run one or all of them. The tests directory also contains a bin directory for the unit test executables that the bash script will run.

## Building From Source

1. Clone the repo
2. Make a `build` directory
3. cd into the repo and run `./build.sh -clean`

To use this, you need two files: `cjson.h` (in the src directory) and (on linux) `libcjson.a` (in the main build directory). Include the
header file in your project and link the archive.
