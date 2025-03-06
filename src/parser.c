#include "cjson.h"
#include <stdlib.h>
#include <string.h>

/* Variable to keep track of the depth of the branch of the parse tree that
the parser is on. For debugging purposes. */
int astDepth = 0;
char keyboardInput;

ObjectAST* parse(const char* sourcePath) {
    printf("Begin parsing %s.\n", sourcePath);
//    scanf("%c", &keyboardInput);

    SourceLexState state = initLexer(sourcePath);
    ObjectAST* root = parseObject(&state);
    fclose(state.source);

    printf("End parsing %s.\n", sourcePath);
    return root;
}

ObjectAST* parseObject(SourceLexState* state) {
    printf("Parsing object (ast depth: %d)...\n", astDepth);
//    scanf("%c\n", &keyboardInput);
    
    if (state->token != LBRACE) {
        printf("MAYHEM: We in serious trouble now.\n");
        exit(0);
    }
    
    // first, create an empty object ast node 
    ObjectAST* obj = (ObjectAST*)malloc(sizeof(ObjectAST));
    
    // get the next lexeme (should be the first key of the json object)
    lex(state);
    // if the lexeme is not a string, it is an invalid key, print an error
    if (state->token != STR) {
        printf("ERROR: object key must be a string.\n");
        exit(1);
    }

    // otherwise, set the key attribute of obj to the key that was read
    obj->key = (char*)malloc(sizeof(char) * MAX_LEX_LEN); // We need to allocate memory to store the key in
    strcpy(obj->key, state->lexeme); // we need to copy the lexeme into the key char* so that they don't reference the same memory

    printf("    recorded key: %s\n", obj->key);
//    scanf("%c", &keyboardInput);
    // get the next lexeme (should be a colon)
    lex(state);
    // if the next lexeme is not a colon, it is invalid, print an error
    if (state->token != COLON) {
        printf("ERROR: object key must be followed by a colon.\n");
        exit(1);
    }

    // otherwise, parse the value
    astDepth++;
    ValueAST* value = parseValue(state);
    astDepth--;
    obj->value = value;

    lex(state);

    // there is another key-value pair to process
    if (state->token == COMMA) {
        astDepth++;
        NextPairAST* nextPair = parseNextPair(state);
        astDepth--;
        obj->nextPair = nextPair;

        return obj;
    }
    // end of object only one entry
    else if (state->token == RBRACE) {
        obj->nextPair = NULL;
        lex(state);
        return obj;
    }
    else {
        printf("ERROR: unknown symbol (parser.c:58) %s.\n", state->lexeme);
        exit(1);
    }
    printf("Done parsing object.\n");
}

ArrayAST* parseArray(SourceLexState* state) {
    printf("Parsing array (ast depth: %d)...\n", astDepth); 
//    scanf("%c", &keyboardInput);
    if (state->token != LBRACKET) {
        printf("MAYHEM: We in serious trouble now.\n");
        exit(0);
    }

    // create a new array object
    ArrayAST* array = (ArrayAST*)malloc(sizeof(ArrayAST));
    // parse the first value
    astDepth++;
    ValueAST* value = parseValue(state);
    astDepth--;

    array->value = value;

    // get the next unprocessed state
    lex(state);
    // if the lexeme is a comma, there is another value to parse
    if (state->token == COMMA) {
        // parse the value
        astDepth++;
        NextValueAST* nextValue = parseNextValue(state);
        astDepth--;
        array->nextValue = nextValue;
        printf("    state: %s\n", state->lexeme);
//        scanf("%c", &keyboardInput);
        return array;
    }
    else if (state->token == RBRACKET) {
        printf("    found the end the array.\n");
//        scanf("%c", &keyboardInput);
        array->nextValue = NULL;
        lex(state);
        return array;
    }
    else {
        printf("ERROR: unknown character (parser.c:107) %s.\n", state->lexeme);
        exit(1);
    }
    printf("Done parsing array.\n");
}

