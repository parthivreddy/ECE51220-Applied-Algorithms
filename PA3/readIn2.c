#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "pa3.h"

Node * createNode(float weight, int label) {
    Node * node = malloc(sizeof(Node));
    if(node == NULL) {
        fprintf(stderr, "Couldn't allocate node mem\n");
        return NULL;
    }
    node->next = NULL;
    node->tail = NULL;
    node->weight = weight;
    node->label = label;
    
    return node;
}

headNode * createHeadNode() {
    headNode * node = malloc(sizeof(headNode));
    if(node == NULL) {
        fprintf(stderr, "Couldn't allocate head node mem\n");
        return NULL;
    }
    //node->sources = NULL;
    node->dests = NULL;
    node->visited = false;
    return node;
}



headNode ** createGraph(char * filename, int * edgeNum, int * nodeNum) {
    FILE * fp = fopen(filename, "r");
    if(fp == NULL) {
        fprintf(stderr, "Can't open %s\n", filename);
        return NULL;
    }
    fgetc(fp); //eat V
    fgetc(fp); //eat space
    char nNum = fgetc(fp);
    *nodeNum = (int) nNum - 48;
    nNum = fgetc(fp); //eats new line or next val
    while(nNum != '\n') {
        *nodeNum = *nodeNum * 10 + ((int)nNum - 48);
        nNum = fgetc(fp);
    }
    fgetc(fp); //eat E
    fgetc(fp); //eat space
    char nEdge = fgetc(fp);
    *edgeNum = (int) nEdge - 48;
    nEdge = fgetc(fp);
    while(nEdge != '\n') {
        *edgeNum = *edgeNum * 10 + ((int)nEdge - 48);
        nEdge = fgetc(fp);
    }
    //fgetc(fp); //eat newline

    headNode ** adjList = malloc(*nodeNum * sizeof(headNode *)); 
    if(adjList == NULL) {
        fprintf(stderr, "Couldn't allocate memory for adjlist\n");
        return NULL;
    }
    for(int i = 0; i < *nodeNum; i++) {
        adjList[i] = createHeadNode();
    }
    char label = fgetc(fp);
    while(!feof(fp)) {
        int dest;
        int source;
        dest = (int) label - 48;
        label = fgetc(fp);
        while(label != ' ') {
            dest = dest * 10 + ((int)label - 48);
            label = fgetc(fp);
        }
        label = fgetc(fp);
        source = (int) label - 48;
        label =fgetc(fp);
        while(label != ' ') {
            source = source * 10 + ((int)label - 48);
            label = fgetc(fp);
        }
        double weight;
        fscanf(fp, "%lf", &weight);
        fgetc(fp); //eat new line maybe dont need
        //Here tail will be NULL until populated
        Node * destNode = createNode(weight, dest);
        //Node * sourceNode = createNode(weight, source);
        if(adjList[source - 1]->dests == NULL) {
            adjList[source - 1]->dests = destNode;
        }
        else {
            adjList[source - 1]->dests->tail->next = destNode;
        }
        // if(adjList[dest - 1]->sources == NULL) {
        //     adjList[dest - 1]->sources = sourceNode;
        // }
        // else {
        //     adjList[dest - 1]->sources->tail->next = sourceNode;
        // }
        
        adjList[source - 1]->dests->tail = destNode;
        //adjList[dest - 1]->sources->tail = sourceNode;
        label = fgetc(fp);
    }
    fclose(fp);
    return adjList;

}

void freeGraph(headNode ** adjList, int numNodes) {
    for(int i = 0; i < numNodes; i++) {
        Node * dest = adjList[i]->dests;
        //Node * source = adjList[i]->sources;
        Node * nextDest = dest->next;
        // Node * sourceNext = source->next;
        while(nextDest != NULL) {
            free(dest);
            dest = nextDest;
            nextDest = dest->next;
        }
        free(dest);
        // while(sourceNext != NULL) {
        //     free(source);
        //     source = sourceNext;
        //     sourceNext = source->next;
        // }
        // free(source);
        free(adjList[i]);
    }
    free(adjList);
    return;
}

void printGraph(headNode ** adjList, int numNodes) {
    for(int i = 0; i < numNodes; i++) {
        headNode * currHead = adjList[i];
        //Node * source = currHead->sources;
        Node * dest = currHead->dests;
        // while(source != NULL) {
        //     printf("Edges going to: %d; Source:%d, weight: %f\n", i+1, source->label, source->weight);
        //     source = source->next;
        // }
        while(dest != NULL) {
            printf("Edges going from: %d; Dest:%d, weight: %f\n", i+1, dest->label, dest->weight);
            dest = dest->next;
        }
        printf("------------\n");
    }
}

void printMCM(char * filename) {
    FILE * fp = fopen(filename, "rb");
    if(fp == NULL) {
        fprintf(stderr, "Can't open %s\n", filename);
        return;
    }
    float MCM;
    fread(&MCM, sizeof(MCM), 1, fp);
    printf("MCM: %lf\n", MCM);
    fclose(fp);
    return;
}

void freePred(int ** predTable, int numEdges) {
    for(int i = 0; i <= numEdges; i++) {
        free(predTable[i]);
    }
    free(predTable);
}

void freeDist(double ** distTable, int numEdges) {
    for(int i = 0; i <= numEdges; i++) {
        free(distTable[i]);
    }
    free(distTable);
}