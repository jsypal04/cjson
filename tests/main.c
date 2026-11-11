#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "cjson.h"
#include "lex.h"
#include "utils.h"
#include "testify.h"

TEST(test_stripw) {
    char str[32] = "hello, world";
    printf("%s\n", str);
    stripw(str, strlen(str));
    printf("%s\n", str);
    if (strcmp(str, "hello,world") != 0) {
        return FAILED;
    }
    return PASSED;
}

TEST(test_stripw_escape_whites) {
    char str[32] = " hel\rlo,\t world\n";
    printf("%s\n", str);
    stripw(str, strlen(str));
    printf("%s\n", str);
    if (strcmp(str, "hello,world") != 0)
        return FAILED;
    return PASSED;
}

TEST(test_stripw_empty) {
    char str[8] = "";
    stripw(str, strlen(str));
    if (strcmp(str, "") != 0)
        return FAILED;
    return PASSED;
}

TEST(test_stripw_only_whites) {
    char str[8] = " \n\t   \r";
    stripw(str, strlen(str));
    if (strcmp(str, "") != 0)
        return FAILED;
    return PASSED;
}

TEST(test_EXP_BUFF) {
    char* buffer = malloc(sizeof(char) * 10);
    memset(buffer, 'a', sizeof(char) * 10);
    size_t ok;
    size_t n = 10;
    EXP_BUFF(buffer, n, ok);
    EXP_BUFF(buffer, n, ok);

    if (strcmp(buffer, "aaaaaaaaaa") != 0) {
        free(buffer);
        return FAILED;
    }

    if (n != 40) {
        free(buffer);
        return FAILED;
    }

    free(buffer);
    return PASSED;
}

TEST(test_EXP_BUFF_CJ_KvPair) {
    size_t ok;
    size_t cap = 10;
    CJ_KvPair* o = malloc(sizeof(CJ_KvPair) * cap);

    size_t tmp_cap = cap;
    EXP_BUFF(o, cap, ok);
    EXP_BUFF(o, cap, ok);
    if (ok != 0 || cap != 4 * tmp_cap) {
        free(o);
        return FAILED;
    }

    free(o);
    return PASSED;
}

/**
 * Tests for value.c
 */

TEST(test_putarr) {
    int result = FAILED;

    CJ_Value arr = {0};
    CJ_Value num = {0};
    cj_num_init(&num, 1.3);
    cj_arr_init(&arr);
    cj_putarr(&arr, num);
    printf("len = %zu\n", arr.val.arr.len);
    printf("num = %f\n", arr.val.arr.a[0].val.num);
    if (arr.val.arr.len == 1 && arr.val.arr.a[0].val.num == 1.3) {
        result = PASSED;
    }

    cj_free_value(&num);
    cj_free_value(&arr);
    return result;
}

TEST(test_cj_free_value_obj_twice) {
    CJ_Value obj = {0};
    CJ_Value val = {0};
    cj_obj_init(&obj);
    cj_str_init(&val, "hello, world!");
    cj_putobj(&obj, "field", val);
    cj_free_value(&obj);
    cj_free_value(&obj);
    if (obj.val.obj.o != NULL) {
        return FAILED;
    }
    return PASSED;
}

/**
 * TESTS FOR lex.c
 */

// This is a problem
TEST(test_lex_two_error_double_free) {
    char json[16] = "{+";

    CJ_Src src = {0};
    cj_open(&src, json, strlen(json));
    cj_next_token(&src);
    cj_close(&src);
    return PASSED;
}

TEST(test_cj_parse_string_long_str) {
    char str[3039] = "\"hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\"";

    CJ_Src src = {0};
    cj_open(&src, str, strlen(str));

    printf("tok = %d\n", cj_peek_token(&src));
    if (cj_peek_token(&src) != TOK_STR) {
        cj_close(&src);
        return FAILED;
    }

    cj_close(&src);
    return PASSED;
}

TEST(test_cj_next_token) {
    CJ_Src src = {0};
    char json[16] = "{\"hello\"";

    int8_t r = cj_open(&src, json, strlen(json));
    if (r != 0) {
        cj_close(&src);
        return FAILED;
    }

    if (cj_peek_token(&src) != TOK_LBRACE) {
        cj_close(&src);
        return FAILED;
    }

    cj_next_token(&src);

    if (cj_peek_token(&src) != TOK_STR) {
        cj_close(&src);
        return FAILED;
    }

    cj_close(&src);
    return PASSED;
}

TEST(test_cj_next_token_empty) {
    CJ_Src src = {0};
    char json[16] = "";
    int8_t r = cj_open(&src, json, strlen(json));
    if (r != 0) {
        cj_close(&src);
        return FAILED;
    }
    if (cj_peek_token(&src) != TOK_EOF) {
        cj_close(&src);
        return FAILED;
    }
    cj_close(&src);
    return PASSED;
}

