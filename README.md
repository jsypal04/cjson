# CJSON

cjson is json parser for serializing and deserializing json strings. I originally
implemented it in C but it was not working so I decided to rewrite it in C++.

## Building from Source

To build the project from source:

```
git clone https://github.com/jsypal04/cjson.git
make
```
This compiles a shared library to the build directory. The main.cc file is to
provide an entrypoint to the library for testing. It compiles to an executable
called `cjson`. Make sure `cjson` loads the library from the build directory, you
have to add it to your library path:

```sh
export LD_LIBRARY_PATH=$PWD/build
```
or in fish:

```fish
set -x LD_LIBRARY_PATH $PWD/build

```

## Installation

To install the library and associated header file `json.h` system wide, run the
following command:

```
sudo make install
```
