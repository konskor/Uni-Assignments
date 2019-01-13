#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Interface.h"

struct NodeTag {
                    AirportCode Airport;
                    struct NodeTag *Link;
                };



void InsertNewLastNode(char *A, NodeType **L)
{
      NodeType *N, *P;

      N=(NodeType *)malloc(sizeof(NodeType));
      strcpy(N->Airport, A);
      N->Link=NULL;

      if (*L == NULL) {
         *L=N;
      } else {
         P=*L;
         while (P->Link != NULL) P=P->Link;
         P->Link=N;
      }

}



void PrintList(NodeType *L)
{
      NodeType *N;

      printf("(");
      N=L;
      while(N != NULL) {
         printf("%s", N->Airport);
         N=N->Link;
         if (N!=NULL) printf(",");
      }
      printf(")\n");
}


void DeleteLastNode(NodeType **L)
{
      NodeType *PreviousNode, *CurrentNode;

      if (*L != NULL) {
         if ((*L)->Link == NULL){
            free(*L);
            *L=NULL;
         } else {
            PreviousNode=*L;
            CurrentNode=(*L)->Link;
            while (CurrentNode->Link != NULL){
               PreviousNode=CurrentNode;
               CurrentNode=CurrentNode->Link;
            }
            PreviousNode->Link=NULL;
            free(CurrentNode);
         }
      }
}


void InsertNewSecondNode (NodeType **L)
{
      NodeType *N;
      N=(NodeType *)malloc(sizeof(NodeType));
      strcpy(N->Airport,"BRU");
      N->Link=(*L)->Link;
      (*L)->Link=N;
}



NodeType *ListSearch(char *A, NodeType *L)
{
      NodeType *N;

      N=L;
      while (N != NULL){
        if (strcmp(N->Airport,A)==0){
           return N;
        } else {
           N=N->Link;
        }
      }
      return N;
}

int Length(NodeType *L){
	int count;
	NodeType *N;
	
	N=L;
	if(N==NULL){
		count=0;

	}else {
		count=1;
		}
	
	while(N->Link!=NULL){
		N=N->Link;
		count+=1;
	}
return count;	
}

void Delete(NodeType *L,int i){
	int j;
	NodeType *N,*Q,*H;
	N=L;
	H=L;
	
	if(i<1 || i>Length(L)) printf("Error, node not found\n"); // 1<=i<=length
	
	if(N!=NULL){			//if list empty then break;
		
		
		if(N->Link==NULL){		//if list has one node
			free(N);
			N=NULL;
			return;
		}
									//if list has more than one node(i>=1)
		for(j=1;j<i-1;j++){			//ignores if i==1 || i==2
			N=N->Link;				
		}
		
		
			
		Q=N;			
		N=N->Link;		
		
		if(i==1){ 		//free first node
			free(H);
			H=N;		//head is now "second node"
			return;
		}
		
		
		if(i==2){			//free second node
		
		H->Link=N->Link;	//head links with 3rd node
		free(N);
		N=H;				//resetting N
		return;
		}
							
							//i>=3
							//Q is (i-1)node and N is i-node
		Q->Link=N->Link;	//(i-1)node->link=(i+1)node
		N->Link=NULL;		//here we delete the node
		free(N);
		N=NULL;
	}
return;	
}


void Insert(NodeType *L,char *X,int i){
	NodeType *N,*Q;
	int j;
	N=L;
	
	Q=(NodeType *)malloc(sizeof(NodeType *)); //creating a new node, in case we need it
	
	if(N!=NULL){		//if list is not empty, continue 
	
	if(i<1 || i>Length(L)+1) printf("Error,node not found\n");
 	
		if(N->Link==NULL ){ //if list has one node
			
			if(i==1) strcpy(N->Airport,X);
			if(i==2){
				
				Q=(NodeType *)malloc(sizeof(NodeType *)); //creating new node
				
				N->Link=Q;
				strcpy(Q->Airport,X);
				Q->Link=NULL;  //Q is last node
				
			}
		}
										//list has more than one node
		for(j=1;j<i-1;j++){  			//ignores if i==1 || i==2
		
			if(N->Link==NULL) break; 	//arrived to last node
				
			N=N->Link;					//N will be (i-1)Node	
		}
		
			if(i==1){
			 strcpy(N->Airport,X);
			 return;
			}
			
			if(i==2){
				N=N->Link;
				strcpy(N->Airport,X);
				return;
			}	
		
			if(N->Link==NULL){ //i=length(L)+1 aka N is last node(for now)
				
				Q=(NodeType *)malloc(sizeof(NodeType *));	//creating new node
				
				N->Link=Q;									//Q is the new last node
				strcpy(Q->Airport,X);
				Q->Link=NULL;			
			}else{
				N=N->Link;			//N is i-node
				strcpy(N->Airport,X);	
			}
	}
return;	
}
	
