#include "../../src/cjson.h"
#include <stdio.h>


int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("ERROR: To few arguments passed to loadTest1\n");
        return 1;
    }
    else if (argc > 2) {
        printf("ERROR: To many arguments passed to loadTest1\n");
        return 1;
    }

    const char* path = argv[1];
    
    Map* json_data = load_file(path); 
    
    return 0;
}
