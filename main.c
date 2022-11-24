#include "defs.h"

int main(int argc, char *argv[])
{
    BuildingType building;
    initBuilding(&building);
    //get the hunter names
    char* hunterNames[MAX_HUNTERS];
    int i;
    // for(i = 0; i < MAX_HUNTERS; i++) {
    //     scanf("%s", hunterNames[i]);
    // }
    // populate Rooms
    srand(time(NULL));

    populateRooms(&building);


    //create ghost
    GhostType ghost;
    initGhost(&ghost, &building);
    printf("%d\n", ghost.type);
    pthread_t ghostThread;
    pthread_create(&ghostThread, NULL, ghostMove, &ghost);
    
    pthread_join(ghostThread, NULL);
    printBuilding(&building);

    // populateRooms(&building);
    // initGhost(&ghost, &building);
    // printf("The ghosts room is now in: ");
    // printRoom(ghost.room);
    // ghostMove(&ghost);
    // printf("The ghosts room is now in: ");
    // printRoom(ghost.room);
    // ghostMove(&ghost);
    // printf("The ghosts room is now in: ");
    // printRoom(ghost.room);
    // ghostMove(&ghost);
    // printf("The ghosts room is now in: ");
    // printRoom(ghost.room);
    // ghostMove(&ghost);
    // printf("The ghosts room is now in: ");
    // printRoom(ghost.room);
    // ghostMove(&ghost);
    // printf("The ghosts room is now in: ");
    // printRoom(ghost.room);
    // ghostMove(&ghost);
    // printf("The ghosts room is now in: ");
    // printRoom(ghost.room);

    // printBuilding(&building);
    cleanupRoomList(&building.rooms);

    // return 0;
}


/*
  Function:  randInt
  Purpose:   returns a pseudo randomly generated number, 
             in the range min to (max - 1), inclusively
       in:   upper end of the range of the generated number
   return:   randomly generated integer in the range [min, max-1) 
*/
int randInt(int min, int max)
{
    return rand() % (max - min) + min;
}

/*
  Function:  randFloat
  Purpose:   returns a pseudo randomly generated number, 
             in the range min to max, inclusively
       in:   upper end of the range of the generated number
   return:   randomly generated integer in the range [0, max-1) 
*/
float randFloat(float a, float b) {
    // Get a percentage between rand() and the maximum
    // between rand() and RAND_MAX
    float random = ((float) rand()) / (float) RAND_MAX;
    // Scale it to the range we want, and shift it
    return random * (b - a) + a;


}