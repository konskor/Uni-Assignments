#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BF.h"
#include "HT.h"

/* Hash Functions */

int HashInt(int x, int M){		// Hash function for int (id) ==> Primary HT

// x = input
// M is size of hash table (buckets)



	int a ,b;		// random numbers
	int p;		// BIG prime number

	a = 1;
	b = 2;

	p = 1009;

	int result =  ( ( a * abs(x) + b) % p) % M;			// abs() => absolute value

	return result + 1;			// +1 because we are starting from the second block
}







int HashS(char *string, int M){		// Hash function for char* (all the others) ==> Secondary HT

// string = input
// M is size of hash table (buckets)



	int a;		// random numbers
	int p;		// BIG prime number

	int h;		// result of hashing
	int i;


	a = 0;			// instead of 1

	p = 1009;

	for( i = 0; i < strlen(string); i++){

		h = ( h * a + string[i]) % p;

	}


	return ( abs(h) % M) + 1;			// +1 because we are starting from the second block
}



/////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////


/* Primary Hash functions */

int HT_CreateIndex( char *fileName, char attrType, char* attrName, int attrLength, int buckets){


	int fd;


	if( BF_CreateFile(fileName) < 0){		// creating the HT file

		BF_PrintError("BF_Create error in HT_CreateIndex() : ");
		return -1;

	}

	if( ( fd = BF_OpenFile(fileName) ) < 0){		// opening file to use

		BF_PrintError("BF_Open error in HT_CreateIndex() : ");
		return -1;

	}


	// Creating HT_Info 


	Block0* block1 = malloc( sizeof(Block0) );

	if( block1 == NULL){

		perror(" Malloc() error in HT_CreateIndex() : ");

		return -1;
	}


	strcpy(block1->type,"HT");

	block1->fd = fd;

	block1->attrLength=attrLength;
	strcpy(block1->attrName,attrName);

	block1->attrType=attrType;
	block1->numBuckets=buckets;


	// Allocating buckets+1 blocks 

	int i;
	for(i = 0 ; i < buckets + 1 ; i++){			// basically we want the (buckets + the starting block) which has HT_info

		if( BF_AllocateBlock(fd) < 0){

			BF_PrintError("BF_AllocateBlock error in HT_CreateIndex() : ");
			return -1;

		}
	}


	// Now Read first block and Write in this one , HT_info 

	void* block = malloc( BLOCK_SIZE) ;

	if( BF_ReadBlock( fd, 0, &block) < 0){					// now block points to where the "block data" is stored

		BF_PrintError("BF_ReadBlock error in HT_CreateIndex() : ");
		return -1;

	}


	memcpy(block,block1,sizeof(Block0));				// copy HT_info in "block data"


	if( BF_WriteBlock(fd,0) < 0){

		BF_PrintError("BF_WriteBlock error in HT_CreateIndex() : ");
		return -1;

	}



	// Closing Block file 

	if( BF_CloseFile(fd) < 0){

		BF_PrintError("BF_CloseFile error in HT_CreateIndex() : ");
		return -1;
	}

	// Freeing memory 

	free( block1);

	return 0;
}


///////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////



