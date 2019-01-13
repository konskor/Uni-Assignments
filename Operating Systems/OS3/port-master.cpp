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
#include "PriorityList.hpp"


int stop = 1;			// stop == 0 => don't pass through while loop (or break)

/*

void signal_handler(int sig){

	signal(SIGINT,signal_handler);

	stop = 0;

}
*/


int main(int argc, char* argv[]){

//	signal( SIGINT, signal_handler);

	if( argc != 5){

		printf("Please enter the correct arguments \n");
		return -1;
	}

	int i;
	char* charges_name;
	int shmid;

	for( i = 0; i < argc; i++){

		if( strcmp(argv[i] , "-c") == 0 ){

			charges_name = argv[i+1]; 

		}else if(strcmp(argv[i] , "-s") == 0){

			shmid = atoi( argv[i+1] );
			printf("Shmid in vessel %d\n",shmid);

		}

	}

	/* Time */

	struct timeval tv;
	double now = 0;


	signal(SIGINT,SIG_IGN);



	/*Opening Charges.txt file */

	FILE* fp;
	char buffer[10];

	char* type1, *scost1;
	char* type2, *scost2;
	char* type3, *scost3;

	double cost1, cost2, cost3;


	if( (fp = fopen( argv[2], "r") ) == NULL){

		perror("Error opening charges ");
		return -1;
	}

	/* Reading the contents */

	fgets(buffer,10,fp);	// read line

	type1 = strtok(buffer," ");			// Small
	scost1 = strtok(NULL," ,\n");

	cost1 = atoi( scost1);


	fgets(buffer,10,fp);

	type2 = strtok(buffer," ");			// Medium
	scost2 = strtok(NULL," ,\n");

	cost2 = atoi( scost2);



	fgets(buffer,10,fp);

	type3 = strtok(buffer," ");			// Large
	scost3 = strtok(NULL," ,\n");

	cost3 = atoi( scost3);



	if( fclose(fp) == -1){

		perror("Error Closing charges ");
		return -1;
	}

	/*Closing Charges.txt file */

	/* Attaching memory */

	SHM *mem;

	mem = (SHM*) shmat(shmid, NULL, 0);

	if( mem == (void *) -1 ){
 
		perror(" Error attaching memory (in vessel) "); 
		exit(1);
	}


	printf("(Port-Master) mem->start is: %f ", mem->start);

	/* Log name */
	char *log = ( (char*) mem ) + mem->offset_logname;
	printf("(Port-Master) log_name is : %s \n", log);

	/* PLrecords_name */

	char *PLrecords = ( (char*) mem) + mem->offset_PLname;
	printf("(Port-Master) PLrecords is : %s \n", log);

	/* Application */
	App *application = (App*) ( ( (char *) mem) + mem->offset_application);



//	signal(SIGINT, signal_handler);

	int k;

	bool PQ_find = false;		// if we find something in PQ => PQ_find = true
	bool approved = false;

	bool Position_find = false;


	PriorityList* PQ;
	PQ = new PriorityList();
	Position* current = NULL;

	Node* temp1;

	double duration = 0;		// duration of stay
	double temp_cost = 0;
	double departure = 0;

	char temp2[4];


	printf("Hi from port-master (1) \n");

/* Start the procedure */

while( stop != 0){		// wait for signal interrupt ( Control + C )


//	signal(SIGINT,SIG_IGN);

	//mem->check = 0 ,if: a) no application arrived , b) all application have been serviced

	// Can be interrupted

	if( sem_wait( &(mem->check) ) == -1){	// wait for an application to arrive in generic queue

		if( errno == EINTR){

			sem_post( &(mem->check) );

			continue;
		}

		perror("Sem_wait error in port-master(1) ");

		exit(1);

	}



	signal(SIGINT,SIG_IGN);

	/* Check if there is a priority application */


	if( PQ->head != NULL){		// we have a priority application 

		temp1 = PQ->head;



		/* Reading memory */

		if( sem_wait( &(mem->mutex) ) == -1){

			perror("Sem_wait error in port-master(2) ");

			exit(1);

		}
		mem->readers_count++;
		if( mem->readers_count == 1){

			if( sem_wait( &(mem->rw_mutex) ) == -1){

				perror("Sem_wait error in port-master(3)");
				exit(1);
			}
		}

		sem_post( &(mem->mutex) );


		while( temp1 != NULL){

			/* 6 cases */ 

			if( strcmp( temp1->queue_type, "S") == 0 ){	// "Small" PQ

				if( mem->S_available > 0 ){		// free small position

					PQ->deleteNode(temp1);
					temp1 = NULL;

					PQ_find = true;

					sem_post( &(mem->pr_Small) );
					break;

				}

			}else if( strcmp( temp1->queue_type, "M") == 0 ){	// "Medium" PQ

				if( mem->M_available > 0){

					PQ->deleteNode(temp1);
					temp1 = NULL;

					PQ_find = true;

					sem_post( &(mem->pr_Medium) );
					break;
				}

			}else if( strcmp( temp1->queue_type, "L") == 0 ){	// "Large" PQ

				if( mem->L_available > 0){

					PQ->deleteNode(temp1);
					temp1 = NULL;

					PQ_find = true;

					sem_post( &(mem->pr_Large) );
					break;
				}
			}else if( strcmp( temp1->queue_type, "uSM") == 0 ){// "Small willing to upgrade to Medium" PQ

				// First, check if either a Small or Medium position is available
				if( (mem->S_available > 0)  || (mem->M_available > 0) ){

					PQ->deleteNode(temp1);
					temp1 = NULL;

					PQ_find = true;

					sem_post( &(mem->pr_uSM) );
					break;
				}

			}else if( strcmp( temp1->queue_type, "uSL") == 0 ){// "Small willing to upgrade to Large" PQ

				// First, check if either a Small or Medium position is available
				if( (mem->S_available > 0)  || (mem->L_available > 0) ){

					PQ->deleteNode(temp1);
					temp1 = NULL;

					PQ_find = true;

					sem_post( &(mem->pr_uSL) );
					break;
				}

			}else if( strcmp( temp1->queue_type, "uML") == 0 ){// "Medium willing to upgrade to Large" PQ

				// First, check if either a Small or Medium position is available
				if( (mem->M_available > 0)  || (mem->L_available > 0) ){

					PQ->deleteNode(temp1);
					temp1 = NULL;

					PQ_find = true;

					sem_post( &(mem->pr_uML) );
					break;
				}
			
			}


			temp1 = temp1->next;		// proceed to the next node

		}

		// Finished reading in memory



		if( sem_wait( &(mem->mutex) ) == -1){

			perror("Sem_wait error in port-master (4) ");
			exit(1);

		}
		mem->readers_count--;
		if( mem->readers_count == 0)	sem_post( &(mem->rw_mutex));
		sem_post( &(mem->mutex) );

	}



	if( PQ_find == false){// we didn't find anything in PQ, wakeup next app in generic Queue

		sem_post( &(mem->queue) );

		/******************************* LOG FILE ****************************************/





		if( sem_wait( &(mem->rw_mutex) ) == -1){


			perror("Sem_wait error in port-master (5) ");
			exit(1);

		}

		fp = fopen( log, "a");
		if(fp == NULL){

			perror("Error opening log file in port-master (1) ");
			exit(1);

		}

		gettimeofday(&tv,NULL);
		now = tv.tv_sec;


		fprintf(fp, " Port-master wakes up GENERIC QUEUE vessel at %f \n", (now - mem->start) );

		if( fclose(fp) == -1){

			perror("Error closing log file in port-master (1) ");
			exit(1);

		}

		sem_post( &(mem->rw_mutex) );





	}else{		// PQfind == true

		/******************************* LOG FILE ****************************************/

		if( sem_wait( &(mem->rw_mutex) ) == -1){


			perror("Sem_wait error in port-master (6) ");

			exit(1);

		}


		fp = fopen( log, "a");
		if(fp == NULL){

			perror("Error opening log file in port-master (2) ");
			exit(1);

		}

		gettimeofday(&tv,NULL);
		now = tv.tv_sec;

		fprintf(fp, "Port-master wakes up PRIORITY QUEUE vessel at %f \n",  (now - mem->start) );

		if( fclose(fp) == -1){

			perror("Error closing log file in port-master (2) ");
			exit(1);

		}

		sem_post( &(mem->rw_mutex) );

	}


	PQ_find = false;		// resetting its value

	if( sem_wait( &(mem->full) ) == -1){		// wait for the "application form"

		perror("Sem_wait error in port-master (7) ");
		exit(1);
	}


	Position* current = NULL;
	char* tempz;


/* Read the application form and decide */

	if( sem_wait( &(mem->rw_mutex) ) == -1){		// might potentially write in memory

		perror("Sem_wait error in port-master (8) ");
		exit(1);
	}


	if( application->enter == true){	// the vessel wants to enter


		/******************************* LOG FILE ****************************************/

		fp = fopen( log, "a");
		if(fp == NULL){

			perror("Error opening log file in port-master (3) ");
			exit(1);

		}

		gettimeofday(&tv,NULL);
		now = tv.tv_sec;

		fprintf(fp, "Port-master checks application form of vessel %ld at %f (ENTERING) \n", (long) application->pid,  (now - mem->start) );



		if( fclose(fp) == -1){

			perror("Error closing log file in port-master (3) ");
			exit(1);

		}

		printf(" (port-master) Checking application form of vessel entering \n");



		/*If SIGINT was caught ==> stop = 0 .
			but for the current applicant (at least untill we know he approved or not) , and all the vessels in port, we try to ignore it */



		if( application->PQ == false) 
			mem->counter = mem->counter +2;		// the apps that need to be "serviced"
			// each vessel will want to enter(+1) and exit(+1)
			//increment the counter if it just arrived in queue(not coming from PQ)

		/* Small vessels */

	printf("(port-master) application type is %s ******* \n", application->type);


		if( strcmp( application->type , "S") == 0 ){	// vessel is Small

			printf(" (port-master) Checking application for small positions \n");


			if( mem->S_available > 0 ){
			//there is an empty Small position(and nobody moves in port)

				application->approval = true;
				mem->counter--;


				mem->number_vessels++;		// Vessels that came to the port(statistics)
				mem->number_S++;			// Small reserved positions (for statistics)

				approved = true;		// for later...

				/* Waiting time */

				gettimeofday(&tv,NULL);
				now = tv.tv_sec;

				mem->total_waiting+= now - application->arrive_queue;		// total waiting time
				mem->waiting_S+= now - application->arrive_queue;			// waiting time for S positions

				current = (Position *) ( ( ( (char*) mem ) + mem->offset_Small) );

				/* Search for the available space */

				for( i = 0; i < mem->S_total; i++){



					if( current->available == true){

						mem->S_available--;		// reducing counter
						Position_find = true;


						/* For Position */
						current->available = false;		// no longer available

						current->pid = application->pid;



						strcpy( current->type, application->type);

//						strcpy( current->type, mem->application.type);


							/* Arrival Time */
						gettimeofday(&tv,NULL);
						now = tv.tv_sec;
						current->arrival_time = now - mem->start;		// arrive in port

						/* For Application Form */

//						strcpy(mem->application.postype, "S");

						// calculate the offset
						application->offset = ( (char *) current) -( (char *) mem) ;

						break;	// found our available spot

					}

						current = current + 1;



				}


			}else if( (mem->S_available == 0) && ( application->upgrade == true)){
			//Searching if we can put the vessel in another position (upgrade)

				if( strcmp( application->utype , "M") == 0 ){

					/* Upgrade to Medium */

					if( mem->M_available > 0 ){
					//there is an empty Medium position(and nobody moves in port)

						application->approval = true;
						mem->counter--;

						mem->number_vessels++;		// Vessels that came to the port(statistics)
						mem->number_M++;			// Medium reserved positions (for statistics)

						approved = true;		// for later...

						/* Waiting time */

						gettimeofday(&tv,NULL);
						now = tv.tv_sec;

						mem->total_waiting+= now - application->arrive_queue;		// total waiting time
						mem->waiting_M+= now - application->arrive_queue;			// waiting time for M positions


						Position* current;

						current = (Position *) ( ( ( (char*) mem ) + mem->offset_Medium) );

						/* Search for the available space */

						for( i = 0; i < mem->M_total; i++){

							if( current->available == true){

								mem->M_available--;		// reducing counter

								/* For Position */
								current->available = false;		// no longer available

								current->pid = application->pid;

								strcpy(current->type, application->type);


//								strcpy( current->type, mem->application.type);

									/*Arrival Time */
								gettimeofday(&tv,NULL);
								now = tv.tv_sec;
								current->arrival_time = now - mem->start;	// arrive in port

								/* For Application Form */

//								strcpy(mem->application.postype, "M");

								// calculate the offset
								application->offset = ( (char *) current) -( (char *) mem);

								break;	// found our available spot

							}

							current = current + 1;

						}

					}else{		// no available spots, neither for its size nor upgrade(M)

						application->approval = false;
						mem->counter--;

						approved = false;		// for later...

						strcpy(temp2,"uSM");

					/* Add a new Node in our list (which keeps the order of our failed apps*/
						PQ->AddNode(temp2);

					}

				}else if( strcmp( application->utype , "L") == 0 ){

					/* Upgrade to Large */

					if( mem->L_available > 0 ){
					//there is an empty Medium position(and nobody moves in port)

						application->approval = true;
						mem->counter--;

						mem->number_vessels++;		// Vessels that came to the port(statistics)
						mem->number_L++;			// Large reserved positions (for statistics)

						approved = true;		// for later...


						/* Waiting time */

						gettimeofday(&tv,NULL);
						now = tv.tv_sec;

						mem->total_waiting+= now - application->arrive_queue;// total waiting time
						mem->waiting_L+= now - application->arrive_queue;// waiting time for L positions


						Position* current;

						current = (Position *) ( ( ( (char*) mem ) + mem->offset_Large) );

						/* Search for the available space */

						for( i = 0; i < mem->L_total; i++){

							if( current->available == true){

								mem->L_available--;		// reducing counter

								/* For Position */
								current->available = false;		// no longer available

								current->pid = application->pid;

								strcpy( current->type, application->type);

//								strcpy( current->type, mem->application.type);

									/* Arrival Time */
								gettimeofday(&tv,NULL);
								now = tv.tv_sec;
								current->arrival_time = now - mem->start;	// arrive in port

								/* For Application Form */

//								strcpy(mem->application.postype, "L");

								// calculate the offset
								application->offset = ( (char *) current) -( (char *) mem);

								break;	// found our available spot

							}

							current = current + 1;



						}


					}else{// no available spots, neither for its size(S) nor upgrade(L)

						application->approval = false;

						approved = false;		// for later...

						strcpy(temp2,"uSL");

					/* Add a new Node in our list (which keeps the order of our failed apps*/
						PQ->AddNode(temp2);

					}

				}

			}else if( (mem->S_available == 0) && ( application->upgrade == false) ){
					// no available spots for the (non-upgradeable) Small vessel

				application->approval = false;

				approved = false;		// for later...

				strcpy(temp2,"S");

				/* Add a new Node in our list (which keeps the order of our failed apps*/
				PQ->AddNode(temp2);

			}

		}





		/* Medium Vessels */



		if( strcmp( application->type , "M") == 0 ){	// vessel is Medium



			if( mem->M_available > 0 ){
			//there is an empty Medium position(and nobody moves in port)

				application->approval = true;
				mem->counter--;

				mem->number_vessels++;		// Vessels that came to the port(statistics)
				mem->number_M++;			// Medium reserved positions (for statistics)

				approved = true;		// for later...

				/* Waiting time */

				gettimeofday(&tv,NULL);
				now = tv.tv_sec;

				mem->total_waiting+= now - application->arrive_queue;		// total waiting time ( arrive_queue = when did it arrive in queue ? )
				mem->waiting_M+= now - application->arrive_queue;			// waiting time for M positions

				current = (Position *) ( ( ( (char*) mem ) + mem->offset_Medium) );


				/* Search for the available space */

				for( i = 0; i < mem->M_total; i++){


					if( current->available == true){

						mem->M_available--;		// reducing counter

						/* For Position */
						current->available = false;		// no longer available

						current->pid = application->pid;

						strcpy( current->type, application->type);

//						strcpy( current->type, mem->application.type);

							/*Arrival Time */
						gettimeofday(&tv,NULL);
						now = tv.tv_sec;
						current->arrival_time = now - mem->start;		// arrive in port

						/* For Application Form */

//						strcpy(mem->application.postype, "M");

						// calculate the offset
						application->offset = ( (char *) current) -( (char *) mem) ;

						break;	// found our available spot

					}

					current = current + 1;

				}


			}else if( (mem->M_available == 0) && ( application->upgrade == true) ){
			//Searching if we can put the vessel in another position (upgrade)

				if( mem->L_available > 0 ){
				//there is an empty Medium position(and nobody moves in port)

					application->approval = true;
					mem->counter--;

					mem->number_vessels++;		// Vessels that came to the port(statistics)
					mem->number_L++;			// Large reserved positions (for statistics)

					approved = true;		// for later...


					/* Waiting time */

					gettimeofday(&tv,NULL);
					now = tv.tv_sec;

					mem->total_waiting+= now - application->arrive_queue;		// total waiting time
					mem->waiting_L+= now - application->arrive_queue;			// waiting time for L positions


					Position* current;

					current = (Position *) ( ( ( (char*) mem ) + mem->offset_Large) );

					/* Search for the available space */

					for( i = 0; i < mem->L_total; i++){

						if( current->available == true){

							mem->L_available--;		// reducing counter

							/* For Position */
							current->available = false;		// no longer available

							current->pid = application->pid;

							strcpy( current->type, application->type);



//							strcpy( current->type, mem->application.type);

								/*Arrival Time */
							gettimeofday(&tv,NULL);
							now = tv.tv_sec;
							current->arrival_time = now - mem->start;	// arrive in port

							/* For Application Form */

//							strcpy(mem->application.postype, "L");

							// calculate the offset
							application->offset = ( (char *) current) -( (char *) mem);

							break;	// found our available spot

						}


						current = current + 1;

					}

				}else{		// no available spots, neither for its size nor upgrade

					application->approval = false;

					approved = false;		// for later...

					strcpy(temp2,"uML");

				/* Add a new Node in our list (which keeps the order of our failed apps*/
					PQ->AddNode(temp2);

				}

			}else if( (mem->M_available == 0) && ( application->upgrade == false) ){
				// no available spots for the (non-upgradeable) Medium vessel

				application->approval = false;

				approved = false;		// for later...

				strcpy(temp2,"M");

				/* Add a new Node in our list (which keeps the order of our failed apps*/
				PQ->AddNode(temp2);


			}

		}









		/* Large Vessels */

		if( strcmp( application->type , "L") == 0 ){	// vessel is 





			if( mem->L_available > 0 ){
			//there is an empty Medium position(and nobody moves in port)

				application->approval = true;
				mem->counter--;

				mem->number_vessels++;		// Vessels that came to the port(statistics)
				mem->number_L++;			// Large reserved positions (for statistics)

				approved = true;		// for later...

				/* Waiting time */

				gettimeofday(&tv,NULL);
				now = tv.tv_sec;

				mem->total_waiting+= now - application->arrive_queue;		// total waiting time
				mem->waiting_L+= now - application->arrive_queue;			// waiting time for L positions

				Position *current;


				current = (Position *) ( ( ( (char*) mem ) + mem->offset_Large) );

				/* Search for the available space */

				for( i = 0; i < mem->L_total; i++){

					if( current->available == true){

						mem->L_available--;		// reducing counter

						/* For Position */
						current->available = false;		// no longer available

						current->pid = application->pid;

						strcpy( current->type, application->type);

//						strcpy( current->type, mem->application.type);

							/*Arrival Time */
						gettimeofday(&tv,NULL);
						now = tv.tv_sec;
						current->arrival_time = now - mem->start;		// arrive in port

						/* For Application Form */

						// calculate the offset
						application->offset = ( (char *) current ) -( (char *) mem);

						break;	// found our available spot

					}

					current = current + 1;

				}


			}else{	// no available spots

				application->approval = false;

				approved = false;		// for later...

				strcpy(temp2,"L");

				/* Add a new Node in our list (which keeps the order of our failed apps )*/
				PQ->AddNode(temp2);

			}

		}


		sem_post( &(mem->rw_mutex) );		// finished writing in memory
		sem_post( &(mem->in_evaluation) );// wakeup vessel, and let it read port-master's decision

		if( approved == true){		// approved the application

			if( sem_wait( &(mem->maneuver_pm) ) == -1){	// wait for vessel to finish maneuvering


				perror(" Sem_wait error in port-master (9) ");
				exit(1);
			}

		}



	}else{		// enter = false, vessel wants to leave


	/****************************** LOG FILE *********************************/

		fp = fopen( log, "a");
		if(fp == NULL){

			perror("Error opening log file in port-master (4) ");
			exit(1);

		}

		gettimeofday(&tv,NULL);
		now = tv.tv_sec;

		fprintf(fp, " Port-master checks application form of vessel %ld at %f (EXITING) \n", (long) application->pid ,  (now - mem->start) );

		if( fclose(fp) == -1){

			perror("Error closing log file in port-master (4) ");
			exit(1);

		}

		current = (Position *) ( ( (char*) mem ) + application->offset );// which position are we talking about

		application->approval = true;
		mem->counter--;

		current->available = true;		// position now available

		if( strcmp( current->postype,"S") == 0 ){		// Small Position

			mem->S_available++;

			/* Waiting time */

			gettimeofday(&tv,NULL);
			now = tv.tv_sec;

			mem->total_waiting+= now - application->arrive_queue;		// total waiting time
			mem->waiting_S+= now - application->arrive_queue;			// waiting time for S positions



			sem_post( &(mem->rw_mutex) );		// finished writing in memory
			sem_post( &(mem->in_evaluation) );// wakeup vessel, and let it read port-master's decision

			if( sem_wait( &(mem->maneuver_pm) ) == -1){	// wait for vessel to finish maneuvering

				perror("Sem_wait error in port-master (10) ");
				exit(1);

			}

			/* After finishing maneuvering , calculate cost */

			/* Departure of vessel */
			gettimeofday(&tv,NULL);
			departure = tv.tv_sec;

			if( sem_wait( &(mem->rw_mutex) ) == -1){		// access memory again

					perror("Sem_wait error in port-master (11) ");
					exit(1);

			}

			duration = departure - current->arrival_time;		// duration of stay

			/* Calculate the Cost */

			temp_cost =  ( (duration/mem->time_period) + 1) * cost1;

			mem->total_money+= temp_cost;

			mem->total_money_S+= temp_cost;

			sem_post( &(mem->rw_mutex) );

		}else if( strcmp( current->postype,"M") == 0 ){		// Medium Position

			mem->M_available++;

			/* Waiting time */

			gettimeofday(&tv, NULL);
			now = tv.tv_sec;

			mem->total_waiting+= now - application->arrive_queue;		// total waiting time
			mem->waiting_M+= now - application->arrive_queue;			// waiting time for S positions



			sem_post( &(mem->rw_mutex) );		// finished writing in memory
			sem_post( &(mem->in_evaluation) );// wakeup vessel, and let it read port-master's decision

			if( sem_wait( &(mem->maneuver_pm) ) == -1){	// wait for vessel to finish maneuvering

				perror("Sem_wait error in port-master (12) ");
				exit(1);

			}


			/* After finishing maneuvering , calculate cost */

			/* Departure of vessel */
			gettimeofday(&tv,NULL);
			departure = tv.tv_sec;


			if( sem_wait( &(mem->rw_mutex) ) == -1){		// access memory again

				perror("Sem_wait error in port-master (13) ");
				exit(1);

			}




			duration = departure - current->arrival_time;		// duration of stay

			/* Calculate the Cost */

			temp_cost =  ( (duration/mem->time_period) + 1) * cost1;

			mem->total_money+= temp_cost;

			mem->total_money_M+= temp_cost;

			sem_post( &(mem->rw_mutex) );

		}else if( strcmp( current->postype,"L") == 0 ){		// Large Position

			mem->L_available++;

			/* Waiting time */

			gettimeofday(&tv,NULL);
			now = tv.tv_sec;

			mem->total_waiting+= now - application->arrive_queue;		// total waiting time
			mem->waiting_L+= now - application->arrive_queue;			// waiting time for S positions



			sem_post( &(mem->rw_mutex) );		// finished writing in memory
			sem_post( &(mem->in_evaluation) );// wakeup vessel, and let it read port-master's decision

			if( sem_wait( &(mem->maneuver_pm) ) == -1){	// wait for vessel to finish maneuvering

				perror("Sem_wait error in port-master (14) ");
				exit(1);

			}


			/* After finishing maneuvering , calculate cost */

			/* Departure of vessel */
			gettimeofday(&tv,NULL);
			departure = tv.tv_sec;

			/* Time */

			if( sem_wait( &(mem->rw_mutex) ) == -1){		// access memory again

				perror("Sem_wait error in port-master (15) ");
				exit(1);
			}


			duration = departure - current->arrival_time;		// duration of stay

			/* Calculate the Cost */

			temp_cost =  ( (duration/mem->time_period) + 1) * cost1;

			mem->total_money+= temp_cost;

			mem->total_money_L+= temp_cost;

			sem_post( &(mem->rw_mutex) );


		}



	/****************************** LOG FILE *********************************/

		if( sem_wait( &(mem->rw_mutex) ) == -1){

			perror(" Sem_wait erron in port-master (16) ");
			exit(1);
		}



		fp = fopen( log, "a");
		if(fp == NULL){

			perror("Error opening log file in port-master (5) ");
			exit(1);

		}

		gettimeofday(&tv,NULL);
		now = tv.tv_sec;

		fprintf(fp, " Port-master writes in PLrecords at after vessel %ld exited at %f, with total cost = %f \n", (long) application->pid, (now - mem->start), temp_cost );

		if( fclose(fp) == -1){

			perror("Error closing log file in port-master (5) ");
			exit(1);

		}


		sem_post( &(mem->rw_mutex) );



	/* Writing in PLrecords */


		if( sem_wait( &(mem->rw_mutex) ) == -1){

			perror("Sem_wait error in port-master (17) ");
			exit(1);
		}



		fp = fopen( PLrecords, "a");			// append write

		if( fp == NULL){

			perror("Error opening PL_Records (1) ");
			exit(1);

		}

		fprintf( fp, "Vessel :%ld	Position: %s	Cost: %f	Arrival:%f  	Departure:%f \n", (long) application->pid, current->type, temp_cost, current->arrival_time, departure );


		if( fclose(fp) == -1){

			perror(" Error closing PL_Records (1) ");
			exit(1);

		}

		sem_post( &(mem->rw_mutex) );

	}


	/* Want to read memory */
	if( sem_wait( &(mem->mutex) ) == -1){

		perror("Sem_wait error in port-master(18) ");
		exit(1);
	}
	mem->readers_count++;

	if( mem->readers_count == 1){
		if( sem_wait( &(mem->rw_mutex) ) == -1){

			perror("Sem_wait error in port-master(19)");
			exit(1);
		}
	}

	sem_post( &(mem->mutex) );


	if( mem->counter == 0){		// serviced all the applications => Continue?

		/* Finished reading from memory */
		if( sem_wait( &(mem->mutex) ) == -1){

			perror("Sem_wait error in port-master(20) ");
			exit(1);
		}
		mem->readers_count--;

		if( mem->readers_count == 0){
			if( sem_post( &(mem->rw_mutex) ) == -1){

				perror("Sem_wait error in port-master(21)");
				exit(1);
			}
		}
		sem_post( &(mem->mutex) );

		printf("(Port-master)Finished our jobs. Exit? (y/n) ==>( char then enter) \n");
		char c;

		scanf(" %c", &c);
		printf(" C character is %c \n", c);

		if( c == 'y'){

			break;

		}else if( c == 'n'){

			continue;
		}

	}

		/* Finished reading from memory */
		if( sem_wait( &(mem->mutex) ) == -1){

			perror("Sem_wait error in port-master(22) ");
			exit(1);
		}
		mem->readers_count--;
		if( mem->readers_count == 0){
			if( sem_post( &(mem->rw_mutex) ) == -1){

				perror("Sem_wait error in port-master(23)");
				exit(1);
			}
		}

		sem_post( &(mem->mutex) );

}		// bracket of while




/* Start exiting procedure, all the vessels in port must exit */

	/* If there are vessels in port, let them exit in orderly fashion */

	/************************* LOG FILE ****************************/

	sem_wait( &(mem->rw_mutex) );

	fp = fopen( log, "a");
	if(fp == NULL){

		perror("Error opening log file in port-master (6) ");
		exit(1);

	}

	gettimeofday(&tv,NULL);
	now = tv.tv_sec;

//		fprintf(fp, "Port-master writes in PLrecords after vessel %ld exited (EXITING) \n", current->application.pid);

	fprintf(fp, "Port-master starts EXIT PROCEDURE  \n");

	if( fclose(fp) == -1){

		perror("Error closing log file in port-master (6) ");
		exit(1);

	}


	sem_post( &(mem->rw_mutex) );

	Position* temp3;





	/* If there is any lingering vessel in here, slowly let them leave 
		in an organised manner */



while( (mem->S_total - mem->S_available != 0) || (mem->M_total - mem->M_available !=0) ||  (mem->L_total - mem->L_available != 0) ){	// all 3 must be false(no vessel in port)


	/* All the vessels OUTSIDE the port=> detach memory and exit */ 

	/* As for INSIDE => sem_wait( &(mem->queue) ) <-- generic queue, and wait for ok from port*/

	sem_post( &(mem->queue) );		// wakeup the vessel, waiting to exit the port

	sem_wait( &(mem->full) );		// wait for vessel to write the application form



	temp3 = (Position *) ( ( (char *) mem) + application->offset );	//which position

	temp3->available = true;


	if( strcmp( current->postype,"S") == 0 ){

		mem->S_available++;

		/* Waiting time */

		gettimeofday(&tv,NULL);
		now = tv.tv_sec;

		mem->total_waiting+= now - application->arrive_queue;		// total waiting time
		mem->waiting_S+= now - application->arrive_queue;			// waiting time for S positions



		sem_post( &(mem->rw_mutex) );		// finished writing in memory
		sem_post( &(mem->in_evaluation) );// wakeup vessel, and let it read port-master's decision

		if( sem_wait( &(mem->maneuver_pm) ) == -1){	// wait for vessel to finish maneuvering

			perror("Sem_wait error in port-master (10) ");
			exit(1);

		}

		/* After finishing maneuvering , calculate cost */

		/* Departure of vessel */
		gettimeofday(&tv,NULL);
		departure = tv.tv_sec;

		if( sem_wait( &(mem->rw_mutex) ) == -1){		// access memory again

			perror("Sem_wait error in port-master (11) ");
			exit(1);

		}

		duration = departure - current->arrival_time;		// duration of stay

		/* Calculate the Cost */

		temp_cost =  ( (duration/mem->time_period) + 1) * cost1;

		mem->total_money+= temp_cost;

		mem->total_money_S+= temp_cost;

		sem_post( &(mem->rw_mutex) );


	}else if( strcmp( current->postype,"M") == 0 ){

		mem->M_available++;

		/* Waiting time */

		gettimeofday(&tv,NULL);
		now = tv.tv_sec;

		mem->total_waiting+= now - application->arrive_queue;		// total waiting time
		mem->waiting_M+= now - application->arrive_queue;			// waiting time for S positions

		sem_post( &(mem->rw_mutex) );		// finished writing in memory
		sem_post( &(mem->in_evaluation) );// wakeup vessel, and let it read port-master's decision

		if( sem_wait( &(mem->maneuver_pm) ) == -1){	// wait for vessel to finish maneuvering

			perror("Sem_wait error in port-master (10) ");
			exit(1);

		}

		/* After finishing maneuvering , calculate cost */

		/* Departure of vessel */
		gettimeofday(&tv,NULL);
		departure = tv.tv_sec;

		if( sem_wait( &(mem->rw_mutex) ) == -1){		// access memory again

				perror("Sem_wait error in port-master (11) ");
				exit(1);

		}

		duration = departure - current->arrival_time;		// duration of stay

		/* Calculate the Cost */

		temp_cost =  ( (duration/mem->time_period) + 1) * cost2;

		mem->total_money+= temp_cost;

		mem->total_money_M+= temp_cost;

		sem_post( &(mem->rw_mutex) );

	}else if( strcmp( current->postype,"L") == 0 ){


		mem->L_available++;

		/* Waiting time */

		gettimeofday(&tv,NULL);
		now = tv.tv_sec;

		mem->total_waiting+= now - application->arrive_queue;		// total waiting time
		mem->waiting_L+= now - application->arrive_queue;			// waiting time for S positions



		sem_post( &(mem->rw_mutex) );		// finished writing in memory
		sem_post( &(mem->in_evaluation) );// wakeup vessel, and let it read port-master's decision

		if( sem_wait( &(mem->maneuver_pm) ) == -1){	// wait for vessel to finish maneuvering



			perror("Sem_wait error in port-master (10) ");
			exit(1);

		}

		/* After finishing maneuvering , calculate cost */

		/* Departure of vessel */
		gettimeofday(&tv,NULL);
		departure = tv.tv_sec;

		if( sem_wait( &(mem->rw_mutex) ) == -1){		// access memory again

			perror("Sem_wait error in port-master (11) ");
			exit(1);

		}

		duration = departure - current->arrival_time;		// duration of stay

		/* Calculate the Cost */

		temp_cost =  ( (duration/mem->time_period) + 1) * cost3;

		mem->total_money+= temp_cost;

		mem->total_money_L+= temp_cost;

		sem_post( &(mem->rw_mutex) );


	}

	/* Writing in PLrecords */

	if( sem_wait( &(mem->rw_mutex) ) == -1){

		perror("Sem_wait error in port-master (17) ");
		exit(1);
	}



	fp = fopen( PLrecords, "a");			// append write

	if( fp == NULL){

		perror("Error opening PL_Records (2) ");
		exit(1);

	}

	fprintf( fp, "%ld	%s	%f	%f	%f \n", (long) application->pid, current->postype, temp_cost, current->arrival_time, departure );



	if( fclose(fp) == -1){

		perror(" Error closing PL_Records (2) ");
		exit(1);

	}

	sem_post( &(mem->rw_mutex) );

	/* Continue to the next one */

}






/* No more vessels in port */


	/* Writing in log that we are leaving */

	sem_wait( &(mem->rw_mutex));

	fp = fopen( log, "a");
	if( fp == NULL){

		perror("Error opening log file in port-master (6)");

		exit(1);
	}

	fprintf(fp, "\n\n Port-master exiting .... \n\n");

	if(fclose(fp) == -1){

		perror("Error closing log file in port-master (6)");
		exit(1);

	}

	sem_post( &(mem->rw_mutex) );

	/* Detaching memory and exitng */

	if( shmdt( (void *) mem )== -1){

		perror(" Error detaching memory in port-master ");
		exit(1);

	}

	delete PQ;		// delete PQ list



	exit(0);

}		//bracket of main







