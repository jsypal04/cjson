// this test ensures parser functionality on all basic syntax
#include "../../src/cjson.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define NUM_KEYS 10
#define NUM_VALUES 12

char* correctKeys[] = {"\"main\"", "\"id\"", "\"inStock\"", "\"id\"", "\"inStock\"", "\"mainVendor\"", "\"CEO\"", 
                       "\"firstName\"", "\"lastName\"", "\"age\""};
char parsedKeys[NUM_KEYS][MAX_LEX_LEN];
unsigned int numKeysParsed = 0;

char* correctValues[] = {"array", "object", "0", "true", "object", "1.34", "false", "null", "object", "\"John\"", "\"Doe\"", "-34.28"};
char parsedValues[NUM_VALUES][MAX_LEX_LEN];
unsigned int numValuesParsed = 0;

// forward declarations
void checkObject(ObjectAST* obj);
void checkArray(ArrayAST* array);
void checkNextPair(NextPairAST* nextPair);
void checkValue(ValueAST* value);
void checkNextValue(NextValueAST* nextValue);

void checkObject(ObjectAST* obj) {
    if (numKeysParsed >= NUM_KEYS) {
        printf("ERROR: parsed test 1 (basic syntax test) failed. Too many keys were found.\n"); 
        exit(1);
    }
    strcpy(parsedKeys[numKeysParsed], obj->key);
    numKeysParsed++;

    if (obj->value == NULL) {
        printf("ERROR: parser test 1 (basic syntax test) failed. The value field of the ObjectAST struct is NULL.\n");
        exit(1);
    }
    
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
    if (numKeysParsed >= NUM_KEYS) {
        printf("ERROR: parsed test 1 (basic syntax test) failed. Too many keys were found.\n"); 
        exit(1);
    }

    strcpy(parsedKeys[numKeysParsed], nextPair->key);
    numKeysParsed++;
    
    checkValue(nextPair->value);
    if (nextPair->nextPair != NULL) {
        checkNextPair(nextPair->nextPair);
    }
}

void checkValue(ValueAST* value) {
    if (value->obj != NULL) {
        if (numValuesParsed >= NUM_VALUES) {
            printf("ERROR: parser test 1 (basic syntax test) failed. Too many values were found.\n");
            exit(1);
        }

        strcpy(parsedValues[numValuesParsed], "object");
        numValuesParsed++;
        checkObject(value->obj);
    }
    else if (value->array != NULL) {
        if (numValuesParsed >= NUM_VALUES) {
            printf("ERROR: parser test 1 (basic syntax test) failed. Too many values were found.\n");
            exit(1);
        }

        strcpy(parsedValues[numValuesParsed], "array");
        numValuesParsed++;
        checkArray(value->array);
    }
    else if (value->lexeme != NULL) {
        if (numValuesParsed >= NUM_VALUES) {
            printf("ERROR: parser test 1 (basic syntax test) failed. Too many values were found.\n");
            exit(1);
        }

        strcpy(parsedValues[numValuesParsed], value->lexeme);
        numValuesParsed++;
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

    bool passed = true;
    for (int i = 0; i < NUM_KEYS; i++) {
        if (strcmp(parsedKeys[i], correctKeys[i])) {
            passed = false;
            printf(
                "ERROR: parser test 1 (basic syntax test) failed. Parsed key %s and correct key %s do not match.\n", 
                parsedKeys[i], correctKeys[i]
            );
        }
    }
    for (int i = 0; i < NUM_VALUES; i++) {
        if (strcmp(parsedValues[i], correctValues[i])) {
            passed = false;
            printf(
                "ERROR: parser test 1 (basic syntax test) failed. Parsed key %s and correct key %s do not match.\n", 
                parsedValues[i], correctValues[i]
            );
        }
    }

    if (passed) {
        printf("Parser test 1 passed.\n");
        exit(0);
    }
    exit(1);
}
