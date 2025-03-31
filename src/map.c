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
    KeyValuePair* keyValuePairArr = malloc(sizeof(KeyValuePair) * initMapCap);
    map->pairs = keyValuePairArr;
    map->size = 0;
    map->mapCap = initMapCap;
    return map;
}

void destroyMap(Map* map) {
    // need to iterate through each element of map->pairs and free memory allocated in each
    free(map->pairs);
    map->pairs = NULL;

    free(map);
    map = NULL;
}


void insert(Map* map, char* key, void* value, char type) {
    if (map->size >= map->mapCap / 2) {
        // TODO: Implement capacity expansion
        //     allocate more memory
        //     copy existing map to new map
        //     destroy destroy old map 
        return;
    }

    KeyValuePair pair;
    unsigned long hashcode = hash(key, map->mapCap);
    pair.key = strdup(key);
    pair.value = value;
    pair.type = type;
    map->pairs[hashcode] = pair;
    map->size++;
    
    printf("Value inserted, hashcode of key: %lu\n", hashcode);
}


void* get(Map* map, char* key, char* type) {
    unsigned long hashcode = hash(key, map->mapCap);
    
    *type = map->pairs[hashcode].type;
    void* value = map->pairs[hashcode].value;
    return value;
}
