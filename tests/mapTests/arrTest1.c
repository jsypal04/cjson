#include "../../src/cjson.h"


int main(int argc, char* argv[]) {
    MapArray* array = initMapArray(10);

    appendInt(&array, 5);
    appendFloat(&array, 32.4);
    appendString(&array, "Hello");

    printMapArray(array);
    destroyMapArray(array);

    return 0;
}
