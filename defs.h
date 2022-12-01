#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_STR            64
#define FEAR_RATE           1
#define MAX_FEAR          100
#define MAX_HUNTERS         4
#define USLEEP_TIME         0
#define BOREDOM_MAX        99

// You may rename these types if you wish
typedef enum { EMF, TEMPERATURE, FINGERPRINTS, SOUND } EvidenceClassType;
typedef enum { POLTERGEIST, BANSHEE, BULLIES, PHANTOM } GhostClassType;



int randInt(int, int);          // Generates a pseudorandom integer between the parameters
float randFloat(float, float);  // Generates a pseudorandom float between the parameters

typedef struct {
    char name[MAX_STR]; //name of room
    struct RoomList* neighbours; //list of rooms that are connected to this room
    struct EvidenceList* evidence; //list of evidence in this room
    struct HunterArray* hunterArr; //array of hunters in this room
    int numHunters; //number of hunters in room
    struct GhostType* ghost; //ghost in room
    sem_t mutex; //mutex for room

} RoomType;

typedef struct RoomNode {
    RoomType* room;
    struct RoomNode* next;
} RoomNodeType;

typedef struct RoomList {
    RoomNodeType* head;
    RoomNodeType* tail;
    int size;
} RoomListType;

typedef struct {
    GhostClassType type;
    RoomType* room;
    struct Building* building;
    int boredom;
} GhostType;

typedef struct {
    EvidenceClassType type; // EMF, TEMPERATURE, FINGERPRINTS, SOUND
    float value;
} EvidenceType;

typedef struct EvidenceNode{
    EvidenceType* data;
    struct EvidenceNode* next;
} EvidenceNodeType;

typedef struct EvidenceList {
    EvidenceNodeType *head;
    EvidenceNodeType *tail;
    int size;
} EvidenceListType;

typedef struct {
    char name[MAX_STR];
    RoomType* room;
    EvidenceClassType type;  // EMF, TEMPERATURE, FINGERPRINTS, SOUND
    EvidenceListType* ghostlyEvidence; // list of ghostly evidence collected
    EvidenceListType* nonGhostlyEvidence; // list of evidence collected
    int fear;
    int boredom;
    struct Building* building;
} HunterType;

typedef struct HunterArray {
    int size; // number of hunters in the array
    HunterType* hunters[MAX_HUNTERS]; // array of pointers to hunters
} HunterArrayType;


typedef struct Building {
    RoomListType rooms; // list of rooms in the building
    EvidenceListType evidence; // list of evidence collected
} BuildingType;

// Room functions
void populateRooms(BuildingType*);   // Populates the building with sample data for rooms
void initRoom(RoomType*, char*);        // Initializes the room, mallocs RoomListType, EvidenceListType, HunterType
void initRoomList(RoomListType*);       // Initializes the RoomListType, mallocs RoomNodeType
void appendRoom(RoomListType*, RoomNodeType*); // Appends room to end of roomlist
void connectRooms(RoomType*, RoomType*); // Creates new nodes and connects them together

void cleanupNeighbourList(RoomListType*);

// Evidence functions
void initEvidenceList(EvidenceListType*); // Initializes the EvidenceListType, mallocs EvidenceNodeType
void appendEvidence(EvidenceListType*, EvidenceType*); // Appends evidence to evidencelist
void initEvidence(EvidenceType*, EvidenceClassType, float);
void printEvidenceList(EvidenceListType*);
void printEvidence(EvidenceType*);
void getRandomEvidence(EvidenceListType*, EvidenceType**);
int checkEvidence(EvidenceClassType, float);
int checkIfHaveEvidence(HunterType*, EvidenceType*);
int checkEvidenceThreeTypes(HunterType*);

// Building functi
void initBuilding(BuildingType*);       // Initializes the building, mallocs RoomListType, EvidenceListType, HunterType
void populateRooms(BuildingType*);      // Populates the building with sample data for rooms
void printBuilding (BuildingType*);      // Prints the building
void printNeighbours(RoomNodeType*);    // Prints the neighbours of a room


void cleanupRoomList(RoomListType*);
void cleanupEvidenceList(EvidenceListType*);
void cleanupRoomData(RoomType*);
void cleanupEvidenceData(EvidenceListType*);


//Ghost functions
void* ghostMove(void*);
void leaveEvidence(GhostType*);
float generateEvidence(EvidenceClassType, int);
void moveGhostRoom(GhostType*);
void initGhost(GhostType*, BuildingType*);
RoomType* initGhostRoom(GhostType*, BuildingType*);
GhostClassType initGhostType();
void printRoom(RoomType*);

// Hunter functions
void* hunterMove(void*);
void initHunter(HunterType*, char*, RoomType*, EvidenceClassType, BuildingType*);
void pickupEvidence(HunterType*);
void getEvidence(HunterType*);
void moveHunterRoom(HunterType*);
void removeHunter(HunterType*);
void addHunter(HunterType*);
void communicate(HunterType*);
void initHunterArray(HunterArrayType*);
void printHunter(HunterType*);
void cleanupHunter(HunterType*);

void cleanupBuilding(BuildingType*);
void printResults(HunterType*, HunterType*, HunterType*, HunterType*, GhostType*); 


// WHEN INIT BUILDING MALLOC ROOMLIST
void calculateGhost(HunterType*);
int checkFear(HunterType*);

void printResults(HunterType*, HunterType*, HunterType*, HunterType*, GhostType*);
int checkFear(HunterType*);
void calculateGhost(HunterType*);