#include <stdio.h>

#include <stdlib.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <errno.h>

#include <unistd.h>
#include <fcntl.h>

#include <sys/stat.h>
#include <poll.h>

#include <string.h>

#include <dirent.h>

#include <pthread.h>

#include <arpa/inet.h>


#include "header.hpp"


extern volatile sig_atomic_t flag;
extern volatile bool CS_inside;

extern pthread_mutex_t mutex;

extern pthread_cond_t condition;





///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////



void Poll_Print(struct pollfd* fds, int fd_counter){

	int i = 0;
	
	for( i = 0; i < fd_counter; i++){
	
		if( i == fd_counter-1){	// the last_one
		
			printf("%d\n\n", fds[i].fd);
			continue;		
		}
		
		printf("%d->", fds[i].fd);	
	} 



}





void Poll_Increase_Size(struct pollfd** fds, int* size){		// if array of struct of pollfd is full ==> realloc()


	size_t new_size = ( (*size) * 3)/2;		// increase by 3/2
	
	struct pollfd* fds1 = (struct pollfd*) realloc( (void*) *fds, new_size);
	if( fds1 == NULL ){
		perror("Something is wrong in Poll_increase_size() ");
		return;
	
	}
	
	
	*size = new_size;	// insert new values
	*fds = fds1;
}












void Poll_update(struct pollfd* fds, int i, int fd_counter){

	if( i != fd_counter-1 ){	// not the last fd

		memcpy(&fds[i], &fds[fd_counter-1], sizeof(struct pollfd) );
	}
	
	fds[fd_counter-1].fd = -1;
	fds[fd_counter-1].events = 0;
	fds[fd_counter-1].revents = 0;
	
	return;
}








int set_NONBLOCK(int fd){

	int flags = 0;
	
	if( (flags = fcntl(fd,  F_GETFL, 0) ) < 0 ) return -1;

	if( fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0) return -1;

	return 0;
}









int getString(int fd, char* buffer, int length, int* bytes_read){

	int i = 0;
	
	char c = '1';
	
	int bytes = 0;
	
//	while( ( i < length) && (read(fd,&c,sizeof(char) ) > 0 ) && ( c != '\0' ) ){

	while( ( i < length) && ( c != '\0' ) ){
	
		bytes = read(fd,&c,sizeof(char));
		
		if( bytes == 0 ){	// Reached EOF??
		
			printf("Reached EOF so early?? Error in getString() \n");
	
			*bytes_read = 0;
			return -1;		
		}else if( bytes < 0){
		
			perror("Error in read() inside getString() : ");
			
			*bytes_read = -1;			
			return -1;
		
		}
	
	
		buffer[i] = c;		
		
		i++;
	}


	if( c != '\0'){
	
		printf("Something went wrong in getString() with our string (didn't finish with NULL char)\n");
		
		return -1;	
	}

	buffer[i] = c; 	// NULL char = '\0'


	*bytes_read = i;	 


	return 0;
}



////////////////////////////////////////////////////////
///////////////////////////////////////////////////////
//////////////////////////////////////////////////////


unsigned long int getTimestamp(char* path){

	if( path == NULL){
	
		printf("Inside getTimestamp(), path== NULL??\n\n");
		return 0;
	}
	
//	printf("\nInside getTimestamp() , path is %s \n",path);

	struct stat stat1;

	if( stat(path, &stat1) < 0){
	
		perror("Error in getTimestamp() ==> stat() : ");
		printf("\n");
	
		return 0;
	}

	unsigned long int timestamp = stat1.st_mtime;		// last modified
	
	return timestamp;
}

unsigned long int getFileSize(char* path){

	if( path == NULL){
	
		printf("Inside getFileSize(), path== NULL??\n\n");
		return 0;
	}
	
	

	struct stat stat1;

	if( stat(path, &stat1) < 0){
	
		perror("Error in getFileSize() ==> stat() : ");
		printf("\n");
	
		return 0;
	}

	unsigned long int size = stat1.st_size;		// file size in bytes
	
	return size;



}





////////////////////////////////////////////////////
///////////////////////////////////////////////////
//////////////////////////////////////////////////
/////////////////////////////////////////////////