NextPairAST* parseNextPair(SourceLexState* state) {
    printf("Parsing next pair (ast depth: %d)...\n", astDepth);
//    scanf("%c", &keyboardInput);
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
    nextPair->key = (char*)malloc(sizeof(char) * MAX_LEX_LEN);
    strcpy(nextPair->key, state->lexeme);

    printf("    recorded key: %s\n", nextPair->key);
//    scanf("%c", &keyboardInput);
    lex(state);
    if (state->token != COLON) {
        printf("ERROR: object key must be followed by a colon.\n");
        exit(1);
    }
    astDepth++;
    ValueAST* value = parseValue(state);
    astDepth--;
    nextPair->value = value;
    
    lex(state);

    if (state->token == COMMA) {
        // another pair to parse
        astDepth++;
        NextPairAST* nextNextPair = parseNextPair(state);
        astDepth--;
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
    printf("Done parsing next pair.\n");
}

ValueAST* parseValue(SourceLexState* state) {
    printf("Parsing value (ast depth: %d)...\n", astDepth);
//    scanf("%c", &keyboardInput);
    // first, create an empty value object
    ValueAST* value = (ValueAST*)malloc(sizeof(ValueAST));
    printf("    state given to parseValue: %s\n", state->lexeme);
//    scanf("%c", &keyboardInput);
    
    // get the next lexeme (value assumes it receives an already processed state
    lex(state);
    printf("    first state retrieved by parseValue: %s\n", state->lexeme);
//    scanf("%c", &keyboardInput);
    // if the value is a primitive (str, num, bool, or null) simply set the value object's lexeme attribute to it
    if (state->token == STR || state->token == NUM || state->token == BOOL || state->token == NONE) {
        value->lexeme = (char*)malloc(sizeof(char) * MAX_LEX_LEN);
        strcpy(value->lexeme, state->lexeme);
        value->array = NULL;
        value->obj = NULL;
        // lex to get the next unprocessed state
//        lex(state);
        return value;
    }
    // if the lexeme is a left bracket, the value is an array
    else if (state->token == LBRACKET) {
        // parse the array
        astDepth++;
        ArrayAST* array = parseArray(state);
        astDepth--;
        value->array = array;
        value->lexeme = NULL;
        value->obj = NULL;
        return value;
    }
    // if the lexeme is a left bracket, the value is an object
    else if (state->token == LBRACE) {
        // parse the object 
        astDepth++;
        ObjectAST* obj = parseObject(state);
        astDepth--;
        value->obj = obj;
        value->lexeme = NULL;
        value->array = NULL;
        return value;
    }
    // otherwise, the value is unvalid
    else {
        // error
        printf("ERROR: Invalid value.\n");
        exit(1);
    }
    printf("Done parsing value.\n");
}

NextValueAST* parseNextValue(SourceLexState* state) {
    printf("Parsing next value (ast depth: %d)...\n", astDepth);
//    scanf("%c", &keyboardInput);
    if (state->token != COMMA) {
        printf("MAYHEM: We in serious trouble now.\n");
        exit(0);
    }
    
    // create an empty nextValue object
    NextValueAST* nextValue = (NextValueAST*)malloc(sizeof(NextPairAST));
    // parse the value
    astDepth++;
    ValueAST* value = parseValue(state);
    astDepth--;
    // set the value attribute to the parsed value
    nextValue->value = value;
    // get the next state
    lex(state);
    // if the lexeme is a comma, parse another value
    if (state->token == COMMA) {
        // parse another nextValue
        astDepth++;
        NextValueAST* nextNextValue = parseNextValue(state);
        astDepth--;
        nextValue->nextValue = nextNextValue;
        return nextValue;
    }
    // other wise return as is
    else {
        // return as is
        nextValue->nextValue = NULL;
        return nextValue;
    }
    printf("Done parsing next value.\n");
}

void destroyAST(ObjectAST* root) {
    destroyObject(root);
    free(root);
    root = NULL;
}

void destroyObject(ObjectAST* obj) {
    if (obj->key != NULL) {
        free(obj->key);
        obj->key = NULL;
    }

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
    if (nextPair->key != NULL) {
        free(nextPair->key);
        nextPair->key = NULL;
    }

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
    if (value->lexeme != NULL) {
        free(value->lexeme);
        value->lexeme = NULL;
    }

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
