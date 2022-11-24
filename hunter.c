#include "defs.h"

void* hunterMove(void* h) {

    HunterType* hunter = (HunterType*)h;

    while(hunter->boredom <= 0 || hunter->fear => 100) {
        if(hunter->room->ghost != NULL) {
            hunter->fear ++;
            hunter->boredom = BOREDOM_MAX;
        }
        else{
            hunter->boredom--;
        }
        int rand = randInt(0,3);

            if(rand == 0) {
                moveHunterRoom(hunter);
            } else if (rand == 1) {
                pickupEvidence(hunter);
            } else {
                communicate(hunter);
            }
        usleep(USLEEP_TIME);
    }
        
}



void initHunter(HunterType* hunter, RoomType* room) {
    hunter->room = room;
    hunter->boredom = BOREDOM_MAX;
    hunter->fear = 0;
    hunter->ghostlyEvidence = calloc(1, sizeof(EvidenceListType));
    hunter->nonGhostlyEvidence = calloc(1, sizeof(EvidenceListType));
    initEvidenceList(hunter->ghostlyEvidence);
    initEvidenceList(hunter->nonGhostlyEvidence);
}

void pickupEvidence(HunterType* hunter) {
    EvidenceType* ev;
    if(hunter->room->evidence == 0) {
        float val = generateEvidence(hunter->type, 0);
        initEvidence(&ev, hunter->type, val);
        ev = genEvidence(hunter)
    } else {
       ev = getEvidence(hunter);
    }

    
    
}

EvidenceType* getEvidence(hunter) {

    // Lock room
    sem_wait(&hunter->room->mutex);

    EvidenceNodeType* curr = hunter->room->evidence->head;
    EvidenceNodeType* prev = NULL;
    EvidenceType* ret = NULL;

    while(curr != null) {
        // Check if type == type
        if(hunter->type == curr->data->type) {
            ret = curr->data;
            break;
        }
        prev = curr;
        curr = curr->next;
    }

    if(curr == NULL) { return NULL; }
    
    if(prev == NULL) {
        hunter->room->evidence->head = curr->next;
    } else {
        prev->next = curr->next;
    }

    // FREE THE NODE!!! NOT THE DATA
    free(curr);

    // Unlock room
    sem_post(&hunter->room->mutex);
    
    return ret;
}





