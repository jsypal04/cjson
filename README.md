# CJSON

cjson is a json parser written in C for marshalling and unmarshalling json data into a custom unordered map.


## Use

Much to your dismay this project is currently unusable. The idea is it will provide an api with functions that convert json text input to a C unordered map. 
Right now I am planning on simply having one header file and one static library to add to a project.

## Supported Platforms

I am building this on linux but, out of the goodness of my heart, am considering also making is compatable with Windows.


## File Structure

The folder structure of the project is as follows:

- **root**
  - **src**
    - C source files for the project
  - **tests**
    - **bin**
      - binary files for all unit tests
    - **jsonSrc**
      - json source files for all unit tests
    - **lexTests**
      - C source code for lexer unit tests
    - **parserTests**
      - C source code for parser unit tests
    - test.sh (script to run unit tests)
  - build.sh (script to build the entire project including unit tests)

There are two main directories: src, and tests. src contains the project's source code. tests contains unit tests written in C with a bash script
to run one or all of them. The tests directory also contains a bin directory for the unit test executables that the bash script will run.

## Building From Source

I'll deal with this later
