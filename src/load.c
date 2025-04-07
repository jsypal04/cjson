#include "cjson.h"


void traverse_obj(ObjectAST* obj, Map* json_data) {
    insertString(&json_data, obj->key, "hello world");
}

void traverse_arr(ArrayAST* arr, Map* json_data);

void traverse_npair(NextPairAST* npair, Map* json_data);

void traverse_val(ValueAST* val, Map* json_data);

void traverse_nval(NextValueAST* nval, Map* json_data);


Map* load_file(const char* path) {
    ObjectAST* ast = parse(path);
    Map* json_data = initMap(1);

    traverse_obj(ast, json_data);
    printMap(json_data);

    return json_data;
}
