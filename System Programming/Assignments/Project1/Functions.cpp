#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <ctype.h>


#include "header.hpp"


bool valid_Date_Time(char* string){

	int day,month,year;
	int hour,minutes;

	sscanf( string, "%d-%d-%d %d:%d", &day,&month,&year, &hour,&minutes);

	// check year
	if( (year >= 1900) && (year <= 9999) ){
	
		//check month
		if( (month >=1) && (month<=12) ){
		
			//check days
			if( ( (day >=1) && (day<=31) ) && ( (month == 1) || (month == 3) || (month == 5) || (month == 7) || (month == 8) || (month == 10) || (month == 13) ) ){
			
				return true;
				
			}else if( ( (day >=1) && (day<=30) ) && ( (month == 4) || (month == 6) || (month == 9) || (month == 1) ) ){
		
				return true;
		
		
			}else if( ( (day >=1) && (day<=28) ) && ( month == 2 ) ){
			
				return true;
			
			}else{
				printf(" In %s, day is invalid \n", string);
				return false;			
			}
		}else{
		
			printf("In %s, Month is invalid.\n",string);
			return false;	
		}
	}else{
	
		printf("In %s, year is invalid. \n",string);
	
		return false;	
	}
}


bool isNumber(char* string){

	int i;
	
	
	for( i = 0; i < strlen(string); i++){
	
		if( isdigit( string[i] ) == false ) return false;	
	}
	
	return true;
}




/*
bool valid_Transaction(char* transaction, HashTable1* TransactionHT, List<Wallet>* UserList){

	char transactionId[50];

	char user1[50];
	char user2[50];		

	int amount;
	
	char date[50];
	char time[50];
	
	int assignments = 0;



	printf(" Transaction is %s \n", transaction);

	assignments = sscanf(line,"%s %s %s %s %s ", transactionId, user1, user2, &amount, date, time);
	
	/* Check if we have enough or more than enough arguments *
	
	if( assignments < 4 ){	// 4 <= items to be filled <= 6

		printf("Error in TransactionFile: transactionId User1 User2 value date time \n");
			
		return false;		
	} 
		
		
		
		
		
	/* Check if transactionId already exists (hashTable) *
	
	char* result;
		
	TransactionHT->SearchRecord1(transactionId, &result );
		
	if( result != NULL){	// transactionID exists
	
		printf("Error in TransactionFile: transactionId already exists !!!! \n\n\n");
		return false;
	}
		
	// transactionID doesn't exist (add to the TransactionHT ) 
		
	printf("\nTransactionID %s doesn't exist ===> OK \n\n", transactionId); 		
	
	if( isNumber(transactionId) == true){	// transactionId is a number
	
		if( max < atoi(transactionId) ) max = atoi(transactionId);
	}
	
	TransactionHT->AddRecord1(transactionId); // add transactionId to HT
	
	
	
	/* Check if user1,user2 exists  *
		
		
	/* Check User1 *
	
	if( UserExists(UserList,user1) == false){	// user doesn't exist => exit program
		
		printf("User %s  doesn't exist ==> continue to next transaction \n", user1);
		continue;
	}
		
		
		/* Find Node of user1 *
	Node<Wallet>* node = UserList->head;
		
	while( node != NULL){
		
		if( strcmp( node->data->WalletID, user1) == 0 ){
			
			printf(" Found our Node , breaking out of loop (1) \n");
				
			break;			
		}
			
		node = node->next;		
	}
		
	if( node == NULL){	// if  our node doesn't exist ===> ERROR
		
		printf(" ERROR , SOMETHING IS WRONG HERE !!!! (1)");
				
		return false;	
		
	}
		
		
		
		
		
		
		
	
		/* Check if user1 = user2 *
		
	if( strcmp(user1,user2) == 0){	//check if same user
		
		printf(" Same User %s in this transaction => continue to next transaction \n", user1);
		return false;
	}
		
		
		
		
		
		/* Check user2 *
	
	if( UserExists(UserList,user2) == false){	// user doesn't exist => exit program
		
		printf("User %s  doesn't exist ==> continue to next transaction \n", user2);
		continue;
	}
	
		/* Find Node of user2 *
	Node<Wallet>* node1 = UserList->head;
		
	while( node1 != NULL){
		
		if( strcmp( node1->data->WalletID, user2) == 0 ){
			
			printf(" Found our Node , breaking out of loop (2) \n");
				
			break;			
		}
			
		node1 = node1->next;		
	}
		
	if( node1 == NULL){	// if  our node doesn't exist ===> ERROR
		
		printf(" ERROR, SOMETHING IS WRONG HERE !!!! (2)");
				
		return false;	
	}
		

	/* Check if user1 has enough money, check the balance *
	
	
	if( amount <= 0){	// check if the transaction amount is invalid
		
		printf("The transaction amount is invalid ( <= 0) ===> continue to next transaction \n");
		return false;		
	}
	
	
	if( node->data->amount < amount){	// user1 can't pay
		
		printf("User1 can't complete this transaction , not enough $$$ \n");
		return false;		
	}
	
	// user1 has enough $$$ ====> OK

	if( extra == true){  // additional check
	
		int length = snprintf(NULL,0,"%s %s",date,time) + 1;
		
		char array[length];
		
		snprintf(array,length,
	
	
	}






	return true;



}

*/



/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////





/* Universal Hashing */
int HashFunction(char *string, int M){		// Hash function for char* 

// string = input
// M is size of hash table (buckets)



	int a;		// random numbers
	int p;		// BIG prime number

	int h = 0 ;		// result of hashing
	int i = 0;


	a = 0;			// instead of 1

	p = 1009;

	for( i = 0; i < strlen(string); i++){

		h = ( h * a + string[i]) % p;

	}

	int result = abs(h) % M;

	return result;			
}



/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////








/* Bucket Functions */

/************** IMPORTANT ==> result must be allocated **/

