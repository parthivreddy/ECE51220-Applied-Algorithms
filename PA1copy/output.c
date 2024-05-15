#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "pa1.h"

bool writeTextHelper(char * filename, Node * root) {
    FILE * fp = fopen(filename, "w");
    if(fp == NULL) {
        fprintf(stderr, "Can't write to %s\n", filename);
        return false;
    }
    writeText(fp, root);
    fclose(fp);
    return true;
}
void writeText(FILE * fp, Node * curr) {
    //post order 
    if(curr == NULL) {
        return;
    }
    writeText(fp, curr->left);
    writeText(fp, curr->right);
    if(curr->label == -1) {//internal node
        fprintf(fp, "(%.10le %.10le %d)\n", curr->leftWire, curr->rightWire, curr->numInverters);
    }
    else {
        fprintf(fp, "%d(%.10le)\n", curr->label, curr->sinkCap2);
    }

}

bool writeOutHelper(char * filename, Node * root) {
    FILE * fp = fopen(filename, "wb");
    if(fp == NULL) {
        fprintf(stderr, "Can't write to %s\n", filename);
        return false;
    }
    writeOut(fp, root);
    fclose(fp);
    return true;
}

void writeOut(FILE * fp, Node * curr) {
    if(curr == NULL) {
        return;
    }
    writeOut(fp, curr->left);
    writeOut(fp, curr->right);
    if(curr->label != -1) {
        fwrite(&(curr->label), 1, sizeof(int), fp);
        fwrite(&(curr->sinkCap2), 1, sizeof(double), fp);
    }
    else {
        fwrite(&(curr->label), 1, sizeof(int), fp);
        fwrite(&(curr->leftWire), 1, sizeof(double), fp);
        fwrite(&(curr->rightWire), 1, sizeof(double), fp);
        fwrite(&(curr->numInverters), 1, sizeof(int), fp);
    }

}

void readRC(char * filename) {
    FILE * fp = fopen(filename, "rb");
    int label;
    int numInverters;
    double sinkCap;
    double leftWire;
    double rightWire;
    while(!feof(fp)) {
        if(fread(&label, sizeof(int),1,fp) == 1) {
            if(label != -1) {
                fread(&sinkCap, sizeof(double), 1, fp);
                printf("%d(%.10le)\n", label, sinkCap);
            }
            else {
                fread(&leftWire, sizeof(double), 1, fp);
                fread(&rightWire, sizeof(double), 1, fp);
                fread(&numInverters, sizeof(int), 1, fp);
                printf("%d(%.10le %.10le %d)\n", label, leftWire, rightWire, numInverters);
            }
        }

    }
    fclose(fp);
}