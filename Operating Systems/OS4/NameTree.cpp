#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <dirent.h>

#include <sys/stat.h>
#include <wait.h>

#include <sys/inotify.h>



//#include "NameTree.hpp"
//#include "Inode.hpp"

#include "Mystructs.hpp"



/* Used for quicksort() , at Tree::Creation() */
int str_cmp(const void *a, const void *b){

	const char **temp = (const char **) a;
	const char **temp1 = (const char **) b;

	return strcmp(*temp,*temp1);

}

//////////////////////////////////////////////////////
/////////////////////////////////////////////////////
////////////////////////////////////////////////////

void copy1(char* source, char* dest, char mode, bool contents = false){

	if( mode == 'd' ){

		//	mkdir(dest);		// create the

		pid_t child;

		child = fork();

		if( child == -1){

			perror("Error in copy ( fork (1) )");
			return;

		}else if( child == 0){

			if( contents == true){// want to copy the contents of dir, not the whole dir

				int length = snprintf(NULL,0,"%s/.", source) + 1;

				char source1[length];
				snprintf(source1,length,"%s/.", source);

				execlp("/bin/cp","cp", "-avrf" , source1 , dest, (char*) NULL); 

				perror("Error exec in copy (1): ");
				return;

			}else{

				execlp("/bin/cp","cp", "-avrf" , source , dest, (char*) NULL); 

				perror("Error exec in copy (1): ");
				return;
			}

		}else{		//parent, waits for child to finish

			int status,temp;

			temp = waitpid(child, &status, 0);
			if( temp == -1){

				perror("Error waitpid in copy (1) : ");
				return;
			}

			/* INCOMPLETE, MIGHT DO SOMETHING HERE LATER */

			return;

		}

	}else{		// copy file into directory

		pid_t child;

		child = fork();

		if( child == -1){

			perror("Error in copy ( fork (2) )");
			return;

		}else if( child == 0){

			execlp("/bin/cp","cp", "-avrf" , source , dest, (char*) NULL); 
			perror("Error exec in copy (2): ");
			return;

		}else{

			int status,temp;

			temp = waitpid(child, &status, 0);
			if( temp == -1){

				perror("Error waitpid in copy (2) : ");
				return;
			}

			/* INCOMPLETE, MIGHT DO SOMETHING HERE LATER */

			return;

		}

	}



}

///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////




void Removedir( char* name){		// remove either file or directory

	printf(" Removing %s \n\n", name);


	pid_t child;

	child = fork();

	if( child == -1){

		perror("Error in RemoveDir ( fork (1) )");
		return;

	}else if( child == 0){

		execlp("/bin/rm","rm", "-r" , name, (char*) NULL); 

		perror("Error exec in copy (1): ");
		return;

	}else{		//parent, waits for child to finish

		int status,temp;

		temp = waitpid(child, &status, 0);
		if( temp == -1){

			perror("Error waitpid in copy (1) : ");
			return;
		}


		printf(" Removed %s \n\n", name);




		/* INCOMPLETE, MIGHT DO SOMETHING HERE LATER */

		return;

	}

}


//////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
///////////////////////////////////////////////////////









/* class Node: node of list of watchers */ 

Node::Node(int wd1, TNode* tnode1){

	wd = wd1;
	tnode = tnode1;

	modified = new TList();

}


Node::~Node(){ 

	tnode = NULL;

	TLNode* temp = modified->head;

	while( temp != NULL){

		temp->tnode = NULL;		// we don't want to delete the TNodes here

		temp = temp->next;

	}

	temp = NULL;

	delete modified;



}





/* Class List: List of watchers */


List::List(){ 

	head = NULL;
	tail = NULL;

}


List::~List(){

	Node* temp = head;

	while( temp != NULL){

		head = head->next;

		delete temp;

		temp = head;

	}

	temp = NULL;
	head = NULL;
	tail = NULL;

}





void List::AddNode(Node* node1){

	// Add Node to the end of the list

	if( head == NULL){

		head = node1;
		tail = head;

		return;

	}

	// else,

	tail->next = node1;
	tail = node1;


}





void List::deleteNode(Node* node1){

	//remove Node  from list

	if(head != NULL){

		Node* temp;


		/*TNode is head */

		if( head == node1){

			temp = head;
			head = head->next;
			delete temp;

			temp = NULL;

			return;

		}

		/* TNode is tail */
		if( tail == temp){

			temp = head;

			while(temp->next != tail) temp = temp->next;	// get to TLNode before tail

			tail = temp;
			temp = temp->next;		// last TNode

			delete temp;
			tail->next = NULL;

			temp = NULL;

			return;

		}


		/* TNode is between head and tail */

		temp = head;

		Node* temp1;

		while( (temp != NULL) && ( temp->next != node1) ) temp = temp->next;

		if( temp == NULL){

			printf(" Node doesn't exist, cannot delete(TList)\n");
			return;
		}

		temp1 = temp->next;
		temp->next = temp1->next;
		delete temp1;

		temp = NULL;
		temp1 = NULL;

		return;

	}

}







void List::getNode(int wd1, Node** result){

	Node* temp = head;

	while( temp != NULL){

		if( temp->wd == wd1){

			printf(" Found our Node (with wd %d)  (getNode) \n\n", wd1);

			*result = temp;

			temp = NULL;
			return;
		}

		temp = temp->next;

	}

	printf(" Didn't find our Node (with wd %d) (getNode) \n\n", wd1);

	*result = NULL;

}
































/////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////







	/* TNode (Node of Name Tree) */

TNode::TNode(char* name1, INode* inode1, LINode* list1, TList *parent1){		// constructor

	int len;
	len = snprintf(NULL,0,"%s", name1) + 1;


	name = new char[len];
	strcpy(name,name1);			// strcopy name

	INode* temp;


	if( inode1 == NULL){	// inode1 doesn't exist => create it

		temp = new INode(name1,list1);

		inode = temp;		// get the address


		if(inode == NULL) printf("ERRROORRR !!!!!!");

		temp = NULL;

	}else{			// inode1 exists

		inode = inode1;		// get the address

	}


	//insert this->object into the list of nodes pointing to this
	inode->Tlist->AddNode(this);
	inode->name_counter++;




	child = new TList();	// initialise list of pointers 

	parent = parent1;		// pointer to the list of TNodes in Parent


}


