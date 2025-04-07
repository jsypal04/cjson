#ifndef CJSON_H
#define CJSON_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_LEX_LEN 10000

// Token defintions
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

// lexer state type defintion
typedef struct SourceLexState {
    FILE* source;
    char  nextChar;
    Token token;
    char* lexeme;
} SourceLexState;

/*
Lexer Functions
*/
SourceLexState initLexer(const char* sourcePath);
void lex(SourceLexState* statePtr);



/************************
AST TYPES

The types for each node of the AST for the parser are defined below
************************/

// forward AST type definitions
typedef struct ObjectAST    ObjectAST;
typedef struct NextPairAST  NextPairAST;
typedef struct ValueAST     ValueAST;
typedef struct NextValueAST NextValueAST;
typedef struct ArrayAST     ArrayAST;

// only one field should be populated, all else null
struct ValueAST {
    char*      lexeme;
    ArrayAST*  array;
    ObjectAST* obj;
};

struct NextValueAST {
    ValueAST*     value;
    NextValueAST* nextValue;
};

struct NextPairAST {
    char*        key;
    ValueAST*    value;
    NextPairAST* nextPair;
};

struct ArrayAST {
    ValueAST*     value;
    NextValueAST* nextValue;
};

struct ObjectAST {
    char*        key;
    ValueAST*    value;
    NextPairAST* nextPair;
};

/*
AST/Parser FUNCTIONS (naturally we use a recursive descent parser like a boss)

The functions for the recursive descent descent parser are defined here. There is
one for each node plus a bonus one at the beginning. All functions should return with
state containing the next lexeme to be processed.
*/

ObjectAST*    parse(const char* sourcePath);
ObjectAST*    parseObject(SourceLexState* state);
ArrayAST*     parseArray(SourceLexState* state);
NextPairAST*  parseNextPair(SourceLexState* state);
ValueAST*     parseValue(SourceLexState* state);
NextValueAST* parseNextValue(SourceLexState* state);

// destructor for the AST
void destroyAST(ObjectAST* root);
void destroyObject(ObjectAST* obj);
void destroyArray(ArrayAST* array);
void destroyNextPair(NextPairAST* nextPair);
void destroyValue(ValueAST* value);
void destroyNextValue(NextValueAST* nextValue);


/*
 * Data Structure to store the JSON data (An ordered map)
 */
#define STRING 's'
#define INT    'i'
#define FLOAT  'f'
#define BOOL   'b'
#define ARRAY  'a'
#define MAP    'm'

typedef struct {
    char* key;
    void* value;
    char type; // can be either s (string), i (int), f (float), b (bool), a (array), m (map)
} KeyValuePair;

typedef struct {
    KeyValuePair* pairs;
    int size;
    int mapCap;
} Map;

// Function to allocate the necessary heap memory for a map with a given initial capacity
Map* initMap(int initMapCap);

/* Function to free all allocated memory used to create the map 
 *
 * PLEASE NOTE: This function does NOT free memory of the values stored in the in the map.
 *  you must go through the map destroying any values that you allocated and then stored in the map.
 * */
void destroyMap(Map* map);

/*
 * Duplicates `source` and returns a pointer to the new map. 
 * - `source` is not destroyed, the caller still needs to destroy it.
 * - NOTE: If a nested map is encountered, need to use recursion.
 * */
Map* mapdup(Map* source);

// Functions to insert a value with a given key and type into the map
void insertInt(   Map** map_ref, char* key, int   value);
void insertFloat( Map** map_ref, char* key, float value);
void insertString(Map** map_ref, char* key, char* value);
void insertMap(   Map** map_ref, char* key, Map*  value);

/* Function to retrieve the value mapped to a given key 
 *
 * The `type` parameter is the address where the type of the retrieved value will be stored.
 * You must use it to cast the void* to the correct type after the value has been retrieved.
 * */
void* get(Map* map, char* key, char* type);

void printMap(Map* map);


/* 
 * Functions to load json parsed by the parser into a Map
 * */
Map* load_file(const char* path);

#endif
