# CJSON

---

cjson is a json parser written in c for marshalling and unmarshalling json data in c struct (maybe I'll end up marshalling it to a map, stay tuned).

As of right now the only thing implmented is a lexical analyzer for json source files which, according to my test, does not actually work.


## Use

---

Much to your dismay this project is currently unusable as I just started it a couple days ago. The idea is it will provide an
api with functions that convert json text input to a c struct (or map). Right now I am planning on simply having one header
file and one static library to add to a project.

## Supported Platforms

---

I am building this on linux and only linux with no other operating system in mind. Cry about it.


## File Structure
---

The folder structure of the project is as follows:

- **root**
  - **build** (contains cmake generated files)
    - libcjson.a
  - **src**
    - CMakeLists.txt
    - lexer.c
    - lexer.h
  - **tests**
    - **bin**
      - binary files for all unit tests
    - **jsonSrc**
      - json source files for all unit tests
    - **lexTests**
      - **build** (contains cmake generated files)
      - CMakeLists.txt
      - C source code for unit tests
    - test.sh

There are three main directories: build, src, and tests. build contains all cmake generated files for the
project. src contains the project's source code. tests contains unit tests written in C with a bash script
to run one or all of them. The tests directory also contains build directory for each unit test that contains
the unit test executable that the bash script will run.

## Building From Source

---

I'll deal with this later
