#include "../../src/cjson.h"
#include <stdio.h>

#define KEYS_LEN 10

char* keys[KEYS_LEN] = {"hello", "world", "how", "are", "you", "doing", "today", "right", "now", "???"};

int main() {
    Map* map = initMap(10);
    insertInt(&map, keys[0], 123);
    insertInt(&map, keys[1], -12345678);
    for (int i = 2; i < KEYS_LEN; i++) {
        insertFloat(&map, keys[i], 3.14159);
    }
    printMap(map);

    char* json = dump(map);
    printf("%s\n", json);

    destroyMap(map);
}