void SearchBucket(unsigned char** start, int numOfRecords, int bucket_size, char* walletId, struct Records** result  ){		// numOfRecords = max number of records in bucket

/* This function  searches for a record ( which has record.Wallet->walletID = WalletId ) 

		 in a bucket pointed by **start. 	*/
	
	int available;
	unsigned char* current = *start;
	
	int current_records = 0;
	
	memcpy( &available, current, sizeof(int) );
	
	current_records = numOfRecords - available;
	
	
	if( current_records == 0 ){	// bucket is empty
	
		printf(" Couldn't find record, bucket is empty(deleting result => returning .. \n");
		
		current = NULL;
		
		delete *result;
		*result = NULL;
		
		return;	
	}

	//else bucket isn't empty, search the records

	int i;
	struct Records record;
	
	current = current + sizeof(int);	// pass the integer

	for( i = 0; i < current_records; i++ ){
	
		memcpy( &record, current, sizeof( struct Records) );
		
		if( strcmp( (record.wallet)->WalletID, walletId ) == 0 ){	// found the record we were looking for
			
			(*result)->wallet = record.wallet;
			
			(*result)->list = record.list;
			
			printf("Found our record, returning!! \n");
			
			current = NULL;			
			
			return;		
		}
		
		current = current + sizeof(struct Records);	// move to the next record
	}
	
	// didn't find record in this bucket => check *next pointer
	
	unsigned char* next = NULL;
	
	current = *start + ( bucket_size - 1);	// reached the end of the bucket
	current = current - ( sizeof(unsigned char*) - 1);	// go back a few bytes => reposition
	
	memcpy(&next, current, sizeof(unsigned char*) );
	
	
	if( next == NULL){
	
		printf(" Record doesn't exist => returning .. \n");
		
		current = NULL;
		next = NULL;
		
		delete *result;
		*result = NULL;
		
		return;
	}
	
	// next != NULL
	
	SearchBucket(&next, numOfRecords, bucket_size, walletId, result);	// recursive call for next bucket
	
	current = NULL;
	next = NULL;
	
	//delete *result;	//already deleted ( will cause problems if deleted again) 
	*result = NULL;
		
	return;
}









//Recursive Function (INCOMPLETE=> maybe search if it already exists?)=>NOPE we have SearchBucket
void Add2Bucket(unsigned char** start, int numOfRecords,int bucketSize, struct Records* record ){		// numOfRecords = max number of records in bucket

	
	/* This function  searches for empty space in a bucket pointed by **start. 
		If buckets is full, check for *next bucket.
			If next = NULL, allocate memory for next bucket and insert record
			Else, recursive call for next bucket
	*/


	int available;
	int k;
	
	unsigned char* current = *start;
	
	memcpy(&available, current, sizeof(int) );		// put sizeof(int) bytes, shown by *current, into available
	
	if( available != 0){	//empty space in bucket
	
		k = numOfRecords - available;
		
		current = current + sizeof(int);	// pass the integer value
		
		current = current + k * sizeof(struct Records);	// reach the empty spot
		
		memcpy(current,record,sizeof(struct  Records) );	// write the record in the empty spot
		
		/* updating available counter */
		
		available--;
		
		memcpy(*start, &available, sizeof(int) );		
		
		current = NULL;
		
		return;	
	} 

	//else, bucket is full => check *next pointer

	unsigned char* next = NULL;

	current = *start + ( bucketSize - 1);	// reached the end of the bucket
	current = current - ( sizeof(unsigned char*) - 1);	// go back a few bytes => reposition
			
	memcpy(&next,current, sizeof(unsigned char*) );		// next has the address(value) stored in *current
	
	if( next == NULL ){	//no next bucket
	
		next = new unsigned char[bucketSize];	//allocating new bucket
		
		memcpy(current,&next, sizeof(unsigned char* ) );	// write the address of next bucket (in current bucket)

		memcpy(next, &numOfRecords, sizeof(int) );	//initialise available counter
		
		/* Repositioning and inserting */
		
		current = next + sizeof(int);
		
		memcpy( current, record, sizeof( struct Records ) );
		
		/* Updating available of next bucket */
		
		memcpy(&available, next, sizeof(int) );
		
		available--;
		
		memcpy(next , &available, sizeof(int) );
		
		
		
		/* Recursive call */
		//Add2Bucket(&next, numOfRecords, bucketSize, record );
		
		//after recursive call , found the spot => returning	
		
		next = NULL;
		current = NULL;
		
		return;	
	}

	//else, next != NULL => recursive call for next bucket

	
	/* Recursive call */
	Add2Bucket(&next, numOfRecords, bucketSize, record );
	
	next = NULL;
	current = NULL;
		
	return;
}








// Recursive Function 
void ClearBucket(unsigned char** start, int numOfRecords, int bucket_size ){// numOfRecords = max number of records in bucket

	/* This function clears the bucket given by "start", and deletes said bucket in 3 scenarios:
	
		1) Bucket is empty => just delete it
		2) Bucket isn't full => no overflow buckets 
		
		3) Bucket is full, but *next = NULL => no overflow bucket
		
	*/

	int j;

	unsigned char* current = NULL;
	unsigned char* next = NULL;
	
	
	int available;
	int current_records;
	
	struct Records record;
	
	
	current = *start;
	
	memcpy(&available,current,sizeof(int) );
		
	current_records = numOfRecords - available;
		
	if( current_records == 0){	// no records found in this bucket ( so we just delete it and return )
		
		printf(" Bucket has no records, deleting bucket \n");
		
		delete[] current;
		current = NULL;
		
		*start = NULL;
			
		return;
		
	}// else there are records in this bucket
		
	/* Basically, we need to delete the list of transactions of users (and NULL the pointers to their wallets) */
		
	current = current + sizeof(int);	// move the pointer past the integer
		
	for( j = 0; j < current_records; j++){		// go through all the records currently in this bucket
		
		//record = (struct Records*) current;		// type cast current address -> struct Record
			
		
		/* Read record from bucket and store it in our local struct record */
		memcpy(&record,current,sizeof(struct Records) );
			
		/* "Delete" the Record */
		record.wallet = NULL;	// we don't want to delete the wallets (yet)

		delete record.list;		//delete this list
		record.list = NULL;
			
		/* No need to write back, will be deleted anyways */
			
		/* Moving to he next record */
		
		current = current + sizeof(struct Records);		
		
	} // deleted all the transaction lists of users in this bucket
	
	
	
	
	if( current_records != numOfRecords ){	// bucket wasn't full 
	
		printf(" Bucket wasn't full, so it didn't have overflow buckets ==> deleting bucket \n");
		
		current = *start;
		
		delete[] current;
		
		current = NULL;
		*start = NULL;
		next = NULL;
		
		
		return;	

	} //else, bucket was full => check for overflow bucket
	
	//current = start + (bucket_size-1) - ( sizeof(unsigned char*) - 1 );	// the last 8 bytes are a char* pointer => next bucket;
	
	current = *start + ( bucket_size - 1);	// reached the end of the bucket
	current = current - ( sizeof(unsigned char*) - 1);	// go back a few bytes => reposition
			
	memcpy(&next,current, sizeof(unsigned char*) );		// next has the address(value) stored in *current
		
	if( next == NULL){	// no next buckets
			
		//delete bucket[i];
		//bucket[i] = NULL;
		
		
		
		delete[] *start;
		*start = NULL;
			
		next = NULL;
		current = NULL;
		
		//printf("Deleted all the records in this bucket, returning from ClearBucket() \n");	
				
		return;		// return to the previous bucket (or just return from the function )	
			
	}else{	// we have overflow buckets to explore
	
		ClearBucket( &next, numOfRecords, bucket_size );		// recursive call , for the next bucket
		
		// after this , all subsequent buckets have been deleted, and *next = NULL
		
		delete[] *start;	//deleting this bucket (byte array)
		
		next = NULL;
		*start = NULL;
		current = NULL;
		
		//printf("Deleted all the records in this bucket, returning from ClearBucket() \n");	
		
		return;
	}
	
	
	
	printf("Deleted all the records in this bucket, returning from ClearBucket() \n");	
}





