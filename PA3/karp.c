#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "pa3.h"

//outer array value is for k and inner is for node (0 ind corresponds to node 1)
double ** distanceTable(int numNodes, int numEdges) {
    double ** table = malloc((numNodes + 1) * sizeof(double *));
    for(int i = 0; i <= numNodes; i++) {
        table[i] = malloc(numNodes * sizeof(double));
    }
    return table;
}

int ** predTable(int numNodes, int numEdges) {
    int ** table = malloc((numNodes + 1) * sizeof(int *));
    for(int i = 0; i <= numNodes; i++) {
        table[i] = malloc(numNodes * sizeof(int));
    }
    return table;
}

bool checkK(int k) { //checks if k is power of 2
    return (k == 0 ? false : ((k & (k - 1)) == 0) ? true : false);
}


// float MCM(char * filename, headNode ** adjList, int numEdges, int numNodes, int * checkValid) {
//     double ** dTab = distanceTable(numNodes, numEdges);
//     int ** pTab = predTable(numNodes, numEdges);
//     for(int k = 0; k <= numEdges; k++) {
//         for(int v = 0; v < numNodes; v++) {
//             dTab[k][v] = INFINITY;
//             pTab[k][v] = __INT_MAX__;
//         }
//     }
//     dTab[0][0] = 0; //initializing source
    
//     for(int k = 1; k <= numEdges; k++) {
//         //now for each edge from each node
//         //(k - 1) is source node
//         // int * storeUpdatedNodes = malloc(numNodes * sizeof(int));
//         // int updateInd = 0;
//         for(int u = 0; u < numNodes; u++) {
//             if(dTab[k - 1][u] != INFINITY) {
//                 Node * destNode = adjList[u]->dests;
//                 //CHECK FOR CYCLE AFTER EACH ROW HAS BEEN FULLY FILLED?
//                 while(destNode != NULL) {
//                     //this loop fills in the distances for a row
//                     //curNode->label - 1 (need minus 1 because node labels start at 1 instead of 0)
//                     //u is not always k - 1
//                     //need to store origins of v node
//                     if(dTab[k][(destNode->label) - 1] > dTab[k - 1][u] + destNode->weight) {
//                         dTab[k][(destNode->label) - 1] = dTab[k - 1][u] + destNode->weight;
//                         pTab[k][(destNode->label) - 1] = u + 1; //u+1 because pred table is storing actual node num
//                     }
//                     //check after a given destination node best distance is found
//                     if(checkK(k) || k == numEdges) { //test if k is power of 2 or if last possible value of k
//                         int * cycle = malloc((k + 1)*sizeof(int));
//                         double MCM = 0;
//                         int startInd;
//                         int endInd;
//                         if(detectCycle(k, adjList, adjList[destNode->label - 1], pTab, destNode->label - 1, &cycle, &MCM, dTab, numNodes, &startInd, &endInd)) {
//                             //now check to see if all edge conditions are fulfilled
//                             printDistTable(dTab, numEdges, numNodes);
//                             double * piArray = piVals(adjList, MCM, dTab, numNodes, k);
//                             if(evalMCM(adjList, MCM, piArray, numNodes)) {
//                                 //printPredTable(pTab, numEdges, numNodes);
//                                 free(piArray);
//                                 freeDist(dTab, numEdges);
//                                 freePred(pTab, numEdges);
//                                 //printCycle(cycle, startInd, endInd);
//                                 writeCycle(filename, cycle, startInd, endInd);
//                                 free(cycle);
//                                 *checkValid = 1;
//                                 return MCM;
//                             }
//                             free(piArray);
//                         }
//                         //resetVisited(adjList, cycle, numNodes);
//                         free(cycle);
//                         //printf("do cycle checking/early termination\n");
//                     }
//                     destNode = destNode->next;
//                 }
//             }
//         }
//         //printPredTable(pTab, numEdges, numNodes);
//     }
//     freeDist(dTab, numEdges);
//     freePred(pTab, numEdges);
//     return -1;
// }
float MCM(char * filename, headNode ** adjList, int numEdges, int numNodes, int * checkValid) {
    double ** dTab = distanceTable(numNodes, numEdges);
    int ** pTab = predTable(numNodes, numEdges);
    for(int k = 0; k <= numNodes; k++) {
        for(int v = 0; v < numNodes; v++) {
            dTab[k][v] = INFINITY;
            pTab[k][v] = __INT_MAX__;
        }
    }
    dTab[0][0] = 0; //initializing source
    
    for(int k = 1; k <= numNodes; k++) {
        //now for each edge from each node
        //(k - 1) is source node
        for(int u = 0; u < numNodes; u++) {
            if(dTab[k - 1][u] != INFINITY) {
                Node * destNode = adjList[u]->dests;
                //CHECK FOR CYCLE AFTER EACH ROW HAS BEEN FULLY FILLED?
                while(destNode != NULL) {
                    //this loop fills in the distances for a row
                    if(dTab[k][(destNode->label) - 1] > dTab[k - 1][u] + destNode->weight) {
                        dTab[k][(destNode->label) - 1] = dTab[k - 1][u] + destNode->weight;
                        pTab[k][(destNode->label) - 1] = u + 1; //u+1 because pred table is storing actual node num
                    }
                    destNode = destNode->next;
                }
            }
        }
        if(checkK(k) || k == numNodes) { //test if k is power of 2 or if last possible value of k
            for(int i = 0; i < numNodes; i++) {
                if(dTab[k][i] != INFINITY) {
                    int * cycle = malloc((k + 1)*sizeof(int));
                    if(cycle == NULL) {
                        fprintf(stderr, "\nHELPME\n------------\n");
                    }
                    double MCM = 0;
                    int startInd;
                    int endInd;
                    if(detectCycle(k, adjList, adjList[i], pTab, i, &cycle, &MCM, dTab, numNodes, &startInd, &endInd)) {
                        //now check to see if all edge conditions are fulfilled
                        //printDistTable(dTab, numEdges, numNodes);
                        double * piArray = piVals(adjList, MCM, dTab, numNodes, k);
                        if(evalMCM(adjList, MCM, piArray, numNodes)) {
                            //printPredTable(pTab, numEdges, numNodes);
                            free(piArray);
                            freeDist(dTab, numNodes);
                            freePred(pTab, numNodes);
                            //printCycle(cycle, startInd, endInd);
                            writeCycle(filename, cycle, startInd, endInd);
                            free(cycle);
                            *checkValid = 1;
                            //printf("early terminated at k: %d\n", k);
                            return MCM;
                        }
                        free(piArray);
                    }
                    free(cycle);
                }
            }
        }
    }
    freeDist(dTab, numNodes);
    freePred(pTab, numNodes);
    return -1;
}