bool Recursive_Initialize(char* prefix, List2** result ){				// Initialise our File_name list (List2) given an input dir	==> Recursively

	/* Here we try to build our path, by adding additional items in our prefix 
	
		Example: prefix = ./1_input ===> new_prefix: ./1_input/a
	*/
	
	
	int counter = 0;		//dir entry counter

	DIR* dir = opendir(prefix);		// open prefix folder

	struct dirent *direntp = NULL;

	if( dir == NULL){
	
		perror("Something went wrong with opendir in Recursive_Initializer() :  ");
		return false;
	}
	
	
	struct stat temp;
		
	while( (direntp = readdir( dir) ) != NULL){		// read all dir entries	
	
		counter++;
	
	
		// ignore . and .. entries
		if( (strcmp(direntp->d_name,".") == 0) || (strcmp(direntp->d_name,"..") == 0) ) continue;
	
	
		// create filename/ dirname (new prefix)
		unsigned short int length = snprintf(NULL,0,"%s/%s", prefix, direntp->d_name) + 1;
		char name[length];
		snprintf(name, length,"%s/%s", prefix, direntp->d_name);
		
		
		//check if entry is dir or file
		if( stat(name,&temp)  == -1){

			perror("Stat() in RecursiveSender: ");
			closedir(dir);		//close our current directory
			
			return false; 
		}

		/* We only care about regular files and directories */
		
		char mode;

		switch( temp.st_mode & S_IFMT){

			case S_IFDIR:	mode = 'd';		// if directory
							break;

			case S_IFREG:	mode = 'f';		//if regural file
							break;

			default:	printf("Don't care about this type of file \n");
						break;
		}
	
	
		if( mode == 'd'){	// if dir entry is directory ===> dive
		
			if( Recursive_Initialize(name, result) == false) return false;


		}else if( mode == 'f'){	//if dir entry is file ==> add to the list
		
		
			// the paths will be : dirname/dir1/dir2/.../file (not ./dirname )
			
			// we want: dir1/dir2/..../file
			
			char* name1 = strtok(name,"/");			// name1 = dirname

			char* name2 = strtok(NULL,"");				// name2 = dir1/dir2/.../file
		
			
			(*result)->AddNode2(name2);		// add this filename in the list ( of file names)			
		}
		// checking next dir entry
		
	}
	// finished with this dir ==> close dir and return
				
	closedir(dir);		//close our current directory

	return true;
} 


//////////////////////////////////////////////////////
/////////////////////////////////////////////////////
////////////////////////////////////////////////////
///////////////////////////////////////////////////


//bool Recursive_Create(char* construct, char* remain, bool file){		// given a filename (Recursive Receiver)

bool Recursive_Create(char* construct, char* remain){		// given a filename (Recursive Receiver)


	/* Given a path, create the necessary directories and then create the file */
	
	char* object = strtok(remain,"/");		// it can be either sub-dir or (file or dir)
	
	char* remain1 = strtok(NULL,"");
	
	
	// create new construct (current path) exists
		
	int length = snprintf(NULL,0,"%s/%s", construct, object ) + 1;
	char construct1[length];
	snprintf(construct1,length,"%s/%s", construct, object); 
	
	
	if( remain1 != NULL){	//	this is a sub-directory ( example a/b == (strtok) ==> result is a )
	
		printf("Construct1  %s is a sub-DIR \n", construct1);
	
		//check if directory exists, if not ==> create it
		
		if( mkdir(construct1, 0777) == -1){
		
			if( errno != EEXIST){

				perror("Mkdir error in RecursiveReceiver ");
			}
		}
		
		
		if( Recursive_Create(construct1, remain1) == false) return false;
						
	}else{		// remain1 == NULL ===> we have reached the file 
	
		int temp = 0;
		
		unsigned int length = 0;
		
//		printf("Receiver Child: Length of dir entry (content length) is %d \n", length);
	
		/* We only care about regular files and directories */
		
		printf("Construct is %s\n", construct);
		printf("Object is %s\n", object);
		
		
		
		// Open/Create File
		
		printf("Construct1 is %s \n", construct1);
		
		
		int file_fd = open(construct1, O_CREAT|O_RDONLY, 0777);
		
		if( file_fd == -1){
		
			perror("Couldn't open file in Recursive_Create");
			return false;	
		}
		
		close(file_fd);
		
	
	}
	
	return true;

}



