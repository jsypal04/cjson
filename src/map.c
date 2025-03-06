#include <string.h>
#include "cjson.h"

// Hashing function for the map
unsigned long hash(char* key, int mapCap) {
    unsigned long hashcode = 11;

    int i = 0;
    while (key[i] != '\0') {
        unsigned int intrep = (unsigned int)key[i];
        hashcode = (hashcode * 139 + intrep) % mapCap;
        i++;
    }
    return hashcode;
}


Map* initMap(int initMapCap) {
    Map* map = (Map*)malloc(sizeof(Map));
    map->size = 0;
    map->mapCap = initMapCap;
    for (int i = 0; i < initMapCap; i++) {
        map->pairs[i] = NULL;
    }
    return map;
}

void destroyMap(Map* map) {
    // need to iterate through each element of map->pairs and free memory allocated in each
    for (int i = 0; i < map->size; i++) {
        free(map->pairs[i]);
        map->pairs[i] = NULL;
    }
    free(map);
    map = NULL;
}


void insert(Map* map, char* key, void* value) {
    if (map->size >= map->mapCap / 2) {
        // allocate more memory
        // copy existing map to new map
        // destroy 
        return;
    }

    KeyValuePair* pair = (KeyValuePair*)malloc(sizeof(KeyValuePair));
    unsigned long hashcode = hash(key, map->mapCap);
    pair->key = strdup(key);
    pair->value = value;
    map->pairs[hashcode] = pair;
    map->size++;
}


void* get(Map* map, char* key) {
    unsigned long hashcode = hash(key, map->mapCap);
    void* value = map->pairs[hashcode]->value;
    return value;
}