//run this function for every vertex in D table that is not an infinity for the kth row
bool detectCycle(int k, headNode ** adjList, headNode * source, int ** predTable, int tableInd, int ** cycle, double * MCM, double ** distTable, int numNodes, int * startInd, int * endInd) {
    //detect a cycle in O(k) and return the path of the cycle
    //int ind = 0;
    int * saveRow = malloc(numNodes * sizeof(int));
    int * saveStartCycle = malloc(numNodes * sizeof(int));
    (*cycle)[0] = tableInd + 1;
    source->visited = true;
    saveRow[tableInd] = k;
    saveStartCycle[tableInd] = 0; //if first node is source of cycle save 0th index of cycle array
    int nodeLabel = predTable[k][tableInd];
    //source Val is the label of the source node (outer array in adjList)
    //use adjList to see if a node is visited or not

    //For self edges must add a check to see if predTable[k][sourceVal - 1] == source, if so then self-edge = cycle
    *MCM = distTable[k][tableInd]; //if there is a cycle MCM is the value at the last/source node at k
    //int nextSourceVal = sourceVal;
    
    //SAVE THE INDEX AT WHICH THE SOURCE NODE IS TO OUTPUT THE CORRECT CYCLE
    //LIKE CYCLE = CYCLE[NODELABEL - 1:] (IN PYTHON BUT HERE JUST SAVE THE INDEX AND USE IT TO OUTPUT CYCLE)
    int help = saveRow[nodeLabel - 1];

    //Double check if i <= k is necessary why not just i < k
    //Because what if self edges
    for(int i = 1; i <= k; i++) {
        if(adjList[nodeLabel - 1]->visited == true) {
            //*MCM = distTable[not k but something else][nodeLabel - 1] this is the node that was source and end of cycle=
            int help2 = saveRow[nodeLabel - 1];
            *MCM = (distTable[saveRow[nodeLabel - 1]][nodeLabel - 1] - distTable[k-i][nodeLabel - 1])/ (saveRow[nodeLabel - 1] - (k-i));
            *startInd = saveStartCycle[nodeLabel - 1];
            *endInd = i - 1;
            free(saveStartCycle);
            free(saveRow);
            //*MCM = *MCM / (k - i);
            //Put RESETVISITED Here i should be the number of items in cycle
            resetVisited(adjList, *cycle, i - 1);
            //output the indexes of the cycle used
            //should be 
            return true;
        }
        saveRow[nodeLabel - 1] = k - i;
        help = saveRow[nodeLabel - 1];
        //dont do cycle[i] when i == k
        (*cycle)[i] = nodeLabel;
        saveStartCycle[nodeLabel - 1] = i;
        adjList[nodeLabel - 1]->visited = true;
        nodeLabel = predTable[k-i][nodeLabel - 1];
        //nextSourceVal = nodeLabel;
    }
    // for(int i = k; i >= 0; i--) {
    //     cycle[ind] = predTable[k][]
    // }
    //RESET VISITED HERE i+1 should be the number of items in cycle
    resetVisited(adjList, *cycle, k);
    free(saveRow);
    free(saveStartCycle);
    return false;


}

