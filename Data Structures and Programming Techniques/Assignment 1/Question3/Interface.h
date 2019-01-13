//Type definitions
typedef char AirportCode[4];
typedef struct NodeTag  NodeType;
typedef NodeType *NodePointer;


// function prototypes
void InsertNewSecondNode(NodeType **);
void InsertNewLastNode(char *, NodeType **);
void DeleteLastNode(NodeType **);
NodeType *ListSearch(char *, NodeType *);
void PrintList(NodeType *);

int Length(NodeType *);
void Delete(NodeType *,int);
void Insert(NodeType *,char *,int);

