#include "defs.h"

/*
Function: printResults
Purpose: Prints the results of the game
      in: h1, h2, h3, h4, g
*/
void printResults(HunterType* h1, HunterType* h2, HunterType* h3, HunterType* h4, GhostType* g) {

    printf("All done! who won?\n");
    int ghostFlag = 0;
    int hunterFlag = -1;

    int hunterFearArray[4];

    hunterFearArray[0] = checkFear(h1);
    hunterFearArray[1] = checkFear(h2);
    hunterFearArray[2] = checkFear(h3);
    hunterFearArray[3] = checkFear(h4);

    ghostFlag = hunterFearArray[0] + hunterFearArray[1] + hunterFearArray[2] + hunterFearArray[3];


    const char* enumTypes[] = {"POLTERGEIST", "BANSHEE", "BULLIES", "PHANTOM"};
    printf("\n");
    if(ghostFlag == 4) {
        printf("The ghost wins! It was type %s.\n", enumTypes[g->type]);
    } else {
        if(hunterFearArray[0] == 0){
            calculateGhost(h1);
        }
        if(hunterFearArray[1] == 0){
        }
        if(hunterFearArray[2] == 0){
            calculateGhost(h3);
            
        }
        if(hunterFearArray[3] == 0){
            calculateGhost(h4);
        }
        printf("The hunters win!\n");
        printf("The ghost was type %s.\n", enumTypes[g->type]);
    }

}

/*
Function: checkFear()
Purpose: Checks if the hunter has run away in fear or gotten too bored
      in: HunterType pointer - h
*/
int checkFear(HunterType* h){
    if(h->fear >= 100){
        printf("%20s has run away in fear!\n", h->name);
        return 1;
    } else if(h->boredom <= 0) {
        printf("%20s got too bored...\n", h->name);
        return 1;
    } else {
        printf("%20s uncovered all the evidence needed to find the ghost!\n", h->name);
        return 0;
    }    
}

/*
Function: calculateGhost()
 Purpose: Calculates and prints the ghost type based on the evidence collected
      in: HunterType pointer - h
*/
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
        printf("%s collected evidence of type EMF, TEMPERATURE, and FINGERPRINTS.\n", h->name);
        printf("They speculate the ghost is a POLTERGEIST.\n\n");
    } else if (emfFlag == 1 && tempFlag == 1 && fingerFlag == 0 && soundFlag == 1) {
        printf("%s collected evidence of type EMF, TEMPERATURE, and SOUND.\n", h->name);
        printf("They speculate the ghost is a BANSHEE.\n\n");
    } else if (emfFlag == 1 && tempFlag == 0 && fingerFlag == 1 && soundFlag == 1) {
        printf("%s collected evidence of type EMF, FINGERPRINTS, and SOUND.\n", h->name);
        printf("They speculate the ghost is a BULLIES\n\n");
    } else if (emfFlag == 0 && tempFlag == 1 && fingerFlag == 1 && soundFlag == 1) {
        printf("%s collected evidence of type TEMPERATURE, FINGERPRINTS, and SOUND.\n", h->name);
        printf("They speculate the ghost is a PHANTOM.\n\n");
    } else {
        
    }

}

