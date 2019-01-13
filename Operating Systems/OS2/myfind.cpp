#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <stdio.h>
#include <cstdlib>
#include <wait.h>
#include <math.h>

#include <time.h>
#include <sys/time.h>


#include "MyStruct.hpp"

#define READ 0
#define WRITE 1






int signal_count = 0;


void handler(int sig){

	signal( SIGUSR2, handler);

//	if(sig == SIGUSR2)
	 signal_count++;		//increment number of SIGUSR2 signals

}



int main(int argc, char* argv[] ){

/* Starting timer */

	struct timeval begin;

	if( gettimeofday(&begin, NULL ) == -1){

		perror("Time Error in leaf (1)");
		exit(1);

	}

	time_t start = begin.tv_sec;






	signal( SIGUSR2, handler);

	bool sflag = false;
	int height = 0;
	char *dataName = NULL;
	char *pattern = NULL;


	char* height_s= NULL;

	if( argc < 7) {

		printf("Error, not enough arguments\n");
		exit(1);
	}


	int i = 0;


	for( i = 0; i < argc; i++){

		if( strcmp(argv[i] , "-h") == 0 ){

			height = atoi(argv[i+1]);

			if(height < 1){

				printf("Error with height"); 
				return -1;

			}

			height_s = argv[i+1];


		}else if(strcmp(argv[i] , "-d") == 0){

			dataName = argv[i+1];

			if ( dataName == NULL){

				printf("Error with dataName");
				 return -1;
			}

		}else if( strcmp(argv[i] , "-p") == 0){

			pattern = argv[i+1];

			if ( pattern == NULL){

				printf("Error with pattern");
				 return -1;
			}

		}else if( strcmp(argv[i] , "-s") == 0){

			sflag = true;
		}

	}




/* The following code is from Verify.c
	It reads the binary file 			*/

	FILE *fpb;
	BRec rec;
	long lSize;
	int numOfrecords;

	fpb = fopen (dataName,"rb");

	if (fpb==NULL) {
		printf("Cannot open binary file\n");
		return 1;
	}

	// check number of records

	fseek (fpb , 0 , SEEK_END);
	lSize = ftell (fpb);
	rewind (fpb);

	numOfrecords = (int) lSize/sizeof(rec);

	fclose(fpb);

	/* End of Read */


/* Creating the arguments of exec*() */

	/* Snprintf() (1) */

	int len = snprintf(NULL,0, "%d", numOfrecords) + 1 ;			// +1 NULL byte
	if( len < 0){

		perror("Snprintf Error (1)");
		return -1;
	}

	char* total_records = new char[ len ];
//	char total_records[ len ];

	snprintf( total_records, len , "%d", numOfrecords);


	char* flag = new char[6];			// for -s flag

	if ( sflag == false) strcpy( flag, "false");
	else strcpy(flag, "true");


	/* Snprintf() (2) */

	pid_t ppid = getpid();
	len = snprintf(NULL, 0, "%d", ppid) + 1 ;			// +1 NULL byte
	if( len < 0){

		perror("Snprintf Error (2)");
		return -1;

	}
	char* root_pid = new char[ len ];
//	char root_pid[ len ];

	snprintf( root_pid, len , "%d", ppid );


/* Creating pipe */

	int p[2];					//pipe between root and sm0

	if( pipe(p) == -1){

		perror("pipe call (1) in root"); 
		exit(1); 

	}

	pid_t pid;

	switch( pid = fork() ){

		case -1: 
		{
			perror("fork failure (1) in root");
			exit(1);
		}
		case 0: 				// child proccess
		{


		/* 	dataName = name of binary
			start/end = range of search
			pattern = what we are searching (  (sub)string )
			height_s = height in string
			write_pipe = fd of WRITE_END of pipe
			flag = -s flag (true or false)

		*/

			/* here we use p to write (from sm0 to root ) */

			if( close(p[READ]) == -1 ){			// so we have to close the read end of p

				perror("Close call (1) in root");
				exit(1);
			}

			/*When you call close, you always close the file handle. When a file handle is closed, the reference count on its open file description is decremented. If it goes to zero, the open file description is also released and the reference count on the file itself is decremented. Only if that goes to zero is the kernel's file structure freed.

There is no chance for one process to release a resource another process is using because shared resources are reference counted.

			*/

			/* Snprintf() (3) */

			int len1 = snprintf(NULL,0, "%d", p[WRITE]) + 1 ;			// +1 NULL byte
			if( len1 < 0){

				perror("Snprintf Error (3)");
				return -1;
			}
			char* write_pipe = new char[ len1 ];	// transfer write_end pipe to SM0
			snprintf( write_pipe, len1 , "%d", p[WRITE] );


			if( strcmp( flag , "false") == 0 ){

				char* start_rec = new char[2];
				strcpy( start_rec, "1");

				char* end_rec = total_records;
				printf("Total records is : %s \n", total_records);




				execlp("./splitter","splitter", dataName, pattern, flag, root_pid, write_pipe, height_s, start_rec, end_rec, total_records, (char *) NULL);

			}else{	// flag == true

				char* start_child = new char[2];
				strcpy( start_child, "1");


				/* Snprintf() (4) */
				int total_c = pow(2,height);


				printf("height is %d\n", height);
				printf("total_c is %d\n", total_c);

				len1 = snprintf(NULL, 0, "%d", total_c ) + 1 ;			// +1 NULL byte
				if( len1 < 0){

					perror("Snprintf Error (4)");
					return -1;
				}
				char *total_child = new char[ len1 ];
				snprintf( total_child, len1 , "%d", total_c );

				char *end_child = total_child;

				execlp("./splitter", "splitter", dataName, pattern, flag, root_pid, write_pipe, height_s, total_records, start_child, end_child, total_child, (char *)NULL);



			perror ("exec failure (1) in root"); 
			exit(1);
			}


		}


		default: break;
	}

	// Parent (Root)

	if( close(p[WRITE]) == -1 ){			// we close write end ( root will only read)

		perror("Close call (1) in root");
		exit(1);
	}

	int status = 0;


	int nbytes = 0 ;
	int records_found = 0 ;

	/* Read number of records found (read an integer) */

	nbytes = read_all( p[READ], &records_found, sizeof(int) );		//blocked

	if ( nbytes < sizeof(int) ) {

		perror("Read Error in root (1)"); 
		exit(1);
	}

	if( records_found == 0 ){

		printf("No records found ==> exiting \n");
		exit(0);
	}


	nbytes = 0;
	i = 0;

	BRec* matrix  = new BRec[ records_found ];

	/* Read the records and store them in a matrix? (maybe different struct)  */

	for( i = 0; i < records_found; i++){

		nbytes = read_all( p[READ], &matrix[i], sizeof(BRec) );

		if ( nbytes < sizeof(BRec) ) {

			perror("Read Error in root (2)"); 
			exit(1);
		}
	}

	waitpid( pid, &status, 0);	//reap sm0 

	printf("Gathered the records\n");



	/* Matrix has all the records */

	/* Pass the matrix into SortNode */

	pid_t pid1;



	printf("Sorting\n");


	pid1 = fork();		// for sort Node (sorting pipe and printing to stdout)

	/* Write contents of pipe into a file , and sort file */



	if( pid1 == -1 ){

			perror("fork failure (2) in root");
			exit(1);

	}else if( pid1 ==  0){ 				// child proccess == sortNode

		FILE *fp1 ;

		fp1 = fopen("output.txt", "w+" );

		if( fp1 == NULL){

			perror("Error opening output.txt in root");
			exit(1);
		}

		int length1 = 0;
		printf("records_found is %d\n\n", records_found);

		for( i = 0; i < records_found; i++){


			/* Turn Brec into a string */

			length1 = snprintf( NULL, 0, " %ld %s %s  %s %d %s %s %-9.2f\n", \
							matrix[i].custid, matrix[i].LastName, matrix[i].FirstName, \
							matrix[i].Street, matrix[i].HouseID, matrix[i].City, \
							 matrix[i].postcode, matrix[i].amount ) + 1; // for the NULL byte

//			printf( " %ld %s %s  %s %d %s %s %-9.2f\n", \
					matrix[i].custid, matrix[i].LastName, matrix[i].FirstName, \
					matrix[i].Street, matrix[i].HouseID, matrix[i].City, \
					 matrix[i].postcode, matrix[i].amount );

			char buffer[length1];
			snprintf( buffer, length1, " %ld %s %s  %s %d %s %s %-9.2f\n", \
					matrix[i].custid, matrix[i].LastName, matrix[i].FirstName, \
					matrix[i].Street, matrix[i].HouseID, matrix[i].City, \
					 matrix[i].postcode, matrix[i].amount );




			fwrite( buffer, 1, strlen(buffer), fp1);


		}


		execlp( "sort", "sort", "output.txt", (char*) NULL);

		perror ("exec failure (2) in root"); 
		exit(1);

	}

	/* Parent Process */

