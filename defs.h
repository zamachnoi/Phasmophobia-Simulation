#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
// #include <unistd.h>
// #include <pthread.h>
// #include <semaphore.h>

#define MAX_STR            64
#define FEAR_RATE           1
#define MAX_FEAR          100
#define MAX_HUNTERS         4
#define USLEEP_TIME     50000
#define BOREDOM_MAX        99

// You may rename these types if you wish
typedef enum { EMF, TEMPERATURE, FINGERPRINTS, SOUND } EvidenceClassType;
typedef enum { POLTERGEIST, BANSHEE, BULLIES, PHANTOM } GhostClassType;

int randInt(int, int);          // Generates a pseudorandom integer between the parameters
float randFloat(float, float);  // Generates a pseudorandom float between the parameters

typedef struct {
    char name[MAX_STR];
    struct RoomList* neighbours;
    struct EvidenceList* evidence;
    struct HunterArray* hunterArr;
    int numNeighbours;
    int numHunters;
} RoomType;

typedef struct RoomNode {
    RoomType* room;
    struct RoomNode* next;
} RoomNodeType;

typedef struct RoomList {
    RoomNodeType* head;
    RoomNodeType* tail;
} RoomListType;

typedef struct {
    GhostClassType type;
    RoomType* room;
    int boredom;
} GhostType;

typedef struct {
    EvidenceClassType type;
    float value;
} EvidenceType;

typedef struct EvidenceNode{
    EvidenceType* data;
    struct EvidenceNode* next;
} EvidenceNodeType;

typedef struct EvidenceList {
    EvidenceNodeType *head;
    EvidenceNodeType *tail;
} EvidenceListType;

typedef struct {
    RoomType* room;
    EvidenceClassType type; 
    EvidenceListType* evidence;
} HunterType;

typedef struct HunterArray {
    int size;
    HunterType* hunters[MAX_HUNTERS];
} HunterArrayType;


typedef struct Building {
    GhostType ghost;
    RoomListType rooms;
    HunterArrayType* hunterArray;
} BuildingType;

// Room functions
void populateRooms(BuildingType*);   // Populates the building with sample data for rooms
void initRoom(RoomType*, char*);        // Initializes the room, mallocs RoomListType, EvidenceListType, HunterType
void initRoomList(RoomListType*);       // Initializes the RoomListType, mallocs RoomNodeType
void appendRoom(RoomListType*, RoomNodeType*); // Appends room to end of roomlist
void connectRooms(RoomType*, RoomType*); // Creates new nodes and connects them together

// Evidence functions
void initEvidenceList(EvidenceListType*); // Initializes the EvidenceListType, mallocs EvidenceNodeType
void appendEvidence(EvidenceListType*, EvidenceType*); // Appends evidence to evidencelist

// Building functi
void initBuilding(BuildingType*);       // Initializes the building, mallocs RoomListType, EvidenceListType, HunterType
void populateRooms(BuildingType*);      // Populates the building with sample data for rooms

// WHEN INIT BUILDING MALLOC ROOMLIST