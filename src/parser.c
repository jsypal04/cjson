#include "cjson.h"
#include <stdlib.h>

/* Variable to keep track of the depth of the branch of the parse tree that
the parser is on. For debugging purposes. */
int astDepth = 0;

ObjectAST* parse(const char* sourcePath) {
    printf("Begin parsing %s.\n", sourcePath);

    SourceLexState state = initLexer(sourcePath);
    ObjectAST* root = parseObject(&state);
    fclose(state.source);

    printf("End parsing %s.\n", sourcePath);
    return root;
}

ObjectAST* parseObject(SourceLexState* state) {
    printf("Parsing object...\n");
    if (state->token != LBRACE) {
        printf("MAYHEM: We in serious trouble now.\n");
        exit(0);
    }

    ObjectAST* obj = (ObjectAST*)malloc(sizeof(ObjectAST));

    lex(state);
    if (state->token != STR) {
        printf("ERROR: object key must be a string.\n");
        exit(1);
    }
    obj->key = state->lexeme;
    lex(state);
    if (state->token != COLON) {
        printf("ERROR: object key must be followed by a colon.\n");
        exit(1);
    }
    astDepth++;
    ValueAST* value = parseValue(state);
    astDepth--;
    obj->value = value;

    // there is another key-value pair to process
    if (state->token == COMMA) {
        astDepth++;
        NextPairAST* nextPair = parseNextPair(state);
        astDepth--;
        obj->nextPair = nextPair;
        return obj;
    }
    // end of object
    else if (state->token == RBRACE) {
        obj->nextPair = NULL;
        return obj;
    }
    else {
        printf("ERROR: unknown symbol %s.\n", state->lexeme);
        exit(1);
    }
    printf("Done parsing object.\n");
}

ArrayAST* parseArray(SourceLexState* state) {
    if (state->token != LBRACKET) {
        printf("MAYHEM: We in serious trouble now.\n");
        exit(0);
    }

    ArrayAST* array = (ArrayAST*)malloc(sizeof(ArrayAST));
    ValueAST* value = parseValue(state);
    if (state->token == COMMA) {
        NextValueAST* nextValue = parseNextValue(state);
        array->nextValue = nextValue;
        return array;
    }
    else if (state->token == RBRACKET) {
        array->nextValue = NULL;
        lex(state);
        return array;
    }
    else {
        printf("ERROR: unknown character %s.\n", state->lexeme);
        exit(1);
    }
}

NextPairAST* parseNextPair(SourceLexState* state) {
    if (state->token != COMMA) {
        printf("MAYHEM: We in serious trouble now.\n");
        exit(0);
    }

    NextPairAST* nextPair = (NextPairAST*)malloc(sizeof(NextPairAST));
    lex(state);
    if (state->token != STR) {
        printf("ERROR: object key must be a string.\n");
        exit(1);
    }
    nextPair->key = state->lexeme;
    lex(state);
    if (state->token != COLON) {
        printf("ERROR: object key must be followed by a colon.\n");
        exit(1);
    }
    ValueAST* value = parseValue(state);
    nextPair->value = value;

    if (state->token == COMMA) {
        // another pair to parse
        NextPairAST* nextNextPair = parseNextPair(state);
        nextPair->nextPair = nextNextPair;
        return nextPair;
    }
    else if (state->token == RBRACE) {
        // end of object
        nextPair->nextPair = NULL;
        return nextPair;
    }
    else {
        // error
        printf("ERROR: unknown symbol %s.\n", state->lexeme);
        exit(1);
    }
}

ValueAST* parseValue(SourceLexState* state) {
    ValueAST* value = (ValueAST*)malloc(sizeof(ValueAST));

    lex(state);
    if (state->token == STR || state->token == NUM || state->token == BOOL || state->token == NONE) {
        value->lexeme = state->lexeme;
        value->array = NULL;
        value->obj = NULL;
        lex(state);
        return value;
    }
    else if (state->token == LBRACKET) {
        // array
        ArrayAST* array = parseArray(state);
        value->array = array;
        value->lexeme = NULL;
        value->obj = NULL;
        return value;
    }
    else if (state->token == LBRACE) {
        // object
        ObjectAST* obj = parseObject(state);
        value->obj = obj;
        value->lexeme = NULL;
        value->array = NULL;
        return value;
    }
    else {
        // error
        printf("ERROR: Invalid value.\n");
        exit(1);
    }
}

NextValueAST* parseNextValue(SourceLexState* state) {
    if (state->token != COMMA) {
        printf("MAYHEM: We in serious trouble now.\n");
        exit(0);
    }

    NextValueAST* nextValue = (NextValueAST*)malloc(sizeof(NextPairAST));
    ValueAST* value = parseValue(state);
    nextValue->value = value;
    lex(state);
    if (state->token == COMMA) {
        // parse another nextValue
        NextValueAST* nextNextValue = parseNextValue(state);
        nextValue->nextValue = nextNextValue;
        return nextValue;
    }
    else {
        // return as is
        nextValue->nextValue = NULL;
        return nextValue;
    }
}

void destroyAST(ObjectAST* root) {
    destroyObject(root);
    free(root);
    root = NULL;
}

void destroyObject(ObjectAST* obj) {
    if (obj->value != NULL) {
        destroyValue(obj->value);
        free(obj->value);
        obj->value = NULL;
    }

    if (obj->nextPair != NULL) {
        destroyNextPair(obj->nextPair);
        free(obj->nextPair);
        obj->nextPair = NULL;
    }
}

void destroyArray(ArrayAST* array) {
    if (array->value != NULL) {
        destroyValue(array->value);
        free(array->value);
        array->value = NULL;
    }

    if (array->nextValue != NULL) {
        destroyNextValue(array->nextValue);
        free(array->nextValue);
        array->nextValue = NULL;
    }
}

void destroyNextPair(NextPairAST* nextPair) {
    if (nextPair->value != NULL) {
        destroyValue(nextPair->value);
        free(nextPair->value);
        nextPair->value = NULL;
    }

    if (nextPair->nextPair != NULL) {
        destroyNextPair(nextPair->nextPair);
        free(nextPair->nextPair);
        nextPair->nextPair = NULL;
    }
}

void destroyValue(ValueAST* value) {
    if (value->obj != NULL) {
        destroyObject(value->obj);
        free(value->obj);
        value->obj = NULL;
    }
    if (value->array != NULL) {
        destroyArray(value->array);
        free(value->array);
        value->array = NULL;
    }
}

void destroyNextValue(NextValueAST* nextValue) {
    if (nextValue->value != NULL) {
        destroyValue(nextValue->value);
        free(nextValue->value);
        nextValue->value = NULL;
    }

    if (nextValue->nextValue != NULL) {
        destroyNextValue(nextValue->nextValue);
        free(nextValue->nextValue);
        nextValue->nextValue = NULL;
    }
}