///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////





/* After we create the file, and we receive the FILE_SIZE bytes */

bool Fill_File(char* file, unsigned long file_size, int sock){

	int bytes = 0;

	char* buffer = new char[file_size];
	
	// Read from socket file_size*bytes
	if( ( bytes = read_all(sock,buffer,file_size) ) < file_size ){		// if we read less than desired bytes
	
		return false;	
	} 

	// open file and write the bytes
	int fd = open(file, O_WRONLY|O_APPEND);			//write only
	if( fd == -1){
		perror("Error in open() inse Fill_File() : ");
		printf("\n");
		
		return false;	
	}


	if( write_all( fd, buffer, file_size) < 0){

		return false;
	}

	// close file 
	close(fd);

	delete[] buffer;
	buffer = NULL;
		
	return true;
}

















///////////////////////////////////////////////////
//////////////////////////////////////////////////
/////////////////////////////////////////////////
////////////////////////////////////////////////
///////////////////////////////////////////////





/* http://www.linux-mag.com/id/308/ */

int read_all(int fd, void* buffer, size_t count){

	int nbytes;
	int total = 0;

	char* buffer1 = (char *) buffer;	// void * ---> char * , and buffer1 contains address of buffer[0]

	while( count != 0 ) {

		nbytes = read(fd, buffer1, count);

		if( nbytes <= 0){		// previously < 0     <==== IMPORTANT

			if( (errno == EAGAIN) || (errno == EWOULDBLOCK) ) continue;			// not yet ready, continue

			if(errno == EINTR) continue;		// if interrupted by signal , try again


			if( nbytes < 0){		// error occured with connection

				perror("Read Error in read_all() ");
				
			}else{		// EOF reached <===== peer shutdowns earlier than expected
			
				printf("Reached EOF in read_all()??  ==> ERROR return -1 \n");
			}			
			
			return -1;
		}

		buffer1 = buffer1 + nbytes;		//move the pointer
		count = count - nbytes;		// reduce count
		total = total + nbytes;		// increase the number of bytes read

	}

	return total;
}


int write_all(int fd, void* buffer, size_t count){

	int nbytes;
	int total = 0;

	char* buffer1 = (char *) buffer;	// void * ---> char * , and buffer1 contains address of buffer[0]

	while( count != 0 ) {

		nbytes = write(fd, buffer1, count);

		if( nbytes < 0){
		
			if(errno == EINTR) continue;	// if interrupted by signal try again
			
			if( errno == EAGAIN) continue;	// if this happens ==> peer Receiver buffer full and our Sender buffer is also full 

			perror("Write Error in write_all() ");		// maybe the reading end closed
			return -1;
		}

		buffer1 = buffer1 + nbytes;			//move the pointer
		count = count - nbytes;				//reduce count
		total = total + nbytes;				// increase the number of bytes written

	}
	
	return total;
}





//////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


/* Circular Buffer */


CNode::CNode(unsigned long IP1, unsigned short port1){		// component of circular buffer

	path = NULL;
	
	timestamp = 0;
	
	IP = IP1;
	port = port1;
}



CNode::CNode(char* path1, unsigned long int  timestamp1, unsigned long IP1, unsigned short port1){		// component of circular buffer


	int length = strlen(path1) + 1;
	path = new char[length];
	strcpy(path,path1);
	
	timestamp = timestamp1;
	
	IP = IP1;
	port = port1;
}



CNode::~CNode(){

	if( path != NULL) delete[] path;
	path = NULL;
}


/* Circular Buffer */


CBuffer::CBuffer(int size1){


	size = size1;
	
	array = new CNode*[size];		// created an array of pointers to CNodes
	
	int i = 0;
	for( i = 0; i < size; i++)	array[i] = NULL;

	
	count = 0;	// the buffer is empty at start

	readIndex = 0;
	writeIndex = 0;


}


CBuffer::~CBuffer(){

	int i = 0;
	
	for( i = 0; i < size; i++){
	
		if( array[i] != NULL) delete array[i];	
		array[i] = NULL;
	}

	delete[] array;
	array = NULL;
}