// might not be needed after all

/*
void UpdateBucket(unsigned char** start, int numOfRecords, struct Records* record  ){		// numOfRecords = max number of records in bucket

/* This function  searches for a record ( which has record.Wallet->walletID  ) and updates their transaction list 

		 in a bucket pointed by **start. 	*
	
	int available;
	unsigned char* current = *start;
	
	memcpy( &available, current, sizeof(int) );
	
	current_records = numOfRecords - available;
	
	
	if( current_records == 0 ){	// bucket is empty
	
		printf(" Couldn't find record, bucket is empty (  SOMETHING IS WRONG (1) !!!!! )  => returning .. \n\n");
		
		current = NULL;
		
		//delete *result;
		//*result = NULL;
		
		return;	
	}

	//else bucket isn't empty, search the records

	int i;
	struct Records record1;
	
	current = current + sizeof(int);	// pass the integer

	for( i = 0; i < current_records; i++ ){
	
		memcpy( &record1, current, sizeof( struct Records) );
		
		if( strcmp( (record1.wallet)->WalletID, record->wallet->WalletID ) == 0 ){	// found the record we were looking for
			
			//(*result)->wallet = record.wallet;
			
			//(*result)->list = record.list;
			
			memcpy(current,record,sizeof( struct Records ) );	// overwriting this record
			
			
			printf("Found our record, returning!! \n");
			
			current = NULL;			
			
			return;		
		}
		
		current = current + sizeof(struct Records);	// move to the next record
	}
	
	// didn't find record in this bucket => check *next pointer
	
	unsigned char* next;
	
	current = *start + ( bucket_size - 1);	// reached the end of the bucket
	current = current - ( sizeof(unsigned char*) - 1);	// go back a few bytes => reposition
	
	memcpy(&next, current, sizeof(unsigned char*) );
	
	
	if( next == NULL){
	
		printf(" Record doesn't exist ( SOMETHING IS WRONG (2) !!!!!! ) => returning .. \n");
		
		current = NULL;
		next = NULL;
		
		//delete *result;
		//*result = NULL;
		
		return;
	}
	
	// next != NULL
	
	SearchBucket(&next, numOfRecords, record);	// recursive call for next bucket
	
	current = NULL;
	next = NULL;
	
	//delete *result;	//already deleted ( will cause problems if deleted again) 
	//*result = NULL;
		
	return;
}


*/









//debugging purpose
void PrintBucket(unsigned char** start, int numOfRecords, int bucket_size){


/* This function  prints all the contents (WalletID of the bucket)	*/
	
	int available = 0;
	unsigned char* current = *start;
	
	int current_records = 0;
	
	memcpy( &available, current, sizeof(int) );
	
	current_records = numOfRecords - available;
	
	
	if( current_records == 0 ){	// bucket is empty
	
		printf("Bucket is empty(nothing to print) => returning .. \n");
		
		current = NULL;
				
		return;	
	}

	//else bucket isn't empty, search the records

	int i;
	struct Records record;
	
	current = current + sizeof(int);	// pass the integer

	for( i = 0; i < current_records; i++ ){
	
		memcpy( &record, current, sizeof( struct Records) );
		
		printf("%s \n", (record.wallet)->WalletID );
		
		current = current + sizeof(struct Records);	// move to the next record
	}
	
	if( current_records < numOfRecords){
	
		printf("\n Bucket isn't full, no need to check *next for Print() \n\n");
	
		return;
	}
	
	
	
	//  check *next pointer (finished with this bucket)
	
	unsigned char* next = NULL;
	
	current = *start + ( bucket_size - 1);	// reached the end of the bucket
	current = current - ( sizeof(unsigned char*) - 1);	// go back a few bytes => reposition
	
	memcpy(&next, current, sizeof(unsigned char*) );
	
	
	if( next == NULL){
	
		printf(" Record doesn't exist => returning .. \n");
		
		current = NULL;
		next = NULL;
				
		return;
	}
	
	// next != NULL
	
	PrintBucket(&next, numOfRecords, bucket_size);	// recursive call for next bucket
	
	current = NULL;
	next = NULL;
	
	//delete *result;	//already deleted ( will cause problems if deleted again) 
	//*result = NULL;
		
	return;





}

























/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
































// CHECK FUNCTIONS AGAIN




/* Bucket Functions ( for HashTable1) */

/************** IMPORTANT ==> result must be allocated( not necessary here) ******************/

