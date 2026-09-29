#include "../../src/cjson.h"


int main(int argc, char* argv[]) {
    MapArray* array = initMapArray(10);
    MapArray* subarray = initMapArray(3);
    appendInt(&subarray, 1);
    appendInt(&subarray, 2);

    Map* map = initMap(3);
    insertFloat(&map, "height", 21.5);
    insertString(&map, "hello", "world");

    appendInt(&array, 5);
    appendFloat(&array, 32.4);
    appendString(&array, "Hello");
    appendMapArray(&array, subarray);
    appendMap(&array, map);

    printMapArray(array);
    destroyMapArray(array);

    return 0;
}
