#include "defs.h"


void initEvidenceList(EvidenceListType* ev) {
    ev->size = 0;
    ev->head = NULL;
    ev->tail = NULL;
}

void appendEvidence(EvidenceListType* evList, EvidenceType* ev) {
    EvidenceNodeType* node = calloc(1, sizeof(EvidenceNodeType));
    node->data = ev;

    if (evList->head == NULL) {
        evList->head = node;
        evList->tail = node;
    } else {
        evList->tail->next = node;
        evList->tail = node;
    }
    evList->size++;
}


void initEvidence(EvidenceType* ev, EvidenceClassType et, float value) {
    ev->type = et;
    ev->value = value;
}

// Print evidence list contents
void printEvidenceList(EvidenceListType* evList) {
    EvidenceNodeType* node = evList->head;
    while (node != NULL) {
        printEvidence(node->data);
        node = node->next;
    }
}

// Print evidence contents
void printEvidence(EvidenceType* ev) {
    printf("Evidence type: %d, value: %f\n", ev->type, ev->value);
}

/*
    Function: generateEvidence()
        Purpose: Generate a random value for the evidence (either ghostly value or not)
            in: EvidenceClassType type, int ghostly
*/
float generateEvidence (EvidenceClassType et, int ghostly){
    if(et == EMF){
        if(ghostly == 1) return randFloat(4, 5.00);
        else if(ghostly == 0) return randFloat(0, 5);
    }else if(et == TEMPERATURE){
        if(ghostly == 1) return randFloat(-10.00, 1);
        else return randFloat(1.1, 10.00);
    }else if(et == FINGERPRINTS){
        if (ghostly == 1) return 1;
        else return 0;
    }else{
        if (ghostly == 1) return randFloat(65,75);
        else return randFloat(40, 70);
    }
}


int checkEvidence(EvidenceClassType et, float value){
    // Check if evidence is ghostly
    if(et == EMF){
        if(value >= 4.7) return 1;
        else return 0;
    }else if(et == TEMPERATURE){
        if(value <= 1) return 1;
        else return 0;
    }else if(et == FINGERPRINTS){
        if(value == 1) return 1;
        else return 0;
    }else{
        if(value >= 65) return 1;
        else return 0;
    }
}

void getRandomEvidence(EvidenceListType* list, EvidenceType** ev) {
    if (list->size == 0) {
        printf("ERROR: Evidence list is empty\n");
        return;
    }
    int random = randInt(0, list->size);
    EvidenceNodeType* node = list->head;
    for(int i = 0; i < random; i++) {
        
        node = node->next;
    }
    *ev = node->data;
    return;
}


int checkIfHaveEvidence(HunterType* hunter, EvidenceType* ev) {
    EvidenceNodeType* node = hunter->ghostlyEvidence->head;
    while (node != NULL) {
        if (node->data == ev) {
            return 1;
        }
        node = node->next;
    }
    node = hunter->nonGhostlyEvidence->head;
    while (node != NULL) {
        if (node->data == ev) {
            return 1;
        }
        node = node->next;
    }
    return 0;

}

void checkEvidenceThreeTypes(Hunter)