//	printf("\n");

	int status1 = 0;

	waitpid( pid1, &status1, 0);		// wait for sort-child to terminate 




	/* Read the following statistical struct */

	nbytes = 0;
	SRec strec;


	nbytes = read_all( p[READ], &strec, sizeof(SRec) );

	if ( nbytes < sizeof(SRec) ) {

		perror("Read Error in root (3)"); 
		exit(1);
	}



	if( close(p[READ]) == -1 ){				// closing read end of p =====>  closed p

		perror("Close call (5) in root");
		exit(1);
	}


		/* Stopping timer */

	struct timeval finale;

	if( gettimeofday(&finale, NULL ) == -1){

		perror("Time Error in leaf (2)");
		exit(1);

	}

	time_t end = finale.tv_sec;


	double turn_around_time = (double) (end-start);

	/*Print the statistics */ 	// %ld for long

	printf("\nMin time of searchers is %f seconds \n" , strec.search_min );
	printf("Max time of searchers is %f seconds \n" , strec.search_max );
	printf("Average time of searchers is %f seconds \n\n" , strec.search_average );

	printf("Min time of mergers/splitters is %f seconds \n" , strec.split_min );
	printf("Max time of mergers/splitters is %f seconds \n" , strec.split_max );
	printf("Average time of mergers/splitters is %f seconds \n\n" , strec.split_average );

	printf(" Turnaround Time is %f seconds \n", turn_around_time );

	printf(" Signal count is %d\n", signal_count);

	/* Freeing up memory */

	delete[] matrix;
	delete[] flag;
	delete[] total_records;
	delete[] root_pid;

	exit(0);
}



