// basic syntax lexing test
// this test uses the lexTest1.json source file
#include <stdlib.h>
#include "../../src/cjson.h"

#define TOKEN_LEN 45

short failed = 0;

Token correctTokens[TOKEN_LEN];
Token generatedTokens[TOKEN_LEN];

void populateCorrectTokens() {
    correctTokens[0] = LBRACE;
    correctTokens[1] = STR; // "main"
    correctTokens[2] = COLON;
    correctTokens[3] = LBRACKET;
    correctTokens[4] = LBRACE;
    correctTokens[5] = STR; // "id"
    correctTokens[6] = COLON;
    correctTokens[7] = NUM; // 0
    correctTokens[8] = COMMA;
    correctTokens[9] = STR; // "inStock"
    correctTokens[10] = COLON;
    correctTokens[11] = BOOL; // true
    correctTokens[12] = RBRACE;
    correctTokens[13] = COMMA;
    correctTokens[14] = LBRACE;
    correctTokens[15] = STR; // "id"
    correctTokens[16] = COLON;
    correctTokens[17] = NUM; // 1.34
    correctTokens[18] = COMMA;
    correctTokens[19] = STR; // "inStock"
    correctTokens[20] = COLON;
    correctTokens[21] = BOOL; // false
    correctTokens[22] = RBRACE;
    correctTokens[23] = RBRACKET;
    correctTokens[24] = COMMA;
    correctTokens[25] = STR; // "mainVendor"
    correctTokens[26] = COLON;
    correctTokens[27] = NONE;
    correctTokens[28] = COMMA;
    correctTokens[29] = STR; // "CEO"
    correctTokens[30] = COLON;
    correctTokens[31] = LBRACE;
    correctTokens[32] = STR; // "firstName"
    correctTokens[33] = COLON;
    correctTokens[34] = STR; // "John"
    correctTokens[35] = COMMA;
    correctTokens[36] = STR; // "lastName"
    correctTokens[37] = COLON ;
    correctTokens[38] = STR; // "Doe"
    correctTokens[39] = COMMA;
    correctTokens[40] = STR; // "age"
    correctTokens[41] = COLON;
    correctTokens[42] = NUM; // -34.28
    correctTokens[43] = RBRACE;
    correctTokens[44] = RBRACE;
}

int main(int argc, char* argv[]) {
    populateCorrectTokens();

    char* jsonSrc = argv[1];

    FILE* src_stream = fopen(jsonSrc, "r");
    char source[MAX_FILE_SIZE_B] = "\0";
    uint32_t src_idx = 0;
    char c = getc(src_stream);
    while (c != EOF) {
        source[src_idx] = c;
        src_idx++;
        c = getc(src_stream);
    }
    source[src_idx] = '\0';
    fclose(src_stream);

    SourceLexState state = initLexer(source);

    int index = 0;
    while (state.nextChar != EOF) {
        if (index >= TOKEN_LEN) {
            printf("ERROR: lexer test 1 (basic syntax test) failed. Lexer output too many tokens %d/%d.\n", index, TOKEN_LEN);
            failed = 1;
            break;
        }

        // printf("%d. token: %d, lexeme: %s\n", index, state.token, state.lexeme);
        generatedTokens[index] = state.token;
        index++;

        lex(&state);

        if (state.token == END) {
            break;
        }
    }

    for (int i = 0; i < TOKEN_LEN; i++) {
        if (generatedTokens[i] != correctTokens[i]) {
            printf("ERROR: lexer test 1 (basic syntax test) failed. Token (%d) output in position %d is incorrect (correct token: %d).\n", generatedTokens[i], i, correctTokens[i]);
            failed = 1;
        }
    }

    if (!failed) {
        printf("SUCCESS: lexer test 1 passed.\n");
        exit(0);
    }

    exit(1);

}