bool CBuffer::Remove(char** path, unsigned long int* timestamp, unsigned long int* IP, unsigned short int* port ){

	if( count == 0){
	
		printf("Circular Buffer is empty, can't remove \n");	
		return false;
	}
	
	CNode* temp = array[readIndex];
	
	if( temp == NULL)
	
	*timestamp = temp->timestamp;
	
	*IP = temp->IP;
	*port = temp->port;
	
	

	// now for the path
	if( temp->path != NULL){
	
		char* string = temp->path;
	
		int length = strlen( string ) + 1;
		*path = new char[length];
		strcpy(*path, string);
		
		string = NULL;	
	}else{
	
		*path = NULL;
	}
	
	
	/* Fixing ReadIndex , WriteIndex and count */
	
	int new_ReadIndex = ( readIndex + 1) % size;		
	readIndex = new_ReadIndex;		

//	int new_writeIndex = ( writeIndex + 1) % size;		
//	writeIndex = new_writeIndex;	
	
	count--;	// decrementing counter

	temp = NULL;
	return true;
}



/* 2 CBuffer::Add */

bool CBuffer::Add(char* path, unsigned long int timestamp, unsigned long int IP, unsigned short int port ){

	if( count == size){
	
		printf("Circular Buffer is full \n");	
		return false;
	}
	
	if( path == NULL){
	
		printf("Error inside CBuffer::Add(), path is NULL \n\n");
		return false;
	}
	
//	CNode* temp = array[writeIndex];
	

	/* Creating the new CNode */
	array[writeIndex] = new CNode(path,timestamp,IP,port);
	
	
	
	
	/* Fixing ReadIndex , WriteIndex and count */
	
//	int new_ReadIndex = ( readIndex + 1) % size;		
//	readIndex = new_ReadIndex;		

	int new_writeIndex = ( writeIndex + 1) % size;		
	writeIndex = new_writeIndex;	
	
	count++;	// incrementing counter	
	

//	temp = NULL;
	return true;
}


bool CBuffer::Add( unsigned long int IP, unsigned short int port ){

	if( count == size){
	
		printf("Circular Buffer is full \n");	
		return false;
	}
		
//	CNode* temp = array[writeIndex];
	
	/* Creating the new CNode */
	array[writeIndex] = new CNode(IP,port);
	
	
	
	
	/* Fixing ReadIndex , WriteIndex and count */
	
//	int new_ReadIndex = ( readIndex + 1) % size;		
//	readIndex = new_ReadIndex;		

	int new_writeIndex = ( writeIndex + 1) % size;		
	writeIndex = new_writeIndex;	
	
	count++;	// incrementing counter	
	

//	temp = NULL;
	return true;
}





bool CBuffer::empty(){

	if( count == 0) return true;
	
	return false;
}


bool CBuffer::full(){

	if( count == size) return true;
	
	return false;
}




////////////////////////////////////////////////////
///////////////////////////////////////////////////
//////////////////////////////////////////////////
/////////////////////////////////////////////////
////////////////////////////////////////////////
///////////////////////////////////////////////















/* Client List Node */

Node::Node(unsigned long IP1, unsigned short port1 ){

/*

	IP_socket.sin_family = AF_INET;
	
	IP_socket.sin_port = port;
	
	IP_socket.sin_addr.s_addr = IP;
*/

	IP = IP1;
	port = port1;

	
	this->next = NULL;
}


Node::~Node(){

	next = NULL;
}



/* Client List */

List::List(){

	head = NULL;
	tail = NULL;
	
	counter = 0;
}



List::~List(){

	if( head == NULL) return;

	Node* temp = NULL;
	
	while( head != NULL){
	
		temp = head;
		
		head = head->next;
	
		delete temp;
		temp = NULL;	
	}

	temp = NULL;
	tail = NULL;
}
	






void List::AddNode(unsigned long IP, unsigned short port){

	if( head == NULL){	// list is empty
	
		head = new Node(IP,port);
		
		tail = head;
		
		this->counter++;
		
		return;	
	}

	// list is not empty

	Node *node = new Node(IP,port);
	
	tail->next = node;
	
	tail = node;
	
	node = NULL;
	
	this->counter++;
}






