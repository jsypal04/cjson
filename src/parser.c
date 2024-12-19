#include "cjson.h"
#include <stdio.h>

ObjectAST* parse(const char* sourcePath) {
    SourceLexState state = initLexer(sourcePath);
    ObjectAST* root = parseObject(&state);
    fclose(state.source);
    return root;
}
