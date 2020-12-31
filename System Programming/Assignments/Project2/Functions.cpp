#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>

#include <dirent.h>

#include <sys/errno.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <fcntl.h>



#include "header.hpp"


void Removedir( char* name){		// remove either file or directory

	printf(" Removing %s \n\n", name);


	pid_t child;

	child = fork();

	if( child == -1){

		perror("Error in RemoveDir ( fork (1) )");
		return;

	}else if( child == 0){

		execlp("/bin/rm","rm", "-r" , name, (char*) NULL); 

		perror("Error exec in Removedir (1): ");
		return;

	}else{		//parent, waits for child to finish

		int status,temp;

		temp = waitpid(child, &status, 0);
		if( temp == -1){

			perror("Error waitpid in Removedir (1) : ");
			return;
		}


		printf(" Removed %s \n\n", name);

		return;

	}

}



























/* http://www.linux-mag.com/id/308/ */

int read_all(int fd, void* buffer, size_t count){

	int nbytes;
	int total = 0;

	char* buffer1 = (char *) buffer;	// void * ---> char * , and buffer1 contains address of buffer[0]

	while( count != 0 ) {

		nbytes = read(fd, buffer1, count);

		if( nbytes < 0){

			if(errno == EINTR) continue;		// if interrupted by signal , try again

			perror("Read Error in read_all() ");
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

			perror("Write Error in write_all() ");		// maybe the reading end closed
			return -1;
		}

		buffer1 = buffer1 + nbytes;			//move the pointer
		count = count - nbytes;				//reduce count
		total = total + nbytes;				// increase the number of bytes written

	}
	
	return total;
}









bool RecursiveReceiver(char* construct, char* remain, int fifo, int buffer_size, int* bytes ){


	/* Given a path, create the necessary directories and then create the file */
	
	char* object = strtok(remain,"/");		// it can be either sub-dir or (file or dir)
	
	char* remain1 = strtok(NULL,"");
	
	
	// create new construct (current path) exists
		
	int length = snprintf(NULL,0,"%s/%s", construct, object ) + 1;
	char construct1[length];
	snprintf(construct1,length,"%s/%s", construct, object); 
	
	
	if( remain1 != NULL){	//	this is a sub-directory ( example a/b == (strtok) ==> result is a )
	
		printf("Receiver Child: Construct1  %s is a sub-DIR \n", construct1);
	
		//check if directory exists, if not ==> create it
		
		if( mkdir(construct1, 0777) == -1){
		
			if( errno != EEXIST){

				perror("Mkdir error in RecursiveReceiver ");
			}
		}
		
		
		if( RecursiveReceiver(construct1, remain1, fifo, buffer_size, bytes) == false) return false;
						
	}else{		// remain1 == NULL ===> we have reached the file or empty dir
	
		int temp = 0;
		
		unsigned int length = 0;
		
		/* Initiating timer */
		
		alarm(30);	
		
		if( ( temp = read_all(fifo, &length, sizeof(unsigned int) ) ) == -1 ){		// an error occured while reading length of file
		
			alarm(0);
		
			return false;
		}
		*bytes = (*bytes) + temp;
		
		alarm(0);
		
		printf("Receiver Child: Length of dir entry (content length) is %d \n", length);
	
		/* We only care about regular files and directories */
		
		printf("Construct is %s\n", construct);
		printf("Object is %s\n", object);
			
		if( length == 0){	// if dir entry is directory ===> create empty directory
		
			printf("Receiver Child: Creating %s DIR \n", construct1);
		
			//if( mkdir(object, 0777) == -1){
			if( mkdir(construct1, 0777) == -1){
		
				if( errno != EEXIST){

					perror("Mkdir error in RecursiveReceiver (2) ");
				}
			}
			
			return true;		
		}

		//else this is a file
		
		// Open File
		
		printf("Construct1 is %s \n", construct1);
		
		printf("Receiver Child: Writing file %s \n", construct1);
		
		FILE* fp = fopen(construct1, "w");
		
		if( fp == NULL){
		
			perror("Couldn't create file in RecursiveReceiver ");
			return false;		
		}
		
		
		
		char buffer[buffer_size];
		
		int write_size = 0;
		
		while( length != 0 ){		//write untill we have filled the file
		
			write_size = 0;
		
			if( length > buffer_size){
			
				alarm(30);
		
				if( (temp = read_all(fifo,buffer,buffer_size) ) == -1 ){
		
					alarm(0);
		
					return false;
				}
				
				alarm(0);
				
				write_size = buffer_size;
				
				
			}else{
			
				alarm(30);
			
				if( (temp = read_all(fifo,buffer,length) ) == -1 ){
		
					alarm(0);
		
					return false;
				}
				
				alarm(0);			
				
				write_size = length;
				
			}	
			*bytes = (*bytes) + temp;
		
			fwrite(buffer, sizeof(char), write_size, fp);

			length = length - temp;		
		}
		
		printf("Receiver Child : Finished writing file %s \n", construct1);
					
		if( fclose(fp) == -1){
			
			perror("Couldn't close file in Recursive Sender");
				
			return false;
		}
	
	}
	
	return true;

}