void List::deleteNode(unsigned long IP, unsigned short port){	// delete a specified node 

	if( head == NULL){
	
		printf("List is empty , cannot delete\n");
		return;
	}
	

	printf("IP is %ld , port is %d\n", IP, port);
	
	// Check if head->temp = temp

	printf(" Head->IP is %ld and  Head->port is %d\n", head->IP, head->port);

	if( ( head->IP == IP) && ( head->port == port ) ){
	
		printf("\nDeleting Head\n");

	
		// We need both IP and port, as identification elements 
	
		Node* node = head;
		
		head = head->next;
		if(head == NULL) tail = NULL;
		
		//delete temp;
		delete node;
		node = NULL;
		
		this->counter--;	
		
		return;
	}
	
	
	// if tail is temp

	printf(" Tail->IP is %ld and  Tail->port is %d\n", tail->IP, tail->port);

	if( ( tail->IP == IP) && ( tail->port == port ) ){
	
		printf("\nDeleting tail\n");
	
		Node* temp1 = head;
	
		while( temp1->next != tail) temp1 = temp1->next;
		// reached the node before tail
		
		tail = temp1;
		
		temp1 = temp1->next;
		delete temp1;
		temp1 = NULL;
		
		tail->next = NULL;
		
		this->counter--;
		
		return;
	}
	
	// temp is between head and tail
	
	Node* current = head->next;
	Node* previous = head;
	
	while( ( current!= NULL) &&( current != tail ) ){
		
		if( ( current->IP == IP) && ( current->port == port ) ){
		
			previous->next = current->next;
			
			delete current;
			
			current = NULL;
			previous = NULL;
			
			this->counter--;
			
			return;
		
		}
		
		// this isn't the node moving on
		
		previous = current;
		
		current = current->next;
	
	
	}
	
	previous = NULL;
	current = NULL;
	
	// didn't find the Node in this List
	
	printf(" Didn't find the node you were looking for, cannot delete \n");
	
	return;
}




bool List::Search(unsigned long IP, unsigned short port){

	Node* temp = head;

	while( temp != NULL){
	
		printf("\n\nHi\n\n");
		
		printf("Temp->Ip is %ld and IP is %ld\n", temp->IP, IP);
		printf("Temp->port is %d and port is %d\n", temp->port, port);

		if( ( temp->IP == IP) && ( temp->port == port ) ){
		
			printf("We found our Node\n\n");
		
			temp = NULL;
			return true;		
		}
		
	
		temp = temp->next;
	
	}


	temp = NULL;
	
	return false;
}











/* Job List (Server) */

Node1::Node1(int fd1, char* job1, unsigned long IP1, unsigned short port1, unsigned long dest_IP1, unsigned short int dest_port1 ){



	fd = fd1;
	
	job = new char[strlen(job1)+1];
	strcpy(job,job1);

	IP = IP1;
	port = port1;
	
	dest_IP = dest_IP1;
	dest_port = dest_port1;
	
	
	filename = NULL;
	

	this->next = NULL;
}


Node1::Node1(int fd1, char* job1, unsigned long IP1, unsigned short port1, unsigned long dest_IP1, unsigned short int dest_port1, char* filename1 ){



	fd = fd1;
	
	job = new char[strlen(job1)+1];
	strcpy(job,job1);

	IP = IP1;
	port = port1;
	
	dest_IP = dest_IP1;
	dest_port = dest_port1;
	
	
	filename = new char[strlen(filename1)+1];
	strcpy(filename,filename1);
	

	this->next = NULL;
}









Node1::~Node1(){

	delete[] job;
	job = NULL;

	if( filename != NULL) delete[] filename;
	filename = NULL;

	next = NULL;
}



/* Client List */

List1::List1(){

	head = NULL;
	tail = NULL;
	
	counter = 0;
}



List1::~List1(){

	if( head == NULL) return;

	Node1* temp = NULL;
	
	while( head != NULL){
	
		temp = head;
		
		head = head->next;
	
		delete temp;
		temp = NULL;	
	}

	temp = NULL;
	tail = NULL;
}
	






//void List1::AddNode1(int fd, char* job, unsigned long IP, unsigned short port){

