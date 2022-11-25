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

    //create hunters
    HunterType hunter1;
    HunterType hunter2;
    HunterType hunter3;
    HunterType hunter4;
    initHunter(&hunter1, "Hunter1", building.rooms.head->room, EMF, &building);
    initHunter(&hunter2, "Hunter2", building.rooms.head->room, TEMPERATURE, &building);
    initHunter(&hunter3, "Hunter3", building.rooms.head->room, FINGERPRINTS, &building);
    initHunter(&hunter4, "Hunter4", building.rooms.head->room, SOUND, &building);
    pthread_t hunter1Thread;
    pthread_t hunter2Thread;
    pthread_t hunter3Thread;
    pthread_t hunter4Thread;
    pthread_create(&hunter1Thread, NULL, hunterMove, &hunter1);
    pthread_create(&hunter2Thread, NULL, hunterMove, &hunter2);
    pthread_create(&hunter3Thread, NULL, hunterMove, &hunter3);
    pthread_create(&hunter4Thread, NULL, hunterMove, &hunter4);

    
    pthread_join(ghostThread, NULL);
    pthread_join(hunter1Thread, NULL);
    pthread_join(hunter2Thread, NULL);
    pthread_join(hunter3Thread, NULL);
    pthread_join(hunter4Thread, NULL);

    
    
    printBuilding(&building);

    printHunter(&hunter1);
    printHunter(&hunter2);
    printHunter(&hunter3);
    printHunter(&hunter4);   


   

    // printBuilding(&building);
    cleanupHunter(&hunter1);
    cleanupHunter(&hunter2);
    cleanupHunter(&hunter3);
    cleanupHunter(&hunter4);

    cleanupBuilding(&building);


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