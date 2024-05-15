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

Node * convertParent(Node * curr, double invCap) {//converts parent node into inverter node when inserting inverter exactly at parent node and driving both branches
    curr->inverting = (curr->left->inverting == 0) ? 1 : 0;
    curr->numInverters = 1;
    curr->leftWire = 0;
    curr->rightWire = 0;
    curr->sinkCapacitance = invCap;
    curr->capacitance = invCap;
    return curr;
}
// //inserts Node into the correct position in the list and returns head of list
// //ascending order
// listNode * insertSort(listNode * head, listNode * node) {
//     if(head == NULL) {
//         return node;
//     }
//     if(node->timeArr < head->timeArr) { //have to insert before head
//         node->next = head;
//         return node;
//     }
//     listNode * prev = head;
//     listNode * curr = head;

//     while(curr != NULL) {
//         if(node->timeArr < curr->timeArr) {
//             prev->next = node;
//             node->next = curr;
//             return head;
//         }
//         prev = curr;
//         curr = curr->next;
//     }
//     //curr is null and node should be at end of list
//     prev->next = node;
//     node->next = NULL;
//     return head;
// }

// //finds capacitance plus wire cap
// //takes in option and returns (c + ce, q – re(c + ce/2)) == the capacitance and required arrival time of upstream
// listNode * optionCalc(listNode * option) {
//     return NULL;
// }


// //once generate combined list, generate B values for each node in list, then combine every B value into one list
// //only keep one thing(q) per B

// //takes in 2 option lists and returns a list of them combined: (cL+cR, min(LTime, RTime))
// listNode * combineOptions(listNode * Lbranch, listNode * Rbranch) {
//     if(Lbranch == NULL) {
//         return Rbranch;
//     }
//     else if(Rbranch == NULL) {
//         return Lbranch;
//     }
    
//     listNode * head = malloc(sizeof(listNode));
//     listNode * combined = head;

    
//     while(Rbranch != NULL && Lbranch != NULL) { //what happens when one list reaches the end but the other one doesn't
//         if(combined == NULL) {
//             combined = malloc(sizeof(listNode));
//         }
//         combined->branchCap = Rbranch->branchCap + Lbranch->branchCap;

//         if(Rbranch->timeArr < Lbranch->timeArr) { //saving min required arrival time
//             combined->timeArr = Rbranch->timeArr;
//             //combined->parent = Rbranch->parent;
//             listNode * Rtemp = Rbranch;
//             Rbranch = Rbranch->next;
//             free(Rtemp);
//         }
//         else { //what happens if delays are equal? iterate them both?
//             combined->timeArr = Lbranch->timeArr;
//             //combined->parent = Lbranch->parent;
//             listNode * Ltemp = Lbranch;
//             Lbranch = Lbranch->next;
//             free(Ltemp);
//         }
//         insertSort(head, combined);

//         combined = NULL;
//     }
//     //possible one of the branches aren't freed
//     while(Rbranch != NULL) {
//         listNode * Rtemp = Rbranch;
//         Rbranch = Rbranch->next;
//         free(Rtemp);
//     }
//     while(Lbranch != NULL) {
//         listNode * Ltemp = Lbranch;
//         Lbranch = Lbranch->next;
//         free(Ltemp);
//     }
//     return head;
// }

//calculates sink capacitance (pretty much capacitance of all nodes below plus C' of curr node - capacitance of wire above)
//used for calcmaxwrlength function
double calcCap(double * invSpecs, double * wrSpecs, Node * node) {
    if(node->left == NULL && node->right == NULL) { //leaf node
        return node->sinkCapacitance;
    }
    double leftCap; //vars for storing capacitance of left and right branches
    double rightCap;
    leftCap = node->left->sinkCapacitance + node->leftWire*wrSpecs[1]/2;
    rightCap = node->right->sinkCapacitance + node->rightWire*wrSpecs[1]/2;
    //the issue here is that the capacitance of the node must change if an inverter is added below it
            //so in order for this to work we must recalculate the capacitance of all of our nodes everytime an inverter is added
    // if(node->left->numInverters > 0) {//inverter on left
    //     leftCap = invSpecs[0] + node->leftWire*wrSpecs[1]/2;
    // }
    // else { //the issue here is that the capacitance of the node must change if an inverter is added below it
    //         //so in order for this to work we must recalculate the capacitance of all of our nodes everytime an inverter is added
    //     leftCap = node->left->capacitance + node->leftWire*wrSpecs[1]/2;
    // }
    // if(node->right->numInverters > 0) {//inverter on right
    //     rightCap = invSpecs[0] + node->rightWire*wrSpecs[1]/2;
    // }
    // else { //the issue here is that the capacitance of the node must change if an inverter is added below it
    //         //so in order for this to work we must recalculate the capacitance of all of our nodes everytime an inverter is added
    //     rightCap = node->right->capacitance + node->rightWire*wrSpecs[1]/2;
    // }
    return leftCap + rightCap;
}