//can pass in the cycle and just mark each thing in the cycle to be false. Need num of elements in cycle
// void resetVisited(headNode ** adjList, int * cycle, int numNodes) {
//     for(int i = 0; i < numNodes; i++) {
//         adjList[i]->visited = false;
//     }
//     // for(int i = 0; i < numVals in Cycle; i++) {
//     //     int j = cycle[i]
//     //     adjList[j - 1]->visited = false;
//     // }

// }

void resetVisited(headNode ** adjList, int * cycle, int numInCycle) {
    //printf("\n");
    for(int i = 0; i <= numInCycle; i++) {
        //printf("nodeLabel: %d ", cycle[i]);
        adjList[cycle[i] - 1]->visited = false;
    }
    //printf("\n");
    // for(int i = 0; i < numVals in Cycle; i++) {
    //     int j = cycle[i]
    //     adjList[j - 1]->visited = false;
    // }

}

double * piVals(headNode ** adjList, double mcm, double ** distTab, int numNodes, int k) {
    double * piVals = malloc(numNodes * sizeof(double));
    for(int v = 0; v < numNodes; v++) {
        double piVal = INFINITY;
        for(int i = 0; i <= k; i++) {
            double help3 = distTab[i][v] - i*mcm;
            if(distTab[i][v] - i*mcm < piVal) {
                piVal = distTab[i][v] - i*mcm;
            }
        }
        piVals[v] = piVal;
    }
    //pival[0] corresponds to node 1, [1] corresponds to node 2 and so on
    return piVals;
}

bool evalMCM(headNode ** adjList, double mcm, double * piVals, int numNodes) {
    // printf("\n");
    // for(int i = 0; i < numNodes; i++) {
    //     printf("piVal[%d]: %lf ", i + 1, piVals[i]);
    // }
    // printf("\n");
    for(int i = 0; i < numNodes; i++) {
        Node * curNode = adjList[i]->dests;
        while(curNode != NULL) {
            //minus 1 because label corresponds to node num not index num
            if(piVals[(curNode->label - 1)] > piVals[i] + curNode->weight - mcm) {
                double diff = piVals[(curNode->label - 1)] - (piVals[i] + curNode->weight - mcm);
                if((diff / (mcm + diff)) > (1 * pow(10, -8)) || diff == INFINITY) {
                    return false;
                }
            }
            curNode = curNode->next;
        }
    }
    return true;
}
