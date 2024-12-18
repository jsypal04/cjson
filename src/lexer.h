#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_STR_LEN 10000

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

typedef struct SourceLexState {
    FILE* source;
    char nextChar;
    Token token;
    char* lexeme;

} SourceLexState;

SourceLexState initLexer(const char* sourcePath);

void lex(SourceLexState* statePtr);

#endif
