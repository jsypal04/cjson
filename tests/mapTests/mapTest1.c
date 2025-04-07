/*
 * UNIT TEST 1: Map Capacity Expansion
 *
 * 
 *
 * */


#include "../../src/cjson.h"
#include <stdio.h>

#define TEST_ARR_LEN 11

char* keys[] =   { "main", "firstName", "lastName", "age", "height", "weight", "role", "address", "hello", "goodbye", "hello" };
char* values[] = { "0", "John", "Doe", "35", "120", "170", "CEO", "123 Main St", "world", "world", "hello" };


int main(int argc, char *argv[]) {
    Map* map = initMap(10);
    for (int i = 0; i < TEST_ARR_LEN; i++) {
//        printMap(map);
        insertString(&map, keys[i], values[i]);
    }
    
    printMap(map);

    char type;
    void* value = get(map, "firstName", &type);
    printf("Retrieved Value: %s\n", (char*)value);


    return EXIT_SUCCESS;
}
