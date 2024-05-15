#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "pa2.h"

// void swap(Node * node, int ind1, int ind2) {
//     int tempWidth = node->dims[ind1][0];
//     int tempLength = node->dims[ind1][1];
//     node->dims[ind1][0] = node->dims[ind2][0];
//     node->dims[ind1][1] = node->dims[ind2][1];
//     node->dims[ind2][0] = tempWidth;
//     node->dims[ind2][1] = tempLength;
// }

// int partition(Node * node, int left, int right) {
//     int pivotWidth = node->dims[right][0];
//     int pivotLength = node->dims[right][1];
//     int swapInd = left - 1;
//     for(int i = 0; i < right; i++) {
//         if(node->dims[i][0] < pivotWidth) {
//             swapInd++;
//             swap(node, i, swapInd);
//         }
//         else if(node->dims[i][0] == pivotWidth) {
//             if()
//         }
//     }
//     swap(node, swapInd + 1, right);
// }

// void quicksort(Node * node, int left, int right) {
//     if(left > right) {
//         return;
//     }
//     int pivotInd = partition(node, left, right);
//     quicksort(node, left, pivotInd - 1);
//     quicksort(node, pivotInd + 1, right);
// }

void merge(Node * node, int left, int median, int right, int * check) {
    if(left > median || median > right) {
        fprintf(stderr, "Should never get here in merge\n");
        *check = 1; //error
        return;
    }

    //temp arr stores the indices of the reps
    //int * temparr = malloc((right - left + 1) * sizeof(int));

    long ** temparr = malloc((right - left + 1) * sizeof(long*));
    if(temparr == NULL) {
        *check = 1;
        return;
    }
    for(int l = 0; l < (right - left + 1); l++) {
        temparr[l] = malloc(2 * sizeof(long));


        
        if(temparr[l] == NULL) {
            for(int q = 0; q < l; q++) {
                free(temparr[q]);
            }
            free(temparr);
            *check = 1;
            return;
        }
    }
    // for(int i = 0; i < node->numReps; i++) {
    //     printf("(%d, %d)", array[i][0], array[i][1]);
    // }
    // printf("\n!!!!!!!!!!!!!!!!\n");
    

    int i = left; //ind for left part of array
    int j = median + 1; //ind for right part of array
    int k = 0; //ind for temporary array

    while((i <= median) && (j <= right)) { //both arrays still have elements
        if(node->dims[i][0] < node->dims[j][0]) {
            if(node->dims[i][1] <= node->dims[j][1]) {
                //rep j is suboptimal
                j++; //skip over this rep
                node->numReps = node->numReps - 1;
            }
            temparr[k][0] = node->dims[i][0];
            temparr[k][1] = node->dims[i][1];
            i++;
        }
        else if(node->dims[i][0] == node->dims[j][0]) {
            //remove whichever one has bigger length
            //if equal length then keep one from array on left
            if(node->dims[i][1] <= node->dims[j][1]) {
                j++; //skip over
                temparr[k][0] = node->dims[i][0];
                temparr[k][1] = node->dims[i][1];
                i++;
            }
            else { //i has bigger length
                i++;
                temparr[k][0] = node->dims[j][0];
                temparr[k][1] = node->dims[j][1];
                j++;
            }
            node->numReps = node->numReps - 1;
        }
        else {//node j has smaller width
            if(node->dims[j][1] <= node->dims[i][1]) {
                //rep i is suboptimal
                i++; //skip over this rep
                node->numReps = node->numReps - 1;
            }
            temparr[k][0] = node->dims[j][0];
            temparr[k][1] = node->dims[j][1];
            j++;
        }
        k++;
    }

    while(i <= median) {
        temparr[k][0] = node->dims[i][0];
        temparr[k][1] = node->dims[i][1];
        i++;
        k++;
    }
    while(j <= right) {
        temparr[k][0] = node->dims[j][0];
        temparr[k][1] = node->dims[j][1];
        j++;
        k++;
    }

    k = 0; //reset index of temparray
    for(int z = left; z <= right; z++) {
        node->dims[z][0] = temparr[k][0];
        node->dims[z][1] = temparr[k][1];
        //printf("(%d,%d)\n", array[z][0], array[z][1]);
        free(temparr[k]);
        k++;
    }
    free(temparr);
    return;
}

void mergeSort(Node * node, int left, int right, int * check) {
    if(left >= right) { //if out of bounds or only 1 element
        return;
    }
    if(*check == 1) {
        return;
    }
    int m = (left+right)/2;
    mergeSort(node, left, m, check);
    mergeSort(node, m+1, right, check);
    merge(node, left, m, right, check);
    // printf("!!!!!!!!!!!!!!!!!!!!!\n");
    // printf("left: %d, right: %d\n", left, right);
    // for(int i = 0; i < node->numReps; i++) {
    //     printf("(%d, %d)", array[i][0], array[i][1]);
    // }
    // printf("\n");
    return;
}

void sortNodeReps(Node * node, int * check) {
    if(node == NULL) {
        return;
    }
    if(*check == 1) {
        return;
    }
    sortNodeReps(node->left, check);
    sortNodeReps(node->right, check);
    if(node->label != -1 && node->numReps > 1) {
        mergeSort(node, 0, node->allReps - 1, check);
    }
    return;
}

