/**
 * cjson.h
 *
 * This file is the header file for the libcjson library.
 */

#ifndef CJSON_H
#define CJSON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct cj_value   CJ_Value;
typedef struct cj_kv_pair CJ_KvPair;
typedef struct cj_arr     CJ_Arr;
typedef struct cj_obj     CJ_Obj;

struct cj_arr {
    size_t cap;
    size_t len;
    CJ_Value* a;
};

struct cj_obj {
    size_t cap;
    size_t len;
    CJ_KvPair* o;
};

struct cj_value {
    enum tag {
        CJ_NULL = 0,
        CJ_NUM  = 1,
        CJ_STR  = 2,
        CJ_BOOL = 3,
        CJ_ARR  = 4,
        CJ_OBJ  = 5
    } tag;
    union value {
        double num;
        char* str;
        bool  b;
        CJ_Arr arr;
        CJ_Obj obj;
    } val;
};

struct cj_kv_pair {
    char* key;
    CJ_Value value;
};

/**
 * Functions to manipulate the CJ_Value type.
 */

/* Zeros the CJ_Value object. This is interpreted as null. */
void   cj_null_init(CJ_Value* val);

/* Initialize the CJ_Value with the given boolean */
int8_t cj_bool_init(CJ_Value* val, bool b);

/* Initialize the CJ_Value with the given double */
int8_t cj_num_init(CJ_Value* val, double num);

/* Initialize the CJ_Value with the given string */
int8_t cj_str_init(CJ_Value* val, char* str);

/* Initialize the CJ_Value to an empty array */
int8_t cj_arr_init(CJ_Value* val);

/* Initialize the CJ_Value to an empty object */
int8_t cj_obj_init(CJ_Value* val);

/* Get the value associated with `key` from `v` into `result` */
int8_t cj_getobj(CJ_Value  v, char* key, CJ_Value* result);

/* Get the value at index `idx` from `v` into `result` */
int8_t cj_getarr(CJ_Value  v, uint32_t idx, CJ_Value* result);

/* Put `item` into `v` with key `key` */
int8_t cj_putobj(CJ_Value* v, char* key, CJ_Value item);

/* Append `item` to `v` */
int8_t cj_putarr(CJ_Value* v, CJ_Value item);

/* Frees all dynamically allocated memory from `val` */
void cj_free_value(CJ_Value* val);

/**
 * Serialization and deserialization functions.
 */

/* Parses a char* of raw json into the CJ_Value provided in dest. Returns 0 for
 * success and a non-zero code for failure. */
int8_t cj_parse(CJ_Value* dest, char* json, size_t n);

/* Parses the json file `path` into a CJ_Value. Returns 0 for
 * success and a non-zero code for failure. */
int8_t cj_parsef(CJ_Value* dest, const char* path);

/* Serializes the CJ_Value into a char*. On failure, returns NULL. */
char* cj_dump(CJ_Value value);

#ifdef __cplusplus
}
#endif
#endif
