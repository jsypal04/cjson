#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