void SearchBucket1(unsigned char** start, int numOfRecords, int bucket_size, char* Id, char** result  ){	// numOfRecords = max number of records in bucket

/* This function  searches for a record(char*) ( where strcmp( record, Id ) == 0 ) 

		 in a bucket pointed by **start. 	
*/



// The function is almost the same as the previous one
	
	int available;
	unsigned char* current = *start;
	
	memcpy( &available, current, sizeof(int) );
	
	int current_records = numOfRecords - available;
	
	
	if( current_records == 0 ){	// bucket is empty
	
		printf(" Couldn't find record, bucket is empty => returning .. \n");
		
		current = NULL;
		
	//	delete *result;
		*result = NULL;
		
		return;	
	}

	//else bucket isn't empty, search the records

	int i;
	//struct Records record;
	
	char* record = NULL;		// temp
	
	current = current + sizeof(int);	// pass the integer

	for( i = 0; i < current_records; i++ ){
	
		memcpy( &record, current, sizeof( char* ) );
		
		if( strcmp( record, Id ) == 0 ){	// found the record we were looking for
			
			*result = record;
			
			printf("Found our record, returning!! \n");
			
			current = NULL;	
			record = NULL;		
			
			return;		
		}
		
		current = current + sizeof( char* );	// move to the next record
	}
	
	// didn't find record in this bucket => check *next pointer
	
	unsigned char* next = NULL;
	
	current = *start + ( bucket_size - 1);	// reached the end of the bucket
	current = current - ( sizeof(unsigned char*) - 1);	// go back a few bytes => reposition
	
	memcpy(&next, current, sizeof(unsigned char*) );
	
	
	if( next == NULL){
	
		printf(" Record doesn't exist => returning .. \n");
		
		current = NULL;
		next = NULL;
		
		//delete *result;
		*result = NULL;
		
		record = NULL;
		
		return;
	}
	
	// next != NULL
	
	SearchBucket1(&next, numOfRecords, bucket_size, Id, result);	// recursive call for next bucket
	
	current = NULL;
	next = NULL;
	
	//delete *result;	//already deleted ( will cause problems if deleted again) 
	*result = NULL;
	
	record = NULL;
		
	return;
}





//Recursive Function (INCOMPLETE=> maybe search if it already exists?)=>NOPE we have SearchBucket
void Add2Bucket1(unsigned char** start, int numOfRecords,int bucketSize, char* record ){		// numOfRecords = max number of records in bucket

	
	/* This function  searches for empty space in a bucket pointed by **start. 
		If buckets is full, check for *next bucket.
			If next = NULL, allocate memory for next bucket and insert record
			Else, recursive call for next bucket
	*/


	int available;
	int k;
	
	unsigned char* current = *start;
	
	char* record1 = new char[ strlen(record) + 1];
	
	strcpy(record1,record);
	
	
	memcpy(&available, current, sizeof(int) );		// put sizeof(int) bytes, shown by *current, into available
	
	if( available != 0){	//empty space in bucket
	
		k = numOfRecords - available;
		
		current = current + sizeof(int);	// pass the integer value
		
		current = current + k * sizeof(char*);	// reach the empty spot
		
		memcpy(current, &record1, sizeof(char*) );	// write the record(address inside of records variable)  in the empty spot
		
		/* updating available counter */
		
		available--;
		
		memcpy(*start, &available, sizeof(int) );		
		
		current = NULL;
		record = NULL;
		
		record1 = NULL;
		
		return;	
	} 

	//else, bucket is full => check *next pointer

	unsigned char* next = NULL;

	current = *start + ( bucketSize - 1);	// reached the end of the bucket
	current = current - ( sizeof(unsigned char*) - 1);	// go back a few bytes => reposition
			
	memcpy(&next,current, sizeof(unsigned char*) );		// next has the address(value) stored in *current
	
	if( next == NULL ){	//no next bucket
	
		next = new unsigned char[bucketSize];	//allocating new bucket
		
		memcpy(current,&next, sizeof(unsigned char*) );	// write the address of next bucket (in current bucket)

		memcpy(next, &numOfRecords, sizeof(int) );	//initialise available counter
		
		/* Repositioning and inserting */
		
		current = next + sizeof(int);
		
		memcpy( current, &record1, sizeof( char* ) );
		
		/* Updating available of next bucket */
		
		memcpy(&available, next, sizeof(int) );
		
		available--;
		
		memcpy(next , &available, sizeof(int) );
		
		
		
		/* Recursive call */
		//Add2Bucket(&next, numOfRecords, bucketSize, record );
		
		//after recursive call , found the spot => returning	
		
		next = NULL;
		current = NULL;
		
		record = NULL;
		
		record1 = NULL;
		
		return;	
	}

	//else, next != NULL => recursive call for next bucket

	delete[] record1;	// we don't need record1 anymore 
	record1 = NULL;
	
	/* Recursive call */
	Add2Bucket1(&next, numOfRecords, bucketSize, record );
	
	next = NULL;
	current = NULL;
	
	record = NULL;
	
	return;
}








