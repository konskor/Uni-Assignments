#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <time.h>
#include <sys/time.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <unistd.h>
#include <semaphore.h>

#include <signal.h>
#include <errno.h>

#include "MyStructs.hpp"




/* GLOBAL VARIABLE */
int stop = 1;


/* Signal_handler for SIGINT */
void signal_handler( int sig){

	signal(SIGINT,signal_handler);

	stop = 0;


}

/* Function to handle situations, if the prematurely exits due to SIGINT  */
/*
void cleanup(SHM* mem, Position* position, bool in_port, int mantime){

	if( in_port == false){		// just detach memory

		/******************************* LOG FILE ****************************************

		double now;

		struct timeval tv;
		gettimeofday(&tv,NULL);
		now = tv.tv_sec;

		sem_wait( &(mem->rw_mutex) );


		FILE* fp = fopen( mem->log_name, "a");
		if(fp == NULL){

			perror("Error opening log file in vessel (11) ");
			exit(1);

		}

		fprintf(fp,"Vessel %ld EMERGENCY EXIT at %f (OUTSIDE PORT) \n", (long) getpid(),       now - mem->start );

		if( fclose(fp) == -1){

			perror("Error closing log file in vessel (11) ");
			exit(1);

		}

		sem_post( &(mem->rw_mutex) );		// finished writing in memory


		if( shmdt( (void *) mem )== -1){	// detaching shared memory and exiting

			perror(" Error detaching memory in vessel (1) ");
			exit(1);
		}

	}else{		// in_port = true,

		sem_wait( &(mem->queue) );		// enter queue, want to leave

		/* Write application form, when waking up 

		sem_wait( &(mem->rw_mutex));	// write in memory

		mem->application.pid = getpid();
		mem->application.offset = ( ( (char*) position) - ( (char*) mem) );

		sem_post( &(mem->rw_mutex) );	// stop writing in memory

		sem_wait( &(mem->in_evaluation) );	// wait untill port-master read our app and decide

		sleep( mantime );

		sem_post( &(mem->maneuver_pm) );	//wakeup port-master, done maneuvering

		/******************************* LOG FILE ***************************************

		sem_wait( &(mem->rw_mutex));

		FILE* fp = fopen( mem->log_name, "a");
		if(fp == NULL){

			perror("Error opening log file in vessel (12) ");
			exit(1);

		}

		fprintf(fp, " Vessel %ld EMERGENCY EXIT, paid %f at %f (IN PORT) \n", (long) getpid() );

		if( fclose(fp) == -1){

			perror("Error closing log file in vessel (12) ");
			exit(1);

		}
		sem_post( &(mem->rw_mutex) );		// finished writing in memory

		/* Detaching memory and leaving 

		if( shmdt( (void *) mem )== -1){	// detaching shared memory and exiting

			perror(" Error detaching memory in vessel (2) ");
			exit(1);
		}


	}

}

*/





