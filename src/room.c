#include "defs.h"

/*
Function: initRoomList()
 Purpose: initialize a room list
        in: RoomListType* roomList
*/
void initRoomList(RoomListType* roomList) {
    roomList->size = 0;
    roomList->head = NULL;
    roomList->tail = NULL;
}

/*
Function: initRoom()
 Purpose: initialize a room
        in: RoomType* room, char* name
        out: initizalized room
*/
void initRoom(RoomType* room, char* name) {
    strcpy(room->name, name);
    room->neighbours = (RoomListType*) calloc(1, sizeof(RoomListType));
    room->evidence = (EvidenceListType*) calloc(1, sizeof(EvidenceListType));
    room->hunterArr = (HunterArrayType*) calloc(1, sizeof(HunterArrayType));

    initRoomList(room->neighbours);
    initEvidenceList(room->evidence);
    initHunterArray(room->hunterArr);
    // initialize semaphore
    sem_init(&room->mutex, 0, 1);

    room->numHunters = 0;
    room->ghost = NULL;
}

/*
Function: appendRoom()
 Purpose: append a room to a room list
        in: RoomListType* roomList, RoomNodeType* roomNode
        out: room list with appended room
*/
void appendRoom(RoomListType* roomList, RoomNodeType* roomNode) {
    if (roomList->head == NULL) {
        roomList->head = roomNode;
        roomList->tail = roomNode;
    } else {
        roomList->tail->next = roomNode;
        roomList->tail = roomNode;
    }
    roomList->size++;
}

/*
Function: connectRooms()
 Purpose: connect two rooms together
        in: RoomType* room1, RoomType* room2
        out: room1 and room2 are connected
*/
void connectRooms(RoomType* room1, RoomType* room2) {
    RoomNodeType* node1 = calloc(1, sizeof(RoomNodeType));
    RoomNodeType* node2 = calloc(1, sizeof(RoomNodeType));

    node1->room = room1;
    node2->room = room2;

    appendRoom(room1->neighbours, node2);
    appendRoom(room2->neighbours, node1);
}

/*
Function: printNeighbours()
 Purpose: print the neighbours of a room
        in: RoomNodeType* roomNode
*/
void printNeighbours(RoomNodeType* roomNode) {
    RoomNodeType* neighbourNode = roomNode->room->neighbours->head;
    while (neighbourNode != NULL) {
        printf(" | %s", neighbourNode->room->name);
        
        neighbourNode = neighbourNode->next;
    }
    printf("\nnum neighbours: %d\n", roomNode->room->neighbours->size);
}