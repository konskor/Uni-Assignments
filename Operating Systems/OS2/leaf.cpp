#include <sys/types.h>
#include <string.h>
#include <unistd.h>

#include <signal.h>

#include <stdio.h>
#include <cstdlib>


#include <time.h>
#include <sys/time.h>

#include "MyStruct.hpp"

#define READ 0
#define WRITE 1



int main( int argc, char* argv[]){



/*	-s flag == TRUE

	argv[1] = dataName (of binary)
	argv[2] = pattern (substring)
	argv[3] = -s flag (true or false)
	argv[4] = root process id
	argv[5] = fd of write_end of pipe ( write to parent)

	argv[6] = total number of Records
	argv[7] = searcher_number ( range of leaf children)
	argv[8] = total number of leaf children

*/

/*	-s flag == FALSE

	argv[1] = dataName (of binary)
	argv[2] = pattern (substring)
	argv[3] = -s flag (true or false)
	argv[4] = root process id
	argv[5] = fd of write_end of pipe ( write to parent)
	argv[6] = total number of records

	argv[7] = start_record
	argv[8] = end_record ( range of records)

*/

/* Starting timer*/


	struct timeval begin;

	if( gettimeofday(&begin, NULL ) == -1){

		perror("Time Error in leaf (1)");
		exit(1);

	}

	time_t start_time = begin.tv_sec;


	/* Adjusting the ranges, based on @87 in piazza */

	int total_records = atoi(argv[6]);

	int start = 0;
	int end = 0;

	if( strcmp(argv[3], "true") == 0 ){			//-s flag == true

		int searcher_no = atoi(argv[7]);
		int total_searchers = atoi(argv[8]);

		if( searcher_no == 1){		//first searcher

			start = 1;

			// total_searchers = 2^h
			// k = total_records

			end = (start-1) + 2 * (total_records * searcher_no) / ( total_searchers * (total_searchers+1));

		}else if( searcher_no == total_searchers){		//last searcher

			start = ( (total_records * (searcher_no - 1) * searcher_no) / ( total_searchers * (total_searchers+1) ) ) + 1;

			end = total_records;

		}else if ( (searcher_no > 1) && (searcher_no < total_searchers) ) {		// 1 < searcher_no < last

			start = ( (total_records * (searcher_no - 1) * searcher_no) / ( total_searchers * (total_searchers+1) ) ) + 1;


			end = (start-1) + 2 * (total_records * searcher_no) / ( total_searchers * (total_searchers+1));


		}

	}else if(strcmp( argv[3], "false") == 0){			//-s flag == false



		start =  atoi(argv[7]);			// start = start_record
		end = atoi(argv[8]);			// end = end_record



	}

	/* In case start < 1 , then return count = 0 */

	if( (start < 1) || (start > total_records ) || ( end < 1) || ( end > total_records) ){

		printf("Finished search\n");


		int WtoParent = atoi( argv[5]);

		/* Write how many records found */

		printf("Begining write\n");

		int counter = 0;
		int nbytes = 0;


		nbytes = write_all( WtoParent, &counter, sizeof(int) );

		if( nbytes < 0){

			perror("Write Error in leaf (1)"); 
			exit(1);
		}


		/* Statistics */

		struct timeval finale;

		if( gettimeofday(&finale, NULL ) == -1){

			perror("Time Error in leaf (2)");
			exit(1);

		}

		time_t end_time = finale.tv_sec;

		double execution = end_time - start_time;

		/* Write execution time */

		nbytes = write_all( WtoParent, &execution, sizeof(double) );

			if( nbytes < 0){

				perror("Write Error in leaf (2)"); 
				exit(1);
			}

		printf("Finished writing\n");

		if( close(WtoParent) == -1){

			perror("Close call (1) in leaf");
			exit(1);
		}

		int root_pid = atoi(argv[4]);
		kill(root_pid, SIGUSR2);

		exit(0);
	}




	/* Now we search the binary file */

	FILE *fpb;
	BRec rec;
	int i = 0;



	fpb = fopen (argv[1],"rb");
	if (fpb==NULL) {
		printf("Cannot open binary file\n");
		return 1;
	}

	int recordsize = sizeof(BRec);

	fseek(fpb,recordsize*(start-1),SEEK_SET);

	int len = 0;
	int counter = 0;

	char *check;

	List *list = new List();

	// argv[2] = pattern


	printf("Starting Search \n");




	for( i = 0; i < (end - start +1) ; i++){

		fread(&rec, sizeof(BRec), 1, fpb);

		/* Customer Id (long int) */

		len = snprintf( NULL, 0, "%ld", rec.custid) + 1;		//+1 for /0

		char *cust_id = new char[len];
		snprintf( cust_id, len, "%ld", rec.custid);

		check = strstr( cust_id, argv[2]); 

		delete[] cust_id;			//free memory
		cust_id = NULL;

		if( check != NULL){		// found it

			counter++;
			list->addRec(rec);

			continue;
		}



		/* First Name */

		check = strstr( rec.FirstName, argv[2] );

		if( check != NULL){		// found it

			counter++;
			list->addRec(rec);

			continue;
		}

		/* Last Name */

		check = strstr( rec.LastName, argv[2] );

		if( check != NULL){		// found it

			counter++;
			list->addRec(rec);

			continue;
		}

		/* Street Name */

		check = strstr( rec.Street, argv[2] );

		if( check != NULL){		// found it

			counter++;
			list->addRec(rec);



			continue;
		}

		/* House ID (int) */

		len = snprintf( NULL, 0, "%d", rec.HouseID ) + 1;		//+1 for /0

		char *House_ID = new char[len];
		snprintf( House_ID, len, "%d", rec.HouseID);

		check = strstr( House_ID, argv[2]); 

		delete[] House_ID;			//free memory
		House_ID = NULL;

		if( check != NULL){		// found it

			counter++;
			list->addRec(rec);

			continue;
		}

		/* City */

		check = strstr( rec.City, argv[2] );

		if( check != NULL){		// found it

			counter++;
			list->addRec(rec);

			continue;
		}

		/* PostCode */

		check = strstr( rec.postcode, argv[2] );

		if( check != NULL){		// found it

			counter++;
			list->addRec(rec);

			continue;
		}

		/* Amount (float) */

		len = snprintf( NULL, 0,"%-9.2f", rec.amount ) + 1;		//+1 for /0

		char *Amount = new char[len];
		snprintf( Amount, len, "%-9.2f", rec.amount);

		check = strstr( Amount, argv[2]); 

		delete[] Amount;			//free memory
		Amount = NULL;

		if( check != NULL){		// found it

			counter++;
			list->addRec(rec);

			continue;
		}




	}


	fclose(fpb);

	printf("Finished search\n");
	printf("Found %d records\n", counter);

	int WtoParent = atoi( argv[5]);

	/* Write how many records found */

	printf("Begining write\n");

	int nbytes = 0;

	nbytes = write_all( WtoParent, &counter, sizeof(int) );

	if( nbytes < 0){

		perror("Write Error in leaf (1)"); 
		exit(1);
	}

	/* Write the Records */

	Node* temp = list->getHead();

	BRec rec1;

	while(temp != NULL){

		rec1 = temp->getRecord();


//      	printf("%ld %s %s  %s %d %s %s %-9.2f\n", \
		rec1.custid, rec1.LastName, rec1.FirstName, \
		rec1.Street, rec1.HouseID, rec1.City, rec1.postcode, \
		rec1.amount);




		nbytes = write_all( WtoParent, &rec1, sizeof(BRec) );

		if( nbytes < 0){

			perror("Write Error in leaf (2)"); 
			exit(1);
		}

		temp = temp->getNext();

	}

	delete list;


	/* Stopping timer */

	struct timeval finale;

	if( gettimeofday(&finale, NULL ) == -1){

		perror("Time Error in leaf (2)");
		exit(1);

	}

	time_t end_time = finale.tv_sec;

	double execution = end_time - start_time;

	/* Write execution time */

	nbytes = write_all( WtoParent, &execution, sizeof(double) );

		if( nbytes < 0){

			perror("Write Error in leaf (2)"); 
			exit(1);
		}

	printf("Finished writing\n");

	if( close(WtoParent) == -1){

		perror("Close call (1) in leaf");
		exit(1);
	}

	int root_pid = atoi(argv[4]);
	kill(root_pid, SIGUSR2);

	exit(0);

}







































