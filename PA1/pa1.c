#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "pa1.h"


wireNode * pushWire(wireNode * node, double wire, double delay) {
    wireNode * head = malloc(sizeof(wireNode));
    if(head == NULL) {
        fprintf(stderr, "failed to allocate memory for wireNode");
        return NULL;
    }
    head->next = node;
    head->upWire = wire;
    head->upDelay = delay;
    return head;
}

wireNode * popWire(wireNode * node) {
    if(node == NULL) {
        fprintf(stderr, "Can't pop, Wire stack is empty");
        return NULL;
    }
    wireNode * curr = node->next;
    free(node);
    return curr;
}


//TODO: can be wire above root node and below leaf nodes if inverter not added at a node
//if inverter above root then it is root but has no right node
void upstreamCaps(Node * node, double wrCap, double invCap) { //nodes now have total capacitance seen by the resistor
    wireNode * head = NULL;
    Node * root = node;
    head = upstreamCapsHelper(node, head, wrCap, invCap);
    if(node != root) {
        fprintf(stderr, "node should be root after cap upsteam!\n");
    }
    //rootNodeCap = rootNodeCap + invCap;
    //Node * node should be root node at this point
    //node->capacitance = rootNodeCap;

}
wireNode * upstreamCapsHelper(Node * node, wireNode * head, double wrCap, double invCap) { 
    //passes in root of tree/subtree and fills in capacitance values (bottom up)
    //is the total seen capacitance at that level
    if(node == NULL) {
        return NULL;
    }
    //use a stack to hold wire distance that you came from
    //a couple of things to do: make sure leftWire and rightWire exist
    if(node->leftWire != -1) {
        head = pushWire(head, node->leftWire, -1);
        head = upstreamCapsHelper(node->left, head, wrCap, invCap);
    }
    if(node->rightWire != -1) {
        head = pushWire(head, node->rightWire, -1);
        head = upstreamCapsHelper(node->right, head, wrCap, invCap);
    }

    double capAtNode = 0; //capacitance at node without adding it to the subtree C'
    if(head != NULL) {
        double wireLength = head->upWire; //getting length of wire above node
        node->upWire = wireLength;
        head = popWire(head); //removing from stack
    
    //need to account for capacitance of root node since nothing will be in stack
    //also possible nothing in stack
    //TODO: Need to add a check if the node is directly under an inverter because then there is an extra downstream cap
        if(node->label == -1) { //internal node-> no sink capacitance
            capAtNode = ((wireLength * wrCap)/2) + (node->leftWire * wrCap)/2 + (node->rightWire * wrCap)/2;
            //node->subCap = capAtNode;
            node->capacitance = capAtNode + node->left->capacitance + node->right->capacitance;
        }
        else{//leaf node
            node->capacitance = ((wireLength * wrCap)/2) + node->sinkCapacitance;
            //node->subCap = node->capacitance;
        }
    }
    else { //root node or it is inverter node if inverter node left wire above root

//NEED TO ADD INVERTER DOWNSTREAM CAPACITCANCE
        if(node->right != NULL) { //root node
            capAtNode =  (node->leftWire * wrCap)/2 + (node->rightWire * wrCap)/2;
            node->capacitance = capAtNode + node->left->capacitance + node->right->capacitance + invCap;
            //node->subCap = capAtNode;
        }
    }
    return head;
}


//TODO: can be wire below leaf nodes and can be wire above root node
void downstreamDelay(Node * node, double wrRes, double invRes) { 
    //Node * root = node;
    wireNode * head = NULL;
    downstreamDelayHelper(node, head, wrRes, invRes);
    return;
}

