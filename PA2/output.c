#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "pa2.h"

void printOut13(char * filename, Node * root) {
    FILE * fp = fopen(filename, "w");
    //printf("(%d,%d)\n", root->offsetX, root->offsetY);
    fprintf(fp, "(%d,%d)\n", root->offsetX, root->offsetY);
    fclose(fp);
}

void printOut24Helper(char * filename, Node * root) {
    FILE * fp = fopen(filename, "w");
    printOut24(fp, root);
    fclose(fp);
}
void printOut24(FILE* fp, Node * node) {
    if(node == NULL) {
        return;
    }
    printOut24(fp, node->left);
    printOut24(fp, node->right);
    if(node->label == -1) {
        return;
    }
    //printf("%d((%d,%d)(%d,%d))\n", node->label, node->dims[node->bestRep][0], node->dims[node->bestRep][1], node->originX, node->originY);
    fprintf(fp, "%d((%d,%d)(%d,%d))\n", node->label, node->dims[node->bestRep][0], node->dims[node->bestRep][1], node->originX, node->originY);
}