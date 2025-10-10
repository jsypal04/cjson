#include "../../src/cjson.h"
#include "utils/test_utils.h"
#include <stdio.h>

#define JSON_SRC "~/cjson/tests/jsonSrc/loadTest1.json"

extern void print_malloc_arr();

void create_and_destroy_map() {
    Map* map = make_map();
    destroyMap(map);

    // print_malloc_arr();

    /*int iterations = 0;

    while (true) {
        Map* map = make_map();
        destroyMap(map);
    }*/
}

void end_to_end() {
    while (true) {
        Map* map = make_map();
        char* data = dump(map);
        Map* map2 = load(data);

        free(data);
        destroyMap(map);
        destroyMap(map2);
    }
}

void dumping() {
    while (true) {
        Map* map = make_map();
        const char* data = dump(map);
        destroyMap(map);
    }
}

void loading() {
    while (true) {
        Map* map = load_file(JSON_SRC);
        destroyMap(map);
    }
}

char input;

int main() {
    printf("\nOpen the system monitor to track memory usage (Press enter when ready)...");
    scanf("%c", &input);
    printf("\n\n");

    printf("Which memory leak test would you like to run?\n\n");
    printf("1) end_to_end (tests maps, loading, and dumping)\n");
    printf("2) create_and_destroy_map (only tests map/array data structures)\n");
    printf("3) dumping\n");
    printf("4) loading\n\n");
    printf("(1/2/3/4)>");

    scanf("%c", &input);

    switch (input) {
        case '1': {
            end_to_end();
            break;
        }
        case '2': {
            create_and_destroy_map();
            break;
        }
        case '3': {
            dumping();
            break;
        }
        case '4': {
            loading();
            break;
        }
        default: printf("Bad selection.\n");
    }

    return 0;
}
