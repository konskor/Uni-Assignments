#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/inotify.h>

#include <sys/stat.h>

#include <time.h>

//#include "NameTree.hpp"
//#include "Inode.hpp"

#include "Mystructs.hpp"


/* INCOMPLETE, CHECK AGAIN  *******************************/



INode::INode( char* path, LINode* list1){

	struct stat temp;

//	printf("Hi from inode1");

	if( stat(path,&temp)  == -1){

		perror("Stat() in INode constructor: ");
		exit(1);

	}

//	printf("Hi from inode");


	inode_number = temp.st_ino;		// inode number


	/* We only care about regular files and directories */

	switch( temp.st_mode & S_IFMT){

		case S_IFDIR:	mode = 'd';		// if directory
						break;

		case S_IFREG:	mode = 'f';		//if regural file
						break;

		default:	printf("Don't care about this type of file \n");
					break;
	}

	last_mod = temp.st_mtime;		// last modified time
	size = temp.st_size;		//size

	Tlist = new TList();

	name_counter = 0;




		/* THIS WAS CHANGED */
	list = list1;			// pointer to list of INodes


//	list1->AddINode(this);	// add this inode to the list


	copy = NULL;

	next = NULL;

}






INode::~INode(){

	delete Tlist;			//which most likely will be NULL 

//	printf("Hello in destructor INode (1) \n\n");

	list->Update(this);		// update the list of INodes , as this INode will not be accessible

//	printf("Hello in destructor INode (2) \n\n");

	next = NULL;



	/* Don't delete the copy from here */

	/* Deleting the copy */

//	if( copy != NULL) delete copy;


/*

	if( copy != NULL){			// we are source and a copy exists

		//delete copy function

		TLNode* temp;
//		TNode* temp1;

		temp = copy->Tlist->head;	// head of the tree node list which point to this (copy) inode

		while( copy->Tlist->head != NULL){

			copy->Tlist->head = copy->Tlist->head->next;


			temp->tnode->inode = NULL;	// we NULL this, to avoid circular delete 

			delete temp;		// delete TLNode

			temp = copy->Tlist->head;

	

	}

*/


		/* REMEMBER TO DELETE THE INODE FROM THE COPY LIST */
		/* DID IT 


	}
*/


	/* Don't delete the copy from here */

	copy = NULL;


//	printf("Hello in destructor INode (3) \n\n");


}



INode* INode::getCopy(){

	return copy;
}














/************************************** LINode ( List of Inodes) *************************/

LINode::LINode(){

	head = NULL;
	tail = NULL;
}


LINode::~LINode(){

	INode* temp = head;

	while(head != NULL){

		head = head->next;

		delete temp;
		temp = head;

	}

	head = NULL;
	tail = NULL;
	temp = NULL;

}














void LINode::AddINode(INode* inode1){

	// Add TNode to the end of the list

	if( head ==NULL){

		head = inode1;
		tail = head;

		return;

	}

	// else,

	tail->next = inode1;
	tail = inode1;

}


///////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
////////////////////////////////////////////////////////


