#ifndef CJSON_H
#define CJSON_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_STR_LEN 10000

// forward AST type definitions
typedef struct ObjectAST ObjectAST;
typedef struct NextPairAST NextPairAST;
typedef struct ValueAST ValueAST;
typedef struct NextValueAST NextValueAST;
typedef struct ArrayAST ArrayAST;

typedef enum Token {
    END,
    NONE,
    STR,
    NUM,
    BOOL,

    LBRACE,
    RBRACE,
    LBRACKET,
    RBRACKET,
    COLON,
    COMMA
} Token;

/************************
AST TYPES

The types for each node of the AST for the parser are defined below
************************/
typedef struct SourceLexState {
    FILE* source;
    char nextChar;
    Token token;
    char* lexeme;
} SourceLexState;

// only one field should be populated, all else null
struct ValueAST {
    char* lexeme;
    ArrayAST* array;
    ObjectAST* obj;
};

struct NextValueAST {
    ValueAST* value;
    NextValueAST* nextValue;
};

struct NextPairAST {
    char* key;
    ValueAST* value;
    NextPairAST* nextPair;
};

struct ArrayAST {
    ValueAST* value;
    NextValueAST* nextValue;
};

struct ObjectAST {
    char* key;
    ValueAST* value;
    NextPairAST* nextPair;
};


SourceLexState initLexer(const char* sourcePath);
void lex(SourceLexState* statePtr);

/*
AST FUNCTIONS

The functions for the recursive descent descent parser are defined here. There is
one for each node plus a bonus one at the beginning. All functions should return with
state containing the next lexeme to be processed.
*/

ObjectAST* parse(const char* sourcePath);
ObjectAST* parseObject(SourceLexState* state);
ArrayAST* parseArray(SourceLexState* state);
NextPairAST* parseNextPair(SourceLexState* state);
ValueAST* parseValue(SourceLexState* state);
NextValueAST* parseNextValue(SourceLexState* state);

#endif
