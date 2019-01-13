/* This code is from Chapter 4 of the book "Algorithms in C" by Robert Sedgewick. */

#include <stdlib.h>
#include "Item.h"
typedef struct STACKnode* link;
struct STACKnode { Item item; link next; };
static link head;


link NEW(Item item, link next)
  { link x = malloc(sizeof *x);
    x->item = item; x->next = next;
    return x;
  }
void STACKinit(int maxN)
  { head = NULL; }
int STACKempty()
  { return head == NULL; }
STACKpush(Item item)
  { head = NEW(item, head); }
Item STACKpop()
  { Item item = head->item;
    link t = head->next;
    free(head); head = t;
    return item;
  }
  
  void Post_to_Infix(char *c,int k){	//k=strlen(c)
  	printf("c is %s\n",c);
  	printf("k is %d\n",k);
	  
	
  	int i=0;
  	
  	char *temp1,*temp2,*temp3;
  	
	temp1=malloc(sizeof(char*));
	temp2=malloc(sizeof(char*));
	temp3=malloc(sizeof(char*));
	
	
  	
  	
  	
  	while(c[i]!='\0'){	//'\0' is EOF(end of file)
  		
		printf("*c is %c\n",c[i]);
  		
  		
  		
  		if(c[i]==' '|| c[i]==',') continue;				//ignore spaces and comas
  		if(c[i]>='0' && c[i]<='9'){
  			

			
			STACKpush(c[i]);
			
  			
		}else if(c[i]=='+' || c[i]=='-' || c[i]=='*' || c[i]=='/' || c[i]== '^'){
			if(head->next==NULL) printf("Error not enough values\n");				// less than 2 values on stack
			
	
			
			temp2=STACKpop();			//second operand
			temp3=STACKpop();			//first operand
			printf("temp2 is %c\n",temp2);
			printf("temp3 is %c\n",temp3);
			
		
			printf("temp1 is %c\n",temp1);
			strcpy(temp1,"(");
			strcat(temp1,temp2);
			strcat(temp1,&c[i]);
			strcat(temp1,temp3);
			strcat(temp1,")");
			
			
			
			
			
			STACKpush(*temp1);
			
			
			
		}
	i++;	
	}	
}   
  