// Recursive Function 
void ClearBucket1(unsigned char** start, int numOfRecords, int bucket_size ){// numOfRecords = max number of records in bucket

	/* This function clears the bucket given by "start", and deletes said bucket in 3 scenarios:
	
		1) Bucket is empty => just delete it
		2) Bucket isn't full => no overflow buckets 
		
		3) Bucket is full, but *next = NULL => no overflow bucket
		
	*/

	int j;

	unsigned char* current = NULL;
	unsigned char* next = NULL;
	
	
	int available;
	int current_records;
	
	char* record = NULL;
	
	
	current = *start;
	
	memcpy(&available,current,sizeof(int) );
		
	current_records = numOfRecords - available;
		
	if( current_records == 0){	// no records found in this bucket ( so we just delete it and return )
		
		printf(" Bucket has no records, deleting bucket \n");
		
		delete[] current;	// char* = a local variable in main (
		current = NULL;
		
		*start = NULL;
		record = NULL;
			
		return;
		
	}// else there are records in this bucket
		
	/* Basically, we need to delete records (pointers to strings) inside this bucket */
		
	current = current + sizeof(int);	// move the pointer past the integer
		
	for( j = 0; j < current_records; j++){ // go through all the records currently in this bucket
		
		//record = (struct Records*) current;		// type cast current address -> struct Record
			
		
		/* Read record from bucket and store it in our local struct record */
		memcpy(&record,current, sizeof(char*) );
			
		/* "Delete" the Record */

		delete[] record;		//delete this string
		record = NULL;
			
		/* No need to write back, will be deleted anyways */
			
		/* Moving to the next record */
		
		current = current + sizeof(char*);		
		
	} // deleted all the pointers to strings  in this bucket
	
	
	
	
	if( current_records != numOfRecords ){	// bucket wasn't full 
	
		printf(" Bucket wasn't full, so it didn't have overflow buckets ==> deleting bucket \n");
		
		current = *start;
		
		delete[] current;
		
		current = NULL;
		*start = NULL;
		next = NULL;
		
		record = NULL;
		
		
		return;	

	} //else, bucket was full => check for overflow bucket
	
	//current = start + (bucket_size-1) - ( sizeof(unsigned char*) - 1 );		// the last 8 bytes are a char* pointer => next bucket;
	
	current = *start + ( bucket_size - 1);	// reached the end of the bucket
	current = current - ( sizeof(unsigned char*) - 1);	// go back a few bytes => reposition
			
	memcpy(&next,current, sizeof(unsigned char*) );		// next has the address(value) stored in *current
	
	
	
	
	
	if( next == NULL){	// no next buckets
			
		//delete bucket[i];
		//bucket[i] = NULL;
		
		
		
		delete[] *start;
		*start = NULL;
			
		next = NULL;
		current = NULL;
		
		record = NULL;
		
		//printf("Deleted all the records in this bucket, returning from ClearBucket() \n");	
				
		return;		// return to the previous bucket (or just return from the function )	
			
	}else{	// we have overflow buckets to explore
	
		ClearBucket1( &next, numOfRecords, bucket_size );		// recursive call , for the next bucket
		
		// after this , all subsequent buckets have been deleted, and *next = NULL
		
		delete[] *start;	//deleting this bucket (byte array)
		
		next = NULL;
		*start = NULL;
		current = NULL;
		
		record = NULL;
		
		//printf("Deleted all the records in this bucket, returning from ClearBucket() \n");	
		
		return;
	}
	
	
	
	printf("Deleted all the records in this bucket, returning from ClearBucket() \n");	
}















/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////


/* Some extra functions */


bool UserExists(List<Wallet>* list, char* user){

	/* This function chekcs, whether the user is already registered in the system/list */
	
	bool result = false;
	
	printf("\n\nInside USerEXISTS() \n\n");
	
	Node<Wallet>* node = list->head;
	
	while( node != NULL){	// traverse all the nodes of the list
	
		printf(" node->data->walletID %s and user %s\n", node->data->WalletID, user );
	
		if( strcmp( node->data->WalletID, user) == 0 ){  // if user exists
			printf("\n\n OK!!!!\n\n");
		
			result = true;
			break;		
		}
	
		node = node->next;
	
	}
	node = NULL;
	printf("\n\n");

	return result;
	//returns either true or false
}




bool BitcoinExists(List<RLNode>* list, char* bitcoin){

	/* This function chekcs, whether the user is already registered in the system/list */
	
	bool result = false;
	
	
	Node<RLNode>* node = list->head;
	
	while( node != NULL){	// traverse all the nodes of the list
	
		if( strcmp( node->data->name, bitcoin ) == 0 ){  // if bitcoin exists
		
			result = true;
			break;		
		}
	
		node = node->next;
	
	}
	node = NULL;

	return result;
	//returns either true or false
}





/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

Transaction::Transaction(char* tID, char* senderID, char* receiverID, int v, char* date, char* tim){

	int length = strlen(tID) + 1;
	transactionID = new char[length];
	
	strcpy(transactionID,tID);

//////////////////////////////////


	length = strlen(senderID) + 1;
	senderWalletID = new char[length];
	
	strcpy(senderWalletID,senderID);

//////////////////////////////////


	length = strlen(receiverID) + 1;
	receiverWalletID = new char[length];
	
	strcpy(receiverWalletID,receiverID);
	
//////////////////////////////////////


	value = v;

//////////////////////////////////////

	length = snprintf(NULL,0,"%s %s", date,tim ) + 1;

	char tim1[length];
	
	snprintf(tim1,length,"%s %s", date,tim) + 1;
	
	/* Now create the timestamp */
	struct tm vtm = {0};
	
	if( strptime(tim1, "%d-%m-%Y %H:%M",&vtm) != NULL){
	
		timestamp =	mktime(&vtm);	
	
	}else{ 
		printf(" Error in !!strptime!!! in Transaction constructor \n\n"); 
	}




}







Transaction::Transaction(char* tID, char* senderID, char* receiverID, int v, time_t time1){

	int length = strlen(tID) + 1;
	transactionID = new char[length];
	
	strcpy(transactionID,tID);

//////////////////////////////////


	length = strlen(senderID) + 1;
	senderWalletID = new char[length];
	
	strcpy(senderWalletID,senderID);

//////////////////////////////////


	length = strlen(receiverID) + 1;
	receiverWalletID = new char[length];
	
	strcpy(receiverWalletID,receiverID);
	
//////////////////////////////////////


	value = v;

//////////////////////////////////////

	timestamp = time1;



}


Transaction::Transaction(char* tID, char* senderID, char* receiverID, int v){	//date and time is not given

	int length = strlen(tID) + 1;
	transactionID = new char[length];
	
	strcpy(transactionID,tID);

//////////////////////////////////


	length = strlen(senderID) + 1;
	senderWalletID = new char[length];
	
	strcpy(senderWalletID,senderID);

//////////////////////////////////


	length = strlen(receiverID) + 1;
	receiverWalletID = new char[length];
	
	strcpy(receiverWalletID,receiverID);
	
//////////////////////////////////////


	value = v;

//////////////////////////////////////

	timestamp = time(NULL);		//get current time



}



Transaction::~Transaction(){

	delete[] transactionID;
	transactionID = NULL;
	
	delete[] senderWalletID;
	senderWalletID = NULL;
	
	delete[] receiverWalletID;
	receiverWalletID = NULL;
	
}


///////////////////////////////////////////////////////////////////