double calcCap2(double * invSpecs, double * wrSpecs, Node * node) {
    if(node->left == NULL && node->right == NULL) { //leaf node
        return node->sinkCapacitance;
    }
    return node->leftWire*wrSpecs[1]/2 + node->rightWire*wrSpecs[1]/2;
}


//max wire length that an inverter can drive given sink capacitance
//must subtract upWire capacitance from capacitance because putting inverter somewhere along the wire
double calcMaxWrLen(Node * curr, double * invSpecs, double * wrSpecs, Node * parent, double maxDelay, Node * root, int * checkStringency) {
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
    double wrLen1;
    double wrLen2;


    wrLen1 = (-b + pow(d, 0.5)) / (2*a);
    wrLen2 = (-b - pow(d, 0.5)) / (2*a);
    if(wrLen1 <= 0 && wrLen2 <= 0) {
        *checkStringency = 1;
        if(curr->left == NULL) { //leaf node
            //USE curr->sinkCap2
            //place an inverter right below and see if it helps with capacitance
            //or maybe place an inverter 0 wirelength above leaf node? -> require parent node
            //MAYBE DON'T INSERT INVERTER HERE THIS IS JUST FOR CHECKING, OUTPUT DIFFERENT THINGS in checkStringency
            if(invSpecs[0] >= curr->sinkCapacitance) {
                return EXIT_FAILURE;
            }
            curr->left
            double wr = calcMaxWrLen()
        }
        double leftMaxWr = calcMaxWrLen(curr->left, invSpecs, wrSpecs, curr->left->sinkCapacitance, maxDelay, root, checkStringency);
        double rightMaxWr = calcMaxWrLen(curr->right, invSpecs, wrSpecs, curr->right->sinkCapacitance, maxDelay, root, checkStringency);
        if(leftMaxWr - curr->leftWire < rightMaxWr - curr->rightWire) {
            //insert on left
            Node * inv = createInverter(curr->left, 0, curr->leftWire, invSpecs, wrSpecs);
            curr->curWr = curr->curWr - curr->leftWire;
            curr->left = inv;
            curr->leftWire = 0;
            double wr = calcMaxWrLen(curr, invSpecs, wrSpecs, curr->sinkCapacitance, maxDelay, root, checkStringency);
            if(checkStringency) {
                Node * inv2 = createInverter(curr->right, 0, curr->rightWire, invSpecs, wrSpecs);
                curr->curWr = curr->curWr - curr->rightWire;
                curr->right = inv2;
                curr->rightWire = 0;
                double wr2 = calcMaxWrLen(curr, invSpecs, wrSpecs, curr->sinkCapacitance, maxDelay, root, checkStringency);
                if(checkStringency) { //truly can't avoid constraint
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
        else {
            Node * inv = createInverter(curr->right, 0, curr->rightWire, invSpecs, wrSpecs);
            curr->curWr = curr->curWr - curr->rightWire;
            curr->right = inv;
            curr->rightWire = 0;
            double wr = calcMaxWrLen(curr, invSpecs, wrSpecs, curr->sinkCapacitance, maxDelay, root, checkStringency);
            if(checkStringency) {
                Node * inv2 = createInverter(curr->left, 0, curr->leftWire, invSpecs, wrSpecs);
                curr->curWr = curr->curWr - curr->leftWire;
                curr->left = inv2;
                curr->leftWire = 0;
                double wr2 = calcMaxWrLen(curr, invSpecs, wrSpecs, curr->sinkCapacitance, maxDelay, root, checkStringency);
                if(checkStringency) { //truly can't avoid constraint
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
    
    if(wrLen1 > 0) {
        return wrLen1;
    }
    else {
        return wrLen2;
    }
}

//calculates delay from inverter down to closest node
// double calcInvDownDelay(double * invSpecs, double * wrSpecs, double wrToNode, Node * downNode, Node * parent) {
//     //double delParChild = downNode->delay - parent->delay; //delay on wire between parent node and child node
//     double subWire = downNode->upWire - wrToNode; //wire above inverter insertion check
//     double nodeCap = downNode->capacitance - ((subWire*wrSpecs[1])/2) + invSpecs[1]; //cap excluding top part of wire
//     double res = wrToNode*wrSpecs[0] + invSpecs[2]; //resistance including inverter resistance

//     double delay;
//     delay = res*nodeCap + downNode->curDelay;
//     if(parent!=NULL) {
//         downNode->curDelay = downNode->delay - parent->delay;
//     }
//     //downNode->curDelay = delay; //local delay without inserting inverter
//     return delay;
// }

Node * bottomUp(Node * curr, double maxDelay, Node * parent, double * wrSpecs, double * invSpecs, Node * root, int * checkStringency) {
    if(curr == NULL) {
        return root;
    }
    if(*checkStringency == 1) {
        return root;
    }
    
    bottomUp(curr->left, maxDelay, curr, wrSpecs, invSpecs, root, checkStringency);
    bottomUp(curr->right, maxDelay, curr, wrSpecs, invSpecs, root, checkStringency);
    if(curr->left != NULL && curr->right != NULL) { //reevaluate polarity of internal nodes
        curr->inverting = curr->left->inverting;
    } 
    if(parent == NULL) { //at root node, can't insert inverters above root
        if(curr->left == NULL && curr->right == NULL) {//root node is only node
            Node * inv = createInverter(curr, 0, 0, invSpecs, wrSpecs);
            return inv;
        }
        if(curr->left->inverting == 1) { //make sure correct polarity
            Node * inv = createInverter(curr->left, curr->leftWire/2, curr->leftWire/2, invSpecs, wrSpecs);
            curr->leftWire = curr->leftWire / 2;
            curr->left = inv;
        }
        if(curr->right->inverting == 1) { //make sure correct polarity
            Node * inv = createInverter(curr->right, curr->rightWire/2, curr->rightWire/2, invSpecs, wrSpecs);
            curr->rightWire = curr->rightWire / 2;
            curr->right = inv;
        }
        return root;
    }
    //recalculate capacitance
    // double leftCap = curr->left->capacitance + curr->leftWire*wrSpecs[1]/2;
    // double rightCap = curr->right->capacitance + curr->rightWire*wrSpecs[1]/2;
    // curr->capacitance = leftCap + rightCap;
    //curr->sinkCapacitance = calcCap(invSpecs, wrSpecs, curr);
    //double del = calcInvDownDelay(invSpecs, wrSpecs, curr->upWire, curr, parent);
    
    double maxWr = calcMaxWrLen(curr, invSpecs, wrSpecs, curr->sinkCapacitance, maxDelay, root, checkStringency);
    if(checkStringency) {
        return root;
    }

    //printf("node:%d, delay:%le\n",curr->label, del);
    //printf("maxwr:%le\n", maxWr);
    //printf("maxWr2:%le\n", maxWr2);

    double currentUpWire = curr->upWire; 
    //wire above node. will change if inverter added

    //before adding an inverter check which side the child is on
    int branch = -1; //if 0 on right, if 1 on left
    //int branch2 = -1; //check to see which side of grandparent, parent is on

    if(parent->left == curr) {
        branch = 1;
    }
    else {
        branch = 0;
    }

    // if(grandParent->left == parent) {
    //     branch2 = 1;
    // }
    // else {
    //     branch2 = 0;
    // }


    if(currentUpWire < maxWr) { //need to insert inverter above or at parent node 
        //update parent curWr
        parent->curWr = parent->curWr + currentUpWire; //add wr length that must be driven from left and right branches
        //now wr that inverter could be driving could be greater than the max it can drive
        if(parent->curWr == maxWr) { //if equal add inverter targeting both branches
            //Node * inv = createInverter(parent, parent->upWire, 0, invSpecs, wrSpecs); //create inverter just above parent node
            //could also make parent node the inverter instead
            parent = convertParent(parent, invSpecs[0]); //converting parent to inverter
        }
        else if (parent->curWr > maxWr) {
            //create inverter pointing towards longest branch wire or is it branch with longest delay?
            //longest delay
            //calculate maxWrLength inverter can drive for both 
            double leftMaxWr = calcMaxWrLen(parent->left, invSpecs, wrSpecs, parent->left->sinkCapacitance, maxDelay, root, checkStringency);
            if(checkStringency) {
                return root;
            }
            double rightMaxWr = calcMaxWrLen(parent->right, invSpecs, wrSpecs, parent->right->sinkCapacitance, maxDelay, root, checkStringency);
            if(checkStringency) {
                return root;
            }
            if(leftMaxWr-parent->leftWire < rightMaxWr-parent->rightWire) {
                Node * inv = createInverter(parent->left, 0, parent->leftWire, invSpecs, wrSpecs);
                //now decrease curWr by however much wire we drove
                parent->curWr = parent->curWr - parent->leftWire;
                parent->left = inv;
                parent->leftWire = 0;
            }
            else {
                Node * inv = createInverter(parent->right, 0, parent->rightWire, invSpecs, wrSpecs);
                parent->curWr = parent->curWr - parent->rightWire;
                parent->right = inv;
                parent->rightWire = 0;
            }
        }
    }

    else if(maxWr == currentUpWire) {
        //convert parent node to inverter or add inverter right above parent node
        if(branch) {
            Node * inv = createInverter(parent->left, 0, parent->leftWire, invSpecs, wrSpecs);
            parent->left = inv;
            parent->leftWire = 0;
        }
        else {
            Node * inv = createInverter(parent->right, 0, parent->leftWire, invSpecs, wrSpecs);
            parent->right = inv;
            parent->rightWire = 0;
        }
    }

    else { //need to add inverters below parent node
        Node * inv = createInverter(curr, currentUpWire - maxWr, maxWr, invSpecs, wrSpecs);
        if(branch) {
            parent->left = inv;
            parent->leftWire = inv->upWire;
        }
        else {
            parent->right = inv;
            parent->rightWire = inv->upWire;
        }
        //need to find some way to attach it to the tree
        currentUpWire = currentUpWire - maxWr;
        curr->upWire = maxWr;
        maxWr = calcMaxWrLen(inv, invSpecs, wrSpecs, invSpecs[1], maxDelay, root, checkStringency);
        if(checkStringency) {
            return root;
        }

        //currentUpWire = currentUpWire - maxWr;
        //ADD CHECK TO SEE IF STRINGENT HERE
        while(currentUpWire > maxWr) {
            inv = createInverter(inv, currentUpWire - maxWr, maxWr, invSpecs, wrSpecs);
            if(branch) {
                parent->left = inv;
                parent->leftWire = inv->upWire;
            }
            else {
                parent->right = inv;
                parent->rightWire = inv->upWire;
            }
            currentUpWire = currentUpWire - maxWr;
            maxWr = calcMaxWrLen(inv, invSpecs, wrSpecs, invSpecs[1], maxDelay, root, checkStringency);
            if(checkStringency) {
                return root;
            }

            //now there could be some wire between inverter and node
        }
        if(maxWr == currentUpWire) {
            //insert inverter at node targeting only current branch
            inv = createInverter(inv, 0, maxWr, invSpecs, wrSpecs);
            if(branch) {
                parent->left = inv;
                parent->leftWire = inv->upWire;
            }
            else {
                parent->right = inv;
                parent->rightWire = inv->upWire;
            }
        }
        else { //need to update length that parent node sees, need to recalculate maxWire length using maxDelay minus delay along wire between inverter and node
            parent->curWr = parent->curWr + inv->upWire;
            if(parent->curWr > maxWr) { //place inverter at node targeting longest branch, but must make sure both branches were visited
                double leftMaxWr = calcMaxWrLen(parent->left,  invSpecs, wrSpecs, parent->left->sinkCapacitance, maxDelay, root, checkStringency);
                if(checkStringency) {
                    return root;
                }
                double rightMaxWr = calcMaxWrLen(parent->right, invSpecs, wrSpecs, parent->right->sinkCapacitance, maxDelay, root, checkStringency);
                if(checkStringency) {
                    return root;
                }
                if(leftMaxWr-parent->leftWire < rightMaxWr-parent->rightWire) {
                    inv = createInverter(parent->left, 0, parent->leftWire, invSpecs, wrSpecs);
                    //now decrease curWr by however much wire we drove
                    parent->curWr = parent->curWr - parent->leftWire;
                    parent->left = inv;
                    parent->leftWire = 0;
                }
                else {
                    inv = createInverter(parent->right, 0, parent->rightWire, invSpecs, wrSpecs);
                    parent->curWr = parent->curWr - parent->rightWire;
                    parent->right = inv;
                    parent->rightWire = 0;
                }
            }
        }
    }
    

    //now check if right and left branches have same polarit, if not add an inverter to branch with longest delay
    if(curr->left != NULL && curr->right != NULL) {//make sure not 

    
        if(curr->left->inverting != curr->right->inverting) {
            //diff polarities

            //insert inverter targeting longer branch in the middle of it
            double leftMaxWr = calcMaxWrLen(curr->left, invSpecs, wrSpecs, curr->left->sinkCapacitance, maxDelay, root, checkStringency);
            if(checkStringency) {
                return root;
            }
            double rightMaxWr = calcMaxWrLen(curr->right, invSpecs, wrSpecs, curr->right->sinkCapacitance, maxDelay, root, checkStringency);
            if(checkStringency) {
                return root;
            }
            if(leftMaxWr-curr->leftWire < rightMaxWr-curr->rightWire) {
                Node * inv = createInverter(curr->left, curr->leftWire/2, curr->leftWire/2, invSpecs, wrSpecs);
                //now decrease curWr by however much wire we drove
                curr->curWr = curr->curWr - curr->leftWire/2;
                curr->left = inv;
                curr->leftWire = curr->leftWire/2;
            }
            else {
                Node * inv = createInverter(curr->right, curr->rightWire/2, curr->rightWire/2, invSpecs, wrSpecs);
                curr->curWr = curr->curWr - curr->rightWire/2;
                curr->right = inv;
                curr->rightWire = curr->rightWire/2;
            }
        }
    }
    return root;
}
