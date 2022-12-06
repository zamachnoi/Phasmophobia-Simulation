#include "defs.h"

/*
Function: ghostMove()
 Purpose: This function is the main function for the ghost thread. 
          It will pick move ghost to a random room or leave evidence in the room it is in.
          It will also check if the ghost is bored and if it is, it will leave the house.
        in: void* g - a void pointer to the ghost struct
*/
void *ghostMove(void* g) {
    
    GhostType* ghost = (GhostType*) g;
    while(ghost->boredom > 0) {

        if(ghost->room->hunterArr->size > 0) {
            ghost->boredom = BOREDOM_MAX;

            int randNum =randInt(0,2);

            if(randNum) {
                leaveEvidence(ghost);   
            }
    
       } else {
            ghost->boredom--;

            int randNum = randInt(0,3); // 3 options Leave evidence, move, or do nothing
            if(randNum == 0) {
                // printf("left evidence.\n");
                leaveEvidence(ghost);
            } else if (randNum == 1) {
                // printf("ghost moved\n");
                moveGhostRoom(ghost);
            } else {
                // printf("Didnt move\n");
            }
            
       }
         usleep(USLEEP_TIME);
    }
    printf("The ghost got bored and left the house\n");
    
}

/*
Function: leaveEvidence()
 Purpose: This function will generate a random evidence type and value and add it to the room the ghost is in.
        in: GhostType* ghost - a pointer to the ghost struct
        out: updated rooms evidence list
*/
void leaveEvidence(GhostType* ghost) {

    EvidenceType* ev = calloc(1, sizeof(EvidenceType));
    EvidenceClassType num[3];
    
    switch(ghost->type) {
        case POLTERGEIST:
            num[0] = EMF;
            num[1] = TEMPERATURE;
            num[2] = FINGERPRINTS;
            break;
        case BANSHEE:
            num[0] = EMF;
            num[1] = TEMPERATURE;
            num[2] = SOUND;
            break;
        case BULLIES:
            num[0] = EMF;
            num[1] = FINGERPRINTS;
            num[2] = SOUND;
            break;
        case PHANTOM:
            num[0] = TEMPERATURE;
            num[1] = FINGERPRINTS;
            num[2] = SOUND;
            break;
    }
    // lock room
    sem_wait(&ghost->room->mutex);
    
    EvidenceClassType type = num[randInt(0,3)];
    float value = generateEvidence(type, 1);

    initEvidence(ev, type, value);
    appendEvidence(ghost->room->evidence, ev);

    // append evidence to master list of evidence
    appendEvidence(&ghost->building->evidence, ev);
    
    const char* types[] = {"EMF", "TEMPERATURE", "FINGERPRINTS", "SOUND"};

    char evidenceGhostly[MAX_STR];

    if (checkEvidence(ev->type, ev->value)){
        strcpy(evidenceGhostly, "GHOSTLY");
    } else {
        strcpy(evidenceGhostly, "NON-GHOSTLY");
    }
    printf("The ghost left %s %s evidence in %s\n", evidenceGhostly, types[ev->type], ghost->room->name);

    // unlock room
   sem_post(&ghost->room->mutex);

    
}

/*
Function: moveGhostRoom()
 Purpose: This function will move the ghost to a random room in the building.
        in: GhostType* ghost - a pointer to the ghost struct
        out: updated ghost room
*/
void moveGhostRoom(GhostType* ghost) {

    int randRoom = randInt(0, ghost->room->neighbours->size);
    RoomNodeType* roomNode = ghost->room->neighbours->head;
    for(int i = 0; i < randRoom; i++) {
        roomNode = roomNode->next;
    }

    RoomType* prevRoom = ghost->room;
    RoomType* newRoom = roomNode->room;

    // Prevent deadlock when locking rooms
    // If the hunter's room is available for modification ,lock it
    if(sem_trywait(&(ghost->room->mutex)) == 0) {
        //if the new room is not available for modification, unlock the hunter's room and return nothing
        if(sem_trywait(&(newRoom->mutex)) != 0) {
            sem_post(&(ghost->room->mutex));
            return;
        }
    }    
    printf("The ghost moved to %s\n", newRoom->name);
    // remove ghost from previous room
    prevRoom->ghost = NULL;
    newRoom->ghost = (struct GhostType*) ghost;

    
    ghost->room = newRoom;

    // unlock the room
    sem_post(&ghost->room->mutex);
    sem_post(&prevRoom->mutex);
}

/*
Function: initGhost() 
 Purpose: This function will initialize the ghost struct and set the ghost's room to a random room in the building.
        in: GhostType* ghost - a pointer to the ghost struct
            BuildingType* building - a pointer to the building struct
        out: updated ghost struct
*/
void initGhost(GhostType* ghost, BuildingType* building) {
    
    ghost->type = initGhostType();
    ghost->room = initGhostRoom(ghost, building);
    ghost->building = (struct Building*) building;
    ghost->boredom = BOREDOM_MAX;
}

/*
Function: initGhostRoom() 
 Purpose: This function will set the ghost's room to a random room in the building.
        in: GhostType* ghost - a pointer to the ghost struct
            BuildingType* building - a pointer to the building struct
        out: updated ghost struct
    return: RoomType* - a pointer to the room the ghost is in
*/
RoomType* initGhostRoom(GhostType* ghost, BuildingType* building) {
    int randRoom = randInt(1, building->rooms.size);
    RoomNodeType* roomNode = building->rooms.head;
    for(int i = 0; i < randRoom; i++) {
        roomNode = roomNode->next;
    }
    roomNode->room->ghost = (struct GhostType*) ghost;
    return roomNode->room;
    
}

/*
Function: initGhostType()
 Purpose: This function will randomly generate a ghost type.
    return: GhostClassType - the ghost type
*/
GhostClassType initGhostType() {
    int randNum = randInt(0,4);
    switch(randNum) {
        case 0:
            return POLTERGEIST;
        case 1:
            return BANSHEE;
        case 2:
            return BULLIES;
        case 3:
            return PHANTOM;
    }
    return POLTERGEIST;
}