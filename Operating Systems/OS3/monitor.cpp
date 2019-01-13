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
#include <signal.h>

#include <errno.h>


#include "MyStructs.hpp"



int stop = 1;			// stop == 0 => don't pass through while loop (or break)


int main(int argc, char* argv[]){



	if( argc != 7){

		printf("Please enter the correct arguments \n");
		return -1;
	}

	int i, shmid;

	int time, stattimes;

	for( i = 0; i < argc; i++){

		if( strcmp(argv[i] , "-d") == 0 ){

			time = atoi( argv[i+1] );

		}else if( strcmp( argv[i] , "-t") == 0){

			stattimes = atoi( argv[i+1] );

		 }else if(strcmp(argv[i] , "-s") == 0){

			shmid = atoi( argv[i+1] );

		}

	}

	/* Attaching memory */

	SHM *mem;

	mem = (SHM*) shmat(shmid, NULL, 0);

	if( mem == (void *) -1 ){
 
		perror(" Error attaching memory (in vessel) "); 
		exit(1);
	}

	char *log = ( (char*) mem ) + mem->offset_logname;
	printf("(Monitor) log_name is : %s \n", log);


	/* Time */

	struct timeval tv;
	time_t now ;

	FILE *fp;

	double temp = 0;


	Position* current = NULL;


	while( stop != 0 ){			// stop == 0 , when we SIGINT

		if( time <= stattimes ){

			if( sleep( time ) > 0){		//gets interrupted by signal

			 	if( stop == 0) break;
			}

		printf("**************(MONITOR)Woke up from slumber (1) \n");


			signal(SIGINT,SIG_IGN);

			if( sem_wait( &(mem->rw_mutex) ) == -1 ){

				perror("Sem_wait in monitor (1) ");
				exit(1);
			}

			/**** PRINTING CURRENT STATE IN LOG ******/

			fp = fopen(log, "a");
			if(fp == NULL){

				perror("Error opening log file in monitor (1) ");
				exit(1);

			}

			fprintf(fp, "\n\n ***** PRINTING CURRENT STATE OF PORT ***** \n");

			fprintf(fp, " Vessels in port : %d \n", (mem->S_total + mem->M_total + mem->L_total) - (mem->S_available + mem->M_available +mem->L_available) );

			/* Printing the ships in port */

				/* Starting with Small */

			fprintf( fp, "Vessels in Small Positions : %d \n", mem->S_total - mem->S_available);


			current = (Position *) ( ( (char*) mem) + mem->offset_Small);

			for( i = 0; i < mem->S_total; i++){

				if( current->available == false){

					fprintf(fp, "Position: S%d		Vessel: %ld 	type: %s	arrival_time: %f\n", current->counter, (long) current->pid, current->type, current->arrival_time);

				}

				current = current +1;

			}


				/*  Medium */

			fprintf( fp,"\n Vessels in Medium Positions : %d \n",mem->M_total - mem->M_available);

			current = (Position *) ( ( (char*) mem) + mem->offset_Medium);

			for( i = 0; i < mem->M_total; i++){

				if( current->available == false){

					fprintf(fp, "Position: M%d		Vessel: %ld 	type: %s	arrival_time: %f \n", current->counter, (long) current->pid, current->type, current->arrival_time);

				}

				current = current + 1;

			}

				/* Large */

			fprintf( fp,"\n Vessels in Large Positions : %d \n",mem->L_total - mem->L_available);

			current = (Position *) ( ( (char*) mem) + mem->offset_Large);

			for( i = 0; i < mem->L_total; i++){

				if( current->available == false){

					fprintf(fp, "Position: L%d		Vessel: %ld 	type: %s	arrival_time: %f \n", current->counter, (long) current->pid, current->type, current->arrival_time);


				}

				current = current + 1;


			}

			current = NULL;

			if( fclose(fp) == -1){

				perror("Error closing log file in monitor (1) ");
				exit(1);

			}


			sem_post( &(mem->rw_mutex) );



			 sleep( stattimes-time);



// example: times = 10, stattimes = 20=> every 10 seconds=>current state and every 20 secs=>stats
// after 10 seconds pass => print current state, then wait another 10 seconds => print stats

			if( sem_wait( &(mem->rw_mutex) ) == -1){

				perror(" Sem_wait in monitor (2)");
			}

			fp = fopen(log, "a");
			if(fp == NULL){

				perror("Error opening log file in monitor (2) ");
				exit(1);

			}

			fprintf( fp, "\n ******** PORT STATISTICS ****** \n");

			/* Average waiting time */

				/* For every position */
			temp = mem->total_waiting/mem->number_vessels;

			fprintf( fp,"\n Average waiting time: %f \n", temp);

				/* For Small positions */
			temp = mem->waiting_S/mem->number_S;

			fprintf( fp," Average waiting time for S positions: %f \n", temp);

				/* For Medium Positions */
			temp = mem->waiting_M/mem->number_M;

			fprintf( fp,"Average waiting time for M positions: %f \n", temp);

				/* For Large Positions */
			temp = mem->waiting_L/mem->number_L;

			fprintf( fp," Average waiting time for L positions: %f \n\n", temp);

			/* Revenue (Income) */

			fprintf( fp, " Total Revenue of port: %f \n", mem->total_money);

			fprintf( fp, " Total Revenue gathered from S : %f \n", mem->total_money_S);

			fprintf( fp, " Total Revenue gathered from M : %f \n", mem->total_money_M);

			fprintf( fp, " Total Revenue gathered from L : %f \n", mem->total_money_S);

				/* Average Revenue */

					/* Total */

			temp = mem->total_money/mem->number_vessels;

			fprintf( fp, "\n Average Revenue gathered : %f \n", temp );

					/* Small */

			temp = mem->total_money_S/mem->number_S;

			fprintf( fp, " Average Revenue gathered from S : %f \n", temp);

					/* Medium */

			temp = mem->total_money_M/mem->number_M;

			fprintf( fp, " Average Revenue gathered from M : %f \n", temp);

					/* Large */

			temp = mem->total_money_L/mem->number_L;

			fprintf( fp, " Average Revenue gathered from L : %f \n\n", temp);

			if( fclose(fp) == -1){

				perror("Error closing log file in monitor (2) ");
				exit(1);

			}


			sem_post( &(mem->rw_mutex) );




		}else{		// stattimes < time

			if( sleep( stattimes) > 0){

				if( stop == 0) break;

			} 

			if( sem_wait( &(mem->rw_mutex) ) == -1){

				perror("Error on sem_wait in monitor (3) ");
				exit(1);

			}

			fp = fopen(log, "a");
			if(fp == NULL){

				perror("Error opening log file in monitor (3) ");
				exit(1);

			}

			fprintf( fp, "\n\n ******** PORT STATISTICS ****** \n");

			/* Average waiting time */

				/* For every position */
			temp = mem->total_waiting/mem->number_vessels;

			fprintf( fp,"\n Average waiting time: %f \n", temp);

				/* For Small positions */
			temp = mem->waiting_S/mem->number_S;

			fprintf( fp," Average waiting time for S positions: %f \n", temp);

				/* For Medium Positions */
			temp = mem->waiting_M/mem->number_M;

			fprintf( fp,"Average waiting time for M positions: %f \n", temp);

				/* For Large Positions */
			temp = mem->waiting_L/mem->number_L;

			fprintf( fp," Average waiting time for L positions: %f \n\n", temp);

			/* Revenue (Income) */

			fprintf( fp, " Total Revenue of port: %f \n", mem->total_money);

			fprintf( fp, " Total Revenue gathered from S : %f \n", mem->total_money_S);

			fprintf( fp, " Total Revenue gathered from M : %f \n", mem->total_money_M);

			fprintf( fp, " Total Revenue gathered from L : %f \n", mem->total_money_S);

				/* Average Revenue */

					/* Total */

			temp = mem->total_money/mem->number_vessels;

			fprintf( fp, "\n Average Revenue gathered : %f \n", temp );

					/* Small */

			temp = mem->total_money_S/mem->number_S;

			fprintf( fp, " Average Revenue gathered from S : %f \n", temp);

					/* Medium */

			temp = mem->total_money_M/mem->number_M;

			fprintf( fp, " Average Revenue gathered from M : %f \n", temp);

					/* Large */

			temp = mem->total_money_L/mem->number_L;

			fprintf( fp, " Average Revenue gathered from L : %f \n\n", temp);

			if( fclose(fp) == -1){

				perror("Error closing log file in monitor (3) ");
				exit(1);

			}


			sem_post( &(mem->rw_mutex) );

			if(stop == 0) break;


			if( sleep( time-stattimes ) > 0){	// interrupted by signal

				if( stop == 0) break;

			}


			if( sem_wait( &(mem->rw_mutex) ) == -1){

				if( errno == EINTR){
					sem_post( &(mem->rw_mutex) );
					continue;
				}

				perror("Sem_wait error in monitor (4) ");

			}


			/**** PRINTING CURRENT STATE IN LOG ******/

//			fp = fopen( mem->log_name, "a");

			fp = fopen(log, "a");
			if(fp == NULL){

				perror("Error opening log file in monitor (4) ");
				exit(1);

			}

			fprintf(fp, "\n\n ***** PRINTING CURRENT STATE OF PORT ***** \n");

			fprintf(fp, " Vessels in port : %d \n", (mem->S_total + mem->M_total + mem->L_total) - (mem->S_available + mem->M_available +mem->L_available) );

			/* Printing the ships in port */

				/* Starting with Small */

			fprintf( fp, "Vessels in Small Positions : %d \n", mem->S_total - mem->S_available);

			current = (Position *) ( ( (char*) mem) + mem->offset_Small);

			for( i = 0; i < mem->S_total; i++){

				if( current->available == false){

					fprintf(fp, "Position: S%d		Vessel: %ld 	type: %s	arrival_time: %f\n", current->counter, (long) current->pid, current->type, current->arrival_time);

				}

				current = current + 1;

			}


				/*  Medium */

			fprintf( fp,"\n Vessels in Medium Positions : %d \n",mem->M_total - mem->M_available);

			current = (Position *) ( ( (char*) mem) + mem->offset_Medium);

			for( i = 0; i < mem->M_total; i++){

				if( current->available == false){

					fprintf(fp, "Position: M%d		Vessel: %ld 	type: %s	arrival_time: %f \n", current->counter, (long) current->pid, current->type, current->arrival_time);

				}

				current = current + 1;

			}

				/* Large */

			fprintf( fp,"\n Vessels in Large Positions : %d \n",mem->L_total - mem->L_available);

			current = (Position *) ( ( (char*) mem) + mem->offset_Large);

			for( i = 0; i < mem->L_total; i++){

				if( current->available == false){

					fprintf(fp, "Position: L%d		Vessel: %ld 	type: %s	arrival_time: %f \n", current->counter, (long) current->pid, current->type, current->arrival_time);

				}

				current = current + 1;

			}

			current = NULL;

			if( fclose(fp) == -1){

				perror("Error closing log file in monitor (4) ");
				exit(1);

			}



			sem_post( &(mem->rw_mutex) );

		}



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





		printf("(Monitor)Finished our jobs. Exit? (y/n) ==>( char then enter) \n");
		char c;


		scanf(" %c", &c);
		printf(" C character is %c \n", c);

		if( c == 'y'){

			break;

		}else if( c == 'n'){

			continue;
		}



	}



	}

	/* Writing in log that we are leaving */

	sem_wait( &(mem->rw_mutex));


	fp = fopen(log, "a");
	if( fp == NULL){

		perror("Error opening log file in monior (5)");

		exit(1);
	}

	fprintf(fp, "\n\n Monitor exiting .... \n\n");

	if(fclose(fp) == -1){

		perror("Error closing log file in monitor (6)");
		exit(1);

	}

	sem_post( &(mem->rw_mutex) );


	/* Detaching memory and exitng */

	if( shmdt( (void *) mem )== -1){

		perror(" Error detaching memory in monitor ");
		exit(1);

	}

	exit(0);

}






