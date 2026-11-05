/**
 * value.c
 *
 * This file contains functions for manipulating the CJ_Value type
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "cjson.h"
#include "utils.h"


int8_t cj_getobj(CJ_Value v, char* key, CJ_Value* result) {
    if (v.tag != CJ_OBJ) {
        return 1;
    }
    CJ_Obj obj = v.val.obj;
    for (size_t i = 0; i < obj.len; i++) {
        if (strcmp(obj.o[i].key, key) != 0) continue;
        *result = obj.o[i].value;
        return 0;
    }

    return 1;
}

int8_t cj_getarr(CJ_Value v, uint32_t idx, CJ_Value* result) {
    if (v.tag != CJ_ARR) {
        return 1;
    }
    CJ_Arr arr = v.val.arr;
    for (size_t i = 0; i < arr.len; i++) {
        if (i != idx) continue;
        *result = arr.a[i];
    }
    return 1;
}

// Don't free anything that you put into item. You can free key though.
int8_t cj_putobj(CJ_Value* v, char* key, CJ_Value item) {
    if (v->tag != CJ_OBJ) {
        return 1;
    }
    CJ_Obj obj = v->val.obj;
    if (obj.cap == obj.len) {
        size_t ok;
        EXP_BUFF(obj.o, obj.cap, ok);
        if (ok != 0) return ok;
    }
    CJ_KvPair pair = {0};
    pair.key = strdup(key);
    // I should duplicate the CJ_Value here but I need to write that helper first.
    pair.value = item;
    obj.o[obj.len] = pair;
    obj.len++;
    v->val.obj = obj;

    return 0;
}

// Don't free anything that you put into item.
int8_t cj_putarr(CJ_Value* v, CJ_Value item) {
    if (v->tag != CJ_ARR) {
        return 1;
    }
    CJ_Arr arr = v->val.arr;
    if (arr.cap == arr.len) {
        size_t ok;
        EXP_BUFF(arr.a, arr.cap, ok);
        if (ok != 0) return ok;;
    }
    arr.a[arr.len] = item;
    v->val.arr = arr;
    return 0;
}

void cj_free_obj(CJ_Obj* obj) {
    for (size_t i = 0; i < obj->len; i++) {
        free(obj->o[i].key);
        obj->o[i].key = NULL;
        cj_free_value(&obj->o[i].value);
    }

    free(obj->o);
    obj->o = NULL;
}

void cj_free_arr(CJ_Arr* arr) {
    for (size_t i = 0; i < arr->len; i++) {
        cj_free_value(&arr->a[i]);
    }

    free(arr->a);
    arr->a = NULL;
}

void cj_free_value(CJ_Value* val) {
    switch (val->tag) {
        case CJ_STR:
            free(val->val.str);
            val->val.str = NULL;
            break;
        case CJ_ARR:
            cj_free_arr(&val->val.arr);
            break;
        case CJ_OBJ:
            cj_free_obj(&val->val.obj);
            break;
        default:
           // Nothing to do. Just to silence clangd.
           break;
    }
}
