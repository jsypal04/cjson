/**
 * dump.c
 *
 * This file contains functions to serialize a CJ_Value into a char*
 */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cjson.h"
#include "utils.h"

/* Doubles buffer_p if space_needed is greater than or equal to
    cap - 1. The -1 is there to ensure an extra byte for null termination. */
int8_t expand(char** buffer_p, size_t cap, size_t space_needed) {
    char* buffer = *buffer_p;
    /* Ensure there is an extra byte at the end for the null terminator. */
    if (space_needed >= cap - 1) {
        int8_t ok;
        EXP_BUFF(buffer, cap, ok);
        if (ok != 0) {
            free(buffer);
            buffer = NULL;
            return ok;
        }
    }
    return 0;
}

char* dump_arr(CJ_Arr arr) {
    size_t i;
    size_t space_needed;

    size_t len = 0;
    size_t cap = 256;
    char* buffer = malloc(sizeof(char) * cap);

    if (buffer == NULL) return NULL;

    sprintf(buffer, "[");
    len++;

    for (i = 0; i < arr.len; i++) {
        char* v = cj_dump(arr.a[i]);
        if (v == NULL) {
            free(buffer);
            return NULL;
        }

        space_needed = len + (i == arr.len - 1 ? strlen(v) : strlen(v) + 1);
        if (expand(&buffer, cap, space_needed) != 0) {
            free(v);
            return NULL;
        }

        if (i == arr.len - 1) {
            sprintf(buffer + len, "%s", v);
        } else {
            sprintf(buffer + len, "%s,", v);
        }
        len = space_needed;

        free(v);
        v = NULL;
    }

    if (expand(&buffer, cap, len + 1) != 0) {
        return NULL;
    }
    sprintf(buffer + len, "]");
    len++;

    return buffer;
}

char* dump_obj(CJ_Obj obj) {
    size_t i;
    size_t space_needed;

    size_t len = 0;
    size_t cap = 256;
    char* buffer = malloc(sizeof(char) * cap);

    if (buffer == NULL) return NULL;

    sprintf(buffer, "{");
    len++;

    for (i = 0; i < obj.len; i++) {
        char* v = cj_dump(obj.o[i].value);
        if (v == NULL) {
            free(buffer);
            return NULL;
        }
        space_needed = len + strlen(obj.o[i].key) + strlen(v) + 3;
        space_needed += (i == obj.len - 1) ? 0 : 1;

        if (expand(&buffer, cap, space_needed) != 0) {
            free(v);
            return NULL;
        }

        if (i == obj.len - 1) {
            sprintf(buffer + len, "\"%s\":%s", obj.o[i].key, v);
        } else {
            sprintf(buffer + len, "\"%s\":%s,", obj.o[i].key, v);
        }
        len = space_needed;

        free(v);
        v = NULL;
    }

    if (expand(&buffer, cap, len + 1) != 0) {
        return NULL;
    }
    sprintf(buffer + len, "}");
    len++;

    return buffer;
}

char* cj_dump(CJ_Value value) {
    size_t space_needed;
    size_t len = 0;
    size_t cap = 512;
    char* buffer = malloc(sizeof(char) * cap);

    switch (value.tag) {
        case CJ_STR: {
            space_needed = strlen(value.val.str) + len + 2;
            if (expand(&buffer, cap, space_needed) != 0)
                return NULL;
            sprintf(buffer + len, "\"%s\"", value.val.str);
            len = space_needed;
            break;
        }
        case CJ_NUM: {
            space_needed = len + 25;
            if (expand(&buffer, cap, space_needed) != 0)
                return NULL;
            sprintf(buffer + len, "%g", value.val.num);
            len = space_needed;
            break;
        }
        case CJ_NULL: {
            space_needed = len + 4;
            if (expand(&buffer, cap, space_needed) != 0)
                return NULL;
            sprintf(buffer + len, "null");
            len = space_needed;
            break;
        }
        case CJ_BOOL: {
            space_needed = len + 5;
            if (expand(&buffer, cap, space_needed) != 0)
                return NULL;
            sprintf(buffer + len, "%s", value.val.b ? "true" : "false");
            len = space_needed;
            break;
        }
        case CJ_ARR: {
            char* arr_s = dump_arr(value.val.arr);
            if (arr_s == NULL) {
                free(buffer);
                return NULL;
            }
            space_needed = len + strlen(arr_s);
            if (expand(&buffer, cap, space_needed) != 0) {
                free(arr_s);
                return NULL;
            }
            sprintf(buffer + len, "%s", arr_s);
            len = space_needed;
            free(arr_s);
            arr_s = NULL;
            break;
        }
        case CJ_OBJ: {
            char* obj_s = dump_obj(value.val.obj);
            if (obj_s == NULL) {
                free(buffer);
                return NULL;
            }
            space_needed =  len + strlen(obj_s);
            if (expand(&buffer, cap, space_needed) != 0) {
                free(obj_s);
                return NULL;
            }
            sprintf(buffer + len, "%s", obj_s);
            len = space_needed;
            free(obj_s);
            obj_s = NULL;
            break;
        }
    }

    return buffer;
}
