/*
 * Testing the Map data structure generally before I write actual unit tests
 *
 * The main thing I worry about now is issues with memory management. For example, 
 * say I create a function and make an interger and put that integer in the map then return
 * a pointer to the map from the function. The value the map actually stores is a pointer to the
 * integer. My question is, since the integer is stored on the stack it should be popped when the
 * function returns so what will be the value stored in the map AFTER the function returns it.
 * I doubt it will be the same integer.
 *
 * Assuming that I am right about the above behavior, I will probably need to malloc an int and 
 * then copy that passed value into the malloced memory. Then the integer won't go out of scope when
 * the function returns.
 *
 * */


#include "../../src/cjson.h"

#define TEST_ARR_LEN 3



/* Map* makeMap() {
    Map* map = initMap(20);
    char* value = "hello";
    printf("Address of `value`: %x\n", &value);

    insertFloat(map, "main", 12.5);
    char type;
    float* retrievedVal = get(map, "main", &type);

    printf("--------Values makeMap scope--------\n");
    printf("%f (address: %x)\n", *retrievedVal, retrievedVal);

    return map;
}

int main(int argc, char *argv[]) {
    Map* map = makeMap();
    char type;
    float* retrievedVal = get(map, "main", &type);

    printf("--------Values in main scope--------\n");
    printf("%c\n", type);
    printf("%f (address: %x)\n", *retrievedVal, retrievedVal);

    return EXIT_SUCCESS;
} */

int main(int argc, char *argv[]) {
    Map* map = initMap(40);     
    printf("Successfully created a new Map\n");

    char* keys[] = { "main1", "main2", "main3", "table", "chair", "stool", "bench", "couch", "bed" };
    char* strings[] = { "hello", "world", "value" };
    int ints[] = { 10, 125, -32 };
    float floats[] = { 1.5, 205.3456, -32.23 };

    int j = 0;
    for (int i = 0; i < TEST_ARR_LEN; i++) {
        // printMap(map);
        insertString(map, keys[j], strings[i]);
        j++;
        // printMap(map);
        insertInt(map, keys[j], ints[i]);
        j++;
        // printMap(map);
        insertFloat(map, keys[j], floats[i]);
        j++;
        printf("Map Size: %d\n", map->size);
    }
    printMap(map);
    destroyMap(map);

    return EXIT_SUCCESS;
}
