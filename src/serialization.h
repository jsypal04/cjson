#ifndef DUMP_H
#define DUMP_H

#include "cjson.h"
#include <stdint.h>

/*
 * writes the character passed to c at index idx of string_ref
 * */
uint32_t write_char(char** string_ref, uint32_t idx, char c);

/*
 * Serializes a MapArray as a json string
 * */
uint32_t dump_array(char** current_output, uint32_t current_idx, MapArray* array);

/*
 * Serializes a char* as a json string
 * */
uint32_t dump_string(char** current_output, uint32_t current_idx, char* value);

/*
 * Serializes a float as a json string
 * */
uint32_t dump_float(char** current_output, uint32_t current_idx, float value);

/*
 * Serializes an int as a json string
 * */
uint32_t dump_int(char** current_output, uint32_t current_idx, int value);

/*
 * Serializes a Map as a json string
 * */
uint32_t dump_map(char** current_output, uint32_t current_idx, Map* map);

#endif
