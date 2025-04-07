/*
 * NOTE to self: It is very possible that when the map capacity needs to be expanded, the map variable passed 
 * by the caller of `insert` might lose access to the map when I destroy it. I do re-assign it to `biggerMap`
 * but there might be some weird stuff going on with stack and heap memory that i'm not aware of.
 * */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "cjson.h"


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


// TODO: Implement way to destroy nested maps. This function will probably result in memory leaks right now.
void destroyMap(Map* map) {
    // need to iterate through each element of map->pairs and free memory allocated in each
    for (int i = 0; i < map->mapCap; i++) {
        if (map->pairs[i].key != NULL) {
            free(map->pairs[i].key);
            map->pairs[i].key = NULL;
        }
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

/*
 * Duplicates `source` and returns a pointer to the new map. 
 * - `source` is not destroyed, the caller still needs to destroy it.
 * - NOTE: If a nested map is encountered, need to use recursion.
 * */
Map* mapdup(Map* source) {
    Map* copy = initMap(source->mapCap);

    for (int i = 0; i < source->size; i++) {
        KeyValuePair pair;
        pair.key = strdup(source->pairs[i].key);
        pair.type = source->pairs[i].type;

        switch (source->pairs[i].type) {
            case STRING: {
                pair.value = strdup((char*)source->pairs[i].value);
                break;
            }
            case INT: {
                int* val_cpy = malloc(sizeof(int));
                *val_cpy = *(int*)source->pairs[i].value;
                pair.value = val_cpy;
                break;
            }
            case FLOAT: {
                float* val_cpy = malloc(sizeof(float));
                *val_cpy = *(float*)source->pairs[i].value;
                pair.value = val_cpy;
                break;
            }
            case MAP: {
                pair.value = mapdup((Map*)source->pairs[i].value);
                break;
            }
        }
        copy->pairs[i] = pair;
        copy->size++;
    }
    return copy;
}

Map* insert(Map* map, char* key, void* value, char type) {
    assert(map->size <= map->mapCap);

    if (map->size == map->mapCap) {
        // expand the map:

        Map* bigger_map = mapdup(map);

        // 3. destroy old map
        destroyMap(map);
        map = NULL;

        // Add new value

        KeyValuePair pair;
        pair.key = strdup(key);
        pair.type = type;

        switch (type) {
            case STRING: {
                pair.value = strdup(value);
                break;
            }
            case INT: {
                int* val_cpy = malloc(sizeof(int));
                *val_cpy = *(int*)value;
                pair.value = val_cpy;
                break;
            }
            case FLOAT: {
                float* val_cpy = malloc(sizeof(float));
                *val_cpy = *(float*)value;
                pair.value = val_cpy;
                break;
            }
            case MAP: {
                Map* val_cpy = mapdup((Map*)value);
                pair.value = val_cpy;
                break;
            }
        }
        bigger_map->pairs[bigger_map->size] = pair;
        bigger_map->size++;
        
        return bigger_map;
    }

    KeyValuePair pair;
    pair.key = strdup(key);
    pair.type = type;

//    printf("%s\n", (char*)value);
    
    switch (type) {
        case STRING: {
            pair.value = strdup((char*)value);
            break;
        }
        case INT: {
            int* val_cpy = malloc(sizeof(int));
            *val_cpy = *(int*)value;
            pair.value = val_cpy;
            break;
        }
        case FLOAT: {
            float* val_cpy = malloc(sizeof(float));
            *val_cpy = *(float*)value;
            pair.value = val_cpy;
            break;
        }
        case MAP: {
            Map* val_cpy = mapdup((Map*)value);
            pair.value = val_cpy;
            break;
        }
    }

    map->pairs[map->size] = pair;
    map->size++;

    return NULL;
}

void insertInt(Map** map_ref, char* key, int value) {
    // printf("Address in map_ref (typed insert function 1): %x\n", *map_ref);
    int* val = malloc(sizeof(int));
    *val = value;
    Map* result = insert(*map_ref, key, val, INT);

    if (result == NULL) { return; }

    *map_ref = result;

    // printf("Address in map_ref (typed insert function 2): %x\n", *map_ref);
}

void insertFloat(Map** map_ref, char* key, float value) {
    // printf("Address in map_ref (typed insert function 1): %x\n", *map_ref);
    float* val = malloc(sizeof(float));
    *val = value;
    Map* result = insert(*map_ref, key, val, FLOAT);
    
    if (result == NULL) { return; }

    *map_ref = result;

    // printf("Address in map_ref (typed insert function 2): %x\n", *map_ref);
}

void insertString(Map** map_ref, char* key, char* value) {
//    char* val = strdup(value); // MIGHT ACTUALLY NEED THIS LINE. I'M NOT SURE.
    Map* result = insert(*map_ref, key, value, STRING);

    if (result == NULL) { return; }

    *map_ref = result;
    // printf("Address in map_ref (typed insert function 2): %x\n", *map_ref);
}

void insertMap(Map** map_ref, char* key, Map* value) {
    Map* result = insert(*map_ref, key, value, MAP);

    if (result == NULL) { return; }

    *map_ref = result;
}

void* get(Map* map, char* key, char* type) {
    assert(map->size <= map->mapCap);

    for (int i = 0; i < map->size; i++) {
        if (strcmp(map->pairs[i].key, key) == 0) {
            printf("Found key %s in map\n", key);
            *type = map->pairs[i].type;
            return map->pairs[i].value;
        }
    }

    *type = '\0';
    return NULL;
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
            case MAP: {
                printf("%s:\n", map->pairs[i].key);
                printMap((Map*)map->pairs[i].value); 
            }

        }
    }
    printf("------------------------\n");
}

