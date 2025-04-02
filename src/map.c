/*
 * NOTE to self: It is very possible that when the map capacity needs to be expanded, the map variable passed 
 * by the caller of `insert` might lose access to the map when I destroy it. I do re-assign it to `biggerMap`
 * but there might be some weird stuff going on with stack and heap memory that i'm not aware of.
 * */


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

    for (int i = 0; i < map->mapCap; i++) {
        map->pairs[i].key = NULL;
        map->pairs[i].value = NULL;
    }

    return map;
}

void destroyMap(Map* map) {
    // need to iterate through each element of map->pairs and free memory allocated in each
    for (int i = 0; i < map->mapCap; i++) {
        if (map->pairs[i].value != NULL) {
            free(map->pairs[i].value);
            map->pairs[i].value = NULL;
        }
    }

    free(map->pairs);
    map->pairs = NULL;

    free(map);
    map = NULL;
}

void insert(Map* map, char* key, void* value, char type) {
    /* if (map->size >= map->mapCap / 2) {
        // TODO: Implement capacity expansion
        //     allocate more memory
        //     copy existing map to new map
        //     destroy destroy old map 

        printf("Expanding the map.\n");
        Map* biggerMap = initMap(map->mapCap * 2);
        for (int i = 0; i < map->mapCap; i++) {
            if (map->pairs[i].value == NULL) {
                continue;
            }

            KeyValuePair pair;
            unsigned long hashcode = hash(map->pairs[i].key, biggerMap->mapCap);
            pair.key = map->pairs[i].key;
            pair.value = map->pairs[i].value;
            biggerMap->pairs[hashcode] = pair;
            biggerMap->size++;
        }

        destroyMap(map);
        map = biggerMap;
    } */

    KeyValuePair pair;
    unsigned long hashcode = hash(key, map->mapCap);

    if (map->pairs[hashcode].value != NULL) {
        pair.key = strdup(key);
        pair.value = value;
        pair.type = type;
        map->pairs[hashcode] = pair;
    }
    else {
        pair.key = strdup(key);
        pair.value = value;
        pair.type = type;
        map->pairs[hashcode] = pair;
        map->size++;
    }
    
    printf("Value inserted, hashcode of key: %lu\n", hashcode);
}

void insertInt(Map* map, char* key, int value) {
    int* val = malloc(sizeof(int));
    *val = value;
    insert(map, key, val, INT);
}

void insertFloat(Map* map, char* key, float value) {
    float* val = malloc(sizeof(float));
    *val = value;
    insert(map, key, val, FLOAT);
}

void insertString(Map* map, char* key, char* value) {
    int length = strlen(value);
    char* val = strdup(value);
    insert(map, key, val, STRING);
}

void* get(Map* map, char* key, char* type) {
    unsigned long hashcode = hash(key, map->mapCap);
    
    *type = map->pairs[hashcode].type;
    void* value = map->pairs[hashcode].value;
    return value;
}

void printMap(Map* map) {
    printf("------------------------\n");
    for (int i = 0; i < map->mapCap; i++) {
        char type = map->pairs[i].type;
        switch (type) {
            case STRING: {
                printf("%s: %s\n", map->pairs[i].key, (char*)map->pairs[i].value);
                break;
            }
            case INT: {
                printf("%s: %d\n", map->pairs[i].key, *(int*)map->pairs[i].value);
                break;
            }
            case FLOAT: { 
                printf("%s: %f\n", map->pairs[i].key, *(float*)map->pairs[i].value);
                break;
            }
            default: {
                printf("Empty map slot.\n");
            }
        }
    }
    printf("------------------------\n");
}
