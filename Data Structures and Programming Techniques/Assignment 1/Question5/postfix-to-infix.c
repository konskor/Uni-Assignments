/* This code is from Chapter 4 of the book "Algorithms in C" by Robert Sedgewick. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Item.h"
#include "STACK.h"

typedef struct STACKnode* link;
struct STACKnode {  ItemStack item; 
					link next;};
					
static link head;



main(int argc, char *argv[])
  { char *a;						
  	link result;					//result of the function post to infix
  	
  	result=malloc(sizeof *result);
  	a=malloc(sizeof(char));
  	
	gets(a);						//input string
	printf("The postfix form:\n");
	printf("%s\n",a);
  
  	int i, k = strlen(a);
  	
  	printf("%d\n",k);
  	
    STACKinit(k);
       
    Post_to_Infix(a,k);
    

    
    if(STACKempty==0) printf("STACK EMPTY");
    
    printf("The infix form:\n");
  
    
    
  }
  

  
  
  
  
  
  
  
