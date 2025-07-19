#include "cjson.h"
#include "serialization.h"
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

uint32_t dump_array(char** current_output, uint32_t current_idx, MapArray* array) {
    // write the opening bracket
    current_idx = write_char(current_output, current_idx, '[');

    for (int i = 0; i < array->size; i++) {
        Element element = array->array[i];

        switch (element.type) {
            case 's': {
                current_idx = dump_string(current_output, current_idx, (char*)element.value);
                break;
            };
            case 'i': {
                current_idx = dump_int(current_output, current_idx, *(int*)element.value);
                break;
            };
            case 'f': {
                current_idx = dump_float(current_output, current_idx, *(float*)element.value);
                break;
            };
            // case 'b': dump_bool(current_output);
            case 'a': {
                current_idx = dump_array(current_output, current_idx, (MapArray*)element.value);
                break;
            }
            case 'm': {
                current_idx = dump_map(current_output, current_idx, (Map*)element.value);
                break;
            }
        }

        if (i < array->size - 1) {
            // write the comma
            current_idx = write_char(current_output, current_idx, ',');
        }
    }

    // write the closing bracket
    current_idx = write_char(current_output, current_idx, ']');
    return current_idx;
}

uint32_t dump_string(char** current_output, uint32_t current_idx, char* value) {
    current_idx = write_char(current_output, current_idx, '"');
    for (int i = 0; i < strlen(value); i++) {
        current_idx = write_char(current_output, current_idx, value[i]);
    }
    current_idx = write_char(current_output, current_idx, '"');
    return current_idx;
}

uint32_t dump_float(char** current_output, uint32_t current_idx, float value) {
    char buffer[100] = "\0";
    snprintf(buffer, sizeof(buffer), "%f", value);

    for (int i = 0; i < strlen(buffer); i++) {
        current_idx = write_char(current_output, current_idx, buffer[i]);
    }
    return current_idx;
}

uint32_t dump_int(char** current_output, uint32_t current_idx, int value) {
    char buffer[100] = "\0";
    snprintf(buffer, sizeof(buffer), "%d", value);

    for (int i = 0; i < strlen(buffer); i++) {
        current_idx = write_char(current_output, current_idx, buffer[i]);
    }
    return current_idx;
}

void dump_bool(char** current_output);

uint32_t dump_map(char** current_output, uint32_t current_idx, Map* map) {
    // write the opening brace
    current_idx = write_char(current_output, current_idx, '{');

    for (int i = 0; i < map->size; i++) {
        KeyValuePair pair = map->pairs[i];

        // write the key to the output string here
        current_idx = write_char(current_output, current_idx, '"');

        uint32_t keylen = strlen(pair.key);
        for (int i = 0; i < keylen; i++) {
            current_idx = write_char(current_output, current_idx, pair.key[i]);
        }

        current_idx = write_char(current_output, current_idx, '"');
        current_idx = write_char(current_output, current_idx, ':');

        switch (pair.type) {
            case 's': {
                current_idx = dump_string(current_output, current_idx, (char*)pair.value);
                break;
            };
            case 'i': {
                current_idx = dump_int(current_output, current_idx, *(int*)pair.value);
                break;
            };
            case 'f': {
                current_idx = dump_float(current_output, current_idx, *(float*)pair.value);
                break;
            };
            // case 'b': dump_bool(current_output);
            case 'a': {
                current_idx = dump_array(current_output, current_idx, (MapArray*)pair.value);
                break;
            }
            case 'm': {
                current_idx = dump_map(current_output, current_idx, (Map*)pair.value);
                break;
            }
        }
        
        if (i != map->size - 1) {
            // write a comma
            current_idx = write_char(current_output, current_idx, ',');
        }
    }
    // write the closing brace
    current_idx = write_char(current_output, current_idx, '}');
    return current_idx;
}

char* dump(Map* map) {
    char* json = malloc(sizeof(char) * MAX_FILE_SIZE_B);
    uint32_t current_idx = 0;

    dump_map(&json, current_idx, map);
    return json;
}

int dumpf(Map* map, const char* output_path) {
    FILE* output_file = fopen(output_path, "w");

    if (output_file == NULL) {
        printf("Failed to create file.\n");
        return 1;
    }

    char* json = dump(map);
    fputs(json, output_file);
    fclose(output_file);

    return 0;
}
