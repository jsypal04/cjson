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
    for (int i = 0; i < map->size; i++) {
        // printf("Destroying map pair %d\n", i);
        if (map->pairs[i].key != NULL) {
            free(map->pairs[i].key);
            map->pairs[i].key = NULL;
        }
        // printf("Destroyed key %d\n", i);
        if (map->pairs[i].value != NULL) {
            if (map->pairs[i].type != MAP && map->pairs[i].type != ARRAY) {
                free(map->pairs[i].value);
                // printf("Destroyed value that is not MAP or ARRAY...\n");
                map->pairs[i].value = NULL;
            }
            else if (map->pairs[i].type == ARRAY) {
                destroyMapArray((MapArray*)map->pairs[i].value);
            }
            else if (map->pairs[i].type == MAP) {
                destroyMap((Map*)map->pairs[i].value);
            }
            else {
                printf("ERROR: unknown type %c found while destroying map.\n", map->pairs[i].type);
                exit(1);
            }
        }
        // printf("Destroyed value %d\n", i);
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
            case ARRAY: {
                pair.value = arrdup((MapArray*)source->pairs[i].value);
                break;
            }
            default: {
                printf("ERROR: unkown type %d found in mapdup.\n", source->pairs[i].type);
                exit(1);
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
        Map* bigger_map = initMap(map->mapCap * 2);

        // copy values from old map
        for (int i = 0; i < map->size; i++) {
            KeyValuePair pair;
            pair.key = strdup(map->pairs[i].key);
            switch (map->pairs[i].type) {
                case INT: {
                    int* value = malloc(sizeof(int));
                    *value = *(int*)map->pairs[i].value;
                    pair.value = value;
                    break;
                }
                case FLOAT: {
                    float* value = malloc(sizeof(float));
                    *value = *(float*)map->pairs[i].value;
                    pair.value = value;
                    break;
                }
                case STRING: {
                    pair.value = strdup((char*)map->pairs[i].value);
                    break;
                }
                case MAP: {
                    pair.value = mapdup((Map*)map->pairs[i].value);
                    break;
                }
                case ARRAY: {
                    pair.value = arrdup((MapArray*)map->pairs[i].value);
                    break;
                }
                default: {
                    printf("ERROR: unkown type %d found expanding map in insert.\n", map->pairs[i].type);
                    exit(1);
                }
            }
            bigger_map->pairs[i] = pair;
            bigger_map->size++;
        }

        // 3. destroy old map
        destroyMap(map);
        map = NULL;

        // Add new value

        KeyValuePair pair;
        pair.key = strdup(key);
        pair.type = type;
        pair.value = value;

        bigger_map->pairs[bigger_map->size] = pair;
        bigger_map->size++;

        return bigger_map;
    }

    KeyValuePair pair;
    pair.key = strdup(key);
    pair.type = type;
    pair.value = value;

//    printf("%s\n", (char*)value);

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
    char* val = strdup(value); // MIGHT ACTUALLY NEED THIS LINE. I'M NOT SURE.
    Map* result = insert(*map_ref, key, val, STRING);

    if (result == NULL) { return; }

    *map_ref = result;
    // printf("Address in map_ref (typed insert function 2): %x\n", *map_ref);
}

void insertMap(Map** map_ref, char* key, Map* value) {
    Map* result = insert(*map_ref, key, value, MAP);

    if (result == NULL) { return; }

    *map_ref = result;
}

void insertMapArray(Map** map_ref, char* key, MapArray* value) {
    Map* result = insert(*map_ref, key, value, ARRAY);

    if (result == NULL) { return; }

    *map_ref = result;
}

void* get(Map* map, char* key, char* type) {
    assert(map->size <= map->mapCap);

    for (int i = 0; i < map->size; i++) {
        if (strcmp(map->pairs[i].key, key) == 0) {
            *type = map->pairs[i].type;
            return map->pairs[i].value;
        }
    }

    *type = '\0';
    return NULL;
}

