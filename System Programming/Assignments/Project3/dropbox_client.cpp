#include <stdio.h>

#include <sys/wait.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <netdb.h>

#include <stdlib.h>
#include <signal.h>  

#include <unistd.h> 

#include <arpa/inet.h>      

#include <string.h>

#include <errno.h>

#include <poll.h>

#include <pthread.h>

#include <sys/stat.h>
#include <fcntl.h>

#include "header.hpp"




volatile sig_atomic_t flag = 0;
//int flag = 0;

/* Global variables */

pthread_mutex_t mutex;

//pthread_cond_t empty;
pthread_cond_t condition;



//volatile bool main_inside = false;		// if main thread is inside Critical Section
volatile bool CS_inside = false;		// if main thread is inside Critical Section



void Quit(int sig){

	flag = 1; 
}




////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

int enterMain(){	// if you must enter CS

	printf("\n\n Entering CS Main %ld\n\n", pthread_self() );

	int error = 0;

	if( flag == 1) return -1;

/************************ LOCKED ****************************/
	if( (error = pthread_mutex_lock(&mutex) ) > 0){

		printf("There is something wrong with mutex { enterMain() } : ");
		return error;
	}
	
	
	if( flag == 1){		// SIGINT was caught from some thread ( not necessarily this one)

		if(  (error = pthread_mutex_unlock(&mutex) ) > 0 ){
		
			printf("There is something wrong with mutex { enterMain() }  (1) : ");
			return error;
		}		
		return -1;	
	}
	
	
	
	
	
	
/************************** CONDITION ******************************/	

	while( CS_inside == true ){
	
		if( (error = pthread_cond_wait(&condition,&mutex) ) > 0 ){
		
			printf("There is something wrong with condition_variable { enterMain() }  (1) : ");
			return error;		
		}
	
	
	
		if( flag == 1){		// SIGINT was caught from some thread ( not necessarily this one)

			if(  (error = pthread_mutex_unlock(&mutex) ) > 0 ){
		
				printf("There is something wrong with mutex { enterMain() }  (2) : ");
				return error;				
			}
				
			return -1;	
		}
				
	}
	
	
	
	
	
	
	
	CS_inside = true;
	
	if( (error = pthread_mutex_unlock(&mutex) ) > 0 ){
		
		printf("There is something wrong with mutex { enterMain() }  (3) : ");
		return error;
	}

/************************** UNLOCKED ***********************/

	printf("\n\n Entering CS Main %ld\n\n", pthread_self() );

	return 0;
}

/////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
///////////////////////////////////////////////////////
//////////////////////////////////////////////////////


int enterMain1(){	// if you want to enter CS but don't want to actually block

	printf("\n\n Entering CS Main1 %ld\n\n", pthread_self() );

	int error = 0;
	
	if( flag == 1) return -1;

	/************************ LOCKED ****************************/
	if( (error = pthread_mutex_lock(&mutex) ) > 0){

		printf("There is something wrong with mutex { enterMain1() } : ");
		return error;
	}
	
	
	if( flag == 1){		// SIGINT was caught from some thread ( not necessarily this one)

		if(  (error = pthread_mutex_unlock(&mutex) ) > 0 ){
		
			printf("There is something wrong with mutex { enterMain1() }  (1) : ");
			return error;
		}		
		return -1;	
	}
	
	
	
	
	
	
	
	

	if( CS_inside == true){		// if someone is already in CS ==> return
	
		if( (error = pthread_mutex_unlock(&mutex) ) > 0 ){
		
			printf("There is something wrong with mutex { enterMain() }  (2) : ");
			return error;
		}
		return EBUSY;			
	}
	
	
	
	
	
	
	
	CS_inside = true;
	
	if( (error = pthread_mutex_unlock(&mutex) ) > 0 ){
		
		printf("There is something wrong with mutex { enterMain() }  (3) : ");
		return error;
	}
	
		printf("\n\n Entering CS Main1 %ld\n\n", pthread_self() );
	
	return true;
}


///////////////////////////////////////////////
//////////////////////////////////////////////
/////////////////////////////////////////////
////////////////////////////////////////////
///////////////////////////////////////////

int enterWorker(CBuffer* Cbuffer){

	printf("\n\n Entering CS Worker %ld\n\n", pthread_self() );

	int error = 0;

	if( flag == 1) return -1;

/************************ LOCKED ****************************/
	if( (error = pthread_mutex_lock(&mutex) ) > 0){

		printf("There is something wrong with mutex { enterWorker() ) } : ");
		return error;
	}
	
	
	
	
	
	
	if( flag == 1){		// SIGINT was caught from some thread ( not necessarily this one)

		if(  (error = pthread_mutex_unlock(&mutex) ) > 0 ){
		
			printf("There is something wrong with mutex { enterWorker() }  (1) : ");
			return error;
		}		
		return -1;	
	}
	
	
	
	
	
	
	
/************************** CONDITION ******************************/	

//	while( (CS_inside == true) || ( Cbuffer->empty() == true) ){
	
	while( ( Cbuffer->empty() == true) || (CS_inside == true) ){
	
		if( (error = pthread_cond_wait(&condition, &mutex) ) > 0 ){
		
			printf("There is something wrong with condition_variable { enterWorker() }  (1) : ");
			return error;		
		}
	
	
	
	
		if( flag == 1){		// SIGINT was caught from some thread ( not necessarily this one)

			if(  (error = pthread_mutex_unlock(&mutex) ) > 0 ){
		
				printf("There is something wrong with mutex { enterWorker() }  (2) : ");
				return error;				
			}
				
			return -1;	
		}
				
	}
	
	
	
	
	
	
	
	
	
	CS_inside = true;
	
	
	if( (error = pthread_mutex_unlock(&mutex) ) > 0 ){
		
		printf("There is something wrong with mutex { enterWorker() ) : ");
		return error;
	}

/************************** UNLOCKED ***********************/

	printf("\n\n Entering CS Worker (1) %ld\n\n", pthread_self() );

	return 0;
}

///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////




int enterWorker1(){	// for pending items, at the start of while loop


	printf("\n\n Entering CS Worker1 %ld\n\n", pthread_self() );

	int error = 0;

	if(flag == 1) return -1;
	
/************************ LOCKED ****************************/
	if( (error = pthread_mutex_lock(&mutex) ) > 0){

		printf("There is something wrong with mutex { enterWorker() ) } : ");
		return error;
	}
	
	
	
	
	
	
	if( flag == 1){		// SIGINT was caught from some thread ( not necessarily this one)

		if(  (error = pthread_mutex_unlock(&mutex) ) > 0 ){
		
			printf("There is something wrong with mutex { enterWorker() }  (1) : ");
			return error;
		}		
		return -1;	
	}
	
	
	
	
	
	
	
/************************** CONDITION ******************************/	

	while( CS_inside == true ){
	
		if( (error = pthread_cond_wait(&condition,&mutex) ) > 0 ){
		
			printf("There is something wrong with condition_variable { enterWorker() }  (1) : ");
			return error;		
		}
	
	
	
	
		if( flag == 1){		// SIGINT was caught from some thread ( not necessarily this one)

			if(  (error = pthread_mutex_unlock(&mutex) ) > 0 ){
		
				printf("There is something wrong with mutex { enterWorker() }  (2) : ");
				return error;				
			}
				
			return -1;	
		}
				
	}	//end of while loop
	
	
	
	
	
	
	
	
	
	CS_inside = true;
	
	
	if( (error = pthread_mutex_unlock(&mutex) ) > 0 ){
		
		printf("There is something wrong with mutex { enterWorker() ) : ");
		return error;
	}

/************************** UNLOCKED ***********************/

	printf("\n\n Entering CS Worker1 (1) %ld\n\n", pthread_self() );


	return 0;
}

/////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
///////////////////////////////////////////////////////
//////////////////////////////////////////////////////
/////////////////////////////////////////////////////
////////////////////////////////////////////////////
///////////////////////////////////////////////////




int exitCS(){

	int error = 0;
	
	if( flag == 1) return -1;

	/************************ LOCKED ****************************/
	if( (error = pthread_mutex_lock(&mutex) ) > 0){

		printf("There is something wrong with mutex { exitCS() ) } : ");
		return error;
	}
	
	
	
	/*
	
	if( flag == 1){		// SIGINT was caught from some thread ( not necessarily this one)

		if(  (error = pthread_mutex_unlock(&mutex) ) > 0 ){
		
			printf("There is something wrong with mutex { exitCS() }  (1) : ");
			return error;
		}		
		return -1;	
	}
	
	*/
	
	
	
	
	
	
	CS_inside = false;
	
	if( ( error = pthread_cond_broadcast(&condition) ) > 0){			// will only fail with EINVAL

		printf("There is something wrong with condition_variable { exitCS() }  (2) : ");
		return error;
	}
	
	
	
	
	
	

	if( (error = pthread_mutex_unlock(&mutex) ) > 0 ){
		
		printf("There is something wrong with mutex { exitCS() ) (3) : ");
		return error;
	}
	
	printf("\n\n Exiting CS %ld\n\n", pthread_self() );
	
	
	return 0;

}



/////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////








