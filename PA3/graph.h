//stores destination of edges
typedef struct _Node {
    struct _Node * next;
    struct _Node * tail; //memory address of current last node in linked list
    int label;
    double weight;
    //int curInd;

} Node;

//[0] is source
typedef struct _headNode {
    //struct _Node * sources; //sources of edges that point to headNode
    struct _Node * dests; //destination of edges that headNode points to
    bool visited; //true if visited false if not
} headNode;