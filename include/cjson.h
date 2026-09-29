/**
 * cjson.h
 *
 * This function provides the C compatiable API for this library
 * */
#ifndef CJSON_H
#define CJSON_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct CJSON CJSON;
typedef struct CVALUE CVALUE;

CJSON* new_json();
void  free_json(CJSON* json);

char* json_get_string(CJSON* json, const char* key);
float json_get_float(CJSON* json, const char* key);

#ifdef __cplusplus
}
#endif

#endif
