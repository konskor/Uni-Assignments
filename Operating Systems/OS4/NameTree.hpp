//#ifndef NAMETREE_H
//#define NAMETREE_H


#include <stdio.h>
#include <string.h>

//#include "Inode.hpp"


class INode;
class LINode;


class TNode;		// forward declarion(incomplete type) for compilation purposes

//
class TList;




/* List of watchers (used by inotify) */

class Node{

	public:

	int wd;				// watch descriptor
	TNode* tnode;		// name assosciated with watch descriptor

	TList* modified;		// list of tnodes(files) that are marked as modified

	Node* next;


	/* Functions */


	Node(int,TNode* );
	~Node();


};



class List{

	public:

	Node* head;
	Node* tail;

	List();
	~List();

	void AddNode(Node*);
	void deleteNode(Node*);

	void getNode(int,Node**);


};










































	/* Complementary classes for children, in Name Tree */

class TLNode{		// Tree (child) List Node

public:

	/* Variables */

	TNode* tnode;
	TLNode* next;

	/* Functions */

	TLNode(TNode*);
	~TLNode();

};



class TList{		// Tree List (for children TNodes)

public:

	/* Variables */

	TLNode* head;
	TLNode* tail;

	/* Functions */

	TList();
	~TList();

	void AddNode(TNode*);
	void AddNode1(TNode*);
//	AddNode(char*);



	void deleteNode(TNode*);

	void Print();

	void Update(TNode*);



};







	/* Name Tree */

class TNode{			// Node of Tree (Name Tree)

public:

	/* Variables */

//	char name[100];

	char *name;			//will use snprintf and dynamically allocate

	INode* inode;			// pointer to struct Inode (mystruct)

	TList* child;			// list of pointers to children


	//this was changed
	TList* parent;



	/* Functions */

//	TNode(char*,INode*);

//	TNode(char*,INode*,LINode*);

	TNode(char*,INode*,LINode*,TList*);
	~TNode();

};







class Tree{				// our Name Tree

public:

	TNode* root;

	Tree();				// constructor
	~Tree();			// destructor

	void Creation(char*,LINode**,bool);
	void Creation1(char*,LINode**,bool);

	void Print();

	bool Exists(char*,char,bool);			// if a file or dir exists in this tree


//	void Synchronisation(Tree **);		// this one is for Source 
//	void Synchronisation1(Tree **);		// This one is for Backup

	void Synchronisation(Tree**, LINode**);		// this one is for Source 
	void Synchronisation1(Tree**, Tree**, char*, bool);		// This one is for Backup


	void RemoveWD(int,List**,bool);



};


//#endif




