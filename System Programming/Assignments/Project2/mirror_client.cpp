#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <sys/stat.h>

#include <dirent.h>

#include <signal.h>


#include "header.hpp"





volatile sig_atomic_t quit_flag = 0;

volatile sig_atomic_t USR1_flag = 0;
volatile sig_atomic_t USR2_flag = 0;


/* Handlers */

void Quit(int sig){		// handler for SIGINT, SIGQUIT

	quit_flag = 1;
}


void USR1_handler(int sig){

	USR1_flag = 1;

}



void USR2_handler(int sig){

	USR2_flag = 1;

}








int main( int argc, char* argv[]){

	//SIGNALS
	
	// SIGINT, SIGQUIT
	struct sigaction sa;
	sa.sa_handler = &Quit;
	sigemptyset(&sa.sa_mask);	//block all signals
	sa.sa_flags = SA_RESTART;	// retry system calls


	if( sigaction(SIGINT,&sa,NULL) == -1){
	
		perror("Sigaction error (1): ");
		return -1;	
	}
		
	if( sigaction(SIGQUIT,&sa,NULL) == -1){
	
		perror("Sigaction error (2): ");
		return -1;	
	}


	//SIGUSR1
	struct sigaction sa1;
	sa1.sa_handler = &USR1_handler;
	sigemptyset(&sa1.sa_mask);	//block all signals
	sa1.sa_flags = SA_RESTART;	// retry system calls

	if( sigaction(SIGUSR1,&sa1,NULL) == -1){
	
		perror("Sigaction error (3): ");
		return -1;	
	}
		

	//SIGUSR2
	struct sigaction sa2;
	sa2.sa_handler = &USR2_handler;
	sigemptyset(&sa2.sa_mask);	//block all signals
	sa2.sa_flags = SA_RESTART;	// retry system calls

	if( sigaction(SIGUSR2,&sa2,NULL) == -1){
	
		perror("Sigaction error (4): ");
		return -1;	
	}





	if( argc < 13){
	
		printf("Error not enough arguments \n");
		
		return -1;	
	}



	/* Read Input */

	int id = 0;
	
	char* common = NULL;
	
	char* input = NULL;
	
	char* mirror = NULL;
	
	int buffer_size = 0;
	
	char* log_file = NULL;
	






	int i = 0;

	for( i = 0; i < argc; i++){

		if( strcmp(argv[i] , "-n") == 0 ){		// ID
		
			if( i == argc-1){
				printf(" Something is wrong: -n ..... \n");
				return -1;		
			}
		
			id = atoi( argv[i+1] );
			
			continue;
		}
		
		

		if( strcmp(argv[i] , "-c") == 0 ){		// Common_dir
		
			if( i == argc-1){
				printf(" Something is wrong: -c ..... \n");
				return -1;		
			}
		
			int length = 0;

			length = strlen( argv[i+1] ) + 1; 
		
			common = new char[length];
			
			strcpy(common, argv[i+1] );
			
			continue;


		}
		
		if( strcmp(argv[i] , "-i") == 0 ){		// Input_dir
		
			if( i == argc-1){
				printf(" Something is wrong: -i ..... \n");
				return -1;		
			}
		
			int length = 0;

			length = strlen( argv[i+1] ) + 1; 
		
			input = new char[length];
			
			strcpy(input, argv[i+1] );
			
			continue;		
		

		}
		
		if( strcmp(argv[i] , "-m") == 0 ){		// Mirror_dir
		
			if( i == argc-1){
				printf(" Something is wrong: -m ..... \n");
				return -1;		
			}
		
			int length = 0;

			length = strlen( argv[i+1] ) + 1; 
		
			mirror = new char[length];
			
			strcpy(mirror, argv[i+1] );
			
			continue;
		
		

		}		


		if( strcmp(argv[i] , "-b") == 0 ){		// Buffer_Size
		
			if( i == argc-1){
				printf(" Something is wrong: -h2 ..... \n");
				return -1;		
			}
		
			buffer_size = atoi( argv[i+1] );
			
			if( buffer_size <= 0){
				
				printf("buffer_size must be > 0 \n");
				return -1;
			
			} 
			
			continue;
		}
		
		

		if( strcmp(argv[i] , "-l") == 0 ){		// log file
		
			if( i == argc-1){
				printf(" Something is wrong: -b ..... \n");
				return -1;		
			}
		
			int length = 0;

			length = strlen( argv[i+1] ) + 1; 
		
			log_file = new char[length];
			
			strcpy(log_file, argv[i+1] );
			
			continue;
		}

	}







	// common_dir must not be the same as input_dir or mirror_dir 
	if( ( strcmp( common, input) == 0) || (strcmp( common, mirror) == 0 ) ){
	
		printf("Common_dir is the same as input_dir or mirror_dir ==> Exiting .... \n");
		
		delete[] common;
		delete[] input;	
		delete[] mirror;
		delete[] log_file;
	
		common = NULL;
		input = NULL;
		mirror = NULL;
		log_file = NULL;
		
	
		return -1;
	}



	// input_dir must not be the same as mirror_dir 
	if( strcmp( mirror, input) == 0 ){
	
		printf("Common_dir is the same as input_dir or mirror_dir ==> Exiting .... \n");
	
		delete[] common;
		delete[] input;	
		delete[] mirror;
		delete[] log_file;
	
		common = NULL;
		input = NULL;
		mirror = NULL;
		log_file = NULL;
	
	
		return -1;
	}


	/* Finished Reading input */
	
	
	/* STEP 1 */
	
	
// open input_dir
	
	DIR* input_dir = opendir(input);
	
	if( input_dir == NULL){	// input dir doesn't exist ==> exit
	
		printf("Input dir doesn't exist ===> exiting.... \n");
	
		delete[] common;
		delete[] input;	
		delete[] mirror;
		delete[] log_file;
	
		common = NULL;
		input = NULL;
		mirror = NULL;
		log_file = NULL;
	
	
		return -1;	
	}

	closedir(input_dir);



// open mirror_dir 

	DIR* mirror_dir = opendir(mirror);
	
	if( mirror_dir != NULL){	// mirror dir exist ==> exit
	
		printf("Mirror dir already exists! ==> exiting.....  \n");
	
		delete[] common;
		delete[] input;	
		delete[] mirror;
		delete[] log_file;
	
		common = NULL;
		input = NULL;
		mirror = NULL;
		log_file = NULL;
	
	
		return -1;	
	}
	// mirror_dir doesn't exist ==> create it

	if ( mkdir( mirror, 0777) == -1){
	
		printf("Something went wrong, creating mirror_dir \n");
		
		delete[] common;
		delete[] input;	
		delete[] mirror;
		delete[] log_file;
	
		common = NULL;
		input = NULL;
		mirror = NULL;
		log_file = NULL;
	
	
		return -1;	
	}
	
	closedir(mirror_dir);
	
	
	
// open common_dir 

	DIR* common_dir = opendir(common);
	
	if( common_dir == NULL){
	
		// common_dir doesn't exist ==> create it

		if ( mkdir( common, 0777) == -1){
	
			printf("Something went wrong, creating common_dir \n");
		
			delete[] common;
			delete[] input;	
			delete[] mirror;
			delete[] log_file;
		
			common = NULL;
			input = NULL;
			mirror = NULL;
			log_file = NULL;
		
	
			return -1;	
		}


		common_dir = opendir(common);
		
		if( common_dir == NULL){
		
			printf("Something still went wrong, with common_dir \n");
		
			delete[] common;
			delete[] input;	
			delete[] mirror;
			delete[] log_file;
		
			common = NULL;
			input = NULL;
			mirror = NULL;
			log_file = NULL;
			
			return -1;		
		}




	}
	
	closedir(common_dir);
	
	
	
	// open/create log_file

	FILE* log = fopen(log_file,"w");
	
	if( log == NULL){
	
		perror("Error opening log in mirror_client ");
		delete[] common;
		delete[] input;	
		delete[] mirror;
		delete[] log_file;
		
		common = NULL;
		input = NULL;
		mirror = NULL;
		log_file = NULL;
			
		return -1;	
		
	}
	
	fprintf(log, "Connected with ID: %d \n", id);
	
	fclose(log);	

//	fclose(log);
	
	
	/* STEP 2 */
	
	int length = 0;
	char* ID_file = NULL;
	
	
	length = snprintf(NULL,0,"%s/%d.id", common, id) + 1;
	ID_file = new char[length];
	snprintf(ID_file,length,"%s/%d.id",common, id);
	
	
	printf("ID file is %s\n", ID_file);
	

	if( access(ID_file, F_OK) != -1){		// ID file already exists??
	
		printf("ID file exists in common_dir \n");
		
		delete[] ID_file;
		
		delete[] common;
		delete[] input;	
		delete[] mirror;
		delete[] log_file;
		
		common = NULL;
		input = NULL;
		mirror = NULL;
		log_file = NULL;
			
		ID_file = NULL;	
			
		return -1;
	}

	// ID file doesn't exist ===> we create it
	
	FILE* fp = fopen( ID_file, "w");
	
	if( fp == NULL){
	
		perror("Couldn't create ID file ");
		
		delete[] ID_file;
		
		delete[] common;
		delete[] input;	
		delete[] mirror;
		delete[] log_file;
		
		common = NULL;
		input = NULL;
		mirror = NULL;
		log_file = NULL;
			
		ID_file = NULL;	
			
		return -1;	
	}


	// write the processID inside
	
	pid_t myself = getpid();
	
	fwrite( &myself, sizeof(pid_t), 1, fp);
	
	
	fclose(fp);		// close the ID file


	// Creating list of IDs (checking if we have synchronised with a client)
	List* ID_list = new List();


	int counter = 0;		//counter for ID files , encountered in this loop



	while(quit_flag != 1){ 	// untill SIGQUIT or SIGINT
	
		/* Search ./common_dir */
		
		common_dir = opendir(common);		// open common_dir
		if( common_dir == NULL){
		
			perror(" Opendir(common_dir) error ");
			return -1;	
		}
		
		struct dirent *direntp = NULL;
	
	
		counter = 0;		//counter for ID files , encountered in this loop
		
		// Our ID in string
		int length = snprintf(NULL,0,"%d.id", id) + 1;
		char IDf[length];
		snprintf(IDf, length,"%d.id", id);
		
		//printf(" IDf is %s \n", IDf);
		
	
	
		while(  (direntp = readdir( common_dir) ) != NULL){
		
		//	printf("direntp name is %s\n", direntp->d_name);
		
			// ignore . and .. entries and your own ID_File
			if( (strcmp(direntp->d_name,".") == 0) || (strcmp(direntp->d_name,"..") == 0) || ( strcmp(IDf, direntp->d_name) == 0 )  ) continue;
		
		
			//Search for *.id files 
			if( strstr( direntp->d_name,".id") != NULL ){
			
			//	printf("Entered ststr IF \n");
			
				counter++;		// found an ID file, incrementing counter
			
				//already synchronised
				if( ID_list->Search( direntp->d_name) == true) continue;
				
				//else ====> SYNCHRONISE
				
				char* temp = strdup( direntp->d_name);
				
				char* Id2 = temp;
				
				printf("ID2 is %s \n", Id2);
				
				
				Id2 = strtok(temp,".");// find the ID of the other client				
				
				int i = 0;
				
				bool complete = false;	//if transfer was completed
				
				
				/* LOG FILE */
				log = fopen(log_file,"a");
				fprintf(log,"Synchronising with %s \n",Id2);		// closing it (for now)
				fclose(log);								
								
				
				
				printf("Parent: Starting 3 tries of synchronisation \n");	
							
				
				for( i = 0; i < 3; i++){ 
				
					/* 1st child ==> sender */

					pid_t sender = 0;
	
					sender = fork();
		
					if( sender == -1){
		
						perror(" Fork failed (1) ");
						continue;
					
					}else if( sender == 0){		// child ==> exec
				
						// creating buffersize as string
						int length = snprintf(NULL,0,"%d", buffer_size) + 1;
						char buff[length];
						snprintf(buff,length,"%d", buffer_size) + 1;	
					
					
						// creating our ID as string
						length = snprintf(NULL,0,"%d", id) + 1;
						char IDS[length];
						snprintf(IDS,length,"%d", id);
				
				
				
						execlp("./sender","sender", common, input, Id2, buff, IDS, log_file, NULL);
	
						perror("Sender execlp() fail ");
					
						return -1;	
					}
		
		
					/* 2nd child ==> receiver */

					pid_t receiver = 0;
	
					receiver = fork();
		
					if( receiver == -1){
		
						perror(" Fork failed (1) ");
						continue;
					
					}else if( receiver == 0){		// child ==> exec
	
	
						// creating buffersize as string
						int length = snprintf(NULL,0,"%d", buffer_size) + 1;
						char buffS[length];
						snprintf(buffS,length,"%d", buffer_size);	
					
					
						// creating our ID as string
						length = snprintf(NULL,0,"%d", id) + 1;
						char IDS[length];
						snprintf(IDS,length,"%d", id);	
					
						
						execlp("./receiver","receiver", common, mirror, Id2, buffS, IDS, log_file, NULL);
	
						perror(" Sender execlp() fail ");
					
						return -1;
					}
				
				
					pid_t temp_pid = 0;
					pid_t temp_pid1 = 0;
				
					int status = 0;
						
					
				
					// wait for the children (1 for each child)
				
					printf("\n\n\n\nParent: 1st wait\n\n\n\n");
				
					/* 1st wait */
				
					temp_pid = wait(&status);
					
					if( temp_pid == -1){
				
						perror("Wait error (1) ");
						return -1;				
					}
										
								
					if( WIFEXITED(status) ){
						//exited normally (either exit(0) or exit(1) )	
																
						
						if(  USR1_flag == 1){	//an error occured
						
//							delete[] temp;
//							temp = NULL;
					
							USR1_flag = 0;
							
							printf("An error occured with child %d , trying again\n",temp_pid);
															
							continue;	// the for loop					
						}
						
						
						if( USR2_flag == 1){ // we received SIGUSR2 (SIGALRM)
					
							/*	An alarm caused one of the children to exit
									===> kill(SIGINT) the other child
							*/	
						
							
							if( temp_pid == sender){
														
								kill(receiver,SIGINT);	// interrupt the other child

							}else{	//temp_pid = receiver
						
								kill(sender,SIGINT);
							}							
						
							/* Removing the mirror/Id2  dir*/
						
							int length1 = snprintf(NULL,0,"%s/%s", mirror, Id2 ) + 1;
							char mirror_client[length1];
							snprintf(mirror_client, length1,"%s/%s", mirror, Id2 );
						
							Removedir(mirror_client);
						
						
							/* Write to log file ? */
						
//							if( temp != NULL) delete[] temp;
//							temp = NULL;
						
							USR2_flag = 0;
					
							continue;
						}				
																		
						
					}else if( WIFSIGNALED(status) ){
				
						if( WCOREDUMP(status) ){
					
							printf("Core dumped in child %d --> exiting.. \n", temp_pid);	
							return -1;	
						}
										
	
					
				
					}
					
					
				
					// if we are here, the previous child exited normally ( exit(0) ) or we couldn't synchronise with it after 3 tries
				
					printf("\n\n\n\nParent: Second Wait\n\n\n\n");
				
					/* 2nd wait */				
				
					temp_pid = wait(&status);
				
				
					if( temp_pid == -1){
				
						perror("Wait error (2) ");
						return -1;				
					}
												
					if( WIFEXITED(status) ){
						//exited normally (either exit(0) or exit(1) )	
						

						
						
						if( WEXITSTATUS(status) == 0){
						
							complete = true;
							break;						
						}
						
									
					
						if( USR1_flag == 1){	//an error occured in child
					
							printf("An error occured with child %d , trying again\n",temp_pid);
										
							USR1_flag = 0;					
					
							continue;	// the for loop					
						}
						//else, exited normally ( exit(0) )
						
						
						if( USR2_flag == 1){ // we received SIGUSR2
					
							/*	An alarm caused one of the children to exit
									===> kill(SIGINT) the other child
							*/	
						
							
							if( temp_pid == sender){
						
								kill(receiver,SIGINT);	// interrupt the other child

							}else{	//temp_pid = receiver
						
								kill(sender,SIGINT);
							}							
						
							/* Removing the mirror/Id2  dir*/
						
							int length1 = snprintf(NULL,0,"%s/%s", mirror, Id2 ) + 1;
							char mirror_client[length1];
							snprintf(mirror_client, length1,"%s/%s", mirror, Id2 );
						
							Removedir(mirror_client);
						
						
							/* Write to log file ? */
						
//							delete[] temp;
						
							USR2_flag = 0;
					
							continue;		//out of the loop
						}				
						
						//else, exited normally ( exit(0) )
						
						complete = true;
						
						break;						
					
					}else if( WIFSIGNALED(status) ){
				
						if( WCOREDUMP(status) ){
					
							printf("Core dumped in child %d --> exiting.. \n", temp_pid);	
							return -1;	
						}
										
										
						printf("\n\n\n\n\n\n\nHELLO (4) \n\n\n\n\n\n\n\n\n");				
										
										
										
					}
					
					
					if( quit_flag == 1) break;	//exiting the for loop
					
					
				}
				
			
	
				if( complete == true){
	
					// If all went well ==> add name to the list
				
					ID_list->AddNode(direntp->d_name);
				
					printf("ID %d completed transfer with ID %s\n", id,Id2);	
					
					/* LOG FILE */
					log = fopen(log_file,"a");
					fprintf(log,"Finished Synchronising with %s \n",Id2);		// closing it (for now)
					fclose(log);
					
								
				
				}
				
				
				
				if( i == 3) printf("Parent: We couldn't synchronise with child %s after 3 tries.... \n", Id2);				
				
				
				delete[] temp;
												
			}	// this is the if( strstr() ) clause
			
			if( quit_flag == 1) break;		// exiting the while( readdir() ) loop
				
		
		} // moving to the next dir entry
	
	
	
	
	
		/* Checking if any client exited */
	
		if(counter < ID_list->counter){	// if in this loop, we encountered less ID_files, than what we have on he list
		
			printf("Parent: Some people have exited, checking... \n");
		
			rewinddir(common_dir);

			int iterations = ID_list->counter - counter;	// how many clients have left
			int i = 0;

			for( i = 0; i < iterations; i++){
		
				Node* node = ID_list->head;
			
				while( node != NULL){
			
					if( node->Exist(common_dir) == false) break;
				
					rewinddir(common_dir);
				
					node = node->next;
				}
			
				if( node == NULL){
			
					printf("ERROR something should have been missing");
					return -1;
				
				}
				//else node != NULL
			
				int length = strlen(node->ID) + 1;	// node->ID is type ID.id, and we want the ID
				char temp[length];
				strcpy(temp,node->ID);
			
				char* ID2s = strtok(temp,".");
				
				// Creating the name mirror/ID2s ==> to delete it
				length = snprintf(NULL,0,"%s/%s", mirror, ID2s) + 1;
				char dir[length];
				snprintf(dir,length,"%s/%s", mirror, ID2s);
				
				Removedir(dir);
				
				ID_list->deleteNode(node->ID);
				
				node = NULL;
				
				
			}
			printf("Parent: Finished checking \n");			
		}
		
		closedir(common_dir);	

		sleep(3);	

	}// continuing loop

	printf("\nParent: Exited main loop, because we received SIGINT/SIGQUIT\n");



	/* LOG FILE */
	log= fopen(log_file,"a");
	
	if( log == NULL){
	
		perror(" Error final fopen ");
		return -1;
	}
	
	fprintf(log,"Exiting as ID: %d \n",id);
	fclose(log);
	
	/* Deleting our mirror_dir and Id file */

	printf("Parent: Deleting mirror_dir and ID_file \n");

	Removedir(mirror);
	
	if( unlink(ID_file) < 0){
	
		perror("Error deleting ID_file ");
	}
	


	printf("Parent: Freeing up memory \n");

	/* Freeing up memory */
	
	delete ID_list;
	ID_list = NULL;

	
	printf("Freed the list\n");

	delete[] common;
	delete[] input;	
	delete[] mirror;
	delete[] log_file;
	
	delete[] ID_file;
	
	common = NULL;
	input = NULL;
	mirror = NULL;
	log_file = NULL;

	ID_file = NULL;

	printf("Parent: Exit! \n");
	
	return 0;

}




