#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Interface.h"


struct NodeTag{
	ItemType Item;
	Link Left;
	Link Right;
};

int main(void){
	
/*Testing Binary Tree ADT */
	
	printf("The testing of Binary Tree ADT starts:\n");
	Link A,B,C,D,E,F,G;
	
	A=Create(A);	// creating nodes
	A->Item='/';
	
	B=Create(B);
	B->Item='*';
	
	C=Create(C);
	C->Item='+';
	
	D=Create(D);
	D->Item='4';
	
	E=Create(E);
	E->Item='6';
	
	F=Create(F);
	F->Item='5';
	
	G=Create(G);
	G->Item='3';
	
	
	
	if(IsEmpty(A)!=0) printf("Tree is empty at start\n");
	else printf("Tree is not empty start\n");
	printf("\n");	
		
		
	A=MakeTree(A,B,C);	//A is root, with left child B and right child C
	B=MakeTree(B,D,E);	//B is root of left subtree, with left child D and right child E 
	C=MakeTree(C,F,G);	//C is root of right subtree  with left child F and right child G
	

	
	
	if(IsEmpty(A)!=0) printf("Tree is empty now\n");
	else printf("Tree is not empty now\n");
	printf("\n");
	

	Print(A,0);
	
	printf("\n");
	
	printf("Height is %d\n",Height(A));
	printf("Size is %d\n",Size(A));
	
	printf("PreOrder is:");
	PreOrder(A);
	printf("\n");
	
	printf("PostOrder is:");
	PostOrder(A);
	printf("\n");
	
	printf("InOrder is:");
	InOrder(A);
	printf("\n");
	
	printf("LevelOrder is:");
	LevelOrder(A);
	printf("\n");
	
	printf("Path Length is:%d\n",PathLength(A,0));
	printf("Internal Path Length is : %d\n",InternalPathLength(A,0));
	printf("External Path Length is : %d\n",ExternalPathLength(A));
	
	
	Delete(A);
	
	printf("Done Testing\n");

/*Postfix Expression Problem*/
	
	printf("Moving on to postfix problem\n");
	
Link root;
char exp[20];

printf("Write the postfix expression\n");
scanf("%s",exp);

int top=-1; 									//initialise stack
root=Create2(exp);					//create the expression tree

printf("Tree created\n");
Print(root,0);

	
return 0;		
}

