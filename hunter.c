#include "defs.h"

/*
Function: hunterMove()
 Purpose: This function is the main function for the hunter thread. It will
          move the hunter to a random room, pick up evidence, and communicate
          with other hunters. It will continue to do this until the hunter
          has found the ghost or died of fear.
       in: void* h - a void pointer to the hunter
*/
void* hunterMove(void* h) {

    HunterType* hunter = (HunterType*)h;
    int check = checkEvidenceThreeTypes(hunter);
    while(hunter->boredom > 0 && hunter->fear < 100 && check == 0) {
        if(hunter->room->ghost != NULL) {
            hunter->fear += FEAR_RATE;
        }
        hunter->boredom--;
        int rand = randInt(0,3);
            if(rand == 0) {
                moveHunterRoom(hunter);
            } else if (rand == 1) {
                pickupEvidence(hunter);
            } else {
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


/*
Function: initHunter()
 Purpose: This function initializes the hunter struct
       in: HunterType* hunter - a pointer to the hunter
           char* name - the name of the hunter
           RoomType* room - the room the hunter is in (usually van)
           EvidenceClassType et - evidence type the hunter can pick up
           BuildingType* building - the building the hunter is in
       out: Initialized hunter object
*/
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

/*
Function: pickupEvidence()
 Purpose: This function is called when the hunter chooses to pickup evidence in the room that it is in. If there is no evidence in the room,
            it will generate a random piece of evidence. If there is evidence in the room, it will call the getEvidence() function.
         in: HunterType* hunter - a pointer to the hunter
         out: Hunter with updated evidence list
*/
void pickupEvidence(HunterType* hunter) {
    
    if(hunter->room->evidence->size == 0) {
        // Generate evidence and check if it is ghostly or not
        float val = generateEvidence(hunter->type, 0);
        EvidenceType* ev = calloc(1, sizeof(EvidenceType));
        initEvidence(ev, hunter->type, val);

        // Append evidence to master evidence list for EZ FREEING BRO
        appendEvidence(&hunter->building->evidence, ev);

        appendEvidence(hunter->nonGhostlyEvidence, ev);
        const char* types[] = {"EMF", "TEMPERATURE", "FINGERPRINTS", "SOUND"};

        printf("%s has picked up RANDOM %s evidence.\n", hunter->name, types[ev->type]);

    } else {
        getEvidence(hunter);
    }
}

/*
Function: getEvidence()
 Purpose: This function is called when the hunter chooses to pickup evidence in the room that it is in, if there is evidence for it to pickup.
            It will lock the room, find and check the type of the evidence in the room, and remove it from the room. It will then unlock the room.
         in: HunterType* hunter - a pointer to the hunter
         out: Hunter with updated evidence list
*/
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

    // print that shi
    const char* types[] = {"EMF", "TEMPERATURE", "FINGERPRINTS", "SOUND"};

    char evidenceGhostly[MAX_STR];

    if (checkEvidence(ret->type, ret->value)){
        strcpy(evidenceGhostly, "GHOSTLY");
    } else {
        strcpy(evidenceGhostly, "NON-GHOSTLY");
    }
    printf("%s has picked up %s %s evidence.\n", hunter->name, evidenceGhostly, types[ret->type]);
}


/*
Function: moveHunterRoom()
 Purpose: This function is called when the hunter chooses to move to a random room. It will lock the room, and then move the hunter to a random room.
         in: HunterType* hunter - a pointer to the hunter
         out: Hunter with updated room, rooms with updated hunter pointers
*/
void moveHunterRoom(HunterType* hunter) {

    int randRoom = randInt(0, hunter->room->neighbours->size);
    RoomNodeType* roomNode = hunter->room->neighbours->head;
    for(int i = 0; i < randRoom-1; i++) {
        roomNode = roomNode->next;
    }

    RoomType* prevRoom = hunter->room;
    RoomType* newRoom = roomNode->room;
    
    // lock rooms
    // Prevent deadlock when locking rooms
    // If the hunter's room is available for modification ,lock it
    if(sem_trywait(&(hunter->room->mutex)) == 0) {
        //if the new room is not available for modification, unlock the hunter's room and return nothing
        if(sem_trywait(&(newRoom->mutex)) != 0) {
            sem_post(&(hunter->room->mutex));
            return;
        }
    }    

    // Do the funky room moving
    removeHunter(hunter);
    hunter->room = newRoom;
    addHunter(hunter);

    // Unlock it
    sem_post(&prevRoom->mutex);
    sem_post(&hunter->room->mutex);
    
}

/*
Function: addHunter()
 Purpose: This function adds a hunter to the hunter array in the room that it is in.
         in: HunterType* hunter - a pointer to the hunter
         out: Updated room with hunter pointer
*/
void addHunter(HunterType* hunter) {
    hunter->room->hunterArr->hunters[hunter->room->hunterArr->size] = hunter;
    hunter->room->hunterArr->size++;
}

/*
Function: removeHunter()
 Purpose: This function removes a hunter from the hunter array in the room that it is in.
         in: HunterType* hunter - a pointer to the hunter
         out: Updated room with hunter pointer
*/
void removeHunter(HunterType* hunter) {
    for(int i = 0; i < hunter->room->hunterArr->size; i++) {
        if(hunter->room->hunterArr->hunters[i] == hunter) {
            hunter->room->hunterArr->hunters[i] = NULL;
            //move all hunters down
            for(int j = i; j < hunter->room->hunterArr->size-1; j++) {
                hunter->room->hunterArr->hunters[j] = hunter->room->hunterArr->hunters[j+1];
            }

            // LOWER THE SIZE@!!
            hunter->room->hunterArr->size--;
            return;
        }
    }
    
}   

/*
Function: communicate()
 Purpose: This function is called when the hunter chooses to communicate with another hunter. It will lock the room, and then pick a random hunter in the room to communicate with.
         in: HunterType* hunter - a pointer to the hunter
*/
void communicate(HunterType* hunter) {
    
    // Lock room
    // sem_wait(&hunter->room->mutex);

    // Unlock room and return if theres no other hunter to communicate with
    if(hunter->room->hunterArr->size == 1) {
        // sem_post(&hunter->room->mutex);
        return;
    }

    // Get random hunter to communicate with
    int randHunter = randInt(0, hunter->room->hunterArr->size);
    while(hunter->room->hunterArr->hunters[randHunter] == hunter) {
        randHunter = randInt(0, hunter->room->hunterArr->size);
        
    }
    HunterType* otherHunter = hunter->room->hunterArr->hunters[randHunter];

    
    // Get random evidence from the hunter communicating
    EvidenceType* ev = NULL;
    getRandomEvidence(hunter->ghostlyEvidence, &ev);

    // If hunter doesnt have evidence to give, unlock room and return
    if(ev == NULL) {
        // Unlock room
        // sem_post(&hunter->room->mutex);
        return;
    } else {

        // Give evidence to other hunter
        if(checkIfHaveEvidence(otherHunter, ev) == 0) {
            printf("Hunter %s gave evidence to hunter %s\n", hunter->name, otherHunter->name);
            appendEvidence(otherHunter->ghostlyEvidence, ev);
            

            // Print that shit 
            const char* types[] = {"EMF", "TEMPERATURE", "FINGERPRINTS", "SOUND"};

            char evidenceGhostly[MAX_STR];

            if (checkEvidence(ev->type, ev->value)){
                strcpy(evidenceGhostly, "GHOSTLY");
            } else {
                strcpy(evidenceGhostly, "NON-GHOSTLY");
            }
            printf("%s has shared %s %s evidence with %s.\n", hunter->name, evidenceGhostly, types[ev->type], otherHunter->name);
        }
    }
    // Unlock room
    //sem_post(&hunter->room->mutex);   
}

/*
Function: initHunterArray()
 Purpose: This function initializes the hunter array.
         in: HunterArrayType* hunterArr - a pointer to the hunter array
*/
void initHunterArray(HunterArrayType* hunterArr) {
    hunterArr->size = 0;
}

// Function to print a hunter
void printHunter(HunterType* hunter ) {
    printf("Hunter: %s, Room: %s, Boredom: %d, Fear: %d\n", hunter->name, hunter->room->name, hunter->boredom, hunter->fear);
    printf("Ghostly Evidence: \n");
    printEvidenceList(hunter->ghostlyEvidence);
    printf("Non Ghostly Evidence: \n");
    printEvidenceList(hunter->nonGhostlyEvidence);
    printf("\n");
}


