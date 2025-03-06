#include <string.h>
#include "cjson.h"


int hash();

Map* initMap() {
    Map* map = (Map*)malloc(sizeof(Map));
    map->size = 0;
    map->mapCap = INIT_MAP_CAP;
    for (int i = 0; i < INIT_MAP_CAP; i++) {
        map->pairs[i] = NULL;
    }
    return map;
}

void destroyMap(Map* map) {
    // need to iterate through each element of map->pairs and free memory allocated in each
}


int insert(Map* map, char* key, void* value) {
    if (map->size >= INIT_MAP_CAP) {
        return 1;
    }

    KeyValuePair* pair = (KeyValuePair*)malloc(sizeof(KeyValuePair));
    pair->key = strdup(key);
    pair->value = value;
    map->pairs[map->size] = pair;
    map->size++;
    
    return 0;
}
