//#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "pa2.h"




int main(int argc, char ** argv) {
    if(argc != 6) {
        fprintf(stderr, "Not enough command line args\n");
        return EXIT_FAILURE;
    }
    //printf("%s\n", argv[2]);

    Node * root = readTree(argv[1]);
    if(root == NULL) {
        return EXIT_FAILURE;
    }
    //findDeep(root);
    //printTree(root);
    storeMaxDims(root);
    //findMaxDims(root);
    storeCoords(root, 0, 0);
    printOut13(argv[2], root);
    printOut24Helper(argv[3], root);
    int check = 0;
    sortNodeReps(root, &check);
    if(check == 1) {
        return EXIT_FAILURE;
    }
    check = 0;
    combineOptions(root, &check);
    if(check == 1) {
        freeTree(root);
        return EXIT_FAILURE;
    }

    choosePacking(root);
    choosePackingReps(root);
    storeMaxDims(root);
    storeCoords(root, 0, 0);
    printOut13(argv[4], root);
    printOut24Helper(argv[5], root);
    freeTree(root);

    return EXIT_SUCCESS;
}