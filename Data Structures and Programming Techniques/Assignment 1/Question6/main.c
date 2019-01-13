#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Interface.h"



int main(int argc,char *argv[])
{	int i,k;
	int length;
    NodeType *L;
    L=NULL;
      
    printf("How many nodes?\n");
    scanf("%d",&length);
    
	
	printf("Inserting random elements\n");  
	     
    for(i=1;i<=length;i++){
	  k=rand();	
      InsertNewLastNode(k, &L);
  	}
  	
  	printf("\n");
  	
  	printf("Before Sorting:\n");
 	PrintList(L);
  	
 
  	
  	OrganiseList(L);
  	
  	printf("After Sorting:\n");
  	PrintList(L);
  	
  	return 0;
	  

}





