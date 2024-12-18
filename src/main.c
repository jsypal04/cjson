#include "lexer.h"

int main() {
    SourceLexState state = initLexer("source.json");

    printf("Lexeme: %s\n", state.lexeme);
    while (state.nextChar != EOF) {
        lex(&state);

        printf("---Lexeme---\n");
        printf("Lexeme: %s\n", state.lexeme);
        printf("Token: %d\n", state.token);
        printf("------------\n\n");
    }

    exit(0);

}
