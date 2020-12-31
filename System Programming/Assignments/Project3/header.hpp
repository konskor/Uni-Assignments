#include <stdio.h>

#include <poll.h>

#include <netdb.h>
#include <netinet/in.h>


class List2;

class CBuffer;

/* Thread Function */
void* Thread_function(void*);


/* Helpful Functions */

void Poll_Print(struct pollfd*, int);

void Poll_Increase_Size(struct pollfd**, int*);

void Poll_update(struct pollfd* fds, int i, int fd_counter);	// whenever we erase an entry => swap the deleted entry with the last entry ( and decrease our fd_counter)

int set_NONBLOCK(int);			// set socket as O_NONBLOCK

int getString(int,char*,int,int*);	// read bytes from socket , until we read '\0' 

// if anything goes wrong (reach EOF or closed connection) ==> return -1


//////////////////////////////////
/////////////////////////////////
////////////////////////////////

unsigned long int getTimestamp(char*);

unsigned long int getFileSize(char*);
//////////////////////////////////
/////////////////////////////////
////////////////////////////////

bool Recursive_Initialize(char*, List2**);			// initalize List2 ==> our file_list

bool Recursive_Create(char*, char*);				// craete the file in the appropriate subfolder

bool Fill_File(char*, unsigned long , int);			// read from socket and fill the file



////////////////////////////////
///////////////////////////////
//////////////////////////////
/////////////////////////////

/* Functions for entering and exiting CS */

int enterMain();			// Main thread entering CS
int enterMain1();			// try to enter, but don't block

int enterWorker(CBuffer*);			// Worker thread entering CS
int enterWorker1();			// only condition is for someone to not be in CS

int exitCS();			// both do the exact same thing when exiting Critical Section


/////////////////////////////
////////////////////////////
///////////////////////////

// Read/Write functions
int read_all(int, void*, size_t);
int write_all(int, void*, size_t);



/////////////////////////////
/////////////////////////////
////////////////////////////


//extern volatile sig_atomic_t flag;
//extern volatile bool CS_inside;

//extern pthread_mutex_t mutex;

//extern pthread_cond_t condition;










/*
struct in_addr {

	unsigned long s_addr;        // Internet address (32 bits)

}


struct sockaddr_in{

	unsigned short sin_family;   // Internet protocol (AF_INET) 

	unsigned short sin_port;     // Address port (16 bits) 
	struct in_addr sin_addr;   /// Internet address (32 bits) 
}

*/







/* Circular buffer */


class CNode{

public:

	char* path;
	
	time_t timestamp;		// our version
	
	unsigned long int IP;
	unsigned short int port;
	
	
	CNode(unsigned long, unsigned short);
	CNode(char*, unsigned long, unsigned long, unsigned short);
	
	~CNode();
	
};





class CBuffer{		// Circular buffer

public:

	CNode** array;		// an array of pointers to CNodes
	
	int size;	// size of buffer
	
	int count;	// how many spots have been filled	

	int readIndex;	// available to read

	int writeIndex;		// available to write
	
	/* Functions */

	CBuffer(int);
	~CBuffer();

	bool Add(char*, unsigned long int, unsigned long int, unsigned short int);	// GET_FILE
	bool Add(unsigned long int, unsigned short int);	// connect to client
	 
	bool Remove(char**, unsigned long int*, unsigned long int*, unsigned short int*);
	
	bool full();
	bool empty();
		
};

















/* Client List */



class Node{

public:

//	struct sockaddr_in IP_socket;
	
	unsigned long IP;
	unsigned short int port;
	
	Node* next;
	
	Node(unsigned long, unsigned short);
	~Node();
	
};






class List{

public:

	Node* head;
	Node* tail;
	
	int counter;

	List();
	~List();
	
	void AddNode(unsigned long, unsigned short);
	void deleteNode(unsigned long, unsigned short);
	
	bool Search(unsigned long, unsigned short);			//if a client already exists
	
	void Broadcast(char*,unsigned long, unsigned short);	// usable by server only
	
	
};





/* pending job list for active sockets */


class Node1{

public:

	int fd;
	
	char* job;
	
	unsigned long IP;			// these are in HOST BYTE ORDER
	unsigned short int port;	// info for new/exiting user(server) or our info(client)
	
	unsigned long dest_IP;		// peer info (in case we the connection fails ==> we would not have any way to know )
	unsigned short int dest_port;
	
	
	/* This is exclusive to dropbox_client */
	
	char* filename;
	
	
	
	
	Node1* next;
	
	Node1(int, char*, unsigned long, unsigned short, unsigned long, unsigned short);			
	
	Node1(int, char*, unsigned long, unsigned short, unsigned long, unsigned short, char*);		// exclusive to dropbox_client
	
	~Node1();
};






class List1{

public:

	Node1* head;
	Node1* tail;
	
	int counter;

	List1();
	~List1();
	
	void AddNode1(int, char*, unsigned long, unsigned short, unsigned long, unsigned  short);

//	void deleteNode1(int,char*, unsigned long, unsigned short, unsigned long, unsigned short);
	void deleteNode1(int);

	
//	bool Search1(int,char*,unsigned long, unsigned short);			
	
	
	
	//void Search_Find1(int, char*, unsigned long, unsigned short, Node1** result){
	void Search_Find1(int, Node1** result);
};


/* File List */

class Node2{

public:


	char* pathname;
	
//	time_t timestamp;		// will be calculated on the spot ==> stat() --> modification time
	
	Node2* next;
	
	Node2(char*);
	~Node2();
};






class List2{

public:

	Node2* head;
	Node2* tail;
	
	int counter;

	List2();
	~List2();
	
	void AddNode2(char*);
	void deleteNode2(char*);
	
//	bool Search1(int,char*,unsigned long, unsigned  short);			//if a client already exists
	
	
	void Search_Find2(char*, Node2** result);
	
	void Print();
	
//	void Initialize(char*);

};



/* Pending List */


class Node3{

public:

	CNode* node;
	Node3* next;
	
	Node3(unsigned long, unsigned short);
	Node3(char*, unsigned long, unsigned long, unsigned short);

	~Node3();
};


class List3{	

/* if CBuffer is full, put all pending jobs in this List
		 ==> avoids deadlock 
		 
 Example: everybody wants to insert something into buffer, but buffer is full

		=> put everything you wanted to insert, in this List3, and try to free up space
*/

public:

	Node3* head;
	Node3* tail;
	
	int counter;

	List3();
	~List3();
	

	void AddNode3(unsigned long, unsigned short);
	void AddNode3(char*, unsigned long, unsigned long, unsigned short);
	
	void deleteNode3();	// always delete head
	
//	bool Search1(int,char*,unsigned long, unsigned  short);			//if a client already exists
	
	
//	void Search_Find2(char*, Node3** result);
	
//	void Print();
	
//	void Initialize(char*);

};













/*  struct Args ==> thread function */

struct Args{		// pass arhuments to thread function

	List* client_list;
	CBuffer* Cbuffer;
	
	unsigned long int this_IP;
	unsigned short int listening_port;
	
	char* dirname;
	
};

