void List1::AddNode1(int fd, char* job, unsigned long IP, unsigned short port, unsigned long dest_IP, unsigned short int dest_port){

	if( head == NULL){	// list is empty
	
		head = new Node1(fd,job,IP,port, dest_IP, dest_port);
		
		tail = head;
		
		this->counter++;
		
		return;	
	}

	// list is not empty

	Node1 *node = new Node1(fd,job,IP, port, dest_IP, dest_port);
	
	tail->next = node;
	
	tail = node;
	
	node = NULL;
	
	this->counter++;
}






//void List1::deleteNode1(int fd, char* job, unsigned long IP, unsigned short port){	// delete a specified node 

void List1::deleteNode1(int fd){	// delete a specified node 
	if( head == NULL){
	
		printf("List1 is empty , cannot delete\n");
		return;
	}
	

	
	// Check if head->temp = temp
	
	if( head->fd == fd){
	
		// We need both IP and port, as identification elements 
	
		Node1* node = head;
		
		head = head->next;
		if(head == NULL) tail = NULL;
		
		//delete temp;
		delete node;
		node = NULL;
		
		this->counter--;	
		
		return;
	}
	
	
	// if tail is temp

	if( tail->fd == fd){
	
		Node1* temp1 = head;
	
		while( temp1->next != tail) temp1 = temp1->next;
		// reached the node before tail
		
		tail = temp1;
		
		temp1 = temp1->next;
		delete temp1;
		temp1 = NULL;
		
		tail->next = NULL;
		
		this->counter--;
		
		return;
	}
	
	// temp is between head and tail
	
	Node1* current = head->next;
	Node1* previous = head;
	
	while( current != tail){

		if( current->fd == fd){
		
			previous->next = current->next;
			
			delete current;
			
			current = NULL;
			previous = NULL;
			
			this->counter--;
			
			return;
		
		}
		
		// this isn't the node moving on
		
		previous = current;
		
		current = current->next;
	
	
	}
	
	previous = NULL;
	current = NULL;
	
	// didn't find the Node in this List
	
	printf(" Didn't find the node you were looking for, cannot delete \n");
	
	return;
}







//void List1::Search_Find1(int fd, char* job, unsigned long IP, unsigned  short port, Node1** result){

void List1::Search_Find1(int fd, Node1** result){

	if( head == NULL){
	
		printf("The list is empty , cannot find node");
		return;	
	}

	if( head->fd == fd){
		*result = head;	
		return;
	}

	if( tail->fd == fd ){
		*result = tail;	
		return;
	}
	

	Node1* current = head->next;
	
	while( current != tail ){
	
		if( current->fd == fd ){
		
			*result = current;
			current = NULL;
			return;
		}
	
		current = current->next;	
	}


	current = NULL;
	*result = NULL;
	
	return;
}



/////////////////////////////////////////////
////////////////////////////////////////////
///////////////////////////////////////////
//////////////////////////////////////////
/////////////////////////////////////////
////////////////////////////////////////



/* Our File name List (client) */

Node2::Node2(char* path1){

	
	pathname = new char[strlen(path1)+1];
	strcpy(pathname,path1);

//	timestamp = getTimestamp(path1);		//retrieve Timestamp of file

	this->next = NULL;
}


Node2::~Node2(){

	delete[] pathname;
	pathname = NULL;

	next = NULL;
}





List2::List2(){

	head = NULL;
	tail = NULL;
	
	counter = 0;
}



List2::~List2(){

	if( head == NULL) return;

	Node2* temp = NULL;
	
	while( head != NULL){
	
		temp = head;
		
		head = head->next;
	
		delete temp;
		temp = NULL;	
	}

	temp = NULL;
	tail = NULL;
}
	





void List2::AddNode2(char* path){

	if( head == NULL){	// list is empty
	
		head = new Node2(path);
		
		tail = head;
		
		this->counter++;
		
		return;	
	}

	// list is not empty

	Node2 *node = new Node2(path);
	
	tail->next = node;
	
	tail = node;
	
	node = NULL;
	
	this->counter++;
}



