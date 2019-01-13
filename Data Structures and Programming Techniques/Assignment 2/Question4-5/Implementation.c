#include <stdio.h>
#include <stdlib.h>
#include "Interface.h"
#define SIZE 30

int top;
Link stack[SIZE];

struct NodeTag{
	ItemType Item;
	Link Left;
	Link Right;
};


/*ADT Binary Tree */
	
Link Create(Link N){				//create empty binary tree
	
	N=(struct NodeTag *)malloc(sizeof(struct NodeTag ));
	
	if(N==NULL) printf("No memory");
	
	N->Left=NULL;					//childs must be null
	N->Right=NULL;
	
	return N;
			
}
 

int IsEmpty(Link N){			//check if binary tree is empty
	
	return ((N->Left==NULL) && (N->Right==NULL));		//return 1 if true, else return 0 if false
}

Link MakeTree(Link Root,Link L,Link R){	
	
	Root->Left=L;		//Link Left is now left child of Root
	Root->Right=R;	//Link right is now right child of Root	
	
	return Root;	
}



void Delete(Link N){		//delete binary tree,non recursively
	
	if(N!=NULL){				//deleting the tree in post order
		
		Delete(N->Left);		//deleting left subtree
		Delete(N->Right);		//deleting right subtree
		
		
		printf("Removing item %c\n", N->Item);
		free(N);				//deleting root
		
		N=NULL;					//setting root=NULL
	}	
	
}




int Height(Link N){
	int left_height,right_height;
	
	Link Q=N;
	
	if(Q!=NULL){
		left_height=Height(N->Left);	//compute each subtree height
		right_height=Height(N->Right);
		
		if(left_height>right_height) return (left_height+1);		//+1, to add the root
		else return (right_height+1);							//right_height=>left_height		
	}	
}




void PrintLevel(Link N,int level){			//complementary function to LevelOrder
	
	if(N!=NULL){
		
		if(level==1) printf("%c ",N->Item);		//reached said level, print item
		if(level>1){
			PrintLevel(N->Left,level-1); 		//call function to reach said level and print the respective items
			PrintLevel(N->Right,level-1);		//recursively,from both subtrees
		}		
	}
}
					
					
void LevelOrder(Link N){
	
	if(N==NULL) return;
	
	int height=Height(N);		//compute height
	int i;
	
	
	for(i=1;i<=height;i++){		//print every level till the last level , and print that too
		
		PrintLevel(N,i);
		
	}
}	




void PreOrder(Link N){
	Link Q=N;
	if(Q!=NULL){
		printf("%c ",Q->Item);		//print root
		PreOrder(Q->Left);			//print left subtree
		PreOrder(Q->Right);			//print right subtree
	}
}

void InOrder(Link N){
	Link Q=N;
	if(Q!=NULL){
		InOrder(Q->Left);		//print left subtree
		printf("%c ",Q->Item);	//print root
		InOrder(Q->Right);		//print right subtree
	}
}


void PostOrder(Link N){
	Link Q=N;
	if(Q!=NULL){
		PostOrder(Q->Left);		//print left subtree
		PostOrder(Q->Right);	//print right subtree
		printf("%c ",Q->Item);	//print root
	}
}

void Print(Link N,int k){	//the tree is printed horizontally				
	int i;			 
	
	if(N==NULL) return;
	
	Print(N->Right,k+10);	
							
		
	for(i=0;i<k;i++) printf(" ");
	
	printf("%c\n",N->Item);
	Print(N->Left,k+10); 	
	
}



int Size(Link N){ 
	
	if(N==NULL) return 0;
	else{
		return (Size(N->Left)+Size(N->Right)+1);		//+1 for root
	}
}

int PathLength(Link N,int k){	//k=level of tree, at start k=0
	
	Link Q=N;
	if(Q==NULL) return 0;

return k+PathLength(Q->Left,k+1)+PathLength(Q->Right,k+1);	
	
}

int InternalPathLength(Link N,int k){	//since we have an extended tree , InternalPathLength=PathLength
	
	Link Q=N;
	if(Q==NULL) return 0;   
	
	return k+InternalPathLength(Q->Left,k+1)+InternalPathLength(Q->Right,k+1);
}

int ExternalPathLength(Link N){
	
	Link Q=N;		
	
	return InternalPathLength(Q,0)+(2*Size(Q));		//for every binary tree: external path=internal path+ (2*n)
													//n is the number of internal nodes=Size(root)												
}



/* Stack Implementations to use for the postfix problem */

Link Create2(char exp[]){	//similar to the previous Create, but with a few alterations
	
Link temp;
 
int position;		//shows 1st,2nd,...... character of string
char ch;
 
position=0;		    //first character of string
 
ch=exp[position];
 
	while(ch!='\0'){ 		/*create new node*/
 		
 		temp=Create(temp);
 		
		temp->Item=ch;
 
			if((ch>='0') && (ch<='9')) push(temp);
			
			else if(ch=='+' ||ch=='-' || ch=='*' || ch=='/'){ 

				temp->Right=pop(); 
				temp->Left=pop();
 
				push(temp);
 
			}
			position++;
 
		ch=exp[position];
 
	}
 
 	temp=pop();
 
return(temp);
}

void push(Link N){ 
 
if(top>=SIZE-1) printf("Stack is full\n");
  
stack[++top]=N;
 
}


Link pop(){
 
Link N;
 
if(top==-1) printf("stack is empty\n");

N=stack[top--];
  
return N;
}



				