void combineOptions(Node * node, int * check) {
    if(*check == 1) { //if can't allocate memory
        return;
    }
    if(node == NULL) {
        return;
    }
    //postorder (bottom up)
    combineOptions(node->left, check);
    combineOptions(node->right, check);


    if(node->label == -1) {
        bool divType = checkDiv(node);
        int numOptionsLeft = node->left->numReps;
        int numOptionsRight = node->right->numReps;
        // printf("-----------------------------\n");
        // for(int i = 0; i < numOptionsLeft; i++) {
        //     printf("(%d, %d)||", node->left->dims[i][0], node->left->dims[i][1]);
        // }
        // printf("\n");
        // for(int i = 0; i < numOptionsRight; i++) {
        //     printf("(%d, %d)||", node->right->dims[i][0], node->right->dims[i][1]);
        // }
        // printf("\n");

        int maxPossible = numOptionsLeft + numOptionsRight - 1;
        node->dims = malloc(maxPossible * sizeof(long*));
        if(node->dims == NULL) {
            fprintf(stderr, "Can't allocate memory for internal\n");
            *check = 1;
            return;
        }

        for(int i = 0; i < maxPossible; i++) {
            node->dims[i] = malloc(4* sizeof(long)); //left 2 are for width and height lengths, right 2 are for index of the rep (left, right)
            if(node->dims[i] == NULL) {
                for(int j = 0; j < i; j++) {
                    free(node->dims[j]);
                }
                fprintf(stderr, "Can't allocate mem for internal\n");
                *check = 1;
                free(node->dims);
                return;
            }
        }
        node->allReps = maxPossible;

        int k = 0; //where we are putting choices in consolidated array
        if(!divType) { //V node
            int l = 0; //marker for left choices
            int r = 0; //marker for right choices
            int addWidth;
            int maxLength;
            while(l < numOptionsLeft && r < numOptionsRight) { //consolidate options as long as both arrays still have options
                addWidth = node->left->dims[l][0] + node->right->dims[r][0];
                maxLength = max(node->left->dims[l][1], node->right->dims[r][1]);
                node->dims[k][0] = addWidth;
                node->dims[k][1] = maxLength;
                node->dims[k][2] = l;
                node->dims[k][3] = r;
                k++;
                if(node->left->dims[l][1] == maxLength) { //if left is the bigger length
                    if(node->right->dims[r][1] == maxLength) {//if right and left have equivalent lengths increment both
                        r++;
                    }
                    l++;
                }
                else {
                    r++;
                }
            }
            node->numReps = k; //because k was incremented an extra time
        }
        
        else { //H node
            int l = numOptionsLeft - 1;
            int r = numOptionsRight - 1;
            int ** temparr = malloc(maxPossible * sizeof(long*)); //array that holds the values that will be reversed
            if(temparr == NULL) {
                *check = 1;
                fprintf(stderr, "Can't allocate memory for temp array\n");
                return;
            }
            for(int i = 0; i < maxPossible; i++) {
                temparr[i] = malloc(4*sizeof(long));
                if(temparr[i] == NULL) {
                    for(int j = 0; j < i; j++) {
                        free(temparr[i]);
                    }
                    fprintf(stderr, "Can't allocate memory for temp array2\n");
                    *check = 1;
                    free(temparr);
                    return;
                }
            }
            int tempInd = 0; //index for temp array
            int maxWidth;
            int addLength;
            while(l >= 0 && r >= 0) {
                maxWidth = max(node->left->dims[l][0], node->right->dims[r][0]);
                addLength = node->left->dims[l][1] + node->right->dims[r][1];
                temparr[tempInd][0] = maxWidth;
                temparr[tempInd][1] = addLength;
                temparr[tempInd][2] = l;
                temparr[tempInd][3] = r;
                tempInd++;
                if(node->left->dims[l][0] == maxWidth) { //if left is the bigger length
                    if(node->right->dims[r][0] == maxWidth) {//if right and left have equivalent lengths increment both
                        r--;
                    }
                    l--;
                }
                else {
                    r--;
                }
            }
            //now transfer the values from the temp array to node in reverse manner
            node->numReps = tempInd; //because tempInd incremented 1 extra time
            --tempInd;
            for(int i = 0; i < node->numReps; i++) {
                node->dims[i][0] = temparr[tempInd][0];
                node->dims[i][1] = temparr[tempInd][1];
                node->dims[i][2] = temparr[tempInd][2];
                node->dims[i][3] = temparr[tempInd][3];
                //printf("Rep:%d: (%d, %d)", i, node->dims[i][0], node->dims[i][1]);
                --tempInd;
            }
            for(int i = 0; i < node->allReps; i++) {
                free(temparr[i]);
            }
            free(temparr);
        }
        // for(int i = 0; i < node->numReps; i++) {
        //     printf("(%d, %d), reps: (%d, %d) ", node->dims[i][0], node->dims[i][1], node->dims[i][2], node->dims[i][3]);
        // }
        // printf("\n");
    }


}

void choosePacking(Node * root) {
    long long bestPacking = __LONG_LONG_MAX__;
    long length;
    long width;
    for(int i = 0; i < root->numReps; i++) {
        //printf("Rep:%d (%d, %d)\n", i, root->dims[i][0], root->dims[i][1]);
        length = root->dims[i][0];
        width = root->dims[i][1];
        if(length*width < bestPacking) {
            bestPacking = length*width;
            root->bestRep = i;
        }
    }
}

void choosePackingReps(Node * node) {
    if(node == NULL) {
        return;
    }

    //root node already has best rep populated
    if(node->label == -1) {
        node->left->bestRep = node->dims[node->bestRep][2];
        node->right->bestRep = node->dims[node->bestRep][3];
    }
    choosePackingReps(node->left);
    choosePackingReps(node->right);
}


