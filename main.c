/**
 * This program serves as a dummy executable to link against the shared library
 * for testing.
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cjson.h"

int main() {
    char json[5012] = "{\"name\": \"Joseph Sypal\",\n\"age\": 22,\n\"registered_to_vote\": true,\n\"likes_cheese\": null}";

    CJ_Value value = {0};
    cj_parse(&value, json, strlen(json));

    if (value.tag != CJ_OBJ) {
        printf("Failed: Object must be top level.\n");
        exit(EXIT_FAILURE);
    }
    printf("Found object as top level entity\n");
    for (size_t i = 0; i < value.val.obj.len; i++) {
        CJ_KvPair pair = value.val.obj.o[i];
        printf("key = %s\n", pair.key);
        printf("value = ");
        switch (pair.value.tag) {
            case CJ_STR:
                printf("%s\n\n", pair.value.val.str);
                break;
            case CJ_NUM:
                printf("%f\n\n", pair.value.val.num);
                break;
            case CJ_NULL:
                printf("null\n\n");
                break;
            case CJ_BOOL:
                printf("%s\n\n", pair.value.val.b ? "true" : "false");
                break;
            case CJ_OBJ:
                printf("object\n\n");
                break;
            case CJ_ARR:
                printf("array\n\n");
                break;
        }
    }

    char* s = cj_dump(value);
    if (s != NULL) {
        printf("%s\n", s);
        free(s);
        s = NULL;
    } else {
        printf("NULL\n");
    }

    cj_free_value(&value);
}