wireNode * downstreamDelayHelper(Node * curr, wireNode * head, double wrRes, double invRes) {
    if(curr == NULL) {
        return NULL;
    }

    if(head != NULL) { //if non root node
        double wireLength = head->upWire;
        double upDelay = head->upDelay;
        head = popWire(head);

        //I THINK SAME OPERATION FOR internal and leaf
        double wireRes = wrRes * wireLength; //Getting actual resistance of wire above
        curr->delay = upDelay + (wireRes * curr->capacitance);
    }
    else { //root node
        curr->delay = invRes * curr->capacitance;
    }
    if(curr->left != NULL) {
        head = pushWire(head, curr->leftWire, curr->delay);
    
        head = downstreamDelayHelper(curr->left, head, wrRes, invRes);
    }
    if(curr->right != NULL) {
        head = pushWire(head, curr->rightWire, curr->delay);
    
        head = downstreamDelayHelper(curr->right, head, wrRes, invRes);
    }
    return head;
}

void printTreeCaps(Node * root) {
    if(root == NULL) {
        return;
    }
    if(root->left == NULL || root->right == NULL) {
        if(root->right != NULL || root->left != NULL) {
            fprintf(stderr, "not a strictly binary tree!");
        } 
        printf("%d(%.10le)\n", root->label, root->capacitance);
    }
    else { //internal node
        printf("(%.10le %.10le) : cap: %.10le\n", root->leftWire, root->rightWire, root->capacitance);
    }
    printTreeCaps(root->left);
    printTreeCaps(root->right);
}

void printTreeDelays(Node * root) {
    if(root == NULL) {
        return;
    }
    if(root->left == NULL || root->right == NULL) {
        if(root->right != NULL || root->left != NULL) {
            fprintf(stderr, "not a strictly binary tree!");
        } 
        printf("%d(%.10le)\n", root->label, root->delay);
    }
    else { //internal node
        printf("(%.10le %.10le) : delay: %.10le\n", root->leftWire, root->rightWire, root->delay);
    }
    printTreeDelays(root->left);
    printTreeDelays(root->right);
}

bool writeDelaysHelper(char * filename, Node * root) {
    FILE * fp = fopen(filename, "wb");
    if(fp == NULL) {
        fprintf(stderr, "Can't open and write to %s\n", filename);
        return false;
    }
    writeDelays(fp, root);
    fclose(fp);
    return true;
}

void writeDelays(FILE * ofp, Node * root) { //file already opened
    if(root == NULL) {
        return;
    }
    if(root->label != -1) { //leaf node

        fwrite(&(root->label), 1, sizeof(int), ofp);
        fwrite(&(root->delay), 1, sizeof(double), ofp);
    }
    writeDelays(ofp, root->left);
    writeDelays(ofp, root->right);
}

void readDelays(char * filename) {
    FILE * fp = fopen(filename, "rb");
    int label;
    double delay;
    while(!feof(fp)) {//until end of file
        if(fread(&label, sizeof(int), 1, fp) == 1 && fread(&delay, sizeof(double), 1, fp) == 1) {
            printf("label: %d with delay: %.15le\n", label, delay);
        }
        // fread(&label, sizeof(int), 1, fp);
        // fread(&delay, sizeof(double), 1, fp);
        
    }
    fclose(fp);
}

bool checkStringent(double * invSpecs, double maxDelay) {
    if(maxDelay <= (invSpecs[1] * invSpecs[2])) {
        return false;
    }
    else {
        return true;
    }
}

bool printPreorderHelper(char * filename, Node * root) {
    FILE * fp = fopen(filename, "w");
    if(fp == NULL) {
        fprintf(stderr, "Can't print preorder traversal of tree!\n");
        return false;
    }
    printPreorder(fp, root);
    fclose(fp);
    return true;
}
void printPreorder(FILE * fp, Node * curr) {
    if(curr == NULL) {
        return;
    }
    if(curr->label != -1) {//leaf node
        fprintf(fp, "%d(%.10le)\n", curr->label, curr->sinkCap2);
    }
    else {
        fprintf(fp, "(%.10le %.10le)\n", curr->leftWire, curr->rightWire);
    }

    printPreorder(fp, curr->left);
    printPreorder(fp, curr->right);
}


