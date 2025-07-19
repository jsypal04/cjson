#include <assert.h>
#include <ctype.h>
#include <iso646.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "cjson.h"
#include "deserialization.h"


bool is_int(char* value) {
    if (value[0] != '-' && !isdigit(value[0])) {
        return false;
    }

    for (int i = 1; i < strlen(value); i++) {
        if (!isdigit(value[i])) {
            return false;
        }
    }

    return true;
}


bool is_float(char* value) {
    if (value[0] != '-' && !isdigit(value[0])) {
        return false;
    }

    bool decimal_found = false;
    for (int i = 1; i < strlen(value); i++) {
        if (!isdigit(value[i]) && value[i] != '.') {
            return false;
        }
        else if (value[i] == '.' && decimal_found) {
            return false;
        }
        else if (value[i] == '.' && !decimal_found) {
            decimal_found = true;
        }
    }
    return true && decimal_found;
}


Map* traverse_obj(ObjectAST* obj) {
    // Implement this to return a map with everything in it
    Map* json_data = initMap(100);
    char type = traverse_val(obj->value);

    switch (type) {
        case INT: {
            assert(obj->value->lexeme != NULL);

            int val = atoi(obj->value->lexeme);
            insertInt(&json_data, obj->key, val);
            break;
        }
        case FLOAT: {
            assert(obj->value->lexeme != NULL);

            float val = atoi(obj->value->lexeme);
            insertFloat(&json_data, obj->key, val);
            break;
        }
        case STRING: {
            assert(obj->value->lexeme != NULL);

            insertString(&json_data, obj->key, obj->value->lexeme);
            break;
        }
        case MAP: {
            assert(obj->value->obj != NULL);

            Map* submap = traverse_obj(obj->value->obj);
            insertMap(&json_data, obj->key, submap);
            break;
        }
        case ARRAY: {
            assert(obj->value->array != NULL);

            MapArray* subarray = traverse_arr(obj->value->array);
            insertMapArray(&json_data, obj->key, subarray);
            break;
        }
    }

    if (obj->nextPair != NULL) {
        traverse_npair(obj->nextPair, json_data);
    }

    return json_data;
}

MapArray* traverse_arr(ArrayAST* arr) {
    MapArray* array = initMapArray(100);
    char type = traverse_val(arr->value);

    switch (type) {
        case INT: {
            assert(arr->value->lexeme != NULL);

            int val = atoi(arr->value->lexeme);
            appendInt(&array, val);
            break;
        }
        case FLOAT: {
            assert(arr->value->lexeme != NULL);

            float val = atof(arr->value->lexeme);
            appendFloat(&array, val);
            break;
        }
        case STRING: {
            assert(arr->value->lexeme != NULL);

            appendString(&array, arr->value->lexeme);
            break;
        }
        case MAP: {
            assert(arr->value->obj != NULL);

            Map* submap = traverse_obj(arr->value->obj);
            appendMap(&array, submap);
            break;
        }
        case ARRAY: {
            assert(arr->value->array != NULL);

            MapArray* subarray = traverse_arr(arr->value->array);
            appendMapArray(&array, subarray);
            break;
        }
        default: {
            printf("ERROR: unknown type %c found in `traverse_arr`\n", type);
            exit(1);
        }
    }

    if (arr->nextValue != NULL) {
        traverse_nval(arr->nextValue, array);
    }

    return array;
}

void traverse_npair(NextPairAST* npair, Map* json_data) {
    char type = traverse_val(npair->value);

    switch (type) {
        case INT: {
            assert(npair->value->lexeme != NULL);

            int value = atoi(npair->value->lexeme);
            insertInt(&json_data, npair->key, value);
            break;
        }
        case FLOAT: {
            assert(npair->value->lexeme != NULL);

            float value = atof(npair->value->lexeme);
            insertFloat(&json_data, npair->key, value);
            break;
        }
        case STRING: {
            assert(npair->value->lexeme != NULL);

            insertString(&json_data, npair->key, npair->value->lexeme);
            break;
        }
        case MAP: {
            assert(npair->value->obj != NULL);

            Map* submap = traverse_obj(npair->value->obj);
            insertMap(&json_data, npair->key, submap);
            break;
        }
        case ARRAY: {
            assert(npair->value->array != NULL);

            MapArray* subarray = traverse_arr(npair->value->array);
            insertMapArray(&json_data, npair->key, subarray);
            break;
        }
        default: {
            printf("Not handling this type yet\n");
        }
    }
    if (npair->nextPair != NULL) {
        traverse_npair(npair->nextPair, json_data);
    }
}

char traverse_val(ValueAST* val) {
    assert(val->lexeme != NULL || val->array != NULL || val->obj != NULL);

    if (val->lexeme != NULL) {
        if (is_int(val->lexeme)) {
            return INT;
        }
        else if (is_float(val->lexeme)) {
            return FLOAT;
        }
        else {
            return STRING;
        }
    }
    else if (val->array != NULL) {
        return ARRAY;
    }
    else {
        return MAP;
    }
}

void traverse_nval(NextValueAST* nval, MapArray* json_arr) {
    char type = traverse_val(nval->value);

    switch (type) {
        case INT: {
            assert(nval->value->lexeme != NULL);

            int value = atoi(nval->value->lexeme);
            appendInt(&json_arr, value);
            break;
        }
        case FLOAT: {
            assert(nval->value->lexeme != NULL);

            float value = atof(nval->value->lexeme);
            appendFloat(&json_arr, value);
            break;
        }
        case STRING: {
            assert(nval->value->lexeme != NULL);

            appendString(&json_arr, nval->value->lexeme);
            break;
        }
        case MAP: {
            assert(nval->value->obj != NULL);

            Map* submap = traverse_obj(nval->value->obj);
            appendMap(&json_arr, submap);
            break;
        }
        case ARRAY: {
            assert(nval->value->array != NULL);

            MapArray* submap = traverse_arr(nval->value->array);
            appendMapArray(&json_arr, submap);
        }
        default: {
            printf("Not handling this type yet\n");
        }
    }
    if (nval->nextValue != NULL) {
        traverse_nval(nval->nextValue, json_arr);
    }
}



Map* load_file(const char* path) {
    FILE* src_stream = fopen(path, "r");
    char source[MAX_FILE_SIZE_B] = "\0";
    uint32_t src_idx = 0;
    char c = getc(src_stream);
    while (c != EOF) {
        source[src_idx] = c;
        src_idx++;
        c = getc(src_stream);
    }
    source[src_idx] = '\0';
    fclose(src_stream);

    ObjectAST* ast = parse(source);
    Map* json_data = traverse_obj(ast);

    destroyAST(ast);

    return json_data;
}