void Transaction::Print(){

	// we have to first make timestamp into string ===> strftime()

	char buffer[17];
	
	struct tm *tm1 = NULL;
	
	tm1 = localtime( &timestamp);
	
	strftime( buffer, 17,"%d-%m-%Y %H:%M",tm1);



	// Now we print
	printf("%s %s %s %d %s\n", transactionID, senderWalletID, receiverWalletID, value, buffer);
	
	return;
}


time_t Transaction::HourMinutes(){

	time_t result = 0;
	
	struct tm *tm1 = NULL;

	tm1 = localtime(&timestamp);
	
	struct tm tm2 = {0};
	tm2.tm_mday = 1;
	
	tm2.tm_hour = tm1->tm_hour;
	tm2.tm_min = tm1->tm_min;


	result = mktime(&tm2);
	
	return result;
}

















/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////










/* TNode */

TNode::TNode(char* name, int amount1){ // constructor for everybody else

	int length = strlen(name) + 1;
	
	user = new char[length];
	
	strcpy(user,name);
	
	amount = amount1;
	
	/* children */
	
	left = NULL;
	right = NULL;
	
	/* pointer to transaction Node */
	
	node = NULL;

}



TNode::TNode(char* name, int amount1, TLNode* tlnode){ // constructor for all the left children

	int length = strlen(name) + 1;
	
	user = new char[length];
	
	strcpy(user,name);
	
	amount = amount1;
	
	/* children */
	
	left = NULL;
	right = NULL;
	
	/* pointer to transaction Node */
	
	node = tlnode;	// not NULL, because it is a left children ( created because of a transaction)
	//node points the TLNode containing this transaction

}


TNode::~TNode(){ 

	delete[] user;
	user = NULL; 

	delete left;
	left = NULL;
	
	delete right;
	right = NULL;
	
	node = NULL;

}






void TNode::TransCount(int* counter, HashTable1* visited, bool root){	// recursive function to count how many transactions

	// visited List<char> or HashTable1(records will be transactionIDs)

	/* visited is a hashtable which contains transactionIds(char*) as records
	
		This is used because we might have a certain situation:
		
		Two Tree nodes (of this bitcoin Tree) where used for the same transaction ==> incorrect increment of counter	
	 */
	 

	if( root == true){
	
		if( left != NULL){		// traverse the left subtree
		
			printf(" HIIIIIIIIIIIIIIIIIIIIIIIIIIIII \n");
		
			(*counter)++;
			
			printf("counter is %d\n", *counter);
	
			left->TransCount(counter,visited,false);
		}


		if( right != NULL){
	
			right->TransCount(counter,visited,false);
		}

		return;
		
	}

////////////////////////////////////////////////////////////
	
	
	if( left != NULL){		// traverse the left subtree
		
		char* record = NULL;
		
		visited->SearchRecord1( this->node->transaction->transactionID, &record);		// search if we already visited this transaction
		
		if( record == NULL){
		
			(*counter)++;	//didn't find it , increment the counter
		
			visited->AddRecord1( this->node->transaction->transactionID );
	
		}
	
		record = NULL;	//don't need this anymore
	
	
		left->TransCount(counter,visited,false);
	}


	if( right != NULL){ // traverse the right subtree
	
		right->TransCount(counter,visited,false);
	}


}





void TNode::Unspent(int* unspent){

/* We will try to as |right| in the tree ,as we can.

	When we reach the final right node , *unspent = tnode->value
	
	If root->right = NULL  ==> return

*/

	if( this->right != NULL){
	
		// if right grandchild doesn't exist ===> right child is the last one ==>  *unspent = right->amount
	
		if( right->right != NULL){	//right grandchild exists =>continue
		
			right->Unspent(unspent);
		
		}else{	//  right grandchild doesn't exist 
		
			*unspent = right->amount;
		
		}	
	}

}



void TNode::TraceCoin(HashTable1* visited, bool root){

	if( root == true){
	
//		node->transaction->Print();	
	
		if( left != NULL){		// traverse the left subtree
		
			left->TraceCoin(visited,false);
		}


		if( right != NULL){
	
			right->TraceCoin(visited,false);
		}

		return;
		
	}
	
//////////////////////////////////////////////////////////////



	if( node != NULL){	// node is pointer to TLNode (general transactionList Node)
	
		char* record = NULL;
		
		visited->SearchRecord1( this->node->transaction->transactionID, &record);		// search if we already visited this transaction
		
		if( record == NULL){	//haven't printed this transaction
		
			visited->AddRecord1( this->node->transaction->transactionID );
			
			node->transaction->Print();	
		}
	
		record = NULL;	//don't need this anymore
	}
	
	
	
	
	
	if( left != NULL){		// traverse the left subtree	
	
		left->TraceCoin(visited,false);
	}


	if( right != NULL){ // traverse the right subtree
	
		right->TraceCoin(visited,false);
	}
}









/* Tree */

Tree::Tree(){ root = NULL; }

Tree::Tree(TNode* temp){ root = temp; }


Tree::~Tree(){ delete root; root = NULL; }


/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////







/* Template Node */

template <class T>

Node<T>::Node(T* temp){

	data = temp;
	
	next = NULL;
}

template <class T>

Node<T>::~Node(){

	delete data;
	data = NULL;
	
	next = NULL;

}




/* Template List */


template <class T>

List<T>::List(){

	head = NULL;
	tail = NULL;
	
}


template <class T>

List<T>::~List(){

	Node<T> *temp;
	
	while( head != NULL){
	
		temp = head;
		
		head = head->next;
	
		delete temp;
		temp = NULL;
	
	}

	temp = NULL;
	tail = NULL;

}


template <class T>

void List<T>::AddNode(T* temp){

	if( head == NULL){	// list is empty
	
		head = new Node<T>(temp);
		
		tail = head;
		
		return;	
	}

	// list is not empty

	Node<T> *node = new Node<T>(temp);
	
	tail->next = node;
	
	tail = node;
	
	node = NULL;


}

template <class T>

