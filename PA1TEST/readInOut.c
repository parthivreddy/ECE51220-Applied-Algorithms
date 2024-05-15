#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "pa1.h"


//reading in time constraint

double readTime(char * str) {
    return atof(str);
}

//reading in inverter specs

bool readInverter(char * stream, double * invSpecs) {
    char buff[17];
    FILE * fp = fopen(stream, "r");
    if (fp == NULL) {
        fprintf(stderr, "failed to open %s\n", stream);
        return false;
    }

    for (int i = 0; i < 3; i++) {
        fscanf(fp, "%s", buff);
        invSpecs[i] = atof(buff);
    }
    fclose(fp);
    return true;
}

//read in wire specs
bool readWire(char * stream, double * wireSpecs) {
    char buff[17];
    FILE * fp = fopen(stream, "r");
    if (fp == NULL) {
        fprintf(stderr, "failed to open %s\n", stream);
        return false;
    }

    for (int i = 0; i < 2; i++) {
        fscanf(fp, "%s", buff);
        wireSpecs[i] = atof(buff);
    }
    fclose(fp);
    return true;
}

Node * createNode(double leftWire, double rightWire, double capacitance, int label, double sinkCap) {
    Node * node = malloc(sizeof(Node));
    if (node == NULL) {
        fprintf(stderr, "unable to make new node");
        return NULL;
    }
    node->left = NULL;
    node->right = NULL;
    node->leftWire = leftWire; // -1 if leaf node
    node->rightWire = rightWire; // -1 if leaf node
    node->label = label; // -1 if non leaf node
    node->capacitance = capacitance; //may have to create sink capacitance parameter as well
    node->delay = 0; //delay is 0 to start
    node->inverting = 1; //all are inverting to start (src is inverter)
    node->sinkCapacitance = sinkCap;
    node->sinkCap2 = sinkCap;
    //node->curDelay = 0; //0 to start
    node->curWr = 0;
    node->numInverters = 0;
    node->checkInverting = 1;
    node->checkCap = 0;
    node->checkDelay = 0;
    return node;
}

Node * createLeaf(FILE * fp, int label) {
    char buf[17];
    //fgetc(fp); //eat '('
    fgets(buf, 17, fp);
    fgetc(fp); //eat ')'
    double cap = atof(buf);
    return createNode(-1, -1, cap, label, cap);
}

Node * createInternal(FILE * fp) {
    char bufLeft[17];
    char bufRight[17];
    fgets(bufLeft, 17, fp);
    fgetc(fp); //eat white space
    fgets(bufRight, 17, fp);
    fgetc(fp); //eat ')'
    return createNode(atof(bufLeft), atof(bufRight), -1, -1, -1);
}

stackNode * push(Node * node, stackNode * head) {

    stackNode * sn = malloc(sizeof(stackNode));
    if (sn == NULL) {
        fprintf(stderr, "failed to allocate stackNode memory");
        return NULL;
    }

    sn->next = head;
    sn->curr = node;
    head = sn;
    return sn;
}

bool isEmpty(stackNode * head) {
    return head==NULL;
}

stackNode * pop(stackNode * head) {
    if(isEmpty(head)) {
        fprintf(stderr, "Can't pop, stack is empty");
        return NULL;
    }
    stackNode * curr = head->next;
    free(head);
    return curr;
}




//read in tree
Node * readTreeList(char * stream) {
    FILE * fp = fopen(stream, "r");
    if (fp == NULL) {
        fprintf(stderr, "failed to open %s\n", stream);
        return NULL;
    }

    stackNode * head = NULL;
    //int check = 1;
    char label = fgetc(fp);
    while (!feof(fp)) {
        //char label = fgetc(fp);
        int num = label;
        num = num - 48;
        if(label != '(') {
            //leaf node
            label = fgetc(fp);
            while(label != '(') {
                num = num * 10 + (label - 48);
                label = fgetc(fp);
            }
            Node * leafNode = createLeaf(fp, num);
            head = push(leafNode, head);
        }
        else {
            //check = 0;
            //internal node
            Node * internal = createInternal(fp);
            Node * Rightleaf = head->curr;
            head = pop(head);
            Node * Leftleaf = head->curr;
            head = pop(head);
            internal->right = Rightleaf;
            internal->left = Leftleaf;
            head = push(internal, head);

        }
        fgetc(fp); //eat new line
        label = fgetc(fp); // get label or (
    }
    fclose(fp);
    Node * curr = head->curr;
    head = pop(head);
    return curr;
}

void printTree(Node * root) {
    if(root == NULL) {
        return;
    }
    if(root->left == NULL || root->right == NULL) {
        if(root->right != NULL || root->left != NULL) {
            fprintf(stderr, "not a strictly binary tree!");
        } 
        printf("%d(%.10le)\n", root->label, root->capacitance);
    }
    
    else {
        printf("(%.10le %.10le)\n", root->leftWire, root->rightWire);
    }
    printTree(root->left);
    printTree(root->right);
}


void freeTree(Node * root) {
    if(root == NULL) {
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}