void printMap(Map* map) {
    printf("------------------------\n");
    for (int i = 0; i < map->size; i++) {
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
                break;
            }
            case ARRAY: {
                printf("%s:\n", map->pairs[i].key);
                printMapArray((MapArray*)map->pairs[i].value);
                break;
            }
            default: {
                printf("ERROR: unknown type %d found in `printMap`\n", type);
                exit(1);
            }
        }
    }
    printf("------------------------\n");
}

MapArray* initMapArray(int arr_cap) {
    MapArray* arr = malloc(sizeof(MapArray));
    arr->cap = arr_cap;
    arr->size = 0;

    Element* elements = malloc(sizeof(Element) * arr_cap);
    arr->array = elements;

    return arr;
}

void destroyMapArray(MapArray *array) {
    for (int i = 0; i < array->size; i++){
        if (array->array[i].type != MAP && array->array[i].type != ARRAY) {
            if (array->array[i].value != NULL) {
                free(array->array[i].value);
                array->array[i].value = NULL;
            }
        }
        else if (array->array[i].type == ARRAY) {
            destroyMapArray((MapArray*)array->array[i].value);
        }
        else if (array->array[i].type == MAP) {
            destroyMap((Map*)array->array[i].value);
        }
    }

    free(array->array);
    array->array = NULL;

    free(array);
    array = NULL;
}

MapArray* arrdup(MapArray* array) {
    assert(array->size <= array->cap);

    MapArray* copy = initMapArray(array->cap);

    for (int i = 0; i < array->size; i++) {
        switch (array->array[i].type) {
            case INT: {
                appendInt(&copy, *(int*)array->array[i].value);
                break;
            }
            case FLOAT: {
                appendFloat(&copy, *(float*)array->array[i].value);
                break;
            }
            case STRING: {
                appendString(&copy, (char*)array->array[i].value);
                break;
            }
            case MAP: {
                Map* value = mapdup((Map*)array->array[i].value);
                appendMap(&copy, value);
                break;
            }
            case ARRAY: {
                MapArray* value = arrdup((MapArray*)array->array[i].value);
                appendMapArray(&copy, value);
                break;
            }
            default: {
                printf("ERROR: unkown type %d found in arrdup().\n", array->array[i].type);
                exit(1);
            }
        }
    }
    return copy;
}

MapArray* append(MapArray* arr, void* value, char type) {
    assert(arr->size <= arr->cap);

    if (arr->size == arr->cap) {
        // TODO: Expand array
        MapArray* bigger_arr = initMapArray(arr->cap * 2);

        // copy values
        for (int i = 0; i < arr->size; i++) {
            Element element;
            switch (arr->array[i].type) {
                case INT: {
                    appendInt(&arr, *(int*)arr->array[i].value);
                    break;
                }
                case FLOAT: {
                    appendFloat(&arr, *(float*)arr->array[i].value);
                    break;
                }
                case STRING: {
                    appendString(&arr, (char*)arr->array[i].value);
                    break;
                }
                case MAP: {
                    Map* value = mapdup((Map*)arr->array[i].value);
                    appendMap(&arr, value);
                    break;
                }
                case ARRAY: {
                    MapArray* value = arrdup((MapArray*)arr->array[i].value);
                    appendMapArray(&arr, value);
                    break;
                }
                default: {
                    printf("ERROR: unkown type %d found expanding capacity of an array.\n", arr->array[i].type);
                    exit(1);
                }
            }
            bigger_arr->array[i] = element;
            bigger_arr->size++;
        }

        // insert new element
        Element element;
        element.value = value;
        element.type = type;

        arr->array[arr->size] = element;
        arr->size++;

        return bigger_arr;
    }

    Element element;
    element.value = value;
    element.type = type;

    arr->array[arr->size] = element;
    arr->size++;

    return NULL;
}

void appendInt(MapArray** arr_ref, int value) {
    int* val_cpy = malloc(sizeof(int));
    *val_cpy = value;
    MapArray* result = append(*arr_ref, val_cpy, INT);

    if (result == NULL) { return; }

    *arr_ref = result;
}

