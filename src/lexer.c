#include "lexer.h"
#include <complex.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

SourceLexState initLexer(const char* sourcePath) {
    SourceLexState initialState;
    initialState.source = fopen(sourcePath, "r");
    if (initialState.source == NULL) {
        printf("ERROR: source file not found.\n");
        exit(1);
    }

    initialState.nextChar = getc(initialState.source);

    if (initialState.nextChar != '{') {
        printf("ERROR: First character should be a left brace.\n");
        exit(1);
    }

    initialState.token = LBRACE;
    initialState.lexeme = "{";

    return initialState;
}

void lex(SourceLexState* state) {
    // fetch next character
    state->nextChar = getc(state->source);

    // printf("Beginning lex()...\n");
    // printf("\nInitial Value of state->lexeme: %s\n", state->lexeme);
    // printf("First Character Read: %c\n", state->nextChar);
    // printf("File Stream Pointer Loc: %ld\n\n", ftell(state->source));

    // if character is whitespace skip till non whitespace
    while (isspace(state->nextChar)) {
        // printf("%x\n", state->nextChar);
        state->nextChar = getc(state->source);
    }

    // check for EOF
    if (state->nextChar == EOF) {
        state->lexeme = "\0";
        state->token = END;
        return;
    }
    // if character is a quote enter string state
    if (state->nextChar == '"') {
        char lexemeBuf[MAX_STR_LEN] = "\0";

        // retrieve characters till another quote is reached
        int index = 0;
        lexemeBuf[index] = state->nextChar;
        index++;

        state->nextChar = getc(state->source);
        lexemeBuf[index] = state->nextChar;
        index++;

        while (state->nextChar != '"') {
            state->nextChar = getc(state->source);
            if (index < MAX_STR_LEN) {
                lexemeBuf[index] = state->nextChar;
                index++;
            }
            else {
                printf("ERROR: Maximum string length (10000) exceeded.\n");
                exit(1);
            }
        }
        // that string is the lexeme
        state->lexeme = lexemeBuf;
        // token is STR
        state->token = STR;
    }
    // if character is a number or dash enter number state
    else if (isdigit(state->nextChar) || state->nextChar == '-') {
        // fetch characters while character is numeric
        char lexemeBuf[MAX_STR_LEN] = "\0";

        // printf("Lexeme: %s\n", state->lexeme);
        // printf("Lexeme Buf: %s\n", lexemeBuf);

        int index = 0;
        lexemeBuf[index] = state->nextChar;
        index++;

        state->nextChar = getc(state->source);

        while (isdigit(state->nextChar)) {
            if (index < MAX_STR_LEN) {
                lexemeBuf[index] = state->nextChar;
                index++;
            }
            else {
                printf("ERROR: Maximum lexeme length (10000) exceeded.\n");
                exit(1);
            }
            state->nextChar = getc(state->source);
        }
        // check for period
        if (state->nextChar != '.') {
            state->token = NUM;
            state->lexeme = lexemeBuf;

            // move the file pointer back one byte
            fseek(state->source, -1, SEEK_CUR);
            return;
        }

        // adds the period
        if (index < MAX_STR_LEN) {
            lexemeBuf[index] = state->nextChar;
            index++;
        }
        else {
            printf("ERROR: Maximum lexeme length (10000) exceeded.\n");
            exit(1);
        }

        // fetch the fractional part
        state->nextChar = getc(state->source);
        if (!isdigit(state->nextChar)) {
            printf("ERROR: Invalid syntax (number must have fractional part)\n");
            exit(1);
        }
        while (isdigit(state->nextChar)) {
            if (index < MAX_STR_LEN) {
                lexemeBuf[index] = state->nextChar;
                index++;
            }
            else {
                printf("ERROR: Maximum lexeme length (10000) exceeded.\n");
                exit(1);
            }

            state->nextChar = getc(state->source);
        }

        // move the file pointer back one byte
        fseek(state->source, -1, SEEK_CUR);

        state->token = NUM;
        state->lexeme = lexemeBuf;
    }
    // if character is a letter enter boolean state
    else if (isalpha(state->nextChar)) {
        // fetch characters while they are letters
        char lexemeBuf[MAX_STR_LEN] = "\0";
        int index = 0;

        if (index < MAX_STR_LEN) {
            lexemeBuf[index] = state->nextChar;
            index++;
        }
        else {
            printf("ERROR: Maximum lexeme length (10000) exceeded.\n");
            exit(1);
        }

        state->nextChar = getc(state->source);
        while (isalpha(state->nextChar)) {
            if (index < MAX_STR_LEN) {
                lexemeBuf[index] = state->nextChar;
                index++;
            }
            else {
                printf("ERROR: Maximum lexeme length (10000) exceeded.\n");
                exit(1);
            }

            state->nextChar = getc(state->source);
        }

        // check that the word is true or false
        if (!strcmp("true", lexemeBuf) || !strcmp("false", lexemeBuf)) {
            state->lexeme = lexemeBuf;
            state->token = BOOL;
        }
        else if (!strcmp("null", lexemeBuf)) {
            state->lexeme = lexemeBuf;
            state->token = NONE;
        }
        else {
            printf("ERROR: The only identifiers allowed are true, false, and null\n");
            exit(1);
        }
        // move the file pointer back one byte
        fseek(state->source, -1, SEEK_CUR);
    }
    // if the character is a left brace return info for left braces
    else if (state->nextChar == '{') {
        state->lexeme = "{";
        state->token = LBRACE;
    }
    // if the character is a right brace return info for right braces
    else if (state->nextChar == '}') {
        state->lexeme = "}";
        state->token = RBRACE;
    }
    // if the character is a left bracket return info for left brackets
    else if (state->nextChar == '[') {
        state->lexeme = "[";
        state->token = LBRACKET;
    }
    // if the character is a right bracket return info for right brackets
    else if (state->nextChar == ']') {
        state->lexeme = "]";
        state->token = RBRACKET;
    }
    else if (state->nextChar == ':') {
        state->lexeme = ":";
        state->token = COLON;
    }
    else if (state->nextChar == ',') {
        state->lexeme = ",";
        state->token = COMMA;
    }
    else {
        printf("Unknown character: %c (hex: %x)\n", state->nextChar, state->nextChar);
    }
}
