#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "pa3.h"

Node * createNode1(float weight, int label) {
    Node * node = malloc(sizeof(Node));
    if(node == NULL) {
        fprintf(stderr, "Couldn't allocate node mem\n");
        return NULL;
    }
    node->next = NULL;
    node->weight = weight;
    node->label = label;
    
    return node;
}

headNode * createHeadNode1() {
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



Node ** createGraph1(char * filename, int * edgeNum, int * nodeNum) {
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

    Node ** adjList = malloc(*nodeNum * sizeof(Node *)); 
    if(adjList == NULL) {
        fprintf(stderr, "Couldn't allocate memory for adjlist\n");
        return NULL;
    }
    for(int i = 0; i < *nodeNum; i++) {
        adjList[i] = NULL;
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
        float weight;
        fscanf(fp, "%f", &weight);
        fgetc(fp); //eat new line maybe dont need
        //Here tail will be NULL until populated
        Node * node = createNode(weight, dest);
        if(adjList[source - 1] == NULL) {
            adjList[source - 1] = node;
            
        }
        else {
            adjList[source - 1]->tail->next = node;
        }
        
        adjList[source - 1]->tail = node;
        label = fgetc(fp);
    }
    fclose(fp);
    return adjList;

}

void freeGraph1(Node ** adjList, int numNodes) {
    for(int i = 0; i < numNodes; i++) {
        Node * curr = adjList[i];
        Node * next = adjList[i]->next;
        while(next != NULL) {
            free(curr);
            curr = next;
            next = curr->next;
        }
        free(curr);
    }
    free(adjList);
    return;
}

void printGraph1(Node ** adjList, int numNodes) {
    for(int i = 0; i < numNodes; i++) {
        Node * curr = adjList[i];
        while(curr != NULL) {
            printf("source: %d, dest:%d, weight: %f\n", i+1, curr->label, curr->weight);
            curr = curr->next;
        }
    }
}

void printMCM1(char * filename) {
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