#include "defs.h"

/*
Function: initEvidenceList()
    Purpose: Initialize the evidence list
        in: EvidenceListType* ev
*/
void initEvidenceList(EvidenceListType* ev) {
    ev->size = 0;
    ev->head = NULL;
    ev->tail = NULL;
}

/*
Function: appendEvidence()
    Purpose: Append evidence to the end of the evidence list
        in: EvidenceListType* evList, EvidenceType* ev
        out: updated evList
*/
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

/*
Function: initEvidence()
    Purpose: Initialize the evidence
        in: EvidenceType* ev, EvidenceClassType et, float value
        out: initialized evidence structure
*/
void initEvidence(EvidenceType* ev, EvidenceClassType et, float value) {
    ev->type = et;
    ev->value = value;
}

/*
Function: printEvidenceList()
    Purpose: Print the evidence list
        in: EvidenceListType* evList
*/
void printEvidenceList(EvidenceListType* evList) {
    EvidenceNodeType* node = evList->head;
    while (node != NULL) {
        printEvidence(node->data);
        node = node->next;
    }
}

/*
Function: printEvidence()
 Purpose: Print the evidence
     in: EvidenceType* ev
*/
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

/*
Function: checkEvidence()
 Purpose: Check if evidence is ghostly
        in: EvidenceClassType type, float value
    return: 1 if ghostly, 0 if not
*/
int checkEvidence(EvidenceClassType et, float value){
    // Check if evidence is ghostly
    if(et == EMF){
        if(value >= 4.9) return 1;
        else return 0;
    }else if(et == TEMPERATURE){
        if(value <= 0) return 1;
        else return 0;
    }else if(et == FINGERPRINTS){
        if(value == 1) return 1;
        else return 0;
    }else{
        if(value >= 70) return 1;
        else return 0;
    }
}
/*
Function: getRandomEvidence() 
    Purpose: Get a random evidence from the evidence list
        in: EvidenceListType* list, EvidenceType** ev
        out: ev, or nothing if list is empty
*/
void getRandomEvidence(EvidenceListType* list, EvidenceType** ev) {
    if (list->size == 0) {
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

/*
Function: checkIfHaveEvidence()
    Purpose: Check if the hunter has the evidence passed into function
        in: HunterType* hunter, EvidenceType* ev
        out: 1 if hunter has evidence, 0 if not
*/
int checkIfHaveEvidence(HunterType* hunter, EvidenceType* ev) {

    // Check ghostly  evidence
    EvidenceNodeType* node = hunter->ghostlyEvidence->head;
    while (node != NULL) {
        if (node->data == ev) {
            return 1;
        }
        node = node->next;
    }

    // Check non ghostly evidence
    node = hunter->nonGhostlyEvidence->head;
    while (node != NULL) {
        if (node->data == ev) {
            return 1;
        }
        node = node->next;
    }
    return 0;

}

/*
Function: checkEvidenceThreeTypes()
 Purpose: Check if the hunter has 3 of the 4 types of evidence
        in: HunterType* hunter
    return: 1 if hunter has 3 of the 4 types of evidence, 0 if not
*/
int checkEvidenceThreeTypes(HunterType* hunter) {
    int EMFflag = 0;
    int TEMPflag = 0;
    int FINGERflag = 0;
    int SOUNDflag = 0;

    EvidenceNodeType* node = hunter->ghostlyEvidence->head;
    while (node != NULL) {
        if (node->data->type == EMF) {
            EMFflag = 1;
        } else if (node->data->type == TEMPERATURE) {
            TEMPflag = 1;
        } else if (node->data->type == FINGERPRINTS) {
            FINGERflag = 1;
        } else if (node->data->type == SOUND) {
            SOUNDflag = 1;
        }
        node = node->next;
    }

    // check if 3 of the 4 flags are 1
    if(EMFflag + TEMPflag + FINGERflag + SOUNDflag >= 3) {
        printf("FOUND GHOST------------------------------------------------\n");
        return 1;

    } else {
        return 0;
    }
}
