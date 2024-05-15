#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "pa1.h"


//right now just inserting on a wire
//probably need some value check to see if I'm inserting at a node
Node * createInverter(Node * downNode, double upWire, double leftWire, double * invSpecs, double * wrSpecs) {
    Node * inv = malloc(sizeof(Node));
    inv->left = downNode;
    inv->right = NULL;

    inv->leftWire = leftWire;
    inv->rightWire = -1;

    inv->upWire = upWire;
    inv->inverting =  (downNode->inverting == 0) ? 1 : 0;
    inv->numInverters = 1;
    inv->capacitance = invSpecs[0];
    inv->sinkCapacitance = invSpecs[0];
    inv->checkInverting = 1;
    inv->label = -1;
    inv->delay = 0;
    return inv;
}

double calcWrLen(Node * curr, double * invSpecs, double * wrSpecs, Node * parent, double maxDelay, Node * root, int * checkStringency) {
    *checkStringency = 0;
    double del = 0;
    if(curr->label == 33) {
        printf("stop\n");
    }
    //if non leaf node (essentially inverter is a leaf node)
    if(curr->numInverters == 0 && curr->left != NULL && curr->right != NULL) {
        upstreamsinkCaps(curr, NULL, wrSpecs[1], invSpecs, root);
        del = downstreamsinkDelay(curr, wrSpecs[0], invSpecs[2], root);
    }
    
    double a = wrSpecs[1]*wrSpecs[0]/2;
    double b = wrSpecs[1]*(curr->sinkCapacitance) + invSpecs[2]*wrSpecs[1];
    double c = -maxDelay + invSpecs[2] * (curr->sinkCapacitance + invSpecs[1]) + del;
       
    //quadratic formula
    double d = b*b - 4*a*c; //within sqr root
    double wrLen1 = 0;
    double wrLen2 = 0;

    //possible d is negative so wr lengths will be imaginary
    if(d < 0) {
        *checkStringency = 1;
    }
    else {
        wrLen1 = (-b + pow(d, 0.5)) / (2*a);
        wrLen2 = (-b - pow(d, 0.5)) / (2*a);
        if(wrLen1 <= 0 && wrLen2 <= 0) {
            *checkStringency = 1;
        }
    }
    if(!(*checkStringency)) { //if able to have some wr length above node
        if(wrLen1 > 0) {
            return wrLen1;
        }
        else {
            return wrLen2;
        }
    }

    if(*checkStringency) { //reposition inverters here
        if(curr->left == NULL && curr->right == NULL) { //leaf node. Must insert inverter above the node
            Node * inv = createInverter(curr, curr->upWire, 0, invSpecs, wrSpecs);
            if(parent->left == curr) {
                parent->left = inv;
            }
            else {
                parent->right = inv;
            }
            curr->upWire = 0;
        }
        else {
            double leftMaxWr = calcMaxWrLen(curr->left, invSpecs, wrSpecs, curr, maxDelay, root, checkStringency);
            double rightMaxWr = calcMaxWrLen(curr->right, invSpecs, wrSpecs, curr, maxDelay, root, checkStringency);
            if(leftMaxWr - curr->leftWire < rightMaxWr - curr->rightWire) {
                //insert on left
                Node * inv = createInverter(curr->left, 0, curr->leftWire, invSpecs, wrSpecs);
                curr->curWr = curr->curWr - curr->leftWire;
                curr->left = inv;
                curr->leftWire = 0;
                double wr = calcMaxWrLen(curr, invSpecs, wrSpecs, parent, maxDelay, root, checkStringency);
                if(*checkStringency) { 
                    Node * inv2 = createInverter(curr->right, 0, curr->leftWire, invSpecs, wrSpecs);
                    curr->curWr = curr->curWr - curr->rightWire;
                    curr->right = inv2;
                    curr->rightWire = 0;
                    double wr2 = calcMaxWrLen(curr, invSpecs, wrSpecs, parent, maxDelay, root, checkStringency);
                    if(*checkStringency) { //if you truly can't insert
                        return EXIT_FAILURE;
                    }
                    else {
                        return wr2;
                    }
                }
                else {
                    return wr;
                }
            }
            else { //insert on right first
            Node * inv = createInverter(curr->right, 0, curr->rightWire, invSpecs, wrSpecs);
            curr->curWr = curr->curWr - curr->rightWire;
            curr->right = inv;
            curr->rightWire = 0;
            double wr = calcMaxWrLen(curr, invSpecs, wrSpecs, parent, maxDelay, root, checkStringency);
            if(*checkStringency) {
                Node * inv2 = createInverter(curr->left, 0, curr->leftWire, invSpecs, wrSpecs);
                curr->curWr = curr->curWr - curr->leftWire;
                curr->left = inv2;
                curr->leftWire = 0;
                double wr2 = calcMaxWrLen(curr, invSpecs, wrSpecs, parent, maxDelay, root, checkStringency);
                if(*checkStringency) { //truly can't avoid constraint
                    return EXIT_FAILURE;
                }
                else {
                    return wr2;
                }
            }
            else {
                return wr;
            }               
            }
        
        }   
    }
}

Node * bottomUp(Node * curr, double maxDelay, Node * parent, double * wrSpecs, double * invSpecs, Node * root, int * checkStringency) {
    if(curr == NULL) {
        return root;
    }
    if(*checkStringency) {
        return root;
    }
    bottomUp(curr->left, maxDelay, curr, wrSpecs, invSpecs, root, checkStringency);
    bottomUp(curr->right, maxDelay, curr, wrSpecs, invSpecs, root, checkStringency);

    if(curr->left != NULL && curr->right != NULL) {
        //reevaluating polarity of internal nodes
        if(curr->left->inverting != curr->right->inverting) {
            fprintf(stderr, "Branches polarity must be equivalent\n");
        }
        curr->inverting = curr->left->inverting;
    }

    if(parent == NULL) {
        //at root node, can't insert inverters above root
        if(curr->left == NULL && curr->right == NULL) {
            //root node is only node
            Node * inv = createInverter(curr, 0, 0, invSpecs, wrSpecs);
            return inv;
        }
        if(curr->left->inverting == 1) {
            Node * inv = createInverter(curr->left, (curr->leftWire)/2, (curr->leftWire)/2, invSpecs, wrSpecs);
            curr->leftWire = (curr->leftWire) / 2;
            curr->left = inv;
        }
        if(curr->right->inverting == 1) { //make sure correct polarity
            Node * inv = createInverter(curr->right, curr->rightWire/2, curr->rightWire/2, invSpecs, wrSpecs);
            curr->rightWire = curr->rightWire / 2;
            curr->right = inv;
        }
        return root;
    }

    double maxWr = calcMaxWrLen(curr, invSpecs, wrSpecs, parent, maxDelay, root, checkStringency);
    if(*checkStringency) {
        return root;
    }

    double currentUpWire = curr->upWire;
    int branch = -1; //if 0 on right, if 1 on left
    //MIGHT NOT WORK ANYMORE SINCE INSERTING INVERTER ABOVE LEAF NODES IS POSSIBLE
    if(parent->left == curr) {
        branch = 1;
    }
    else {
        branch = 0;
    }

}