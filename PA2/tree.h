typedef struct _stackNode {
    struct _stackNode * next;
    struct _Node * curr;
} stackNode;


//make dims a long array
//make origin and offset longs as well
// also numReps and allReps
// same with bestRep
typedef struct _Node {
    struct _Node * left;
    struct _Node * right;
    int label;
    int numReps; //number of optimal representations
    int ** dims; //outer array is different representations
    //inner array[0]=width and array[1] = length
    int originX;
    int originY;
    int offsetX;
    int offsetY;
    char label2;
    int rep; //each node will have their own representation num
    int bestRep; 
    int allReps; //all reps including suboptimal ones
    //0 if no internal nodes below
} Node;
