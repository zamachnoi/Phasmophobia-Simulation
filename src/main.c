#include "defs.h"

int main(int argc, char *argv[])
{
    // Initialize the building
    BuildingType building;
    initBuilding(&building);
    srand(time(NULL));

    // Make the hunter and ghost structs
    HunterType hunter1;
    HunterType hunter2;
    HunterType hunter3;
    HunterType hunter4;
    GhostType ghost;

    //get the hunter names
    char hunterNames[MAX_HUNTERS][MAX_STR];
    int i;
    printf("Please enter hunter names...\n");
    for(i = 0; i < MAX_HUNTERS; i++) {
        printf("Hunter %d: ", i+1);
        scanf("%s", hunterNames[i]);
    }

    // Add rooms to building
    populateRooms(&building);
   
    // Initialize the hunters
    initHunter(&hunter1, hunterNames[0], building.rooms.head->room, EMF, &building);
    initHunter(&hunter2, hunterNames[1], building.rooms.head->room, TEMPERATURE, &building);
    initHunter(&hunter3, hunterNames[2], building.rooms.head->room, FINGERPRINTS, &building);
    initHunter(&hunter4, hunterNames[3], building.rooms.head->room, SOUND, &building);

    // Initialize the ghost
    initGhost(&ghost, &building);

    // Create the threads
    pthread_t hunter1Thread;
    pthread_t hunter2Thread;
    pthread_t hunter3Thread;
    pthread_t hunter4Thread;

    pthread_t ghostThread;

    // Start the threads
    pthread_create(&hunter1Thread, NULL, hunterMove, &hunter1);
    pthread_create(&hunter2Thread, NULL, hunterMove, &hunter2);
    pthread_create(&hunter3Thread, NULL, hunterMove, &hunter3);
    pthread_create(&hunter4Thread, NULL, hunterMove, &hunter4);
    
    pthread_create(&ghostThread, NULL, ghostMove, &ghost);

    // Bring the threads back
    pthread_join(ghostThread, NULL);
    pthread_join(hunter1Thread, NULL);
    pthread_join(hunter2Thread, NULL);
    pthread_join(hunter3Thread, NULL);
    pthread_join(hunter4Thread, NULL);

    
    // Print the results
    printResults(&hunter1, &hunter2, &hunter3, &hunter4, &ghost);
    

//    printHunter(&hunter1);
//     printHunter(&hunter2);
//     printHunter(&hunter3);
//     printHunter(&hunter4);

    // printBuilding(&building);

    // Cleanup everything
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