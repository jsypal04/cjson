#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

// Going to want to test this function out a bunch
int8_t exp_buff(void** p, size_t* n) {
    if (p == NULL || n == NULL || *n > SIZE_MAX / 2) {
        return -1;
    }

    const size_t new_n = (*n) * 2;

    void* tmp = realloc(*p, new_n);
    if (tmp == NULL && new_n != 0) {
        return -1;
    }
    memset((unsigned char*)tmp + *n, 0, new_n - *n);

    *p = tmp;
    *n = new_n;
    return 0;
}

// Remove all whitespace from the given string in place.
void stripw(char* str, size_t n) {
    size_t i;
    size_t j;

    j = 0;
    for (i = 0; i < n; i++) {
        if (isspace(str[i])) continue;
        if (i == j) {
            j++;
            continue;
        }

        str[j] = str[i];
        j++;
    }

    if (n - j > 0)
        bzero(str + j, n - j);
}
