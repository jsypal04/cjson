#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "lex.h"

void set_single_char_tok(CJ_Token* tok, char lex, CJ_TokenTag tag) {
    char lex_buff[2] = "\0";
    lex_buff[0] = lex;

    strncpy(tok->lexeme, lex_buff, tok->lex_cap);
    tok->lex_len++;

    tok->token = tag;
}

int8_t parse_string(CJ_Src** src_p, CJ_Token* tok) {
    CJ_Src* src = *src_p;

    if (src->src[src->p] != '"') return -1;

    src->p++;
    uint32_t str_start = src->p;
    while (src->src[src->p] != '"') {
        char ch = src->src[src->p];
        src->p += (ch == '\\' ? 2 : 1);
        if (src->p > src->src_len) {
            return -1;
        }
    }

    tok->lex_len = src->p - str_start;
    while (tok->lex_len >= tok->lex_cap) {
        int8_t ok;
        EXP_BUFF(tok->lexeme, tok->lex_cap, ok);
        if (ok != 0) return -1;
    }
    strncpy(tok->lexeme, src->src + str_start, tok->lex_len);
    tok->token = TOK_STR;
    src->p++;
    return 0;
}

int8_t parse_num_or_id(CJ_Src** src_p, CJ_Token* tok) {
    CJ_Src* src = *src_p;

    uint32_t start = src->p;

    if (isalpha(src->src[src->p])) {
        tok->token = TOK_ID;
        while (isalnum(src->src[src->p])) {
            src->p++;
            if (src->p > src->src_len) {
                return -1;
            }
        }
    } else if (isdigit(src->src[src->p]) || src->src[src->p] == '-') {
        tok->token = TOK_NUM;
        bool has_radix = false;
        src->p++;
        while (isdigit(src->src[src->p]) || (src->src[src->p] == '.' && !has_radix)) {
            if (src->src[src->p] == '.') {
                has_radix = true;
            }
            src->p++;
            if (src->p > src->src_len) {
                return -1;
            }
        }
    } else {
        return -1;
    }

    tok->lex_len = src->p - start;
    while (tok->lex_len >= tok->lex_cap) {
        int8_t ok;
        EXP_BUFF(tok->lexeme, tok->lex_cap, ok);
        if (ok != 0) return -1;
    }
    strncpy(tok->lexeme, src->src + start, tok->lex_len);
    return 0;
}

int8_t cj_open(CJ_Src *src, char *json, size_t n) {
    src->src = strndup(json, n);
    src->src_len = n;
    src->p = 0;

    CJ_Token t = {0};
    src->cur_tok = t;

    return cj_next_token(src);
}

int8_t cj_close(CJ_Src *src) {
    if (src == NULL) return -1;

    if (src->cur_tok.lexeme != NULL) {
        free(src->cur_tok.lexeme);
        src->cur_tok.lexeme = NULL;
    }

    if (src->src != NULL) {
        free(src->src);
        src->src = NULL;
    }

    memset(src, 0, sizeof *src);
    return 0;
}

int8_t cj_next_token(CJ_Src *src) {
    CJ_Token tok = src->cur_tok;

    if (tok.lexeme != NULL) {
        free(tok.lexeme);
        tok.lexeme = NULL;
        src->cur_tok.lexeme = NULL;
    }

    while (isspace(src->src[src->p])) {
        src->p++;
        if (src->p > src->src_len) {
            return -1;
        }
    }

    if (src->p >= src->src_len) {
        tok.token = TOK_EOF;
        src->cur_tok = tok;
        return 0;
    }

    tok.lex_cap = 1024;
    tok.lexeme = malloc(sizeof(char) * tok.lex_cap);
    bzero(tok.lexeme, tok.lex_cap);

    switch (src->src[src->p]) {
        case '{':
            set_single_char_tok(&tok, '{', TOK_LBRACE);
            src->p++;
            break;

        case '}':
            set_single_char_tok(&tok, '}', TOK_RBRACE);
            src->p++;
            break;

        case '[':
            set_single_char_tok(&tok, '[', TOK_LBRACK);
            src->p++;
            break;

        case ']':
            set_single_char_tok(&tok, ']', TOK_RBRACK);
            src->p++;
            break;

        case ':':
            set_single_char_tok(&tok, ':', TOK_COLON);
            src->p++;
            break;

        case ',':
            set_single_char_tok(&tok, ',', TOK_COMMA);
            src->p++;
            break;

        case '"':
            if (parse_string(&src, &tok) != 0) {
                free(tok.lexeme);
                return -1;
            }
            break;

        default:
            if (parse_num_or_id(&src, &tok) != 0) {
                free(tok.lexeme);
                return -1;
            }
            break;
    }

    src->cur_tok = tok;

    return 0;
}

void clone_src(CJ_Src* dest, CJ_Src src) {
    memset(dest, 0, sizeof *dest);

    dest->src = strndup(src.src, src.src_len);
    dest->src_len = src.src_len;
    dest->p = src.p;

    dest->cur_tok.lex_cap = src.cur_tok.lex_cap;
    dest->cur_tok.lex_len = src.cur_tok.lex_len;
    dest->cur_tok.token = src.cur_tok.token;
    dest->cur_tok.lexeme = malloc(sizeof(char) * src.cur_tok.lex_cap);
    bzero(dest->cur_tok.lexeme, sizeof(char) * src.cur_tok.lex_cap);
    strncpy(dest->cur_tok.lexeme, src.cur_tok.lexeme, src.cur_tok.lex_cap);
}

CJ_TokenTag cj_peek_next_token(CJ_Src* src) {
    CJ_Src src_clone = {0};
    clone_src(&src_clone, *src);
    cj_next_token(&src_clone);

    CJ_TokenTag tag = cj_peek_token(&src_clone);
    cj_close(&src_clone);
    return tag;
}

CJ_TokenTag cj_peek_token(CJ_Src* src) {
    return src->cur_tok.token;
}

// DO NOT free the string that is returned. It is not yours.
char* cj_peek_lex(CJ_Src* src) {
    return src->cur_tok.lexeme;
}
