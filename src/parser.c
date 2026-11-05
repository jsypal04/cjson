#include <stdlib.h>
#include <string.h>

#include "cjson.h"
#include "lex.h"
#include "utils.h"

#define INIT_CAP 20

int8_t parse_obj(CJ_Value* dest, CJ_Src* src);

int8_t parse_arr(CJ_Value* dest, CJ_Src* src) {
    CJ_Arr arr = {0};
    arr.cap = INIT_CAP;
    arr.a = malloc(sizeof(CJ_Value) * arr.cap);

    cj_next_token(src);
    while (cj_peek_token(src) != TOK_RBRACK) {
        CJ_Value val = {0};
        switch (cj_peek_token(src)) {
            case TOK_STR:
                val.tag = CJ_STR;
                val.val.str = strdup(cj_peek_lex(src));
                break;
            case TOK_NUM:
                val.tag = CJ_NUM;
                val.val.num = atof(cj_peek_lex(src));
                break;
            case TOK_ID:
                if (strcmp(cj_peek_lex(src), "null") == 0) {
                    val.tag = CJ_NULL;
                } else if (strcmp(cj_peek_lex(src), "true") == 0) {
                    val.tag = CJ_BOOL;
                    val.val.b = true;
                } else if (strcmp(cj_peek_lex(src), "false") == 0) {
                    val.tag = CJ_BOOL;
                    val.val.b = false;
                } else {
                    // error
                    return 1;
                }
                break;
            case TOK_LBRACK:
                if (parse_arr(&val, src) != 0) {
                    // error
                    return 1;
                }
                break;
            case TOK_LBRACE:
                if (parse_obj(&val, src) != 0) {
                    // error
                    return 1;
                }
                break;
            default:
                // error
                return 1;
        }

        if (arr.len >= arr.cap) {
            int8_t ok;
            EXP_BUFF(arr.a, arr.cap, ok);
            if (ok != 0) {
                // error
                return 1;
            }
        }

        arr.a[arr.len] = val;
        arr.len++;
    }

    dest->tag = CJ_ARR;
    dest->val.arr = arr;

    return 0;
}

int8_t parse_obj(CJ_Value* dest, CJ_Src* src) {
    CJ_Obj obj = {0};
    obj.cap = INIT_CAP;
    obj.o = malloc(sizeof(CJ_KvPair) * obj.cap);

    cj_next_token(src);
    while (cj_peek_token(src) != TOK_RBRACE) {
        if (cj_peek_token(src) != TOK_STR) {
            // error
            return 1;
        }

        CJ_KvPair pair = {0};
        pair.key = strdup(cj_peek_lex(src));

        cj_next_token(src);
        if (cj_peek_token(src) != TOK_COLON) {
            // error
            return 1;
        }
        cj_next_token(src);

        switch (cj_peek_token(src)) {
            case TOK_STR:
                pair.value.tag = CJ_STR;
                pair.value.val.str = strdup(cj_peek_lex(src));
                break;
            case TOK_NUM:
                pair.value.tag = CJ_NUM;
                pair.value.val.num = atof(cj_peek_lex(src));
                break;
            case TOK_ID:
                if (strcmp(cj_peek_lex(src), "null") == 0) {
                    pair.value.tag = CJ_NULL;
                } else if (strcmp(cj_peek_lex(src), "true") == 0) {
                    pair.value.tag = CJ_BOOL;
                    pair.value.val.b = true;
                } else if (strcmp(cj_peek_lex(src), "false") == 0) {
                    pair.value.tag = CJ_BOOL;
                    pair.value.val.b = false;
                } else {
                    // error
                    return 1;
                }
                break;
            case TOK_LBRACK:
                if (parse_arr(&pair.value, src) != 0) {
                    // error
                    return 1;
                }
                break;
            case TOK_LBRACE:
                if (parse_obj(&pair.value, src) != 0) {
                    // error
                    return 1;
                }
                break;
            default:
                // error
                return 1;
        }

        if (obj.len >= obj.cap) {
            int8_t ok;
            EXP_BUFF(obj.o, obj.cap, ok);
            if (ok != 0) {
                // error
                return 1;
            }
        }

        obj.o[obj.len] = pair;
        obj.len++;

        cj_next_token(src);
        if (cj_peek_token(src) == TOK_COMMA) {
            cj_next_token(src);
        }
    }

    dest->tag = CJ_OBJ;
    dest->val.obj = obj;

    return 0;
}

int8_t cj_parse(CJ_Value* dest, char* json, size_t n) {
    CJ_Src src = {0};
    cj_open(&src, json, n);

    if (cj_peek_token(&src) != TOK_LBRACE) {
        return 1;
    }

    int8_t r = parse_obj(dest, &src);

    if (r != 0) {
        return r;
    }

    cj_close(&src);

    return 0;
}
