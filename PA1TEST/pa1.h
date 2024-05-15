#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "tree.h"

//readinout.c functions
double readTime(char *);

bool readInverter(char *, double *);

bool readWire(char *, double *);

Node * createNode(double, double, double, int, double);

Node * createLeaf(FILE *, int);

Node * createInternal(FILE *);

stackNode * push(Node *, stackNode *);

bool isEmpty(stackNode *);

stackNode * pop(stackNode *);

Node * readTreeList(char *);

void printTree(Node *);

void freeTree(Node *);

//pa1.c functions
wireNode * pushWire(wireNode *, double, double);

wireNode * popWire(wireNode *);

void upstreamCaps(Node *, double, double);

wireNode * upstreamCapsHelper(Node *, wireNode *, double, double);

void downstreamDelay(Node *, double , double);

wireNode * downstreamDelayHelper(Node *, wireNode *, double, double);

void printTreeCaps(Node *);

void printTreeDelays(Node *);

bool writeDelaysHelper(char *, Node *);

void writeDelays(FILE *, Node *);

void readDelays(char *);

bool checkStringent(double *, double);

bool printPreorderHelper(char *, Node *);

void printPreorder(FILE *, Node *);

//placeInverters.c functions

Node * bottomUp(Node *, double, Node *, double *, double *, Node *, int *);

double calcInvDownDelay(double *, double *, double, Node *, Node *);

double calcMaxWrLen(Node *, double *, double *, double, double, Node *, int *);

//delay.c functions
void upstreamsinkCaps(Node *, Node *, double, double *, Node *);

void downstreamsinkDelayHelper(Node *, Node *, double, double, double *, Node *);

double downstreamsinkDelay(Node *, double, double, Node *);

Node * createInverter(Node *, double, double, double *, double *);

Node * convertParent(Node *, double);

//checkValid.c functions

bool checkInverting(Node *, Node *);

bool checkDelayHelper(Node *, double, double *, double *, Node *);

void checkDelay(Node *, Node *, double *, double *, double *, Node *);

void checkUpCaps(Node *, Node *, double *, double *, Node *, Node *);

void checkDownDelays(Node *, Node *, double, double, double *, Node *, Node *);

//output.c functions
bool writeTextHelper(char *, Node *);

void writeText(FILE *, Node *);

bool writeOutHelper(char *, Node *);

void writeOut(FILE *, Node *);

void readRC(char *);