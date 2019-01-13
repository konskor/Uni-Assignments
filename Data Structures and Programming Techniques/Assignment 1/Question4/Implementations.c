/* This is the file QueueImplementation.c */

#include <stdio.h>
#include <stdlib.h>
#include "QueueInterface.h"

struct QueueNodeTag {
                    ItemType Item;
                    struct QueueNodeTag *Link;
                						};

struct QPointers {
        			QueueNode *Front;
        			QueueNode *Rear;
    							};            

typedef struct QPointers Queue;

void InitializeQueue(Queue *Q)
{
    Q->Front=NULL;
    Q->Rear=NULL;
}

int Empty(Queue *Q)
{
   return(Q->Front==NULL);
}

int Full(Queue *Q)
{
   return(0);   //the queue doesnt have a fixed size
}


void Insert(ItemType R, Queue *Q)
{
   QueueNode *Temp;

   Temp=(QueueNode *)malloc(sizeof(QueueNode));

   if (Temp==NULL){
      printf("System storage is exhausted");
   } else {
      Temp->Item=R;
      Temp->Link=NULL;
      if (Q->Rear==NULL){
         Q->Front=Temp;
         Q->Rear=Temp;
      } else {
         Q->Rear->Link=Temp;
         Q->Rear=Temp;
      }
   }
}

   
void Remove(Queue *Q, ItemType *F)
{
   QueueNode *Temp;


   if (Q->Front==NULL){
      printf("attempt to remove item from an empty queue");
   } else {
      *F=Q->Front->Item;
      Temp=Q->Front;
      Q->Front=Temp->Link;
      free(Temp);
      if (Q->Front==NULL) Q->Rear=NULL;
   }
}


void LastManStanding(Queue *Q,int spare){	//Russian Roulette.... or not? SPOILERS its rigged......
	QueueNode *temp;
	Queue *N;
	int i;

	//transforming the current linked list into a circular one
	Q->Rear->Link=Q->Front;	 //connecting last node with the first node
	
	N=Q;
		
	
	while((N->Front)->Link!=(N->Front)){	 //it is a circular list, so if it links with itself, then no other nodes exist, one person left
		
		if(spare==1){ 						//everyone removes the next guy;
			
			temp=N->Front;							//temp is (M-1)person
			N->Front=N->Front->Link;				//N is M-person
			temp->Link=N->Front->Link;				//links with M+1 person
			
			
			printf("We remove the %d person\n",N->Front->Item);
			
			
			N->Front->Link=NULL;			//erasing node
			free(N->Front);
			N->Front=temp->Link;		//resseting the procedure starting form M+1 person
			
			printf("N->front shows %d person\n",N->Front->Item);
			//printf("The last man standing if we kill the 2th person each time, is the number %d person\n",N->Front->Item);
			
		}
		
		
		if(spare>1){
		
			for(i=1;i<spare;i++){		
			
				N->Front=N->Front->Link;  //N->Front is (M-1)person
			}
		
			temp=N->Front;						//temp is (M-1) person						
			N->Front=N->Front->Link;			//N->Front is M-person
			temp->Link=N->Front->Link;			//temp->Link shows M+1 person
			printf("We remove the %d person\n",N->Front->Item);
		
			N->Front->Link=NULL;		//we start removing the person(nullifying the link)
			free(N->Front);				//erasing the contents of M node(removing person M)
		
			N->Front=temp->Link;	//reseting the procedure starting from now from M+1 person instead
			printf("N->front is %d\n",N->Front->Item);			
		}
	}
	
printf("The last man standing if we remove the %dth person each time, is the number %d person\n",spare+1,N->Front->Item);
	
}