void List2::deleteNode2(char* path1){	// delete a specified node 

	if( head == NULL){
	
		printf("List2 is empty , cannot delete\n");
		return;
	}
	

	
	// Check if head->temp = temp
	
	if( strcmp(head->pathname, path1) == 0){
	
		// We need both IP and port, as identification elements 
	
		Node2* node = head;
		
		head = head->next;
		if(head == NULL) tail = NULL;
		
		delete node;
		node = NULL;
		
		this->counter--;	
		
		return;
	}
	
	
	// if tail is temp

	if( strcmp( tail->pathname, path1) == 0){
	
		Node2* temp1 = head;
	
		while( temp1->next != tail) temp1 = temp1->next;
		// reached the node before tail
		
		tail = temp1;
		
		temp1 = temp1->next;
		delete temp1;
		temp1 = NULL;
		
		tail->next = NULL;
		
		this->counter--;
		
		return;
	}
	
	// temp is between head and tail
	
	Node2* current = head->next;
	Node2* previous = head;
	
	while( current != tail){

		if( strcmp( current->pathname, path1) == 0){
		
			previous->next = current->next;
			
			delete current;
			
			current = NULL;
			previous = NULL;
			
			this->counter--;
			
			return;
		
		}
		
		// this isn't the node moving on
		
		previous = current;
		
		current = current->next;
	
	
	}
	
	previous = NULL;
	current = NULL;
	
	// didn't find the Node in this List
	
	printf(" Didn't find the node you were looking for, cannot delete \n");
	
	return;
}




void List2::Search_Find2(char* path1, Node2** result){

	if( head == NULL){
	
		printf("The list is empty , cannot find node");
		return;	
	}



	if( strcmp( head->pathname, path1) == 0){
		*result = head;	
		return;
	}



	if( strcmp(tail->pathname,path1) == 0 ){
		*result = tail;	
		return;
	}
	

	Node2* current = head->next;
	
	while( current != tail ){
	
		if( strcmp( current->pathname,path1) == 0 ){
		
			*result = current;
			return;
		}
	
		current = current->next;	
	}


	current = NULL;
	*result = NULL;
	
	return;
}


void List2::Print(){

	Node2* temp = head;
	
	printf("\nPrinting our File List\n");
	
	while(temp != NULL){
	
		printf("%s\n",temp->pathname);
	
		temp = temp->next;
	}


}




/* List3 ==> Pending list for circular buffer */


Node3::Node3(char* path1, unsigned long int  timestamp1, unsigned long IP1, unsigned short port1){

	node = new CNode(path1,timestamp1,IP1,port1);

	this->next = NULL;
}

Node3::Node3(unsigned long IP1, unsigned short port1){

	node = new CNode(IP1,port1);
	this->next = NULL;
}


Node3::~Node3(){

	delete node;
	node = NULL;

	next = NULL;
}





List3::List3(){

	head = NULL;
	tail = NULL;
	
	counter = 0;
}



List3::~List3(){

	if( head == NULL) return;

	Node3* temp = NULL;
	
	while( head != NULL){
	
		temp = head;
		
		head = head->next;
	
		delete temp;
		temp = NULL;	
	}

	temp = NULL;
	tail = NULL;
}
	





void List3::AddNode3(char* path1, unsigned long int  timestamp1, unsigned long IP1, unsigned short port1){

	if( head == NULL){	// list is empty
	
		head = new Node3(path1,timestamp1,IP1,port1);
		
		tail = head;
		
		this->counter++;
		
		return;	
	}

	// list is not empty

	Node3 *node = new Node3(path1, timestamp1, IP1, port1);
	
	tail->next = node;
	
	tail = node;
	
	node = NULL;
	
	this->counter++;
}


void List3::AddNode3(unsigned long IP1, unsigned short port1){

	if( head == NULL){	// list is empty
	
		head = new Node3(IP1,port1);
		
		tail = head;
		
		this->counter++;
		
		return;	
	}

	// list is not empty

	Node3 *node = new Node3(IP1, port1);
	
	tail->next = node;
	
	tail = node;
	
	node = NULL;
	
	this->counter++;
}





void List3::deleteNode3(){	// delete head

	if( head == NULL){
	
		printf("List3 is empty , cannot delete\n");
		return;
	}

	
	Node3* node = head;
		
	head = head->next;
	if(head == NULL) tail = NULL;
		
	delete node;
	node = NULL;
		
	this->counter--;	
		
	return;
}
	
	

















