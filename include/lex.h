#ifndef LEX_H
#define LEX_H

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

typedef enum cj_tok_tag {
    TOK_EOF  = 0,
    TOK_NUM  = 1,
    TOK_STR  = 2,
    TOK_ID   = 3,

    TOK_LBRACK = 6,
    TOK_RBRACK = 7,
    TOK_LBRACE = 8,
    TOK_RBRACE = 9,
    TOK_COLON  = 10,
    TOK_COMMA  = 11
} CJ_TokenTag;

typedef struct cj_tokens {
    CJ_TokenTag token;
    size_t lex_len;
    size_t lex_cap;
    char* lexeme;
} CJ_Token;

typedef struct cj_src {
    char* src;
    size_t src_len;
    uint32_t p;
    CJ_Token cur_tok;
} CJ_Src;

int8_t cj_open(CJ_Src* src, char* json, size_t n);

int8_t cj_close(CJ_Src* src);

int8_t cj_next_token(CJ_Src* src);

CJ_TokenTag cj_peek_token(CJ_Src* src);
char*       cj_peek_lex(  CJ_Src* src);

#endif
