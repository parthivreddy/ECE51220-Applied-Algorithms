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

double calcInvWr(Node * curr, double * invSpecs, double * wrSpecs, double maxDelay) {
    double a = wrSpecs[1]*wrSpecs[0]/2;
    double b = wrSpecs[1]*(invSpecs[0]) + invSpecs[2]*wrSpecs[1];
    double c = -maxDelay + invSpecs[2] * (invSpecs[0] + invSpecs[1]);
    double d = b*b - 4*a*c; //within sqr root
    double wrLen1 = 0;
    double wrLen2 = 0;
    if(d < 0) {
        return -1;
    }
    else {
        wrLen1 = (-b + pow(d, 0.5)) / (2*a);
        wrLen2 = (-b - pow(d, 0.5)) / (2*a);
        if(wrLen1 <= 0 && wrLen2 <= 0) {
            return -1;
        }
    }
    if(wrLen1 > 0) {
        return wrLen1;
    }
    else {
        return wrLen2;
    }
}

double calc2InvWr(Node * curr, double * invSpecs, double * wrSpecs, double maxDelay) {
    double a = wrSpecs[1]*wrSpecs[0]/2;
    double b = wrSpecs[1]*(2*invSpecs[0]) + invSpecs[2]*wrSpecs[1];
    double c = -maxDelay + invSpecs[2] * (2*invSpecs[0] + invSpecs[1]);
    double d = b*b - 4*a*c; //within sqr root
    double wrLen1 = 0;
    double wrLen2 = 0;
    if(d < 0) {
        return -1;
    }
    else {
        wrLen1 = (-b + pow(d, 0.5)) / (2*a);
        wrLen2 = (-b - pow(d, 0.5)) / (2*a);
        if(wrLen1 <= 0 && wrLen2 <= 0) {
            return -1;
        }
    }
    if(wrLen1 > 0) {
        return wrLen1;
    }
    else {
        return wrLen2;
    }
}