int main(int argc, char ** argv) {
    // if(argc != 9) {
    //     fprintf(stderr, "not enough arguments:%d\n", argc);
    //     return EXIT_FAILURE;
    // }
    //fprintf(stderr, "argv:%s\n%s\n%s\n%s\n%s\n", argv[0], argv[1], argv[2], argv[3], argv[4]);
    double maxDelay = atof(argv[1]);
    //printf("%s\n", argv[1]);
    double * invSpecs = malloc(3 * sizeof(double));
    //Reading in files

    if(!readInverter(argv[2], invSpecs)) {
        free(invSpecs);
        return EXIT_FAILURE;
    }
    //invSpecs[0] = input capacitance (upstream Load==capacitance sink)
    //invSpecs[1] = ouput capacitance (downstream cap)
    //invSpecs[2] = output resistance
    double * wrSpecs = malloc(2 * sizeof(double));
    if(!readWire(argv[3], wrSpecs)) {
        free(invSpecs);
        free(wrSpecs);
        return EXIT_FAILURE;
    }
    //wrSpecs[0] = per unit Resistance
    //wrSpecs[1] = per unit Capacitance
    Node * root = readTreeList(argv[4]);
    if(root == NULL) {
        free(invSpecs);
        free(wrSpecs);
        return EXIT_FAILURE;
    }

    if(!printPreorderHelper(argv[5], root)) {
        free(invSpecs);
        free(wrSpecs);
        freeTree(root);
        return EXIT_FAILURE;
    }

    //printf("hello\n");
    upstreamCaps(root, wrSpecs[1], invSpecs[1]);

    //printTreeCaps(root);

    downstreamDelay(root, wrSpecs[0], invSpecs[2]);

    //printTreeDelays(root);

    //Filling in capacitances 
    if(!writeDelaysHelper(argv[6], root)) {
        free(invSpecs);
        free(wrSpecs);
        freeTree(root);
        return EXIT_FAILURE;
    }
    //readDelays("p1.elmore");

    //double maxDelay = 100;
    if(!checkStringent(invSpecs, maxDelay)) {
        freeTree(root);
        free(invSpecs);
        free(wrSpecs);
        fprintf(stderr, "delay constraint is too stringent. Can't make progress by adding inverter\n");
        return EXIT_FAILURE;
    }
    int checkStringency = 0;
    Node * root2 = bottomUp(root, maxDelay, NULL, wrSpecs, invSpecs, root, &checkStringency);
    if(root2 != root) { //if have to insert inverter above root node
        root->numInverters = 1;
        root = root2;
    }
    if(checkStringency == 1) {
        freeTree(root);
        free(invSpecs);
        free(wrSpecs);
        fprintf(stderr, "delay constraint is too stringent. Can't make progress by adding inverter part 2\n");
        return EXIT_FAILURE;
    }
    bool check = checkInverting(root, NULL);
    printf("Your boolean variable for inverting is: %s\n", check ? "true" : "false");
    bool check2 = checkDelayHelper(root, maxDelay, invSpecs, wrSpecs, root);
    printf("Your boolean variable for delay is: %s\n", check2 ? "true" : "false");

    root->numInverters = 1; //explicitly stating implicit inverter at root
    // wr = calcMaxWrLen(invSpecs, wrSpecs, 1, maxDelay);
    //printf("\n%le\n", wr);
    if(!writeTextHelper(argv[7], root)) {
        free(invSpecs);
        free(wrSpecs);
        freeTree(root);
        return EXIT_FAILURE;
    }
    
    if(!writeOutHelper(argv[8], root)) {
        free(invSpecs);
        free(wrSpecs);
        freeTree(root);
        return EXIT_FAILURE;
    }
    freeTree(root);
    free(invSpecs);
    free(wrSpecs);

    // readDelays("examples/5.elmore");
    // printf("\n\n");
    // readDelays("outputs/5.elmore");
    // readRC("examples/5.btopo");
    // printf("\n\n");
    // readRC("outputs/5.btopo");

    return EXIT_SUCCESS;

}


// int main() {

// }