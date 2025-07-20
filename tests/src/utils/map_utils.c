#include "test_utils.h"

char* keys[KEYS_LEN] = {"hello", "world", "how", "are", "you", "doing", "today", "right", "now", "???"};

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
