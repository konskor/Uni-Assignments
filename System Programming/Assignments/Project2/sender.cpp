#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>

#include <dirent.h>

#include <sys/errno.h>
#include <sys/stat.h>

#include <signal.h>

#include <fcntl.h>

#include "header.hpp"



int fifo_fd = -1;	//-1 because there is no valid file descriptor = -1
char Pipe[30];


bool creator = false;

/* IMPORTANT ====> LOG FILES and SIGNAL/alarm() */

void ALRM_handler(int sig){

	printf("child %d received SIGALRM, sending SIGUSR2 to parent and exiting\n", getpid());

	pid_t father = getppid();
	
	kill(father,SIGUSR2);
	
	close(fifo_fd);	
	
	if(creator == true) unlink(Pipe);
	
	exit(1);

}


void Quit(int sig){		// handler for SIGINT, SIGQUIT

	printf("child %d received Signal %d , exiting\n", getpid(), sig );

	close(fifo_fd);
	
	if(creator == true) unlink(Pipe);
	
	exit(1);
	
	
	
	
}





int main(int argc, char* argv[] ){


/*	everything char*

	arg1 = common_dir name
	arg2 = input_dir name
	
	arg3 = ID2
	arg4 = buffer_size
	
	arg5 = ID
	
	arg6 = log_file
	
*/

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
	
	
	signal(SIGPIPE,SIG_IGN);			//ignore SIGPIPE, we will handle it in our code


//	bool creator = true;


	// Creating name of the pipe 

//	unsigned int length = snprintf(NULL,0,"%s/%s_to_%s.fifo", argv[1], argv[5], argv[3]) + 1;
//	char Pipe[length];
	snprintf(Pipe,30,"%s/%s_to_%s.fifo", argv[1], argv[5], argv[3]);
	
	
	printf(" common is %s \n", argv[1]);

	/* Create FIFO ( or just open it if it exists) */
	
//	int fifo_fd = 0;

	
	alarm(30);

	
	if( mkfifo(Pipe,0666) == -1){	// an error has occured
	
		if( errno == EEXIST){ 	// fifo already exists ==> just open it
		
//			creator = false;	// we didn't create it
		
			printf("Sender Child: FIFO already exists, opening it... \n"); 
		
			if( ( fifo_fd = open(Pipe,O_WRONLY) ) == -1 ){
			
				perror("Couldn't open FIFO in sender (1) ");
				
				close(fifo_fd);
				
				exit(1);
			}
			
			
		
		}else{	//another error occured
		
			perror("Couldn't create FIFO in sender ");
			
			close(fifo_fd);
			
			exit(1);		
		}
	
	
	
	}else{	// created fifo, now we open it
	
		creator = true;
	
		printf("Sender child: Created FIFO, opening it ... \n");
	
		if( ( fifo_fd = open(Pipe,O_WRONLY) ) == -1 ){
			
			perror("Couldn't open FIFO in sender (2) ");
			
			close(fifo_fd);
				
			exit(1);
			
		}	
	} 

	alarm(0);

	printf("Sender child: Opened FIFO, starting the procedure \n");
	
	
	


//	int file_counter = 0;		// how many files did we send (might not need it)
	
	int buffer_size = atoi( argv[4]);
	
	int bytes = 0;
	
	printf("\n\nSender Child: Starting RecursiveSender with starting prefix:%s \n\n", argv[2]);
	
	if( RecursiveSender(argv[2], fifo_fd, buffer_size, argv[3], argv[6]) == false){	//something went wrong during transportation
		
		pid_t father = getppid();	// parent pid
	
		kill(father,SIGUSR1);
		
		if( close(fifo_fd) == -1){
	
			perror(" Error closing FIFO in sender (1) ");
			exit(1);
		}
		
		if( creator == true) unlink(Pipe);	// if we created the named pipe ===> destroy it
		
		exit(1);
	}	

	short int k = 0;		// -1 means we have done with our files/dir
	
	alarm(30);
	
	if( write_all(fifo_fd,&k, sizeof(short int)) == -1){// write -1 ==> means we finally finished

		pid_t father = getppid();	// parent pid
	
		kill(father,SIGUSR1);
		
		if( close(fifo_fd) == -1){
	
			perror(" Error closing FIFO in sender (2) ");
			exit(1);
		}
		
		if( creator == true) unlink(Pipe);	// if we created the named pipe ===> destroy it
		
		exit(1);

	}
	
	alarm(0);


	printf("Sender Child: Finished the proceudre, cleaning up! \n");


	if( close(fifo_fd) == -1){
	
		perror(" Error closing FIFO in sender (3) ");
		exit(1);
	}
	
	
	if( creator == true){	// if we created the named pipe ===> destroy it
	
		if( unlink(Pipe) < 0){
		
			perror("Couldn't delete Pipe ");
		}else{
		
			printf("Deleted %s Pipe \n", Pipe);
		}
	}
	
	printf("Sender Child: Exit! \n");
	
	exit(0);
}