TEST(test_cj_next_token_invalid_num) {
    CJ_Src src = {0};
    char json[16] = "123abc";
    int8_t r = cj_open(&src, json, strlen(json));
    if (r != 0) {
        cj_close(&src);
        return FAILED;
    }
    if (cj_peek_token(&src) != TOK_NUM) {
        cj_close(&src);
        return FAILED;
    }

    cj_next_token(&src);
    if (cj_peek_token(&src) != TOK_ID) {
        cj_close(&src);
        return FAILED;
    }

    cj_close(&src);
    return PASSED;
}

TEST(test_cj_next_token_invalid) {
    CJ_Src src = {0};
    char json[16] = "+##$";
    int8_t r = cj_open(&src, json, strlen(json));
    if (r == 0) {
        cj_close(&src);
        return FAILED;
    }

    cj_close(&src);
    return PASSED;
}

TEST(test_lex_unterminated_str) {
    char json[32] = "\"this is unterminated";
    CJ_Src src = {0};
    int8_t r = cj_open(&src, json, strlen(json));
    if (r == 0) {
        cj_close(&src);
        return FAILED;
    }

    cj_close(&src);
    return PASSED;
}

TEST(test_lex_esc_quotes_in_str) {
    char json[32] = "\"this is a\\\" string\"";
    CJ_Src src = {0};
    cj_open(&src, json, strlen(json));
    printf("lex = '%s'\n", cj_peek_lex(&src));
    if (cj_peek_token(&src) != TOK_STR || strcmp(cj_peek_lex(&src), "this is a\\\" string") != 0) {
        cj_close(&src);
        return FAILED;
    }
    cj_close(&src);
    return PASSED;
}

TEST(test_lex_negative_num) {
    char json[32] = "-32";
    CJ_Src src = {0};
    int8_t r = cj_open(&src, json, strlen(json));
    if (r != 0) {
        cj_close(&src);
        return FAILED;
    }
    printf("lex = '%s'\n", cj_peek_lex(&src));
    if (cj_peek_token(&src) != TOK_NUM || strcmp(cj_peek_lex(&src), "-32")) {
        cj_close(&src);
        return FAILED;
    }
    cj_close(&src);
    return PASSED;
}

TEST(test_lex_multiple_radix) {
    char json[32] = "32.0.3.1";
    CJ_Src src = {0};
    int8_t r = cj_open(&src, json, strlen(json));
    if (
        r != 0 ||
        cj_peek_token(&src) != TOK_NUM ||
        strcmp(cj_peek_lex(&src), "32.0") != 0
    ) {
        cj_close(&src);
        return FAILED;
    }
    r = cj_next_token(&src);
    if (r == 0) {
        cj_close(&src);
        return FAILED;
    }

    cj_close(&src);
    return PASSED;
}

void clone_src(CJ_Src* dest, CJ_Src src);
TEST(test_lex_clone_src) {
    char json[16] = "{\"key\":";
    CJ_Src src = {0};
    CJ_Src src_clone = {0};
    cj_open(&src, json, strlen(json));
    clone_src(&src_clone, src);

    bool ok =
        src.p == src_clone.p &&
        src.src_len == src_clone.src_len &&
        src.src != src_clone.src &&
        strcmp(src.src, src_clone.src) == 0 &&
        src.cur_tok.token == src_clone.cur_tok.token &&
        src.cur_tok.lex_cap == src_clone.cur_tok.lex_cap &&
        src.cur_tok.lex_len == src_clone.cur_tok.lex_len &&
        src.cur_tok.lexeme != src_clone.cur_tok.lexeme &&
        strcmp(src.cur_tok.lexeme, src_clone.cur_tok.lexeme) == 0;

    cj_close(&src);
    cj_close(&src_clone);
    return ok ? PASSED : FAILED;
}

/**
 * Test for dump.c
 */

TEST(test_cj_dump_100_fields) {
    CJ_Value val = {0};
    cj_obj_init(&val);
    for (int i = 0; i < 100; i++) {
        char key[32] = "\0";
        sprintf(key, "field_%d", i);
        CJ_Value item = {0};
        cj_num_init(&item, (double) i);
        cj_putobj(&val, key, item);
    }
    char* result = cj_dump(val);
    printf("%s\n", result);
    cj_free_value(&val);
    return PASSED;
}

