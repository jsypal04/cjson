#include "cjson.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#undef malloc
#undef free

#define MEMORY_FREE_LOG "/home/sypalj/cjson/logs/memory_free.log"


void* debug_malloc(size_t size, const char* filename, int line_number) {
    printf("Running debug malloc\n");
    return malloc(size);
}

void debug_free(void *ptr, const char *filename, int line_number) {
    printf("Running debug free\n");

    time_t now = time(NULL);
    const char* timestamp = asctime(localtime(&now));

    FILE* logfile = fopen(MEMORY_FREE_LOG, "a");
    if (logfile == NULL) {
        printf("ERROR: Failed to open logs/memory_free.log\n");
        free(ptr);
        return;
    }

    if (ptr == NULL) {
        // fprintf(logfile, "[%s] %s:%d Attempted to free a NULL pointer", timestamp, filename, line_number);
        fclose(logfile);
        return;
    }

    // fprintf(logfile, "[%s] %s:%d Freeing memory at", timestamp, filename, line_number);
    free(ptr);
    fclose(logfile);
}
