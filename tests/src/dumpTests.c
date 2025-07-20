#include "../../src/cjson.h"
#include "utils/test_utils.h"
#include <stdio.h>
#include <string.h>

#define NUM_TESTS 2

char* correct_json = "{\"hello\":123,\"world\":-12345678,\"how\":\"world\",\"are\":{\"age\":26,\"hello\":\"world\"},\"you\":3.141590,\"doing\":[67,-67.672638,\"hello, world!\",{\"age\":26,\"hello\":\"world\"}]}";

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