TEST(test_cj_dump_num_keeps_precision) {
    double n = 1.22323423432234231231;

    CJ_Value obj = {0};
    CJ_Value num = {0};
    cj_obj_init(&obj);
    cj_num_init(&num, n);
    cj_putobj(&obj, "key", num);
    char* json = cj_dump(obj);
    cj_free_value(&obj);
    cj_free_value(&num);
    cj_parse(&obj, json, strlen(json));
    cj_getobj(obj, "key", &num);
    if (num.tag != CJ_NUM) {
        cj_free_value(&obj);
        free(json);
        return FAILED;
    }
    printf("num = %f\n", num.val.num);
    if (n != num.val.num) {
        cj_free_value(&obj);
        free(json);
        return FAILED;
    }

    cj_free_value(&obj);
    free(json);
    return PASSED;

}

/**
 * E2E TESTS
 */

TEST(test_e2e_trailing_garbage) {
    char json[512] = "{\"field\":[1, 2, 3, 4],\"field2\":true}ofdbvdfbkfbkd";
    CJ_Value val = {0};
    int8_t r = cj_parse(&val, json, strlen(json));
    if (r == 0) {
        cj_free_value(&val);
        return FAILED;
    }

    cj_free_value(&val);
    return PASSED;
}

TEST(test_e2e_obj_no_commas) {
    char json[128] = "{\"key\": 1 \"key2\": 2 \"key3\": 3}";
    CJ_Value val = {0};
    int8_t r = cj_parse(&val, json, strlen(json));
    cj_free_value(&val);
    return r == 0 ? FAILED : PASSED;
}

TEST(test_e2e_obj_trailing_comma) {
    char json[128] = "{\"key\": 1, \"key2\": 2, \"key3\": 3,}";
    CJ_Value val = {0};
    int8_t r = cj_parse(&val, json, strlen(json));
    cj_free_value(&val);
    return r == 0 ? FAILED : PASSED;
}

TEST(test_e2e_arr_no_commas) {
    char json[128] = "{\"key\": [1 2 3 4]}";
    CJ_Value val = {0};
    int8_t r = cj_parse(&val, json, strlen(json));
    cj_free_value(&val);
    return r == 0 ? FAILED : PASSED;
}

TEST(test_e2e_arr_trailing_comma) {
    char json[128] = "{\"key\": [1, 2, 3, 4,]}";
    CJ_Value val = {0};
    int8_t r = cj_parse(&val, json, strlen(json));
    cj_free_value(&val);
    return r == 0 ? FAILED : PASSED;
}

TEST(test_e2e_array) {
    char json[512] = "{\"field\":[1, 2, 3, 4],\"field2\":true}";
    CJ_Value val = {0};
    cj_parse(&val, json, strlen(json));

    if (val.val.obj.o[0].value.tag != CJ_ARR) {
        cj_free_value(&val);
        return FAILED;
    }

    cj_free_value(&val);
    return PASSED;
}

TEST(test_e2e_fat_flat) {
    char path[32] = "json/fat_flat.json";
    CJ_Value val = {0};
    int8_t r = cj_parsef(&val, path);
    if (r != 0) {
        cj_free_value(&val);
        printf("Failed to parse json string.\n");
        return FAILED;
    }
    // TODO: add more rigourous checks here.
    cj_free_value(&val);
    return PASSED;
}

TEST(test_e2e_thin_flat) {
    char json[5012] = "{\"name\":\"Joseph Sypal\",\"age\":22,\"registered_to_vote\":true,\"likes_cheese\":null}";

    CJ_Value val = {0};
    int8_t r = cj_parse(&val, json, strlen(json));
    if (r != 0) {
        printf("Failed to parse the json string.\n");
        return FAILED;
    }
    char* rountrip = cj_dump(val);
    if (rountrip == NULL) {
        printf("Serialization failed.\n");
        return FAILED;
    }
    if (strcmp(json, rountrip) != 0) {
        printf("Serialized value does match json input.\n");
        printf("Serialized value:\n%s\n\n", rountrip);
        printf("Input json:\n%s\n\n", json);
        return FAILED;
    }
    return PASSED;
}

TEST(test_stress_thin_deep) {
    const char path[32] = "json/thin_deep.json";
    CJ_Value val = {0};
    int8_t r = cj_parsef(&val, path);
    if (r != 0) {
        printf("Failed to parse initial file.\n");
        cj_free_value(&val);
        return FAILED;
    }

    for (int i = 0; i < 10000; i++) {
        char* json = cj_dump(val);
        cj_free_value(&val);
        if (json == NULL) {
            printf("Failed to serialize json.\n");
            return FAILED;
        }
        int8_t r = cj_parse(&val, json, strlen(json));
        free(json);
        json = NULL;
        if (r != 0) {
            printf("Failed to parse serialized json.\n");
            cj_free_value(&val);
            return FAILED;
        }
    }

    cj_free_value(&val);
    return PASSED;
}

int main(int argc, char** argv) {
    return testify_run(argc, argv);
}
