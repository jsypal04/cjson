#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "cjson.h"


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
        else {
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
    }

    if (obj->nextPair != NULL) {
        traverse_npair(obj->nextPair, json_data);
    }

    return json_data;
}

void traverse_arr(ArrayAST* arr, Map* json_data);

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

void traverse_nval(NextValueAST* nval, Map* json_data);


Map* load_file(const char* path) {
    ObjectAST* ast = parse(path);
    Map* json_data = traverse_obj(ast);

    printMap(json_data);

    destroyAST(ast);

    return json_data;
}
