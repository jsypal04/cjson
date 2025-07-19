#include "cjson.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>

uint32_t write_char(char** string_ref, uint32_t idx, char c) {
    if (idx >= MAX_FILE_SIZE_B) {
        printf("Object too large to serialize.");
        return idx;
    } 
    (*string_ref)[idx] = c;
    return idx + 1;
}

void dump_array(char** current_output);

void dump_string(char** current_output);

uint32_t dump_float(char** current_output, uint32_t current_idx, float value) {
    char buffer[100] = "\0";
    snprintf(buffer, sizeof(buffer), "%f", value);
    printf("writing value %s\n", buffer);

    for (int i = 0; i < strlen(buffer); i++) {
        current_idx = write_char(current_output, current_idx, buffer[i]);
    }
    return current_idx;
}

uint32_t dump_int(char** current_output, uint32_t current_idx, int value) {
    char buffer[100] = "\0";
    snprintf(buffer, sizeof(buffer), "%d", value);
    printf("writing value %s\n", buffer);

    for (int i = 0; i < strlen(buffer); i++) {
        current_idx = write_char(current_output, current_idx, buffer[i]);
    }
    return current_idx;
}

void dump_bool(char** current_output);

uint32_t dump_map(char** current_output, uint32_t current_idx, Map* map) {
    // write the opening brace
    printf("writing opening brace...\n");
    current_idx = write_char(current_output, current_idx, '{');

    for (int i = 0; i < map->size; i++) {
        KeyValuePair pair = map->pairs[i];

        // write the key to the output string here
        printf("writing key %s...\n", pair.key);
        uint32_t keylen = strlen(pair.key);
        for (int i = 0; i < keylen; i++) {
            current_idx = write_char(current_output, current_idx, pair.key[i]);
        }
        printf("writing colon...\n");
        current_idx = write_char(current_output, current_idx, ':');

        switch (pair.type) {
            // case 's': dump_string(current_output);
            case 'i': {
                current_idx = dump_int(current_output, current_idx, *(int*)pair.value);
                break;
            };
            case 'f': {
                current_idx = dump_float(current_output, current_idx, *(float*)pair.value);
                break;
            };
            // case 'b': dump_bool(current_output);
            // case 'a': dump_array(current_output);
            // case 'm': dump_map(current_output, current_idx, pair.value);
        }
        
        if (i != map->size - 1) {
            // write a comma
            printf("writing comma...\n");
            current_idx = write_char(current_output, current_idx, ',');
        }
    }
    // write the closing brace
    printf("writing closing brace...\n");
    current_idx = write_char(current_output, current_idx, '}');
    return current_idx;
}

char* dump(Map* map) {
    printf("mallocating memory...\n");
    char* json = malloc(sizeof(char) * MAX_FILE_SIZE_B);
    uint32_t current_idx = 0;

    printf("calling dump...\n");
    dump_map(&json, current_idx, map);
    printf("returning serialized json...\n");
    return json;
}
