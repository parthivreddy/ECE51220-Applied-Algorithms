#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "pa3.h"


int main(int argc, char ** argv) {
    if(argc != 4) {
        fprintf(stderr, "Not enough arguments\n");
        return EXIT_FAILURE;
    }
    int edgeNum;
    int nodeNum;
    headNode ** adjList = createGraph(argv[1], &edgeNum, &nodeNum);
    if(adjList == NULL) {
        fprintf(stderr, "Unable to create graph\n");
        return EXIT_FAILURE;
    }
    //printGraph(adjList, nodeNum);
    int checkValid = 0;
    float mcm = MCM(argv[3], adjList, edgeNum, nodeNum, &checkValid);
    if(!checkValid) {
        fprintf(stderr, "Can't produce a valid MCM\n");
        return EXIT_FAILURE;
    }
    //printf("Calc MCM: %f\n", mcm);
    writeOut2(argv[2], mcm);
    freeGraph(adjList, nodeNum);
    //printMCM(argv[2]);
    return EXIT_SUCCESS;
}