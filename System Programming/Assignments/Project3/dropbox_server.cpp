
#define _GNU_SOURCE


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

#include <poll.h>

#include <errno.h>

#include <string.h>

#include "header.hpp"



/* IMPORTANT

	I assume that: a) IP --> unsigned long
				   b) port --> short int

*/


/*
	IP and port : 

	a) In structs sockaddr_in--> network byte order

	b) when we use them ---> big endian (host byte order)
	
	AKA, when a terminal uses them (outside of struct) ==> ntohl\s()  


*/



/* USE POLL() and realloc() to increase size of array !!!!!!!!!!!!!! */
















//volatile sig_atomic_t flag = 0;

int flag1 = 0;


void Quit(int sig){

	flag1 = 1; 
}









int main(int argc, char* argv[]){

	if( argc != 3){
	
		printf("Please enter the port number: '-p Port_number' \n");
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
	sa1.sa_handler = SIG_IGN;	// ignore this signal
	sigemptyset(&sa1.sa_mask);	//block all signals
	sa1.sa_flags = SA_RESTART;	// retry system calls		
			
		
	if( sigaction(SIGPIPE,&sa1,NULL) == -1){
	
		perror("Sigaction error (2): ");
		return -1;	
	}




	int port = atoi(argv[2]);		// Server port number


	// server_socket is our listening socket
	int server_socket = -1, client_socket = -1;		// client_socket will be the active socket ==> sends or receives bytes
	
	
	
	struct sockaddr_in server, client;
	
    socklen_t clientlen = 0;
    
    struct sockaddr *serverptr=(struct sockaddr *)&server;
    struct sockaddr *clientptr=(struct sockaddr *)&client;
    
  
    
    /* Printing IP of machine */
    
    char hostname[100];
    struct hostent *host_entry = NULL;
    char IP_str[INET_ADDRSTRLEN];
    
    unsigned long this_IP = 0;
    
    
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

    this_IP = inet_addr(IP_str);						// NETWORK BYTE ORDER
    
    printf("\n\nServer IP is %s\n\n", IP_str);
    
    
    
    /* Create a (passive) socket ===> our listening socket */
    
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){

        perror("Error in socket() ");
        
        serverptr = NULL;
        clientptr = NULL;
        
        return -1;
    }    
    
    
    /* Configuring socket ==> SO_REUSEADDR */   
    int enable = 1;   
    if( setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int) ) < 0 ){
    
    	perror("Error in setsockopt() (1) : ");
    	
    	close(server_socket);
    	return -1;   
    }  
       
       
        
    /* Configuring our server address struct */    
    server.sin_family = AF_INET;       /* Internet domain */
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);      /* The given port */
    

    /* Bind socket to address ==> give the socket a "name" -> IP/port */
    if (bind(server_socket, serverptr, sizeof(server)) < 0){

        perror("Error in bind() ");
        
        close(server_socket);
        
        serverptr = NULL;
        clientptr = NULL;
        
        return -1;
     }
     
     
	/* Now we are ready to accept our clients */
	if (listen(server_socket, 5) < 0){		// socket is now a passive/server socket 
	
     	perror("Error in listen() ");
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
	
	printf("\n\nServer port is %d\n", ntohs(sin.sin_port) );
	printf("Server port is %d (network byte order)\n\n", sin.sin_port );
	
	


	/* Creating the client_list and job list */
	
	List* client_list = new List();
	
	List1* job_list = new List1();
	
	
	/* Creating an array of struct pollfd */
	
	int size = 50;   
    struct pollfd* fds = new struct pollfd[size];
    int fd_counter = 0;
    int timeout = 0;				// how long to wait, for any changes in our fds
    
    int i = 0;
    for( i = 0; i < size; i++){
    
    	fds[i].fd = -1;			// INVALID value ==> ignore
    	fds[i].events = 0;
    	fds[i].revents = 0;
    }
    
    
    
    
    /* Insert listening socket into array */
    
    fds[0].fd = server_socket;
    fds[0].events = POLLIN;
    
    fd_counter++;		// increasing counter
    
    
    
    
    /* Until we receive a SIGINT */     
    
    int retval = 0;
     
    while( flag1 != 1){		// SIGINT flag
    
    	retval = 0;
     
		clientlen = sizeof(client);
		
		printf("\n\nArrived at poll() \n\n");
		printf("fd_counter is %d \n", fd_counter);
		
		
		
		//Printing fds
		Poll_Print(fds,fd_counter);
		
		
		
		retval = poll(fds,size,-1);		// block untill an event pops up
		if( retval < 0 ){
		
			if( errno == EINTR) continue;	// interrupted by SIGINT  (change value of flag from 0 --> 1 )
		
			perror("Error in poll() ");
			
			close(server_socket);
			
			serverptr = NULL;
			clientptr = NULL;
			
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
			
				if( flag1 == 1) break;
			
			
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
//					return -1;
	
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
					
					if( strcmp(buffer,"LOG_ON") == 0){
					
						printf("Received LOG_ON \n");
			
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
							
							printf("Read Error (1) \n\n");
												
							shutdown(fds[i].fd,SHUT_RDWR);					
							close(fds[i].fd);			// free the file descriptor
												
							/* Switch last fds of pollfd with the fds[i] */
						
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
												
							/* Switch last fds of pollfd with the fds[i] */
						
							Poll_update(fds,i,fd_counter);
							fd_counter--;	
							
							delete[] buffer;
							buffer = NULL;	
									
							continue;					
						}
				

			     		shutdown(fds[i].fd,SHUT_RD);
						close(fds[i].fd);
				
						Poll_update(fds,i,fd_counter);	// removing the fd and putting the last fd in its place
						fd_counter--;
				
							
						// network byte order ==> host byte order
						unsigned long Host_IP_address = ntohl(Network_IP_address);
						unsigned short int Host_port = ntohs(Network_port);
						
						printf("Host port is %d and network port %d\n", Host_port, Network_port);
				
			
						/* Search if client already exists */
			
						if( client_list->Search(Host_IP_address,Host_port) == false){
				
							printf("Client doesn't exist in our list \n");
					
							client_list->AddNode(Host_IP_address,Host_port);
					
							/* inform the rest of clients about the new user */
				
							//create the message
							int length = snprintf(NULL,0,"USER_ON") + 1;
							char* message = new char[length]; 	
							snprintf(message,length,"USER_ON");
					
							// create sockets waiting to connect , put them in the List1, POLLOUT 
							
							Node* temp = client_list->head;
							
							while( temp != NULL){		// To each user (currently on the system) , send a USER_ON <Network_IP_address,Network_port>
							
								clientlen = sizeof(client);
							
							
								if( (temp->IP == Host_IP_address) && ( temp->port == Host_port) ){	// ignore this client( won't send to himself)
								
									temp = temp->next;									
									continue;	
								}
							

								/* 	1) Create a socket, 
									2) set it into O_NONBLOCK
									3) Connect with client
										a) if it connects ==> great! send the message
										
										b) if it doesn't (returns -1) ==> check the error:
										
											i) EINPROGRESS ==> the "connection" is in progress and you will be informed when it's ready ===> don't connect 						
							
											ii) EAGAIN/EWOULDBLOCK ==> not enough ports, check again later ==> Connect() again							
								*/
							
							
							
								// Creating socket and setting into O_NONBLOCK
    
							    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){

							        perror("Error in socket() (2) ");
							        printf("\n");
        
        							temp = temp->next;
							        continue;
							    }    
    
    
							    /* Configuring socket ==> SO_REUSEADDR */   
							    int enable = 1;   
							    if( setsockopt(client_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int) ) < 0 ){
    
							    	perror("Error in setsockopt() (3) : ");
							    	printf("\n");
    	
							    	close(client_socket);
							    	temp = temp->next;
							    	
							    	continue;   
							    } 
							    
							   	if( set_NONBLOCK(client_socket) == -1){
	
									perror("Error in set_NONBLOCK() (5) : ");
									printf("\n");
     		
							     	close(client_socket);
     								        	
    							    temp = temp->next;
    							    continue;
								}
							    
						        /* Configuring our client address struct */    
							    client.sin_family = AF_INET;       /* Internet domain */
							    client.sin_addr.s_addr = htonl(temp->IP);
							    client.sin_port = htons(temp->port);      							    
								
															     
							    // Now try to connect with clients
							     
							     int k = 0;
							     
							     printf("Connecting to IP:%ld , Port: %d \n\n", temp->IP, temp->port);
							     
							     clientlen = sizeof(client);
							     
							     k = connect(client_socket,clientptr,clientlen);
							     if( k < 0){		// an error has occured
							     
							     	
							     	perror("Error in connect (1) : ");
							     	
							     	if( errno == EINPROGRESS){
							     	
							     		// Our "Conection" is in progress ==> POLLOUT when ready
							     							  							     							 							     							     		
//							     		job_list->AddNode1(client_socket, message, Network_IP_address, Network_port, temp->IP, temp->port); 		
	
							     		job_list->AddNode1(client_socket, message, Host_IP_address, Host_port, temp->IP, temp->port); 			
							     		
							     		 // Updating poll fd ==> adding the socket to the list
							     		
							     		fd_counter++;
		     					    	if( fd_counter > size) Poll_Increase_Size(&fds,&size);
							     		
							     		
							     		fds[fd_counter-1].fd = client_socket;
							     		fds[fd_counter-1].events = POLLOUT;
							     		fds[fd_counter-1].revents = 0;
							     		
							     		printf("Created a PENDING CONNECTION USER_ON with fd_counter %d \n\n", fd_counter);
							     		
							     		
							     		printf("\n\nPENDING CLIENT SOCKET %d USER_ON\n",client_socket);
							     		
							     		client_socket = -1;	//resetting its value
							     				     		
							     		temp = temp->next;						     		 							     	
							     		continue;						     	
							     	}
							     	
							     	// this might happen if we have ECONNRESET(other client closed) or some general error
							     	
							     	perror("Error in connect (1) : ");
							     	
							     	close(client_socket);
							     	
							     	temp = temp->next;							     							     	
							     	continue;						     	     							     
							     }
							     
							    // In this case, connect was successful
							    
							    
							    
							    printf("\n\nConnection was successful!!!!!!!!!!!!!! \n\n");
													
								if( write_all(client_socket,message,length) < 0){		// sending "USER_ON"
								
									// something is wrong with peer ==> close connection
							
									printf("Write Error (1)\n");
								
									shutdown(client_socket,SHUT_RDWR);
								
									close(client_socket);
									
									temp = temp->next;									
									continue;							
								}
							
							
								if( write_all(client_socket,&Network_IP_address,sizeof(unsigned long) ) < 0 ){	// sending IP_address of new user
								
									// something is wrong with peer ==> close connection
									
									printf("Write Error (2)\n");
									
									shutdown(client_socket,SHUT_RDWR);
								
									close(client_socket);
									
									temp = temp->next;									
		
									continue;							
								}
							
							
								if( write_all(client_socket,&Network_port, sizeof(unsigned short int) ) < 0){		// sending port of new user
								
									// something is wrong with peer ==> close connection
									printf("Write Error (3)\n");
								
									shutdown(client_socket,SHUT_RDWR);
								
									close(client_socket);
									
									temp = temp->next;													



									continue;							
								}
							
								// We have finished sending data => send FIN packet and close connection
								
								shutdown(client_socket,SHUT_WR);		// no longer writes
								
								shutdown(client_socket, SHUT_RD);		// no longer Reads
								
								close(client_socket);
								
								printf("\n\nFinished pending connection USER_ON \n\n");

								
								temp = temp->next;
																	
							}	// end of while loop (temp)
							
							
							// finished sending ( or creating pending connections) to the other clients
							
							printf("Successfull LOG_ON!!!!!\n\n");	
					
							delete[] message;
							message = NULL;
											
				
						}else{
					
							printf("User already exists ==> don't send USER_ON \n");				
						}
												
//////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
				
					}else if(strcmp(buffer,"LOG_OFF") == 0){

						printf("Received LOG_OFF \n");
			
						/* Read the IP and Port */
							
						unsigned long Network_IP_address = 0;
						unsigned short int Network_port = 0;


						if( read_all(fds[i].fd, &Network_IP_address, sizeof(unsigned long) ) < 0 ){		// returns -1 if EOF reached or read return -1
						
							printf("ERROR in read LOG_OFF (1) \n");
						
							// check for ECONNRESET or EAGAIN and EWOULDBLOCK
							// also check if read_all returned 0 ==> reached EOF
							
							//in all of the cases ==> ERROR, client has abruptly ended connection
												
							shutdown(fds[i].fd,SHUT_RDWR);					
							close(fds[i].fd);			// free the file descriptor
												
							/* Switch last fds of pollfd with the fds[i] */
						
							Poll_update(fds,i,fd_counter);
							fd_counter--;
							
							delete[] buffer;
							buffer = NULL;	
								
									
							continue;												
						}				
						
						
						if( read_all(fds[i].fd, &Network_port, sizeof(unsigned short int) ) < 0){
						
							printf("ERROR in read LOG_OFF (2) \n");
						
							// check for ECONNRESET or EAGAIN and EWOULDBLOCK
							// also check if read_all returned 0 ==> reached EOF
							
							//in all of the cases ==> ERROR, client has abruptly ended connection
												
							shutdown(fds[i].fd,SHUT_RDWR);					
							close(fds[i].fd);			// free the file descriptor
												
							/* Switch last fds of pollfd with the fds[i] */
						
							Poll_update(fds,i,fd_counter);
							fd_counter--;
							
							delete[] buffer;
							buffer = NULL;	
								
									
							continue;					
						}	
				
						shutdown(fds[i].fd,SHUT_RD);
											
						// network byte order ==> host byte order
						unsigned long Host_IP_address = ntohl(Network_IP_address);
						unsigned short int Host_port = ntohs(Network_port);
				
				
			
						/* Search if client exists */
			
						if( client_list->Search(Host_IP_address,Host_port) == true){
				
							// Nothing else to send => shutdown WR_end and close client_socket
				
	   						
	   						shutdown(fds[i].fd,SHUT_WR);	
							close(fds[i].fd);
					
							/* Updating our pollfd */
							Poll_update(fds,i,fd_counter);
							fd_counter--;
							
								
							printf("Client exist in our list (LOG_OFF) ===> Deleting.. \n");					
							client_list->deleteNode(Host_IP_address,Host_port);
					
							/* inform the rest of clients about the leaving user */
				
							//create the message
							int length = snprintf(NULL,0,"USER_OFF") + 1;
							char* message = new char[length]; 	
							snprintf(message,length,"USER_OFF");
							
							
							/* Creating connections for USER_OFF */
							
							Node* temp = client_list->head;
							
							printf("Before while loop \n\n");
							
							while( temp != NULL){
							
								clientlen = sizeof(client);
							
								if( (temp->IP == Host_IP_address) && ( temp->port == Host_port) ){	// ignore this client( won't send to himself)
								
									temp = temp->next;									
									continue;
								}
							

								/* 	1) Create a socket, 
									2) set it into O_NONBLOCK
									3) Connect with client
										a) if it connects ==> great! send the message
										
										b) if it doesn't (returns -1) ==> check the error:
										
											i) EINPROGRESS ==> the "connection" is in progress and you will be informed when it's ready ===> don't connect 						
							
											ii) EAGAIN/EWOULDBLOCK ==> not enough ports, check again later ==> Connect() again							
								*/
							
							
							
								// Creating socket and setting into O_NONBLOCK
    
							    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){

							        perror("Error in socket() (3) ");
							        printf("\n");
							        
							        temp = temp->next;
        
							        continue;
							    }    
    
    
							    /* Configuring socket ==> SO_REUSEADDR */   
							    int enable = 1;   
							    if( setsockopt(client_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int) ) < 0 ){
    
							    	perror("Error in setsockopt() (3) : ");
    								printf("\n");
    	
							    	close(client_socket);
							    	
							    	temp = temp->next;
							    	
							    	continue;   
							    } 
							    
							   	if( set_NONBLOCK(client_socket) == -1){
	
									perror("Error in set_NONBLOCK() (5) : ");   
									printf("\n");  		
     		     		
							     	close(client_socket);
							     	
							     	temp = temp->next;
						        	
    							    continue;
								}
							    
						        /* Configuring our client address struct */    
							    client.sin_family = AF_INET;       /* Internet domain */
							    client.sin_addr.s_addr = htonl(temp->IP);
							    client.sin_port = htons(temp->port);      							    
							     
							    // Now try to connect with clients
							     
							     int k = 0;
							     
							     k = connect(client_socket,clientptr,clientlen);		//trying to connect
							     if( k < 0){		// an error has occured
							     	
							     	
//							     	if( (errno == EINPROGRESS) || (errno == EWOULDBLOCK) ){
									
									if( errno == EINPROGRESS ){
							     	
							     		// Our "Conection" is in progress ==> POLLOUT when ready
							     							     		
//							     		job_list->AddNode1(client_socket, message, Network_IP_address, Network_port);
							     		
//							     		job_list->AddNode1(client_socket, message, Network_IP_address, Network_port, temp->IP, temp->port);
							     		
 									     		 		
							     		job_list->AddNode1(client_socket, message, Host_IP_address, Host_port, temp->IP, temp->port); 									     		 									     		
							     		 // Updating poll fd ==> adding the socket to the list
							     		
							     		fd_counter++;
		     					    	if( fd_counter > size) Poll_Increase_Size(&fds,&size);
							     		
							     		fds[fd_counter-1].fd = client_socket;
							     		fds[fd_counter-1].events = POLLOUT;
							     		fds[fd_counter-1].revents = 0;
							     		
							     		printf("Created a PENDING CONNECTION USER_OFF with fd_counter %d \n\n", fd_counter);
							     		
							     		printf("\n\nPENDING CLIENT SOCKET %d USER_OFF\n",client_socket);
							     		
							     		temp = temp->next;

							     		 							     	
							     		continue;						     	
							     	}
							     	
							     	// this might happen if we have ECONNRESET(other client closed) or some general error
							     	
							     	perror("Error in connect (2) : ");
							     	printf("\n");
							     	
							     	close(client_socket);
							     	
							     	temp = temp->next;
							     							     	
							     	continue;						     	     							     
							     
							     }	// end of (k < 0)  if clause
							     
							    // In this case, connect was successful
							    
							    printf("\n\nConnection was successful (1) !!!!!!!!!!!!!! \n\n");

							
								if( write_all(client_socket,message,length) < 0){
								
									// something is wrong with peer ==> close connection
									
									printf("Write Error (4)\n");
									
									shutdown(client_socket,SHUT_RDWR);
								
									close(client_socket);
									
									temp = temp->next;
									
									

									
									continue;							
								}
							
							
								if( write_all(client_socket,&Network_IP_address,sizeof(unsigned long) ) < 0){
								
									// something is wrong with peer ==> close connection
									
									printf("Write Error (5) \n");
									
									shutdown(client_socket,SHUT_RDWR);
								
									close(client_socket);
									
									temp = temp->next;
									

																		
									continue;							
								}
							
							
								if( write_all(client_socket,&Network_port, sizeof(unsigned short int) ) < 0){
								
									// something is wrong with peer ==> close connection
									
									printf("Write Error (6)\n");
									
									shutdown(client_socket,SHUT_RDWR);
								
									close(client_socket);
									
									temp = temp->next;
									

													
									continue;							
								}
							
								// We have finished sending data => send FIN packet and close connection
								
								shutdown(client_socket,SHUT_WR);		// no longer writes
								
								shutdown(client_socket, SHUT_RD);		// no longer Reads
								
								close(client_socket);
								
								printf("\n\nFinished pending connection USER_OFF \n\n");

																
								
								temp = temp->next;
																
							}	// end of while loop
							
							
							// finished sending ( or creating pending connections) to the other clients
							
							//client_list->Broadcast( message, Network_IP_address, Network_port);
							//Moving on to the next connection 
				
							printf("Successfull LOG_OFF!!!!!\n\n");
					
							delete[] message;
							message = NULL;
							
							
				
						}else{	// user doesn't exist
					
							printf("User doesn't exist ==> send back ERROR_IP_PORT_NOT_FOUND_IN_LIST\n");
					
					
							//create the message
							int length = snprintf(NULL,0,"ERROR_IP_PORT_NOT_FOUND_IN_LIST") + 1;
							char* message = new char[length]; 	
							snprintf(message,length,"ERROR_IP_PORT_NOT_FOUND_IN_LIST");
					
					
							if( write_all(fds[i].fd,message,length) < 0 ){
							// Something is wrong with peer
						
								shutdown( fds[i].fd, SHUT_RDWR);							
								close(fds[i].fd);
								
								Poll_update(fds,i,fd_counter);
								fd_counter--;								
							
								continue;
							}
					
    		 				shutdown(fds[i].fd,SHUT_WR);
							close(fds[i].fd);
							
							// Updating Poll
							
							Poll_update(fds,i,fd_counter);
							fd_counter--;
							
					
							printf("ERROR_IP_PORT_NOT_FOUND_IN_LIST : Successfully sent!! \n");
												
							delete[] message;
							message = NULL;
														    	
	    				}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	

	
	    			}else if( strcmp(buffer,"GET_CLIENTS") == 0){
					
						printf("Received GET_CLIENTS\n");
						

			     		shutdown(fds[i].fd,SHUT_RD);
			     		
			     		printf("Wait a bit\n");
			     		

				
				
						//Creating message 
						int length = snprintf(NULL,0,"CLIENT_LIST %d", client_list->counter) + 1;
						char* message = new char[length]; 	
						snprintf(message,length,"CLIENT_LIST %d", client_list->counter);
				
						if( write_all(fds[i].fd,message,length) < 0){

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
					
				
						printf("Sent the message, writing the clients\n");

				
						/* Write all the clients in the socket */
				
						Node* temp = client_list->head;
				
						unsigned long Network_IP = 0;
						unsigned short int Network_port = 0;
				
				
						while( temp != NULL){
				
							Network_IP = htonl(temp->IP);
							Network_port= htons(temp->port);			
				
							// Write IP
							if( write_all(fds[i].fd, &Network_IP, sizeof(unsigned long)) < 0){
     						
			     				if( shutdown(fds[i].fd,SHUT_WR) < 0){
			     					perror("Error in shutdown() (17): ");		    		
     								printf("\n");
     							}
     	     					
			     				close(fds[i].fd);
			     				
			     				Poll_update(fds,i,fd_counter);
			     				fd_counter--;
			     				
								break;				
							}
				
							// Write Port
							if( write_all(fds[i].fd, &Network_port, sizeof(unsigned short)) < 0){
				    						
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
				
							temp = temp->next;
							
						} // We sent everything
						
						
						if( temp != NULL){
	  			   			// something went wrong while sending  clients
  			   		
  			   				printf("Something went wrong in while loop\n");

  			   		
  				   			shutdown(fds[i].fd,SHUT_RDWR);
  			   			
  				   			close(fds[i].fd);
  			   			
  				   			Poll_update(fds,i,fd_counter);
  				   			fd_counter--;
  				   			
  				   			continue;  			   		
  			   			}
  				   		
  				   		shutdown(fds[i].fd,SHUT_WR);		   		
  						close(fds[i].fd);		// free the file descriptor
 
  						Poll_update(fds,i,fd_counter);
						fd_counter--;	
						
						delete[] message;
						message = NULL;		
						
						printf("GET_CLIENTS finished succesfully \n");		
					
					
					}else{	// Gibberish
					
						printf("Don't recognise this string : %s \n", buffer);
						
						shutdown(fds[i].fd,SHUT_WR);
						shutdown(fds[i].fd,SHUT_RD);
						
						close(fds[i].fd);
						
						Poll_update(fds,i,fd_counter);
						fd_counter--;						
					}
					
					
					
					delete[] buffer;
					buffer = NULL;
					
					continue;				
				
				}	// end if POLLIN
				
//			}	// end if POLLIN
			
			
			
			
				/* fds[i] has established connection (maybe) */				
				if( fds[i].revents & POLLOUT){	// we can send our data , after we check if the connection has been succesfully established
			
			
					printf("\n\nThere is some pending connection\n\n");
			
					/* Searching the node which describes the job the socket needs to do (send a message) */
					Node1* mynode = NULL;
				
					job_list->Search_Find1(fds[i].fd,&mynode);
			
					if( mynode == NULL){
				
						printf("ERROR the node should exist \n");
					
						close(server_socket);
						close(fds[i].fd);
					
						return -1;				
					}
				
					/* Check if the connection was established */
			
					int error = 0;
			
					socklen_t optlen = sizeof(int);
			
					//if( getsockopt( fds[i].fd , SOL_SOCKET, SO_ERROR, (void*) &error, sizeof(int) ) < 0){
					if( getsockopt( fds[i].fd , SOL_SOCKET, SO_ERROR, (void*) &error, &optlen ) < 0){
				
						perror("Error in getsockopt() : ");
				
						printf("\n");
				
						close(fds[i].fd);
					
						Poll_update(fds,i,fd_counter);
						fd_counter--;
					
						continue;			
					}
			
					printf("error is %d \n", error);
			
					if( error > 0){		// an error has occured with connect ==> connection failed

						errno = error;

						close(fds[i].fd);		// don't need Poll_update here
						fds[i].fd = -1;
					
						printf("\n\n Trying to connect() again ...\n\n");
					
									
						/************************* Trying again ****************************************/
					
						clientlen = sizeof(client);
							

						/* 	1) Create a socket, 
							2) set it into O_NONBLOCK
							3) Connect with client
								a) if it connects ==> great! send the message
										
								b) if it doesn't (returns -1) ==> check the error:
										
								i) EINPROGRESS/EWOULDBLOCK ==> the "connection" is in progress and you will be informed when it's ready ===> don't connect 						
							
								ii) EAGAIN ==> not enough ports, check again later ==> Connect() again							
						*/
							
							
							
						// Creating socket and setting into O_NONBLOCK
	    
					    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0){

							perror("Error in socket() (4) ");
							printf("\n");
						
							job_list->deleteNode1(fds[i].fd);

							Poll_update(fds,i,fd_counter);
							fd_counter--;
							
							mynode = NULL;
							client_socket = -1;
					
													        
							continue;
						}    
    
    
	    			    /* Configuring socket ==> SO_REUSEADDR */   
					    int enable = 1;   
					    if( setsockopt(client_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int) ) < 0 ){
    
					    	perror("Error in setsockopt() (3) : ");
	    					printf("\n");
	    	
					    	close(client_socket);
				    	
				    	
					    	job_list->deleteNode1(fds[i].fd);

							Poll_update(fds,i,fd_counter);
							fd_counter--;
						
							mynode = NULL;
							client_socket = -1;
							    	
					    	continue;   
						} 
							    
					   	if( set_NONBLOCK(client_socket) == -1){
	
							perror("Error in set_NONBLOCK() (5) : ");   
							printf("\n");  		
     		     		
					     	close(client_socket);
				     	
					    	job_list->deleteNode1(fds[i].fd);

							Poll_update(fds,i,fd_counter);
							fd_counter--;
						
							mynode = NULL;
							client_socket = -1;
				     	
							     		        	
	    			    	continue;
						}
							    
			       		 /* Configuring our client address struct */    
					    client.sin_family = AF_INET;       /* Internet domain */
					    client.sin_addr.s_addr = htonl( mynode->dest_IP);
					    client.sin_port = htons(mynode->dest_port);      							    
							     
					    // Now try to connect with clients
							     
					     int k = 0;
							     
					     k = connect(client_socket,clientptr,clientlen);		//trying to connect
					     if( k < 0){		// an error has occured
							     
							     	
							     	
					   		//if( (errno == EINPROGRESS) || (errno == EWOULDBLOCK) ){
				   		
					   		if( errno == EINPROGRESS ){
							     	
								// Our "Conection" is in progress ==> POLLOUT when ready
							     		
							 	// Updating poll fd ==> updating fds[i]

								fds[i].fd = client_socket;	
								fds[i].events = POLLOUT;		
							     		
								printf("Updated a PENDING CONNECTION \n\n");
							
								mynode->fd = client_socket;		// updating our node
							
								client_socket = -1;
							
								mynode = NULL;
							

														     			     		 							     	
								continue;			// continuing with fds[i+1]			     	
						   	}
							     	
						   	// this might happen if we have ECONNRESET(other client closed) or some general error
								     	
						  	perror("Error in connect (3) : ");
						  	printf("\n");
							     	
							close(client_socket);
						
							job_list->deleteNode1(fds[i].fd);		// if the client did reset ==> he will ask the client_list anew (so need to send USER_ON, USER_OFF)
						
							Poll_update(fds,i,fd_counter);
							fd_counter--;
						
						
							mynode = NULL;
							     								     							     	
					     	continue;						     	     							     
						}
							     
						// In this case, (2nd) connect was successful
					
						printf("\n\n (2nd) Connection was successful!!!!!!!!!!!!!! \n\n");
					

					
						unsigned long Network_IP = htonl(mynode->IP);
						unsigned short int Network_port = htons(mynode->port);
					
							
						if( write_all(client_socket,mynode->job, strlen(mynode->job) + 1) < 0){
								
							// something is wrong with peer ==> close connection
						
							printf("Write Error (7)\n");

									
							shutdown(client_socket,SHUT_RDWR);
								
							close(client_socket);
							job_list->deleteNode1(fds[i].fd);
						
							Poll_update(fds,i,fd_counter);
							fd_counter--;
						
							mynode = NULL;
									
							continue;							
						}
							
							
						if( write_all(client_socket,&Network_IP,sizeof(unsigned long) ) < 0){
								
							// something is wrong with peer ==> close connection
						
							printf("Write Error (8)\n");

									
							shutdown(client_socket,SHUT_RDWR);
								
							close(client_socket);				// job aborted
							job_list->deleteNode1(fds[i].fd);
									
							Poll_update(fds,i,fd_counter);		// remember fds[i].fd = client_socket 
							fd_counter--;
						
							mynode = NULL;
																		
							continue;							
						}
							
							
						if( write_all(client_socket,&Network_port, sizeof(unsigned short int) ) < 0){
								
							// something is wrong with peer ==> close connection
						
							printf("Write Error (9)\n");

									
							shutdown(client_socket,SHUT_RDWR);
								
							close(client_socket);
							job_list->deleteNode1(fds[i].fd);

							Poll_update(fds,i,fd_counter);
							fd_counter--;
						
							mynode = NULL;									
													
							continue;							
						}
							
						// We have finished sending data => send FIN packet and close connection
								
						shutdown(client_socket,SHUT_WR);		// no longer writes
								
						shutdown(client_socket, SHUT_RD);		// no longer Reads
								
						close(client_socket);
						
						job_list->deleteNode1(fds[i].fd);		// job finished								
					
						Poll_update(fds,i,fd_counter);	// no longer need to watch this socket		
						fd_counter--;
					
						mynode = NULL;					
					
						printf("\n\nFinished pending connection (1) \n\n");

					
					
						continue;			// continue to fds[i+1]
																
					}	// end if statement (error > 0)
							
							
	
					// in the case of (error == 0) ==> (1st) connect was successfull
				
					printf("\n\nPending Connection was successful!!!!!!!!!!!!!! \n\n");
				

					
					unsigned long Network_IP = htonl(mynode->IP);
					unsigned short int Network_port = htons(mynode->port);	
					
					
					if( write_all(fds[i].fd,mynode->job, strlen(mynode->job) + 1) < 0){
								
						// something is wrong with peer ==> close connection
					
						printf("Write Error (10)\n");

					
									
						shutdown(fds[i].fd,SHUT_RDWR);
								
						close(fds[i].fd);
						job_list->deleteNode1(fds[i].fd);
						
						Poll_update(fds,i,fd_counter);
						fd_counter--;
						
						mynode = NULL;
									
						continue;							
					}
							
							
					if( write_all(fds[i].fd,&Network_IP,sizeof(unsigned long) ) < 0){
								
						// something is wrong with peer ==> close connection
					
						printf("Write Error (11)\n");

									
						shutdown(client_socket,SHUT_RDWR);
								
						close(fds[i].fd);				// job aborted
						job_list->deleteNode1(fds[i].fd);
									
						Poll_update(fds,i,fd_counter);		// remember fds[i].fd = client_socket 
						fd_counter--;
						
						mynode = NULL;
																		
						continue;							
					}
							
							
					if( write_all(fds[i].fd,&Network_port, sizeof(unsigned short int) ) < 0){
								
						// something is wrong with peer ==> close connection
					
						printf("Write Error (12)\n");

									
						shutdown(client_socket,SHUT_RDWR);
								
						close(fds[i].fd);
						job_list->deleteNode1(fds[i].fd);

						Poll_update(fds,i,fd_counter);
						fd_counter--;
						
						mynode = NULL;									
													
						continue;							
					}
							
					// We have finished sending data => send FIN packet and close connection
								
					shutdown(fds[i].fd,SHUT_WR);		// no longer writes
								
					shutdown(fds[i].fd, SHUT_RD);		// no longer Reads
								
					close(fds[i].fd);								
					
					job_list->deleteNode1(fds[i].fd);		// job finished
					
					Poll_update(fds,i,fd_counter);	// no longer need to watch this socket		
					fd_counter--;
					
					mynode = NULL;
				
					Network_IP = -1;
					Network_port = -1;
					
					printf("\n\nFinished pending connection (2) \n\n");

					
//					continue;			// continue to fds[i+1]	
			
				}	// end of POLLOUT if clause	
					
		
			}	// end for loop
		    
		}	// end of ( retval > 0) if clause) 	    
		    
	}	// end of while loop ==> received SIGINT


	for( i = 0; i < size; i++){		/// close all open connections
	
		if( fds[i].fd == -1) continue;
	
		shutdown(fds[i].fd,SHUT_RDWR);
		close(fds[i].fd);
		fds[i].fd = -1;
	}
	
	printf("\nCleaning up \n");

	delete[] fds;
	fds = NULL;


	delete client_list;
	client_list = NULL;
	
	delete job_list;
	job_list = NULL;

	printf("\nExiting.... \n");

	return 0;

}