TNode::~TNode(){		// destructor


	printf("Deleting TNode with name : %s\n\n", name);

/*
	if( inode != NULL){

		printf("Heeeelllo \n\n\n");


		inode->Tlist->Update(this);		// NULL the tnode then delete the TLNode 

		printf("Heeeelllo (1) \n\n\n");


		inode->name_counter--;

		if( inode->name_counter == 0) delete inode;

		printf("Heeeelllo (2) \n\n\n");

		inode = NULL;		// no longer pointing to inode

		printf("Heeeelllo (2) \n\n\n");

	}
*/

		inode->Tlist->Update(this);		// NULL the tnode then delete the TLNode 

//		printf("Heeeelllo (1) \n\n\n");


		inode->name_counter--;

		if( inode->name_counter == 0) delete inode;

//		printf("Heeeelllo (2) \n\n\n");

		inode = NULL;		// no longer pointing to inode




		delete child;		// call destructor of list of pointers to (children) TNodes
//		child = NULL;


//		unlink(name);		//delete file with name: name
//		delete[] name;

//		printf("Heeeelllo (2) \n\n\n");


	if( parent != NULL ){

		printf(" PARENT UPDATE IN TNODE::~TNODE \n\n");


//		parent->Update(this);		// remove us from parent list
		parent = NULL;

	}

	printf("Deleted TNode with name : %s\n\n", name);

//		printf("Heeeelllo (3) \n\n\n");

//		unlink(name);		//delete file with name: name
		delete[] name;

//		printf("Heeeelllo (4) \n\n\n");


}





















/*

TNode::~TNode(){		// destructor


	printf("Deleting TNode with name : %s\n\n", name);







	if( inode->mode == 'f'){		// regular file

//		inode->list->deleteNode(this);		// remove from the list which point to this node

	if( inode != NULL){

		inode->Tlist->Update(this);		// NULL the tnode then delete the TLNode 

		inode->name_counter--;

		if( inode->name_counter == 0) delete inode;

		inode = NULL;		// no longer pointing to inode

	}


//		unlink(name);		//delete file with name: name
		delete[] name;

		delete child;		// call destructor of list of pointers to (children) TNodes
//		child = NULL;


	}else{		//directory

		delete child;		// delete the children first
		child = NULL;

//		inode->list->deleteNode(this);		// remove from the list which point to this node


	if( inode != NULL){

		inode->Tlist->Update(this);		// NULL the tnode then delete the TLNode 
		inode->name_counter--;

		if( inode->name_counter == 0) delete inode;

		inode = NULL;		// no longer pointing to inode

	}


//		rmdir(name);		//delete directory with name: name


		delete[] name;

	}



	if( parent != NULL ){

		parent->Update(this);		// remove us from parent list
		parent = NULL;

	}


}
*/









	/* TList (List of pointers to child TNodes) */
	/* TList ( List of  (child) Tree Nodes) */

TList::TList(){

	head = NULL;
	tail = NULL;

}


TList::~TList(){

	TLNode* temp = head;

	while(head != NULL){

		head = head->next;

		delete temp;
		temp = head;

	}

	head = NULL;
	tail = NULL;
	temp = NULL;

}

////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////

void TList::AddNode(TNode* tnode1){

	// Add TNode to the end of the list

	if( head == NULL){

		head = new TLNode(tnode1);
		tail = head;

		return;

	}

	// else,

	TLNode* temp = new TLNode(tnode1);

	tail->next = temp;
	tail = temp;

	temp = NULL;

}









void TList::AddNode1(TNode* tnode1){

	// Add TNode to the appropriate position in the list, using strcmp()

	if( head == NULL){

		head = new TLNode(tnode1);
		tail = head;

		return;

	}


	//else

	TLNode* temp = new TLNode(tnode1);

	TLNode* temp1 = head;

	while(temp1 != NULL){	// strcmp all the nodes

		if( strcmp(tnode1->name, temp1->tnode->name) == 0){

			printf( " %s already exists, in AddNode1 \n\n", tnode1->name);
			return;

//		}else if( strcmp(tnode1->name, temp1->tnode->name) > 0 ){

		}else if( strcmp(tnode1->name, temp1->tnode->name) < 0 ){

			temp->next = temp1->next;		//temp is the middle guy

			temp1->next = temp;

			temp1 = NULL;
			temp = NULL;

			return;

		}

		temp1 = temp1->next;

	}

	// if every single child is returns < 0, just add it after tail

	tail->next = temp;
	tail = temp;

	temp = NULL;
	temp1 = NULL;

}














////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////











void TList::deleteNode(TNode* tnode1){

	//remove TLNode(which contains our wanted TNode) from list

	if(head != NULL){

		TLNode* temp;


		/*TNode is head */

		if( head->tnode == tnode1){

			temp = head;
			head = head->next;
			delete temp;

			return;

		}

		/* TNode is tail */
		if( tail == temp){

			temp = head;

			while(temp->next != tail) temp = temp->next;	// get to TLNode before tail

			tail = temp;
			temp = temp->next;		// last TNode

			delete temp;
			tail->next = NULL;

			return;

		}


		/* TNode is between head and tail */

		temp = head;

		TLNode* temp1;

		while( (temp != NULL) && ( temp->next->tnode != tnode1) ) temp = temp->next;

		if( temp == NULL){

			printf(" TNode doesn't exist, cannot delete(TList)\n");
			return;
		}

		temp1 = temp->next;
		temp->next = temp1->next;
		delete temp1;

		temp = NULL;
		temp1 = NULL;

		return;

	}

}





void TList::Update(TNode* tnode1){


/* Same as deleteNode, only this time we null the tnode, before deleting */

	if(head != NULL){

		TLNode* temp;


		/*TNode is head */

		if( head->tnode == tnode1){

			temp = head;
			head = head->next;

			temp->tnode = NULL;


			delete temp;

			return;

		}

		/* TNode is tail */
		if( tail == temp){

			temp = head;

			while(temp->next != tail) temp = temp->next;	// get to TLNode before tail

			tail = temp;
			temp = temp->next;		// last TNode

			temp->tnode = NULL;

			delete temp;
			tail->next = NULL;

			return;

		}


		/* TNode is between head and tail */

		temp = head;

		TLNode* temp1;

//		while( (temp != NULL) && ( temp->next->tnode != tnode1) ) temp = temp->next;

		while( (temp->next != NULL)  && (temp->next->tnode != tnode1) ) temp = temp->next;


//		if( temp == NULL){

		if( temp->next == NULL){

			printf(" TNode %s doesn't exist, cannot delete( TList::Update )\n", tnode1->name);

			temp = NULL;

			return;
		}

		temp1 = temp->next;
		temp->next = temp1->next;


		temp1->tnode = NULL;

		delete temp1;

		temp = NULL;
		temp1 = NULL;

		return;

	}

}



////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////





void TList::Print(){

	TLNode* temp = head;

	while( temp != NULL){

		printf(" %s	", temp->tnode->name);

		temp = temp->next;

	}


}












	/* TLNode (Node of TList => pointers to children TNode ) */

TLNode::TLNode(TNode* tnode1){			// constructor

	tnode = tnode1;
	next = NULL;

}


TLNode::~TLNode(){			//destructor

	delete tnode;
	next = NULL;

}














	/* Tree ( our class NameTree ) */

Tree::Tree(){		//destructor

	root = NULL;

}

Tree::~Tree(){

	delete root;

}







////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////



