// this test ensures parser functionality on all basic syntax
#include "../../src/cjson.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUM_KEYS 10

char* correctKeys[] = {"main", "id", "inStock", "id", "inStock", "mainVendor", "CEO", "firstName", "lastName", "age"};
char parsedKeys[NUM_KEYS][MAX_LEX_LEN];
int numKeysParsed = 0;

// forward declarations
void checkObject(ObjectAST* obj);
void checkArray(ArrayAST* array);
void checkNextPair(NextPairAST* nextPair);
void checkValue(ValueAST* value);
void checkNextValue(NextValueAST* nextValue);

void checkObject(ObjectAST* obj) {
    strcpy(parsedKeys[numKeysParsed], obj->key);
    numKeysParsed++;

    checkValue(obj->value);
    if (obj->nextPair != NULL) {
        checkNextPair(obj->nextPair);
    }
}

void checkArray(ArrayAST* array) {
    checkValue(array->value);
    if (array->nextValue != NULL) {
        checkNextValue(array->nextValue);
    }
}

void checkNextPair(NextPairAST* nextPair) {
    strcpy(parsedKeys[numKeysParsed], nextPair->key);
    numKeysParsed++;
    
    checkValue(nextPair->value);
    if (nextPair->nextPair != NULL) {
        checkNextPair(nextPair->nextPair);
    }
}

void checkValue(ValueAST* value) {
    if (value->obj != NULL) {
        checkObject(value->obj);
    }
    else if (value->array != NULL) {
        checkArray(value->array);
    }
}

void checkNextValue(NextValueAST* nextValue) {
    checkValue(nextValue->value);
    if (nextValue->nextValue != NULL) {
        checkNextValue(nextValue->nextValue);
    }
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("ERROR: parserTest1: no json source passed to executable.\n");
        exit(1);
    }
    const char* jsonSrcPath = argv[1];
    ObjectAST* ast = parse(jsonSrcPath);

    printf("\n------------------------------------\n");

    checkObject(ast);
    for (int i = 0; i < NUM_KEYS; i++) {
        printf("Parsed Key: %s\nCorrect Key: %s\n\n", parsedKeys[i], correctKeys[i]);
    }

    exit(0);
}
