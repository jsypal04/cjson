// this test ensures parser functionality on all basic syntax
#include "../../src/cjson.h"
#include <stdlib.h>
#include <stdio.h>

/*ObjectAST* constructCorrectAST() {
    // allocate the nodes, statements are in in-order traversal
    ObjectAST* root = (ObjectAST*)malloc(sizeof(ObjectAST));
    ValueAST* root_val = (ValueAST*)malloc(sizeof(ValueAST));
    ArrayAST* root_val_arr = (ArrayAST*)malloc(sizeof(ArrayAST));
    ValueAST* root_val_arr_val = (ValueAST*)malloc(sizeof(ValueAST));
    ObjectAST* root_val_arr_val_obj = (ObjectAST*)malloc(sizeof(ObjectAST));
    ValueAST* root_val_arr_val_obj_val = (ValueAST*)malloc(sizeof(ValueAST));
    NextPairAST* root_val_arr_val_obj_nextp = (NextPairAST*)malloc(sizeof(NextPairAST));
    ValueAST* root_val_arr_val_obj_nextp_val = (ValueAST*)malloc(sizeof(ValueAST));
    NextValueAST* root_val_arr_nextv = (NextValueAST*)malloc(sizeof(NextValueAST));
    ValueAST* root_val_arr_nextv_val = (ValueAST*)malloc(sizeof(ValueAST));
    ObjectAST* root_val_arr_nextv_val_obj = (ObjectAST*)malloc(sizeof(ObjectAST));
    ValueAST* root_val_arr_nextv_val_obj_val = (ValueAST*)malloc(sizeof(ValueAST));
    NextPairAST* root_val_arr_nextv_val_obj_nextp = (NextPairAST*)malloc(sizeof(NextPairAST));
    ValueAST* root_val_arr_nextv_val_obj_nextp_val = (ValueAST*)malloc(sizeof(ValueAST));
    NextPairAST* root_nextp = (NextPairAST*)malloc(sizeof(NextPairAST));
    ValueAST* root_nextp_val = (ValueAST*)malloc(sizeof(ValueAST));
    NextPairAST* root_nextp_nextp = (NextPairAST*)malloc(sizeof(NextPairAST));
    ValueAST* root_nextp_nextp_val = (ValueAST*)malloc(sizeof(ValueAST));
    ObjectAST* root_nextp_nextp_val_obj = (ObjectAST*)malloc(sizeof(ObjectAST));
    ValueAST* root_nextp_nextp_val_obj_val = (ValueAST*)malloc(sizeof(ValueAST));
    NextPairAST* root_nextp_nextp_val_obj_nextp = (NextPairAST*)malloc(sizeof(NextPairAST));
    ValueAST* root_nextp_nextp_val_obj_nextp_val = (ValueAST*)malloc(sizeof(ValueAST));
    NextPairAST* root_nextp_nextp_val_obj_nextp_nextp = (NextPairAST*)malloc(sizeof(NextPairAST));
    ValueAST* root_nextp_nextp_val_obj_nextp_nextp_val = (ValueAST*)malloc(sizeof(ValueAST));

    // construct tree
    root->value = root_val;
    root->value->array = root_val_arr;
    root->value->array->value = root_val_arr_val;
    root->value->array->value->obj = root_val_arr_val_obj;
    root->value->array->value->obj->value = root_val_arr_val_obj_val;
    root->value->array->value->obj->nextPair = root_val_arr_val_obj_nextp;
    root->value->array->value->obj->nextPair->value = root_val_arr_val_obj_nextp_val;
    root->value->array->nextValue = root_val_arr_nextv;
    root->value->array->nextValue->value = root_val_arr_nextv_val;

}*/

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("ERROR: parserTest1: no json source passed to executable.\n");
        exit(1);
    }
    const char* jsonSrcPath = argv[1];
    ObjectAST* ast = parse(jsonSrcPath);

    printf("%x\n", ast->value);
    
    exit(0);
}