void Tree::Creation(char* path, LINode** list,bool recursion = false){

	/* Creation of root */

	INode* tempz;
	struct stat stat1;

//	if( recursion == false){		//only do it the first time

//		INode* tempz = new INode(path);		// created the i-node of root of sub-tree 

//	char* tempzz = basename(path) ;

//		tempz = new INode( tempzz );		// created the i-node of root of sub-tree 


	/* Get INode of file/directory and search if it exists */

//	if( stat(tempzz,&stat1) == -1){

//		perror("Error in stat() in Tree::Creation : ");
//		return;
//	}


	if( recursion == false){		//only do it the first time

	


		char* tempzz = basename(path);

//		tempz = new INode( tempzz );		// created the i-node of root of sub-tree 


		/* Get INode of file/directory and search if it exists */

		if( stat(tempzz,&stat1) == -1){

			perror("Error in stat() in Tree::Creation (1) : ");
			return;
		}



		/* Search if it exists */

//		printf("\n\n HI \n\n");

		(*list)->getINode(stat1.st_ino, &tempz);

//		printf("tempz inode_number is : %ld\n\n", (long)tempz->inode_number);

//		printf("\n\n HI 2 \n\n");


		if( tempz == NULL){		// inode with this inode number , doesn't exist

//			printf("\n\n HIIIIIIIIIIIii \n\n");


			tempz = new INode(tempzz,*list);

//			printf("\n\n HI 3\n\n");

			(*list)->AddINode(tempz);			// Add INode into the List of INodes

			printf(" Created INode with number: %ld\n\n", (long)tempz->inode_number);

		}

//		printf("\n\n HIIIIIIIIIIIii \n\n");

//		printf("Hi2");

//		if( tempzz != NULL) printf("tempzz OK !!!!!!\n\n\n");
//		if( tempz != NULL) printf("tempz OK !!!!!!\n\n\n");

//		if(tempz->list != NULL) printf("tempz->list OK!!!!!!!!!!\n\n");
//		if(tempz->list == NULL) printf("tempz->list NOT OK!!!!!!!!!!\n\n");


		root = new TNode(tempzz,tempz,*list,NULL);	// created root Tree Node and connected with its inode

		tempz = NULL;		// this way, the destructor is not called
		tempzz = NULL;



	}else{		// recursion == true

//		char* tempzz = basename(path) ;

	//		tempz = new INode( tempzz );		// created the i-node of root of sub-tree 


		/* Get INode of file/directory and search if it exists */

//		if( stat(path,&stat1) == -1){

//			perror("Error in stat() in Tree::Creation (2) : ");
//			return;
//		}

}



	/* Checking its contents */
	DIR* dir;
	struct dirent *direntp;

	int length = 0;


	/* How many files/folders in current dir */

	if ( ( dir = opendir ( path ) ) == NULL ){

		perror("Cannot open dir with in Tree::Creation (1) : ");
		exit(1);

	}else{

		while ( ( direntp = readdir ( dir ) ) != NULL ){

			// IGNORE . and .. directory
			if( (strcmp(direntp->d_name,".") == 0) || (strcmp(direntp->d_name,"..") == 0) ) continue;

			length++;

		}

		closedir ( dir ) ;
	}




/* Sorting the names */

	// We first input the names(char*) in a char** array


	if( length != 0){

//	if( length > 2){

		char* array[length];

		int i = 0;
		int length1 = 0;

		if ( ( dir = opendir ( path ) ) == NULL ){

			perror("Cannot open dir in Tree::Creation (2) : ");
			exit(1);

		}else{

			while ( ( direntp = readdir ( dir ) ) != NULL ){

				// IGNORE . and .. directory
				if( (strcmp(direntp->d_name,".") == 0) || (strcmp(direntp->d_name,"..") == 0) ) continue;

				length1 = snprintf(NULL,0, "%s", direntp->d_name) + 1;

				array[i] = new char[length1];

				snprintf(array[i],length1,"%s", direntp->d_name);

				i++;

			}

			closedir ( dir ) ;
		}

		size_t str_len = sizeof(array) / sizeof(char *);

		qsort(array, str_len, sizeof(char *), str_cmp);





		printf(" The sorted array of root: %s is: ", path);

		for( i =0; i < length; i++)		printf(" %s	", array[i] );

		printf("\n\n\n");


		/* Creating the relative names */

		char *array1[length];		// array of pointers to strings
		int length2 = 0;


		if( recursion == false){

			for( i = 0; i < length; i++){

				length2 = snprintf(NULL,0,"%s/%s", basename(path) , array[i]) + 1;

				array1[i] = new char[length2];

				snprintf(array1[i], length2, "%s/%s", basename(path) , array[i]);


			}

		}else{

			for( i = 0; i < length; i++){

				length2 = snprintf(NULL,0,"%s/%s", path , array[i]) + 1;

				array1[i] = new char[length2];

				snprintf(array1[i], length2, "%s/%s", path , array[i]);

			}


		}



/* We start creating the children of root */

		printf(" Creating children of root %s\n\n", path);


		TNode *temp;
		Tree *temp1;

		for( i = 0; i < length; i++){

//			if( ( strcmp( array[i],".") == 0

//			tempz = new INode(array[i]);

			/* Search if the inode exists */

			if( stat(array1[i], &stat1) == -1){

				perror("Error in stat() in Tree::Creation (3) : ");
				return;
			}

			printf(" HELLO********* \n\n");


			(*list)->getINode(stat1.st_ino, &tempz);

//			printf(" FOUND NODE \n\n");

			



			if( tempz == NULL){

				printf("%s inode doesn't exist\n\n", array1[i]);

				tempz = new INode(array1[i],*list);

				(*list)->AddINode(tempz);			// Add tempz into the List of INodes

			}else{

				printf("%s inode exist with st_ino %ld \n\n", array1[i], (long) tempz->inode_number);



//				printf("\n\nProblem here (1)\n\n");

//				return;

				(*list)->Print();

			}


			printf("\n\n\n\n HI ******\n\n\n\n");


			if( tempz->mode == 'd'){		// this tree node is a directory

				temp1 = new Tree();		// create a new Tree

//				temp1->root = new TNode(array[i],tempz);

				temp1->root = new TNode(array1[i],tempz,*list, root->child);

				root->child->AddNode(temp1->root);	// link the new root as child of old root

				printf("\n\n\n\n HI ******\n\n\n\n");


//				temp1->Creation(array[i],list,true);		// do the procedure again

				temp1->Creation(array1[i],list,true);		// do the procedure again

//				root->child->AddNode(temp1->root);	// link the new root as child of old root



			}else{		// regular file

//				temp = new TNode(array[i], tempz);

				temp = new TNode(array1[i], tempz,*list, root->child);

				root->child->AddNode(temp);

			}

			tempz = NULL;
			temp = NULL;
			temp1 = NULL;

		}

		/* Cleaning memory */

	

		temp = NULL;
		tempz = NULL;
		temp1 = NULL;



		for( i = 0; i < length; i++)	delete[] array[i];
		for( i = 0; i < length; i++)	delete[] array1[i];



	}else{			// only . and .. was found

		if( recursion == false){

			printf(" Directory with name %s is empty\n", basename(path) ); 

		}else{


			printf(" Directory with name %s is empty\n", path); 

		}

	}

}






