//Type Definitions
typedef char AirportCode[4];
typedef struct NodeTag NodeType;
typedef NodeType *NodePointer;

//function prototypes
void InsertNewLastNode(char *, NodeType **);

NodeType *NodeSearch(char *,NodeType *);
void DisplayList(NodeType *);
