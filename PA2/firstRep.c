#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "pa2.h"

int max(int v1, int v2) {
    return (v1 > v2) ? v1 : v2;
    //return max;
}

int checkType(Node * node) {
    //returns 0 if 2 non-div, 1 if non-div on left, 2 if non-div on right, 3 if div on both
    if(node->left->label != -1) {
        if(node->right->label != -1) {
            return 0;
        }
        return 1;
    }
    if(node->right->label != -1) {
        return 2;
    }
    return 3;
}

bool checkDiv(Node * node) {//returns true if H false if V
    return (node->label2 == 'H');
}

int getLeftWidth(Node * node, int rep) {
    if(node->left->label == -1) {
        return -1;
    }
    return node->left->dims[rep][0];
}
int getRightWidth(Node * node, int rep) {
    if(node->right->label == -1) {
        return -1;
    }
    return node->right->dims[rep][0];
}
int getLeftLength(Node * node, int rep) {
    if(node->left->label == -1) {
        return -1;
    }
    return node->left->dims[rep][1];
}
int getRightLength(Node * node, int rep) {
    if(node->right->label == -1) {
        return -1;
    }
    return node->right->dims[rep][1];
}



// void storeMaxDims(Node * node, int rep) {
//     if(node->label != -1) { //not a division node
//         return;
//     }
//     int maxL = 0;
//     int maxW = 0;
//     //if at bottom of tree->special case
//     if(node->left->label != -1 && node->right->label != -1) {
//         if(node->label2 == 'H') {
//             node->addLength = node->left->dims[rep][1] + node->right->dims[rep][1];
//         }
//         else {
//             node->addWidth = node->left->dims[rep][0] + node->right->dims[rep][0];
//         }
//         node->maxWidth = max(node->left->dims[rep][0], node->right->dims[rep][0]);
//         node->maxLength = max(node->left->dims[rep][1], node->right->dims[rep][1]);
//     }
//     else if(node->left->label != -1) {
//         maxL = node->left->dims[rep][1];
//         maxW = node->left->dims[rep][0];
//         if(node->label2 == 'H') {
//             node->addWidth = node->right->addWidth;
//             if(node->right->addLength == 0) {
//                 node->addLength = maxL + node->right->maxLength;
//             }
//             else {
//                 node->addLength = maxL + node->right->addLength;
//             }
//         }
//         else {
//             node->addLength = node->right->addLength;
//             if(node->right->addWidth == 0) {
//                 node->addWidth = maxW + node->right->maxWidth;
//             }
//             else {
//                 node->addWidth = maxW + node->right->addWidth;
//             }
//         }
//         node->maxWidth = max(maxW, node->right->maxWidth);
//         node->maxLength = max(maxL, node->right->maxLength);
//     }
//     else if(node->right->label != -1) {
//         maxL = node->right->dims[rep][1];
//         maxW = node->right->dims[rep][0];
//         if(node->label2 == 'H') {
//             node->addWidth = node->left->addWidth;
//             if(node->left->addLength == 0) {
//                 node->addLength = maxL + node->left->maxLength;
//             }
//             else {
//                 node->addLength = maxL + node->left->addLength;
//             }
//         }
//         else {
//             node->addLength = node->left->addLength;
//             if(node->left->addWidth == 0) {
//                 node->addWidth = maxW + node->left->maxWidth;
//             }
//             else {
//                 node->addWidth = maxW + node->left->addWidth;
//             }
//         }
//         node->maxWidth = max(node->left->maxWidth, maxW);
//         node->maxLength = max(node->left->maxLength, maxL);
//     }
//     else {
//         // maxL = node->left->addLength;
//         // maxW = node->left->addWidth;
//         if(node->label2 == 'H') {
//             node->addWidth = max(node->right->addWidth, node->left->addWidth);
//             if(node->right->addLength == 0) {
//                 node->addLength = max(node->left->maxLength, node->right->maxLength);
//             }
//             else {
//                 node->addLength = node->left->addLength + node->right->addLength;
//             }
//         }
//         else {
//             node->addLength = max(node->left->addLength, node->right->addLength);
//             if(node->right->addWidth == 0) {
//                 node->addWidth = max(node->left->maxWidth, node->right->maxWidth);
//             }
//             else {
//                 node->addWidth = node->left->addWidth + node->right->addWidth;
//             }
//         }
//         node->maxWidth = max(node->left->maxWidth, node->right->maxWidth);
//         node->maxLength = max(node->left->maxLength, node->right->maxLength);
//     }
//     // node->addLength = (*addDims)[1];
//     // node->addWidth = (*addDims)[0];
//     printf("Max Dims %c: (%d, %d)\n", node->label2, node->maxWidth, node->maxLength);
//     printf("%c: (%d, %d)\n", node->label2, node->addWidth, node->addLength);
// }

