#include "../../src/cjson.h"

int main(int argc, char *argv[]) {
    char* keys[] = {
        "meat",
        "team",
        "chair",
        "pair",
        "hello world how are you doing on this very fine day?",
        "a man a plan a canal panama",
        "computer",
        "expedia",
        "ziprecruiter",
        "zzzzzzzz",
        "yellow",
        "vengence",
        "ecnegnev",
        "abcdefghijklmnopqrstuvwxyz",
        "MEAT",
        "TEAM",
        "CHAIR",
        "PAIR",
        "COMPUTER",
        "EXPEDIA",
        "ZIPRECRUITER",
        "ZZZZZZZZZ",
        "YELLOW",
        "VENGENCE",
        "epinpin",
        "cvbnm,ngfghjkyt",
        "03efh20ufbcbeou][][]909()()()()))()1!!!@#$%%^$#@$%^&%$#@#%^%$##%^%$#@#$",
        "!@#$%^&*()(*&^%$#@!@#$%^&^%$#@#$%^&^%$#@@#$%^",
        "canary",
        "postgraduate",
        "andromous",
        "heehaw",
        "subsolar",
        "clinch",
        "sortie",
        "nursemaid",
        "inceptive",
        "consecrate",
        "lobar",
        "mariculture",
        "deify",
        "optical",
        "limit",
        "enrichment",
        "cytotoxic",
        "radio"
    };

    int hashcode_count[46] = {0};

    Map* map = initMap(46);
    for (int i = 0; i < 46; i++) {
        unsigned long hashcode = hash(keys[i], 46);
        hashcode_count[hashcode]++;
        insertInt(map, keys[i], 0);
    }

    printf("There were %d hash collisions.\n", 46 - map->size);
    printf("Hashcode distribution (code: count):\n");
    for (int i = 0; i < 46; i++) {
        printf("%d: %d\n", i, hashcode_count[i]);
    }

    return EXIT_SUCCESS;
}