void* Thread_function(void* args){



	/* Input arguments */
	struct Args* args1 = (struct Args*) args;

	List* client_list = args1->client_list;
	CBuffer* Cbuffer = args1->Cbuffer;
	
	List3* pending_list = new List3();		// pending items to put into circular buffer
	
	unsigned long int this_IP = args1->this_IP;
	unsigned short int listening_port = args1->listening_port;
	
//	char* dirname = args1->dirname;
	
	
	/* Local variables */
	
	char* path = NULL;
	unsigned long int timestamp = 0;
	
	unsigned long int IP = 0;
	unsigned short int port = 0;
	
	
	int len = strlen(args1->dirname) + 1;
	char* dirname = new char[len];
	strcpy(dirname, args1->dirname);
	
	
	int error = 0;
	
	struct sockaddr_in client;
    socklen_t clientlen;

    struct sockaddr *clientptr=(struct sockaddr *)&client;
    
    int client_socket = -1;
		
	while( flag != 1){	//until SIGINT
	
		clientlen = sizeof(client);	
		
		
		
		
		/* Check Pending List of items and Cbuffer */
			
	
		if( pending_list->counter != 0){
		
			if( flag == 1) continue;
		/****************************************************** LOCKED *******************************************/
		
			int error = enterWorker1();		//aquire lock
			if( error == 0){	// all ok
		
			// lock aquired
			
				if( Cbuffer->full() == false){	//there is space, to put our items
			
					int available = Cbuffer->size - Cbuffer->count;
					
					int i = 0;
												    				
 	  				for( i = 0; i < available; i++){
 	  		
 	  					if( pending_list->head == NULL) break;		// don't have any pending items to put into buffer
 	  					
 	  			
 	  					if( pending_list->head->node->path == NULL){
 	  			
 							// Adding to Circular Buffer :  < IP, port>    				
	 	  					if( Cbuffer->Add( pending_list->head->node->IP, pending_list->head->node->port) == false){
 	  					
 		  						printf("\n\n This shouldn't be full (1) \n\n");			
 		  					} 		  				
 						}else{	//path != NULL
 					
 							// Adding to Circular Buffer :  < pathname, version/timestamp, IP, port>
 							if( Cbuffer->Add( pending_list->head->node->path, pending_list->head->node->timestamp, pending_list->head->node->IP, pending_list->head->node->port) == false){
 	  					
 		  						printf("\n\n This shouldn't be full (2) \n\n");			
 		  					} 		  				 					 				 				
 						}	// end of if clause ( path == NULL)  				
 					
						pending_list->deleteNode3();	// delete head and move to the next one					    				

 	  				}	// end of for loop   
 	  	 	  	  	
	 	  		}		// end of if clause (not full)
    			
 		  		int error = exitCS();						
				if( error > 0){
							
					printf("%s (13)\n", strerror(error) );
					return NULL;
				}		
					
		/******************************* UNLOCKED ****************************************/	    	

			}else if( error > 0){		// an error occured while aquiring 
			
				printf("%s (14)\n",strerror(error) );			

			}else{
			
				continue;	// SIGINT ==> flag == -1
			
			}	// end of if clase (error == 0)						
			
		}// end of if clause ( pending jobs )		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
								
	
	
					
		/****************************************************************** Starting our Procedure *********************************************************************/
		
		
		/* Read an item from circular buffer */
		
		if( flag ==1) continue;
		
		/************************** LOCKED ***************************************************/
						
		int error = enterWorker(Cbuffer);				
		if( error > 0 ){
						
			printf("%s (15)\n", strerror(error) );
			return NULL;

		}else if( error < 0) continue;		// SIGINT																																		
		
		
		// Read from buffer
		
		if( Cbuffer->Remove( &path, &timestamp, &IP, &port) == false ){
		
			printf("Something went wrong with Remove() (1) \n");
			
			continue;
			
		}
	
		// if path != NULL , remember to deallocate
		
		error = exitCS();						
		if( error > 0){
							
			printf("%s (16)\n", strerror(error) );
			return NULL;
		}					
					
		/******************************* UNLOCKED ****************************************/
		

		
		if( path == NULL){		// < IP, port> ==> GET_FILE_LIST from this client
	
			printf("\nInside path = NULL, %ld \n\n", pthread_self());		
		
			unsigned long int Network_IP = htonl(IP);
			unsigned short int Network_port = htons(port);
			
			/* Create an (active) and a passive socket */
    
    		if( (client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){

       			 perror("Error in socket() (1) : ");
       			 continue;               			       				
		    }        
    
		    /* Configuring socket ==> SO_REUSEADDR */   
		    int enable = 1;   
		    if( setsockopt(client_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int) ) < 0 ){
    
    			printf("Error in setsockopt() (1) : ");	
    			continue;
			}            		
    
		    /* Configuring (active socket) client address struct */
    		client.sin_family = AF_INET;
    		client.sin_addr.s_addr = Network_IP;
    		client.sin_port = Network_port;
		

			int k = connect(client_socket,clientptr,clientlen);	//block untill we connect
			if( k < 0){
			
				printf("Thread connect() error (1)");			
				continue;			
			}			
			
			printf("Successfuly connected GET_FILE_LIST %ld\n",pthread_self() );
			// connect was successful
			
			// Creating message
		    int length = snprintf(NULL,0,"GET_FILE_LIST") + 1;
			char* get_file_list = new char[length]; 	
			snprintf(get_file_list,length,"GET_FILE_LIST");
			
			if( write_all(client_socket, get_file_list, length) < 0 ){
			
				shutdown(client_socket, SHUT_RDWR);
				close(client_socket);
				client_socket = -1;
				
				delete[] get_file_list;
				get_file_list = NULL;
				
				continue;			
			}
			
			unsigned long int Nthis_IP = htonl(this_IP);
			if( write_all(client_socket, &Nthis_IP, sizeof(unsigned long int) ) < 0 ){
			
				shutdown(client_socket, SHUT_RDWR);
				close(client_socket);
				client_socket = -1;
				
				delete[] get_file_list;
				get_file_list = NULL;
				
				continue;			
			}
			
			unsigned short int Nlistening_port = htons(listening_port);
			if( write_all(client_socket, &Nlistening_port, sizeof(unsigned short int) ) < 0 ){
			
				shutdown(client_socket, SHUT_RDWR);
				close(client_socket);
				client_socket = -1;
				
				
				delete[] get_file_list;
				get_file_list = NULL;
				
				continue;			
			}			
			
			shutdown(client_socket,SHUT_WR);		// nothing else to send
			
			
			/* Now read what client has to say */
			
			length = (strlen("FILE_LIST") + 1) + 10;		// 10 chars for the integer
			char* user_message = new char[length];
			
			int bytes_read = 0;
			
			
			if( getString(client_socket, user_message, length, &bytes_read) == -1){	// error has occured 
					
				/*	What might have happened :
						
					a) Run out of space in buffer ==> is not one of our strings
						
					b) Reached EOF early? ==> graceful shutdown
						
					c) ECONNRESET, something happened with server ==> abnormal shutdown
						
				*/
						
				shutdown(client_socket,SHUT_RD);	// close read END
					
				close(client_socket);			// free the file descriptor
				client_socket = -1;
				
				delete[] get_file_list;
				get_file_list = NULL;
			
				delete[] user_message;
				user_message = NULL;
					
				continue;					
			}			
			
			int num_files = 0;
			if( sscanf( user_message,"%*s %d", &num_files) != 1){
				
				shutdown(client_socket,SHUT_RD);	// close read END
					
				close(client_socket);			// free the file descriptor
				client_socket = -1;
				
				delete[] get_file_list;
				get_file_list = NULL;
			
				delete[] user_message;
				user_message = NULL;
					
				continue;
			} 
			
			
			unsigned short int length1 = 0;
			
			unsigned long int timestamp = 0;
			
//			printf("\n\n\n HELLO \n\n\n");
			
			
			int i = 0;
			for( i = 0; i < num_files; i++){
			
				if( read_all(client_socket,&length1,sizeof(unsigned short int) ) < 0){		// read length of pathname
				
					shutdown(client_socket,SHUT_RD);	// close read END
						
					close(client_socket);			// free the file descriptor
					client_socket = -1;					
				
					delete[] get_file_list;
					get_file_list = NULL;
			
					delete[] user_message;
					user_message = NULL;
					
					break;
		
				}
				
				length1 = ntohs(length1);
				char* path1 = new char[length1];
				
								
				if( read_all(client_socket,path1, length1 ) < 0){		// read pathname
				
					shutdown(client_socket,SHUT_RD);	// close read END
						
					close(client_socket);			// free the file descriptor
					client_socket = -1;					
				
					delete[] get_file_list;
					get_file_list = NULL;
			
					delete[] user_message;
					user_message = NULL;					
					
					break;
		
				}
				
				unsigned long int timestamp1 = 0;
				if( read_all(client_socket,&timestamp1,sizeof(unsigned long int) ) < 0){	// read version/timestamp
				
					shutdown(client_socket,SHUT_RD);	// close read END
						
					close(client_socket);			// free the file descriptor
					client_socket = -1;					
					
					delete[] path1;
					path1 = NULL;
				
					delete[] get_file_list;
					get_file_list = NULL;
			
					delete[] user_message;
					user_message = NULL;
					
					break;
		
				}
				
				timestamp1 = ntohl(timestamp1);
				
//				printf("\n\n\n HELLO (1) \n\n\n");
				
				
				/* Insert into circular buffer */
				
				if( flag == 1) break;
				
				/************************** LOCKED ********************************/
						
				int error = enterWorker1();		// check only if someone is inside		
				if( error > 0 ){
						
					printf("%s (17)\n", strerror(error) );
					return NULL;

				}else if( error < 0) continue;		// SIGINT
				
				
				if( Cbuffer->full() == false){
				
					Cbuffer->Add(path1,timestamp1,IP,port);
					printf("Added %s into Cbuffer\n",path1);
				
				}else{
				
					pending_list->AddNode3(path1,timestamp1,IP,port);
					
					printf("Added %s into Cbuffer\n",path1);				
				}
				
				
				
				error = exitCS();						
				if( error > 0){
							
					printf("%s (18)\n", strerror(error) );
					return NULL;
				}
				
				
//				printf("\n\n\n HELLO (2) \n\n\n");													
					
				/******************************* UNLOCKED ****************************************/								
				
				delete[] path1;
				path1 = NULL;
			
			}	// end of for loop						
			
			delete[] user_message;
			user_message = NULL;
				
			delete[] get_file_list;
			get_file_list = NULL;
			
			printf("GET_FILE_LIST ===> DONE  %ld \n",pthread_self() );			

			continue;
			
/////////////////////////////////////////////////
////////////////////////////////////////////////
///////////////////////////////////////////////
//////////////////////////////////////////////	
		
		}else{	// path != NULL
		
			printf("\nInside path != NULL \n");
		
		
			/* Check if client is in our list */
			
			if( flag == 1) continue;
			
			/************************** LOCKED ********************************/
						
			int error = enterWorker1();		// check only if someone is inside						
			if( error > 0 ){
						
				printf("%s (19)\n", strerror(error) );
				return NULL;

			}else if( error < 0) continue;		// SIGINT
			
			
			
			if( client_list->Search(IP,port) == false){
			
			
				error = exitCS();						
				if( error > 0){
							
					printf("%s (20)\n", strerror(error) );
					return NULL;
				}		
					
				/******************************* UNLOCKED ****************************************/
			
				// client doesn't exist ==> log_off or hasn't logged on yet		
				// either way do nothing
			
				delete[] path;
				path = NULL;

				continue;	

			}	// end of if clause (client search)
			
			
			
			error = exitCS();						
			if( error > 0){
							
				printf("%s (20)\n", strerror(error) );
				return NULL;
			}		
					
			/******************************* UNLOCKED ****************************************/
			
			
			
			
			
			
			/* Clients EXISTS */
			
					
			/* Create Dir_name of client */
			
			struct sockaddr_in sa;
			char* IP_str = new char[INET_ADDRSTRLEN];
			
			sa.sin_addr.s_addr = htonl(IP);
			inet_ntop(AF_INET, &(sa.sin_addr), IP_str, INET_ADDRSTRLEN);
			
			int length = snprintf(NULL,0,"%s_%d",IP_str,port) + 1;
			char* client_dir = new char[length];
			snprintf(client_dir,length,"%s_%d",IP_str,port);
			
					
					
		
			/* Check if file exists */
		
			bool file_exist = false;
		
			length = snprintf(NULL,0,"%s/%s/%s", dirname, client_dir,path) + 1;
			char* filename = new char[length];
			snprintf(filename,length,"%s/%s/%s", dirname, client_dir,path);
			

			if( access(filename,F_OK) == 0) file_exist = true;
		
			
//			if( file_exist == false){
			
			unsigned long int Network_IP = htonl(IP);
			unsigned short int Network_port = htons(port);
				
				
			/* Create an (active) and a passive socket */
    
	   		if( (client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){

	   			 perror("Error in socket() (1) : ");
	   			 continue;               			       				
		    }        
    
		    /* Configuring socket ==> SO_REUSEADDR */   
		    int enable = 1;   
		    if( setsockopt(client_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int) ) < 0 ){
    
	   			printf("Error in setsockopt() (1) : \n");	
	   			continue;
			}            		
    
		    /* Configuring (active socket) client address struct */
	   		client.sin_family = AF_INET;
	   		client.sin_addr.s_addr = Network_IP;
	   		client.sin_port = Network_port;
		

			int k = connect(client_socket,clientptr,clientlen);	//block untill we connect
			if( k < 0){
			
				printf("Thread connect() error (1)\n");			
				continue;			
			}			
				
			// Successfuly connected									
																	
			length = strlen("GET_FILE") + 1;
			char* get_file = new char[length];
			strcpy(get_file,"GET_FILE");
				
			
			unsigned short int length1 = strlen(path) + 1;
				
				
			if( write_all(client_socket, get_file, length) < 0 ){
			
				shutdown(client_socket, SHUT_RDWR);
				close(client_socket);
				client_socket = -1;				
				
			
				delete[] get_file;
				get_file = NULL;
				
				delete[] get_file;
			
				continue;			
			}
			
			unsigned long int Nthis_IP = htonl(this_IP);
			if( write_all(client_socket, &Nthis_IP, sizeof(unsigned long int) ) < 0 ){		// write our IP
			
				shutdown(client_socket, SHUT_RDWR);
				close(client_socket);
				client_socket = -1;				
				
				delete[] get_file;
				get_file = NULL;
				
				continue;			
			}
			
			unsigned short int Nlistening_port = htons(listening_port);
			if( write_all(client_socket, &Nlistening_port, sizeof(unsigned short int) )  < 0 ){		// write listening port
			
				shutdown(client_socket, SHUT_RDWR);
				close(client_socket);
				client_socket = -1;				
				
				delete[] get_file;
				get_file = NULL;
				
				continue;			
			}
				
			unsigned short int Nlength1 = htons(length1);
			if( write_all(client_socket, &Nlength1, sizeof(unsigned short int) )  < 0 ){	// write length of pathname
			
				shutdown(client_socket, SHUT_RDWR);
				close(client_socket);
				client_socket = -1;				
				
				delete[] get_file;
				get_file = NULL;
				
				continue;			
			}
				
				
			if( write_all(client_socket, path, length1)  < 0 ){	// write pathname
			
				shutdown(client_socket, SHUT_RDWR);
				close(client_socket);
				client_socket = -1;				
				
				delete[] get_file;
				get_file = NULL;
				
				continue;			
			}
			
			// now for the version/timestamp
				
			unsigned long int timestamp = 0;
				
			if( file_exist == true){
				
				timestamp = getTimestamp(filename);
				if( timestamp == 0){
					
					printf("An error has occured with getTimestamp() in thread (1) \n");
					return NULL;						
				}
			}
			
			unsigned long int Ntimestamp = htonl(timestamp);
			if( write_all(client_socket, &Ntimestamp, sizeof(unsigned long int) ) < 0 ){		// write our version/timestamp
			
				shutdown(client_socket, SHUT_RDWR);
				close(client_socket);
				client_socket = -1;				
				
				delete[] get_file;
				get_file = NULL;
				
				continue;			
			}
				
			shutdown(client_socket,SHUT_WR);
				
			/* Now we await peer's message */
				
			int size = 20;
			char* buffer = new char[size];
			int bytes_read = 0;
				
			if( getString(client_socket, buffer, size, &bytes_read) == -1){	// error has occured 
					
				/*	What might have happened :
						
						a) Run out of space in buffer ==> is not one of our strings
						
						b) Reached EOF early? ==> graceful shutdown
						
						c) ECONNRESET, something happened with server ==> abnormal shutdown
						
				*/
						
				shutdown(client_socket,SHUT_RD);	// close read END
					
				close(client_socket);			// free the file descriptor
				client_socket = -1;													
		
				delete[] buffer;
				buffer = NULL;
					
				continue;					
			} 
				
				
			if( strcmp(buffer,"FILE_NOT_FOUND") == 0){
				
				printf("\n\n Client doesn't have or has deleted file : %s \n\n", path);
					
				shutdown(client_socket, SHUT_RD);
				close(client_socket);
				client_socket = -1;					
				
			}else if( strcmp(buffer,"FILE_UP_TO_DATE") == 0){
				
				printf("\n\n Our file is up to date : %s \n\n", path);
					
				shutdown(client_socket, SHUT_RD);
				close(client_socket);
				client_socket = -1;				
								
			}else if( strcmp(buffer,"FILE_SIZE") == 0){
			
				unsigned long int timestamp1 = 0;
				unsigned long int length_file = 0;		// number of bytes		
			
				// Read filename_length  (Network byte address)
				if( read_all(client_socket, &timestamp1, sizeof(unsigned long int) ) < 0 ){		// returns -1 if EOF reached or read return -1
						
					// check for ECONNRESET or EAGAIN and EWOULDBLOCK
					// also check if read_all returned 0 ==> reached EOF
							
					//in all of the cases ==> ERROR, client has abruptly ended connection
							
					printf("Read Error (1) thread \n\n");
												
					shutdown(client_socket,SHUT_RDWR);					
					close(client_socket);			// free the file descriptor
					client_socket = -1;					
																			
					continue;												
				}				
				timestamp1 = ntohl(timestamp1);		// Host byte order
								
				// Read filename_length  (Network byte address)
				if( read_all(client_socket, &length_file, sizeof(unsigned long int) ) < 0 ){		// returns -1 if EOF reached or read return -1
						
					// check for ECONNRESET or EAGAIN and EWOULDBLOCK
					// also check if read_all returned 0 ==> reached EOF
							
					//in all of the cases ==> ERROR, client has abruptly ended connection
							
					printf("Read Error (2) thread \n\n");
												
					shutdown(client_socket,SHUT_RDWR);					
					close(client_socket);			// free the file descriptor
					client_socket = -1;					
																			
					continue;												
				}				
				length_file = ntohl(length_file);		// Host byte order	
				
																						
				
				/* Now Fill the file */
				
				char* buffer1 = new char[length_file];
				
				if( read_all(client_socket, buffer1, length_file ) < 0 ){		// returns -1 if EOF reached or read return -1
						
					// check for ECONNRESET or EAGAIN and EWOULDBLOCK
					// also check if read_all returned 0 ==> reached EOF
							
					//in all of the cases ==> ERROR, client has abruptly ended connection
							
					printf("Read Error (3) thread \n\n");
												
					shutdown(client_socket,SHUT_RDWR);					
					close(client_socket);			// free the file descriptor
					client_socket = -1;					
																			
					continue;												
				}			
				
				shutdown(client_socket,SHUT_RD);
				close(client_socket);
				client_socket = -1;				
				
				/* Creating the file */
				
//				if( Recursive_Create(filename,NULL) == false){


				length = snprintf(NULL,0,"%s/%s", client_dir, path ) + 1;
				char* temp_c = new char[length];
				snprintf( temp_c,length,"%s/%s", client_dir, path); 


				
				if( Recursive_Create(dirname, temp_c) == false){
				
					printf("Something went wrong with Recursive_Create() \n\n");
					return NULL;				
				}
				
							
				int file_fd = open(filename,O_WRONLY, 0777);
				if( file_fd < 0){
				
					printf("Error opening/creating file (thread) \n\n");
					continue;				
				}
				
				if( write_all(file_fd, buffer1, length_file)  < 0 ){	// write pathname
			
					printf("Something went wrong while writing into the new file (thread) \n\n");				
					continue;			
				}
				
				close(file_fd);
				file_fd = -1;
				
				delete[] buffer1;
				buffer1 = NULL;
			
			/* Update timestamp, if file does not exist or we have an older version */
			
			// struct stat.m_time autommatically gets updated	
				
			}									
															
			/* Update timestamp, if file does not exist or we have an older version */	
			
			printf("GET_FILE %s ===> DONE  %ld \n",path,pthread_self() );				
			
			delete[] buffer;
			buffer = NULL;
			
			delete[] filename;
			filename = NULL;	
		
			delete[] client_dir;
			client_dir = NULL;
		
			delete[] IP_str;
			IP_str = NULL;
		
			delete[] path;
			path = NULL;
			

			
			continue;	
		}	// end of if clause (path == NULL)
	
	
	
	
	
	}	// end of while loop ( flag != 1)
	
	/* Start cleanup */
	
	args = NULL;
	
	client_list = NULL;
	Cbuffer = NULL;
	
	if( path != NULL) delete[] path;
	path = NULL;
	
	
	delete pending_list;
	pending_list = NULL;
	
	delete[] dirname;
	dirname = NULL;
	
	
	
	pthread_cond_broadcast(&condition);		// wake up any remaining threads
	
	printf("Thread %ld waking up\n", pthread_self() );
	
	pthread_exit(NULL);
	
	
}	// end of function/thread