double calcMaxWrLen(Node * curr, double * invSpecs, double * wrSpecs, Node * parent, double maxDelay, Node * root, int * checkStringency) {
    *checkStringency = 0;
    double del = 0;
    //if non leaf node (essentially inverter is a leaf node)
    if(curr->numInverters == 0 && curr->left != NULL && curr->right != NULL) {
        upstreamsinkCaps(curr, NULL, wrSpecs[1], invSpecs, root);
        del = downstreamsinkDelay(curr, wrSpecs[0], invSpecs[2], root);
    }
    double cap;
    if(curr->left == NULL && curr->right == NULL) {
        cap = curr->sinkCapacitance;
    }
    else {
        cap = curr->capacitance;
    }
    double a = wrSpecs[1]*wrSpecs[0]/2;
    double b = wrSpecs[1]*(cap) + invSpecs[2]*wrSpecs[1];
    double c = -maxDelay + invSpecs[2] * (cap + invSpecs[1]) + del;
       
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
    if(*checkStringency) {
        if(curr->numInverters > 0) { //inverter is in checkStringency
            *checkStringency = 1;
            return EXIT_FAILURE;
        }
        else if(curr->left == NULL && curr->right == NULL) {
            //fprintf(stderr, "Sink Cap can't handle line 126\n");
            Node * inv = createInverter(curr, curr->upWire, 0, invSpecs, wrSpecs);
            if(parent->left == curr) {
                parent->left = inv;
            }
            else {
                parent->right = inv;
            }
            curr->upWire = 0;
            double wr = calcInvWr(curr, invSpecs, wrSpecs, maxDelay);
            if(wr != -1) {
                *checkStringency = 0;
                return wr;
            }
            else {
                *checkStringency = 1;
                return EXIT_FAILURE;
            }
        }
        else { //just add inverters targeting both branches
            Node * invLeft = createInverter(curr->left, 0, curr->leftWire, invSpecs, wrSpecs);
            curr->left = invLeft;
            if(curr->right == NULL) {
                fprintf(stderr, "cancle\n");
            }
            Node * invRight = createInverter(curr->right, 0, curr->rightWire, invSpecs, wrSpecs);
            curr->right = invRight;
            //fprintf(stderr, "adding inverters\n");
            if(curr->right->inverting == curr->left->inverting) {
                curr->inverting = curr->left->inverting;
            }
            else {
                fprintf(stderr, "Error when adding inverter targeting both branches line 153\n");
            }
            double wr = calc2InvWr(curr, invSpecs, wrSpecs, maxDelay);
            if(wr != -1) {
                *checkStringency = 0;
                return wr;
            }
            else {
                *checkStringency = 1;
                return EXIT_FAILURE;
            }
        }
    }
    return -1; //shouldn't get here

    // if(*checkStringency) { //reposition inverters here
    //     if(curr->left == NULL && curr->right == NULL) { //leaf node. Must insert inverter above the node
    //         Node * inv = createInverter(curr, curr->upWire, 0, invSpecs, wrSpecs);
    //         if(parent->left == curr) {
    //             parent->left = inv;
    //         }
    //         else {
    //             parent->right = inv;
    //         }
    //         curr->upWire = 0;
    //     }
    //     else {
    //         double leftMaxWr = calcMaxWrLen(curr->left, invSpecs, wrSpecs, curr, maxDelay, root, checkStringency);
    //         double rightMaxWr = calcMaxWrLen(curr->right, invSpecs, wrSpecs, curr, maxDelay, root, checkStringency);
    //         if(leftMaxWr - curr->leftWire < rightMaxWr - curr->rightWire) {
    //             //insert on left
    //             Node * inv = createInverter(curr->left, 0, curr->leftWire, invSpecs, wrSpecs);
    //             curr->curWr = curr->curWr - curr->leftWire;
    //             curr->left = inv;
    //             curr->leftWire = 0;
    //             double wr = calcMaxWrLen(curr, invSpecs, wrSpecs, parent, maxDelay, root, checkStringency);
    //             if(*checkStringency) { 
    //                 Node * inv2 = createInverter(curr->right, 0, curr->leftWire, invSpecs, wrSpecs);
    //                 curr->curWr = curr->curWr - curr->rightWire;
    //                 curr->right = inv2;
    //                 curr->rightWire = 0;
    //                 double wr2 = calcMaxWrLen(curr, invSpecs, wrSpecs, parent, maxDelay, root, checkStringency);
    //                 if(*checkStringency) { //if you truly can't insert
    //                     return EXIT_FAILURE;
    //                 }
    //                 else {
    //                     return wr2;
    //                 }
    //             }
    //             else {
    //                 return wr;
    //             }
    //         }
    //         else { //insert on right first
    //         Node * inv = createInverter(curr->right, 0, curr->rightWire, invSpecs, wrSpecs);
    //         curr->curWr = curr->curWr - curr->rightWire;
    //         curr->right = inv;
    //         curr->rightWire = 0;
    //         double wr = calcMaxWrLen(curr, invSpecs, wrSpecs, parent, maxDelay, root, checkStringency);
    //         if(*checkStringency) {
    //             Node * inv2 = createInverter(curr->left, 0, curr->leftWire, invSpecs, wrSpecs);
    //             curr->curWr = curr->curWr - curr->leftWire;
    //             curr->left = inv2;
    //             curr->leftWire = 0;
    //             double wr2 = calcMaxWrLen(curr, invSpecs, wrSpecs, parent, maxDelay, root, checkStringency);
    //             if(*checkStringency) { //truly can't avoid constraint
    //                 return EXIT_FAILURE;
    //             }
    //             else {
    //                 return wr2;
    //             }
    //         }
    //         else {
    //             return wr;
    //         }               
    //         }
        
    //     }   
    // }
}

