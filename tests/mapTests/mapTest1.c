#include "../../src/cjson.h"


void printMap(Map* map) {
    for (int i = 0; i < map->mapCap; i++) {
        printf("%s\n", (char*)map->pairs[i].value);
    }
}

int main(int argc, char *argv[]) {
    Map* map = initMap(10);     
    for (int i = 0; i < map->mapCap; i++) {
        map->pairs[i].value = "Empty Slot in Map";
    }
    printf("Successfully created a new Map\n");

    insert(map, "main", "value", 's');
    char type;
    void* value = get(map, "main", &type);
    printf("Type of retrieved value: %c\n", type);

    printMap(map);


    return EXIT_SUCCESS;
}
