#include "../../src/cjson.h"
#include <stdio.h>
#include <string.h>

#define KEYS_LEN 10
#define NUM_TESTS 2

char* keys[KEYS_LEN] = {"hello", "world", "how", "are", "you", "doing", "today", "right", "now", "???"};
char* correct_json = "{\"hello\":123,\"world\":-12345678,\"how\":\"world\",\"are\":{\"age\":26,\"hello\":\"world\"},\"you\":3.141590,\"doing\":[67,-67.672638,\"hello, world!\",{\"age\":26,\"hello\":\"world\"}]}";

Map* make_submap() {
    Map* submap = initMap(5);
    insertInt(&submap, "age", 26);
    insertString(&submap, "hello", "world");
    return submap;
}

MapArray* make_subarray() {
    MapArray* subarray = initMapArray(6);
    Map* submap = make_submap();
    appendInt(&subarray, 67);
    appendFloat(&subarray, -67.672635);
    appendString(&subarray, "hello, world!");
    appendMap(&subarray, submap); 
    return subarray;
}

Map* make_map() {
    Map* map = initMap(10);
    Map* submap = make_submap();
    MapArray* subarray = make_subarray();
    insertInt(&map, keys[0], 123);
    insertInt(&map, keys[1], -12345678);
    insertString(&map, keys[2], "world");
    insertMap(&map, keys[3], submap);
    insertFloat(&map, keys[4], 3.14159);
    insertMapArray(&map, keys[5], subarray);
    return map;
}

int serialization_test() {
    Map* map = make_map(); 
    char* json = dump(map);

    if (strcmp(json, correct_json) != 0) {
        destroyMap(map);
        free(json);
        return 1;
    }
    
    destroyMap(map);
    free(json);
    return 0;
}


int dump_file_test() {
    Map* map = make_map();

    if (dumpf(map, "~/cjson/tests/jsonOutput/_test.json") != 0) {
        destroyMap(map);
        return 1;
    }
    destroyMap(map);
    return 0;
}



int main() {
    char* names[NUM_TESTS] = { "serialization_test", "dump_file_test" };
    int (*tests[NUM_TESTS])() = { serialization_test, dump_file_test };
    int results[NUM_TESTS];

    for (int i = 0; i < NUM_TESTS; i++) {
        // printf("Running test %d/%d: %s...\r", i + 1, NUM_TESTS, names[i]);
        results[i] = tests[i]();
    }
    printf("\n");

    printf("UNIT TEST RESULTS:\n---\n");
    for (int i = 0; i < NUM_TESTS; i++) {
        if (results[i] != 0) {
            printf("FAILED: %s\n", names[i]);
        }
        else {
            printf("PASSED: %s\n", names[i]);
        }
    }

    return 0;
}
