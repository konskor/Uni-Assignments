#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>

#include <dirent.h>

#include <sys/errno.h>
#include <sys/stat.h>

#include <fcntl.h>


#include <signal.h>

#include "header.hpp"



/* IMPORTANT ====> LOG FILES and SIGNAL/alarm() */


int fifo_fd = -1;

char Pipe[30];

bool creator = false;



void ALRM_handler(int sig){

	printf("child %d received SIGALRM, sending SIGUSR2 to parent and exiting\n", getpid());

	pid_t father = getppid();
	
	kill(father,SIGUSR2);
	
	close(fifo_fd);
	
	if( creator == true) unlink(Pipe);	
	
	exit(1);

}


void Quit(int sig){		// handler for SIGINT, SIGQUIT

	printf("child %d received Signal %d , exiting\n", getpid(), sig);

	close(fifo_fd);
	
	if( creator == true) unlink(Pipe);
		
	exit(1);	
}






int main(int argc, char* argv[] ){


/*	everything char*

	arg1 = common_dir name
	arg2 = mirror_dir name
	
	arg3 = ID2
	arg4 = buffer_size
	
	arg5 = ID
	
	arg6 = log_file
	
*/

	signal(SIGPIPE,SIG_IGN);			//ignore SIGPIPE, we will handle it in our code
	
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
	
	
	
	//SIGALRM
	struct sigaction sa1;
	sa1.sa_handler = &ALRM_handler;
	sigemptyset(&sa1.sa_mask);	//block all signals
	sa1.sa_flags = SA_RESTART;	// retry system calls


	if( sigaction(SIGALRM,&sa1,NULL) == -1){
	
		perror("Sigaction error (3): ");
		return -1;	
	}
	
	
	
	
	
	
	
	


//	bool creator = true;


	// Creating name of the pipe 

//	unsigned int length = snprintf(NULL,0,"%s/%s_to_%s.fifo", argv[1], argv[3], argv[5]) + 1;		// common_dir/id2_to_id1
//	char Pipe[length];
	snprintf(Pipe,30,"%s/%s_to_%s.fifo", argv[1], argv[3], argv[5]);
	
	
	
	alarm(30);

	/* Create FIFO ( or just open it if it exists) */
	
	int fifo_fd = 0;
	
	if( mkfifo(Pipe,0666) == -1){	// an error has occured
	
		if( errno == EEXIST){ 	// fifo already exists ==> just open it
		
//			creator = false;	// we didn't create it

			printf("Receiver Child: FIFO already exists, opening it... \n");
		
			if( ( fifo_fd = open(Pipe,O_RDONLY) ) == -1 ){
			
				perror("Couldn't open FIFO in receiver (1) ");
				
				close(fifo_fd);
				
				exit(1);
			}
			
			
		
		}else{	//another error occured
		
			perror("Couldn't create FIFO in receiver ");
			
			close(fifo_fd);
			
			exit(1);		
		}
	
	
	
	}else{	// created fifo, now we open it
	
		creator = true;
	
		printf("Receiver Child: Created FIFO, opening it... \n");
	
		if( ( fifo_fd = open(Pipe,O_RDONLY) ) == -1 ){
			
			perror("Couldn't open FIFO in sender (2) ");
			
			close(fifo_fd);
				
			exit(1);
			
		}
	
	
	}
	
	alarm(0);		//resetting timer 
	
	
	printf("Receiver Child: Opened FIFO, starting procedure \n");

	/* Create mirror_dir/ID2 */
	
	int length1 = snprintf(NULL,0,"%s/%s", argv[2], argv[3] ) + 1;
	char mirror_client[length1];
	snprintf(mirror_client, length1,"%s/%s", argv[2], argv[3] );
	
	
	if( mkdir(mirror_client,0777) == -1){
	
		if(errno != EEXIST){	//if it exists , continue
		
			perror("Mkdir error in receiver ");
		}
		
	}
	
	


	/* Start The Procedure */
	
	int bytes = 0;
	int temp = 0;
	
	
	short int length = 0;
	
	int buffer_size = atoi( argv[4]);
	
	
	/* 1st read */
	
	alarm(30);
		
	//read length of path..... if length = 0 , break
	while(  ( (temp = read_all(fifo_fd, &length, sizeof(short int) ) ) != -1) && ( length != 0 ) ){	// -1 , because 0 is dir and >0 is file
	
			alarm(0);
	
			bytes = 0;		//restart counter	
	
			bytes = bytes + temp;
			
			printf("\n\n Receiver Child: Main loop , length is %d \n\n", length );
			
			char *name = new char[length];
			
			char *name1 = new char[length];		//keep the original name
			
			/* 2nd read */
			
			alarm(30);
			
			temp = read_all(fifo_fd, name, length); 	 //read path of file/dir
			
			alarm(0);	
			
			strcpy(name1,name);
			
			printf("Receiver Child: Main loop, path is name: %s\n",name);
				
			bytes = bytes + temp;
						
 			//we have ./mirror/a/.. and we want the a/..
 			
// 			printf("Name is %s \n", name);
			
			strtok(name,"/");	// the result is .
			strtok(NULL,"/");	// the result is input
			
			char* remain = strtok(NULL,"");	// the result is a/.. (what we want)	
		
			if( RecursiveReceiver(mirror_client, remain, fifo_fd, buffer_size, &bytes) == false){
			
				pid_t father = getppid();
			
				kill(father,SIGUSR1);
				
				if( close(fifo_fd) == -1){
	
					perror(" Error closing FIFO in receiver (1) ");
					exit(1);
				}
		
				if( creator == true) unlink(Pipe);// if we created the named pipe ===> destroy it		
				exit(1);
			}
			
			
			/* LOG FILE */
			FILE* fp1 = fopen(argv[6],"a");
			if( fp1 == NULL){
			
				perror("Couldn't write in log_file , ReceiverChild ");
				return false;
			}
			
			
			fprintf(fp1,"Received %d bytes for %s file/dir \n", bytes, name1);
			fclose(fp1);		
			
			
			
			
			
			
	
			delete[] name;
			name = NULL;
			
			delete[] name1;
			name1 = NULL;
			
			
			printf("\n\n Receiver Child: reached end of loop \n\n");
	}
	
	if( temp == -1){
	
		pid_t father = getppid();
			
		kill(father,SIGUSR1);
				
		if( close(fifo_fd) == -1){
	
				perror(" Error closing FIFO in receiver (1) ");
				exit(1);
			}
		
		if( creator == true) unlink(Pipe);// if we created the named pipe ===> destroy it
	
		exit(1);
	}


	//else we are ok

	printf("Receiver Child: Finished procedure , cleaning up! \n");
	

	if( close(fifo_fd) == -1){
	
		perror(" Error closing FIFO in sender ");
		exit(1);
	}
	
	
	if( creator == true){	// if we created the named pipe ===> destroy it
	
		if( unlink(Pipe) < 0){
		
			perror("Couldn't delete Pipe ");
		}else{
		
			printf("Deleted %s Pipe \n", Pipe);
		}
	}
	
	printf("Receiver child: Exit! \n");
	
	exit(0);
}