Node * bottomUp(Node * curr, double maxDelay, Node * parent, Node * grandpa, double * wrSpecs, double * invSpecs, Node * root, int * checkStringency) {
    if(curr == NULL) {
        return root;
    }
    if(*checkStringency) {
        return root;
    }
    bottomUp(curr->left, maxDelay, curr, parent, wrSpecs, invSpecs, root, checkStringency);
    bottomUp(curr->right, maxDelay, curr, parent, wrSpecs, invSpecs, root, checkStringency);
    if(curr->left != NULL && curr->right != NULL) {
        if(curr->left->inverting != curr->right->inverting) {
            double leftMaxWr = calcMaxWrLen(curr->left, invSpecs, wrSpecs, parent, maxDelay, root, checkStringency);
            if(*checkStringency) {
                return root;
            }
            double rightMaxWr = calcMaxWrLen(curr->right, invSpecs, wrSpecs, parent, maxDelay, root, checkStringency);
            if(*checkStringency) {
                return root;
            }
            if(curr->left->inverting != curr->right->inverting) {
                //fprintf(stderr, "correcting branch polarity\n");
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
    }
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
    int branch2 = -1;
    //MIGHT NOT WORK ANYMORE SINCE INSERTING INVERTER ABOVE LEAF NODES IS POSSIBLE
    if(parent->left == curr) {
        branch = 1;
    }
    else {
        branch = 0;
    }
    if(grandpa != NULL) {
        if(grandpa->left == parent) {
            branch2 = 1;
        }
        else {
            branch2 = 0;
        }
    }
    if(currentUpWire < maxWr) {
        parent->curWr = currentUpWire + parent->curWr;
        if(parent->curWr == maxWr && grandpa != NULL) {
            fprintf(stderr, "Grandpa1\n");
            Node * inv = createInverter(parent, parent->upWire, 0, invSpecs, wrSpecs);
            if(branch2) {
                grandpa->left = inv;
            }
            else {
                grandpa->right = inv;
            }
            parent->upWire = 0;
            parent->curWr = 0;
        }
        else if(parent->curWr > maxWr) {
            //create inverter pointing towards branch with longest delay
            double leftMaxWr = calcMaxWrLen(parent->left, invSpecs, wrSpecs, parent, maxDelay, root, checkStringency);
            if(*checkStringency) {
                return root;
            }
            double rightMaxWr = calcMaxWrLen(parent->right, invSpecs, wrSpecs, parent, maxDelay, root, checkStringency);
            if(*checkStringency) {
                return root;
            }
            if(leftMaxWr-parent->leftWire < rightMaxWr-parent->rightWire) {
                Node * inv = createInverter(parent->left, 0, parent->leftWire, invSpecs, wrSpecs);
                //now decrease curWr by however much wire we drove
                parent->curWr = parent->curWr - parent->leftWire;
                parent->left->upWire = 0;
                parent->left = inv;
                parent->leftWire = 0;
            }
            else {
                Node * inv = createInverter(parent->right, 0, parent->rightWire, invSpecs, wrSpecs);
                parent->curWr = parent->curWr - parent->rightWire;
                parent->right->upWire = 0;
                parent->right = inv;
                parent->rightWire = 0;
            }
        }
    }
    else if(maxWr == currentUpWire && grandpa != NULL) {
        fprintf(stderr, "Grandpa2\n");
        Node * inv = createInverter(parent, parent->upWire, 0, invSpecs, wrSpecs);
        if(branch2) {
            grandpa->left = inv;
        }
        else {
            grandpa->right = inv;
        }
        parent->upWire = 0;
        parent->curWr = 0;
    }
    else { //add inverters below parent node
        Node * inv = createInverter(curr, currentUpWire - maxWr, maxWr, invSpecs, wrSpecs);
        if(branch) {
            parent->left = inv;
            parent->leftWire = inv->upWire;
        }
        else {
            parent->right = inv;
            parent->rightWire = inv->upWire;
        }
        currentUpWire = currentUpWire - maxWr;
        //parent->curWr = currentUpWire;
        curr->upWire = maxWr;
        maxWr = calcMaxWrLen(inv, invSpecs, wrSpecs, parent, maxDelay, root, checkStringency);
        if(*checkStringency) {
            return root;
        }
        while(currentUpWire > maxWr) {
            inv = createInverter(inv, currentUpWire-maxWr, maxWr, invSpecs, wrSpecs);
            if(branch) {
                parent->left = inv;
                parent->leftWire = inv->upWire;
            }
            else {
                parent->right = inv;
                parent->rightWire = inv->upWire;
            }
            currentUpWire = currentUpWire - maxWr;
            //parent->curWr = currentUpWire;
            maxWr = calcMaxWrLen(inv, invSpecs, wrSpecs, parent, maxDelay, root, checkStringency);
            if(*checkStringency) {
                return root;
            }
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
        else {
            parent->curWr = parent->curWr + inv->upWire;
            if(parent->curWr > maxWr) {
                double leftMaxWr = calcMaxWrLen(parent->left, invSpecs, wrSpecs, parent, maxDelay, root, checkStringency);
                if(*checkStringency) {
                    return root;
                }
                double rightMaxWr = calcMaxWrLen(parent->right, invSpecs, wrSpecs, parent, maxDelay, root, checkStringency);
                if(*checkStringency) {
                    return root;
                }
                if(leftMaxWr-parent->leftWire < rightMaxWr-parent->rightWire) {
                    inv = createInverter(parent->left, 0, parent->leftWire, invSpecs, wrSpecs);
                    //now decrease curWr by however much wire we drove
                    parent->curWr = parent->curWr - parent->leftWire;
                    parent->left->upWire = 0;
                    parent->left = inv;
                    parent->leftWire = 0;
                }
                else {
                    inv = createInverter(parent->right, 0, parent->rightWire, invSpecs, wrSpecs);
                    parent->curWr = parent->curWr - parent->rightWire;
                    parent->right->upWire = 0;
                    parent->right = inv;
                    parent->rightWire = 0;
                }
            }
        }
        
    }
    return root;

}