#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ProgrammInterface.h"


int main(int argc,char *argv[])
{	  
	int k,i;
    NodeType *L;
	char *a;
	  
	a=malloc(sizeof(char *));
    L=NULL;
      
    printf("How many nodes will the list have?\n");
      scanf("%d",&k);
      
    printf("Insert Nodes:(strings)\n");  
      for(i=0;i<k;i++){
	  	scanf("%s",a);
      	InsertNewLastNode(a, &L);
  		}
      
	printf("The current list is:\n");
	DisplayList(L);
	  
	printf("What are you searching for?(string)\n");
	  scanf("%s",a);

    if (NodeSearch(a,L) != NULL) {
        printf("Airport Code found\n");
	}
    else{
      	printf("Airport Code not found\n");
	  	}
	  	
return 0;			  
}