// void storeMaxDims(Node * node, int rep) {
//     if(node->label != -1) {//non division node
//         return;
//     }
//     int type = checkType(node); 
//     bool typeDiv = checkDiv(node); //H if true V if false
//     // int maxL = 0;
//     // int maxR = 0;
//     switch(type){
//         case(0):
//             //2 non-div nodes
//             node->maxLength = max(getLeftLength(node, rep), getRightLength(node, rep));
//             node->maxWidth = max(getLeftWidth(node, rep), getRightWidth(node, rep));
//             if(typeDiv) {
//                 node->addLength = getLeftLength(node, rep) + getRightLength(node, rep);
//                 node->addWidth = node->maxWidth;
//             }
//             else {
//                 node->addWidth = getLeftWidth(node, rep) + getRightWidth(node, rep);
//                 node->addLength = node->maxLength;
//             }
//             break;
//         case(1):
//             //non-div on left div on right
//             node->maxLength = max(getLeftLength(node, rep), max(node->right->maxLength, node->right->addLength));
//             node->maxWidth = max(getLeftWidth(node, rep), max(node->right->maxWidth, node->right->addWidth));
//             if(typeDiv) {
//                 node->addLength = getLeftLength(node, rep) + node->right->addLength;
//                 node->addWidth = node->maxWidth;
//             }
//             else {
//                 node->addWidth = getLeftWidth(node, rep) + node->right->addWidth;
//                 node->addLength = node->maxLength;
//             }
//             break;
//         case(2):
//             //non-div on right div on left
//             node->maxLength = max(getRightLength(node, rep), max(node->left->maxLength, node->left->addLength));
//             node->maxWidth = max(getRightWidth(node, rep), max(node->left->maxWidth, node->left->addWidth));
//             if(typeDiv) {
//                 node->addLength = getRightLength(node, rep) + node->left->addLength;
//                 node->addWidth = node->maxWidth;
//             }
//             else {
//                 node->addWidth = getRightWidth(node, rep) + node->left->addWidth;
//                 node->addLength = node->maxLength;
//             }
//             break;
//         case(3): //no leaf nodes as children
//             node->maxLength = max(max(node->left->maxLength, node->left->addLength), max(node->right->maxLength, node->right->addLength));
//             node->maxWidth = max(max(node->left->maxWidth, node->left->addWidth), max(node->right->maxWidth, node->right->addWidth));
//             if(typeDiv) {
//                 node->addLength = node->left->addLength + node->right->addLength;
//                 node->addWidth = node->maxWidth;
//             }
//             else {
//                 node->addWidth = node->left->addWidth + node->right->addWidth;
//                 node->addLength = node->maxLength;
//             }
//             break;
//     }
//     printf("Max Dims %c: (%d, %d)\n", node->label2, node->maxWidth, node->maxLength);
//     printf("%c: (%d, %d)\n", node->label2, node->addWidth, node->addLength);
// }

void storeMaxDims(Node * node) {
    if(node == NULL) {
        return;
    }
    if(node->label != -1) {
        node->offsetX = node->dims[node->bestRep][0];
        node->offsetY = node->dims[node->bestRep][1];
        return;
    }
    storeMaxDims(node->left);
    storeMaxDims(node->right);
    bool divType = checkDiv(node); //H if true V if not
    // node->originX = node->left->originX;
    // node->originY = node->left->originY;

    if(divType) { //H
        node->offsetX = max(node->left->offsetX, node->right->offsetX);
        node->offsetY = node->left->offsetY + node->right->offsetY;
        //node->right->originY = node->originY + node->offsetY;
    }
    else { //V
        node->offsetX = node->left->offsetX + node->right->offsetX;
        node->offsetY = max(node->left->offsetY, node->right->offsetY);
        //node->right->originX = node->originX + node->offsetX;
    }
    return;
}


void storeCoords(Node * node, int originX, int originY) {
    if(node == NULL) {
        return;
    }
    if(node->label != -1) {
        node->originX = originX;
        node->originY = originY;
        return;
    }

    bool divType = checkDiv(node); //H if true V if not
    if(divType) { //H
        storeCoords(node->left, originX, originY + node->right->offsetY);
        storeCoords(node->right, originX, originY);
    }
    else {//V
        storeCoords(node->left, originX, originY);
        storeCoords(node->right, originX + node->left->offsetX, originY);
    }
    return;
}
