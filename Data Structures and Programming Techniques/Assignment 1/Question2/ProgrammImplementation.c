#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ProgrammInterface.h"


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
NodeType *NodeSearch(char *A,NodeType *L){			
		
			NodeType *N;

			N=L;
			while(N!=NULL){
			
				if(strcmp(N->Airport,A)==0){
		
					return N;					//break; (We found the airport code) stop the search
				}
				else{
					 N=N->Link;
					 
						if(N->Link==NULL) return NULL;  //break;(searched the last node , didnt find it)
				
					NodeSearch(A,N);
					}

		}
		
	return L;  
}


void DisplayList(NodeType *L){
	NodeType *N;
	N=L;
	while(N!=NULL){				//print every node and the last node, and then break;
		
		printf("%s->",N->Airport);
		N=N->Link;
		
	}
	printf("NULL");
	printf("\n");
}

