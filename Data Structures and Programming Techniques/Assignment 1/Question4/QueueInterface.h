#include "QueueTypes.h"

void InitializeQueue(Queue *Q);
int Empty(Queue *);
int Full(Queue *);
void Insert(ItemType , Queue *);
void Remove(Queue *, ItemType *);

void LastManStanding(Queue *,int);
int Qsize(Queue *);
void PrintItems(Queue *,int);