void Tree::Creation1(char* path, LINode** list,bool recursion = false){

	/* Creation of root */

	INode* tempz;
	struct stat stat1;

//	if( recursion == false){		//only do it the first time

//		INode* tempz = new INode(path);		// created the i-node of root of sub-tree 

//	char* tempzz = basename(path) ;

//		tempz = new INode( tempzz );		// created the i-node of root of sub-tree 


	/* Get INode of file/directory and search if it exists */

//	if( stat(tempzz,&stat1) == -1){

//		perror("Error in stat() in Tree::Creation : ");
//		return;
//	}


	if( recursion == false){		//only do it the first time

//		char* tempzz = basename(path);

//		tempz = new INode( tempzz );		// created the i-node of root of sub-tree 


		/* Get INode of file/directory and search if it exists */

		if( stat(path,&stat1) == -1){

			perror("Error in stat() in Tree::Creation (1) : ");
			return;
		}



		/* Search if it exists */

//		printf("\n\n HI \n\n");

		(*list)->getINode(stat1.st_ino, &tempz);

//		printf("tempz inode_number is : %ld\n\n", (long)tempz->inode_number);

//		printf("\n\n HI 2 \n\n");


		if( tempz == NULL){		// inode with this inode number , doesn't exist

//			printf("\n\n HIIIIIIIIIIIii \n\n");


			tempz = new INode(path,*list);

//			printf("\n\n HI 3\n\n");

			(*list)->AddINode(tempz);			// Add INode into the List of INodes

			printf(" Created INode with number: %ld\n\n", (long)tempz->inode_number);

		}

//		printf("\n\n HIIIIIIIIIIIii \n\n");

//		printf("Hi2");

//		if( tempzz != NULL) printf("tempzz OK !!!!!!\n\n\n");
//		if( tempz != NULL) printf("tempz OK !!!!!!\n\n\n");

//		if(tempz->list != NULL) printf("tempz->list OK!!!!!!!!!!\n\n");
//		if(tempz->list == NULL) printf("tempz->list NOT OK!!!!!!!!!!\n\n");


		root = new TNode(path,tempz,*list, NULL);	// created root Tree Node and connected with its inode

		tempz = NULL;		// this way, the destructor is not called
//		tempzz = NULL;



	}else{		// recursion == true

//		char* tempzz = basename(path) ;

	//		tempz = new INode( tempzz );		// created the i-node of root of sub-tree 


		/* Get INode of file/directory and search if it exists */

//		if( stat(path,&stat1) == -1){

//			perror("Error in stat() in Tree::Creation (2) : ");
//			return;
//		}

}



	/* Checking its contents */
	DIR* dir;
	struct dirent *direntp;

	int length = 0;


	/* How many files/folders in current dir */

	if ( ( dir = opendir ( path ) ) == NULL ){

		perror("Cannot open dir with in Tree::Creation (1) : ");
		exit(1);

	}else{

		while ( ( direntp = readdir ( dir ) ) != NULL ){

			// IGNORE . and .. directory
			if( (strcmp(direntp->d_name,".") == 0) || (strcmp(direntp->d_name,"..") == 0) ) continue;

			length++;

		}

		closedir ( dir ) ;
	}




/* Sorting the names */

	// We first input the names(char*) in a char** array


	if( length != 0){

//	if( length > 2){

		char* array[length];

		int i = 0;
		int length1 = 0;

		if ( ( dir = opendir ( path ) ) == NULL ){

			perror("Cannot open dir in Tree::Creation (2) : ");
			exit(1);

		}else{

			while ( ( direntp = readdir ( dir ) ) != NULL ){

				// IGNORE . and .. directory
				if( (strcmp(direntp->d_name,".") == 0) || (strcmp(direntp->d_name,"..") == 0) ) continue;

				length1 = snprintf(NULL,0, "%s", direntp->d_name) + 1;

				array[i] = new char[length1];

				snprintf(array[i],length1,"%s", direntp->d_name);

				i++;

			}

			closedir ( dir ) ;
		}

		size_t str_len = sizeof(array) / sizeof(char *);

		qsort(array, str_len, sizeof(char *), str_cmp);





		printf(" The sorted array of root: %s is: ", path);

		for( i =0; i < length; i++)		printf(" %s	", array[i] );

		printf("\n\n\n");


		/* Creating the relative names */

		char *array1[length];		// array of pointers to strings
		int length2 = 0;

/*
		if( recursion == false){

			for( i = 0; i < length; i++){

				length2 = snprintf(NULL,0,"%s/%s", basename(path) , array[i]) + 1;

				array1[i] = new char[length2];

				snprintf(array1[i], length2, "%s/%s", basename(path) , array[i]);


			}

		}else{
*/
			for( i = 0; i < length; i++){

				length2 = snprintf(NULL,0,"%s/%s", path , array[i]) + 1;

				array1[i] = new char[length2];

				snprintf(array1[i], length2, "%s/%s", path , array[i]);

			}


//		}



/* We start creating the children of root */

		printf(" Creating children of root %s\n\n", path);


		TNode *temp;
		Tree *temp1;

		for( i = 0; i < length; i++){

//			if( ( strcmp( array[i],".") == 0

//			tempz = new INode(array[i]);

			/* Search if the inode exists */

			if( stat(array1[i], &stat1) == -1){

				perror("Error in stat() in Tree::Creation (3) : ");
				return;
			}

			printf(" HELLO********* \n\n");


			(*list)->getINode(stat1.st_ino, &tempz);

//			printf(" FOUND NODE \n\n");

			



			if( tempz == NULL){

				printf("%s inode doesn't exist\n\n", array1[i]);

				tempz = new INode(array1[i], *list);

				(*list)->AddINode(tempz);			// Add tempz into the List of INodes

			}else{

				printf("%s inode exist with st_ino %ld \n\n", array1[i], (long) tempz->inode_number);


//				printf("\n\nProblem here (2)\n\n");

//				return;


				(*list)->Print();

			}


			printf("\n\n\n\n HI ******\n\n\n\n");


			if( tempz->mode == 'd'){		// this tree node is a directory

				temp1 = new Tree();		// create a new Tree

//				temp1->root = new TNode(array[i],tempz);

				temp1->root = new TNode(array1[i],tempz, *list, root->child);

				root->child->AddNode1(temp1->root);	// link the new root as child of old root

				printf("\n\n\n\n HI ******\n\n\n\n");


//				temp1->Creation(array[i],list,true);		// do the procedure again

			/* REMBER HERE*/

//				temp1->Creation(array1[i],list,true);		// do the procedure again

				temp1->Creation1(array1[i],list,true);		// do the procedure again

//				root->child->AddNode(temp1->root);	// link the new root as child of old root



			}else{		// regular file

//				temp = new TNode(array[i], tempz);

				temp = new TNode(array1[i], tempz, *list, root->child);

				root->child->AddNode1(temp);

			}

			tempz = NULL;
			temp = NULL;
			temp1 = NULL;

		}

		/* Cleaning memory */

	

		temp = NULL;
		tempz = NULL;
		temp1 = NULL;



		for( i = 0; i < length; i++)	delete[] array[i];
		for( i = 0; i < length; i++)	delete[] array1[i];



	}else{			// only . and .. was found

		printf(" Directory with name %s is empty\n", path); 

	}

}




