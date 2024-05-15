#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "tree.h"

//readin functions
stackNode * push(Node *, stackNode *);
bool isEmpty(stackNode *);
stackNode * pop(stackNode *);
Node * createDiv(char);
//Node * createNode(char *, char, __ssize_t);
Node * createNode(int, FILE*);
Node * readTree(char *);
void freeTree(Node *);
void printTree(Node *);

//firstRep functions
long max(long, long);
int checkType(Node*);
bool checkDiv(Node*);
int getLeftWidth(Node*, int);
int getRightWidth(Node*, int);
int getLeftLength(Node*, int);
int getRightLength(Node*, int);
void storeMaxDims(Node*);
void storeCoords(Node *, int, int);

//optimal functions
void merge(Node *, int, int, int, int*);
void mergeSort(Node *, int, int, int*);
void sortNodeReps(Node *, int*);
void combineOptions(Node *, int*);
void choosePacking(Node *);
void choosePackingReps(Node *);

//output functions
void printOut13(char *, Node *);
void printOut24Helper(char *, Node *);
void printOut24(FILE *, Node *);