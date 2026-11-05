# CJSON

cjson is a serialization/deserialization library for json strings written in C.

## Building from Source

To build the project from source:

```sh
git clone https://github.com/jsypal04/cjson.git
make
```
This compiles a shared library to the build directory. The main.c file is to
provide an entrypoint to the library for testing. It compiles to an executable
called `cjson`. Make sure `cjson` loads the library from the build directory, you
have to add it to your library path:

```sh
export LD_LIBRARY_PATH=$PWD/build
```
or in fish (what I use):

```fish
set -x LD_LIBRARY_PATH $PWD/build
```

## Installation

To install the library and associated header file `cjson.h` system wide, run the
following command:

```
sudo make install
```

## Roadmap

- [ ] Switch the C standard to C89
- [ ] Write tests (this is dependent on pulling the testing framework out of calenter into a more portable library)
