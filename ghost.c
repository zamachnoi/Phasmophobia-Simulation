#include "defs.h"

void *ghostMove(void* g) {
    
    GhostType* ghost = (GhostType*) g;
    while(ghost->boredom > 0) {
        // printf("%d", ghost->boredom);

        if(ghost->room->numHunters > 0) {
            ghost->boredom = BOREDOM_MAX;

            int randNum =randInt(0,2);

            if(randNum) {
                leaveEvidence(ghost);
                
            } else {
               
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
    
}

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

    // unlock room
   sem_post(&ghost->room->mutex);

    
}


void moveGhostRoom(GhostType* ghost) {

    // lock the room
    sem_wait(&ghost->room->mutex);

    int randRoom = randInt(0, ghost->room->neighbours->size);
    RoomNodeType* roomNode = ghost->room->neighbours->head;
    for(int i = 0; i < randRoom; i++) {
        roomNode = roomNode->next;
    }

    RoomType* prevRoom = ghost->room;
    RoomType* newRoom = roomNode->room;
    
    // lock rooms
    sem_wait(&newRoom->mutex);

    // remove ghost from previous room
    prevRoom->ghost = NULL;
    newRoom->ghost = (struct GhostType*) ghost;

    
    ghost->room = newRoom;

    // unlock the room
    sem_post(&ghost->room->mutex);
    sem_post(&prevRoom->mutex);
}

void initGhost(GhostType* ghost, BuildingType* building) {
    
    ghost->type = initGhostType();
    ghost->room = initGhostRoom(ghost, building);
    ghost->building = (struct Building*) building;
    ghost->boredom = BOREDOM_MAX;
}

RoomType* initGhostRoom(GhostType* ghost, BuildingType* building) {
    int randRoom = randInt(1, building->rooms.size);
    RoomNodeType* roomNode = building->rooms.head;
    for(int i = 0; i < randRoom; i++) {
        roomNode = roomNode->next;
    }
    roomNode->room->ghost = (struct GhostType*) ghost;
    return roomNode->room;
    
}


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