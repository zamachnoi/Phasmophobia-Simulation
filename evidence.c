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
        else return randFloat(0, 5);
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


