//#ifndef INODE_H
//#define INODE_H


#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/inotify.h>

//#include "NameTree.hpp"







class INode{				// Nodes of LIST of Inodes ( LIST instead of matrix )

public:

	/* Variables */

//	struct inode;

//	struct stat inode;			// information about file
	//might only include inode number(unique)


//	char* pathname;			// name of file/directory



	ino_t inode_number;

//	mode_t mode
	char mode;		// file('f') or directory('d')

	time_t last_mod;	// last modification

	size_t size;	// size of file/directory

	/* All of the above are obtained with stat() system call */


	TList* Tlist;	// list of names which point to this "i-node"

	int name_counter;	// counter => how many names point to this i-node
// if it is a directory , counter must always be 1 ==> hard links forbidden for directories





/* THIS WAS CHANGED */

	LINode* list;		// pointer to the list of INodes
// this is used to "update" the list, if this inode is to be destroyed
// by calling the destructor from a Tree Node










	INode* copy;		// If SOURCE, then a pointer to its copy
//else if BACKUP ==> NULL

	INode* next;		//next inode in the List of INodes


	/* Functions */

//	INode(char*);			//constructor

	INode(char*,LINode*);			//constructor

	~INode();			//destructor

	INode* getCopy();



};



class LINode{		// List of Inodes

public:

	/* Variables */

	INode* head;
	INode* tail;


	/* Functions */

	LINode();			//constructor
	~LINode();			//destructor

	void AddINode(INode*);
	void deleteINode(INode*);

//	INode* getCopy();

	void Print();

	void getINode(ino_t,INode**);
	void getINode(char*,INode**);

	void CopyPointers(LINode**,char*,char*);		// "fix" the copy* pointer of INodes 


	void Update(INode*);		// update the list , as an inode will not be accessible




};









//#endif


























