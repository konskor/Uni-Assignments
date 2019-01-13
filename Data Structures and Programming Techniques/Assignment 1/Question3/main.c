#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Interface.h"



int main(void)
{
    NodeType *L;
	int position;
	char *a;
	a=malloc(sizeof(char *));
	
	L=NULL;
      
      PrintList(L);
      
      InsertNewLastNode("DUS", &L);
      InsertNewLastNode("ORD", &L);
      InsertNewLastNode("SAN", &L);
      InsertNewLastNode("FOR", &L);
		
	printf("Currently list is:\n");	
	PrintList(L);
	printf("Length of list is %d\n",Length(L)); //testing
	  

		
	printf("Inserting second node ,with data BRU\n");
    InsertNewSecondNode(&L);
	  
	  
    PrintList(L);
	
	printf("Deleting Last Node\n");
    DeleteLastNode(&L);
    
	PrintList(L);  
      

	printf("Testing Insert,HAL in node 2\n");
    Insert(L,"HAL",2);   
    
    PrintList(L);
      
    printf("Deleting node 3\n");
    Delete(L,3);
				
	PrintList(L);
      
      if (ListSearch("DUS",L) != NULL) {
        printf("DUS is an element of the list\n");
      }
}



