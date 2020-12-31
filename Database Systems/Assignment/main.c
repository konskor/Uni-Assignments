#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "BF.h"
#include "HT.h"






int main(int argc, char* argv[]){

	if( argc < 2){

		printf("./main <number of records>,<text file> \n");
		return -1;

	}

	BF_Init();


	int total_records = atoi(argv[1]);

	char attrType = 'i';

	int attrLength = strlen("id") + 1;
	char* attrName = malloc( attrLength);

	strcpy( attrName, "id");

	int buckets = 10;

/* Create the primary HT_Index */

	if( HT_CreateIndex( "primary-index", attrType, attrName, attrLength, buckets) == -1){

		printf("HT_CreateIndex error in main \n");

		return -1;
	}

/* Open the primary HT_Index */

	HT_info* hi;

	if( (hi = HT_OpenIndex("primary-index") ) == NULL){

		printf("HT_OpenIndex error in main \n");

		return -1;
	}

/* Insert entries from given files */

	FILE* file;

	file = fopen(argv[2],"r");

	if( file == NULL){

		printf("Error opening file\n");
		return -1;

	}

	Record record;

	int i = 0;

	char buf[1024];

	int length;




	for( i = 0; i < total_records/2; i++){

		if( fgets(buf,sizeof(buf),file) == NULL){

			printf(" fgets error in main (reached EOF ? ? ) (1) \n");
			return -1;
		}

		buf[strlen(buf) -1] = '\0';		// eat the new line

		// id
		char* sid = strtok(buf,",");
		sid = sid + 1;				// ignore the first {
		int id = atoi(sid);


		// name
		char* name = strtok(NULL,", ");

		length = snprintf(NULL,0,"%s",name) + 1;
		char name1[length];
		snprintf(name1,length,"%s",name);

		name1[strlen(name)-1] = '\0';			// erase the last "
		char* name2 = name1 + 1;		// ignore the first "


		// surname
		char* surname = strtok(NULL,", ");

		length = snprintf(NULL,0,"%s",surname) + 1;
		char surname1[length];
		snprintf(surname1,length,"%s",surname);

		surname1[strlen(surname) -1] = '\0';			// erase the last "
		char* surname2 = surname1 + 1;		// ignore the first "


		// address
		char* address = strtok(NULL,",}");

		length = snprintf(NULL,0,"%s",address) + 1;
		char address1[length];
		snprintf(address1,length,"%s",address);

		address1[strlen(address) -1] = '\0';			// erase the last "
		char* address2 = address1 + 1;		// ignore the first "


		// putting the values in record
		record.id = id;
		strcpy( record.name,name2);
		strcpy( record.surname, surname2);
		strcpy( record.address, address2);



		if( HT_InsertEntry(*hi,record) == -1){

			printf("HT_InsertEntry error in main (1) \n");
			return -1;

		}
		name2 = NULL;
		surname2 = NULL;
		address2 = NULL;
	}



/* Search for entries, based on some criteria */


	for( i = 0; i < total_records/2; i++){

		if( i % 2 == 0){		//print even entries

			if( HT_GetAllEntries( *hi, (void*)&i ) == -1){

				printf("HT_GetAllEntries erron in main\n");
			}

		}
	}









/* Create SHT_Index */

	attrLength = strlen( "name") + 1;

	free(attrName);

	attrName = malloc( attrLength);
	strcpy( attrName, "name");

	buckets = 10;


	if(SHT_CreateSecondaryIndex( "secondary-index", attrName, attrLength, buckets,"primary-index") == -1){

		printf("SHT_CreateSecondaryIndex error in main \n");

		return -1;
	}


/* Open the secondary HT_Index */

	SHT_info* shi;

	if( (shi = SHT_OpenSecondaryIndex("secondary-index") ) == NULL){

		printf("SHT_OpenSecondaryIndex error in main \n");

		return -1;
	}





/* Insert entries from given files */

//	Record record;

	SecondaryRecord record1;

	int blockid;

	for( i = total_records/2; i < total_records; i++){

		if( fgets(buf,sizeof(buf),file) == NULL){

			printf(" fgets error in main (reached EOF ? ? ) (2) \n");
			return -1;
		}
		buf[strlen(buf) -1] = '\0';		// eat the new line

		// id
		char* sid = strtok(buf,",");
		sid = sid + 1;				// ignore the first {
		int id = atoi(sid);


		// name
		char* name = strtok(NULL,", ");

		length = snprintf(NULL,0,"%s",name) + 1;
		char name1[length];
		snprintf(name1,length,"%s",name);

		name1[strlen(name) -1 ] = '\0';			// erase the last "
		char* name2 = name1 + 1;		// ignore the first "








		// surname
		char* surname = strtok(NULL,", ");

		length = snprintf(NULL,0,"%s",surname) + 1;
		char surname1[length];
		snprintf(surname1,length,"%s",surname);


		surname1[strlen(surname) - 1] = '\0';			// erase the last "
		char* surname2 = surname1 + 1;		// ignore the first "





		// address
		char* address = strtok(NULL,",}");

		length = snprintf(NULL,0,"%s",address) + 1;
		char address1[length];
		snprintf(address1,length,"%s",address);

		address1[strlen(address) -1] = '\0';			// erase the last "
		char* address2 = address1 + 1;

		// putting the values in record
		record.id = id;
		strcpy( record.name,name2);
		strcpy( record.surname, surname2);
		strcpy( record.address, address2);



		if( (blockid = HT_InsertEntry(*hi,record) ) == -1){

			printf("HT_InsertEntry error in main (2) \n");
			return -1;

		}

		// putting the values in secondary record 

		record1.record = record;
		record1.blockId = blockid;



		if( SHT_SecondaryInsertEntry(*shi,record1)  == -1){

			printf("SHT_InsertSecondaryEntry error in main \n");
			return -1;

		}

	}


	fclose(file);		// finished with the file



	/* Search for entries in SHT */


	char *value1;
	length = 0;

	for( i = 0; i < total_records; i++){

		if( i % 2 == 1){		//print odd entries

			length = snprintf(NULL,0,"name_%d", i) + 1;
			value1 = malloc( length);

			snprintf(value1,length,"name_%d", i);

			if( SHT_SecondaryGetAllEntries( *shi,*hi, (void*)value1) == -1){

				printf("SHT_GetAllSecondaryEntries error in main\n");
			}

			free(value1);

		}

	}




/* Close the Primary HT_Index */


	if( HT_CloseIndex( hi) == -1){

		printf("HT_CloseIndex error in main \n");

		return -1;
	}



/* Close the Secondary HT_Index */


	if( SHT_CloseSecondaryIndex( shi) == -1){

		printf("SHT_CloseSecondaryIndex error in main \n");

		return -1;
	}


	/* Hash statistics */

	HashStatistics("primary-index");
	HashStatistics("secondary-index");

	free( attrName);

	return 0;
}