/////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////





void Tree::Print(){

	if( root != NULL){

		printf(" %s	", root->name);

		TLNode* temp = root->child->head;

		Tree * temp1;

		TList* list = new TList();

//		printf( "PROBLEM IN TREE PRINT (1) \n\n");

//		return;


		while( temp != NULL){		// print children

			printf(" %s	", temp->tnode->name );

			if( temp->tnode->inode->mode == 'd'){	// if it is a directory

				list->AddNode(temp->tnode);


//				printf("\n\n");

//				temp1 = (Tree *) temp;
//				temp1->Print();

			}

			temp = temp->next;

		}

		printf("\n\n");

		/* Redo the procedure for the (child) directories */

		temp = list->head;


//		printf( "PROBLEM IN TREE PRINT (2) \n\n");

//		return;




		while( temp != NULL){

			temp1 = (Tree *) temp;
			temp1->Print();

			temp1 = NULL;
			temp = temp->next;
		}

		/* Finished the procedure , NULL-ing tnode-field and destroying list*/

//		printf( "PROBLEM IN TREE PRINT (3) \n\n");



		// we don't want to accidentally destroy the tnodes

		temp = list->head;

		while( temp != NULL){

			temp->tnode = NULL;
			temp = temp->next;

		}

		delete list;
	}

}


/////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////



	/*Helps during the synchronisation process */

bool Tree::Exists(char* name1,char mode,bool source){//mode = what we expected the tnode to be(file or dir)


	printf("Searching if EXISTS() in ROOT: %s (used by %d , 1=source , 0=backup) NAME: %s which is (f/d) %c \n\n", root->name, source, name1, mode);


	/* Check if name exists, in this tree */

	TLNode* temp = root->child->head;

	while( temp != NULL){


		if( strcmp(temp->tnode->name,name1) == 0)	break;	// we found the tnode

		temp = temp->next;
	}

	if( temp == NULL){

		printf(" Didn't find : %s in root: %s \n\n", name1, root->name);


		return false;		// we didn't find it


	}


	if( source == false){	// backup is using it

	// By passing the if statement (temp == NULL) means that we have found 
	// that this tnode exists in source ==> return true

		printf(" TNode : %s exists in root: %s \n\n", name1, root->name);


		return true;

	}







	/* From here on , only source-related */


	/* Now we check some extra parameters */

	if( mode == 'd'){		// dir-part of the synchronisation (2.a,2.b)


		if( temp->tnode->inode->mode == 'f' ){	// name exists but is a file

			unlink(temp->tnode->name);	// unlink file

//			printf("\n %s name was found, but it was a file \n\n",temp->tnode->name);

			root->child->deleteNode(temp->tnode);	// remove temp->tnode from root's children

			printf("\n %s name was found, but it was a file => return false \n\n",name1);


			return false;	//not found

		}

		printf(" Directory : %s EXISTS in BACKUP !!! ==> TRUE\n\n", name1);

		return true;		//else we found it

	}


	if( mode == 'f'){		// file-part of the synchronisation (2.c,2.d,2.e)


		if( temp->tnode->inode->mode == 'd' ){	// name exists but is a dir

			Removedir(temp->tnode->name);	// remove dir

//			printf("\n %s name was found, but it was a file \n\n",temp->tnode->name);

			root->child->deleteNode(temp->tnode);	// remove temp->tnode from root's children

			printf("\n %s name was found, but it was a dir \n\n",name1);


			return false;	//not found

		}

		printf(" FILE : %s EXISTS in BACKUP !!! ==> TRUE\n\n", name1);

		return true;		//else we found it

	}

}





























/////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////










//void Tree::Synchronisation(Tree** backupT, LINode** backupL, LINode** sourceL ){

