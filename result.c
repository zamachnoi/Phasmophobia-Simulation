#include "defs.h"

void printResults(HunterType* h1, HunterType* h2, HunterType* h3, HunterType* h4, GhostType* g) {
    int ghostFlag = 0;
    int hunterFlag = -1;

    int hunterFearArray[4];

    hunterFearArray[0] = checkFear(h1);
    hunterFearArray[1] = checkFear(h2);
    hunterFearArray[2] = checkFear(h3);
    hunterFearArray[3] = checkFear(h4);

    ghostFlag = hunterFearArray[0] + hunterFearArray[1] + hunterFearArray[2] + hunterFearArray[3];

    const char* enumTypes[] = {"POLTERGEIST", "BANSHEE", "BULLIES", "PHANTOM"};

    if(ghostFlag == 4) {
        printf("The ghost wins! It was type %s.\n", enumTypes[g->type]);
    } else {
        
        
        
        
        if(hunterFearArray[0] == 0){
            calculateGhost(h1);
            printf("Hunter %s wins! It was type %s.\n", h1->name, enumTypes[g->type]);
        }
        if(hunterFearArray[1] == 0){
            calculateGhost(h2);
            printf("Hunter %s wins! It was type %s.\n", h2->name, enumTypes[g->type]);
            hunterFlag = 1;
        }
        if(hunterFearArray[2] == 0){
            calculateGhost(h3);
            printf("Hunter %s wins! It was type %s.\n", h3->name, enumTypes[g->type]);
            hunterFlag = 2;
        }
        if(hunterFearArray[3] == 0){
            calculateGhost(h4);
            printf("Hunter %s wins! It was type %s.\n", h4->name, enumTypes[g->type]);
            hunterFlag = 3;
        }
    }

}


int checkFear(HunterType* h){
    if(h->fear >= 100){
        printf("%s is too scared!\n", h->name);
        return 1;
    }
    return 0;
}

void calculateGhost(HunterType* h) {
    int emfFlag = 0;
    int tempFlag = 0;
    int fingerFlag = 0;
    int soundFlag = 0;
    
    EvidenceNodeType* curr = h->ghostlyEvidence->head;

    while(curr != NULL) {
        if(curr->data->type == EMF) {
            emfFlag = 1;

        } else if(curr->data->type == TEMPERATURE) {
            tempFlag = 1;

        } else if(curr->data->type == FINGERPRINTS) {
            fingerFlag = 1;

        } else if(curr->data->type == SOUND) {
            soundFlag = 1;
        }
        curr = curr->next;
    }

    // Calculate the ghost type out of the four possibilities
    if(emfFlag == 1 && tempFlag == 1 && fingerFlag == 1 && soundFlag == 0) {
        printf("The hunter collected evidence of type EMF, TEMPERATURE, and FINGERPRINTS.\n");
        printf("They speculate the ghost is a POLTERGEIST.\n");
    } else if (emfFlag == 1 && tempFlag == 1 && fingerFlag == 0 && soundFlag == 1) {
        printf("The hunter collected evidence of type EMF, TEMPERATURE, and SOUND.\n");
        printf("They speculate the ghost is a BANSHEE.\n");
    } else if (emfFlag == 1 && tempFlag == 0 && fingerFlag == 1 && soundFlag == 1) {
        printf("The hunter collected evidence of type EMF, FINGERPRINTS, and SOUND.\n");
        printf("They speculate the ghost is a BULLIES\n");
    } else if (emfFlag == 0 && tempFlag == 1 && fingerFlag == 1 && soundFlag == 1) {
        printf("The hunter collected evidence of type TEMPERATURE, FINGERPRINTS, and SOUND.\n");
        printf("They speculate the ghost is a PHANTOM.\n");
    } else {
        printf("%s got bored\n", h->name);
    }

}

