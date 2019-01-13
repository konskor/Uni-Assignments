#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Interface.h"


struct NodeTag {
                    AirportCode Airport;
                    struct NodeTag *Link;
                };





void InsertNewLastNode(int A, NodeType **L) 
{
      NodeType *N, *P;

      N=(NodeType *)malloc(sizeof(NodeType));
      N->Airport=A;
      N->Link=NULL;

      if (*L == NULL) {
         *L=N;
      } else {
         P=*L;
         while (P->Link != NULL) P=P->Link;
         P->Link=N;
      }

}

void swap(NodeType *L,NodeType *P){	//swap values of nodes
	int temp=L->Airport;
	
	L->Airport=P->Airport;
	P->Airport=temp;

	}
	
void OrganiseList(NodeType *L){		
	NodeType *N,*min,*H;
	
	H=L;			//H for head
	N=L;			
	
	while(H->Link!=NULL){
			
		min=H;
		N=H->Link;
		
		while(N!=NULL){
			
			if(N->Airport < min->Airport) min=N;
			N=N->Link;
		}
		
	swap(H,min);
	H=H->Link;	
	}

}

void PrintList(NodeType *L){
	NodeType *Q;
	Q=L;
	
	while(Q->Link!=NULL){
		printf("%d->",Q->Airport);
		Q=Q->Link;
	}
	
	printf("%d->NULL\n",Q->Airport);
	
}	