void Tree::Synchronisation(Tree** backupT, LINode** backupL ){

	/* DFS in this source tree*/

	/* Every time we encounter a directory => we go deeper */ 

	TLNode* temp;

	temp = root->child->head;



	while( temp != NULL){	// for all children nodes of this root


		printf(" %s  Synchronisation() (source) \n\n", temp->tnode->name);



		/************************** DIRECTORY *********************************/

		if( temp->tnode->inode->mode == 'd'){		// if this tnode is a directory

			/* basically we want to search if dir-name in source, exists in backup */

			/* But first we have to make the dir name  be "apropriate" for backup */

			int length1=snprintf(NULL,0,"%s/%s",(*backupT)->root->name,basename(temp->tnode->name) ) + 1;

			char name1[length1];

			snprintf(name1,length1,"%s/%s",(*backupT)->root->name,basename(temp->tnode->name));






				/******************* 2.A ***************************/

			if( ( (*backupT)->Exists( name1, 'd', true ) ) == false){

/* IMPORTANT*********** Also, the above checks if the name exists , but is a FILE and unlink() 							the name (and deleting the TNode), before returning false */



			/* If the directory name, doesn't exist => copy it => Copy1() */

				copy1(temp->tnode->name, (*backupT)->root->name,'d',false);
				// copy the dir(temp->tnode->name) to the backup dir(backup->root->name)


				/* Now we must update the  tree as necessary */

				/* basically we want to create backup/../.../dir, where dir is the basename of 					the folder we want to transport */


				Tree *temp1 = new Tree();

				// Create the new sub-tree starting from directory name1
				temp1->Creation1(name1,backupL,false);


// the only difference (compared to AddNode() ) is that is uses strcmp, in order to correctly place the child , between its siblings (keep the alphabetical order )
				(*backupT)->root->child->AddNode1(temp1->root);


				printf("Printing Backup Tree and List \n\n");

				(*backupT)->Print();
				(*backupL)->Print();



				temp1 = NULL;



/* TO DO : 1) Tree->Exists() 2) TNode->AddNode1() 3)Creation1() 4) Tree->Synchronisation1() ***/

		/* Creation1() DONE =====> NOT TESTED */
		/* AddNode1() DONE =====> NOT TESTED */
		/* Synchronisation1() DONE =====> NOT TESTED */


			}else{		// returned TRUE, if there exists a directory with the same name


/* Before diving deeper into directory, check if there are extra entries in Backup Tree 
	( which dont appear in source) => Delete these entries */


/* Searching from start (child* TLNode) untill directory (child* TLNode) */


/* After deleting these entries , return Tree* Pointer






			/* Now Check BackupTree ==> Synchronisation1() */


				Tree *temp1;	// the future backup Tree

				Tree *temp3 = this;		//pass this source tree into Synchronisation1()


				/****************** 2.B *****************/




				(*backupT)->Synchronisation1( &(temp3), &temp1, name1,false);


				printf("The new BACKUP TREE after Synchronisation1() %s\n\n", temp1->root->name);



			/* Now Synchronisation1 has guaranteed that, up to temp->tnode->name,
				the 2 trees are the same (deleted "extrea" entries in backup ) */

			/* Also temp1 now shows, to the copy of this directory in backup Tree */

		/* This way we can compare the contents of these directories */


//				Tree *temp2 = (Tree*) temp->tnode;

				Tree *temp2 = (Tree*) temp;


//				printf("*********** %s ********* \n\n",temp->tnode->name);

//				printf("*********** %s ********* \n\n",temp->tnode->name);

			/* Diving Deeper into temp2->name folder */


				temp2->Synchronisation(&temp1,backupL);
				// Redo the procedure

				temp1 = NULL;
				temp2 = NULL;

				temp3 = NULL;


			}

		}










		/**************** FIX SYNCHRONISATION1() for FILES ********/

		/********************** FILE ******************************************/


		if( temp->tnode->inode->mode == 'f'){		// if this tnode is a file

			/* basically we want to search if file-name in source, exists in backup */

			/* But first we have to make the file name  be "apropriate" for backup */

			int length1=snprintf(NULL,0,"%s/%s",(*backupT)->root->name,basename(temp->tnode->name) ) + 1;

			char name1[length1];

			snprintf(name1,length1,"%s/%s",(*backupT)->root->name,basename(temp->tnode->name));



				/******************** 2.C *****************/


			if( ( (*backupT)->Exists( name1, 'f', true ) ) == false){


				printf("FILE: %s doesn't exist in Backup ROOT: %s \n\n", name1, (*backupT)->root->name); 


				/*********************** 2.C1 ******************/

				/* Check if there exists, the "copy inode" of tnode */

				if( temp->tnode->inode->copy != NULL){

//				printf("FILE (in SOURCE): %s (with inode_number: %ld) has a COPY INODE in BACKUP ROOT: %s with inode_number %ld \n\n", name1, temp->tnode->inode->inode_number, (*backupT)->root->name, (long)temp->tnode->inode->copy->inode_number);


				printf("FILE (in SOURCE): %s (with inode_number: %ld) has a COPY INODE in BACKUP ROOT: %s with inode_number %ld \n\n", temp->tnode->name, temp->tnode->inode->inode_number, (*backupT)->root->name, (long)temp->tnode->inode->copy->inode_number);



//basically get the head of the list of names pointing to this (copy) Inode
					TLNode* temp1 = temp->tnode->inode->copy->Tlist->head;

					if( temp1 == NULL){

						printf(" Head of Copy INode of %s is NULL ???? (Synchronisation) (1) \n", temp->tnode->name);

						return;
					}


/* Link the file-name(name1) with the existent inode ==> link with another name to this inode */

					printf("\n\n\n\n LINKING(hard link) ln: %s with(new name) %s \n\n\n\n",temp1->tnode->name , name1);

					if( link( temp1->tnode->name , name1) == -1){

						perror(" Link() in Synchronisation (1) : ");
						return;

					}


		/* Also Create a new TNode for this "hard link" file,
			while connecting the inode* pointer to the existant temp->tnode->inode->copy

 */ 

					TNode* tempz=new TNode(name1,temp->tnode->inode->copy, *backupL,(*backupT)->root->child);

					tempz->inode->Tlist->AddNode1(tempz);

					(*backupT)->root->child->AddNode1(tempz);

					tempz = NULL;


					/************ CHECK AGAIN **********/




				/************************* 2.C2 ********************************/

				}else{	// Copy INode doesn't exist 

					/* Copy the file into  */

					copy1(temp->tnode->name, (*backupT)->root->name,'f',false);

					INode* temp1 = new INode(name1,*backupL);
					(*backupL)->AddINode(temp1);

					TNode* temp2 = new TNode(name1, temp1, *backupL, root->child);

					(*backupT)->root->child->AddNode1(temp2);// keep the order ( with AddNode1)


					/* Also coonect the INodes */

					temp->tnode->inode->copy = temp1;



					temp1 = NULL;
					temp2 = NULL;


				}


				/************************* 2.D ********************************/

				/* Check for extra entries in Backup Tree (until name1 Tnode) ===> DELETE them */

				Tree *temp1;

				Tree *temp2 = this;


				(*backupT)->Synchronisation1( &(temp2), &temp1, name1, false);



				temp1 = NULL;
				temp2 = NULL;


				/************************* 2.E ********************************/

			}else{		// returned TRUE, if there exists a file with the same name

				/* Exists() checks (and deletes) if that name is dir */


				if( temp->tnode->inode->copy == NULL){


					printf(" tnode->name = %s\n\n", temp->tnode->name);

					printf(" BAD BAD BAD \n\n");

				}


				/* find the tnode (in backup) with same inode */

				TLNode* temp1 = temp->tnode->inode->copy->Tlist->head;


				while( temp1 != NULL){

/* Search all the names pointing to (copy) INode, and find the one where 
				temp1->tnode->name -----(transformed)--> name2  == temp->tnode->name
*/



					printf(" Temp1 is %s\n\n", temp1->tnode->name);
					printf(" Backup root right now is : %s\n\n",(*backupT)->root->name);



			/* we want to make temp1->tnode->name appropriate for our search */

					int length = strlen( (*backupT)->root->name ) + 1;// +1 for the '/' character

					char *tempz = (temp1->tnode->name) + length;

					length = snprintf(NULL,0,"%s/%s", root->name, tempz ) + 1;

					char name2[length];

					snprintf(name2,length,"%s/%s", root->name, tempz );


					printf("tempz is %s \n\n",tempz);


					tempz = NULL;


					printf(" NAME2 is %s, temp->tnode->name %s \n\n", name2,temp->tnode->name);

				/* if starting tnode->name = source/dir1/dir2

					tempz = dir1/dir2

					name2 = backup/dir1/dir2

				*/




//					if( strcmp( temp1->tnode->name, temp->tnode->name) == 0) break;

					if( strcmp( name2, temp->tnode->name) == 0){

						printf("Found them... name2: %s , temp->tnode->name: %s\n\n",name2,temp->tnode->name);

						break;

					}

					temp1 = temp1->next;

				}


				if( temp1 == NULL){

					printf("Something went wrong in Synchronise (1) \n\n");
					return;

				}


				/* Compare the contents of inode, of the two(source and backup) 

					source == temp , backup == temp1 					*/

					/* Last modification && size */

				if( (temp->tnode->inode->last_mod != temp1->tnode->inode->last_mod) || (temp->tnode->inode->size != temp1->tnode->inode->size) ){

					printf(" %s and %s have different last_mod_time or size ==> unlink %s\n\n", temp->tnode->name, temp1->tnode->name, temp1->tnode->name); 



				/* If one of these are different, unlink the file-name */

					unlink(temp1->tnode->name);		// unlink file-name in backup

					(*backupT)->root->child->deleteNode(temp1->tnode);	// delete from children


				/****************** 2.C AGAIN *****************************/

					printf(" Repeat 2.C PROCESS \n\n");

					/* Check if there is copy of the inode */

					if( temp->tnode->inode->copy != NULL){

				/****************** 2.C1 AGAIN ****************************/



//basically get the head of the list of names pointing to this (copy) Inode
						TLNode* temp1 = temp->tnode->inode->copy->Tlist->head;

						if( temp1 == NULL){

							printf("Head of Copy INode of %s is NULL ??? (Synchronisation)(2)\n", temp->tnode->name);

							return;
						}


/* Link the file-name(name1) with the existent inode ==> link with another name to this inode */

					printf("\n\n\n\n AGAIN Section C1 : \n");
					printf("LINKING(hard link) ln: %s with(new name) %s \n\n\n\n",temp1->tnode->name , name1);


						if( link( temp1->tnode->name , name1) == -1){

							perror(" Link() in Synchronisation (2) : ");
							return;

						}







				/************************* 2.C2 AGAIN ********************************/

					}else{	// Copy INode doesn't exist 


						printf("\n\n\n\n AGAIN Section C2\n\n\n");

						/* Copy the file into  */

						copy1(temp->tnode->name, (*backupT)->root->name,'f',false);

						INode* temp1 = new INode(name1,*backupL);

						TNode* temp2 = new TNode(name1, temp1, *backupL, root->child);

						(*backupT)->root->child->AddNode1(temp2);
							// keep the order ( with AddNode1)

						temp1 = NULL;
						temp2 = NULL;


					}


					/************************* 2.D AGAIN ********************************/

				/* Check for extra entries in Backup Tree (until name1 Tnode) ===> DELETE them *

					Tree *temp1;

					Tree* temp2 = this;

					(*backupT)->Synchronisation1( &(temp2) , &temp1, name1,false);


					temp1 = NULL;
					temp2 = NULL;
				*/

				}

					/************************* 2.D AGAIN ********************************/

				/* Check for extra entries in Backup Tree (until name1 Tnode) ===> DELETE them */

					Tree *tempz1;

					Tree* tempz2 = this;

					(*backupT)->Synchronisation1( &(tempz2) , &tempz1, name1,false);


					tempz1 = NULL;
					tempz2 = NULL;



			}


//			temp = temp->next;

		}

			temp = temp->next;

	}


	/* Searched all the children */


	/* The two trees are identical, up to the last node(tail) of source */

	/* However there might exist "extra entries" after the "tail" */

	/* So we call Synchronisation1() one last time, by deleting the rest of the enttries */


	Tree* temp1;

//	TLNode* temp2 = root->child->tail;

	TLNode* temp2 = root->child->head;


	if( temp2 == NULL){

		printf(" Head is NULL ==> won't do LAST SYNCHRONISATION1(), because there are no children in directory %s \n\n", root->name);

		temp1 = NULL;
		return;

	}



	int length1=snprintf(NULL,0,"%s/%s",(*backupT)->root->name,basename(temp2->tnode->name) )+ 1;

	char name1[length1];
	snprintf(name1,length1,"%s/%s",(*backupT)->root->name,basename(temp2->tnode->name));





	Tree *temp3 = this;

	(*backupT)->Synchronisation1( &(temp3), &temp1,name1,true);

	temp1 = NULL;
	temp2 = NULL;
	temp3 = NULL;


}




























	/************************** FIX SYNCHRONISATION1 FOR FILES ==> 1)TREE* not needed
		2) char mode needed? 3) bool last implementation (haven't written */ 



