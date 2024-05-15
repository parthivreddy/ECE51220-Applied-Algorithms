typedef struct _Node {
    struct _Node * left;
    struct _Node * right;

    double leftWire; //wirelength to left child
    double rightWire; //wirelength to right child

    double upWire; //save upwire length
    double capacitance; //overall capacitance (running sum from bottom up)
    //double subCap; //capacitance just at that node without adding. Equivalent to capacitance at leaf node
    double sinkCapacitance; //sink capacitance of leaf nodes
    double delay; //time delay
    //double curDelay; //local delays
    double curWr; //current wire length that inverter may be driving
    int label; //label if leaf node
    int inverting; //1 if inverting, 0 if not. Needs to be 0 to be valid
    int numInverters;
    int checkInverting; //for checking inverting
    double checkCap;
    double checkDelay;
    double sinkCap2;
} Node;

typedef struct _stackNode {
    struct _stackNode * next;
    struct _Node * curr;
} stackNode;

typedef struct _wireNode {
    struct _wireNode * next;
    double upWire; //can be upWire length (wirelength above node)
    double upDelay; //delay value from node above
} wireNode;



