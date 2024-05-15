#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "pa3.h"

void writeOut2(char * filename, float mcm) {
    FILE * fp = fopen(filename, "wb");
    if(fp == NULL) {
        fprintf(stderr, "Can't open %s\n", filename);
        return;
    }
    fwrite(&mcm, sizeof(float), 1, fp);
    fclose(fp);
}

void printPredTable(int ** predTable, int numEdges, int numNodes) {
    for(int i = 0; i <= numEdges; i++) {
        for(int j = 0; j < numNodes; j++) {
            printf("(%d, %d): %d :: ", i, j, predTable[i][j]);
        }
        printf("\n");
    }
}

void printDistTable(double ** distTable, int numEdges, int numNodes) {
    for(int i = 0; i <= numEdges; i++) {
        for(int j = 0; j < numNodes; j++) {
            printf("(%d, %d): %lf :: ", i, j, distTable[i][j]);
        }
        printf("\n");
    }
}

void writeCycle(char * filename, int * cycle, int startCycleInd, int endCycleInd) {
    FILE * fp = fopen(filename, "w");
    if(fp == NULL) {
        fprintf(stderr, "Can't open %s\n", filename);
        return;
    }
    for(int i = startCycleInd; i < endCycleInd; i++) {
        fprintf(fp, "%d ", cycle[i]);
    }
    fprintf(fp, "%d\n", cycle[endCycleInd]);
    fclose(fp);
}

void printCycle(int * cycle, int start, int end) {
    printf("\nCycle: ");
    for(int i = start; i <= end; i++) {
        printf("%d ", cycle[i]);
    }
}
