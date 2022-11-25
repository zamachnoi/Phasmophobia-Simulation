#include "defs.h"

void printResults(HunterType* h1, HunterType* h2, HunterType* h3, HunterType* h4, GhostType* g) {
    int ghostFlag = 0;

    if(h1->fear >= 100) {
        printf("%s is too scared!\n", h1->name);
        ghostFlag += 1;
    }
    if(h2->fear >= 100) {
        printf("%s is too scared!\n", h2->name);
        ghostFlag += 1;
    }
    if(h3->fear >= 100) {
        printf("%s is too scared!\n", h3->name);
        ghostFlag += 1;
    }
    if(h4->fear >= 100) {
        printf("%s is too scared!\n", h4->name);
        ghostFlag += 1;
    }

    if(ghostFlag == 4) {
        printf("The ghost wins! It was type %s.\n", g->type);
    } else {
        
    }


}