#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <stdint.h>

#define EXP_BUFF(ptr, n, ok) \
    do { \
        void* exp_tmp_ = (ptr); \
        (ok) = exp_buff(&exp_tmp_, &(n)); \
        if (ok) \
            (ptr) = exp_tmp_; \
    } while (0);

int8_t exp_buff(void** p, size_t* n);

#endif
