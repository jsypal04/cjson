#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <stdint.h>

#define EXP_BUFF(ptr, n, ok) \
    do { \
        void* exp_tmp_ = (ptr); \
        size_t bytes_ = sizeof *(ptr) * (n); \
        (ok) = exp_buff(&exp_tmp_, &bytes_); \
        if ((ok) == 0) { \
            (ptr) = exp_tmp_; \
            (n) = bytes_ / (sizeof *(ptr)); \
        } \
    } while (0);

int8_t exp_buff(void** p, size_t* n);

void stripw(char* str, size_t n);

#endif
