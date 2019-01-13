//type definition
typedef struct NodeTag Node;
typedef Node  *Link;  
typedef char ItemType;
typedef struct Stack *StackLink;

Link Create(Link);
int IsEmpty(Link);
Link MakeTree(Link,Link,Link);
void Delete(Link);

void PreOrder(Link);
void InOrder(Link);
void PostOrder(Link);

             
void LevelOrder(Link);
void PrintLevel(Link,int); //complementary function to LevelOrder

int Height(Link);
int Size(Link);

void Print(Link,int);

int PathLength(Link N,int k);
int InternalPathLength(Link N,int k);
int ExternalPathLength(Link N);

/*Stack functions */

Link Create2(char exp[]);
void push(Link N);
Link pop();
