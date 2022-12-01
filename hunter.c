#include "defs.h"

void* hunterMove(void* h) {

    HunterType* hunter = (HunterType*)h;
    int check = checkEvidenceThreeTypes(hunter);
    while(hunter->boredom > 0 && hunter->fear < 100 && check == 0) {
        // printf(" %s boredom: %d, fear: %d\n", hunter->name, hunter->boredom, hunter->fear);
        //hunter room
        printf("%s room: %s\n", hunter->name, hunter->room->name);
        if(hunter->room->ghost != NULL) {
            hunter->fear += FEAR_RATE;
        }

        hunter->boredom--;
        int rand = randInt(0,3);

            if(rand == 0) {
                // printf("%s moved\n", hunter->name);
                moveHunterRoom(hunter);
                
            } else if (rand == 1) {
                //printf("%s picked up evidence \n", hunter->name);
                pickupEvidence(hunter);
            } else {
                //printf("%s communicated\n", hunter->name);
                communicate(hunter);
            }
         usleep(USLEEP_TIME);
        check = checkEvidenceThreeTypes(hunter);
    }
    if(hunter->fear >= 100) {
        printf("%s has died of fear\n", hunter->name);
    }
    if(check == 1) {
        printf("%s has found the ghost\n", hunter->name);
    }
    removeHunter(hunter);
}



void initHunter(HunterType* hunter, char* name, RoomType* room, EvidenceClassType et, BuildingType* building) {
    hunter->room = room;
    room->hunterArr->hunters[room->hunterArr->size] = hunter;
    room->hunterArr->size++;
    hunter->boredom = BOREDOM_MAX;
    hunter->fear = 0;
    hunter->ghostlyEvidence = calloc(1, sizeof(EvidenceListType));
    hunter->nonGhostlyEvidence = calloc(1, sizeof(EvidenceListType));
    initEvidenceList(hunter->ghostlyEvidence);
    initEvidenceList(hunter->nonGhostlyEvidence);
    strcpy(hunter->name, name);
    hunter->type = et;
    hunter->building = building;
}

void pickupEvidence(HunterType* hunter) {
    
    if(hunter->room->evidence->size == 0) {
        // Generate evidence and check if it is ghostly or not
        float val = generateEvidence(hunter->type, 0);
        EvidenceType* ev = calloc(1, sizeof(EvidenceType));
        initEvidence(ev, hunter->type, val);

        // Append evidence to master evidence list for EZ FREEING BRO
        appendEvidence(&hunter->building->evidence, ev);

        appendEvidence(hunter->nonGhostlyEvidence, ev);
        // Add it to corresponding hunter evidence list
        // if(checkEvidence(hunter->type, val) == 0) {
        //     appendEvidence(hunter->nonGhostlyEvidence, ev);
        // } else {
        //     appendEvidence(hunter->ghostlyEvidence, ev);
        //     printf("ADDING FAKE GHOSTLY HERE %f\n", ev->value);
        // }

    } else {
        getEvidence(hunter);
    }
}

void getEvidence(HunterType* hunter) {

    // Lock room

    sem_wait(&hunter->room->mutex);
    // printf("GET EV: LOCKED ROOM %s\n", hunter->room->name);

    EvidenceNodeType* curr = hunter->room->evidence->head;
    EvidenceNodeType* prev = NULL;
    EvidenceType* ret = NULL;

    while(curr != NULL) {
        // Check if type == type
        if(hunter->type == curr->data->type) {
            ret = curr->data;
            break;
        }
        prev = curr;
        curr = curr->next;
    }

    if(curr == NULL) { 
        // printf("GET EV: UNLOCKED ROOM (CURR = NULL) %s\n", hunter->room->name);
        
        sem_post(&hunter->room->mutex);
        return; }
    
    if(prev == NULL) {
        hunter->room->evidence->head = curr->next;

    } else {
        prev->next = curr->next;
    }

    // FREE THE NODE!!! NOT THE DATA

    // FIX THIS POSSIBY
    if(hunter->room->evidence->head == NULL) {
        hunter->room->evidence->tail = NULL;
    }
    if(hunter->room->evidence->tail == curr) {
        hunter->room->evidence->tail = prev;
    }
    free(curr);

    // Unlock room
    sem_post(&hunter->room->mutex);
    // printf("GET EV: UNLOCKED ROOM %s\n", hunter->room->name);

    
    // Add it to corresponding hunter evidence list
        if(checkEvidence(ret->type, ret->value) == 0) {
            appendEvidence(hunter->nonGhostlyEvidence, ret);
        } else {
            appendEvidence(hunter->ghostlyEvidence, ret);
            hunter->boredom = BOREDOM_MAX;
        }
}



