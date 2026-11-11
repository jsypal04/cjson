MAJOR = 0
MINOR = 1
PATCH = 0

CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude -Itestify

BUILD_DIR = build

SOURCES = $(shell find src -name "*.c" | sed "s/src\///g")
OBJECTS = $(SOURCES:%.c=$(BUILD_DIR)/src/%.o)

LIBNAME  = libcjson.so
SONAME   = $(LIBNAME).$(MAJOR)
REALNAME = $(SONAME).$(MINOR).$(PATCH)

LIB = $(BUILD_DIR)/$(REALNAME)
BIN = $(BUILD_DIR)/cjson
TEST_BIN = $(BUILD_DIR)/cjson-tests

TESTIFY = $(BUILD_DIR)/libtestify.so

all: $(LIB) $(BIN) $(TEST_BIN)

test: $(LIB) $(TEST_BIN)
	LD_LIBRARY_PATH=$(PWD)/$(BUILD_DIR) \
	./$(TEST_BIN)

install: $(LIB)
	cp $(LIB) /usr/local/lib/$(REALNAME)
	ln -sf $(REALNAME) /usr/local/lib/$(SONAME)
	ln -sf $(SONAME) /usr/local/lib/$(LIBNAME)
	mkdir -p /usr/local/include/cjson
	cp include/cjson.h /usr/local/include/cjson/cjson.h

clean:
	rm -rf $(BUILD_DIR)/*

$(LIB): $(OBJECTS)
	$(CC) $(CFLAGS) -shared -fPIC $^ -o $@
	ln -sf $(REALNAME) $(BUILD_DIR)/$(SONAME)
	ln -sf $(SONAME) $(BUILD_DIR)/$(LIBNAME)

$(BIN): main.c $(LIB)
	$(CC) $(CFLAGS) -L$(BUILD_DIR) $< -lcjson -o $@

$(TEST_BIN): tests/main.c $(TESTIFY) $(LIB)
	$(CC) $(CFLAGS) -L$(BUILD_DIR) $< -lcjson -ltestify -o $@

$(TESTIFY):
	make -C testify clean
	make -C testify
	ln -sf ../testify/build/libtestify.so $(TESTIFY)


$(BUILD_DIR)/src/%.o: src/%.c | $(BUILD_DIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

.PHONY: all, install, clean, test