//might add "int* file_counter"

bool RecursiveSender(char* prefix, int fifo, int buffer_size, char* ID2, char* log_file){		// might use file_counter ==> number of files sent

	/* Here we try to build our path, by adding additional items in our prefix 
	
		Example: prefix = ./1_input ===> new_prefix: ./1_input/a
	
	
	*/





	int counter = 0;		//dir entry counter

	DIR* dir = opendir(prefix);		// open prefix folder

	struct dirent *direntp = NULL;

	if( dir == NULL){
	
		perror("Something went wrong with opendir in RecursiveSender() ");
		return false;
	}
	
	
	
	struct stat temp;
	
	
	while( (direntp = readdir( dir) ) != NULL){		// read all dir entries	
	
		counter++;
	
	
		// ignore . and .. entries
		if( (strcmp(direntp->d_name,".") == 0) || (strcmp(direntp->d_name,"..") == 0) ) continue;
	
	
		// create filename/ dirname (new prefix)
		short int length = snprintf(NULL,0,"%s/%s", prefix, direntp->d_name) + 1;
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
		
			if( RecursiveSender(name, fifo, buffer_size, ID2, log_file) == false) return false;



		}else if( mode == 'f'){	//if dir entry is file ==> send over pipe 
		
			
			int bytes = 0;
			int temp1 = 0;	
			
			
			
			
			
			
			//1st write 
			
			alarm(30);
			
			
			if( (temp1 = write_all( fifo, &length, sizeof(short int) ) == -1) ){			// write the length of the filename into pipe (2 bytes)
			
				alarm(0);
			
				if( errno == EPIPE ){
				
					printf("Aborted communication with %s (Read end of %s has been closed ) \n", ID2, ID2);
					
//					pid_t father = getppid();	// parent pid
	
//					kill(father,SIGUSR1);
					
					closedir(dir);
					
					return false;				
				}
				//else, just return false
				return false;		
			}
			
			bytes+= sizeof(short int);
			
			alarm(0);
			

			//2nd Write 
			
			alarm(30);
			
			if( (temp1 = write_all( fifo, name, length ) == -1) ){			// now write the filename
			
				alarm(0);
			
				if( errno == EPIPE ){
				
					printf("Aborted communication with %s (Read end of %s has been closed ) \n", ID2, ID2);
					
//					pid_t father = getppid();	// parent pid
	
//					kill(father,SIGUSR1);
					
					closedir(dir);
					
					return false;				
				}
				//else, just return false
				return false;		
			}
			
//			printf("Sender child: Sent file/dir name %s \n", name);
			
			bytes+= length;
			
			alarm(0);
			
			
			
			
			
			
			
			
			
			// 3rd Write 
			
			alarm(30);			
			
//			printf("Sender child: Sending file of size %ld \n", temp.st_size);
			
			if((temp1 = write_all( fifo, &(temp.st_size), sizeof(unsigned int))==-1)){			// write the length of file
			
				alarm(0);
			
				if( errno == EPIPE ){
				
					printf("Aborted communication with %s (Read end of %s has been closed ) \n", ID2, ID2);
					
//					pid_t father = getppid();	// parent pid
	
//					kill(father,SIGUSR1);
					
					closedir(dir);
					
					return false;				
				}
				//else, just return false
				return false;		
			}
			
			bytes+= sizeof(unsigned int);
			
			alarm(0);
			
			

//			printf("Sender child: Start reading file %s  \n", name);
			
			
			
			/* Open file and write each line into pipe */
			
			FILE* fp = fopen(name,"r");
			
			if( fp == NULL){
			
				perror("Open error in RecursiveSender() ");
				
				closedir(dir);		//close our current directory
				return false;			
			}
			
			
			if( fclose(fp) == -1){
			
				perror("Couldn't close file in Recursive Sender");
				
				closedir(dir);		//close our current directory
				
				return false;
			}
			
			
//			printf("Sender child: File exists, start copying %s  \n", name);
			
			
			
			
			int file_fd = open(name,O_RDONLY);
			if( file_fd == -1){
			
				perror("Something went wrong with open()?? \n");
				return false;			
			}
			int write_size = 0;
			
//			printf("Sender child: Opened the file, start copying %s  \n", name);
			
			
			
			char buffer[buffer_size];
			int length1 = temp.st_size;
			
			

			
			
			while( length1 != 0){
			
				
				if( length1 < buffer_size){
				
					write_size = length1;
					
				}else if( length1 == buffer_size  ){ // length > buffer_size
				
				
					write_size = buffer_size;				

				}else{ 	// length > buffer_size
				
					write_size = buffer_size;					
				}	
				
					
				// load data into buffer	
				if( (temp1 = read_all( file_fd, buffer, write_size) )== -1){
				
					closedir(dir);
					return false;				
				}	
				bytes+= write_size;
				
				alarm(30);
						
				if( (temp1 = write_all( fifo, buffer, write_size) == -1) ){// write the content of file 
					alarm(0);
			
					if( errno == EPIPE ){
				
						printf("Aborted communication with %s (Read end of %s has been closed ) \n", ID2, ID2);
					
						//pid_t father = getppid();	// parent pid
	
						//kill(father,SIGUSR1);
					
						closedir(dir);
					
						return false;				
					}
					//else, just return false
					return false;		
				}
			
				bytes+= write_size;
				
				alarm(0);
				
				length1 = length1 - write_size;
				
				
			}
				
				
			if( close(file_fd) == -1){
			
				perror("Couldn't close file in Recursive Sender (1)");
				
				closedir(dir);		//close our current directory
				
				return false;
			}
			
			
			/* LOG FILE */
			
			FILE* fp1 = fopen(log_file,"a");
			if( fp1 == NULL){
			
				perror("Couldn't write in log_file , RecursiveSender() ");
				return false;
			}
			
			
			fprintf(fp1,"Sent %d bytes for %s file \n", bytes, name);
			
			
			fclose(fp1);
			
			
			
//			printf("\n\n Sender Child: Finished writing file %s \n\n", name);
									
		}
		// checking next dir entry
		
	}
	// finished with this dir ==> close dir and return
	
	
	if( counter == 2){		// the dir had only . and .. inside ==> empty dir
	
		// just send the dir
		
		int length = strlen(prefix) + 1;
		int temp1 = 0;
		int bytes = 0;
		
		alarm(30);
		
//		printf("Sender child: Sending Empty Dir \n");
		
		
		if( (temp1 = write_all( fifo, &length, sizeof(short int) ) == -1) ){// write the length of the dirname into pipe (2 bytes)
		
			alarm(0);
		
			if( errno == EPIPE ){
				
				printf("Aborted communication with %s (Read end of %s has been closed ) \n", ID2, ID2);
					
		//		pid_t father = getppid();	// parent pid
	
		//		kill(father,SIGUSR1);
					
				closedir(dir);
					
				return false;				
			}
			
			//else, just return false
			return false;		
		}
			
		bytes+= sizeof(short int);
		
		alarm(0);
			
			
			
			
			
			
			
		//2nd Write 
			
		alarm(30);	
		
//		printf("Sender Child: The name of the empty dir is %s \n", prefix);
			
		if( (temp1 = write_all( fifo, prefix, length ) == -1) ){ // now write the dirname
			
			alarm(0);
			
			if( errno == EPIPE ){
				
				printf("Aborted communication with %s (Read end of %s has been closed ) \n", ID2, ID2);
					
		//		pid_t father = getppid();	// parent pid
	
		//		kill(father,SIGUSR1);
					
				closedir(dir);
					
				return false;				
			}
			//else, just return false
			return false;		
		}
			
		bytes+= length;
	
		alarm(0);	
			
			
			 
			
			
			
			
			
		// 3rd Write 
			
		int k1 = 0;
		
		alarm(30);
						
		if( (temp1 = write_all( fifo, &k1, sizeof(unsigned int) ) == -1) ){// if length = 0 ==> this is a dir
	
			alarm(0);
			
			if( errno == EPIPE ){
				
				printf("Aborted communication with %s (Read end of %s has been closed ) \n", ID2, ID2);
					
//				pid_t father = getppid();	// parent pid
	
//				kill(father,SIGUSR1);
					
				closedir(dir);
					
				return false;				
			}
			//else, just return false
			return false;		
		}
			
		bytes+= sizeof(unsigned int);
		
		
		alarm(0);	
		
		printf("Sender child: Finished sending empty dir %s \n\n", prefix);
			
			
		/* LOG FILE WRITE */
		FILE* fp1 = fopen(log_file,"a");
		if( fp1 == NULL){
			
			perror("Couldn't write in log_file , RecursiveSender() ");
			return false;
		}
			
			
		fprintf(fp1,"Sent %d bytes for %s empty DIR \n", bytes, prefix);
		
			
		fclose(fp1);	
				
	}
	
	
	
	closedir(dir);		//close our current directory

	return true;
} 
