HT_info* HT_OpenIndex(char* filename){

	int fd;

	if( ( fd = BF_OpenFile(filename) ) < 0){		// opening file to use

		BF_PrintError("BF_Open error in HT_OpenIndex() : ");
		return NULL;

	}


	void* block = malloc( BLOCK_SIZE) ;

	if( BF_ReadBlock( fd, 0, &block) < 0){					// now block points to where the "block data" is stored

		BF_PrintError("BF_ReadBlock error in HT_OpenIndex() : ");
		return NULL;

	}

	Block0* block1=malloc(sizeof(Block0));

	if( block1 == NULL){

		perror(" Malloc error in HT_OpenIndex() : ");
		return NULL;

	}


	memcpy(block1,block,sizeof(HT_info) );				// transfering the bytes of block to HT_info




	if( strcmp(block1->type,"HT") != 0){		// this isn't a HT file

		printf("This isn't a HT file\n\n");

		free(block1);

		return NULL;

	}



	block1->fd = fd;			// fix this since each time we "Open" there is a different descriptor ===> fileDescriptor might not even be needed to save in block (based on eclass)

	/* Update the file descriptor in block 0 */

	memcpy(block, block1, sizeof(Block0) );

	if( BF_WriteBlock(fd,0) < 0){

		BF_PrintError("BF_WriteBlock error in HT_OpenIndex() : ");
		return NULL;
	}


	/* Create the HT_info */

	HT_info* HT_Info = malloc( sizeof(HT_info) );

	HT_Info->fileDesc = block1->fd;

	HT_Info->attrType = block1->attrType;

	HT_Info->attrLength = block1->attrLength;
	HT_Info->attrName = malloc( block1->attrLength);

	strcpy( HT_Info->attrName, block1->attrName);


	HT_Info->numBuckets = block1->numBuckets;

	printf("HT_Info->numBuckets %ld\n", HT_Info->numBuckets);


	// we don't close the file

	free(block1);

	return HT_Info;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


int HT_InsertEntry(HT_info header_info, Record record){	// CHECK later


	/* File is already open */

	int fd = header_info.fileDesc;

	int bucket1;

	if( header_info.attrType == 'i'){		// integer

		bucket1 = HashInt(record.id , header_info.numBuckets);

	}else{		// char

		if( strcmp( header_info.attrName, "name") == 0){

			bucket1 = HashS(record.name , header_info.numBuckets);

		}else if( strcmp( header_info.attrName, "surname") == 0){

			bucket1 = HashS(record.surname , header_info.numBuckets);

		}else{	// address

			bucket1 = HashS(record.address , header_info.numBuckets);

		}

	}

	int blockid = bucket1;

	/* Start reading blocks from bucket1 */

	void* block = malloc( BLOCK_SIZE);

	Block *block1 = malloc( sizeof(Block) );

	if( BF_ReadBlock( fd, blockid, &block) < 0){// now block points to where the "block data" is stored

		BF_PrintError("BF_ReadBlock error in HT_InsertEntry() (1) : ");
		return -1;

	}

	memcpy( block1, block, sizeof(Block) );			// filling our Block structure


	int temp = 0;

	while( block1->counter == 6){	// Move to the  block1.next block , untill we find empty space


		if( block1->next == 0){		// there is no next Block


			if( BF_AllocateBlock(header_info.fileDesc) < 0 ){

				BF_PrintError("BF_AllocateBlock error in HT_InsertEntry() : ");
				return -1;
			}



			if( ( temp = BF_GetBlockCounter( header_info.fileDesc) ) < 0 ){

				BF_PrintError("BF_GetBlockCounter error in HT_InsertEntry() : ");
				return -1;
			}

			block1->next = temp - 1;		// newly added block(added in the end) has blockid BF_GetBlockCounter -1

			memcpy( block, block1, sizeof(Block) );			// Writing back the modified block

			if( BF_WriteBlock( fd, blockid) < 0){

				BF_PrintError("BF_WriteBlock error in HT_InsertEntry() (1) : ");
				return -1;

			}

		}



		/* Now read the next block */

		blockid = block1->next;

		if( BF_ReadBlock( fd, block1->next, &block) < 0){					// now block points to where the "block data" is stored

			BF_PrintError("BF_ReadBlock error in HT_InsertEntry() (2) : ");
			return -1;

		}


		memcpy( block1, block, sizeof(Block) );			// filling our Block structure (with next Block)

	}

	// We found empty space in the block with blockId = blockid

	/* Add the record at the end */

	block1->records[block1->counter].id = record.id;

	strcpy( block1->records[block1->counter].name , record.name);
	strcpy( block1->records[block1->counter].surname , record.surname);
	strcpy( block1->records[block1->counter].address , record.address);

	block1->counter++;			// increment the counter

	memcpy( block, block1, sizeof(Block) );			// filling our Block structure (with next Block)

	if( BF_WriteBlock( fd, blockid) < 0){

		BF_PrintError("BF_WriteBlock error in HT_InsertEntry() (2) : ");
		return -1;

	}

	free(block1);

	return blockid;

}


/////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
///////////////////////////////////////////////////////
//////////////////////////////////////////////////////


int HT_GetAllEntries(HT_info header_info, void* value){

	/* File is already open */

	int bucket1;
	int blocks_counter = 0;

	int fd = header_info.fileDesc;

	int records_found = 0;			// if by the end , records_found == 0 , return -1

	if( header_info.attrType == 'i'){		// integer

		int* value1 = (int*) value;

		bucket1 = HashInt(*value1 , header_info.numBuckets);

		int blockid = bucket1;


		/* Start reading blocks from bucket1 */

		void* block = malloc( BLOCK_SIZE) ;

		Block* block1 = malloc( sizeof(Block) );

		if( BF_ReadBlock( fd, blockid, &block) < 0){					// now block points to where the "block data" is stored

			BF_PrintError("BF_ReadBlock error in HT_GetAllEntries() (1) : ");
			return -1;

		}

		memcpy( block1, block, sizeof(Block) );			// filling our Block structure

		blocks_counter++;				// how many blocks we have read


		int i;

		/* Search first block */

		for( i = 0; i < block1->counter; i++){



			if( block1->records[i].id == *value1 ){

				printf("Record with (key) id: %d, name: %s, surname: %s, address: %s \n", block1->records[i].id, block1->records[i].name, block1->records[i].surname, block1->records[i].address);

				records_found++;

			}
		}

		/* Search the rest of the "list" of blocks */

		while( block1->next != 0){		// Move to the  block1.next block , untill we find empty space

			blockid = block1->next;

			if( BF_ReadBlock( fd, block1->next, &block) < 0){					// now block points to where the "block data" is stored

				BF_PrintError("BF_ReadBlock error in HT_GetAllEntries() (2) : ");
				return -1;

			}

			memcpy( block1, block, sizeof(Block) );			// filling our Block structure

			blocks_counter++;				// how many blocks we have read


			for( i = 0; i < block1->counter; i++){

				if( block1->records[i].id == *value1 ){

					printf("Record with (key) id: %d, name: %s, surname: %s, address: %s \n", block1->records[i].id, block1->records[i].name, block1->records[i].surname, block1->records[i].address);

					records_found++;

				}
			}



		}


		if( records_found == 0){

			printf("No entry found with request value %d \n", *value1);
			return -1;

		}


		free(block1);

		return blocks_counter;


	}else{		// char

		if( strcmp( header_info.attrName, "name") == 0){

			char* value1 = (char*) value;

			bucket1 = HashS(value1 , header_info.numBuckets);

			int blockid = bucket1;

			/* Start reading blocks from bucket1 */

			void* block = malloc( BLOCK_SIZE) ;

			Block* block1 = malloc(sizeof(Block) );

			if( BF_ReadBlock( fd, blockid, &block) < 0){					// now block points to where the "block data" is stored

				BF_PrintError("BF_ReadBlock error in HT_GetAllEntries() (3) : ");
				return -1;

			}

			memcpy( block1, block, sizeof(Block) );			// filling our Block structure

			blocks_counter++;				// how many blocks we have read

			int i;

			/* Search first block */

			for( i = 0; i < block1->counter; i++){

				if( strcmp( block1->records[i].name , value1 ) == 0){

					printf("Record with id: %d, (key) name: %s, surname: %s, address: %s \n", block1->records[i].id, block1->records[i].name, block1->records[i].surname, block1->records[i].address);

						records_found++;

				}
			}

			/* Search the rest of the "list" of blocks */

			while( block1->next != 0){		// Move to the  block1.next block , untill we find empty space

				blockid = block1->next;

				if( BF_ReadBlock( fd, block1->next, &block) < 0){					// now block points to where the "block data" is stored

					BF_PrintError("BF_ReadBlock error in HT_GetAllEntries() (4) : ");
					return -1;

				}

				memcpy( block1, block, sizeof(Block) );			// filling our Block structure

				blocks_counter++;				// how many blocks we have read


				for( i = 0; i < block1->counter; i++){

					if( strcmp( block1->records[i].name , value1 ) == 0){

						printf("Record with id: %d, (key) name: %s, surname: %s, address: %s\n",block1->records[i].id, block1->records[i].name, block1->records[i].surname, block1->records[i].address);

							records_found++;

					}
				}

			}



		if( records_found == 0){

			printf("No entry found with request value (2) \n\n");
			return -1;

		}

			free(block1);

			return blocks_counter;





		}else if( strcmp( header_info.attrName, "surname") == 0){

			char* value1 = (char*) value;

			bucket1 = HashS(value1 , header_info.numBuckets);

			int blockid = bucket1;

			/* Start reading blocks from bucket1 */

			void* block = malloc( BLOCK_SIZE) ;

			Block* block1 = malloc( sizeof(Block) );

			if( BF_ReadBlock( fd, blockid, &block) < 0){					// now block points to where the "block data" is stored

				BF_PrintError("BF_ReadBlock error in HT_GetAllEntries() (5) : ");
				return -1;

			}

			memcpy( block1, block, sizeof(Block) );			// filling our Block structure

			blocks_counter++;				// how many blocks we have read

			int i;

			/* Search first block */

			for( i = 0; i < block1->counter; i++){



				if( strcmp( block1->records[i].surname , value1 ) == 0){

					printf("Record with id: %d, name: %s, (key) surname: %s, address: %s \n", block1->records[i].id, block1->records[i].name, block1->records[i].surname, block1->records[i].address);

					records_found++;
				}
			}

			/* Search the rest of the "list" of blocks */

			while( block1->next != 0){		// Move to the  block1.next block , untill we find empty space

				blockid = block1->next;

				if( BF_ReadBlock( fd, block1->next, &block) < 0){					// now block points to where the "block data" is stored

					BF_PrintError("BF_ReadBlock error in HT_GetAllEntries() (6) : ");
					return -1;

				}

				memcpy( block1, block, sizeof(Block) );			// filling our Block structure

				blocks_counter++;				// how many blocks we have read


				for( i = 0; i < block1->counter; i++){

					if( strcmp( block1->records[i].surname , value1 ) == 0){

						printf("Record with id: %d, name: %s, (key) surname: %s, address: %s\n",block1->records[i].id, block1->records[i].name, block1->records[i].surname, block1->records[i].address);

						records_found++;
					}
				}

			}



			if( records_found == 0){

				printf("No entry found with request value (3) \n\n");
				return -1;

			}

			free(block1);

			return blocks_counter;

		}else{	// address

			char* value1 = (char*) value;

			bucket1 = HashS(value1 , header_info.numBuckets);
			int blockid = bucket1;

			/* Start reading blocks from bucket1 */

			void* block = malloc( BLOCK_SIZE) ;

			Block *block1 = malloc( sizeof(Block) );

			if( BF_ReadBlock( fd, blockid, &block) < 0){					// now block points to where the "block data" is stored

				BF_PrintError("BF_ReadBlock error in HT_GetAllEntries() (7) : ");
				return -1;

			}

			memcpy( block1, block, sizeof(Block) );			// filling our Block structure

			blocks_counter++;				// how many blocks we have read

			int i;

			/* Search first block */

			for( i = 0; i < block1->counter; i++){

				if( strcmp( block1->records[i].address , value1 ) == 0) {

					printf("Record with id: %d, name: %s, surname: %s, (key) address: %s \n", block1->records[i].id, block1->records[i].name, block1->records[i].surname, block1->records[i].address);

					records_found++;
				}
			}


			/* Search the rest of the "list" of blocks */

			while( block1->next != 0){		// Move to the  block1.next block , untill we find empty space

				blockid = block1->next;

				if( BF_ReadBlock( fd, block1->next, &block) < 0){					// now block points to where the "block data" is stored

					BF_PrintError("BF_ReadBlock error in HT_GetAllEntries() (8) : ");
					return -1;

				}

				memcpy( block1, block, sizeof(Block) );			// filling our Block structure

				blocks_counter++;				// how many blocks we have read


				for( i = 0; i < block1->counter; i++){

					if( strcmp( block1->records[i].address , value1 ) == 0){

						printf("Record with id: %d, name: %s, surname: %s, (key) address: %s\n",block1->records[i].id, block1->records[i].name, block1->records[i].surname, block1->records[i].address);

						records_found++;
					}
				}

			}



			if( records_found == 0){

				printf("No entry found with request value (4) \n\n");
				return -1;

			}

			free(block1);

			return blocks_counter;


		}

	}

}




/////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////





int HT_CloseIndex( HT_info* header_info ){

	int fileDesc=header_info->fileDesc;

	if(BF_CloseFile(fileDesc)<0){

		BF_PrintError("BF_CloseFile error in HT_CloseIndex() : ");
		return -1;
	}

	free(header_info->attrName);
	free(header_info);

	return 0;
}

/////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////



int HT_DeleteEntry( HT_info header_info, void *value){

	int bucket1;
	int fd = header_info.fileDesc;

	if( header_info.attrType == 'i'){

		int* value1 = (int*) value;

		bucket1 = HashInt( *value1 , header_info.numBuckets);
		int blockid = bucket1;

		void* block = malloc( BLOCK_SIZE) ;

		Block* block1 = malloc( sizeof(Block) );


		if( BF_ReadBlock( fd, blockid, &block) < 0){

			BF_PrintError("BF_ReadBlock error in HT_DeleteEntry() (1) : ");
			return -1;
		}

		memcpy( block1, block, sizeof(Block) );

		int i;

		/* Searching the first block */

		for( i = 0; i < block1->counter; i++){

			if( block1->records[i].id == *value1 ){

				if((i+1)!=block1->counter){	// if its not the last record in block

				// i != block1.counter-1

				// copy the contents of the last record, to this records[i]

					block1->records[i].id=block1->records[block1->counter-1].id;

					strcpy( block1->records[i].name ,block1->records[block1->counter-1].name);

					strcpy(block1->records[i].surname , block1->records[block1->counter-1].surname);

					strcpy( block1->records[i].address , block1->records[block1->counter-1].address);

				}

				block1->counter--;

				printf("Deleted record with id:%d from block : %d\n", *value1, blockid);

				/* Commiting the changes */

				memcpy(block,block1,sizeof(Block) );

				if( BF_WriteBlock(fd,blockid) < 0){

					BF_PrintError("BF_WriteBlock error in HT_DeleteEntry() (1): ");
					return -1;

				}

				return 0;
			}

		}


		/* Searching the rest of the blocks */


		while( block1->next != 0){		// Move to the  block1.next block

			blockid = block1->next;

			if( BF_ReadBlock( fd, block1->next, &block) < 0){	// now block points to where the "block data" is stored
				BF_PrintError("BF_ReadBlock error in HT_DeleteEntry() (2) : ");
				return -1;
			}

			memcpy( block1, block, sizeof(Block) );			// filling our Block structure

			for( i = 0; i < block1->counter; i++){

				if( block1->records[i].id == *value1 ){

					if((i+1)!=block1->counter){

						block1->records[i].id=block1->records[block1->counter-1].id;

						strcpy( block1->records[i].name ,block1->records[block1->counter-1].name);

						strcpy(block1->records[i].surname,block1->records[block1->counter-1].surname);

						strcpy( block1->records[i].address , block1->records[block1->counter-1].address);

					}

					block1->counter--;
					printf("Deleted record with id:%d from block : %d\n", *value1, blockid);

					/* Commiting the changes */

					memcpy(block,block1,sizeof(Block) );

					if( BF_WriteBlock(fd,blockid) < 0){

						BF_PrintError("BF_WriteBlock error in HT_DeleteEntry() (2) : ");
						return -1;

					}


					return 0;

				}

			}

		}
		printf("No entry with requested value found\n");
		return -1;
	}
	else{		// char

		if( strcmp( header_info.attrName, "name") == 0){

			char* value1 = (char*) value;

			bucket1 = HashS(value1 , header_info.numBuckets);

			int blockid = bucket1;

			void* block = malloc( BLOCK_SIZE) ;

			Block* block1 = malloc( sizeof(Block) );

			if( BF_ReadBlock( fd, blockid, &block) < 0){// now block points to where the "block data" is stored
				BF_PrintError("BF_ReadBlock error in HT_DeleteEntry() (3) : ");
				return -1;
			}

			memcpy( block1, block, sizeof(Block) );			// filling our Block structure

			int i;
			/* Searching the first block */

			for( i = 0; i < block1->counter; i++){

				if( strcmp( block1->records[i].name , value1 ) == 0){

					if((i+1)!=block1->counter){

						block1->records[i].id=block1->records[block1->counter-1].id;

						strcpy( block1->records[i].name ,block1->records[block1->counter-1].name);

						strcpy( block1->records[i].surname , block1->records[block1->counter-1].surname);

						strcpy( block1->records[i].address , block1->records[block1->counter-1].address);

					}

					block1->counter--;

					printf("Deleted record with name:%s from block : %d\n", value1, blockid);

					/* Commiting the changes */

					memcpy(block,block1,sizeof(Block) );

					if( BF_WriteBlock(fd,blockid) < 0){

						BF_PrintError("BF_WriteBlock error in HT_DeleteEntry() (3) : ");
						return -1;

					}



					return 0;
				}
			}

			/* Search the rest of the blocks */

			while( block1->next != 0){		// Move to the  block1.next block

				blockid = block1->next;

				if( BF_ReadBlock( fd, block1->next, &block) < 0){// now block points to where the "block data" is stored

					BF_PrintError("BF_ReadBlock error in HT_DeleteEntry() (4) : ");
					return -1;
				}

				memcpy( block1, block, sizeof(Block) );			// filling our Block structure

				for( i = 0; i < block1->counter; i++){

					if( strcmp( block1->records[i].name , value1 ) == 0){

						if((i+1)!=block1->counter){

							block1->records[i].id=block1->records[block1->counter-1].id;

							strcpy( block1->records[i].name ,block1->records[block1->counter-1].name);

							strcpy( block1->records[i].surname , block1->records[block1->counter-1].surname);

							strcpy( block1->records[i].address , block1->records[block1->counter-1].address);
						}

						block1->counter--;

						printf("Deleted record with name: %s from block: %d\n", value1, blockid);

						/* Commiting the changes */

						memcpy(block,block1,sizeof(Block) );

						if( BF_WriteBlock(fd,blockid) < 0){

							BF_PrintError("BF_WriteBlock error in HT_DeleteEntry() (4) : ");
							return -1;

						}

						return 0;
					}
				}
			}

			printf("No entry with requested value found\n");
			return -1;
		}
		else if( strcmp( header_info.attrName, "surname") == 0){

			char* value1 = (char*) value;

			bucket1 = HashS(value , header_info.numBuckets);
			int blockid = bucket1;

			void* block = malloc( BLOCK_SIZE) ;

			Block* block1 = malloc( sizeof( Block) );

			if( BF_ReadBlock( fd, blockid, &block) < 0){// now block points to where the "block data" is stored

				BF_PrintError("BF_ReadBlock error in HT_DeleteEntry() (5) : ");
				return -1;
			}

			memcpy( block1, block, sizeof(Block) );			// filling our Block structure
			int i;

			for( i = 0; i < block1->counter; i++){

				if( strcmp( block1->records[i].surname , value1 ) == 0){

					if((i+1)!=block1->counter){

						block1->records[i].id=block1->records[block1->counter-1].id;

						strcpy( block1->records[i].name ,block1->records[block1->counter-1].name);

						strcpy( block1->records[i].surname , block1->records[block1->counter-1].surname);

						strcpy( block1->records[i].address , block1->records[block1->counter-1].address);

					}

					block1->counter--;

					printf("Deleted record with surname: %s from block: %d\n", value1, blockid);

					/* Commiting the changes */

					memcpy(block,block1,sizeof(Block) );

					if( BF_WriteBlock(fd,blockid) < 0){

						BF_PrintError("BF_WriteBlock error in HT_DeleteEntry() (5) : ");
						return -1;

					}

					return 0;
				}
			}

			/* Search the rest of the blocks */

			while( block1->next != 0){		// Move to the  block1.next block

				blockid = block1->next;

				if( BF_ReadBlock( fd, block1->next, &block) < 0){					// now block points to where the "block data" is stored

					BF_PrintError("BF_ReadBlock error in HT_DeleteEntry() (6) : ");
					return -1;
				}

				memcpy( block1, block, sizeof(Block) );			// filling our Block structure

				for( i = 0; i < block1->counter; i++){

					if( strcmp( block1->records[i].surname , value1 ) == 0){

						if((i+1)!=block1->counter){

							block1->records[i].id=block1->records[block1->counter-1].id;

							strcpy( block1->records[i].name ,block1->records[block1->counter-1].name);

							strcpy( block1->records[i].surname , block1->records[block1->counter-1].surname);

							strcpy( block1->records[i].address , block1->records[block1->counter-1].address);

						}

						block1->counter--;

						printf("Deleted record with surname: %s from block:%d\n",value1,blockid);

						/* Commiting the changes */

						memcpy(block,block1,sizeof(Block) );

						if( BF_WriteBlock(fd,blockid) < 0){

							BF_PrintError("BF_WriteBlock error in HT_DeleteEntry() (6) : ");
							return -1;

						}


						return 0;
					}
				}
			}
			printf("No entry with requested value found\n");
			return -1;		
		}
		else{	// address

			char* value1 = (char*) value;

			bucket1 = HashS(value1 , header_info.numBuckets);
			int blockid = bucket1;

			void* block = malloc( BLOCK_SIZE) ;

			Block* block1 = malloc( sizeof(Block) );

			if( BF_ReadBlock( fd, blockid, &block) < 0){					// now block points to where the "block data" is stored

				BF_PrintError("BF_ReadBlock error in HT_DeleteEntry() (7) : ");
				return -1;
			}

			memcpy( block1, block, sizeof(Block) );			// filling our Block structure
			int i;

			for( i = 0; i < block1->counter; i++){

				if( strcmp( block1->records[i].surname , value1 ) == 0) {

					if((i+1)!=block1->counter){

						block1->records[i].id=block1->records[block1->counter-1].id;

						strcpy( block1->records[i].name ,block1->records[block1->counter-1].name);

						strcpy( block1->records[i].surname , block1->records[block1->counter-1].surname);

						strcpy( block1->records[i].address , block1->records[block1->counter-1].address);

					}

					block1->counter--;

					printf("Deleted record with address: %s from block: %d\n", value1, blockid);

					/* Commiting the changes */

					memcpy(block,block1,sizeof(Block) );

					if( BF_WriteBlock(fd,blockid) < 0){

						BF_PrintError("BF_WriteBlock error in HT_DeleteEntry() (7) : ");
						return -1;

					}


					return 0;
				}
			}

			/*Search the rest of the blocks */

			while( block1->next != 0){		// Move to the  block1.next block

				blockid = block1->next;

				if( BF_ReadBlock( fd, block1->next, &block) < 0){					// now block points to where the "block data" is stored

					BF_PrintError("BF_ReadBlock error in HT_DeleteEntry() (8) : ");
					return -1;
				}

				memcpy( block1, block, sizeof(Block) );			// filling our Block structure

				for( i = 0; i < block1->counter; i++){

					if( strcmp( block1->records[i].surname , value1 ) == 0){

						if((i+1)!=block1->counter){

							block1->records[i].id=block1->records[block1->counter-1].id;

							strcpy( block1->records[i].name ,block1->records[block1->counter-1].name);

							strcpy( block1->records[i].surname , block1->records[block1->counter-1].surname);

							strcpy( block1->records[i].address , block1->records[block1->counter-1].address);

						}

						block1->counter--;

						printf("Deleted record with address: %s from block:%d\n",value1,blockid);

						/* Commiting the changes */

						memcpy(block,block1,sizeof(Block) );

						if( BF_WriteBlock(fd,blockid) < 0){

							BF_PrintError("BF_WriteBlock error in HT_DeleteEntry() (8) : ");
							return -1;

						}


						return 0;
					}
				}
			}

			printf("No entry with requested value found\n");
			return -1;
		}
	}
}




/////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


int SHT_CreateSecondaryIndex( char *sfileName, char* attrName, int attrLength, int buckets, char* filename){


	int fd;


	if( BF_CreateFile(sfileName) < 0){		// creating the HT file

		BF_PrintError("BF_Create error in SHT_CreateSecondaryIndex() : ");
		return -1;

	}

	if( ( fd = BF_OpenFile(sfileName) ) < 0){		// opening file to use

		BF_PrintError("BF_Open error in SHT_CreateSecondaryIndex() : ");
		return -1;

	}

	/* Creating block0 */


	Block0* block0=malloc(sizeof(Block0));

	if( block0 == NULL){

		perror(" Malloc() error in SHT_CreateSecondaryIndex() : ");

		return -1;
	}


	strcpy(block0->type,"SHT");
	block0->fd = fd;

	block0->attrLength=attrLength;
	strcpy(block0->attrName,attrName);

	block0->numBuckets=buckets;
	strcpy(block0->filename,filename);


	/* Allocating buckets+1 blocks */

	int i;
	for(i = 0 ; i < buckets + 1 ; i++){			// basically we want the (buckets + the starting block) which has SHT_info

		if( BF_AllocateBlock(fd) < 0){

			BF_PrintError("BF_AllocateBlock error in SHT_CreateSecondaryIndex() : ");
			return -1;

		}
	}


	/* Now Read first block and Write in this one , HT_info */

	void* block  = malloc( BLOCK_SIZE) ;

	if( BF_ReadBlock( fd, 0, &block) < 0){					// now block points to where the "block data" is stored

		BF_PrintError("BF_ReadBlock error in SHT_CreateSecondaryIndex() : ");
		return -1;

	}


	memcpy(block,block0,sizeof(Block0));				// copy HT_info in "block data"


	if( BF_WriteBlock(fd,0) < 0){

		BF_PrintError("BF_WriteBlock error in SHT_CreateSecondaryIndex() : ");
		return -1;

	}

	/* Now we need to "synchornize" the two indexes (primary and secondary) 
		They might not have the same buckets...

	*/

		/* Get header_info of primary HT */

	HT_info* HT_Info = HT_OpenIndex(filename);

	int fd1 = HT_Info->fileDesc;

	SHT_info* SHT_Info = SHT_OpenSecondaryIndex(sfileName);

	/* "Copy" all the entries to this index */

	int j, blockid;

	void* block1 = malloc( BLOCK_SIZE);

	Block* block2 = malloc( sizeof(Block) );

	SecondaryRecord Srecord;

	printf(" HT_Info->numBuckets  %ld\n", HT_Info->numBuckets);

	for( i = 1; i < HT_Info->numBuckets + 1; i++){	// traverse all the buckets

		blockid = i;

		/* Search the bucket for records */

		if( BF_ReadBlock( fd1, i, &block1) < 0){					// now block points to where the "block data" is stored

			BF_PrintError("BF_ReadBlock error in SHT_CreateSecondaryIndex() (2) : ");
			return -1;

		}

		memcpy( block2, block1, sizeof(Block) );

		for( j = 0; j < block2->counter ; j++){

			Srecord.record = block2->records[j];

			Srecord.blockId = blockid;

			if( SHT_SecondaryInsertEntry(*SHT_Info, Srecord) == -1){

				printf(" Error SHT_Insert , in CreateSecondaryIndex() (1) \n");

				return -1;
			}

		}

		// finished with the bucket/block, go to the overflow blocks

		while( block2->next != 0 ){

			blockid = block2->next;

			if( BF_ReadBlock( fd1, blockid, &block1) < 0){// now block points to where the "block data" is stored

				BF_PrintError("BF_ReadBlock error in SHT_CreateSecondaryIndex() (3) : ");
				return -1;
			}

			memcpy( block2, block1, sizeof(Block) );

			for( j = 0; j < block2->counter ; j++){

				Srecord.record = block2->records[j];

				Srecord.blockId = blockid;

				if( SHT_SecondaryInsertEntry(*SHT_Info, Srecord) == -1){

					printf(" Error SHT_Insert , in CreateSecondaryIndex() (2) \n");

					return -1;
				}


			}


		}

		// finished wit the overlow blocks ==> move on to the next bucket

	}

	// finished Synchronising closing primary index and freeing memory 

	if( HT_CloseIndex(HT_Info) == -1){

		printf(" Error closing primary index , in CreateSecondaryIndex() ");
		return -1;
	}


	if( SHT_CloseSecondaryIndex(SHT_Info) == -1){

		printf(" Error closing secondary index , in CreateSecondaryIndex() ");
		return -1;
	}


	free( block2);


	/* End of Synchronisation */


	/* Closing Block file */

	if( BF_CloseFile(fd) < 0){

		BF_PrintError("BF_CloseFile error in SHT_CreateSecondaryIndex() : ");
		return -1;
	}

	/* Freeing memory */

	free( block0);
	SHT_Info = NULL;

	return 0;
}


/////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


int SHT_CloseSecondaryIndex( SHT_info* header_info ){

	int fileDesc=header_info->fileDesc;

	if(BF_CloseFile(fileDesc)<0){

		BF_PrintError("BF_CloseFile error in SHT_CloseSecondaryIndex() : ");
		return -1;
	}

	free(header_info->attrName);
	free(header_info);

	return 0;
}


/////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


int SHT_SecondaryGetAllEntries(SHT_info header_info_sht, HT_info header_info_ht, void* value){

	/* File is already open */

	int bucket1;
	int blocks_counter = 0;		// how many blocks we have read untill we found our record

	int fd = header_info_sht.fileDesc;

	char* value1 = (char*) value;

	int records_found = 0;

	if( strcmp( header_info_sht.attrName, "name" ) == 0){		// name

		bucket1 = HashS(value1 , header_info_sht.numBuckets);

		int blockid = bucket1;

		/* Start reading blocks from bucket1 */

		void* block = malloc( BLOCK_SIZE) ;

		Block1* block1 = malloc( sizeof(Block1) );

		if( BF_ReadBlock( fd, blockid, &block) < 0){					// now block points to where the "block data" is stored

			BF_PrintError("BF_ReadBlock error in SHT_SecondaryGetAllEntries() (1) : ");
			return -1;

		}

		memcpy( block1, block, sizeof(Block1) );			// filling our Block structure

		blocks_counter++;				// how many blocks we have read

		int i;

		/* Search first block */

		printf("\n");

		for( i = 0; i < block1->counter; i++){

			if( strcmp( block1->records[i].record.name, value1 ) == 0){

				printf("SecondaryRecord with id: %d, (key) name: %s, surname: %s, address: %s \n", block1->records[i].record.id, block1->records[i].record.name, block1->records[i].record.surname, block1->records[i].record.address);

				records_found++;

			}
		}

		/* Search the rest of the "list" of blocks */

		while( block1->next != 0){		// Move to the  block1.next block , untill we find empty space

			blockid = block1->next;

			if( BF_ReadBlock( fd, block1->next, &block) < 0){					// now block points to where the "block data" is stored

				BF_PrintError("BF_ReadBlock error in SHT_SecondaryGetAllEntries() (2) : ");
				return -1;

			}

			memcpy( block1, block, sizeof(Block1) );			// filling our Block structure

			blocks_counter++;				// how many blocks we have read


			for( i = 0; i < block1->counter; i++){

				if( strcmp( block1->records[i].record.name , value1 ) == 0){

					printf("SecondaryRecord with id: %d, (key) name: %s, surname: %s, address: %s \n", block1->records[i].record.id, block1->records[i].record.name, block1->records[i].record.surname, block1->records[i].record.address);

					records_found++;
				}
			}



		}

		free(block1);

		if( records_found == 0){	// didn't find anything return -1

			printf(" Didn't find an entry with name: %s \n\n", value1);
			return -1;

		}

		return blocks_counter;


	}else if( strcmp( header_info_sht.attrName, "surname") == 0){

		bucket1 = HashS(value1 , header_info_sht.numBuckets);

		int blockid = bucket1;

		/* Start reading blocks from bucket1 */

		void* block = malloc( BLOCK_SIZE) ;

		Block1* block1 = malloc( sizeof(Block1) );

		if( BF_ReadBlock( fd, blockid, &block) < 0){// now block points to where the "block data" is stored

			BF_PrintError("BF_ReadBlock error in SHT_SecondaryGetAllEntries() (3) : ");
			return -1;

		}

		memcpy( block1, block, sizeof(Block) );			// filling our Block structure

		blocks_counter++;				// how many blocks we have read


		int i;

		/* Search first block */

		for( i = 0; i < block1->counter; i++){

			if( strcmp( block1->records[i].record.surname , value1 ) == 0){

				printf("SecondaryRecord with id: %d, name: %s, (key) surname: %s, address: %s \n", block1->records[i].record.id, block1->records[i].record.name, block1->records[i].record.surname, block1->records[i].record.address);

				records_found++;


			}

		}


		/* Search the rest of the "list" of blocks */

		while( block1->next != 0){// Move to the  block1.next block , untill we find empty space

			blockid = block1->next;

			if( BF_ReadBlock( fd, block1->next, &block) < 0){// now block points to where the "block data" is stored

				BF_PrintError("BF_ReadBlock error in HT_GetAllEntries() (4) : ");
				return -1;

			}

			memcpy( block1, block, sizeof(Block1) );			// filling our Block structure

			blocks_counter++;				// how many blocks we have read


			for( i = 0; i < block1->counter; i++){

				if( strcmp( block1->records[i].record.surname , value1 ) == 0){

					printf("Record with id: %d, name: %s, (key) surname: %s, address: %s\n",block1->records[i].record.id, block1->records[i].record.name,block1->records[i].record.surname, block1->records[i].record.address);

					records_found++;
				}
			}

		}

		free( block1);

		if( records_found == 0){	// didn't find anything return -1

			printf(" Didn't find an entry with surname: %s \n\n", value1);
			return -1;

		}


		return blocks_counter;

	}else{	// address

		bucket1 = HashS(value1 , header_info_sht.numBuckets);

		int blockid = bucket1;

		/* Start reading blocks from bucket1 */

		void* block = malloc( BLOCK_SIZE) ;

		Block1* block1 = malloc( sizeof(Block1) );

		if( BF_ReadBlock( fd, blockid, &block) < 0){// now block points to where the "block data" is stored

			BF_PrintError("BF_ReadBlock error in HT_GetAllEntries() (5) : ");
			return -1;

		}

		memcpy( block1, block, sizeof(Block1) );			// filling our Block structure

		blocks_counter++;				// how many blocks we have read

		int i;
		/* Search first block */

		for( i = 0; i < block1->counter; i++){

			if( strcmp( block1->records[i].record.address , value1 ) == 0) {

				printf("SecondaryRecord with id: %d, name: %s, surname: %s, (key) address: %s \n", block1->records[i].record.id, block1->records[i].record.name, block1->records[i].record.surname, block1->records[i].record.address);

				records_found++;

			}
		}

		/* Search the rest of the "list" of blocks */

		while( block1->next != 0){		// Move to the  block1.next block , untill we find empty space

			blockid = block1->next;

			if( BF_ReadBlock( fd, block1->next, &block) < 0){					// now block points to where the "block data" is stored

				BF_PrintError("BF_ReadBlock error in SHT_SecondaryGetAllEntries() (6) : ");
				return -1;

			}

			memcpy( block1, block, sizeof(Block1) );			// filling our Block structure

			blocks_counter++;				// how many blocks we have read


			for( i = 0; i < block1->counter; i++){

				if( strcmp( block1->records[i].record.address , value1 ) == 0){

					printf("Record with id: %d, name: %s, (key) surname: %s, address: %s\n",block1->records[i].record.id, block1->records[i].record.name, block1->records[i].record.surname, block1->records[i].record.address);

					records_found++;

				}

			}

		}

		free(block1);

		if( records_found == 0){	// didn't find anything return -1

			printf(" Didn't find an entry with address: %s \n\n", value1);
			return -1;

		}


		return blocks_counter;


	}

}



//////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////



int SHT_SecondaryInsertEntry(SHT_info header_info, SecondaryRecord record){


	/* File is already open */

	int fd = header_info.fileDesc;

	int bucket1;

	if( strcmp( header_info.attrName, "name") == 0){

		bucket1 = HashS(record.record.name , header_info.numBuckets);

	}else if( strcmp( header_info.attrName, "surname") == 0){

		bucket1 = HashS(record.record.surname , header_info.numBuckets);

	}else{	// address

		bucket1 = HashS(record.record.address , header_info.numBuckets);

	}
	int blockid = bucket1;

	/* Start reading blocks from bucket1 */

	void* block = malloc( BLOCK_SIZE) ;

	Block1* block1 = malloc( sizeof(Block1) );

	if( BF_ReadBlock( fd, blockid, &block) < 0){					// now block points to where the "block data" is stored

		BF_PrintError("BF_ReadBlock error in SHT_SecondaryInsertEntry() (1) : ");
		return -1;

	}

	memcpy( block1, block, sizeof(Block1) );			// filling our Block structure


	int temp = 0;

	while( block1->counter == 6){	// Move to the  block1.next block , untill we find empty space


		if( block1->next == 0){		// there is no next Block


			if( BF_AllocateBlock(header_info.fileDesc) < 0 ){

				BF_PrintError("BF_AllocateBlock error in SHT_SecondaryInsertEntry() : ");
				return -1;
			}



			if( ( temp = BF_GetBlockCounter( header_info.fileDesc) ) < 0 ){

				BF_PrintError("BF_GetBlockCounter error in SHT_SecondaryInsertEntry() : ");
				return -1;
			}

			block1->next = temp - 1;		// newly added block(added in the end) has blockid BF_GetBlockCounter -1

			memcpy( block, block1, sizeof(Block1) );			// Writing back the modified block

			if( BF_WriteBlock( fd, blockid) < 0){

				BF_PrintError("BF_WriteBlock error in SHT_SecondaryInsertEntry() (1) : ");
				return -1;

			}

		}

		/* Now read the next block */

		blockid = block1->next;

		if( BF_ReadBlock( fd, block1->next, &block) < 0){					// now block points to where the "block data" is stored

			BF_PrintError("BF_ReadBlock error in SHT_SecondaryInsertEntry() (2) : ");
			return -1;

		}


		memcpy( block1, block, sizeof(Block1) );			// filling our Block structure (with next Block)

	}

	// We found empty space in the block with blockId = blockid

	/* Add the record at the end */


	block1->records[block1->counter].record.id = record.record.id;

	strcpy( block1->records[block1->counter].record.name , record.record.name);
	strcpy( block1->records[block1->counter].record.surname , record.record.surname);
	strcpy( block1->records[block1->counter].record.address , record.record.address);

	block1->counter++;			// increment the counter

	memcpy( block, block1, sizeof(Block1) );			// filling our Block structure (with next Block)

	if( BF_WriteBlock( fd, blockid) < 0){

		BF_PrintError("BF_WriteBlock error in SHT_SecondaryInsertEntry() (2) : ");
		return -1;

	}

	free(block1);


	return blockid;

}





//////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////


SHT_info* SHT_OpenSecondaryIndex(char* sfilename){

	int fd;

	if( ( fd = BF_OpenFile(sfilename) ) < 0){		// opening file to use

		BF_PrintError("BF_Open error in SHT_OpenSecondaryIndex() : ");
		return NULL;

	}


	void* block = malloc( BLOCK_SIZE) ;

	if( BF_ReadBlock( fd, 0, &block) < 0){					// now block points to where the "block data" is stored

		BF_PrintError("BF_ReadBlock error in SHT_OpenSecondaryIndex() : ");
		return NULL;

	}

	Block0* block1=malloc(sizeof(Block0));

	if( block1 == NULL){

		perror(" Malloc error in SHT_OpenSecondaryIndex() : ");
		return NULL;

	}


	memcpy(block1,block,sizeof(Block0) );			// transfering the bytes of block to HT_info




	if( strcmp(block1->type,"SHT") != 0){		// this isn't a HT file

		printf("This isn't a SHT file\n\n");

		free(block1);

		return NULL;

	}



	block1->fd = fd;// fix this since each time we "Open" there is a different descriptor ===> fileDescriptor might not even be needed to save in block (based on eclass)

	/* Update the file descriptor in block 0 */

	memcpy(block, block1, sizeof(Block0) );

	if( BF_WriteBlock(fd,0) < 0){

		BF_PrintError("BF_WriteBlock error in SHT_OpenSecondaryIndex() : ");
		return NULL;
	}

	/* Create SHT_info */


	SHT_info* SHT_Info = malloc( sizeof(SHT_info) );

	SHT_Info->fileDesc = block1->fd;

	SHT_Info->attrLength = block1->attrLength;
	SHT_Info->attrName = malloc( block1->attrLength);

	strcpy( SHT_Info->attrName , block1->attrName);

	SHT_Info->numBuckets = block1->numBuckets;

	SHT_Info->filename = malloc( strlen( block1->filename) + 1);
	strcpy( SHT_Info->filename, block1->filename);


	// we don't close the file
	free(block1);

	return SHT_Info;
}

//////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////



int HashStatistics(char* filename){

	/* getting the necessary header_info */

	HT_info* header_info;
	SHT_info* header_info1;

	if( ( header_info = HT_OpenIndex(filename) ) == NULL){

		header_info1 = SHT_OpenSecondaryIndex(filename);

		if( header_info1 == NULL){

			printf("Error in HashStatistics, not a hash file \n\n");
			return -1;			// its not a hash file

		}


		/* Check how many Blocks (A) */

		int total_blocks;

		if( ( total_blocks = BF_GetBlockCounter(header_info1->fileDesc) ) < 0){ 

			BF_PrintError("BF_GetBlockCounter error in HashStatistics() (1) : ");
			return -1;
		}

		printf(" Total_blocks of file: %s is %d\n\n", filename, total_blocks);


		/* Check Minimum, Maximum, Average amount of records for each bucket (B) */

		/* Also, find average amount of blocks (C) , as well as
			 how many buckets have overflow blocks and how many overflow blocks (D)  */

		int min, max;
		int i; 

		int bucket_counter = 0,block_counter = 0, counter = 0;

		int block_counter1 = 0;		// same as block_counter , but it resets after each bucket


		float average;


		void* block = malloc( BLOCK_SIZE) ;

		Block1* block1 = malloc( sizeof(Block1) );

		for( i = 1; i < header_info1->numBuckets + 1; i++){

			/* For each buckets, search the list of buckets for min max average */

			if( BF_ReadBlock( header_info1->fileDesc, i, &block) < 0){

				BF_PrintError("BF_ReadBlock error in HashStatistics() (1) : ");
				return -1;

			}

			memcpy(block1,block,sizeof(Block1) );		// transfer contents to block1


			block_counter++;

			min = block1->counter;
			max = block1->counter;

			counter+= block1->counter;

			while( block1->next != 0){

				if( BF_ReadBlock( header_info1->fileDesc, block1->next, &block) < 0){

					BF_PrintError("BF_ReadBlock error in HashStatistics() (2) : ");
					return -1;

				}

				memcpy(block1,block,sizeof(Block1) );		// transfer contents to block1

				block_counter++;
				block_counter1++;

				if( block1->counter < min) min = block1->counter;

				if( block1->counter > max) max = block1->counter;

				counter+= block1->counter;

			}

			//visited all the blocks => going to next bucket

			if( block_counter1 != 0){		// this bucket has overflow blocks

				bucket_counter++;

				printf("Bucket: %d, has %d overflow blocks\n", i, block_counter1);
			}

			block_counter1 = 0;		//resetting counter


		}

		printf("So, we have %d buckets that have overflow blocks\n\n", bucket_counter);

		//visited all the buckets ===> printing the statistics.

		average = counter/header_info1->numBuckets;

		printf("RECORDS, Min: %d, Max:%d, Average: %f \n", min,max,average);

		average = block_counter/header_info1->numBuckets;

		printf("BLOCKS, Average: %f \n\n", average);

		/* Closing the index */

		if( SHT_CloseSecondaryIndex(header_info1) == -1){

			printf("\n\nError closing Index in HashStatistics() (1) \n");
			return -1;

		}

		free(block1);

		return 0;


	}



	/* HT_info */


	/* Check how many Blocks (A) */

	int total_blocks;

	if( ( total_blocks = BF_GetBlockCounter(header_info->fileDesc) ) < 0){ 

		BF_PrintError("BF_GetBlockCounter error in HashStatistics() (2) : ");
		return -1;
	}

	printf(" Total_blocks of file: %s is %d\n\n", filename, total_blocks);


	/* Check Minimum, Maximum, Average amount of records for each bucket (B) */

	/* Also, find average amount of blocks (C) , as well as
		 how many buckets have overflow blocks and how many overflow blocks (D)  */

	int min, max;
	int i; 

	int bucket_counter = 0,block_counter = 0, counter = 0;

	int block_counter1 = 0;		// same as block_counter , but it resets after each bucket

	float average;

	void* block = malloc( BLOCK_SIZE) ;

	Block* block1 = malloc( sizeof(Block) );

	for( i = 1; i < header_info->numBuckets + 1; i++){

		/* For each buckets, search the list of buckets for min max average */

		if( BF_ReadBlock( header_info->fileDesc, i, &block) < 0){

			BF_PrintError("BF_ReadBlock error in HashStatistics() (3) : ");
			return -1;

		}

		memcpy(block1,block,sizeof(Block) );		// transfer contents to block1

		block_counter++;

		min = block1->counter;
		max = block1->counter;

		counter+= block1->counter;

		while( block1->next != 0){

			if( BF_ReadBlock( header_info->fileDesc, block1->next, &block) < 0){

				BF_PrintError("BF_ReadBlock error in HashStatistics() (2) : ");
				return -1;

			}

			memcpy(block1,block,sizeof(Block) );		// transfer contents to block1

			block_counter++;
			block_counter1++;

			if( block1->counter < min) min = block1->counter;

			if( block1->counter > max) max = block1->counter;

			counter+= block1->counter;

		}

		//visited all the blocks => going to next bucket

		if( block_counter1 != 0){		// this bucket has overflow blocks

			bucket_counter++;

			printf("Bucket: %d, has %d overflow blocks\n", i, block_counter1);
		}

		block_counter1 = 0;		//resetting counter


	}

	printf("So, we have %d buckets that have overflow blocks\n\n", bucket_counter);

	//visited all the buckets ===> printing the statistics.

	average = counter/header_info->numBuckets;

	printf("RECORDS, Min: %d, Max:%d, Average: %f \n", min,max,average);

	average = block_counter/header_info->numBuckets;

	printf("BLOCKS, Average: %f \n\n", average);

	/* Closing the index */

	if( HT_CloseIndex(header_info) == -1){

		printf("\n\nError closing Index in HashStatistics() (2) \n");
		return -1;

	}

	free(block1);


	return 0;

}


