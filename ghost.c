#include "defs.h"

void ghostMove(GhostType* ghost) {
    if(ghost->room->numHunters > 0) {
        ghost->boredom = BOREDOM_MAX;

        int randNum = randInt(0,2);

        if(randNum) {
            leaveEvidence(ghost);
            return;
        } else {
            return;
        }
    
    } else {
        ghost->boredom--;

        

        int randNum = randInt(0,3); // 3 options Leave evidence, move, or do nothing
        if(randNum == 0) {
            printf("left evidence.\n");
            leaveEvidence(ghost);
            return;
        } else if (randNum == 1) {
            printf("ghost moved\n");
            moveGhostRoom(ghost);
            return;
        } else {
            printf("Didnt move\n");
            return;
        }
        
        if(ghost->boredom <= 0) {
            //END THREAD
        }
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
    
    EvidenceClassType type = num[randInt(0,3)];
    float value = generateEvidence(type, 1);

    initEvidence(ev, type, value);
    appendEvidence(ghost->room->evidence, ev);
    
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

void moveGhostRoom(GhostType* ghost) {
    int randRoom = randInt(0, ghost->room->neighbours->size);
    RoomNodeType* roomNode = ghost->room->neighbours->head;
    for(int i = 0; i < randRoom-1; i++) {
        roomNode = roomNode->next;
    }
        ghost->room = roomNode->room;
}

void initGhost(GhostType* ghost, BuildingType* building) {
    
    ghost->type = initGhostType();
    ghost->room = initGhostRoom(ghost, building);
    ghost->boredom = BOREDOM_MAX;
}

RoomType* initGhostRoom(GhostType* ghost, BuildingType* building) {
    int randRoom = randInt(1, building->rooms.size);
    RoomNodeType* roomNode = building->rooms.head;
    for(int i = 0; i < randRoom; i++) {
        roomNode = roomNode->next;
    }
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
}