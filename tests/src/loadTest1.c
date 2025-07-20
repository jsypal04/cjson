/*
LOAD TEST 1:

This file test the entire system on a basic json file (jsonSrc/loadTest1.json).
*/

#include "../../src/cjson.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("ERROR: To few arguments passed to loadTest1\n");
        return 1;
    }
    else if (argc > 2) {
        printf("ERROR: To many arguments passed to loadTest1\n");
        return 1;
    }

    const char* path = argv[1];

    Map* json_data = load_file(path);

    char type;

    char* val1 = get(json_data, "hello", &type);
    assert(type == STRING);
    assert(strcmp(val1, "world") == 0);

    MapArray* val2 = get(json_data, "main", &type);
    assert(type == ARRAY);
    assert(val2->size == 2);

    for (int i = 0; i < val2->size; i++) {
        Map* val3 = read_arr(val2, i, &type);
        assert(type == MAP);
        assert(val3->size == 2);

        if (i == 0) {
            int* val4 = get(val3, "id", &type);
            assert(type == INT);
            assert(*val4 == 0);

            char* val5 = get(val3, "inStock", &type);
            assert(type == STRING);
            assert(strcmp(val5, "true") == 0);
        }
        else if (i == 1) {
            float* val4 = get(val3, "id", &type);
            assert(type == FLOAT);

            char* val5 = get(val3, "inStock", &type);
            assert(type == STRING);
            assert(strcmp(val5, "false") == 0);
        }
    }

    char* val6 = get(json_data, "mainVendor", &type);
    assert(type == STRING);
    assert(strcmp(val6, "null") == 0);

    Map* val7 = get(json_data, "CEO", &type);
    assert(type == MAP);
    assert(val7->size == 3);

    char* val8 = get(val7, "firstName", &type);
    assert(type == STRING);
    assert(strcmp(val8, "John") == 0);

    char* val9 = get(val7, "lastName", &type);
    assert(type == STRING);
    assert(strcmp(val9, "Doe") == 0);

    float* val10 = get(val7, "age", &type);
    assert(type == FLOAT);

    destroyMap(json_data);
    printf("Load test 1 passed.\n");

    return 0;
}
