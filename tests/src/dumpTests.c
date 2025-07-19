#include "../../src/cjson.h"
#include <stdio.h>
#include <string.h>

#define KEYS_LEN 10

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

int main() {
    Map* map = initMap(10);
    Map* submap = make_submap();
    MapArray* subarray = make_subarray();
    insertInt(&map, keys[0], 123);
    insertInt(&map, keys[1], -12345678);
    insertString(&map, keys[2], "world");
    insertMap(&map, keys[3], submap);
    insertFloat(&map, keys[4], 3.14159);
    insertMapArray(&map, keys[5], subarray);
    
    char* json = dump(map);
    printf("%s\n", json);

    if (strcmp(json, correct_json) != 0) {
        destroyMap(map);
        free(json);
        return 1;
    }
    
    if (dumpf(map, "/home/sypalj/cjson/tests/jsonOutput/dumpTest1.json") != 0) {
        destroyMap(map);
        free(json);
        return 1;
    }

    destroyMap(map);
    free(json);
    return 0;
}