void List<T>::DeleteNode(T* temp){	// delete a specified node (might do Node<T>* temp)

	if( head == NULL){
	
		printf("List is empty , cannot delete\n");
		return;
	}
	

	
	// Check if head->data = temp
	
	if( head->data == temp){
	
		Node<T>* node = head;
		
		head = head->next;
		if(head == NULL) tail = NULL;
		
		//delete temp;
		delete node;
		node = NULL;	
		
		return;
	
	}
	
	// if tail is temp
	
	if( tail->data == temp){
	
		Node<T>* temp1 = head;
	
		while( temp1->next != tail) temp1 = temp1->next;
		// reached the node before tail
		
		tail = temp1;
		
		temp1 = temp1->next;
		delete temp1;
		temp1 = NULL;
		
		tail->next = NULL;
		
		return;
	}
	
	// temp is between head and tail
	
	Node<T>* current = head->next;
	Node<T>* previous = head;
	
	while( current != tail){
	
		if( current->data == temp){
		
			previous->next = current->next;
			
			delete current;
			
			current = NULL;
			previous = NULL;
			
			return;
		
		}
		
		// this isn't the node moving on
		
		previous = current;
		
		current = current->next;
	
	
	}
	
	previous = NULL;
	current = NULL;
	
	// didn't find the Node in this List
	
	printf(" Didn't find the node you were looking for, cannot delete \n");
	
	return;
}






/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////









/* Nodes used in template list ( aka data) */



	/* (1) BNode */
	
UserBNode::UserBNode(char* name, int v, PTNode* node){

	int length = strlen(name) + 1;
	
	BitcoinID = new char[length];
	strcpy(BitcoinID,name);
	
	value = v;
	
	current = new List<PTNode>();
	
	current->AddNode(node);
}


UserBNode::~UserBNode(){

	delete[] BitcoinID;
	BitcoinID = NULL;
	
	delete current;			// delete list
	current = NULL;
}



	/* (2) UserTLNode */

UserTLNode::UserTLNode( TLNode* tl ){

	transaction = tl;

	//nodes = list;
}



UserTLNode::~UserTLNode(){

	transaction = NULL;		// don't delete
	
	//delete nodes;
	//nodes = NULL;
}


	/* (3) TLNode */

TLNode::TLNode( Transaction* tr ){ transaction = tr; }

TLNode::~TLNode(){ delete transaction; transaction = NULL; }



	/* (4) RLNode */

RLNode::RLNode(char* st , TNode* tnode){

	int length = strlen( st) +1;
	
	name = new char[length];
	strcpy( name,st);
	
	tree = new Tree(tnode);
		
}


RLNode::~RLNode(){

	delete[] name;
	name = NULL;
	
	delete tree;
	tree = NULL;
}


	/* (5) PTNode */

PTNode::PTNode( TNode* pt ){ tnode = pt; }

PTNode::~PTNode(){ tnode = NULL; }		// don't delete






void PTNode::Transaction( int* amount, char* bitcoin, Wallet* receiver, TLNode* tlnode){

	/* 3 scenarios: 1) amount > tnode->value
					2) amount < tnode->value
					3) amount = tnode->value 
					
	*/
	
	printf("\nInside Transaction() \n\n");
	
	int temp = *amount - tnode->amount;
	
	printf("Hi \n");
	 
	if( temp >= 0){	// no right child (used all the value of this tree node)
	
		*amount = temp;
			
		// Create only the left child
		
		tnode->left = new TNode( receiver->WalletID, tnode->amount, tlnode);
		
		
		/* Connect tnode->left with receiver->UserBNode->PTnode */
				
		Node<UserBNode>* node = receiver->bitcoins->head;
	
		while( node != NULL){	// search if receiver has Bitcoin with bitcoinID = bitcoin 

			if( strcmp( node->data->BitcoinID, bitcoin) == 0) break;		// found our node
	
			node = node->next;	
		}
		
		printf("\n\npassed first while \n\n");

		if( node == NULL ){		// receiver doesn't have the bitcoin => Create UserBNode
		
			PTNode* ptnode = new PTNode(tnode->left);
				
			UserBNode* bnode = new UserBNode( bitcoin, tnode->amount, ptnode);		// the new left child has the same value as its parent
			
			receiver->bitcoins->AddNode(bnode);	// add to the user bitcoin list
		
			ptnode = NULL;
			bnode = NULL;
			
			node = NULL;
			
			return;	
		}
		// else , receiver has bitcoin
		
		PTNode* ptnode = new PTNode(tnode->left);
		
		//Node<PTNode>
		
		node->data->current->AddNode(ptnode);
		
		/* updating tnode value */
		this->tnode = tnode->right;	 // ==>NULL , will delete later (the PTNode)
		
		
		ptnode = NULL;
		node = NULL;
		
		return;
		
				
	}else{	// tnode->value > *amount (more than enough)
	
		// *amount = 0;	
	
		int temp1 = tnode->amount - (*amount);
		
				
		// Create left child
		
		tnode->left = new TNode( receiver->WalletID, *amount, tlnode);
		
		// Create right child
			
		tnode->right = new TNode( receiver->WalletID, temp1);
		
		
		
		/* Connect tnode->left with receiver->UserBNode->PTnode */
				
		Node<UserBNode>* node = receiver->bitcoins->head;
	
		while( node != NULL){	// search if receiver has Bitcoin with bitcoinID = bitcoin 

			if( strcmp( node->data->BitcoinID, bitcoin) == 0) break;		// found our node
	
			node = node->next;	
		}


//		return;		// temp

		if( node == NULL ){		// receiver doesn't have the bitcoin => Create UserBNode
				
			PTNode* ptnode = new PTNode(tnode->left);
		
//			UserBNode* bnode = new UserBNode( node->data->BitcoinID, *amount, ptnode);	// node = NULL
			
			UserBNode* bnode = new UserBNode( bitcoin, *amount, ptnode);
			
	//		return;
			
			receiver->bitcoins->AddNode(bnode);	// add to the user bitcoin list
		
			ptnode = NULL;
			bnode = NULL;
			
			node = NULL;
			
			*amount = 0;	// we paid the transaction cost
			
			return;	
		}
		// else , receiver has bitcoin
		
		PTNode* ptnode = new PTNode(tnode->left);
		
		node->data->current->AddNode(ptnode);
		
		ptnode = NULL;
		node = NULL;
		
		
		
		/* Connect tnode->right with sender->bnode->PTNode */
		
		this->tnode = tnode->right;		// not NULL;
		
		*amount = 0;	// we paid the transaction cost
		
		return;	
	}
		

}













