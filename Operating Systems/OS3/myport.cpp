#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <time.h>
#include <sys/time.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <semaphore.h>
#include <unistd.h>
#include <wait.h>

#include <signal.h>

#include "MyStructs.hpp"


int main(int argc, char* argv[]){

	signal( SIGINT, SIG_IGN);	// SIGINT will only affect the "child" processes




	if( argc != 3){

		printf("Please enter a configuration file \n");
		return -1;
	}

	/*Opening Config file */

	FILE* fp;
	char buffer[20];

	char* tempz;

	char type1[2], scapacity1[4], scost1[4];
	char type2[2], scapacity2[4], scost2[4];
	char type3[2], scapacity3[4], scost3[4];

	char charges_name[20], PLrecords_name[20], log_name[20];


	char time_m[4], stattime[4];

	char Stime_period[4];


	if( (fp = fopen( argv[2], "r") ) == NULL){

		perror("Error opening config ");
		return -1;
	}

	/* Reading the contents */

	fgets(buffer,20,fp);	// read line

	tempz = strtok(buffer," ");			// Small
	strcpy(type1,tempz);


	tempz = strtok(NULL," ");
	strcpy(scapacity1,tempz);


	tempz = strtok(NULL," ,\n");
	strcpy(scost1,tempz);



	fgets(buffer,20,fp);

	tempz = strtok(buffer," ");			// Medium
	strcpy(type2,tempz);


	tempz = strtok(NULL," ");
	strcpy(scapacity2,tempz);


	tempz = strtok(NULL," ,\n");
	strcpy(scost2,tempz);



	fgets(buffer,20,fp);

	tempz = strtok(buffer," ");			// Large
	strcpy(type3,tempz);


	tempz = strtok(NULL," ");
	strcpy(scapacity3,tempz);



	tempz = strtok(NULL," ,\n");
	strcpy(scost3,tempz);


	fgets(buffer,20,fp);						// Time_period

	tempz = strtok(buffer," ,\n");
	strcpy(Stime_period,tempz);




	fgets(buffer,20,fp);						// Charges.txt

	tempz = strtok(buffer," ,\n");
	strcpy(charges_name,tempz);


	fgets(buffer,20,fp);						// PL_Records.txt = Public Ledger Records
	tempz = strtok(buffer," ,\n");
	strcpy(PLrecords_name,tempz);



	fgets(buffer,10,fp);						// Log.txt
	tempz = strtok(buffer," ,\n");
	strcpy(log_name,tempz);




	fgets(buffer,20,fp);						// Time vars for monitor
	tempz = strtok(buffer," ,\n");
	strcpy(time_m,tempz);


	tempz = strtok(NULL," ,\n");
	strcpy(stattime,tempz);

	if( fclose(fp) == EOF){

		perror("Error Closing config ");
		return -1;
	}


	/* Initiating log.txt and PLrecords.txt */

	if( (fp = fopen(log_name,"w") ) == NULL){

		perror(" Error opening log in myport ");
		return -1;
	}

	if( fclose(fp) == -1){

		perror("Error closing log in myport ");
		return -1;
	}


	if( (fp = fopen(PLrecords_name,"w") ) == NULL){

		perror(" Error opening log in PLrecords_name ");
		return -1;
	}

	if( fclose(fp) == -1){

		perror("Error closing PLrecords_name in myport ");
		return -1;
	}


	/* Done reading Config file */

	/* Creating Shared Memory */

	key_t key;
	int capacity1 = atoi( scapacity1);
	int capacity2 = atoi( scapacity2);
	int capacity3 = atoi( scapacity3);

	double start = 0;

	int cost1 = atoi( scost1);
	int cost2 = atoi( scost2);
	int cost3 = atoi( scost3);




	//in the same folder

	if( ( key = ftok( argv[0] ,10) ) == -1)
		perror("Cannot create key from (argv[0],10) ");



	int size = sizeof(struct SHMemory) + (capacity1+capacity2+capacity3) * sizeof(Position) + 20 * sizeof(char) + 10 * sizeof(char) + sizeof( struct Application_form);



	// SHMemory, Positions, PLrecords_name, log_name, Application_form,


	int shmid = shmget(key, size, IPC_CREAT|0666);

	if( shmid == -1) perror("Error in creating shared memory (myport) ");


	printf(" Shmid is %d \n", shmid);


	/* Attaching and initialising memory */

	SHM *mem = (SHM *) shmat(shmid, (void *) 0, 0);
	if( mem == (void *) -1 ) perror("Error attaching memory (myport) ");









		/* Initialising semaphores */

	// For more info on the semaphores , check Mystruct.hpp






	if( (sem_init( &(mem->check), 1, 0) ) == -1){	// wait for application to arrive, and decide

		perror( "Error initialising semaphore (2) " );

		if( shmdt( (void *) mem) == -1) perror(" Error detaching memory (myport) (2) ");

		if( shmctl( shmid, IPC_RMID, 0) == -1) 
			perror(" Error removing/deleting memory (myport) (2) ");


		exit(1);
	}



	if( (sem_init( &(mem->full), 1, 0) ) == -1){// inform port-master, there is an application to be read

		perror( "Error initialising semaphore (5) " );

		if( shmdt( (void *) mem) == -1) perror(" Error detaching memory (myport) (5) ");

		if( shmctl( shmid, IPC_RMID, 0) == -1) 
			perror(" Error removing/deleting memory (myport) (5) ");


		exit(1);
	}

	if( (sem_init( &(mem->in_evaluation), 1, 0) ) == -1){// wait for the application to be evaluated by port-master

		perror( "Error initialising semaphore (6) " );

		if( shmdt( (void *) mem) == -1) perror(" Error detaching memory (myport) (6) ");

		if( shmctl( shmid, IPC_RMID, 0) == -1) 
			perror(" Error removing/deleting memory (myport) (6) ");

		exit(1);
	}

	if( (sem_init( &(mem->maneuver_pm), 1, 0) ) == -1){	// port-master waits untill vessel finishes maneuvering

		perror( "Error initialising semaphore (7) " );

		if( shmdt( (void *) mem) == -1) perror(" Error detaching memory (myport) (7) ");

		if( shmctl( shmid, IPC_RMID, 0) == -1) 
			perror(" Error removing/deleting memory (myport) (7) ");

		exit(1);
	}


			/* Readers/Writters problem (mutex,rw_mutex) */
	if( (sem_init( &(mem->mutex), 1, 1) ) == -1){

		perror( "Error initialising semaphore (7) " );

		if( shmdt( (void *) mem) == -1) perror(" Error detaching memory (myport) (7) ");

		if( shmctl( shmid, IPC_RMID, 0) == -1) 
			perror(" Error removing/deleting memory (myport) (7) ");

		exit(1);
	}

	if( (sem_init( &(mem->rw_mutex), 1, 1) ) == -1){

		perror( "Error initialising semaphore (8) " );

		if( shmdt( (void *) mem) == -1) perror(" Error detaching memory (myport) (8) ");

		if( shmctl( shmid, IPC_RMID, 0) == -1) 
			perror(" Error removing/deleting memory (myport) (8) ");

		exit(1);
	}


			/* Priority Queues (7) ( "failed/not approved applications) */


	if( (sem_init( &(mem->pr_Small), 1, 0) ) == -1){

		perror( "Error initialising semaphore (9) " );

		if( shmdt( (void *) mem) == -1) perror(" Error detaching memory (myport) (9) ");

		if( shmctl( shmid, IPC_RMID, 0) == -1) 
			perror(" Error removing/deleting memory (myport) (9) ");

		exit(1);
	}

	if( (sem_init( &(mem->pr_Medium), 1, 0) ) == -1){

		perror( "Error initialising semaphore (10) " );

		if( shmdt( (void *) mem) == -1) perror(" Error detaching memory (myport) (10) ");

		if( shmctl( shmid, IPC_RMID, 0) == -1) 
			perror(" Error removing/deleting memory (myport) (10) ");

		exit(1);
	}



	if( (sem_init( &(mem->pr_Large), 1, 0) ) == -1){

		perror( "Error initialising semaphore (11) " );

		if( shmdt( (void *) mem) == -1) perror(" Error detaching memory (myport) (11) ");

		if( shmctl( shmid, IPC_RMID, 0) == -1) 
			perror(" Error removing/deleting memory (myport) (11) ");

		exit(1);
	}

	if( (sem_init( &(mem->pr_uSM), 1, 0) ) == -1){// priority queue for Small vessels willing to pay for Medium (upgrade)

		perror( "Error initialising semaphore (12) " );

		if( shmdt( (void *) mem) == -1) perror(" Error detaching memory (myport) (12) ");

		if( shmctl( shmid, IPC_RMID, 0) == -1) 
			perror(" Error removing/deleting memory (myport) (12) ");

		exit(1);
	}

	if( (sem_init( &(mem->pr_uSL), 1, 0) ) == -1){// priority queue for Small vessels willing to pay for Large (upgrade)

		perror( "Error initialising semaphore (13) " );

		if( shmdt( (void *) mem) == -1) perror(" Error detaching memory (myport) (13) ");

		if( shmctl( shmid, IPC_RMID, 0) == -1) 
			perror(" Error removing/deleting memory (myport) (13) ");

		exit(1);
	}

	if( (sem_init( &(mem->pr_uML), 1, 0) ) == -1){// priority queue for Medium vessels willing to pay for Large (upgrade)

		perror( "Error initialising semaphore (14) " );

		if( shmdt( (void *) mem) == -1) perror(" Error detaching memory (myport) (14) ");

		if( shmctl( shmid, IPC_RMID, 0) == -1) 
			perror(" Error removing/deleting memory (myport) (14) ");

		exit(1);
	}





		/* Initialising variables */

	mem->counter = 0;

	mem->readers_count = 0;

		/* Money*/

	mem->total_money = 0;
	mem->number_vessels = 0;

	mem->total_money_S = 0;
	mem->total_money_M = 0;
	mem->total_money_L = 0;

	mem->number_S = 0;
	mem->number_M = 0;
	mem->number_L = 0;

		/* Time */

	struct timeval tv;

	gettimeofday(&tv,NULL);
	start = tv.tv_sec;

	mem->start = tv.tv_sec;			// this is our starting point in time
	printf("Mem->start is : %f", mem->start);


	mem->total_waiting = 0;

	mem->waiting_S = 0;
	mem->waiting_M = 0;
	mem->waiting_L = 0;


	mem->time_period = atoi( Stime_period);

	/* Initialising Positions */

	int i = 0;
	char *temp = NULL;
	char *temp1 = ( ( (char *) mem) + sizeof(struct SHMemory) );	// in order to reach the block of memory we want

	Position * current = (Position *) temp1;


	/* Small */

	mem->offset_Small = ( (char *) current) -( (char *) mem);// current_address -base_address

	mem->S_available = capacity1;			// available small positions
	mem->S_total = capacity1;				//total small positions

	mem->cost_S =  cost1;

	for( i = 0; i < capacity1; i++){		//small

		current->available = true;
		strcpy(current->postype,"S");

		current->counter = i+1;

		current = current + 1;

	}

	/* Medium */

	mem->offset_Medium = ( (char *) current) -( (char *) mem) ;

	mem->M_available = capacity2;			// available medium positions
	mem->M_total = capacity2;				//total medium positions

	mem->cost_M = cost2;

	for( i = 0; i < capacity2; i++){		//medium

		current->available = true;

		strcpy(current->postype,"M");

		current->counter = i+1;

		current = current + 1;

	}


	/* Large */

	mem->offset_Large = ( (char *) current) -( (char *) mem) ;

	mem->L_available = capacity3;			// available large positions
	mem->L_total = capacity3;				//total large positions

	mem->cost_L = cost3;

	for( i = 0; i < capacity3; i++){		//large

		current->available = true;

		strcpy(current->postype,"L");

		current->counter = i+1;

		current = current + 1;

	}




	/*PLrecords_name, Log_name, Application, application.vesseltype, application.utype offsets */


	char* tempz1;
	App* tempz2;

		/*PL records */

	tempz1 = ( ( (char*) mem ) + (capacity1+capacity2+capacity3) * sizeof(Position));

	mem->offset_PLname = (long) ( tempz1 - ( (char*) mem) );		// current - base

	for( i = 0; i < 20; i++){

		*tempz1 = PLrecords_name[i]; 

		if( *tempz1 == '\0' ) break;

		tempz1 = tempz1 + 1;

	}

		/* Log_name offset */

	tempz1 = ( ( (char*) mem ) + (capacity1+capacity2+capacity3) * sizeof(Position) + 20 * sizeof(char) );

	mem->offset_logname = (long) (tempz1 - ( (char*) mem) );

	for( i = 0; i < 10; i++){

		*tempz1 = log_name[i]; 

		if( *tempz1 == '\0' ) break;

		tempz1 = tempz1 + 1;

	}


		/* Application offset */

	tempz1 = ( ( (char*) mem ) + (capacity1+capacity2+capacity3) * sizeof(Position) + 20 * sizeof(char) + 10 * sizeof(char) );

	mem->offset_application = (long) ( tempz1 - ( (char*) mem) );

	tempz2 = (App*) ( ( (char*) mem) + mem->offset_application);




	/* Forking and executing */


	pid_t child_a, child_b, child_c;


		/*Port Master */

	child_a = fork();

	if( child_a == -1){

		perror("Error in fork (1) ");

	}else if( child_a == 0){

		int i = snprintf( NULL, 0, "%d",  shmid) + 1;		//+1 for NULL character
		char buffer[i];
		snprintf(buffer , i, "%d", shmid);

		execlp("./port-master","port-master", "-c", charges_name, "-s", buffer, (char *) NULL);
		perror("Error in exec (1) ");


			/* On Error, removing shared memory */
		if( shmdt( (void *) mem) == -1) perror(" Error detaching memory (myport) (16) ");

		if( shmctl( shmid, IPC_RMID, 0) == -1) 
			perror(" Error removing/deleting memory (myport) (16) ");

		exit(1);
	}

		/* Monitor */

	child_b = fork();

	if( child_b == -1){

		perror("Error in fork (2) ");

	}else if( child_b == 0){

		int i = snprintf( NULL, 0, "%d",  shmid) + 1;		//+1 for NULL character
		char buffer[i];
		snprintf(buffer , i, "%d", shmid);

		execlp("./monitor","monitor", "-d", time_m, "-t", stattime, "-s", buffer, (char *) NULL);
		perror("Error in exec (2) ");


			/* On Error, removing shared memory */
		if( shmdt( (void *) mem) == -1) perror(" Error detaching memory (myport) (17) ");

		if( shmctl( shmid, IPC_RMID, 0) == -1) 
			perror(" Error removing/deleting memory (myport) (17) ");

		exit(1);
	}

		/* Vessel1 */

	child_c = fork();		// Small

	if( child_c == -1){

		perror("Error in fork (3) ");

	}else if( child_c == 0){

		int i = snprintf( NULL, 0, "%d",  shmid) + 1;		//+1 for NULL character
		char buffer[i];
		snprintf(buffer , i, "%d", shmid);

//		execlp("./vessel","vessel", "-t", "S", "-u","M", "-p", "parkperiod","-m","mantime","-s", buffer, (char *) NULL);

		execlp("./vessel","vessel", "-t", "S", "-u","M", "-p", "5","-m","5","-s", buffer, (char *) NULL);

		perror("Error in exec (1) ");


			/* On Error, removing shared memory */
		if( shmdt( (void *) mem) == -1) perror(" Error detaching memory (myport) (18) ");

		if( shmctl( shmid, IPC_RMID, 0) == -1) 
			perror(" Error removing/deleting memory (myport) (18) ");

		exit(1);
	}


	wait(NULL);			// wating for everyone to finish
	wait(NULL);
	wait(NULL);





/* Destroying the semaphores */

	if( sem_destroy( &(mem->check)) == -1){

		perror(" Sem_destroy error in myport (1) ");
		exit(1);
	}

	if( sem_destroy( &(mem->queue) )== -1){

		perror(" Sem_destroy error in myport (2) ");
		exit(1);
	}

	if( sem_destroy( &(mem->full)) == -1){

		perror(" Sem_destroy error in myport (3) ");
		exit(1);
	}

	if( sem_destroy( &(mem->in_evaluation) ) == -1){

		perror(" Sem_destroy error in myport (4) ");
		exit(1);
	}

	if( sem_destroy( &(mem->maneuver_pm)) == -1){

		perror(" Sem_destroy error in myport (5) ");
		exit(1);
	}

	/* Readers/Writers semaphores */

	if( sem_destroy( &(mem->rw_mutex)) == -1){

		perror(" Sem_destroy error in myport (6) ");
		exit(1);
	}

	if( sem_destroy( &(mem->mutex)) == -1){

		perror(" Sem_destroy error in myport (7) ");
		exit(1);
	}


	/* Priority Queues */

	if( sem_destroy( &(mem->pr_Small)) == -1){

		perror(" Sem_destroy error in myport (8) ");
		exit(1);
	}

	if( sem_destroy( &(mem->pr_Medium)) == -1){

		perror(" Sem_destroy error in myport (9) ");
		exit(1);
	}

	if( sem_destroy( &(mem->pr_Large)) == -1){

		perror(" Sem_destroy error in myport (10) ");
		exit(1);
	}

	if( sem_destroy( &(mem->pr_uSM)) == -1){

		perror(" Sem_destroy error in myport (11) ");
		exit(1);
	}

	if( sem_destroy( &(mem->pr_uSL)) == -1){

		perror(" Sem_destroy error in myport (12) ");
		exit(1);
	}

	if( sem_destroy( &(mem->pr_uML)) == -1){

		perror(" Sem_destroy error in myport (13) ");
		exit(1);
	}

	/* Detaching Shared Memory (and deleting, if it is the last detach) */

	if( shmctl( shmid, IPC_RMID, 0) == -1) // last detach will destriy the shared memory
		perror(" Error removing/deleting memory (myport) (19) ");

	if( shmdt( (void *) mem) == -1) perror(" Error detaching memory (myport) (19) ");


	printf(" Exiting myport \n");

	return 0;
	}


