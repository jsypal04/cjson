#include <assert.h>
#include <stdio.h>
#include "../../src/cjson.h"
#include "utils/test_utils.h"

#define NUM_TESTS 4

extern char* keys[KEYS_LEN];

// UNIT TESTS

int map_cmp_test_same_maps() {
    Map* map1 = make_map();
    Map* map2 = make_map();

    if (!map_cmp(map1, map2)) return 1;

    return 0;
}

int map_cmp_test_diff_maps() {
    Map* map1 = make_map();
    Map* map2 = initMap(5);

    insertInt(&map2, keys[0], 0);
    insertString(&map2, keys[1], "hello, world");
    insertFloat(&map2, keys[2], 3.14);

    if (map_cmp(map1, map2)) return 1;

    return 0;
} 

int mapdup_test() {
    Map* map = initMap(10);
    insertInt(&map, keys[0], 0);
    insertFloat(&map, keys[1], 3.14);
    insertString(&map, keys[2], "hello, world");
    
    Map* map2 = mapdup(map);

    if (!map_cmp(map, map2)) return 1;

    return 0;
}

int creation_and_insertion_test() {
    Map* map = initMap(10);

    if (map->size != 0 || map->mapCap != 10) {
        return 1;
    }

    insertInt(&map, keys[0], 0);
    insertFloat(&map, keys[1], 3.14);
    insertString(&map, keys[2], "hello, world");

    return 0;
}

int main() {
    char* names[NUM_TESTS] = { 
        "map_cmp_test_same_maps", 
        "map_cmp_test_diff_maps", 
        "mapdup_test", 
        "creation_and_insertion_test" 
    };
    int (*tests[NUM_TESTS])() = { 
        map_cmp_test_same_maps, 
        map_cmp_test_diff_maps, 
        mapdup_test, 
        creation_and_insertion_test 
    };
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
