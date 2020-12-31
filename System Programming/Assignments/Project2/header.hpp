#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>

#include <dirent.h>

#include <sys/errno.h>
#include <sys/stat.h>


bool RecursiveReceiver(char*,char*,int,int,int*);
bool RecursiveSender(char* ,int , int, char*, char*); 

int read_all(int, void*, size_t );
int write_all(int , void* , size_t);


void Removedir(char*);


class Node{

public:

	char* ID;
	Node* next;
	
	Node(char*);
	~Node();
	
	bool Exist(DIR*);
};






class List{

public:

	Node* head;
	Node* tail;
	
	int counter;

	List();
	~List();
	
	void AddNode(char*);
	void deleteNode(char*);
	
	bool Search(char*);	
	
};
