#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "pa1.h"


void upstreamsinkCaps(Node * node, Node * parent, double wrCap, double * invSpecs, Node * rootroot) { 
    //POSSIBLE TO ADD INVERTERS BELOW LEAF NODE NOW

    //passes in root of tree/subtree and fills in capacitance values (bottom up)
    //is the total seen capacitance at that level
    if(node == NULL) { //if below leaf node
        return;
    }
    if(parent != NULL) {
        if(parent->numInverters > 0) { //if below inverter
            return;
        }
    }
    //use a stack to hold wire distance that you came from
    //a couple of things to do: make sure leftWire and rightWire exist
    if(node->left != NULL) {
        upstreamsinkCaps(node->left, node, wrCap, invSpecs, rootroot);
    }
    if(node->right != NULL) {
        upstreamsinkCaps(node->right, node, wrCap, invSpecs, rootroot);
    }

    double capAtNode = 0; //capacitance at node without adding it to the subtree C'
    if(parent != NULL) {
        if(parent->left == node) {
            node->upWire = parent->leftWire;
        }
        else {
            node->upWire = parent->rightWire;
        }
        double wireLength = node->upWire; //getting length of wire above node
    
    //need to account for capacitance of root node since nothing will be in stack
    //also possible nothing in stack
    //TODO: Need to add a check if the node is directly under an inverter because then there is an extra downstream cap
        if(node->label == -1 && node->numInverters == 0) { //internal node-> no sink capacitance
            capAtNode = ((wireLength * wrCap)/2) + (node->leftWire * wrCap)/2 + (node->rightWire * wrCap)/2;
            //node->subCap = capAtNode;
            node->sinkCapacitance = capAtNode + node->left->sinkCapacitance + node->right->sinkCapacitance;
        }
        else if(node->numInverters == 0){//leaf node
            node->sinkCapacitance = ((wireLength * wrCap)/2) + node->sinkCapacitance;
            //node->subCap = node->sinkCapacitance;
        }
        else { //inverter as leaf node
            node->sinkCapacitance = ((wireLength * wrCap)/2) + invSpecs[0];
        }
    }
    else { //root node or it is inverter node if inverter node left wire above root

//NEED TO ADD INVERTER DOWNSTREAM CAPACITCANCE
        if(node != rootroot) {//root node of subtree
            if(node->left != NULL && node->right != NULL) { //making sure not leaf nodes or inverter
                capAtNode =  (node->leftWire * wrCap)/2 + (node->rightWire * wrCap)/2;
                node->sinkCapacitance = capAtNode + node->left->sinkCapacitance + node->right->sinkCapacitance;
                //node->subCap = capAtNode;
            }
        }
        else { //root node of whole tree, need to add downstream cap from inverter
            capAtNode =  (node->leftWire * wrCap)/2 + (node->rightWire * wrCap)/2;
            node->sinkCapacitance = capAtNode + node->left->sinkCapacitance + node->right->sinkCapacitance + invSpecs[1];
        }
    }
    return;
}



double downstreamsinkDelay(Node * node, double wrRes, double invRes, Node * root) { 
    //Node * root = node;
    double maximumBranchDelay = 0;
    downstreamsinkDelayHelper(node, NULL, wrRes, invRes, &maximumBranchDelay, root);
    return maximumBranchDelay;
}

void downstreamsinkDelayHelper(Node * curr, Node * parent, double wrRes, double invRes, double * maximumBranchDelay, Node * root) {
    if(curr == NULL) {
        return;
    }
    if(parent != NULL) {
        if(parent->numInverters > 0) {
            return;
        }
    }

    if(parent != NULL) { //if non root node
        double wireLength;
        if(parent->left == curr) {
            wireLength = parent->leftWire;
        }
        else {
            wireLength = parent->rightWire;
        }
        double upDelay = parent->delay;

        //I THINK SAME OPERATION FOR internal and leaf
        double wireRes = wrRes * wireLength; //Getting actual resistance of wire above
        curr->delay = upDelay + (wireRes * curr->sinkCapacitance);
        
    }
    //IF ALL THE WAY AT THE TOP THEN ROOT NODE WILL HAVE A DELAY
    else { //root 
        if(curr == root) {
            curr->delay = invRes * curr->capacitance;
        }
        else {
            curr->delay = 0; //should not get here
        }
    }
    if(curr->delay > *maximumBranchDelay) {
        *maximumBranchDelay = curr->delay;
    }
    if(curr->left != NULL) {
        downstreamsinkDelayHelper(curr->left, curr, wrRes, invRes, maximumBranchDelay, root);
    }
    if(curr->right != NULL) {
        downstreamsinkDelayHelper(curr->right, curr, wrRes, invRes, maximumBranchDelay, root);
    }
    return;
}