void LINode::deleteINode(INode* inode1){

	//remove TNode from list

	if(head != NULL){

		INode* temp;


		/*TNode is head */

		if( head == inode1){

			temp = head;
			head = head->next;
			delete temp;

			return;

		}

		/* INode is tail */
		if( tail == temp){

			temp = head;

			while(temp->next != tail) temp = temp->next;	// get to TNode before tail

			tail = temp;
			temp = temp->next;		// last TNode

			delete temp;
			tail->next = NULL;

			return;

		}


		/* INode is between head and tail */

		temp = head;

		INode* temp1;

		while( (temp != NULL) && ( temp->next != inode1) ) temp = temp->next;

		if( temp == NULL){

			printf(" TNode doesn't exist, cannot delete\n");
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


void LINode::Update(INode* inode1){

/* Same as DeleteNode, but this one doesn't delete the inode (since destructor is already called)
==> nulls the pointer */

	if(head != NULL){

		INode* temp;


		/*TNode is head */

		if( head == inode1){

//			temp = head;
			head = head->next;

//			delete temp;

			return;

		}

		/* INode is tail */
		if( tail == temp){

			temp = head;

			while(temp->next != tail) temp = temp->next;	// get to TNode before tail

			tail = temp;
			temp = temp->next;		// last TNode

//			delete temp;
			tail->next = NULL;
			temp = NULL;

			return;

		}


		/* INode is between head and tail */

		temp = head;

		INode* temp1;

		while( (temp != NULL) && ( temp->next != inode1) ) temp = temp->next;

		if( temp == NULL){

			printf(" TNode doesn't exist, cannot update\n");
			return;
		}

		temp1 = temp->next;
		temp->next = temp1->next;

//		delete temp1;

		temp = NULL;
		temp1 = NULL;

		return;

	}

}














////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////


void LINode::Print(){

	INode* temp = head;

	if( head == NULL){

		 printf("List of INodes is empty \n");

	}

	while( temp != NULL){

		printf("\n INode:%ld has mode: %c , last modified: %s, size: %ld \
	name_counter: %d\n", (long) temp->inode_number, temp->mode, \
	ctime(&(temp->last_mod)) , (long) temp->size, temp->name_counter);

		printf("The names that are connected to this i-node are:\n");

		temp->Tlist->Print();

		printf("\n\n");

		temp = temp->next;

	}

}















void LINode::getINode(ino_t number, INode** result){

	printf(" Inside getINode() (1) \n\n");

	INode* temp = head;

	if( temp == NULL){

		*result = NULL;
		return;
	}


	while( temp != NULL){

		if( temp->inode_number == number){		// found the inode

			printf(" Found INode (ino_t) \n\n");

			*result = temp;
			break;

		}

		temp = temp->next;

	}

	printf("Passed the while loop in getINode (1) \n\n");


	if(temp == NULL)	*result = NULL;		//didn't find what we wanted

	temp = NULL;

}



void LINode::getINode(char *name1, INode** result){

	printf(" Inside getINode() (2) \n\n");

	INode* temp = head;

	if( temp == NULL){

		*result = NULL;
		return;
	}


	TLNode* temp1 = NULL;

	while( temp != NULL){

		temp1 = temp->Tlist->head;

		while(temp1 != NULL){		// search the TNodes-names which point to this INode 

			if( strcmp( temp1->tnode->name, name1) == 0){		// this INode->name = name1;

				printf(" Found our INode (char*) \n\n");

				*result = temp;

				temp = NULL;
				temp1 = NULL;

				return;
			}

			temp1 = temp1->next;

		}


		temp = temp->next;

	}

	printf("Passed the while loop in getINode (2) \n\n");

	if(temp == NULL)	*result = NULL;		//didn't find what we wanted


	temp = NULL;

}
































void LINode::CopyPointers(LINode** backup,char* source, char* sbackup){

/* We want to fix the copy* pointers of the INodes */

	INode* temp = head;

	if( head == NULL){
		printf("Cannot fix copy pointers, source list is empty\n\n");
		return;
	}

	INode* temp1 = NULL;

	TLNode* temp2 = NULL;

	char* tname = NULL;


	/* Connect this Inode( source root) with the backup Inode (backup root) */

	temp->copy =  (*backup)->head;

	temp = temp->next;


	while( temp != NULL){

		/* Search if a copy INode exists and "link" it */

		/* Searching the list of names in this INode, and see if any of these names are contained in another(copy) INode. If yes link this INode with the "Copy INode" */




		temp2 = temp->Tlist->head;

		while( temp2 != NULL){	//search all the names of this INode

//			tname = temp2->tnode->name;


		/* we want to make temp2->tnode->name appropriate for our search */

			int length = strlen(source) + 1;	// +1 for the '/' charactet

			char *tempz = (temp2->tnode->name) + length;

			length = snprintf(NULL,0,"%s/%s", sbackup, tempz ) + 1;

			char name2[length];

			snprintf(name2,length,"%s/%s", sbackup, tempz );

			tempz = NULL;

		/* if starting tnode->name = source/dir1/dir2

			tempz = dir1/dir2

			name2 = backup/dir1/dir2

		*/



			(*backup)->getINode(name2,&temp1);	// does any (copy) INode contain this name?

			if( temp1 != NULL){

				printf(" Source: %s, has Backup INode: %ld\n\n", temp2->tnode->name, temp1->inode_number);

				break;

			}

			temp2 = temp2->next;

		}

		if( temp2 == NULL){		//this INode has no copy, search the next one

			temp->copy = NULL;

//			printf(" %s has no COPY INode \n\n", temp->tnode->name);

			temp = temp->next;
			continue;
		}


		temp->copy = temp1;			// either NULL, or address to the copy INode

		temp = temp->next;

		temp1 = NULL;
		temp2 = NULL;

//		printf(" Connected %s (in SOURCE) with %s in Backup \n\n", temp->tnode-


	}

	temp = NULL;
	temp1 = NULL;
	temp2 = NULL;

}













