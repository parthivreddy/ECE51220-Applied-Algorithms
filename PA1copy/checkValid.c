#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "pa1.h"



bool checkInverting(Node * curr, Node * parent) { //checks if tree is a valid tree in that it is non-inverting
    if(curr == NULL) {
        return true;
    }
    if(parent != NULL) {
        if(parent->numInverters > 0) {
            curr->checkInverting = (parent->checkInverting == 0) ? 1 : 0;
        }
        else {
            curr->checkInverting = parent->checkInverting;
        }
    }
    else {
        curr->checkInverting = 1;
    }
    if(curr->label != -1) { //leaf node
        if(curr->checkInverting == 1) {
            return false;
        }
    }
    bool check;
    check = checkInverting(curr->left, curr);
    if(!check) { //if one is false then whole tree is wrong
        return check;
    }
    check = checkInverting(curr->right, curr);
    if(!check) {
        return check;
    }
    return true;
}

//function to check if each stage falls within delay constraint

bool checkDelayHelper(Node * curr, double maxDelay, double * invSpecs, double * wrSpecs, Node * root) {
    double checkMaxDelay = 0;
    checkDelay(curr, NULL, invSpecs, wrSpecs, &checkMaxDelay, root);
    if(checkMaxDelay > maxDelay) {
        return false;
    }
    return true;
}
void checkDelay(Node * curr, Node * parent, double * invSpecs, double * wrSpecs, double * checkMaxDelay, Node * root) {
    if(curr == NULL) {
        return;
    }
    if(curr->numInverters > 0 || curr == root) {
        checkUpCaps(curr, NULL, wrSpecs, invSpecs, curr, root); //use upstream inverter capacitance
        //also need downstream capacitance for when inverter is root node
        checkDownDelays(curr, NULL, wrSpecs[1], invSpecs[2], checkMaxDelay, curr, root);
    }
    checkDelay(curr->left, curr, invSpecs, wrSpecs, checkMaxDelay, root);
    checkDelay(curr->right, curr, invSpecs, wrSpecs, checkMaxDelay, root);
    
    return;
}

void checkUpCaps(Node * curr, Node * parent, double * wrSpecs, double * invSpecs, Node * root, Node * rootroot) {
    if(curr == NULL) {
        return;
    }
    if(parent != NULL && parent != root) { //doesn't work if inverter is root
        if(parent->numInverters > 0) {
            return;
        }
    }
    if(curr->left != NULL) {
        checkUpCaps(curr->left, curr, wrSpecs, invSpecs, root, rootroot);
    }
    if(curr->right != NULL) {
        checkUpCaps(curr->right, curr, wrSpecs, invSpecs, root, rootroot);
    }

    if(parent != NULL) {
        if(parent->left == curr) {
            curr->upWire = parent->leftWire;
        }
        else {
            curr->upWire = parent->rightWire;
        }
        double wireLength = curr->upWire;
        if(curr->label == -1 && curr->numInverters == 0) {//if internal node without being an inverter
            double cap = ((wireLength*wrSpecs[1])/2) + (curr->leftWire*wrSpecs[1])/2 + (curr->rightWire*wrSpecs[1])/2;
            curr->checkCap = cap + curr->left->checkCap + curr->right->checkCap;
        }
        else if(curr->numInverters == 0) {
            curr->checkCap = (wireLength*wrSpecs[1])/2 + curr->sinkCap2;
        }
        else { //inverter as leaf node
            curr->checkCap = ((wireLength * wrSpecs[1])/2) + invSpecs[0]; //upstream inv cap
        }   
    }
    else {//root node. 
        if(curr != rootroot) { //root node of subtree(inverter)
            if(curr->right != NULL) {//if inverter is a converted parent
                double cap = (curr->leftWire*wrSpecs[1])/2 + (curr->rightWire*wrSpecs[1])/2 + invSpecs[1];
                curr->checkCap = cap + curr->left->checkCap + curr->right->checkCap;
            }
            else {
                double cap = (curr->leftWire*wrSpecs[1])/2 + invSpecs[1];
                curr->checkCap = cap + curr->left->checkCap;
            }
        }
        else {//root node of entire tree

            double cap = (curr->leftWire*wrSpecs[1])/2 + (curr->rightWire*wrSpecs[1])/2 + invSpecs[1]; //downstream inv cap
            curr->checkCap = cap + curr->left->checkCap + curr->right->checkCap;
        }
    }
}

//need to add a check if maxDelay was violated
void checkDownDelays(Node * curr, Node* parent, double wrRes, double invRes, double * checkMaxDelay, Node * root, Node * rootroot) {
    if(curr == NULL) {
        return;
    }
    if(parent != NULL && parent != root) { //if inverter is not root
        if(parent->numInverters > 0) { //makes sure to return when encountering an inverter below
            return;
        }
    }

    if(parent != NULL) {
        double wireLength = curr->upWire;
        double upDelay = parent->delay;
    
        double wireRes = wrRes * wireLength;
        curr->delay = upDelay + (wireRes * curr->checkCap);

    }
    //IF ALL THE WAY AT THE TOP THEN ROOT NODE WILL HAVE A DELAY
    else {
        if(curr == rootroot) {
            curr->delay = invRes * curr->checkCap;
        }
        else {
            curr->delay = 0;
        }
    }
    
    if(curr->delay > *checkMaxDelay) {
        *checkMaxDelay = curr->delay;
    }

    if(curr->left != NULL) {
        checkDownDelays(curr->left, curr, wrRes, invRes, checkMaxDelay, root, rootroot);
    }
    if(curr->right != NULL) {
        checkDownDelays(curr->right, curr, wrRes, invRes, checkMaxDelay, root, rootroot);
    }
    return;
}