int main(int argc, char* argv[]){

	signal( SIGINT, signal_handler);

	if( argc != 11){

		printf("Please enter the correct arguments \n");
		return -1;
	}

	int i;
	char* type, *postype, *name;

	int shmid, mantime,parkperiod;
	bool upgrade = false;


	for( i = 0; i < argc; i++){

		if( strcmp(argv[i] , "-t") == 0 ){		//type

			type = argv[i+1]; 

		}else if(strcmp(argv[i] , "-u") == 0){		// upgrade type

			upgrade = true;
			postype = argv[i+1];

		}else if(strcmp(argv[i] , "-p") == 0){		// park period

			parkperiod = atoi(argv[i+1]);

		}else if(strcmp(argv[i] , "-m") == 0){		// maneuver time

			mantime = atoi( argv[i+1] );

		}else if(strcmp(argv[i] , "-s") == 0){	// shmid = shared memory id

			shmid = atoi( argv[i+1] );

		// name of vessel (might use pid)
		}

	}

	/* Attaching memory */

	SHM *mem;

	mem = (SHM*) shmat(shmid, NULL, 0);

	if( mem  == (void *) -1 ){
 
		perror(" Error attaching memory (in vessel) "); 
		exit(1);
	}

	/* Initiating local pointers */

		/* Log_name */
	char *log = ( (char*) mem ) + mem->offset_logname;
//	printf("(Vessel) log_name is : %s \n", log);

		/* Application pointer */
	App* application = (App*) ( ( (char*) mem) + mem->offset_application);



	/* Time */

	double arrive_queue = 0;
	double now = 0;

	struct timeval tv;


/*************************************** OUTSIDE OF PORT *********************************/

	/* Arrival Queue */
	gettimeofday(&tv,NULL);
	arrive_queue = tv.tv_sec;		//just arrived in queue


	/******************************* LOG FILE **************************************/

	signal(SIGINT,SIG_IGN);

	if( sem_wait( &(mem->rw_mutex) ) == -1){
/*
		if( errno == EINTR){			//interrupted by signal
			sem_post( &(mem->rw_mutex));

			cleanup(mem,NULL,false, mantime);

			exit(0);
		}
*/
		perror("Sem_wait error in vessel (1) ");
		exit(1);

	}


	FILE *fp;

	fp = fopen( log, "a");
	if(fp == NULL){

		perror("Error opening log file in vessel (1) ");
		exit(1);

	}

	fprintf(fp, " Vessel %ld  is waitng in queue at %f (ENTER PORT) \n", (long) getpid(), (arrive_queue - mem->start) );		// mem->start = starting point in time

	if( fclose(fp) == -1){

		perror("Error closing log file in vessel (1) ");
		exit(1);

	}

	sem_post( &(mem->rw_mutex) );


/* Starting the procedure */

/*	Inform port-master that a new application/vessel has arrived in queue, 
	and check if we have vessels/applications in priority Queues that need our immediate attention 

	( example: if exists vessel of size  M in  pr_medium(semaphore =>sem_getvalue() ) 
	and a M-position has been freed in the port, prioritize this vessel to enter ) 

	If multiple priority vessels, port-master has a list of the (failed) applications in FCFS order (more on that later)

*/

//	signal(SIGINT,signal_handler); // while in queue,or before writing the application form ==> can be interrupted


	sem_post( &(mem->check) );		// inform port-master that a new application has arrived
	if( sem_wait( &(mem->queue) ) ==-1){		// queue = 0 ,block in application queue, until portmaster says otherwise ( there might be an application that needs immediate attention...  PRIORITY)

		if( errno == EINTR){

			sem_post( &(mem->queue) );

		//	cleanup(mem,NULL,false,mantime);
			exit(0);
		}

		perror("Sem_wait error in vessel (2) ");
		exit(1);

	}

//	signal(SIGINT,SIG_IGN);				// after getting out of queue and untill we know if we can enter , we don't want the programm to be interrupted
	//untill we know if this vessel gets in, we shouldn't interrupt it



	/*Writing the application in Node evaluate */

	if( sem_wait( &(mem->rw_mutex) ) ==-1 ){//no other writers (and of course no readers), in shared memory

		if( errno == EINTR){

			sem_post( &(mem->rw_mutex) );

//			cleanup(mem,NULL,false,mantime);
			exit(0);
		}

		perror("Sem_wait error in vessel (3) ");
		exit(1);

	}




//	signal(SIGINT,SIG_IGN);				// after writing the application and untill we know if we can enter , we don't want the programm to be interrupted
	//untill we know if this vessel gets in, we shouldn't interrupt it


	printf("Writing application in vessel \n");





	application->enter = true;
	application->pid = getpid();

	strcpy( application->type, type);

	application->arrive_queue = arrive_queue;		//when did it arrive in queue (stats)

	application->upgrade = upgrade;
	if( upgrade == true){

		strcpy(application->utype,postype);

	}

	application->PQ = false;

	/******************************* LOG FILE ****************************************/

	fp = fopen( log, "a");
	if(fp == NULL){

		perror("Error opening log file in vessel (2) ");
		exit(1);

	}

	gettimeofday(&tv,NULL);
	now = tv.tv_sec;

	fprintf(fp, " Vessel %ld has written the application form at %f (ENTER PORT) \n", (long) getpid(), (now - mem->start) );

	if( fclose(fp) == -1){

		perror("Error closing log file in vessel (2) ");
		exit(1);

	}



	sem_post( &(mem->rw_mutex) );		// finished writing in memory

	sem_post( &(mem->full) );	// wake up portmaster => he will read the application and decide


//	signal(SIGINT,SIG_IGN);				// during the evaluation process, we don't want the programm to be interrupted
	//untill we know if this vessel gets in, we shouldn't interrupt it



	if( sem_wait( &(mem->in_evaluation) ) == -1){	// wait to be evaluated by port_master

		perror("Sem_wait error in vessel (4) ");
		exit(1);

	}

	printf("(Vessel) woke up from evaluation ******* \n") ;


	/* Read our application (in memory) if approved or not */

	if( sem_wait( &(mem->mutex) ) == -1){
/*
		if( errno == EINTR){

			sem_post( &(mem->mutex) );

			cleanup(mem,NULL,false,mantime);
			exit(0);
		}
*/
		perror("Sem_wait error in vessel (5) ");
		exit(1);

	}

	mem->readers_count++;
	if( mem->readers_count == 1){

		if( sem_wait( &(mem->rw_mutex) ) == -1){	//if anyone writes, don't read
/*
			if( errno == EINTR){

				sem_post( &(mem->rw_mutex) );

				cleanup(mem,NULL,false,mantime);
				exit(0);
			}
*/

			perror("Sem_wait error in vessel (6) ");
			exit(1);

		}
	}

//	signal(SIGINT,signal_handler);			//re-enabling signal_handler


	printf("(Vessel) Reading approval ******* \n") ;

	sem_post( &(mem->mutex) );

	bool approval = application->approval;

	Position* position;
	if( approval == true ){

		position = (Position *) ( ( (char*) mem ) + application->offset );

	}else if( (approval == false) && ( stop == 0) ){	//heading towards PQ, but SIGINT was caught so we will detach and exit

		sem_wait( &(mem->mutex) );
		mem->readers_count--;
		if( mem->readers_count == 0)		sem_post( &(mem->rw_mutex));
		sem_post( &(mem->mutex) );
		//finished reading in memory

//		cleanup(mem,NULL,false,mantime);
		exit(0);
	}


//	signal(SIGINT,SIG_IGN);					//don't interrupt semaphore
	if( sem_wait( &(mem->mutex) ) == -1){
/*
		if( errno == EINTR){

			sem_post( &(mem->mutex) );

			cleanup(mem,NULL,false,mantime);
			exit(0);
		}
*/
		perror("Sem_wait error in vessel (7) ");
		exit(1);

	}

//	signal(SIGINT,signal_handler);			//re-enabling signal handler

	mem->readers_count--;
	if( mem->readers_count == 0)		sem_post( &(mem->rw_mutex));
	sem_post( &(mem->mutex) );

	// finished reading memory, free the Node evaluate


	if( approval == false){		// put the vessel in the appropriate "Priority Queue"

//		signal( SIGINT,signal_handler);


		/* Arrival Queue */
		gettimeofday(&tv,NULL);
		arrive_queue = tv.tv_sec;		//waiting again, now in priority queue

		sem_post( &(mem->check) );		//inform port-master , another (failed) app arrived
		// this way we will prevent port-master from blocking in semaphore check



		/******************************* LOG FILE ****************************************/

		if( sem_wait( &(mem->rw_mutex) ) == -1){
/*
			if( errno == EINTR){			// interrupted by signal

				sem_post( &(mem->rw_mutex) );

				cleanup(mem,NULL,false,mantime);
				exit(0);
			}
*/
			perror("Sem_wait error in vessel (8) ");
			exit(1);

		}


		fp = fopen( log, "a");
		if(fp == NULL){

			perror("Error opening log file in vessel (3) ");
			exit(1);

		}

		gettimeofday(&tv,NULL);
		now = tv.tv_sec;

		fprintf(fp, " Vessel %ld couldn't go inside the port  at %f => entering PQ (ENTER PORT) \n", (long) getpid(), (now - mem->start) );

		if( fclose(fp) == -1){

			perror("Error closing log file in vessel (3) ");
			exit(1);

		}

		sem_post( &(mem->rw_mutex) );

		signal(SIGINT,signal_handler);		// vessel can be interruptexd

		if( upgrade == true){

			if( (strcmp(type,"S") == 0) && ( strcmp(postype,"M") == 0 )) {

				if( sem_wait( &(mem->pr_uSM) ) == -1){		// wait in Priority Queue SM

					if( errno == EINTR){

						sem_post( &(mem->pr_uSM) );

//						cleanup(mem,NULL,false,mantime);
						exit(0);
					}

					perror("Sem_wait error in vessel (9) ");
					exit(1);

				}



			}else if( (strcmp(type,"S") == 0) && ( strcmp(postype,"L") ) ){

				if( sem_wait( &(mem->pr_uSL) ) == -1){		// wait in Priority Queue SL

					if( errno == EINTR){

						sem_post( &(mem->pr_uSL) );

//						cleanup(mem,NULL,false,mantime);
						exit(0);
					}

					perror("Sem_wait error in vessel (10) ");
					exit(1);

				}

			}else if( (strcmp(type,"M") == 0) && ( strcmp(postype,"L") ) ){

				if( sem_wait( &(mem->pr_uML) ) == -1){		// wait in Priority Queue ML

					if( errno == EINTR){

						sem_post( &(mem->pr_uML) );

//						cleanup(mem,NULL,false,mantime);
						exit(0);
					}

					perror("Sem_wait error in vessel (11) ");
					exit(1);

				}


			}else if( strcmp(type,"L") == 0){	// can't upgrade to a better position 

				if( sem_wait( &(mem->pr_Large) ) == -1){

					if( errno == EINTR){

						sem_post( &(mem->pr_Large) );

//						cleanup(mem,NULL,false,mantime);
						exit(0);
					}

					perror("Sem_wait error in vessel (12) ");
					exit(1);

				}


			}

		}else{		// upgrade = false

			if( strcmp(type,"S") == 0){

				if( sem_wait( &(mem->pr_Small) ) == -1){		// wait in Priority Queue Small

					if( errno == EINTR){

						sem_post( &(mem->pr_Small) );

//						cleanup(mem,NULL,false,mantime);
						exit(0);
					}

					perror("Sem_wait error in vessel (13) ");
					exit(1);

				}




			}else if( strcmp(type,"M") == 0){

				if( sem_wait( &(mem->pr_Medium) ) == -1){		// wait in Priority Queue Medium

					if( errno == EINTR){

						sem_post( &(mem->pr_Medium) );

//						cleanup(mem,NULL,false,mantime);
						exit(0);
					}

					perror("Sem_wait error in vessel (14) ");
					exit(1);

				}

			}else if( strcmp(type,"L") == 0){

				if( sem_wait( &(mem->pr_Large) ) == -1){		// wait in Priority Queue Large

					if( errno == EINTR){

						sem_post( &(mem->queue) );

//						cleanup(mem,NULL,false,mantime);
						exit(0);
					}

					perror("Sem_wait error in vessel (15) ");
					exit(1);

				}

			}

		}

		/* Write the application again */

		signal(SIGINT,SIG_IGN);

		if( sem_wait( &(mem->rw_mutex) ) == -1){//no other writers (and of course no readers), in shared memory

			if( errno == EINTR){

				sem_post( &(mem->rw_mutex) );

//				cleanup(mem,NULL,false,mantime);
				exit(0);
			}

			perror("Sem_wait error in vessel (16) ");
			exit(1);

		}

//		signal(SIGINT,SIG_IGN);

		application->enter = true;
		application->pid = getpid();

//		strcpy(app_type, type);

		strcpy(application->type, type);

		application->arrive_queue = arrive_queue;		//when did it arrive in queue(stats)

		application->upgrade = upgrade;
		if( upgrade == true)	strcpy(application->utype,postype);

		application->PQ = true;

		/******************************* LOG FILE ****************************************/

		fp = fopen( log, "a");
		if(fp == NULL){

			perror("Error opening log file in vessel (4) ");
			exit(1);

		}

		gettimeofday(&tv,NULL);
		now = tv.tv_sec;

		fprintf(fp, " Vessel %ld has written the application form again at %f, after leaving PQ (ENTER PORT) \n", (long) getpid(), (now - mem->start) );


		if( fclose(fp) == -1){

			perror("Error closing log file in vessel (4) ");
			exit(1);

		}


		sem_post( &(mem->rw_mutex) );		// finished writing in memory

		sem_post( &(mem->full) );// wake up portmaster => he will read the application and decide


//		signal(SIGINT,SIG_IGN);


		if( sem_wait( &(mem->in_evaluation) ) == -1){	// wait to be evaluated by port_master

			perror("Sem_wait error in vessel (17) ");
			exit(1);

		}


/*	 This time , the application will certainly be approved 
	( we woke up the vessel because we found  an empty space ), 
	port-master will give us the offset to the position in port(shared Memory) */

	/* Read our application (in memory) , we care about the offset */

	if( sem_wait( &(mem->mutex) ) == -1){
/*
		if( errno == EINTR){

			sem_post( &(mem->mutex) );

			cleanup(mem,NULL,false,mantime);
			exit(0);
		}
*/
		perror("Sem_wait error in vessel (18) ");
		exit(1);

	}
	mem->readers_count++;
	if(mem->readers_count == 1){

		if( sem_wait( &(mem->rw_mutex) ) == -1){	//if anyone writes, don't read
/*
			if( errno = EINTR){

				sem_post( &(mem->queue) );

				cleanup(mem,NULL,false,mantime);
				exit(0);
			}
*/
			perror("Sem_wait error in vessel (19) ");
			exit(1);

		}
	}
	sem_post( &(mem->mutex) );

	/* From this point on, we have "entered" the port */
 
	position = (Position *) ( ( (char*) mem ) + application->offset );

	if( sem_wait( &(mem->mutex) ) == -1){
/*
		if( errno = EINTR){

			sem_post( &(mem->queue) );

			cleanup(mem,position,true,mantime);
			exit(0);
		}
*/
		perror("Sem_wait error in vessel (20) ");
		exit(1);

	}
	mem->readers_count--;
	if( mem->readers_count == 0)		sem_post( &(mem->rw_mutex));
	sem_post( &(mem->mutex) );

	// finished reading memory, free the Node evaluate

		// the port master has the application of the vessel (which wasn't serviced) , saved in its list


	}




	/******************************* LOG FILE ****************************************/

	if( sem_wait( &(mem->rw_mutex) ) == -1){
/*
		if( errno = EINTR){

			sem_post( &(mem->queue) );

			cleanup(mem,position,true,mantime);		//now in port
			exit(0);
		}
*/
		perror("Sem_wait error in vessel (21) ");
		exit(1);

	}



	fp = fopen( log, "a");
	if(fp == NULL){

		perror("Error opening log file in vessel (5) ");
		exit(1);

	}

	gettimeofday(&tv,NULL);
	now = tv.tv_sec;



	fprintf(fp, "Vessel %ld arrived in port at %f =>maneuvering (ENTER PORT) \n", (long) getpid(), (now - mem->start) );

	if( fclose(fp) == -1){

		perror("Error closing log file in vessel (5) ");
		exit(1);

	}

	sem_post( &(mem->rw_mutex) );

	/* Manuever */

	sleep(mantime);


/*
	if( sleep(mantime) > 0){		//interrupted by a singal (SIGINT)

		sem_post( &(mem->maneuver_pm) );

		cleanup(mem,position,true,mantime);

		exit(0);

	}
*/


	sem_post( &(mem->maneuver_pm) );	// wakeup portmaster, we finished maneuvering, 
// check next application

	/******************************* LOG FILE ****************************************/

	if( sem_wait( &(mem->rw_mutex) ) == -1){
/*
		if( errno = EINTR){

			sem_post( &(mem->rw_mutex) );

			cleanup(mem,position,true,mantime);
			exit(0);
		}
*/
		perror("Sem_wait error in vessel (22) ");
		exit(1);

	}





	fp = fopen( log, "a");
	if(fp == NULL){

		perror("Error opening log file in vessel (6) ");
		exit(1);

	}

	gettimeofday(&tv,NULL);
	now = tv.tv_sec;

	fprintf(fp, " Vessel %ld done maneuvering at %f =>sleep() and calcluate cost until now (ENTER PORT) \n", (long) getpid(), (now - mem->start) );

	if( fclose(fp) == -1){

		perror("Error closing log file in vessel (6) ");
		exit(1);

	}

	sem_post( &(mem->rw_mutex) );




	/* In its position now, does its job/sleep() */

	int k = parkperiod/2;

	sleep(k);

/*
	if( sleep(k) > 0){

		cleanup(mem,position,true,mantime);
		exit(0);
	}
*/



		/* Wakeup and read cost(until now) from public ledger */

	if( sem_wait( &(mem->mutex) ) == -1){		// start reading from memory
/*
		if( errno = EINTR){

			sem_post( &(mem->mutex) );

			cleanup(mem,position,true,mantime);
			exit(0);
		}
*/

		perror("Sem_wait error in vessel (23) ");
		exit(1);

	}

	mem->readers_count++;
	if(mem->readers_count == 1){

		if( sem_wait( &(mem->rw_mutex) ) == -1){	//if anyone writes, don't read
/*
			if( errno = EINTR){

				sem_post( &(mem->queue) );

				cleanup(mem,position,true,mantime);
				exit(0);
			}
*/
			perror("Sem_wait error in vessel (24) ");
			exit(1);

		}


	}

	sem_post( &(mem->mutex) );


	double duration;
//	double now;

	double cost;

	/* Get current time */
	gettimeofday(&tv,NULL);
	now = tv.tv_sec;

	/* Calculating cost (until now) */
	 duration = (now - position->arrival_time);

	if( strcmp(type,"S") == 0){

		cost = ( ( duration/mem->time_period) + 1) *  mem->cost_S;

	}else if( strcmp(type, "M") == 0){

		cost = ( ( duration/mem->time_period) +1) * mem->cost_M;

	}else if( strcmp(type, "L") == 0){

		cost = ( (duration/mem->time_period) +1) * mem->cost_L;

	}




	if( sem_wait( &(mem->mutex) ) == -1){
/*
		if( errno = EINTR){

			sem_post( &(mem->mutex) );

			cleanup(mem,position,true,mantime);
			exit(0);
		}
*/
		perror("Sem_wait error in vessel (25) ");
		exit(1);

	}
	mem->readers_count--;
	if(mem->readers_count == 0)		sem_post( &(mem->rw_mutex));
	sem_post( &(mem->mutex) );

	/******************************* LOG FILE ****************************************/

	if( sem_wait( &(mem->rw_mutex) ) == -1){
/*
		if( errno = EINTR){

			sem_post( &(mem->rw_mutex) );

			cleanup(mem,position,true,mantime);
			exit(0);
		}
*/
		perror("Sem_wait error in vessel (26) ");
		exit(1);

	}


	fp = fopen( log, "a");
	if(fp == NULL){

		perror("Error opening log file in vessel (9) ");
		exit(1);

	}

	gettimeofday(&tv,NULL);
	now = tv.tv_sec;

	fprintf(fp, " Vessel %ld calculated cost (until now) = %f at %f (EXIT PORT) \n", (long) getpid(), cost, (now - mem->start) );


	if( fclose(fp) == -1){

		perror("Error closing log file in vessel (9) ");
		exit(1);

	}

	sem_post( &(mem->rw_mutex) );

	/* Continue sleeping */

	sleep(parkperiod - k);


/* Now we want to leave ( almost same procedure as above) */

	/* Arrival queue */
	gettimeofday(&tv,NULL);
	arrive_queue = tv.tv_sec;		//just arrived in queue


	/******************************* LOG FILE ****************************************/

	if( sem_wait( &(mem->rw_mutex) ) == -1){
/*
		if( errno = EINTR){

			sem_post( &(mem->rw_mutex) );

			cleanup(mem,position,true,mantime);
			exit(0);
		}
*/
		perror("Sem_wait error in vessel (27) ");
		exit(1);

	}

	fp = fopen( log, "a");
	if(fp == NULL){

		perror("Error opening log file in vessel (7) ");
		exit(1);

	}

//	gettimeofday(&tv,NULL);
//	now = tv.tv_sec;

	fprintf(fp, " Vessel %ld arrived in queue at %f (EXIT PORT) \n", (long) getpid(), arrive_queue-mem->start );

	if( fclose(fp) == -1){

		perror("Error closing log file in vessel (7) ");
		exit(1);

	}

	sem_post( &(mem->rw_mutex) );
	sem_post( &(mem->check) );		// inform port-master that a new application has arrived


	if( sem_wait( &(mem->queue) ) == -1){		// queue = 0 ,block in application queue, until portmaster says otherwise ( there might be an application that needs immediate attention...  PRIORITY)
/*
		if( errno = EINTR){

			sem_post( &(mem->queue) );

			cleanup(mem,position,true,mantime);
			exit(0);
		}
*/
		perror("Sem_wait error in vessel (28) ");
		exit(1);

	}




	/*Writing the application in "Application form" */


	if( sem_wait( &(mem->rw_mutex) ) == -1){//no other writers (and of course no readers), in shared memory
/*
		if( errno = EINTR){

			sem_post( &(mem->rw_mutex) );

			cleanup(mem,position,true,mantime);
			exit(0);
		}
*/

		perror("Sem_wait error in vessel (29) ");
		exit(1);

	}





	application->enter = false;		// false, because we want to leave
	application->pid = getpid();

	strcpy(application->type,type);

	application->offset =  ( ( (char*) position) - ( (char*) mem) );


	application->arrive_queue = arrive_queue;

	/******************************* LOG FILE ****************************************/

	fp = fopen( log, "a");
	if(fp == NULL){

		perror("Error opening log file in vessel (8) ");
		exit(1);

	}

	gettimeofday(&tv,NULL);
	now = tv.tv_sec;

	fprintf(fp, " Vessel %ld has written the application form at %f (EXIT PORT) \n", (long) getpid(), (now - mem->start) );

	if( fclose(fp) == -1){

		perror("Error closing log file in vessel (8) ");
		exit(1);

	}


	sem_post( &(mem->rw_mutex) );		// finished writing in memory



	sem_post( &(mem->full) );	// wake up portmaster => he will read the application and decide
	if( sem_wait( &(mem->in_evaluation) ) == -1){	// wait to be evaluated by port_master
/*
		if( errno = EINTR){

			sem_post( &(mem->in_evaluation) );

			cleanup(mem,position,true,mantime);
			exit(0);
		}
*/
		perror("Sem_wait error in vessel (30) ");
		exit(1);

	}



	/*Don't need to read our application (in memory) if approved or not */

	/* 1)The only way an exit application can fail is : someone is moving in port.
		however, by blocking port-master untill any vessel stops maneuvering => can't fail*/

	/* 2) We only want to leave, so the port-master doesn't have to give us any info
		Cost is calculated by port-master after we left the port */


	/******************************* LOG FILE ****************************************/

	if( sem_wait( &(mem->rw_mutex) ) == -1){
/*
		if( errno = EINTR){

			sem_post( &(mem->queue) );

			cleanup(mem,position,true,mantime);
			exit(0);
		}
*/
		perror("Sem_wait error in vessel (31) ");
		exit(1);

	}



	fp = fopen( log, "a");
	if(fp == NULL){

		perror("Error opening log file in vessel (9) ");
		exit(1);

	}

	gettimeofday(&tv,NULL);
	now = tv.tv_sec;

//	fprintf(fp, "Vessel %ld  maneuvering  (EXIT PORT) \n", (long) getpid() );

	fprintf(fp, " Vessel %ld maneuvering at %f (EXIT PORT) \n", (long) getpid(), (now - mem->start) );


	if( fclose(fp) == -1){

		perror("Error closing log file in vessel (9) ");
		exit(1);

	}

	sem_post( &(mem->rw_mutex) );


	/* Exiting port */

	sleep(mantime);

/*
	if( sleep(mantime) > 0){		// interrupted by SIGINT

		sem_post( &(mem->maneuver_pm));

		cleanup(mem,position,true,mantime);
		exit(0);
	}
*/


	sem_post( &(mem->maneuver_pm) );	// wakeup port-master, finished maneuvering
//let him check the next application

	/* From here, we are technically "outside" of port */


	/******************************* LOG FILE ****************************************/

	if( sem_wait( &(mem->rw_mutex) ) == -1){

/*
		if( errno == EINTR){

			sem_post( &(mem->rw_mutex) );

			cleanup(mem,NULL,false,mantime);
			exit(0);
		}
*/

		perror("Sem_wait error in vessel (32) ");
		exit(1);

	}


	fp = fopen( log, "a");
	if(fp == NULL){

		perror("Error opening log file in vessel (10) ");
		exit(1);

	}

	gettimeofday(&tv,NULL);
	now = tv.tv_sec;

//	fprintf(fp, "Vessel %ld done maneuvering => EXITING!!!  (EXIT PORT) \n", (long) getpid() );

	fprintf(fp, " Vessel %ld done maneuvering at %f =>EXITING!!!! (EXIT PORT) \n", (long) getpid(), (now - mem->start) );


	if( fclose(fp) == -1){

		perror("Error closing log file in vessel (10) ");
		exit(1);

	}

	sem_post( &(mem->rw_mutex) );


	/* Left the port, detaching memory */

/* Our job is almost done, detaching shared memory */

	if( shmdt( (void *)mem) == -1){

		perror("Detaching shared memory in vessel (33) " );
		exit(1);
	}

//	printf("Exiting vessel %ld\n", (long) getpid() );


	exit(0);

}














