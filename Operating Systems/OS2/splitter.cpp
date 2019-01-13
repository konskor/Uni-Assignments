#include <sys/types.h>
#include <string.h>
#include <unistd.h>

#include <wait.h>
#include <signal.h>

#include <stdio.h>
#include <cstdlib>

#include <time.h>
#include <sys/time.h>

#include <poll.h>

#include "MyStruct.hpp"

#define READ 0
#define WRITE 1






int main(int argc, char* argv[] ){

/*	-s flag == TRUE

	argv[1] = dataName (of binary)
	argv[2] = pattern (substring)
	argv[3] = -s flag (true or false)
	argv[4] = root process id
	argv[5] = fd of write_end of pipe ( write to grandparent)
	argv[6] = height

	argv[7] = total number of Records
	argv[8] = start_child ( range of leaf children)
	argv[9] = end_child 
	argv[10] = total number of leaf children

*/

/*	-s flag == FALSE

	argv[1] = dataName (of binary)
	argv[2] = pattern (substring)
	argv[3] = -s flag (true or false)
	argv[4] = root process id
	argv[5] = fd of write_end of pipe ( write to grandparent)
	argv[6] = height

	argv[7] = start_record
	argv[8] = end_record ( range of records)
	argv[9] = total number of records

*/



	struct timeval begin;

	if( gettimeofday(&begin, NULL ) == -1){

		perror("Time Error in leaf (1)");
		exit(1);

	}

	time_t start = begin.tv_sec;


	int height = atoi(argv[6]);


/* If h = 0, then we execute leaf node */



	if( height == 0){				//leaf node



		if( strcmp( argv[3] , "true" ) == 0 ){			// -s flag == true



			char *dataName = argv[1];
			char *pattern = argv[2];
			char *flag = argv[3];
			char *root_pid = argv[4];
			char *write_pipe = argv[5];				// write end of pipe to "parent"

			char *total_records = argv[7];
			char *searcher_no = argv[8];		// searcher_number = child_start 
			char *total_searchers = argv[10];

			printf("searcher number = %s", searcher_no);



			execlp("./leaf","leaf", dataName, pattern, flag, root_pid, write_pipe, total_records, searcher_no, total_searchers, (char *) NULL );

		perror ("exec failure in splitter (1)"); 
		exit(1);



		}else{		// -s flag == false



			char *dataName = argv[1];
			char *pattern = argv[2];
			char *flag = argv[3];
			char *root_pid = argv[4];
			char *write_pipe = argv[5];				// write end of pipe to "parent"

			char *total_records = argv[9];
			char *start_record = argv[7];
			char *end_record = argv[8];

			printf(" start_record= %s, end_record = %s \n", start_record, end_record);

			execlp("./leaf","leaf", dataName, pattern, flag, root_pid, write_pipe, total_records, start_record, end_record, (char *) NULL );


		perror ("exec failure in splitter (2)"); 
		exit(1);
		}

	}

	int p1[2];

	int p2[2];
	if( pipe(p1) == -1) { 

		perror("pipe call (1) in splitter"); 
		exit(1); 

	}

	if( pipe(p2) == -1) { 

		perror("pipe call (2) in splitter"); 
		exit(1); 

	}


	pid_t ppid = getpid();		//Parent

	pid_t acpid;				// Child A
	pid_t bcpid;				// Child B





/* Adjust range */

	int child_end1 = 0;			// used if -s flag == true (else we ignore)
	int child_start2 = 0;


	int record_end1 = 0;		// used if -s flag == false (else we ignore)
	int record_start2 = 0;


	if( strcmp( argv[3] , "true" ) == 0 ){			// -s flag == true

// leaf children==> from [start_child, end_child1] to [ start_child, end_child1] and [ childstart_2, end_child ] 


		/* start_child + (end_child - start_child)/2 */

		child_end1 = atoi(argv[8]) + ( atoi(argv[9]) - atoi(argv[8]) ) /2;

		child_start2 = child_end1 + 1;

	}else{			// -s flag == false

//	printf("Entered range if statement\n");

// number of records ==> from [ start_record, end_record] to [ start_record, end_record1 ] and [ start_record2, end_record ]


		/* (end_record - start_record )/2 */



		record_end1 = atoi(argv[7]) + ( atoi(argv[8]) - atoi(argv[7]) )/2;
 
		record_start2 = record_end1 + 1;



	}


/* Create the two children   */

/* First child */

	acpid = fork();

	if( acpid < 0  ){		//fork failed

		perror("fork failure (1) in splitter");
		exit(1);					//exit parent process

	}else if( acpid == 0){		// child A



//		printf("\n\n Child A \n\n");

		int WtoGrandParent = atoi( argv[5] );		//Write end to grand_parent




		if( close(WtoGrandParent) == -1 ){		//we have to close this, otherwise chaos ensues

			perror("Close call (1) in splitter");
			exit(1);
		}



		if( close(p2[READ]) == -1 ){		// p2 is not used here

			perror("Close call (2) in splitter");
			exit(1);

		}



		if( close(p2[WRITE]) == -1 ){		// p2 is not used here

			perror("Close call (3) in splitter");
			exit(1);
		}



		if( close(p1[READ]) == -1 ){		// p1 is used (HERE) to write to the parent (in the future)

			perror("Close call (4) in splitter");
			exit(1);
		}




		/* Snprintf() (1) */


		height = height -1;



		int len = snprintf(NULL,0, "%d", height) + 1 ;			// +1 NULL byte
		if( len < 0){

			perror("Snprintf Error (1)");
			exit(1);
		}
		char *h = new char[ len ];				// h = (new) height as string
		snprintf( h, len, "%d", height );


		/* Snprintf() (2) */

		len = snprintf(NULL,0, "%d", p1[WRITE]) + 1 ;			// +1 NULL byte
		if( len < 0){

			perror("Snprintf Error (2)");
			exit(1);
		}
		char *write_pipe = new char[ len  ];				// transfer write_end pipe to SM
		snprintf( write_pipe, len, "%d", p1[WRITE] );


		if( strcmp( argv[3] , "true" ) == 0 ){			// -s flag == true

			char *dataName = argv[1];
			char *pattern = argv[2];
			char *flag = argv[3];
			char *root_pid = argv[4];

			char *total_records = argv[7];

			char *start_child = argv[8];

			/* Snprintf() (3) */

			len = snprintf(NULL,0, "%d", child_end1) + 1 ;			// +1 NULL byte
			if( len < 0){

				perror("Snprintf Error (3)");
				exit(1);
			}

			char *end_child = new char[ len  ];
			snprintf( end_child, len , "%d", child_end1 );


			char *total_child = argv[10];

			printf(" start_child= %s, end_child= %s \n", start_child, end_child);


			execlp("./splitter","splitter", dataName, pattern, flag, root_pid, write_pipe, h, total_records, start_child, end_child , total_child, (char *) NULL);

			perror ("exec failure  in splitter (3)"); 
			exit(1);



		}else{		// -s flag == false

			char *dataName = argv[1];
			char *pattern = argv[2];
			char *flag = argv[3];
			char *root_pid = argv[4];


			char *start_record = argv[7];

			/* Snprintf() (4) */

			len = snprintf(NULL,0, "%d", record_end1) + 1 ;			// +1 NULL byte
			if( len < 0){

				perror("Snprintf Error (4)");
				exit(1);
			}
			char *end_record = new char[ len  ];
			snprintf( end_record, len , "%d", record_end1 );


			char *total_records = argv[9];

			printf(" start_record= %s, end_record = %s \n", start_record, end_record);

			execlp("./splitter","splitter", dataName, pattern, flag, root_pid, write_pipe, h, start_record, end_record, total_records, (char *) NULL);


			perror ("exec failure  in splitter (4)"); 
			exit(1);
		}

	}






	/* Second child */

	bcpid = fork();

	if( bcpid < 0){		// fork failed


		perror("fork failure (2) in splitter");
		exit(1);

	}else if( bcpid == 0){		// child B



//		printf("\n\n Child B \n\n");


		int WtoGrandParent = atoi( argv[5] ); //Write end to grand_parent

		if( close(WtoGrandParent) == -1 ){		//we have to close this, otherwise chaos ensues

			perror("Close call (5) in splitter");
			exit(1);
		}



		if( close(p1[READ]) == -1 ){		// p1 is not used here

			perror("Close call (6) in splitter");
			exit(1);
		}



		if( close( p1[WRITE] ) == -1 ){		// p1 is not used here

			perror("Close call (7) in splitter");
			exit(1);
		}




		if( close(p2[READ]) == -1 ){// p2 is used (HERE) to write to the parent (in the future)

			perror("Close call (8) in splitter");
			exit(1);

		}


		/* Snprintf() (5) */

		height = height - 1;



		int len = snprintf(NULL,0, "%d", height) + 1 ;			// +1 NULL byte
		if( len < 0){

			perror("Snprintf Error (5)");
			exit(1);

		}

		char *h = new char[ len ];				// h = (new) height as string
		snprintf( h, len , "%d", height);

		/* Snprintf() (6) */

		len = snprintf(NULL,0, "%d", p2[WRITE]) + 1 ;			// +1 NULL byte
		if( len < 0){

			perror("Snprintf Error (6)");
			return -1;
		}
		char *write_pipe = new char[ len ];		// transfer write_end pipe to SM
		snprintf( write_pipe, len , "%d", p2[WRITE]);


		if( strcmp( argv[3] , "true" ) == 0 ){			// -s flag == true

			char *dataName = argv[1];
			char *pattern = argv[2];
			char *flag = argv[3];
			char *root_pid = argv[4];

			char *total_records = argv[7];

			/* Snprintf() (7) */

			len = snprintf(NULL,0, "%d", child_start2) + 1 ;			// +1 NULL byte
			if( len < 0){

				perror("Snprintf Error (7)");
				return -1;
			}

			char *start_child= new char[ len ];

			snprintf( start_child, len , "%d", child_start2);



			char *end_child = argv[9];



			char *total_child = argv[10];

			execlp("./splitter","splitter", dataName, pattern, flag, root_pid, write_pipe, h, total_records, start_child, end_child , total_child, (char *) NULL);

			perror ("exec failure (5) in splitter"); 
			exit(1);

		}else{		// -s flag == false

			char *dataName = argv[1];
			char *pattern = argv[2];
			char *flag = argv[3];
			char *root_pid = argv[4];

			/* Snprintf() (8) */

			len = snprintf(NULL,0, "%d", record_start2) + 1 ;			// +1 NULL byte
			if( len < 0){

				perror("Snprintf Error (8)");
				return -1;
				}
			char *start_record= new char[ len  ];

			snprintf( start_record, len , "%d", record_start2);

			char *end_record = argv[8];

			char *total_records = argv[9];

			execlp("./splitter","splitter", dataName, pattern, flag, root_pid, write_pipe, h, start_record, end_record, total_records, (char *) NULL);

			perror ("exec failure (6) in splitter"); 
			exit(1);

		}

	}





	/* Parent process */


	if( close(p2[WRITE]) == -1 ){		// p2 is used (HERE) to read from bcpid 

		perror("Close call (9) in splitter");
		exit(1);

	}

	if( close(p1[WRITE]) == -1 ){		// p1 is used (HERE) to read from acpid 

		perror("Close call (10) in splitter");
		exit(1);

	}

//	waitpid( bcpid, NULL, 0);


//	printf("Parent Process (1)\n");

	int status = 0;
	pid_t temp;

	int records_found1 = 0;
	int records_found2 = 0;

	int nbytes = 0;

	BRec* matrix1 = NULL;
	BRec* matrix2 = NULL;

	SRec stat1;
	SRec stat2;

	int i = 0;
	int j = 0;







/* (1) */

/* Read from pipe 1 */

	/* Read how many Records found */

	nbytes = read_all( p1[READ], &records_found1, sizeof(int) );	// blocked; wait for something to read

		if ( nbytes < sizeof(int) ) {

			perror("Read Error in splitter (1)"); 
			exit(1);
			}

			if ( records_found1 != 0 ){

			/* Create a matrix of Bin records and read the records currently in the pipe */

				matrix1 = new BRec[records_found1];

			}

		printf("Records_found1 splitter %d\n", records_found1);

//		printf("Passed first block\n");

/* (2) */

/* Read from pipe 2 */

	/* Read how many Records found */

	nbytes = read_all( p2[READ], &records_found2, sizeof(int) );

		if ( nbytes < sizeof(int) ) {

			perror("Read Error in splitter (2)"); 
			exit(1);
		}

		if( records_found2 != 0 ){

			/* Create a matrix of Bin records and read the records currently in the pipe */

			matrix2 = new BRec[records_found2];

		}

		printf("Records_found2 splitter %d\n", records_found2);

//		printf("Passed second block\n");



	printf("Finished waiting in splitter\n");

/* Start reading the records */

	if( ( matrix1 != NULL) || (matrix2 != NULL) ){		//at least one splitter has records

		if( (matrix1 != NULL) && (matrix2 != NULL) ){	// read from both p1 and p2

			/* Read 1 record from each pipe , EACH TIME */
			/* This will prevent any potential overflow of a pipe ===> Write BLOCK */

			/* For additional safety, we also use poll() */


			struct pollfd fdarray1[1];			// fdarray1 = { p1[READ] }
			struct pollfd fdarray2[1];			// fdarray2 = { p2[READ] }

			fdarray1[0].fd = p1[READ];
			fdarray1[0].events = POLLIN;

			fdarray2[0].fd = p2[READ];
			fdarray2[0].events = POLLIN;

			int rdy1 = 0;
			int rdy2 = 0;



			while( (i < records_found1) || (j < records_found2) ){	// read one record from each pipe and continue doing so , until you have read records_found1+records_found2


				rdy1 = poll( fdarray1, 1, 0);		//return immediately
				rdy2 = poll( fdarray2, 1, 0);

				if( (i < records_found1) && (rdy1 == 1) ){	// p1 is ready to be read ( and there are records to be read )

					nbytes = read_all( p1[READ], &matrix1[i], sizeof(BRec) );

//					      	printf("%ld %s %s  %s %d %s %s %-9.2f\n", \
							matrix1[i].custid, matrix1[i].LastName, matrix1[i].FirstName, \
							matrix1[i].Street, matrix1[i].HouseID, matrix1[i].City, \
							 matrix1[i].postcode, matrix1[i].amount);



						if ( nbytes < sizeof(BRec) ) {

							perror("Read Error in splitter (3)"); 
							exit(1);
						}

					i++;

				}

				if( (j < records_found2) && (rdy2 == 1) ){		// p2 is ready to be read

					nbytes = read_all( p2[READ], &matrix2[j], sizeof(BRec) );

//					      	printf("%ld %s %s  %s %d %s %s %-9.2f\n", \
							matrix2[j].custid, matrix2[j].LastName, matrix2[j].FirstName, \
							matrix2[j].Street, matrix2[j].HouseID, matrix2[j].City, \
							 matrix2[j].postcode, matrix2[j].amount);

						if ( nbytes < sizeof(BRec) ) {

							perror("Read Error in splitter (4)"); 
							exit(1);
						}

					j++;

				}

			}


		}else if( matrix1 == NULL){		//only read from p2


			for( j = 0; j < records_found2; j++){

				nbytes = read_all( p2[READ], &matrix2[j], sizeof(BRec) );

				if ( nbytes < sizeof(BRec) ) {

					perror("Read Error in splitter (5)"); 
					exit(1);
				}
			}
		}else if( matrix2 == NULL){			// only read from p1

			for( i = 0; i< records_found1; i++){

				nbytes = read_all( p1[READ], &matrix1[i], sizeof(BRec) );

				if ( nbytes < sizeof(BRec) ) {

					perror("Read Error in splitter (6)"); 
					exit(1);
				}
			}
		}
	}

		waitpid( acpid, &status,0);		// reap child A
		waitpid( bcpid, &status, 0);	// reap child B


	/* Read all the BRecords from the children (later the statistics) */
	/* Time to send the amount_of_records_found  BRecords to "grandparent" */


	int WtoGrandParent = atoi( argv[5] );			// file descriptor

	int count = records_found1 + records_found2;

	printf("total records found %d", count);

	nbytes = write_all( WtoGrandParent, &count , sizeof(int) );

	if ( nbytes < sizeof(int) ) {

		perror("Write Error in splitter (1)"); 
		exit(1);
	}

	if( count != 0){			// if count == 0 , we only send the statistics

		if( matrix1 != NULL){

			for ( i = 0 ; i < records_found1; i++ ){

				nbytes = write_all( WtoGrandParent, &matrix1[i] , sizeof(BRec) );

				if ( nbytes < sizeof(BRec) ) {

					perror("Write Error in splitter (2)"); 
					exit(1);
				}

			}
		}

		if( matrix2 != NULL){

			for ( i = 0 ; i < records_found2; i++ ){

				nbytes = write_all( WtoGrandParent, &matrix2[i] , sizeof(BRec) );

				if ( nbytes < sizeof(BRec) ) {

					perror("Write Error in splitter (3)"); 
					exit(1);
				}

			}
		}

	}


	printf("\nfinished splitter\n");

	/* Time to read the statistics from the children */


	if( height == 1 ){			// It has children ==> "Leaf Nodes"

		/* Will only get run time of the child processes */


		double time1,time2;

		nbytes = read_all( p1[READ] , &time1 , sizeof(double) );

		if ( nbytes < sizeof(double) ) {

			perror("Read Error in splitter (7)"); 
			exit(1);
		}

		if( close(p1[READ]) == -1 ){		// Nothing else to read

			perror("Close call (11) in splitter ");
			exit(1);

		}


		nbytes = read_all( p2[READ] , &time2 , sizeof(double) );

		if ( nbytes < sizeof(double) ) {

			perror("Read Error in splitter (8)"); 
			exit(1);
		}


		if( close(p2[READ]) == -1 ){		// Nothing else to read

			perror("Close call (12) in splitter ");
			exit(1);

		}


		/* Stopping timer */

		struct timeval finale;

		if( gettimeofday(&finale, NULL ) == -1){

			perror("Time Error in leaf (2)");
			exit(1);

			}

		time_t end = finale.tv_sec;

		double execution = end - start;

		SRec stat3;

		stat3.search_min = time1 < time2 ? time1  : time2;
		stat3.search_max = time1 > time2 ? time1  : time2;
		stat3.search_average = (time1 + time2) / 2;

		stat3.split_min = execution + time1 + time2 ;
		stat3.split_max = execution + time1 + time2 ;
		stat3.split_average =  execution + time1 + time2 ;

		nbytes = write_all( WtoGrandParent , &stat3, sizeof( SRec ) );

		if ( nbytes < sizeof(SRec) ) {

			perror("Write Error in splitter (4)"); 
			exit(1);
		}

		if( close(WtoGrandParent) == -1 ){		// Nothing else to write 

			perror("Close call (12) in splitter ");
			exit(1);

		}

		exit(0);

	}else{		// height > 1


		nbytes = read_all( p1[READ] , &stat1 , sizeof(SRec) );

		if ( nbytes < sizeof( SRec ) ) {

			perror("Read Error in splitter (9)"); 
			exit(1);
		}

		if( close(p1[READ]) == -1 ){		// Nothing else to read

			perror("Close call (13) in splitter ");
			exit(1);

		}


		nbytes = read_all( p2[READ] , &stat2 , sizeof(SRec) );

		if ( nbytes < sizeof( SRec) ) {

			perror("Read Error in splitter (10)"); 
			exit(1);
		}


		if( close(p2[READ]) == -1 ){		// Nothing else to read

			perror("Close call (14) in splitter ");
			exit(1);

		}

		/* Stopping timer */

		struct timeval finale;

		if( gettimeofday(&finale, NULL ) == -1){

			perror("Time Error in leaf (2)");
			exit(1);

			}

		time_t end = finale.tv_sec;

		double execution = (double) (end-start);

		SRec stat3;

		stat3.search_min = stat1.search_min < stat2.search_min ? stat1.search_min : stat2.search_min;
		stat3.search_max = stat1.search_max > stat2.search_max ? stat1.search_max : stat2.search_max;
		stat3.search_average = (stat1.search_average + stat2.search_average) / 2;



	/* Calculating new minimum */


		if( stat1.split_min < stat2.split_min ){

			// stat1_min < stat2_min

			if( stat1.split_min < execution ){

				stat3.split_min = stat1.split_min; 

			}else{ //execution < stat1.split_min

				stat3.split_min = execution;

			}

		}else{	//stat2.split_min < stat1.split_min

			if( stat2.split_min < execution ){

				stat3.split_min = stat2.split_min; 

			}else{	// execution < stat2.split_min

				stat3.split_min = execution;

				}
		}

	/* Calculating the new maximum */


		if( stat1.split_max > stat2.split_max ){

			// stat1_max > stat2_max

			if( stat1.split_max > execution ){

				stat3.split_max = stat1.split_max; 

			}else{ //execution > stat1.split_max

				stat3.split_max = execution;

			}

		}else{	//stat2.split_max > stat1.split_max

			if( stat2.split_max > execution ){

				stat3.split_max = stat2.split_max; 

			}else{	// execution > stat2.split_min

				stat3.split_max = execution;

				}
		}


	/* Calculating new average */

		stat3.split_average = (stat1.split_average + stat2.split_average + execution) / 3;

		nbytes = write_all( WtoGrandParent , &stat3, sizeof( SRec ) );

		if ( nbytes < sizeof(SRec) ) {

			perror("Write Error in splitter (5)"); 
			exit(1);
		}

		if( close(WtoGrandParent) == -1 ){		// Nothing else to write 

			perror("Close call (15) in splitter ");
			exit(1);

		}

		exit(0);

	}

}



















