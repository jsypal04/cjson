MAJOR = 0
MINOR = 1
PATCH = 0

CXX = g++
CXXFLAGS = -g -Iinclude

BUILD_DIR = build

SOURCES = $(shell find src -name "*.cc" | sed "s/src\///g") 
OBJECTS = $(SOURCES:%.cc=$(BUILD_DIR)/%.o)

LIBNAME  = libcjson.so
SONAME   = $(LIBNAME).$(MAJOR)
REALNAME = $(SONAME).$(MINOR).$(PATCH)

LIB = $(BUILD_DIR)/$(REALNAME)
BIN = $(BUILD_DIR)/cjson

all: $(BIN) $(LIB)

install: $(LIB)
	cp $(LIB) /usr/local/lib/$(REALNAME)
	ln -sf $(REALNAME) /usr/local/lib/$(SONAME)
	ln -sf $(SONAME) /usr/local/lib/$(LIBNAME)
	mkdir -p /usr/local/include/cjson
	cp include/json.h /usr/local/include/cjson/json.h

clean:
	rm -rf $(BUILD_DIR)

$(LIB): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -shared -fPIC $^ -o $@
	ln -sf $(REALNAME) $(BUILD_DIR)/$(SONAME)
	ln -sf $(SONAME) $(BUILD_DIR)/$(LIBNAME)

$(BIN): $(BUILD_DIR)/main.o $(LIB)
	$(CXX) $(CXXFLAGS) -L$(BUILD_DIR) $< -lcjson -o $@

$(BUILD_DIR)/%.o: src/%.cc | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -fPIC -c $< -o $@

$(BUILD_DIR)/main.o: main.cc | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@


$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

.PHONY: all install clean