void appendFloat(MapArray** arr_ref, float value) {
    float* val_cpy = malloc(sizeof(float));
    *val_cpy = value;
    MapArray* result = append(*arr_ref, val_cpy, FLOAT);

    if (result == NULL) { return; }

    *arr_ref = result;
}

void appendString(MapArray** arr_ref, char* value) {
    char* val_cpy = strdup(value);
    MapArray* result = append(*arr_ref, val_cpy, STRING);

    if (result == NULL) { return; }

    *arr_ref = result;
}

void appendMap(MapArray** arr_ref, Map* value) {
    MapArray* result = append(*arr_ref, value, MAP);

    if (result == NULL) { return; }

    *arr_ref = result;
}

void appendMapArray(MapArray** arr_ref, MapArray* value) {
    MapArray* result = append(*arr_ref, value, ARRAY);

    if (result == NULL) { return; }

    *arr_ref = result;
}

void* read_arr(MapArray* array, int index, char* type) {
    if (index >= array->size) {
        return NULL;
    }

    void* value = array->array[index].value;
    *type = array->array[index].type;

    return value;
}

void printMapArray(MapArray* array) {
    printf("------------------------\n");
    for (int i = 0; i < array->size; i++) {
        switch (array->array[i].type) {
            case INT: {
                printf("  [ %d\n", *(int*)array->array[i].value);
                break;
            }
            case FLOAT: {
                printf("  [ %f\n", *(float*)array->array[i].value);
                break;
            }
            case STRING: {
                printf("  [ %s\n", (char*)array->array[i].value);
                break;
            }
            case MAP: {
                printMap((Map*)array->array[i].value);
                break;
            }
            case ARRAY: {
                printMapArray((MapArray*)array->array[i].value);
                break;
            }
            default: {
                printf("ERROR: unknown type %d found in `printMapArray`\n", array->array[i].type);
                exit(1);
            }
        }
    }
    printf("------------------------\n");
}

bool map_cmp(Map* map1, Map* map2) {
    if (map1->size != map2->size) return false;

    for (int i = 0; i < map1->size; i++) {
        KeyValuePair pair1 = map1->pairs[i];
        KeyValuePair pair2 = map2->pairs[i];

        if (strcmp(pair1.key, pair2.key) != 0) return false;

        if (pair1.type != pair2.type) return false;

        bool values_are_eq;
        switch (pair1.type) {
            case INT: {
                values_are_eq = *(int*)pair1.value == *(int*)pair2.value;
                break;
            }
            case FLOAT: {
                values_are_eq = *(float*)pair1.value == *(float*)pair2.value;
                break;
            }
            case STRING: {
                values_are_eq = strcmp((char*)pair1.value, (char*)pair2.value) == 0;
                break;
            }
            case MAP: {
                values_are_eq = map_cmp((Map*)pair1.value, (Map*)pair2.value);
                break;
            }
            case ARRAY: {
                values_are_eq = arr_cmp((MapArray*)pair1.value, (MapArray*)pair2.value);
                break;
            }
        }

        if (!values_are_eq) return false;
    }
    return true;
}

bool arr_cmp(MapArray* arr1, MapArray* arr2) {
    if (arr1->size != arr2->size) return false;

    for (int i = 0; i < arr1->size; i++) {
        Element element1 = arr1->array[i];
        Element element2 = arr2->array[i];

        if (element1.type != element2.type) return false;

        bool values_are_eq;
        switch (element1.type) {
            case INT: {
                values_are_eq = *(int*)element1.value == *(int*)element2.value;
                break;
            }
            case FLOAT: {
                values_are_eq = *(float*)element1.value == *(float*)element2.value;
                break;
            }
            case STRING: {
                values_are_eq = strcmp((char*)element1.value, (char*)element2.value) == 0;
                break;
            }
            case MAP: {
                values_are_eq = map_cmp((Map*)element1.value, (Map*)element2.value);
                break;
            }
            case ARRAY: {
                values_are_eq = arr_cmp((MapArray*)element1.value, (MapArray*)element2.value);
                break;
            }
        }

        if (!values_are_eq) return false;
    }
    return true;
}
