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


int8_t cj_obj_init(CJ_Value* val) {
    if (val == NULL) return -1;

    CJ_Obj obj = {0};
    obj.cap = 20;
    obj.len = 0;
    obj.o = malloc(sizeof(CJ_KvPair) * obj.cap);

    val->tag = CJ_OBJ;
    val->val.obj = obj;
    return 0;
}

int8_t cj_arr_init(CJ_Value* val) {
    if (val == NULL) return -1;

    CJ_Arr arr = {0};
    arr.cap = 20;
    arr.len = 0;
    arr.a = malloc(sizeof(CJ_Value) * arr.cap);

    val->tag = CJ_ARR;
    val->val.arr = arr;
    return 0;
}

int8_t cj_str_init(CJ_Value* val, char* str) {
    if (val == NULL) return -1;

    val->tag = CJ_STR;
    val->val.str = strdup(str);
    return 0;
}

int8_t cj_num_init(CJ_Value* val, double num) {
    if (val == NULL) return -1;

    val->tag = CJ_NUM;
    val->val.num = num;
    return 0;
}

int8_t cj_bool_init(CJ_Value* val, bool b) {
    if (val == NULL) return -1;

    val->tag = CJ_BOOL;
    val->val.b = b;
    return 0;
}

void cj_null_init(CJ_Value* val) {
    bzero(val, sizeof(CJ_Value));
}

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
        return 0;
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

/* Don't free anything that you put into `item`. */
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
    arr.len++;
    v->val.arr = arr;
    return 0;
}

void cj_free_obj(CJ_Obj* obj) {
    if (obj->o == NULL) {
        memset(obj, 0, sizeof *obj);
        return;
    }

    for (size_t i = 0; i < obj->len; i++) {
        if (obj->o[i].key != NULL) {
            free(obj->o[i].key);
            obj->o[i].key = NULL;
        }
        cj_free_value(&obj->o[i].value);
    }

    free(obj->o);
    memset(obj, 0, sizeof *obj);
}

void cj_free_arr(CJ_Arr* arr) {
    if (arr->a == NULL) {
        memset(arr, 0, sizeof *arr);
        return;
    }

    for (size_t i = 0; i < arr->len; i++) {
        cj_free_value(&arr->a[i]);
    }

    free(arr->a);
    memset(arr, 0, sizeof *arr);
}

void cj_free_value(CJ_Value* val) {
    switch (val->tag) {
        case CJ_STR:
            if (val->val.str == NULL) break;
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
    memset(val, 0, sizeof *val);
}
