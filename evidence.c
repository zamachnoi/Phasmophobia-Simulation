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