void Tree::Synchronisation1(Tree** sourceT, Tree** tempT, char* name2 , bool last){

	/* Backup tree*/

	TLNode* temp;

	TLNode* temp1;

	temp = root->child->head;

	printf(" %s  Synchronisation1() (backup) (1) \n\n", temp->tnode->name);



//	printf("Before the LAST IF \n\n");









			/********************* LAST CALL **********************/


	if( last == true){		// last call of synchronisation1()

		// clear the children to the left of temp Tnode => these are extra entries 

		TLNode* temp2 = temp;
		TLNode* temp3;

	printf("INSIDE the LAST IF \n\n");



		while( temp2 != NULL){


			/************************** FILE **************************/

			if( temp2->tnode->inode->mode == 'f'){		// if this tnode is a file

				/* basically we want to search if file-name in backup, exists in source */

				/* But first we have to make the file-name  be "apropriate" for source */

				int length1=snprintf(NULL,0,"%s/%s",(*sourceT)->root->name,basename(temp2->tnode->name) ) + 1;

				char name1[length1];

				snprintf(name1,length1,"%s/%s",(*sourceT)->root->name,basename(temp2->tnode->name));

//					printf(" HIIIIIIII (1) \n\n");

				if( ( (*sourceT)->Exists( name1, 'f', false ) ) == false){


//					printf(" HIIIIIIII (2) \n\n");

					printf("\n\n LAST IF => FILE: %s DELETING \n\n", name1);


			/* If the file name, doesn't exist in source => Delete this File (in backup) */

					Removedir( temp2->tnode->name);		// does the same thing for files ( rm -r)


					/* Now delete the sub-file (directory) */

					temp3 = temp2;

					temp2 = temp2->next;

					root->child->deleteNode(temp3->tnode);

					temp3 = NULL;

					continue;


				}

				temp2 = temp2->next;

				continue;

			}

		/******************* DIRECTORY ***************************/
 

		if( temp2->tnode->inode->mode == 'd'){		// if this tnode is a directory

			/* basically we want to search if dir-name in backup, exists in source */

			/* But first we have to make the dir-name  be "apropriate" for source */

			int length1=snprintf(NULL,0,"%s/%s",(*sourceT)->root->name,basename(temp2->tnode->name) ) + 1;

			char name1[length1];

			snprintf(name1,length1,"%s/%s",(*sourceT)->root->name,basename(temp2->tnode->name));

//			printf(" HIIIIIIII (3) \n\n");

			if( ( (*sourceT)->Exists( name1, 'd', false ) ) == false){

//				printf(" HIIIIIIII (4) \n\n");

				printf("\n\n LAST IF => DIRECTORY: %s DELETING \n\n", name1);


		/* If the directory name, doesn't exist in source => Delete this Directory (in backup) */

				Removedir( temp2->tnode->name);


				/* Now delete the sub-tree (directory) */

				temp3 = temp2;

				temp2 = temp2->next;

				root->child->deleteNode(temp3->tnode);

				temp3 = NULL;

				continue;

			}


			temp2 = temp2->next;

			continue;


		}


/*

			// Remove file/directories after temp
			Removedir( temp2->tnode->name);		// does the same thing for files ( rm -r)


			temp3 = temp2->tnode;
			temp2 = temp2->next;

			//Remove these TNodes from children of root
			root->child->deleteNode(temp3);

*/

		}

		*tempT = NULL;
		temp2 = NULL;
		temp3 = NULL;



		return;

	}
































	printf( "Searching from %s  to -> %s \n\n", temp->tnode->name, name2);


	while( (temp != NULL) && ( strcmp(temp->tnode->name,name2) != 0 ) ){
	// searching for "extra" entries in backup, and untill we encounter our name-dir


		printf(" %s  Synchronisation1() (backup) (2) \n\n", temp->tnode->name);



		/******************* DIRECTORY ***************************/
 

		if( temp->tnode->inode->mode == 'd'){		// if this tnode is a directory

			/* basically we want to search if dir-name in backup, exists in source */

			/* But first we have to make the dir-name  be "apropriate" for source */

			int length1=snprintf(NULL,0,"%s/%s",(*sourceT)->root->name,basename(temp->tnode->name) ) + 1;

			char name1[length1];

			snprintf(name1,length1,"%s/%s",(*sourceT)->root->name,basename(temp->tnode->name));



			if( ( (*sourceT)->Exists( name1, 'd', false ) ) == false){


		/* If the directory name, doesn't exist in source => Delete this Directory (in backup) */

				Removedir( temp->tnode->name);


				/* Now delete the sub-tree (directory) */

				temp1 = temp;

				temp = temp->next;

				root->child->deleteNode(temp1->tnode);

				temp1 = NULL;


				continue;

			}

		}


		/********************** FILE  *******************************/


		if( temp->tnode->inode->mode == 'f'){		// if this tnode is a file

			/* basically we want to search if file-name in backup, exists in source */

			/* But first we have to make the file-name  be "apropriate" for source */

			int length1=snprintf(NULL,0,"%s/%s",(*sourceT)->root->name,basename(temp->tnode->name) ) + 1;

			char name1[length1];

			snprintf(name1,length1,"%s/%s",(*sourceT)->root->name,basename(temp->tnode->name));



			if( ( (*sourceT)->Exists( name1, 'f', false ) ) == false){


		/* If the file name, doesn't exist in source => Delete this File (in backup) */

				Removedir( temp->tnode->name);		// does the same thing for files ( rm -r)


				/* Now delete the sub-file (directory) */

				temp1 = temp;

				temp = temp->next;

				root->child->deleteNode(temp1->tnode);

				temp1 = NULL;

				continue;

			}

		}



		temp = temp->next;

	}


	if( temp == NULL){

		printf(" The file/dir %s (which exists source) \
				 wasn't found during our search in backup =====> SOMETHINGS WRONG \
					SYNCRONISATION1 (1) \n\n",name2);

		return;

	}

	// temp != NULL


//	printf("Before the LAST IF \n\n");



			/********************* LAST CALL **********************


	if( last == true){		// last call of synchronisation1()

		// clear the children to the left of temp Tnode => these are extra entries 

		TLNode* temp2 = temp->next;
		TLNode* temp3;

 
		while( temp2 != NULL){


			/************************** FILE **************************

			if( temp2->tnode->inode->mode == 'f'){		// if this tnode is a file

				/* basically we want to search if file-name in backup, exists in source 

				/* But first we have to make the file-name  be "apropriate" for source 

				int length1=snprintf(NULL,0,"%s/%s",(*sourceT)->root->name,basename(temp2->tnode->name) ) + 1;

				char name1[length1];

				snprintf(name1,length1,"%s/%s",(*sourceT)->root->name,basename(temp2->tnode->name));

					printf(" HIIIIIIII (1) \n\n");

				if( ( (*sourceT)->Exists( name1, 'f', false ) ) == false){


					printf(" HIIIIIIII (2) \n\n");


			/* If the file name, doesn't exist in source => Delete this File (in backup) 

					Removedir( temp2->tnode->name);		// does the same thing for files ( rm -r)


					/* Now delete the sub-file (directory) 

					temp3 = temp2;

					temp2 = temp2->next;

					root->child->deleteNode(temp3->tnode);

					temp3 = NULL;

					continue;


				}

				temp2 = temp2->next;



			}


		/******************* DIRECTORY **************************
 

		if( temp->tnode->inode->mode == 'd'){		// if this tnode is a directory

			/* basically we want to search if dir-name in backup, exists in source */

			/* But first we have to make the dir-name  be "apropriate" for source 

			int length1=snprintf(NULL,0,"%s/%s",(*sourceT)->root->name,basename(temp2->tnode->name) ) + 1;

			char name1[length1];

			snprintf(name1,length1,"%s/%s",(*sourceT)->root->name,basename(temp2->tnode->name));

			printf(" HIIIIIIII (3) \n\n");

			if( ( (*sourceT)->Exists( name1, 'd', false ) ) == false){

			printf(" HIIIIIIII (4) \n\n");

		/* If the directory name, doesn't exist in source => Delete this Directory (in backup) 

				Removedir( temp2->tnode->name);


				/* Now delete the sub-tree (directory) 

				temp3 = temp2;

				temp2 = temp2->next;

				root->child->deleteNode(temp3->tnode);

				temp3 = NULL;

				continue;

			}


			temp2 = temp2->next;


		}

*/














/*

			// Remove file/directories after temp
			Removedir( temp2->tnode->name);		// does the same thing for files ( rm -r)


			temp3 = temp2->tnode;
			temp2 = temp2->next;

			//Remove these TNodes from children of root
			root->child->deleteNode(temp3);



		}

		*tempT = NULL;
		temp2 = NULL;
		temp3 = NULL;



		return;

	}
*/





	/* Since we found the name2-direcotory/file, return a tree */


	if( temp->tnode->inode->mode == 'd'){		// only if it is a directory

		*tempT = (Tree *) temp;

	}else{ // if it is a file 

//		*tempT = NULL;

	}

	temp = NULL;


}








