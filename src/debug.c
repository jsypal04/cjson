#include "deserialization.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#undef malloc
#undef strdup
#undef free
#undef lex

#define MALLOC_ARR_SIZE 1000000

unsigned long allocations = 0;


char debug_input;

void* debug_malloc(size_t size, const char* filename, int line_number) {
    void* ptr = malloc(size); 
    allocations++;

    // printf("%s:%d, allocated pointer %p of size %zu (%lu allocations).\n", filename, line_number, ptr, size, allocations);
    // scanf("%c", &debug_input);

    return ptr;
}

char* debug_strdup(const char* str, const char* filename, int line_number)  {
    char* copy = strdup(str);
    allocations++;

    // printf("%s:%d, allocated string pointer %p (%lu allocations).\n", filename, line_number, copy, allocations);
    // scanf("%c", &debug_input);

    return copy;
}

void debug_free(void *ptr, const char *filename, int line_number) {

    free(ptr);
    allocations--;

    // printf("%s:%d, freed pointer %p (%lu allocations).\n", filename, line_number, ptr, allocations);
}

void debug_lex(SourceLexState* statePtr) {
    lex(statePtr);
    printf("Lexeme: %s\n", statePtr->lexeme);
}