/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////




/* Hash Table */

HashTable::HashTable(int buckets,int bsize){

	numOfBuckets = buckets;

	numOfRecords = ( bsize - (sizeof(int) + sizeof(char*)) ) / sizeof(struct Records);		// there is an  || int available (at the start of the bucket, if 0 => full) ||
	// and sizeof(char*) is the << next bucket >> pointer  

	bucket_size = bsize;


	/* Allocate memory for the hash table */

	bucket = new unsigned char*[buckets];

	int i;
	
	/* Now for the buckets */
	
	//char* current;
	
	for( i = 0; i < buckets; i++){
	
		bucket[i] = new unsigned char[bucket_size];		// alocating memory for bucket
		
		memcpy( bucket[i] , &numOfRecords, sizeof(int) );	// initialising available counter in each bucket => available = numOfRecords (max records)
		
		//current = bucket[i];	
	}

}





HashTable::~HashTable(){

	int i, j;
	
	int counter;		// how many overflow buckets
	
	
	char* current;
	char* start;
	char* next;
	
	char* previous;
	
	
	int available;
	int current_records;
	
	struct Records record;
	
	
	
	for( i = 0; i < numOfBuckets; i++){		// for each bucket start deleting the transaction list of each user
	
		//start = bucket[i];	
		//current = start;
		
		ClearBucket( &bucket[i] , numOfRecords, bucket_size);		// deleted alll overflow buckets and the "main" bucket
		
		bucket[i] = NULL;
		//bucket[i] was deleted 
	}
	
	
	delete[] bucket;
	bucket = NULL;
	
	
	/* We deleted all the buckets , nothing else to do */
	
		
}




void HashTable::AddRecord(struct Records* record){

	int k = HashFunction(record->wallet->WalletID, numOfBuckets );
	
	unsigned char** pointer = &bucket[k];


	Add2Bucket( &bucket[k], numOfRecords,bucket_size, record);
	
	record = NULL;
	
	pointer = NULL;
}




void HashTable::SearchRecord(char* WalletId, struct Records** record){

	int k = HashFunction(WalletId, numOfBuckets );

//	unsigned char** pointer = &bucket[k];

	SearchBucket( &bucket[k], numOfRecords, bucket_size,WalletId, record);
	
//	pointer = NULL;
}



/*
void HashTable::UpdateRecord(struct Records* record){

	int k = HashFunction(record->wallet->WalletID, numOfBuckets );


	SearchBucket(&bucket[k], numOfRecords, bucket_size, record);
}
*/




void HashTable::Print(){

	int i = 0;
	
	for( i = 0; i < numOfBuckets; i++){
	
		printf("\n\n Printing Bucket[%d] \n", i);
		
		PrintBucket( &bucket[i], numOfRecords, bucket_size );	
	
	}

	printf("\n Finished Printing \n\n");
}











/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////











/* HashTable1 (almost same functions ) */


HashTable1::HashTable1(int buckets,int bsize){

	numOfBuckets = buckets;

	numOfRecords = ( bsize - (sizeof(int) + sizeof(char*)) ) / sizeof(char *); // there is an  || int available (at the start of the bucket, if 0 => full) ||
	// and sizeof(char*) is the << next bucket >> pointer  

	bucket_size = bsize;


	/* Allocate memory for the hash table */

	bucket = new unsigned char*[buckets];

	int i;
	
	/* Now for the buckets */
	
	//char* current;
	
	for( i = 0; i < buckets; i++){
	
		bucket[i] = new unsigned char[bucket_size];		// alocating memory for bucket
		
		memcpy( bucket[i] , &numOfRecords, sizeof(int) );	// initialising available counter in each bucket => available = numOfRecords (max records)
		
		//current = bucket[i];	
	}

}




HashTable1::~HashTable1(){

	int i, j;
	
	int counter;		// how many overflow buckets
	
	
	char* current;
	char* start;
	char* next;
	
	char* previous;
	
	
	int available;
	int current_records;
	
	struct Records record;
	
	
	
	for( i = 0; i < numOfBuckets; i++){		// for each bucket start deleting the transaction list of each user
	
		//start = bucket[i];	
		//current = start;
		
		ClearBucket1( &bucket[i] , numOfRecords,  bucket_size);		// deleted alll overflow buckets and the "main" bucket
		
		bucket[i] = NULL;
		//bucket[i] was deleted 
	}
	
	
	delete[] bucket;
	bucket = NULL;
	
	
	/* We deleted all the buckets , nothing else to do */
	
		
}








void HashTable1::AddRecord1(char* record){

	int k = HashFunction(record, numOfBuckets );

	//unsigned char** pointer = &bucket[k];

	Add2Bucket1( &bucket[k], numOfRecords,bucket_size,record);
	
	record = NULL;
	
//	pointer = NULL;
}




void HashTable1::SearchRecord1(char* Id, char** record){

	int k = HashFunction(Id, numOfBuckets );

	//unsigned char** pointer = &bucket[k];

	SearchBucket1(&bucket[k], numOfRecords, bucket_size, Id, record);
	
	//pointer = NULL;
}




















/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////



/* Wallet */

Wallet::Wallet(char* user, int value){	// this is mostly used


	int length = strlen(user) + 1;
	
	WalletID = new char[length];
	
	strcpy(WalletID,user);
	
	////////////////////
	
	amount = 0;
	
	bitcoins = new List<UserBNode>();
	
	numOfBitcoins = 0;
	
	BitcoinValue = value;
}



Wallet::Wallet(char* user, List<UserBNode>* list, int value){


	int length = strlen(user) + 1;
	
	WalletID = new char[length];
	
	strcpy(WalletID,user);
	
	////////////////////
	
	amount = 0;
	
	bitcoins = list;
	
	numOfBitcoins = 0;
	
	BitcoinValue = value;
}


Wallet::~Wallet(){

	delete[] WalletID;
	WalletID = NULL;
	
	
	delete bitcoins;
	bitcoins = NULL;
}






/* Instantiating the template classes 

	else, we have undefined refference */

template class List<Wallet>;
template class List<TLNode>;
template class List<RLNode>;

template class List<UserTLNode>;

template class List<PTNode>;
template class List<UserBNode>;




















