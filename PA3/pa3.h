#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "graph.h"

//Readin Functios
Node * createNode(float, int);
headNode * createHeadNode();
headNode ** createGraph(char *, int *, int *);

void freeGraph(headNode **, int);

void printGraph(headNode **, int);

void printMCM(char *);
void freePred(int **, int);
void freeDist(double **, int);

//karp functions
double ** distanceTable(int, int);
int ** predTable(int, int);
bool checkK(int);
float MCM(char *, headNode **, int, int, int *);
bool detectCycle(int, headNode **, headNode *, int **, int, int **, double *, double **, int, int*, int*);
void resetVisited(headNode **, int *, int);
double * piVals(headNode **, double, double **, int, int);
bool evalMCM(headNode **, double, double *, int);

//output functions
void writeOut2(char *, float);
void printPredTable(int **, int, int);
void printDistTable(double **, int, int);
void writeCycle(char *, int *, int, int);
void printCycle(int *, int, int);