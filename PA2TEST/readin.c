//#define _GNU_SOURCE
//#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "pa2.h"




stackNode * push(Node * node, stackNode * head) {

    stackNode * sn = malloc(sizeof(stackNode));
    if (sn == NULL) {
        fprintf(stderr, "failed to allocate stackNode memory\n");
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
        fprintf(stderr, "Can't pop, stack is empty\n");
        return NULL;
    }
    stackNode * curr = head->next;
    free(head);
    return curr;
}

// Node * createDiv(char label2) {
//     Node * node = malloc(sizeof(Node));
//     node->label2 = label2;
//     node->label = -1;
//     node->left = NULL;
//     node->right = NULL;
//     node->numReps = 0;
//     node->maxLength = 0;
//     node->maxWidth = 0;
//     node->addLength = 0;
//     node->addWidth = 0;
//     return node;
// }

Node * createDiv(char label2) {
    Node * node = malloc(sizeof(Node));
    if(node == NULL) {
        fprintf(stderr, "failed to allocate div node mem\n");
        return NULL;
    }
    node->label2 = label2;
    node->label = -1;
    node->left = NULL;
    node->right = NULL;
    node->numReps = 0;
    node->originX = 0;
    node->originY = 0;
    node->offsetX = 0;
    node->offsetY = 0;
    node->rep = 0;
    node->bestRep = 0;
    node->allReps = 0;
    return node;
}

Node * createNode(int num, FILE * fp) {
    Node * node = NULL;
    node = malloc(sizeof(Node));
    if(node == NULL) {
        fprintf(stderr, "failed to allocate leaf node mem\n");
        return NULL;
    }
    node->originX = 0;
    node->originY = 0;
    node->offsetX = 0;
    node->offsetY = 0;
    node->left = NULL;
    node->right = NULL;
    node->label2 = 'a';
    node->label = num;
    node->rep = 0;
    node->bestRep = 0;
    int numReps = 0;
    while(fgetc(fp) != ')') { //eats (
        ++numReps;
        if(numReps - 1 == 0) {
            node->dims = malloc(sizeof(long*));
            if(node->dims == NULL) {
                free(node);
                fprintf(stderr, "can't allocate memory for node->dims\n");
                return NULL;
            }
        }
        else {
            long ** temp;
            temp = realloc(node->dims, numReps * sizeof(long*));
            if(temp == NULL) {
                for(int i = 0; i < (numReps - 1); i++) {
                    free(node->dims[i]);
                }
                free(node->dims);
                free(node);
                fprintf(stderr, "Can't reallocate memory\n");
                return NULL;
            }
            node->dims = temp;
        }
        node->dims[numReps-1] = malloc(2*sizeof(long));
        if(node->dims[numReps - 1] == NULL) {
            for(int i = 0; i < (numReps - 1); i++) {
                free(node->dims[i]);
            }
            free(node->dims);
            free(node);
            fprintf(stderr, "Can't allocate rep memory\n");
            return NULL;
        }
        //printf("%d:%p\n", node->label, *(node->dims));
        char widthChar = fgetc(fp);
        int width = (int) widthChar - 48;
        widthChar = fgetc(fp); //eat comma or next char
        while(widthChar != ',') {
            width = width * 10 + ((int) widthChar - 48);
            widthChar = fgetc(fp);
        }
        node->dims[numReps-1][0] = width;
        //fgetc(fp);
        //fgetc(fp); //eat comma
        char lengthChar = fgetc(fp);
        int length = (int) lengthChar - 48;
        lengthChar = fgetc(fp); //eats ) or next char
        while(lengthChar != ')') {
            length = length * 10 + ((int) lengthChar - 48);
            lengthChar = fgetc(fp);
        }
        node->dims[numReps-1][1] = length;
        //fgetc(fp);
        //fgetc(fp); //eat )
        //printf("%c\n", fgetc(fp));
    }
    node->numReps = numReps;
    node->allReps = node->numReps;
    return node;
}

Node * readTree(char * filename) {
    FILE * fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Cant open %s\n", filename);
        return NULL;
    }

    stackNode * head = NULL;
    char label = fgetc(fp);
    
    while(!feof(fp)) {
        if(label == 'V' || label == 'H') {
            Node * nodeDiv = createDiv(label);
            if(nodeDiv == NULL) {
                while(head != NULL) {
                    freeTree(head->curr);
                    head = pop(head);
                }
                fclose(fp);
                return NULL;
            }
            Node * nodeRight = head->curr;
            // if(isEmpty(head)) {
            //     fprintf(stderr, "Malformed input file\n");
            //     freeTree(nodeDiv);
            //     return NULL;
            // }
            head = pop(head);
            Node * nodeLeft = head->curr;
            // if(isEmpty(head)) {
            //     fprintf(stderr, "Malformed input file\n");
            //     freeTree(nodeRight);
            //     freeTree(nodeDiv);
            //     return NULL;
            // }
            head = pop(head);
            nodeDiv->left = nodeLeft;
            nodeDiv->right = nodeRight;
            stackNode * tempcheck = head;
            head = push(nodeDiv, head);
            if(head == NULL) {
                while(tempcheck != NULL) {
                    freeTree(tempcheck->curr);
                    tempcheck = pop(tempcheck);
                }
                fclose(fp);
                return NULL;
            }
        }
        else {
            int num = (int) label - 48;
            label = fgetc(fp);
            while(label != '(') {
                //printf("%c\n", label);
                num = num * 10 + ((int)label - 48);
                label = fgetc(fp);
                //printf("%c\n", label);

            }
            //printf("hello\n");
            Node * node = createNode(num, fp);
            if(node == NULL) {
                while(head != NULL) {
                    freeTree(head->curr);
                    head = pop(head);
                }
                fclose(fp);
                return NULL;
            }
            stackNode * tempcheck = head;
            head = push(node, head);
            if(head == NULL) {
                while(tempcheck != NULL) {
                    freeTree(tempcheck->curr);
                    tempcheck = pop(tempcheck);
                }
                fclose(fp);
                return NULL;
            }
        }
        fgetc(fp); //eat newline
        label = fgetc(fp);
    }
    fclose(fp);
    Node * curr = head->curr;
    head = pop(head);
    return curr;
}

void freeTree(Node * root) {
    if(root == NULL) {
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    //printf("%lu\n", sizeof(root->dims));

    for(int i = 0; i < root->allReps; i++) {
        //printf("dims: (%d,%d)", root->dims[i][0], root->dims[i][1]);
        //printf("ADDr of freeMem: %p\n", &(root->dims[i]));
        free(root->dims[i]);
    }
    //printf("\n");
    if(root->allReps > 0) {
        free(root->dims);
    }

    free(root);
}

void printTree(Node * root) {
    if(root == NULL) {
        return;
    }
    if(root->label == -1) { //division node
        printf("%c\n", root->label2);
    }
    else {
        printf("%d(", root->label);
        for(int i = 0; i < root->numReps; i++) {
            printf("(%ld,%ld)", root->dims[i][0], root->dims[i][1]);
        }
        printf(")\n");
    }
    printTree(root->left);
    printTree(root->right);
}