void moveHunterRoom(HunterType* hunter) {

    // lock the room
    // printf("%s MOVING FROM ROOM: %s\n", hunter->name, hunter->room->name);
    sem_wait(&hunter->room->mutex); // old room
    // printf("LOCKED ROOM %s\n", hunter->room->name);


    int randRoom = randInt(0, hunter->room->neighbours->size);
    RoomNodeType* roomNode = hunter->room->neighbours->head;
    for(int i = 0; i < randRoom-1; i++) {
        roomNode = roomNode->next;
    }

    RoomType* prevRoom = hunter->room;
    RoomType* newRoom = roomNode->room;

    // printf("%s MOVING TO ROOM: %s\n", hunter->name, newRoom->name);

    
    // lock rooms
    sem_wait(&newRoom->mutex); // new room
    // printf("LOCKED ROOM %s\n", newRoom->name);


    // remove hunter from prev room and add it to new room
    
    removeHunter(hunter);
    hunter->room = newRoom;
    addHunter(hunter);
    
    // Add new room to hunter
 
    

    // unlock the room
    
    sem_post(&prevRoom->mutex);
    // printf("UNLOCKED ROOM %s\n", prevRoom->name);

    sem_post(&hunter->room->mutex);
    // printf("UNLOCKED ROOM %s\n", hunter->room->name); // new room
    
}

void addHunter(HunterType* hunter) {
    hunter->room->hunterArr->hunters[hunter->room->hunterArr->size] = hunter;
    hunter->room->hunterArr->size++;
    //printf("Room %s now has %d hunters\n", hunter->room->name, hunter->room->hunterArr->size);
}

void removeHunter(HunterType* hunter) {
    for(int i = 0; i < hunter->room->hunterArr->size; i++) {
        if(hunter->room->hunterArr->hunters[i] == hunter) {
            hunter->room->hunterArr->hunters[i] = NULL;
            //move all hunters down
            for(int j = i; j < hunter->room->hunterArr->size-1; j++) {
                hunter->room->hunterArr->hunters[j] = hunter->room->hunterArr->hunters[j+1];
            }
            hunter->room->hunterArr->size--;
            printf("Room %s now has %d hunters\n", hunter->room->name, hunter->room->hunterArr->size);
            return;
        }
    }
    
}   

void communicate(HunterType* hunter) {
    
    // Lock room
    sem_wait(&hunter->room->mutex);

    // Pick random hunter in the room
    //printf("%s ROOM SIZE: %d\n", hunter->room->name, hunter->room->hunterArr->size);

    if(hunter->room->hunterArr->size == 1) {
        sem_post(&hunter->room->mutex);
        return;
    }

    int randHunter = randInt(0, hunter->room->hunterArr->size);
    while(hunter->room->hunterArr->hunters[randHunter] == hunter) {
        randHunter = randInt(0, hunter->room->hunterArr->size);
        
    }
    HunterType* otherHunter = hunter->room->hunterArr->hunters[randHunter];

    

    EvidenceType* ev = NULL;
    getRandomEvidence(hunter->ghostlyEvidence, &ev);
    if(ev == NULL) {
        // printf("bruh this does not work\n");
    } else {
        if(checkIfHaveEvidence(otherHunter, ev) == 0) {
            printf("Hunter %s gave evidence to hunter %s\n", hunter->name, otherHunter->name);
            appendEvidence(otherHunter->ghostlyEvidence, ev);
        }
        
    }

    // Unlock room
    sem_post(&hunter->room->mutex);
    
}

void initHunterArray(HunterArrayType* hunterArr) {
    hunterArr->size = 0;
}


//NEXT TIME:
/*
- communicate evidence
    - make sure not duplicate data
    - share pointer
    
    When freeing evidence, make sure to set to NULL 
*/

void printHunter(HunterType* hunter ) {
    printf("Hunter: %s, Room: %s, Boredom: %d, Fear: %d\n", hunter->name, hunter->room->name, hunter->boredom, hunter->fear);
    printf("Ghostly Evidence: \n");
    printEvidenceList(hunter->ghostlyEvidence);
    printf("Non Ghostly Evidence: \n");
    printEvidenceList(hunter->nonGhostlyEvidence);
    printf("\n");
}