/* Node */


Node::Node(char* id){

	int length = strlen(id) + 1;
	
	ID = new char[length];

	strcpy(ID, id);
}



Node::~Node(){

	delete ID;
	ID = NULL;
}



bool Node::Exist(DIR* dir){

	struct dirent* direntp = NULL;
	
	while( (direntp = readdir(dir) ) != NULL){
	
		if( strcmp( direntp->d_name, this->ID ) == 0) return true;
	
		//else check the next dir entry	
	}
	
	//this Node->ID doesn't exist in our folder ==> will delete later

	return false;
}









/* List */

List::List(){

	head = NULL;
	tail = NULL;
	
	counter = 0;
}

List::~List(){



	if( head == NULL) return;

	printf("Hi from destructor \n");


	Node* temp;
	
	while( head != NULL){
	
		temp = head;
		
		head = head->next;
	
		delete temp;
		temp = NULL;
	
	}

	temp = NULL;
	tail = NULL;
}
	
	









bool List::Search(char* value){

	Node* temp = head;

	while( temp != NULL){
	
		if( strcmp( temp->ID, value) == 0){
		
			temp = NULL;
			return true;		
		}
		
	
		temp = temp->next;
	
	}


	temp = NULL;
	
	return false;
}


















void List::AddNode(char* temp){

	if( head == NULL){	// list is empty
	
		head = new Node(temp);
		
		tail = head;
		
		this->counter++;
		
		return;	
	}

	// list is not empty

	Node *node = new Node(temp);
	
	tail->next = node;
	
	tail = node;
	
	node = NULL;
	
	this->counter++;


}



void List::deleteNode(char* temp){	// delete a specified node 

	if( head == NULL){
	
		printf("List is empty , cannot delete\n");
		return;
	}
	

	
	// Check if head->temp = temp
	
	if( strcmp(head->ID,temp) == 0) {
	
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
	
	if( strcmp(tail->ID, temp) == 0 ){
	
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
	
	while( current != tail){
	
		if( strcmp(current->ID , temp) == 0){
		
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























