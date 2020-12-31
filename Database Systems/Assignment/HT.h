#include <stdio.h>

/* Records */

typedef struct{

int id;

char name[15];
char surname[20];
char address[40];

}Record;			// sizeof(Record) = 4 + 15 +20 + 40 = 79


typedef struct{

Record record;

int blockId;

}SecondaryRecord;






/* Blocks */

// maybe an extra BLock0 struct fot the first block

typedef struct{		// test

	char type[4];			//type of file

	int fd;

	char attrType;

//	char attrName[50];				//we have 495 bytes available without attrName, fileName

	int attrLength;

	int numBuckets;

	char attrName[20];			// please don't move these two , might have a problem

	char filename[20];			// previously 50



}Block0;




typedef struct{

//int id;					// block_id


int counter;		// Record_counter

Record records[6];			// max_records = 512 - 2 * 4 = 504 / 79 = 6

int next;		// file descriptor for next Block

}Block;


//Block for secondary record



typedef struct{

//int id;					// block_id


int counter;		// Record_counter

SecondaryRecord records[6];			// max_records = 512 - 2 * 4 = 504 / (79 + 4) = 6

int next;		// file descriptor for next Block

}Block1;


/* Hash Tables info */

typedef struct{

int fileDesc;

char attrType;

char* attrName;
//char attrName[8];

int attrLength;


long int numBuckets;

}HT_info;


typedef struct{

int fileDesc;

char* attrName;
//char attrName[8];

int attrLength;

long int numBuckets;

char* filename;				// name of  primary index file ( hash(id) )

}SHT_info;



/* Hash Functions */

int HashInt(int x,int M);		// M ==> buckets (size of hash table)

int HashS(char* string,int M);


/* Primary Hash Table Functions */

int HT_CreateIndex(char* fileName,char attrType,char* attrName,int attrLength,int buckets);


HT_info* HT_OpenIndex(char* fileName);

int HT_CloseIndex(HT_info* header_info);



int HT_InsertEntry(HT_info header_info, Record record);

int HT_DeleteEntry(HT_info header_info,void* value);



int HT_GetAllEntries( HT_info header_info, void *value);


/* Secondary Hash Table Functions */

int SHT_CreateSecondaryIndex(char* sfileName,char* attrName,int attrLength,int buckets,char* fileName);



SHT_info* SHT_OpenSecondaryIndex(char* sfilename);

int SHT_CloseSecondaryIndex(SHT_info* header_info);


int SHT_SecondaryInsertEntry( SHT_info header_info, SecondaryRecord record);


int SHT_SecondaryGetAllEntries(SHT_info header_info_sht, HT_info header_info_ht, void* value);


/* Statistics */

int HashStatistics(char* filename);





