/////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////



























int main(int argc, char* argv[] ){

	if( argc != 13){				//server port, server IP, port Number (client)
	
		printf("Need 12 arguments (argc=13) \n");
	
		return -1;
	}
	
	// SIGINT
	struct sigaction sa;
	sa.sa_handler = &Quit;
	sigemptyset(&sa.sa_mask);	//block all signals
	sa.sa_flags = SA_RESTART;	// retry system calls


	if( sigaction(SIGINT,&sa,NULL) == -1){
	
		perror("Sigaction error (1): ");
		return -1;	
	}
		
	//SIGPIPE
	struct sigaction sa1;
	sa1.sa_handler = SIG_IGN;
	sigemptyset(&sa1.sa_mask);	//block all signals
	sa1.sa_flags = SA_RESTART;	// retry system calls		
			
		
	if( sigaction(SIGPIPE,&sa,NULL) == -1){
	
		perror("Sigaction error (2): ");
		return -1;	
	}
	
	
	/* Read arguments */
	
	char* dirname = NULL;		//  ==> ./dirname
	
	int this_port = 0;		// client's port
	
	int num_threads = 0;
	
	int buffer_size = 0;  
	
	char* IP_server = NULL;
	int server_port = 0;		// Server port number


	
	
	
	
	int i = 0;
	for( i = 0; i < argc; i++){

		if( strcmp(argv[i] , "-d") == 0 ){		// dirname
		
			if( i == argc-1){
				printf(" Something is wrong: -n ..... \n");
				return -1;		
			}

			int length = 0;

			length = strlen( argv[i+1] ) + 1; 
		
			dirname = new char[length];
			
			strcpy(dirname, argv[i+1] );
			
			continue;
		}
		
		

		if( strcmp(argv[i] , "-p") == 0 ){		// portNum ==> clients port
		
			if( i == argc-1){
				printf(" Something is wrong: -c ..... \n");
				return -1;		
			}
		
			this_port = atoi(argv[i+1]);
			
			continue;
		}
		
		
		if( strcmp(argv[i] , "-w") == 0 ){		// Worker threads
		
			if( i == argc-1){
				printf(" Something is wrong: -i ..... \n");
				return -1;		
			}
		
			num_threads = atoi(argv[i+1]);
			
			continue;		
		

		}
		
		if( strcmp(argv[i] , "-b") == 0 ){		// Size of circular buffer
		
			if( i == argc-1){
				printf(" Something is wrong: -m ..... \n");
				return -1;		
			}
		
			buffer_size = atoi(argv[i+1]);
			
			if( buffer_size <= 0){
				
				printf("buffer_size must be > 0 \n");
				return -1;
			
			} 
			
			
			continue;
		
		

		}		


		if( strcmp(argv[i] , "-sp") == 0 ){		// Server Port
		
			if( i == argc-1){
				printf(" Something is wrong: -h2 ..... \n");
				return -1;		
			}
		
			server_port = atoi( argv[i+1] );
			
			continue;
		}
		
		

		if( strcmp(argv[i] , "-sip") == 0 ){		// Server IP
		
			if( i == argc-1){
				printf(" Something is wrong: -b ..... \n");
				return -1;		
			}
		
			int length = 0;

			length = strlen( argv[i+1] ) + 1; 
		
			IP_server = new char[length];
			
			strcpy(IP_server, argv[i+1] );
			
			printf("IP_server is %s\n", IP_server);
			
			continue;
		}

	}
	
	
	/* Creating our File List */
	
	List2* file_list = new List2();
	
	Recursive_Initialize(dirname,&file_list );			// Fill our list
	
	

	/*
	file_list->Print();
	
	
	delete file_list;
	file_list = NULL;
	
	return 0;
	*/
	
	
	/* Printing IP of machine */
    
    char hostname[100];
    struct hostent *host_entry = NULL;
    char IP_str[INET_ADDRSTRLEN];
    
    unsigned long Nthis_IP = 0;			// Network byte order
    unsigned long this_IP = 0;			// Host byte order
    
    
    if( gethostname(hostname,100) < 0){
    
    	perror(" Error in hostname() : ");
    	
    	return -1;
    	
    }
    
    if( ( host_entry = gethostbyname(hostname) ) == NULL){
    
    	herror("Error in gethostbyname() : ");
    
    	return -1;   
    }
    
   
      
    inet_ntop( host_entry->h_addrtype , ( (struct in_addr*) host_entry->h_addr_list[0]), IP_str, INET_ADDRSTRLEN );
    
    if( IP_str == NULL){
    
    	perror("Error in inet_ntop (1) : ");
    
    	return -1;   
    }
    
    
    
     
    
    //inet_pton( host_entry->h_addrtype, IP_str, &this_IP);
    Nthis_IP = inet_addr(IP_str);
    
    this_IP = ntohl(Nthis_IP);
    
    printf("Client IP is %s\n", IP_str);
    
//    printf("Client IP is %ld\n", this_IP);
    
    


	


	// socket is our listening socket
	int server_socket = -1, client_socket = -1;		// client_socket will be the active socket ==> sends or receives bytes
	
	
	
	

	struct sockaddr_in client,server;
    socklen_t clientlen,serverlen;

    struct sockaddr *clientptr=(struct sockaddr *)&client;
	struct sockaddr *serverptr=(struct sockaddr *)&server;    
  
    
     /* Create an (active) and a passive socket */
    
    if( (client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){

        perror("Error in socket() (1) : ");
        
        serverptr = NULL;
        clientptr = NULL;
        
        return -1;
    }    
    
    if( (server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    
    	perror("Error in socket() (2) : ");
        
        serverptr = NULL;
        clientptr = NULL;
        
        return -1;  
    }
    
    
    /* Configuring socket ==> SO_REUSEADDR */   
    int enable = 1;   
    if( setsockopt(client_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int) ) < 0 ){
    
    	perror("Error in setsockopt() (1) : ");
    	
    	close(server_socket);
    	return -1;   
    }
    
    
    enable = 1;   
    if( setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int) ) < 0 ){
    
    	perror("Error in setsockopt() (2) : ");
    	
    	close(server_socket);
    	return -1;   
    }    
      
    
    /* Configuring (active socket) client address struct */
    client.sin_family = AF_INET;
    inet_pton(AF_INET, IP_server, &(client.sin_addr) );
    client.sin_port = htons(server_port);
    
    
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(this_port);    
    
    
    /* Bind socket to address ==> give the socket a "name" -> IP/port */
    if (bind(server_socket, serverptr, sizeof(server)) < 0){

        perror("Error in bind() ");
        
        close(server_socket);
        
        serverptr = NULL;
        clientptr = NULL;
        
        return -1;
     }
    
    
    
    
	/* server_socket is now a passive/server socket */ 
	if (listen(server_socket, 5) < 0){
	
     	perror("Error in listen() ");
     		
     		
     	if( shutdown(server_socket,SHUT_RDWR) < 0)     		
     		perror("Error in shutdown() (1): ");     		
     		     		
     	close(server_socket);
     		
     	serverptr = NULL;
        clientptr = NULL;
        	
        return -1;     		
	}


	/* Set listening socket to O_NONBLOCK */
	
	
	if( set_NONBLOCK(server_socket) == -1){
	
		perror("Error in set_NONBLOCK() (1) : ");
		printf("\n");
     		
     		
     	if( shutdown(server_socket,SHUT_RDWR) < 0){
     		perror("Error in shutdown() (2): ");
     		printf("\n");
     	}     		
     		     		
     	close(server_socket);
     		
     	serverptr = NULL;
        clientptr = NULL;
        	
        return -1;
	}
	
	printf("Listening socket set to O_NONBLOCK \n");



	

	/* Printing port of listening socket */
	
	struct sockaddr_in sin;
	socklen_t len = sizeof(sin);
	
	if( getsockname(server_socket, (struct sockaddr *)&sin, &len) < 0){
	
		perror("Error in getsockname() : ");
		
		close(server_socket);
		
		return -1;
	}
	

	unsigned short int listening_port = ntohs(sin.sin_port);	
	
	printf("Listening port is %d\n", listening_port );
	printf("Listening port is %d (network byte order) \n\n", sin.sin_port );

	

	/* Creating the client_list */
	
	List* client_list = new List();
    
    
    /* Connecting to server */
	if (connect(client_socket, clientptr, sizeof(client)) < 0){		// block untill server responds
		perror("Error in connect() :");  
		
		close(client_socket);
		close(server_socket);
		
    	return -1;
    }
    
    
    /*********************************** Sending LOG_ON ******************************************/
    
    
    printf("\nSending LOG_ON\n");
    
    int size = strlen("LOG_ON") + 1;
    char* log_on = new char[size];
    strcpy(log_on,"LOG_ON");
    
    unsigned long Network_IP = htonl(this_IP);
    unsigned short int Network_port = htons(listening_port);
    
    printf("Network port is %d \n",Network_port);
    
    
    
    if( write_all( client_socket, log_on, size) < 0){	// server closed the connection?
    
    	close(client_socket);
    	close(server_socket);
    	
    	delete[] log_on;
    	log_on = NULL;
    	
    	return -1;   
    }
    
    
	if( write_all( client_socket, &Network_IP, sizeof(unsigned long) ) < 0){	// server closed the connection?
    
    	close(client_socket);
    	close(server_socket);
    
    	delete[] log_on;
    	log_on = NULL;
    
    	return -1;   
    }    
    
    
    if( write_all( client_socket, &Network_port, sizeof( unsigned short int) ) < 0){	// server closed the connection?
    
    	close(client_socket);
    	close(server_socket);
    	
    	delete[] log_on;
    	log_on = NULL;    	
    	
    	return -1;   
    }
    
    
    shutdown(client_socket, SHUT_RDWR);
    
    close(client_socket);
    client_socket = -1;
    
    delete[] log_on;
    log_on = NULL;
    
    
    /******************************************** GET_CLIENTS *********************************************/
    
    printf("\nGET_CLIENTS\n");
    
    int length = strlen("GET_CLIENTS") + 1;
    char* get_clients = new char[length];
    strcpy(get_clients,"GET_CLIENTS");
    
    
    if( (client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){		// create a socket again

        perror("Error in socket() (3) : ");
        
    	serverptr = NULL;
    	clientptr = NULL;
    	
    	delete[] get_clients;
    	get_clients = NULL;
        
		return -1;
    }
    
    
    if (connect(client_socket, clientptr, sizeof(client)) < 0){		// block untill server responds
		perror("Error in connect() (2) :");  
		
		close(client_socket);
		close(server_socket);
		
    	delete[] get_clients;
    	get_clients = NULL;
		
		
    	return -1;
    }
    
    printf("Connected with Server, getting clients \n\n");
    
    // Send the message 
    
    if( write_all( client_socket, get_clients, length) < 0){	// server closed the connection?
    
    	close(client_socket);
    	close(server_socket);
    	
    	delete[] get_clients;
    	get_clients = NULL;
    	
    	
    	return -1;   
    }
    
    shutdown( client_socket, SHUT_WR);
    
    
    // Now read the string sent by server
    
    int size1 = ( strlen("CLIENT_LIST ") + 1 ) + 10; 		// 10 char bytes to represent the integer in string form
    char* buffer = new char[size1];
    
    int bytes_read = 0;
    
    printf("Entering getString getclients \n\n");
    
    if( getString(client_socket, buffer, size1, &bytes_read) == -1){	// error has occured 
					
		/*	What might have happened :
						
			a) Run out of space in buffer ==> is not one of our strings
						
			b) Reached EOF early? ==> graceful shutdown
						
			c) ECONNRESET, something happened with server ==> abnormal shutdown
						
		*/
						
		shutdown(client_socket,SHUT_RD);	// close read END
					
		close(client_socket);			// free the file descriptor
		
		delete[] buffer;
		buffer = NULL;
					
		return -1;					
	}
    
    
    char* client_list_s = strtok(buffer," ");			// client_list_s -> "CLIENT_LIST"
    char* num_clients_s = strtok(NULL,"");				// num_client_s -> "%d"
    
    int num_clients = 0;
    
    if( ( client_list_s == NULL ) || ( num_clients_s == NULL ) || ( strcmp(client_list_s,"CLIENT_LIST") != 0) || ( ( num_clients = atoi(num_clients_s) ) < 0 )  ){		
    
    	// either one of the strings is NULL // we didn't receive our expected message // something is wrong with num_clients
    
    	close( server_socket);
    	close(client_socket);
    	
    	
    	if( client_list_s != NULL){
    		delete[] client_list_s;
    		client_list_s = NULL;
    	}
    	
    	if( num_clients_s != NULL){
    		delete[] num_clients_s;
    		num_clients_s = NULL;
    	}
    	
    	
    	
    	return -1;   
    }
    
  
  	// we received the correct message and num_clients is ok ==> read them through socket INCOMPLETE
  
    i = 0;
    bytes_read = 0;
    
    unsigned long Ntemp_IP = 0;			// Network byte order
    unsigned long Htemp_IP = 0;			// Host byte order
    
    unsigned short int Ntemp_port = 0;
    unsigned short int Htemp_port = 0;
    
	for( i = 0 ; i < num_clients; i++){
	
		
    
    	if( (bytes_read = read_all(client_socket, &Ntemp_IP, sizeof(unsigned long) ) ) < 0 ){
    	
    		printf("Error while reading clients from socket (1) \n");
    		
    		close(server_socket);
    		close(client_socket);
    		
    		delete[] client_list_s;
   			delete[] num_clients_s;
    
		    client_list_s = NULL;
		    num_clients_s = NULL;
    		
    		return -1;   	
    	}
    
    
    	if( (bytes_read = read_all(client_socket, &Ntemp_port, sizeof(unsigned short int) ) ) < 0 ){
    	
    		printf("Error while reading clients from socket (2) \n");
    		
    		close(server_socket);
    		close(client_socket);
		
			delete[] client_list_s;
		    delete[] num_clients_s;
    
		    client_list_s = NULL;
		    num_clients_s = NULL;	
	
	    		
    		return -1;   	
    	}
    
    	/* Turn them into host byte order */
    	
    	Htemp_IP = ntohl(Ntemp_IP);
    	Htemp_port = ntohs(Ntemp_port);
    	
    	
    	if( (Htemp_IP == this_IP) && (Htemp_port == listening_port) ) continue;			// don't add yourself to the list
    	
    	
    	client_list->AddNode(Htemp_IP, Htemp_port);    // add a client to our list of clients
	
	}// end of for loop
	
	
    shutdown(client_socket, SHUT_RD);
    
    close(client_socket);
    client_socket = -1;
    
    
    client_list_s = NULL;
    num_clients_s = NULL;
    
    
/////////////////////////////////////////////////////
//////////////////////////////////////////////////// 
/////////////////////////////////////////////////// 
//////////////////////////////////////////////////
 
 
     /******************** POLLING() ******************************************/
 
 
    
     printf("Beginning Poll() \n");


	/* Set server_socket as O_NONBLOCK ===> we want to poll() */


	if( set_NONBLOCK(server_socket) == -1){
	
		perror("Error in set_NONBLOCK() (1) : ");
		printf("\n");
     		
     		
     	if( shutdown(server_socket,SHUT_RDWR) < 0){
     		perror("Error in shutdown() (1): ");
     		printf("\n");
     	}     		
     		     		
     	close(server_socket);
     		
     	serverptr = NULL;
        clientptr = NULL;
        	
        return -1;
	}
	
	printf("Listening socket set to O_NONBLOCK \n");
	
	
	/* Creating job_list */
	
//	List1* job_list = new job_list();		==> Not needed





    
    /* Initalize struct pollfds */
    
    int retval = 0;
    

    int size_poll = 50;
    struct pollfd* fds = new struct pollfd[size_poll];
    int fd_counter = 0;
    
    
    for(i = 0; i < size_poll;i++){
    
    	fds[i].fd = -1;
    	fds[i].events = 0;
    	fds[i].revents = 0;   
    }
    
    
    // Insert listening socket
    fd_counter++;
    
    fds[0].fd = server_socket;
    fds[0].events = POLLIN;
        
    
    
    /* Creating a circular buffer */
    
    CBuffer* Cbuffer = new CBuffer(buffer_size);
    
    List3* pending_list = new List3();
    
    
    // Initializing circular buffer
    
    Node* temp = client_list->head;
    
    while( temp != NULL){
    
    	if( Cbuffer->Add(temp->IP,temp->port) == false){
    	
    		// Cbuffer is full, put the rest of the clients in our pending list
    		
    		break;    		
    	}
    
    	temp = temp->next;   
    }
    
    
    // Couldn't insert all of them in Circular buffer in the pending list
    if( temp != NULL){
    
    	while( temp != NULL){
    	
    		pending_list->AddNode3(temp->IP,temp->port);
    		temp = temp->next;
    	}
    }
    
    
    /* Initializing mutex and condition varuiables */
    
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&condition, NULL);
    
    
    
    /* Creating worker Threads */
    
    struct Args arguments;
    arguments.client_list = client_list;
    arguments.Cbuffer = Cbuffer;
    arguments.this_IP = this_IP;
    arguments.listening_port = listening_port;   
    
    arguments.dirname = dirname; 
    
    
    pthread_t* thread_array = new pthread_t[num_threads]; 
        
    
    for( i = 0; i < num_threads; i++){
    
    	pthread_create( &(thread_array[i]) , NULL , Thread_function, &arguments );		// function not done / struct args not done ==> will contain < client_list, circular Buffer, our IP, our Port >    
    }
    
    
    
    
                              
    
    while( flag != 1){
    
    
    	if( pending_list->counter != 0 ){		// we have pending jobs to put in circular buffer
    	
    		int error = enterMain1();
    	
    		if( error == 0){    // All ok!
    		
 	  			if( Cbuffer->count != Cbuffer->size){		// Circular buffer is not full
    			
 	  				int available = Cbuffer->size - Cbuffer->count;
    				
 	  				for( i = 0; i < available; i++){
 	  				
 	  					if( pending_list->head == NULL) break;
    				
 	  					if( Cbuffer->Add( pending_list->head->node->IP, pending_list->head->node->port) == false){
 	  					
 	  						printf("\n\n This shouldn't be full \n\n")	;		
 	  					}
 	  					
 	  					pending_list->deleteNode3(); // delete head, go to the next one 					
    				
 	  				}	// end of for loop
    		
 	  			}	// end of if clause (not full)
    			
 	  			pthread_mutex_unlock(&mutex);
    		
			}else if( error > 0){		  	
    
    			printf("%s\n",strerror(error));
    			    	
    		}else{
    		
    			// error == -1 ==> flag = 1 , caused from SIGINT
    			continue;   		    		
    		}	//end of if clause (aquire lock)    	    	    	
    		    	
		}	// end of if clause ( pending jobs )
    	
    	   
    
    	/**************************** POLL**************/
       	clientlen = sizeof(client);

		printf("Arrived at poll\n");

		retval = poll(fds,size_poll,-1);	// block untill there is a new event (or put 10.000 milliseconds ==> 10 seconds)
		if( retval < 0){
		
			if(errno == EINTR) continue;			// most likely SIGINT
			
			perror("Error in poll : ");
			return -1;	
		}


		if( retval > 0 ){	// we have events to resolve
		
			if( fds[0].revents & POLLIN){	// listening socket has ready connections
				
        		printf("Inside POLLIN \n");

        
    			if ( (client_socket = accept(server_socket, clientptr, &clientlen) ) < 0){			// will return immediately ==. we have ready connections (POLLIN)
    	
			   		// clientsptr will have info about client 	
    				perror("Error in accept() ");
    				printf("\n");
    		
    				if( shutdown(server_socket,SHUT_RDWR) < 0){
     					perror("Error in shutdown() (2): ");     		
     					printf("\n");
     				}
    		
    				close(server_socket);
     		
     				serverptr = NULL;
        			clientptr = NULL;
        	
			       	return -1;     	
			   	}
    	
			   	printf("Accepted a connection, putting into poll() \n");
    	
			   	/* Inserting into poll() */
    	
			   	fd_counter++;
			   	if( fd_counter > size) Poll_Increase_Size(&fds,&size);
			    	
   	
				fds[fd_counter-1].fd = client_socket;
			   	fds[fd_counter-1].events = POLLIN;		// we want to read from this
      	
			    /* Configuring new (active) socket */
    	   	
			    int enable1 = 1;   
			    if( setsockopt(client_socket, SOL_SOCKET, SO_REUSEADDR, &enable1, sizeof(int) ) < 0 ){
    
			    	perror("Error in setsockopt() (2) : ");
			    	printf("\n");
    		
    		
    	
			    	if( shutdown(server_socket,SHUT_RDWR) < 0){
			    		perror("Error in shutdown() (3): ");     		
     					printf("\n");
     				}
     		
     	     	
			    	close(server_socket);
     		
     		
			    	if( shutdown(client_socket,SHUT_RDWR) < 0){     		
			    		perror("Error in shutdown() (4): ");     		
     					printf("\n");
     				}
     		
     	     	
			    	close(client_socket);
     		     		
			    	serverptr = NULL;
			       	clientptr = NULL;    	
     	    		
			    	return -1;   
		    	}
		    		
		    	/* Set as NON_BLOCK */
		    		
		    	if( set_NONBLOCK(client_socket) < 0){
		    		
		   			perror("Error in set_NONBLOCK()  (2): ");
		   			printf("\n");
    		 
    		    	if( shutdown(server_socket,SHUT_RDWR) < 0){
			    		perror("Error in shutdown() (5): ");     		
     					printf("\n");
     				}
     	     	
			    	close(server_socket);
     		
     		
			    	if( shutdown(client_socket,SHUT_RDWR) < 0){     		
			    		perror("Error in shutdown() (6): ");     		
     					printf("\n");
     				}
     			
			    	close(client_socket);
     		     		
			    	serverptr = NULL;
			       	clientptr = NULL;    	
     	    		
			    	return -1;   
		    	}
		   			
				client_socket = -1;		//resetting value					

			}	// end of if clause ( check listening socket for new connections
				
				
				
///////////////////////////////////////////////////////
//////////////////////////////////////////////////////
/////////////////////////////////////////////////////
////////////////////////////////////////////////////				
				
			printf("Checking the other fds \n\n");
				
			/* Now for the rest of the sockets from 1 to fd_counter -1 */
		
			for( i = 1; i < fd_counter; i++){  // traverse all the fds
			
				if( flag == 1) break;
			
				clientlen = sizeof(client);
			
				if( fds[i].revents == 0) continue;	// nothing happened 
				
				
			
				if( fds[i].revents & POLLERR){	// an error has occured ==> abort the connection and remove from pollfd
				
					printf("\n\nPOLLER occured \n\n");
					
					perror("POLLER was found : ");
					printf("\n\n");
				
					shutdown(fds[i].fd,SHUT_RDWR);
					close(fds[i].fd);
					
					Poll_update(fds,i,fd_counter);
					fd_counter--;
					
					continue;		
				}
				
								
				/* fds[i] has data to read */				
				if( fds[i].revents & POLLIN){	// there is data to read
				
					printf("Inside POLLIN \n");
				
					int bytes_read = 0;
					char* buffer = new char[20];
				
					if( getString(fds[i].fd, buffer, 20, &bytes_read) == -1){	// error has occured 
					
						/*	What might have happened :
						
							a) Run out of space in buffer ==> is not one of our strings
						
							b) Reached EOF early? ==> graceful shutdown
						
							c) ECONNRESET, something happened with peer ==> abnormal shutdown
						
						*/
						
						printf("\n\nError in getString() \n\n");
					
						shutdown(fds[i].fd,SHUT_WR);	// send FIN packet
						
						shutdown(fds[i].fd,SHUT_RD);	// close read END
					
						close(fds[i].fd);			// free the file descriptor
										
						/* Switch last fds of pollfd with the fds[i] */
						
						Poll_update(fds,i,fd_counter);
						fd_counter--;	
						
						delete[] buffer;
						buffer = NULL;		
						
						continue;
					}
					
					
					// Checking if the buffer contains one of our wanted strings
	
	
//////////////////////////////////////////////
/////////////////////////////////////////////
////////////////////////////////////////////
///////////////////////////////////////////
//////////////////////////////////////////			
					
					if( strcmp(buffer,"USER_ON") == 0){
					
						printf("Received USER_ON \n");
			
						shutdown(fds[i].fd, SHUT_WR);
			
						/* Search if client already exists */
			
						/* Read the IP and Port of client */
							
						unsigned long Network_IP_address = 0;
						unsigned short int Network_port = 0;
	
				
						// Read IP_address of client (Network byte address)
						if( read_all(fds[i].fd, &Network_IP_address, sizeof(unsigned long) ) < 0 ){		// returns -1 if EOF reached or read return -1
						
							// check for ECONNRESET or EAGAIN and EWOULDBLOCK
							// also check if read_all returned 0 ==> reached EOF
							
							//in all of the cases ==> ERROR, client has abruptly ended connection
																			
							shutdown(fds[i].fd,SHUT_RDWR);					
							close(fds[i].fd);			// free the file descriptor
							
							Poll_update(fds,i,fd_counter);
							fd_counter--;
																				
							continue;												
						}				
						
						
						if( read_all(fds[i].fd, &Network_port, sizeof(unsigned short int) ) < 0){		// Read port of client ( Network byte order)
						
							// check for ECONNRESET or EAGAIN and EWOULDBLOCK
							// also check if read_all returned 0 ==> reached EOF
							
							//in all of the cases ==> ERROR, client has abruptly ended connection
												
							shutdown(fds[i].fd,SHUT_RDWR);					
							close(fds[i].fd);			// free the file descriptor
							
							Poll_update(fds,i,fd_counter);
							fd_counter--;
																
							continue;					
						}
				
									
					
    			 		shutdown(fds[i].fd,SHUT_RD);
    			 		
						close(fds[i].fd);
						
						Poll_update(fds,i,fd_counter);
						fd_counter--;
						
				
						// network byte order ==> host byte order
						unsigned long Host_IP_address = ntohl(Network_IP_address);
						unsigned short int Host_port = ntohs(Network_port);
						
						
						
						
						
						/* Check if user exists, and insert into circular buffer */	
						
						if( flag == 1) continue;										
						
						/************************** LOCKED ********************************/
						
						int error = enterMain();				
						if( error > 0 ){
						
							printf("%s (1)\n", strerror(error) );
							return -1;
						}
						
									
						/* Search if client already exists */
			
						if( client_list->Search(Host_IP_address,Host_port) == false){
				
							printf("Client doesn't exist in our list \n");
						
							client_list->AddNode(Host_IP_address,Host_port);
							
							
							//Insert into circular buffer
							
							if( Cbuffer->full() == false){
							
								Cbuffer->Add(Host_IP_address, Host_port);			//adding to circular buffer								
							
							}else{
							
								pending_list->AddNode3(Host_IP_address, Host_port);		// pending job							
							}
							
							
							error = exitCS();						
							if( error > 0){
							
								printf("%s (2)\n", strerror(error) );
								return -1;
							}																								
					/******************************* UNLOCKED ****************************************/
					
					
							delete[] buffer;
							buffer = NULL;	
							
							printf("\n\nUSER_ON complete (2)\n\n");	
							
							continue;
    
    					}else{
    					
    						int error = exitCS();						
							if( error > 0){
							
								printf("%s (3)\n", strerror(error) );
								return -1;
							}																	
					
					/******************************* UNLOCKED ****************************************/
    					
    		
    						printf("ERROR why does user already EXIST???? (USER_ON) \n\n");
    						
    						delete[] buffer;
    						buffer = NULL;
    						
    						continue;   		
    					}
												
//////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
				
					}else if(strcmp(buffer,"USER_OFF") == 0){

						printf("Received USER_OFF \n");
			
						shutdown(fds[i].fd, SHUT_WR);
			
						/* Search if client already exists */
			
						/* Read the IP and Port of client */
							
						unsigned long Network_IP_address = 0;
						unsigned short int Network_port = 0;
	
				
						// Read IP_address of client (Network byte address)
						if( read_all(fds[i].fd, &Network_IP_address, sizeof(unsigned long) ) < 0 ){		// returns -1 if EOF reached or read return -1
						
							// check for ECONNRESET or EAGAIN and EWOULDBLOCK
							// also check if read_all returned 0 ==> reached EOF
							
							//in all of the cases ==> ERROR, client has abruptly ended connection
							
							printf("Read Error (2) \n\n");
												
							shutdown(fds[i].fd,SHUT_RDWR);					
							close(fds[i].fd);			// free the file descriptor
							
							delete[] buffer;
    						buffer = NULL;
																				
							continue;												
						}				
						
						
						if( read_all(fds[i].fd, &Network_port, sizeof(unsigned short int) ) < 0){		// Read port of client ( Network byte order)
						
							// check for ECONNRESET or EAGAIN and EWOULDBLOCK
							// also check if read_all returned 0 ==> reached EOF
							
							//in all of the cases ==> ERROR, client has abruptly ended connection
												
							shutdown(fds[i].fd,SHUT_RDWR);					
							close(fds[i].fd);			// free the file descriptor
							
    						delete[] buffer;
    						buffer = NULL;
									
							continue;					
						}
				
									
					
		    	 		shutdown(fds[i].fd,SHUT_RD);
						close(fds[i].fd);
						
						Poll_update(fds,i,fd_counter);
						fd_counter--;
				
						// network byte order ==> host byte order
						unsigned long Host_IP_address = ntohl(Network_IP_address);
						unsigned short int Host_port = ntohs(Network_port);
					
			
						/* Search if client already exists */											
						
						if(flag == 1) continue;
						
						/************************** LOCKED ********************************/
						
						int error = enterMain();				
						if( error > 0 ){
						
							printf("%s (4)\n", strerror(error) );
							return -1;

						}else if( error < 0) continue;		// SIGINT
						
				
						if( client_list->Search(Host_IP_address,Host_port) == true){
				
							printf("Client exist in our list \n");
					
							client_list->deleteNode(Host_IP_address,Host_port);	
					
							printf("\nSuccessful USER_OFF \n");
							
							int error = exitCS();						
							if( error > 0){
							
								printf("%s (5)\n", strerror(error) );
								return -1;
							}														
					
						/******************************* UNLOCKED ****************************************/														
							
    						delete[] buffer;
    						buffer = NULL;
							
							continue;
					
					
    
		    			}else{
		    			
		    				int error = exitCS();						
							if( error > 0){
							
								printf("%s (6)\n", strerror(error) );
								return -1;
							}														
					
						/******************************* UNLOCKED ****************************************/
    		
		    				printf("ERROR why is user not on our list???? (USER_OFF) \n\n");
		    				
		    				delete[] buffer;
    						buffer = NULL;
		    				
		    				
		    				continue;   		
		    			}
	
	
	
/////////////////////////////////////////////////////////
////////////////////////////////////////////////////////	
///////////////////////////////////////////////////////	
//////////////////////////////////////////////////////
/////////////////////////////////////////////////////

	
		    		}else if( strcmp(buffer,"GET_FILE_LIST") == 0){
					
						printf("Received GET_FILE_LIST\n");
						
						
						
						/* Read the IP and Port of client */
							
						unsigned long Network_IP_address = 0;
						unsigned short int Network_port = 0;
		
				
						// Read IP_address of client (Network byte address)
						if( read_all(fds[i].fd, &Network_IP_address, sizeof(unsigned long) ) < 0 ){		// returns -1 if EOF reached or read return -1
						
							// check for ECONNRESET or EAGAIN and EWOULDBLOCK
							// also check if read_all returned 0 ==> reached EOF
							
							//in all of the cases ==> ERROR, client has abruptly ended connection
																			
							shutdown(fds[i].fd,SHUT_RDWR);					
							close(fds[i].fd);			// free the file descriptor
							
							
							Poll_update(fds,i,fd_counter);
							fd_counter--;
							
							delete[] buffer;
    						buffer = NULL;
							
																			
							continue;												
						}				
						
						
						if( read_all(fds[i].fd, &Network_port, sizeof(unsigned short int) ) < 0){		// Read port of client ( Network byte order)
						
							// check for ECONNRESET or EAGAIN and EWOULDBLOCK
							// also check if read_all returned 0 ==> reached EOF
							
							//in all of the cases ==> ERROR, client has abruptly ended connection
												
							shutdown(fds[i].fd,SHUT_RDWR);					
							close(fds[i].fd);			// free the file descriptor
							
							Poll_update(fds,i,fd_counter);
							fd_counter--;
							
							delete[] buffer;
    						buffer = NULL;
									
							continue;					
						}
													
				    	shutdown(fds[i].fd,SHUT_RD);			// nothing more to read
				     		
				     		
				    	unsigned long Host_IP_address = ntohl( Network_IP_address );
						unsigned short int Host_port = ntohs( Network_port);
						
				     	
				     	if( flag == 1) continue;
				     	/************************** LOCKED ********************************/
						
						int error = enterMain();				
						if( error > 0 ){
						
							printf("%s (7)\n", strerror(error) );
							return -1;

						}else if( error < 0) continue;		// SIGINT
																										
				    		
				    	if( client_list->Search(Host_IP_address, Host_port) == false){
				     						     						    
				     		// just close the connection
				     		
				     		printf("\nThis client doesn't exist ==> ignore GET_FILE_LIST \n\n");
				     		
				     		int error = exitCS();						
							if( error > 0){
							
								printf("%s (8)\n", strerror(error) );
								return -1;
							}														
																		
					
						/******************************* UNLOCKED ****************************************/		
				     		
				     		
				     		
				     		shutdown(fds[i].fd,SHUT_WR);
				     		close(fds[i].fd);
				     		
				     		Poll_update(fds,i,fd_counter);
				     		fd_counter--;
				     		
				     		delete[] buffer;
    						buffer = NULL;				     						     						  
				     		
				    		continue;				    	
				    	}
				     		
				    	//In this case, the client does exist in our list
				    	
				     	error = exitCS();						
						if( error > 0){
							
							printf("%s (9)\n", strerror(error) );
							return -1;
						}														
					
						/******************************* UNLOCKED ****************************************/		
				     	
				     	
				     		
			   		
						//Creating message 
						int length = snprintf(NULL,0,"FILE_LIST %d", file_list->counter) + 1;
						char* message = new char[length]; 	
						snprintf(message,length,"FILE_LIST %d", file_list->counter);
				
						if( write_all(fds[i].fd,message,length) < 0){	//write the message

							// something went wrong with peer ==> close connection
								
				    		if( shutdown(fds[i].fd,SHUT_WR) < 0){
			     				perror("Error in shutdown() (15): ");		    		
    	 						printf("\n");
    	 					}	
     					
				    		close(fds[i].fd);
				     			
				    		Poll_update(fds,i,fd_counter);
				    		fd_counter--;
				    		
				    		delete[] buffer;
    						buffer = NULL;

							continue;				
						}
					
				
						printf("Sent the message, writing the files\n");
				
						/* Write all the files in the socket */
				
						Node2* temp1 = file_list->head;
				
						
						unsigned short int filename_length = 0;						
						unsigned long int timestamp = 0;
						
						char* file1 = NULL;
						
				
						while( temp1 != NULL){		// write each file
						
							length = snprintf(NULL,0,"%s/%s", dirname, temp1->pathname) + 1;
							file1 = new char[length];
							snprintf(file1,length,"%s/%s", dirname, temp1->pathname);
							
							printf("File1 is %s \n", file1);
				
				
							filename_length = htons( strlen(temp1->pathname) + 1); 
														
							// Get timestamp
							if( ( timestamp = getTimestamp(file1)	) == 0){
							
								if( shutdown(fds[i].fd,SHUT_WR) < 0){
				   					perror("Error in shutdown() (17): ");		    		
	     							printf("\n");
	     						}
     	     					
				  				close(fds[i].fd);
			     				
				   				Poll_update(fds,i,fd_counter);
				   				fd_counter--;
			     				
								break;
							}	
							timestamp = htonl(timestamp);	// Network byte order
							
													
							
				
							// Write length of filename
							if( write_all(fds[i].fd, &filename_length, sizeof(unsigned short int) ) < 0){
     						
				   				if( shutdown(fds[i].fd,SHUT_WR) < 0){
				   					perror("Error in shutdown() (17): ");		    		
	     							printf("\n");
	     						}
     	     					
				  				close(fds[i].fd);
			     				
				   				Poll_update(fds,i,fd_counter);
				   				fd_counter--;
			     				
								break;				
							}
																																		
							// Write filename
							if( write_all(fds[i].fd, temp1->pathname, strlen(temp1->pathname) + 1 ) < 0){
     						
				   				if( shutdown(fds[i].fd,SHUT_WR) < 0){
				   					perror("Error in shutdown() (18): ");		    		
	     							printf("\n");
	     						}
     	     					
				  				close(fds[i].fd);
			     				
				   				Poll_update(fds,i,fd_counter);
				   				fd_counter--;
			     				
								break;				
							}
							
							
							// Write version/timestamp of file
							if( write_all(fds[i].fd, &timestamp, sizeof(unsigned long int) ) < 0){
     						
				   				if( shutdown(fds[i].fd,SHUT_WR) < 0){
				   					perror("Error in shutdown() (19): ");		    		
	     							printf("\n");
	     						}
     	     					
				  				close(fds[i].fd);
			     				
				   				Poll_update(fds,i,fd_counter);
				   				fd_counter--;
			     				
								break;				
							}
																																													
							// Proceed to the next tuple				
				
							temp1 = temp1->next;
							
							delete[] file1;
							file1 = NULL;
							
						} // We sent everything
						
						
						if( temp1 != NULL){
	  			   			// something went wrong while sending  clients
  			   		
  			   				printf("Something went wrong in while loop\n");

  			   		
  				   			shutdown(fds[i].fd,SHUT_RDWR);
  			   			
  				   			close(fds[i].fd);
  			   			
  				   			Poll_update(fds,i,fd_counter);
  				   			fd_counter--;
  				   			
  				   			delete[] buffer;
    						buffer = NULL;
  				   			
  				   			continue;  			   		
  			   			}
  				   		
  				   		shutdown(fds[i].fd,SHUT_WR);		   		
  						close(fds[i].fd);		// free the file descriptor
 
  						Poll_update(fds,i,fd_counter);
						fd_counter--;	
						
						delete[] message;
						message = NULL;	
						
						delete[] buffer;
						buffer = NULL;	
						
						printf("\n\nGET_FILES_LIST finished succesfully \n\n");	
						
						continue;	
					
					
////////////////////////////////////////////////////////
///////////////////////////////////////////////////////
//////////////////////////////////////////////////////					
/////////////////////////////////////////////////////
////////////////////////////////////////////////////				
					
					}else if( strcmp(buffer,"GET_FILE") == 0 ){
					
					
						printf("Received GET_FILE\n");
						
						
						
						/* Read the (IP and Port of client) AND  */
							
						unsigned long Network_IP_address = 0;
						unsigned short int Network_port = 0;
						
						unsigned short int filename_length = 0;
						unsigned long int timestamp = 0;
		
				
						// Read IP_address of client (Network byte address)
						if( read_all(fds[i].fd, &Network_IP_address, sizeof(unsigned long) ) < 0 ){		// returns -1 if EOF reached or read return -1
						
							// check for ECONNRESET or EAGAIN and EWOULDBLOCK
							// also check if read_all returned 0 ==> reached EOF
							
							//in all of the cases ==> ERROR, client has abruptly ended connection
																			
							shutdown(fds[i].fd,SHUT_RDWR);					
							close(fds[i].fd);			// free the file descriptor
							
							Poll_update(fds,i,fd_counter);
							fd_counter--;
							
							delete[] buffer;
    						buffer = NULL;
																			
							continue;												
						}				
						
						
						if( read_all(fds[i].fd, &Network_port, sizeof(unsigned short int) ) < 0){		// Read port of client ( Network byte order)
						
							// check for ECONNRESET or EAGAIN and EWOULDBLOCK
							// also check if read_all returned 0 ==> reached EOF
							
							//in all of the cases ==> ERROR, client has abruptly ended connection
												
							shutdown(fds[i].fd,SHUT_RDWR);					
							close(fds[i].fd);			// free the file descriptor
							
							Poll_update(fds,i,fd_counter);
							fd_counter--;
							
							delete[] buffer;
    						buffer = NULL;
									
							continue;					
						}
						
						
						// Read length of filename/path
						if( read_all(fds[i].fd, &filename_length, sizeof(unsigned short int) ) < 0 ){		// returns -1 if EOF reached or read return -1
						
							// check for ECONNRESET or EAGAIN and EWOULDBLOCK
							// also check if read_all returned 0 ==> reached EOF
							
							//in all of the cases ==> ERROR, client has abruptly ended connection
																			
							shutdown(fds[i].fd,SHUT_RDWR);					
							close(fds[i].fd);			// free the file descriptor
							
							Poll_update(fds,i,fd_counter);
							fd_counter--;
							
							delete[] buffer;
    						buffer = NULL;
																			
							continue;												
						}
						
						// Host Byte Order and creating a buffer
						
						filename_length = ntohs(filename_length);		// host byte order
						
						char* pathname = new char[filename_length];
						
						// Read filename/path
						if( read_all(fds[i].fd, pathname, filename_length ) < 0 ){		// returns -1 if EOF reached or read return -1
						
							// check for ECONNRESET or EAGAIN and EWOULDBLOCK
							// also check if read_all returned 0 ==> reached EOF
							
							//in all of the cases ==> ERROR, client has abruptly ended connection
																			
							shutdown(fds[i].fd,SHUT_RDWR);					
							close(fds[i].fd);			// free the file descriptor
							
							Poll_update(fds,i,fd_counter);
							fd_counter--;
							
							delete[] buffer;
    						buffer = NULL;
							
							delete[] pathname;
							pathname = NULL;
																			
							continue;												
						}
						
						// Read version/timestamp
						if( read_all(fds[i].fd, &timestamp, sizeof(unsigned long int) ) < 0 ){		// returns -1 if EOF reached or read return -1
						
							// check for ECONNRESET or EAGAIN and EWOULDBLOCK
							// also check if read_all returned 0 ==> reached EOF
							
							//in all of the cases ==> ERROR, client has abruptly ended connection
																			
							shutdown(fds[i].fd,SHUT_RDWR);					
							close(fds[i].fd);			// free the file descriptor
							
							Poll_update(fds,i,fd_counter);
							fd_counter--;
							
							delete[] buffer;
    						buffer = NULL;
							
							delete[] pathname;
							pathname = NULL;
																										
							continue;												
						}
						
						timestamp = ntohl(timestamp);		// Host byte order
//						filename_length = ntohs(filename_length);		// already done
												
													
				    	shutdown(fds[i].fd,SHUT_RD);			// nothing more to read
				     		
				     		
				    	unsigned long Host_IP_address = ntohl( Network_IP_address );
						unsigned short int Host_port = ntohs( Network_port);
				     		
				     	
				     	if( flag == 1) continue;
				     	/************************** LOCKED ********************************/
						
						int error = enterMain();				
						if( error > 0 ){
						
							printf("%s (10)\n", strerror(error) );
							return -1;

						}else if( error < 0) continue;		// SIGINT
						
						
				     	/* Does client exist in our client_list */	
				    		
				    	if( client_list->Search(Host_IP_address, Host_port) == false){
				    	
				    		int error = exitCS();						
							if( error > 0){
							
								printf("%s (11)\n", strerror(error) );
								return -1;
							}																
					
						/******************************* UNLOCKED ****************************************/
				     		
				     		
				     		// just close the connection
				     		
				     		printf("\nThis client doesn't exist ==> ignore GET_FILE \n\n");
				     		
				     		shutdown(fds[i].fd,SHUT_WR);
				     		close(fds[i].fd);
				     		
				     		Poll_update(fds,i,fd_counter);
				     		fd_counter--;
				     		
				     		delete[] buffer;
    						buffer = NULL;
							
							delete[] pathname;
							pathname = NULL;
				     						     		
				    		continue;				    	
				    	}
				     		
				    	//In this case, the client does exist in our list
				    	
				     	error = exitCS();						
						if( error > 0){
							
							printf("%s (12)\n", strerror(error) );
							return -1;
						}																	
					
						/******************************* UNLOCKED ****************************************/
				     		
				     	
				     	
				     	/* Search if file exists */	

						length = snprintf(NULL,0,"./%s/%s", dirname, pathname) + 1;
						char* file1 = new char[length];
						snprintf(file1,length,"./%s/%s",dirname,pathname);
				     	
				     	
//				     	if( file_list->Search(pathname) == false){		// Requested file doesn't exist
	
						if( access(file1,F_OK) == -1){		// Requested file doesn't exist
				     	
				     		//Creating message 
							int length1 = snprintf(NULL,0,"FILE_NOT_FOUND") + 1;
							char* file_not_found = new char[length1]; 	
							snprintf(file_not_found,length1,"FILE_NOT_FOUND");
				     		
				     	
				     		// Write message
							if( write_all(fds[i].fd, file_not_found, length1 ) < 0){
     						
				   				if( shutdown(fds[i].fd,SHUT_WR) < 0){
				   					perror("Error in shutdown() (18): ");		    		
	     							printf("\n");
	     						}
     	     					
				  				close(fds[i].fd);
			     				
				   				Poll_update(fds,i,fd_counter);
				   				fd_counter--;
				   				
				   				delete[] file_not_found;
				   				file_not_found = NULL;
				   				
								delete[] buffer;
	    						buffer = NULL;
							
								delete[] pathname;
								pathname = NULL;
								
								delete[] file1;
								file1 = NULL;				   				
			     				
								continue;				
							}
							
							shutdown(fds[i].fd, SHUT_WR);		// nothing more to send ==> close socket
							close(fds[i].fd);
							
							Poll_update(fds,i,fd_counter);
							fd_counter--;
							
							delete[] file_not_found;
							file_not_found = NULL;	
							
							delete[] buffer;
    						buffer = NULL;
							
							delete[] pathname;
							pathname = NULL;
							
							delete[] file1;
							file1 = NULL;
													
							
							continue;		// continue to fds[i+1]											     	
				     	}
				     	
				     	
				     	
				     	/* Check timstamps */	
				     	
//				     	unsigned long current_timestamp = getTimestamp(pathname);
	
				     	unsigned long current_timestamp = getTimestamp(file1);									     	
				     	if( current_timestamp == 0){
				     	
				     		if( shutdown(fds[i].fd,SHUT_WR) < 0){
				   				perror("Error in shutdown() (18): ");		    		
	     						printf("\n");
	     					}
     	     					
				  			close(fds[i].fd);
			     				
				   			Poll_update(fds,i,fd_counter);
				   			fd_counter--;
				   			
				   			delete[] buffer;
    						buffer = NULL;
							
							delete[] pathname;
							pathname = NULL;
							
							delete[] file1;
							file1 = NULL;								   			
			     				
							continue;			     	
				     	}
				     	
				     	
				     	
				     	if( timestamp == current_timestamp){		// File up to date
				     	
				     		//Creating message 
							int length1 = snprintf(NULL,0,"FILE_UP_TO_DATE") + 1;
							char* file_ok = new char[length1]; 	
							snprintf(file_ok,length1,"FILE_UP_TO_DATE");
				     		
				     	
				     		// Write message
							if( write_all(fds[i].fd, file_ok, length1 ) < 0){
     						
				   				if( shutdown(fds[i].fd,SHUT_WR) < 0){
				   					perror("Error in shutdown() (18): ");		    		
	     							printf("\n");
	     						}
     	     					
				  				close(fds[i].fd);
			     				
				   				Poll_update(fds,i,fd_counter);
				   				fd_counter--;
	
								delete[] file_ok;
								file_ok = NULL;
				   				
				   				
				   				delete[] buffer;
	    						buffer = NULL;
							
								delete[] pathname;
								pathname = NULL;	
								
								delete[] file1;
								file1 = NULL;			   				
			     				
								continue;				
							}
							
							shutdown(fds[i].fd, SHUT_WR);		// nothing more to send ==> close socket
							close(fds[i].fd);
							
							Poll_update(fds,i,fd_counter);
							fd_counter--;
							
							delete[] file_ok;
							file_ok = NULL;	
							
							delete[] buffer;
    						buffer = NULL;
							
							delete[] pathname;
							pathname = NULL;
							
							delete[] file1;
							file1 = NULL;												
							
							continue;		// continue to fds[i+1]											     	
				     	}
				     	
				     	
				     	/* Different timestamps ==> send file */
				     	
				     	//Creating message 
						int length1 = snprintf(NULL,0,"FILE_SIZE") + 1;
						char* file_SIZE = new char[length1]; 	
						snprintf(file_SIZE,length1,"FILE_SIZE");
				     	
				     	
				
						if( write_all(fds[i].fd,file_SIZE,length1) < 0){		//write the message

							// something went wrong with peer ==> close connection
								
				    		if( shutdown(fds[i].fd,SHUT_WR) < 0){
			     				perror("Error in shutdown() (15): ");		    		
    	 						printf("\n");
    	 					}	
     					
				    		close(fds[i].fd);
				     			
				    		Poll_update(fds,i,fd_counter);
				    		fd_counter--;
				    		
				    		delete[] file_SIZE;
				    		file_SIZE = NULL;
				    		
				    		delete[] buffer;
    						buffer = NULL;
							
							delete[] pathname;
							pathname = NULL;
							
							delete[] file1;
							file1 = NULL;		    		

							continue;				
						}
					
						// Sending version/timestamp
						
						current_timestamp = htonl(current_timestamp);	// Network byte order						

						if( write_all(fds[i].fd, &current_timestamp, sizeof(unsigned long int) ) < 0){
     						
				   			if( shutdown(fds[i].fd,SHUT_WR) < 0){
				   				perror("Error in shutdown() (19): ");		    		
	     						printf("\n");
	     					}
     	     					
				  			close(fds[i].fd);
			     				
				   			Poll_update(fds,i,fd_counter);
				   			fd_counter--;
				   			
				   			
				   			delete[] file_SIZE;
				    		file_SIZE = NULL;
				    		
				    		delete[] buffer;
    						buffer = NULL;
							
							delete[] pathname;
							pathname = NULL;
							
							delete[] file1;
							file1 = NULL;			   			
			     				
							continue;				
						}
						
						// Sending file Size (in bytes)
						
//						unsigned long int file_size = getFileSize(pathname);

						unsigned long int file_size = getFileSize(file1);

						unsigned long int N_file_size = htonl(file_size);

						char* buffer1 = new char[file_size];					//buffer which will contain contents of file
						
//						file_size = htonl(file_size);	// network byte order
						
						if( write_all(fds[i].fd, &N_file_size, sizeof(unsigned long int) ) < 0){
     						
				   			if( shutdown(fds[i].fd,SHUT_WR) < 0){
				   				perror("Error in shutdown() (19): ");		    		
	     						printf("\n");
	     					}
     	     					
				  			close(fds[i].fd);
			     				
				   			Poll_update(fds,i,fd_counter);
				   			fd_counter--;
				   			
				   			delete[] file_SIZE;
				    		file_SIZE = NULL;
				    		
				    		delete[] buffer;
    						buffer = NULL;
							
							delete[] pathname;
							pathname = NULL;
							

							delete[] file1;
							file1 = NULL;
				   				     				
							continue;				
						}									
						
						// Opening File and sending bytes
						
//						int fd_file = open(pathname,O_RDONLY);
	
						int fd_file = open(file1,O_RDONLY);
						if( fd_file < 0){
							
							if( shutdown(fds[i].fd,SHUT_WR) < 0){
				   				perror("Error in shutdown() (19): ");		    		
	     						printf("\n");
	     					}
     	     					
				  			close(fds[i].fd);
			     				
				   			Poll_update(fds,i,fd_counter);
				   			fd_counter--;
				   			
				   			delete[] buffer1;
				   			buffer1 = NULL;
				   			
				   			delete[] file_SIZE;
				    		file_SIZE = NULL;
				    		
				    		delete[] buffer;
    						buffer = NULL;
							
							delete[] pathname;
							pathname = NULL;
							
							delete[] file1;
							file1 = NULL;											   							   			
			     				
							continue;					
						}
						
						if( read_all(fd_file,buffer1,file_size) < 0){		// read contents of file in buffer
						
							if( shutdown(fds[i].fd,SHUT_WR) < 0){
				   				perror("Error in shutdown() (19): ");		    		
	     						printf("\n");
	     					}
	     					
	     					close(fd_file);
     	     					
				  			close(fds[i].fd);
			     				
				   			Poll_update(fds,i,fd_counter);
				   			fd_counter--;
				   			
				   			
				   			delete[] buffer1;
				   			buffer1 = NULL;
				   			
				   			delete[] file_SIZE;
				    		file_SIZE = NULL;
				    		
				    		delete[] buffer;
    						buffer = NULL;
							
							delete[] pathname;
							pathname = NULL;
							
							delete[] file1;
							file1 = NULL;						   							   							   			
			     				
							continue;					
						}
						
						close(fd_file);		// close file
						
						
						// Send contents of file
						
						if( write_all(fds[i].fd,buffer1,file_size) < 0){		//write the message

							// something went wrong with peer ==> close connection
								
				    		if( shutdown(fds[i].fd,SHUT_WR) < 0){
			     				perror("Error in shutdown() (15): ");		    		
    	 						printf("\n");
    	 					}	
     					
				    		close(fds[i].fd);
				     			
				    		Poll_update(fds,i,fd_counter);
				    		fd_counter--;
				    		
				    		
				    		delete[] buffer1;
				   			buffer1 = NULL;
				   			
				   			delete[] file_SIZE;
				    		file_SIZE = NULL;
				    		
				    		delete[] buffer;
    						buffer = NULL;
							
							delete[] pathname;
							pathname = NULL;
							
							delete[] file1;
							file1 = NULL;											    						    						    		

							continue;				
						}
						
						shutdown(fds[i].fd,SHUT_WR);
						close(fds[i].fd);
						
						Poll_update(fds,i,fd_counter);
						fd_counter--;				
						
						
						
						delete[] buffer;
						buffer = NULL;
						
						delete[] pathname;
						pathname = NULL;
						
						delete[] file_SIZE;
						file_SIZE = NULL;
						
						delete[] buffer1;
						buffer1 = NULL;
						
						delete[] file1;
						file1 = NULL;						
														
						printf("GET_FILE finished succesfully \n");
					
						continue;				
					
					}else{	// Gibberish
					
						printf("Don't recognise this string : %s \n", buffer);
						
						shutdown(fds[i].fd,SHUT_WR);
						shutdown(fds[i].fd,SHUT_RD);
						
						close(fds[i].fd);
						
						Poll_update(fds,i,fd_counter);
						fd_counter--;	
						
						delete[] buffer;
						buffer = NULL;
						
						continue;				
					}
				
				}	// end if POLLIN	
		
			}	// end for loop
		    
		}	// end of ( retval > 0) if clause) 
    	
    }	// end of while loop
    
    
    
    /************************************************** LOG_OFF ****************************************************************/
    
    printf("\nLOG_OFF\n");
    
    length = strlen("LOG_OFF") + 1;
    char* log_off = new char[length];
    strcpy(log_off,"LOG_OFF");
    
    
    if( (client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){		// create a socket again

        perror("Error in socket() (4) : ");
        
    	serverptr = NULL;
    	clientptr = NULL;
    	
    	delete[] log_off;
    	log_off = NULL;
        
		return -1;
    }
    
    
    
    /* Configuring (active socket) client address struct */
    client.sin_family = AF_INET;
    inet_pton(AF_INET, IP_server, &(client.sin_addr) );
    client.sin_port = htons(server_port);
       
    
    if (connect(client_socket, clientptr, sizeof(client)) < 0){		// block untill server responds
		perror("Error in connect() (3) :");  
		
		close(client_socket);
		close(server_socket);
		
    	delete[] log_off;
    	log_off = NULL;
		
    	return -1;
    }
    
    
    // Send the message 
    
    if( write_all( client_socket, log_off, length) < 0){	// server closed the connection?
    
    	close(client_socket);
    	close(server_socket);
    	
    	delete[] log_off;
    	log_off = NULL;
    	
    	
    	return -1;   
    }
    
    /* Send the network_IP and Network_port */
    
    
    if( write_all( client_socket, &Network_IP, sizeof(unsigned long) ) < 0){	// server closed the connection?
    
    	close(client_socket);
    	close(server_socket);
    	
    	delete[] log_off;
    	log_off = NULL;
    	
    	
    	return -1;   
    }
    
    if( write_all(client_socket, &Network_port, sizeof(unsigned short int) ) < 0 ){
    
    	close(client_socket);
    	close(server_socket);
    	
    	delete[] log_off;
    	log_off = NULL;
    	
    	
    	return -1;    
    }


	shutdown(client_socket,SHUT_WR);		// nothing more to send
	
	length = strlen("ERROR_IP_PORT_NOT_FOUND") + 1;
    char* error_log_off = new char[length];
    
    bytes_read = 0;


	bytes_read = read(client_socket,error_log_off,length);		// block untill we read something
	
	printf("Bytes read is %d\n", bytes_read);
	
	if( bytes_read == length){	// sent FIN_packet
	
		printf("\n\n ERROR_IP_PORT_NOT_FOUND (LOG_OFF) \n\n");	

	}
	
	/*
	else if( bytes_read > 0){		// > 0 but < length
	
		printf("Probably gibberish were sent\n\n");
	}
	*/
  
  
	shutdown(client_socket, SHUT_RD);  
    close(client_socket);
    
    delete[] log_off;
    log_off = NULL;
    


	/* Pthread join */
	
	printf("\n\nMain thread calling broacast to awaken remaining threads\n");
	
	pthread_cond_broadcast(&condition);		// waking up any remaining threads
	
	printf("Main thread arrived at pthread_join() \n\n");	
	
	for( i = 0; i < num_threads; i++){
	
		pthread_join( thread_array[i] ,NULL);		
	}

	printf("\nAll threads have exited\n");
	
	/* Destroying mutex and condition variable (all the other threads have exited) */
	
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&condition);	
    
    
    printf("Exiting...\n");
    
    shutdown(server_socket, SHUT_RDWR);
    close(server_socket);
    
    delete[] thread_array;
	thread_array = NULL;
    
    delete client_list;
    client_list = NULL;
    
    delete[] dirname;
    dirname = NULL;
    
    delete[] IP_server;
    IP_server = NULL;    
    
 	return 0;
    
} 



































