#include "../../src/cjson.h"
#include <stdlib.h>


int main(int argc, char* argv[]) {
    Map* map1 = initMap(3);
    insertString(&map1, "name", "John Doe");
    insertInt(&map1, "age", 45);
    insertFloat(&map1, "balance", 132.45);

    // printf("Running mapdupTest.c:11\n");
    
    Map* outer_map = initMap(5);
    // printf("Running mapdupTest.c:14\n");
    insertString(&outer_map, "Company", "XYZ, inc.");
    insertInt(&outer_map, "numClients", 1);
    // printf("Running mapdupTest.c:17\n");
    insertMap(&outer_map, "clients", map1);
    // printf("Running mapdupTest.c:19\n");

    printMap(outer_map);

    return EXIT_SUCCESS;
}