void Tree::RemoveWD(int fd, List** list, bool recursive = false ){		// inotify remove watch descriptors recursively

	TLNode* temp = root->child->head;

	Tree* temp1;

	while( temp != NULL){

		if( temp->tnode->inode->mode == 'd'){	// temp is a dir

			temp1 = (Tree*) temp;
			temp1->RemoveWD(fd,list,true);		// recursive call
		}

		temp = temp->next;

	}
		// temp = NULL

	temp1 = NULL;



	/* Search for the watch descriptor */

	temp1 = this;

	temp = (TLNode*) temp1;

	int wd1;

	Node* temp2 = (*list)->head;


	while( temp2 != NULL){

		//if there is a Node with Node->tnode == this tree node (TNode)
		if( temp2->tnode == temp->tnode) break;

		temp2 = temp2->next;
	}

	if( temp2 == NULL){

		printf("\n\n Something is wrong in Tree::RemoveWD, temp2 != NULL \n\n");
		return;

	}

	wd1 = temp2->wd;

//	printf(" wd of temp2 is %d \n\n", wd1);


	printf(" Found our Node in Tree::RemoveWD() \n\n");

	if( inotify_rm_watch(fd, wd1) == -1){

		perror(" Error inotify_rm_watch in Tree::RemoveWD: ");
		printf("\n\n");

	}

	/* Finally delete the Node */

	//
	if( recursive == true)	(*list)->deleteNode(temp2);


	temp2 = NULL;
	temp1 = NULL;
	temp = NULL;


}

























