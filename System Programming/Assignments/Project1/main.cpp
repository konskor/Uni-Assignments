#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <time.h>


#include "header.hpp"











int main(int argc, char* argv[] ){


	if( argc != 13){
	
		printf("Not enough arguments (argc = 13)  \n");
		return -1;
	}
	
	/* Reading stdin */

	int i;

	char* bFile;
	char* tFile;
	
	int bValue;
	
	int senderHTnum;
	int receiverHTnum;
	
	int bucketSize;
	
	
	//int length;


	for( i = 0; i < argc; i++){

		if( strcmp(argv[i] , "-a") == 0 ){		// bitcoinBalanceFile
		
			if( i == argc-1){
				printf(" Something is wrong: -a ..... \n");
				return -1;		
			}
		
			int length;
		
			length = strlen( argv[i+1] ) + 1; 
		
			bFile = new char[length];
			
			strcpy(bFile, argv[i+1] );
			
			continue;
		}

		if( strcmp(argv[i] , "-t") == 0 ){		// transactionsFile
		
			if( i == argc-1){
				printf(" Something is wrong: -t ..... \n");
				return -1;		
			}
		
			int length;

			length = strlen( argv[i+1] ) + 1; 
		
			tFile = new char[length];
			
			strcpy(tFile, argv[i+1] );
			
			continue;


		}
		
		if( strcmp(argv[i] , "-v") == 0 ){		// bitcoinValue
		
			if( i == argc-1){
				printf(" Something is wrong: -v ..... \n");
				return -1;		
			}
		
			bValue = atoi( argv[i+1] );
			
			continue;		
		

		}
		
		if( strcmp(argv[i] , "-h1") == 0 ){		// sender Hashtable NumOfEntries
		
			if( i == argc-1){
				printf(" Something is wrong: -h1 ..... \n");
				return -1;		
			}
		
			senderHTnum = atoi( argv[i+1] );
			
			continue;
		
		

		}		


		if( strcmp(argv[i] , "-h2") == 0 ){		// receiver Hashtable NumOfEntries
		
			if( i == argc-1){
				printf(" Something is wrong: -h2 ..... \n");
				return -1;		
			}
		
			receiverHTnum = atoi( argv[i+1] );
			
			continue;
		}
		
		

		if( strcmp(argv[i] , "-b") == 0 ){		// bucketSize
		
			if( i == argc-1){
				printf(" Something is wrong: -b ..... \n");
				return -1;		
			}
		
			bucketSize = atoi( argv[i+1] );
			
			continue;
		}

	}

	/* Initiating some structs */
	
	HashTable* senderHT = new HashTable( senderHTnum, bucketSize);		// sender HashTable
	HashTable* receiverHT = new HashTable( receiverHTnum, bucketSize);	// receiver HashTable

	List<Wallet>* UserList = new List<Wallet>();	// User/Wallet list
	
	List<TLNode>* TransList = new List<TLNode>();	// Transaction list (all transactions)
	
	List<RLNode>* RootList = new List<RLNode>();	// List of pointers to bitcoin trees 
	
	// Extra struct for validation
	
	HashTable1* TransactionHT = new HashTable1( senderHTnum, bucketSize);










	printf(" Start reading bitCoinBalanceFile \n");
	
	

	/* Opening bitCoinBalanceFile */
	
	FILE* fp;
	
	fp = fopen(bFile,"r");
	if( fp == NULL){
	
		perror("Opening BitcoinFile: ");
		return -1;
	
	}
	
	/* Reading line by line */
	
	char *line = NULL;
	size_t length = 0;
	ssize_t read;
	
	char* token;
	//char name[50];		// IMPORTANT
	
	int assignments = 0;
	
	
	
	while( (read = getline(&line,&length, fp) ) != -1){
	
		printf("Line is %s \n", line);
		
		token = strtok(line," ,\n");	// the first token is the UserID/WalletID
		//strcpy(name,token);
		
		if( token == NULL){	// no input given or first char is space_char
		
			printf("We need at least the name in BitcoinBalanceFile \n");
		
			printf(" Input given : (space_char).... or NULL  ==> ERROR\n");
		
			delete senderHT;
			delete receiverHT;
			
			delete UserList;
			delete TransList;
			delete RootList;
			
			delete TransactionHT;
			
			return -1;	
		}
		
		
		if( UserExists(UserList,token) == true){	// user exists => exit program
		
			printf("User %s already exists ==> exiting \n", token);
		
			delete senderHT;
			delete receiverHT;
			
			delete UserList;
			delete TransList;
			delete RootList;
			
			delete TransactionHT;
			
			return -1;		
		}
		
		//else , user doesn't exist
		
		printf("Creating new User %s \n",token);
		
		Wallet* user = new Wallet(token,bValue);	// create an empty Wallet
		
		UserList->AddNode(user);	//add user/wallet to the list
		
		//printf(" Head of list is %s \n", UserList->head->data->WalletID);
					
		while( ( token = strtok(NULL," ,\n") ) != NULL ){	// bitcoin tokens
			
			if( BitcoinExists(RootList,token) == true){	// bitcoin exists => exit program
		
				printf("Bitcoin %s ( user %s has it) already exists ==> exiting \n", token,user->WalletID);
		
				delete senderHT;
				delete receiverHT;
			
				delete UserList;
				delete TransList;
				delete RootList;
				
				delete TransactionHT;
			
				return -1;
			}	
			//else, bitcoin doesn't exist
		
			/* Create TNode => Tree Node*/
			TNode* node = new TNode(token,bValue);
		
			/* Create PTNode = > (Node) Pointer to Tree Node */
			
			PTNode* node1 = new PTNode(node);
			
		
			/* Create RLNode => Root List Node */
			
			RLNode* node2 = new RLNode(token,node);

			RootList->AddNode(node2);	// add node2 to the list
		
			/* Create UserBNode => User Bitcoins Node */
			UserBNode* node3 = new UserBNode(token,bValue,node1);
			
			user->bitcoins->AddNode(node3);
			
			// update wallet
			user->numOfBitcoins++;

			user->amount = user->amount + bValue;

			/* NULL the variables */
			node = NULL;
			node1 = NULL;
			node2 = NULL;
			node3 = NULL;
			
			
		}	// all Bitcoins inserted 
		
		user = NULL;	// don't call destructor		
	
	}//reached EOF
	
	
	printf("\n\nFINISHED READING BITCOINFILE \n\n\n");
	
	/* Closing bitcoinBalanceFile */
	
	if( fclose(fp) == -1){
	
		perror("Closing BitcoinFile: ");
		return -1; 
	}














	printf("\n\nStart reading TransactionFile \n\n");



	/* Opening TransactionsFile */
	
	fp = fopen(tFile,"r");
	if( fp == NULL){
	
		perror("Opening Transactions File: ");
		return -1;
	
	}
	
	/* Reading line by line */
	
	line = NULL;
	length = 0;
	
	token = NULL;

	char transactionId[50];
	char user1[50];		// IMPORTANT
	char user2[50];

	int amount = 0;	// transaction amount
	
	char date[11];
	char tim[6];


	assignments = 0;	// how many successful assignments after sscanf()
	
	int maxID = 0;	// will be used later , to create new transactionID
	
	time_t last_trans = 0;	//timestamp for last transaction

	
	
	while( (read = getline(&line,&length, fp) ) != -1){
	
		printf(" Line is: %s \n", line);
	
		assignments = sscanf(line,"%s %s %s %d %s %s ", transactionId, user1, user2, &amount, date, tim);
	
		/* Check if we have enough or more than enough arguments */
//		if( assignments < 4 ){	// 4 <= items to be filled <= 6
		
		if( assignments != 6 ){	// 4 <= items to be filled <= 6
		
			printf("Error in TransactionFile: transactionId User1 User2 value date time \n");
		
			delete senderHT;
			delete receiverHT;
			
			delete UserList;
			delete TransList;
			delete RootList;
			
			delete TransactionHT;
			
			return -1;		
		} 
		
		/* Check if transactionId already exists (hashTable) */
	
		char* result;
		
		TransactionHT->SearchRecord1(transactionId, &result );
		
		if( result != NULL){	// transactionID exists
		
			printf("Error in TransactionFile: transactionId already exists !!!! \n\n\n");
			continue;
		}
		
		// transactionID doesn't exist (add to the TransactionHT ) 
		
		printf("\nTransactionID %s doesn't exist ===> OK \n\n", transactionId); 		
	
		if( isNumber(transactionId) == true){	// transactionId is a number
	
			if( maxID < atoi(transactionId) ) maxID = atoi(transactionId);
		}
	
		TransactionHT->AddRecord1(transactionId); // add transactionId to HT1
	
	
	
		/* Check if user1,user2 exist  */
		
		
			/* Check User1 */
	
		if( UserExists(UserList,user1) == false){	// user doesn't exist => exit program
		
			printf("User %s  doesn't exist ==> continue to next transaction \n", user1);
			continue;
		}
		
		printf("\nFound senderUser \n\n");
		
			/* Find Node of user1 */
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
			
			
			delete senderHT;
			delete receiverHT;
			
			delete UserList;
			delete TransList;
			delete RootList;
			
			delete TransactionHT;
			
			return -1;	
		
		}
			/* Check if user1 = user2 */
		
		if( strcmp(user1,user2) == 0){	//check if same user
		
			printf(" Same User %s in this transaction => continue to next transaction \n", user1);
			continue;
		}
		
		
		
		
		
			/* Check user2 */
	
		if( UserExists(UserList,user2) == false){	// user doesn't exist => exit program
		
			printf("User %s  doesn't exist ==> continue to next transaction \n", user2);
			continue;
		}
	
			/* Find Node of user2 */
		Node<Wallet>* node1 = UserList->head;
		
		while( node1 != NULL){
		
			if( strcmp( node1->data->WalletID, user2) == 0 ){
			
				printf(" Found our Node , breaking out of loop (2) \n");
				
				break;			
			}
			
			node1 = node1->next;		
		}
		
		if( node1 == NULL){	// if  our node doesn't exist ===> ERROR
		
			printf(" ERROR , SOMETHING IS WRONG HERE !!!! (2)");
			
			
			delete senderHT;
			delete receiverHT;
			
			delete UserList;
			delete TransList;
			delete RootList;
			
			delete TransactionHT;
			
			return -1;	
		
		}
				
		/* Check if user1 has enough money, check the balance */
	
	
		if( amount <= 0){	// check if the transaction amount is invalid
		
			printf("The transaction amount is invalid ( <= 0) ===> continue to next transaction \n");
			continue;		
		}
	
	
		if( node->data->amount < amount){	// user1 can't pay
		
			printf("User1 can't complete this transaction , not enough $$$ \n");
			continue;		
		}
	
		// user1 has enough $$$ ====> OK
		
		
		printf("\n\nPassed the transaction checks ====> creating transaction \n\n");
		
		/* Now we create the transaction */
		
		Transaction* transaction;
		
		time_t timestamp = 0;
				
		
		// INCOMPLETE CHECK IF assignments == 4 ( and check if record exists => update)
		
		if( assignments == 6){ //have already done this check above
		
		
			/* Creating the timestamp */
			length = snprintf(NULL,0,"%s %s", date, tim ) + 1;

			char tim1[length];
	
			snprintf(tim1,length,"%s %s", date,tim) + 1;
	

			struct tm vtm = {0};
	
			if( strptime(tim1, "%d-%m-%Y %H:%M",&vtm) != NULL){
	
				timestamp =	mktime(&vtm);	
			
			}else{ 
		
				printf(" Error in strptime() (1) \n\n"); 
			
			
				delete senderHT;
				delete receiverHT;
			
				delete UserList;
				delete TransList;
				delete RootList;
			
				delete TransactionHT;
			
				return -1;
			}
		
		
			last_trans = timestamp;	//updating timestamp of last transaction
		
		
			/* Creating Transaction (and all associated nodes ) */
		
			transaction = new Transaction(transactionId, user1, user2, amount, timestamp);
			
			TLNode* tlnode = new TLNode( transaction);
			
			UserTLNode* Utlnode = new UserTLNode( tlnode);
			
			/////////////////////////////////////////////////
						
			TransList->AddNode(tlnode);	//add the tlnode to the general transaction list
			
			
			/* Check if user1,user2 exist in the HashTables 
			
				If yes, update their contents */ 
			
			struct Records* record = new struct Records();
			
			senderHT->SearchRecord(user1,&record);
			
			if( record == NULL){	// a record of user1 doesn't exist
			
				record = new struct Records();	//create it again (previous one was deleted )
			
				record->wallet = node->data;		
			
				record->list = new List<UserTLNode>();
			
				record->list->AddNode(Utlnode);
				
				// Add the record to HashTable
				senderHT->AddRecord(record);
				
				//we don't need record anymore
				
				record->wallet = NULL;
				record->list = NULL;
				
				delete record;				
				record = NULL;
				
			}else{	//a record of user1 exists
			
				record->list->AddNode(Utlnode);
				
				//Update/overwrite record in HashTable (don't need to)
				//senderHT->UpdateRecord(record);
				
				
				//we don't need record anymore
				
				record->wallet = NULL;
				record->list = NULL;
				
				delete record;				
				record = NULL;							
			}
			
			////////////////////////////////////			
			
			/* Now for user2 */
			
			printf("ReceiverHT \n");
			
			printf("user2 is %s , user1 is %s \n", user2, user1);
			
			record = new struct Records();
			
			receiverHT->SearchRecord(user2,&record);
			
			if( record == NULL){	// a record of user1 doesn't exist
			
				record = new struct Records();	// create it again (because it was previously deleted)
			
				record->wallet = node1->data;		
			
				record->list = new List<UserTLNode>();
			
				record->list->AddNode(Utlnode);
				
				// Add the record to HashTable
				receiverHT->AddRecord(record);
				
				record = NULL;
				
			}else{	//a record of user1 exists
			
				record->list->AddNode(Utlnode);
				
				record = NULL;
				
				//Update/overwrite record in HashTable
				//senderHT->UpdateRecord(record);			
			}
			
			/* HashTables are now updated */
			
			
			
			
			printf("Updated Hash tables ==> updating the rest \n");
			
			
			/* Need to update BitcoinTree(s) ,  Wallet->amount, Wallet->bitcoins(list) */
			
			/* Reminder: 
				node ==> user1/wallet node
				node1 ==> user2/wallet node
				
				node2 ==> node of user1 bitcoin list
				node3 ==> PTnode of user1(bitcoin node), PT = pointer to tree node => a node which contains a pointer to a tree node
				
				
				
			*/	
			
			
			/* Starting with Wallet  */
			
				/* user1/Wallet1 (node->data, was found previously) */
			
			node->data->amount = node->data->amount - amount;	//reducing overall balance of user1
			
			node1->data->amount = node1->data->amount + amount;	// increasing overall balance of user2
			
			
			int temp = amount;	//transaction amount
			
			Node<UserBNode>* node2 = node->data->bitcoins->head;
			
			while( (temp > 0 ) && ( node2 != NULL) ){	// traverse the bitcoin List of user1
			
			
			
			
			
			
				if( node2->data->value >= temp){	 // this bitcoin is more than enough to cover the amount (of transaction)
				
					node2->data->value = node2->data->value - temp;	//reducing the "value" of this bitcoin ( ex user1 now has 2$ in bitcoin 12 ) 
					
					
									
					Node<PTNode>* node3 = node2->data->current->head;	// traverse the PT List (pointers to tree nodes )
					
					while( (temp > 0) && ( node3 != NULL ) ){	
					
						printf("\n\n Calling Transaction() \n\n");
						
						if( node1->data == NULL) printf("\n\nERRROOOOORR !!!!!\n\n");
					
						node3->data->Transaction( &temp, node2->data->BitcoinID, node1->data, tlnode);	// we update the tree node, as well as create its children 
						// we also update the user2 (in the above function)
						
						
						
						
						
						if( node3->data->tnode == NULL){  // delete this as the value of tree node was used up
							
							Node<PTNode>* node4 = node3;
							
							node3 = node3->next;		// move to the next node
														
							// UserBNode-> current(PTNode list) -> DeleteNode();
							(node2->data)->current->DeleteNode(node4->data);		// delete node4 (actually ex-node3)
							
							node4 = NULL;
							
							continue;					
						}
						
										
						node3 = node3->next;
					
					}	//after this temp(transaction amount)  should be 0
										
					if( temp != 0){
					
						printf("SOMETHING IS WRONG (3) !!!!!!");
						
						delete senderHT;
						delete receiverHT;
			
						delete UserList;
						delete TransList;
						delete RootList;
			
						delete TransactionHT;
										
						return -1;
					}
					 
				
					/* If this bitcoin has no value (used it all up) */
					if( node2->data->value == 0){
					
						Node<UserBNode>* node4 = node2;
						
						node2 = node2->next;		// move to the next node
														
						// Wallet-> bitcoin(list) -> DeleteNode();
						(node->data)->bitcoins->DeleteNode(node4->data);		// delete node4 (actually ex-node3)
							
						node4 = NULL;
							
						continue;
					}
					
					node3 = NULL;
				
				}else{	// node2->data->value < temp ==> node2->data->value will be 0
				
					node2->data->value = 0;
								
					Node<PTNode>* node3 = node2->data->current->head;	// traverse the PT List (pointers to tree nodes )
					
					while( (temp > 0) && ( node3 != NULL ) ){	
					
						node3->data->Transaction( &temp, node2->data->BitcoinID, node1->data, tlnode);	// we update the tree node, as well as create its children 
						// we also update the user2 (in the above function)
						
						
						if( node3->data->tnode == NULL){  // delete this, as the value of tree node was used up ( will always be true )
							
							Node<PTNode>* node4 = node3;
							
							node3 = node3->next;		// move to the next node
														
							// UserBNode-> current(PTNode list) -> DeleteNode();
							(node2->data)->current->DeleteNode(node4->data);		// delete node4 (actually ex-node3)
							
							node4 = NULL;
							
							continue;					
						}
										
						node3 = node3->next;
					
					}	//after this temp(transaction amount)  should still be != 0
										
					if( node3 != NULL){	// we expect node3 == NULL
					
						printf("SOMETHING IS WRONG (4) !!!!!!");
										
						delete senderHT;
						delete receiverHT;
			
						delete UserList;
						delete TransList;
						delete RootList;
			
						delete TransactionHT;
						
						return -1;
					}
					 
				
					/* this bitcoin has no value (used it all up) ===> DELETE*/
					if( node2->data->value == 0){		// will always be true
					
					Node<UserBNode>* node4 = node2;
						
					node2 = node2->next;		// move to the next node
														
					// Wallet-> bitcoin(list) -> DeleteNode();
					(node->data)->bitcoins->DeleteNode(node4->data);		// delete node4 (actually ex-node3)
							
					node4 = NULL;
							
					continue;
					}
					
					node3 = NULL;
					
				}
				
				
				node2 = node2->next;
			}
			// finished  updating the structures (moving to the next part)
			
			
			if( temp > 0){	// loop finished ( with node2 = NULL ) 
			
				printf("ERROR temp must be 0");
											
				delete senderHT;
				delete receiverHT;
			
				delete UserList;
				delete TransList;
				delete RootList;
			
				delete TransactionHT;		
				
				return -1;
			}
			
			
			/* Updated hashTables and wallets( UserBNodes and UserBNodes->PTNodes ) and Trees */
		
			transaction = NULL;
			tlnode = NULL;
			Utlnode = NULL;
			
			node2 = NULL;
		
		}
		
		node = NULL;
		node1 = NULL;
		
		transaction = NULL;
			
	}//reached EOF
	
	printf(" Finished reading transactionfile \n");
	
	/* Closing bitcoinBalanceFile */
	
	if( fclose(fp) == -1){
	
		perror("Closing TransactionFile: ");
		return -1; 
	}
	
	//if(senderHT == receiverHT) printf("\n\nWHAAAAAAAAAAAT \n");
	
	
	//printf("\nPrinting HashTables \n\n");
	
	//printf("Printing SenderHT\n");
	
	//senderHT->Print();
	
	//printf("Printing ReceiverHT\n\n");
	
	//receiverHT->Print();
	
	/* Waiting for user input */
	
	line = NULL;
	length = 0;
	
	token = NULL;

	//char transactionId[50];
	//char user1[50];		// IMPORTANT
	//char user2[50];

	amount = 0;	// transaction amount
	
	//char date[11];
//	char date1[11];
	
	//char tim[6];
//	char tim1[6];

	char arg[11];
	char arg1[11];
	char arg2[11];
	char arg3[11];
	
	

	assignments = 0;	// how many successful assignments after sscanf()
	
	//maxID = 0;	// will be used later , to create new transactionID
	
	//time_t last_trans = 0;	//timestamp for last transaction
	
	while( (read = getline(&line,&length, stdin) ) != -1){
	
		printf("\n\n Line is %s \n\n", line);
	
	
		token = strtok(line, " ,\n");	
		
		//token = token + 1;	// pass the '\' character
		
		
		
		
		
		
		
		
		
		
		
		
		
		
/******************************** /requestTransaction  ***********************************************************/	
		
		if( strcmp( token, "/requestTransaction") == 0){	
		
			token = strtok(NULL,"");		// get the rest of line
			
			char sender[50];
			char receiver[50];
			int amount = 0;
			
			char date[11];		// DD-MM-YYYY 
			char tim[6];
		
			int assignments = 0;
			
			
			assignments = sscanf(token,"%s %s %d %s %s", sender, receiver, &amount, date,tim);
			
			if( (assignments < 3) || (assignments == 4) ) {
			
				printf(" Problem with arguments, (args == 3) or (args == 5), ==> ignoring your request... ");
				continue;			
			}
			
//			if( assignments == 5){ //date and time was given
			
//				printf("Date is %s \n",date);
				
			/* Check if sender,receiver exist  */
		
		
			/* Check sender */
	
			if( UserExists(UserList,sender) == false){	// user doesn't exist => exit program
		
				printf("User %s  doesn't exist ==> continue to next transaction \n", sender);
				continue;
			}
		
			printf("\nFound senderUser \n\n");
		
			/* Find Node of sender */
			Node<Wallet>* node = UserList->head;
		
			while( node != NULL){
		
				if( strcmp( node->data->WalletID, sender) == 0 ){
			
					printf(" Found our Node , breaking out of loop (3) \n");
				
					break;			
				}
		
				node = node->next;		
			}
		
			if( node == NULL){	// if  our node doesn't exist ===> ERROR
		
				printf(" ERROR , SOMETHING IS WRONG HERE !!!! (2)");
			
		
				delete senderHT;
				delete receiverHT;
			
				delete UserList;
				delete TransList;
				delete RootList;

				delete TransactionHT;
			
				return -1;	
			
			}
			
			
			/* Check if sender = receiver */
		
			if( strcmp(sender,receiver) == 0){	//check if same user
		
				printf(" Same User %s in this transaction => continue to next transaction \n", sender);
				continue;
			}
		
		
		
		
		
				/* Check receiver */
	
			if( UserExists(UserList,receiver) == false){	// user doesn't exist => exit program
			
				printf("User %s  doesn't exist ==> continue \n", receiver);
				continue;
			}

				/* Find Node of receiver */
			Node<Wallet>* node1 = UserList->head;
		
			while( node1 != NULL){
		
				if( strcmp( node1->data->WalletID,receiver) == 0 ){
			
					printf(" Found our Node , breaking out of loop (4) \n");
					
					break;			
				}
			
				node1 = node1->next;		
			}
		
			if( node1 == NULL){	// if  our node doesn't exist ===> ERROR
		
				printf(" ERROR , SOMETHING IS WRONG HERE !!!! (3)");
			
			
				delete senderHT;
				delete receiverHT;
			
				delete UserList;
				delete TransList;
				delete RootList;
			
				delete TransactionHT;
			
				return -1;	
		
			}
				
			/* Check if user1 has enough money, check the balance */
	
		
			if( amount <= 0){	// check if the transaction amount is invalid
		
				printf("The transaction amount is invalid ( <= 0) ===> continue to next transaction \n");
				continue;		
			}
	
	
			if( node->data->amount < amount){	// user1 can't pay
		
				printf("User1 can't complete this transaction , not enough $$$ \n");
				continue;		
			}
	
			// user1 has enough $$$ ====> OK


			/* Additional check : date not before the "last" transaction */ 

			time_t timestamp = 0;

			if( assignments == 5){	// date,time  given ==> create timestamp ==> have to check
			
				char* buffer = NULL;
				
				int length1 = snprintf(buffer,0,"%s %s",date,tim) + 1;
				buffer = new char[length1];
				snprintf(buffer,length1,"%s %s",date,tim);
				
				
				struct tm vtm = {0};
	
				if( strptime(buffer, "%d-%m-%Y %H:%M",&vtm) != NULL){
	
					timestamp =	mktime(&vtm);	
			
				}else{ 
		
					printf(" Error in strptime() (2) \n\n"); 
			
			
					delete senderHT;
					delete receiverHT;
			
					delete UserList;
					delete TransList;
					delete RootList;
			
					delete TransactionHT;
			
					return -1;
				}
				
				
				delete[] buffer;
				buffer = NULL;
				
				//now check if it was after the last one
				
				if( timestamp < last_trans){		// before last transaction ==> IGNORE
				
					printf(" Current transaction is before last one ==> ignoring your request \n");
					continue;				
				}
				
				//else , OK
				
				last_trans = timestamp;		// updating last_transaction timestamp
			
			}else if( assignments == 3){	// date,time not given ==> timestamp = current time { time(NULL) } ==> no need to check
			
				timestamp = time(NULL);		// current date and time
				
				last_trans = timestamp;
			
			} 
			


		
		
			printf("\n\nPassed the transaction checks ====> creating transaction \n\n");
		
			/* We start by creating new TransactionID
				We have kept a maxID counter , so we can easily find new IDs */
				
			maxID = maxID + 1;
			
			int length1 = snprintf(NULL,0,"%d",maxID) +1;
			char transactionId[length1];
			snprintf(transactionId,length1,"%d", maxID);
			
			//Add the new TransactionId to the HashTable
			TransactionHT->AddRecord1(transactionId);	
		
			/* Now we create the transaction */
		
			Transaction* transaction;
		
			/* Creating Transaction (and all associated nodes ) */
		
			transaction = new Transaction(transactionId, sender, receiver, amount, timestamp);
			
			TLNode* tlnode = new TLNode( transaction);
			
			UserTLNode* Utlnode = new UserTLNode( tlnode);
			
			/////////////////////////////////////////////////
						
			TransList->AddNode(tlnode);	//add the tlnode to the general transaction list
			
			
			/* Check if sender, receiver exist in the HashTables 
			
				If yes, update their contents */ 
			
			struct Records* record = new struct Records();
			
			senderHT->SearchRecord(sender,&record);
			
			if( record == NULL){	// a record of sender doesn't exist
			
				record = new struct Records();	//create it again (previous one was deleted )
			
				record->wallet = node->data;		
			
				record->list = new List<UserTLNode>();
			
				record->list->AddNode(Utlnode);
				
				// Add the record to HashTable
				senderHT->AddRecord(record);
				
				//we don't need record anymore
				
				record->wallet = NULL;
				record->list = NULL;
				
				delete record;				
				record = NULL;
				
			}else{	//a record of sender exists
			
				record->list->AddNode(Utlnode);
				
				//Update/overwrite record in HashTable (don't need to)
				//senderHT->UpdateRecord(record);
				
				
				//we don't need record anymore
				
				record->wallet = NULL;
				record->list = NULL;
				
				delete record;				
				record = NULL;							
			}
			
			////////////////////////////////////			
			
			/* Now for receiver */
			
			printf("ReceiverHT \n");
			
			printf("receiver is %s , sender is %s \n", receiver, sender);
			
			record = new struct Records();
			
			receiverHT->SearchRecord(receiver,&record);
			
			if( record == NULL){	// a record of receiver doesn't exist
			
				record = new struct Records();	// create it again (because it was previously deleted)
			
				record->wallet = node1->data;		
			
				record->list = new List<UserTLNode>();
			
				record->list->AddNode(Utlnode);
				
				// Add the record to HashTable
				receiverHT->AddRecord(record);
				
				record = NULL;
				
			}else{	//a record of receiver exists
			
				record->list->AddNode(Utlnode);
				
				record = NULL;
				
				//Update/overwrite record in HashTable
				//senderHT->UpdateRecord(record);			
			}
			
			/* HashTables are now updated */
			
			
			
			
			printf("Updated Hash tables ==> updating the rest \n");
			
			
			/* Need to update BitcoinTree(s) ,  Wallet->amount, Wallet->bitcoins(list) */
			
			/* Reminder: 
				node ==> user1/wallet node
				node1 ==> user2/wallet node
				
				node2 ==> node of user1 bitcoin list
				node3 ==> PTnode of user1(bitcoin node), PT = pointer to tree node => a node which contains a pointer to a tree node
				
				
				
			*/	
			
			
			/* Starting with Wallet  */
			
				/* user1/Wallet1 (node->data, was found previously) */
			
			node->data->amount = node->data->amount - amount;	//reducing overall balance of user1
			
			node1->data->amount = node1->data->amount + amount;	// increasing overall balance of user2
			
			
			int temp = amount;	//transaction amount
			
			Node<UserBNode>* node2 = node->data->bitcoins->head;
			
			while( (temp > 0 ) && ( node2 != NULL) ){	// traverse the bitcoin List of user1
			
			
				if( node2->data->value >= temp){	 // this bitcoin is more than enough to cover the amount (of transaction)
				
					node2->data->value = node2->data->value - temp;	//reducing the "value" of this bitcoin ( ex user1 now has 2$ in bitcoin 12 ) 
					
					
									
					Node<PTNode>* node3 = node2->data->current->head;	// traverse the PT List (pointers to tree nodes )
					
					while( (temp > 0) && ( node3 != NULL ) ){	
					
						printf("\n\n Calling Transaction() \n\n");
						
						if( node1->data == NULL) printf("\n\nERRROOOOORR !!!!!\n\n");
					
						node3->data->Transaction( &temp, node2->data->BitcoinID, node1->data, tlnode);	// we update the tree node, as well as create its children 
						// we also update the user2 (in the above function)
						
						
						
						
						
						if( node3->data->tnode == NULL){  // delete this as the value of tree node was used up
							
							Node<PTNode>* node4 = node3;
							
							node3 = node3->next;		// move to the next node
														
							// UserBNode-> current(PTNode list) -> DeleteNode();
							(node2->data)->current->DeleteNode(node4->data);		// delete node4 (actually ex-node3)
							
							node4 = NULL;
							
							continue;					
						}
						
										
						node3 = node3->next;
					
					}	//after this temp(transaction amount)  should be 0
										
					if( temp != 0){
					
						printf("SOMETHING IS WRONG (3) !!!!!!");
						
						delete senderHT;
						delete receiverHT;
			
						delete UserList;
						delete TransList;
						delete RootList;
			
						delete TransactionHT;
										
						return -1;
					}
					 
				
					/* If this bitcoin has no value (used it all up) */
					if( node2->data->value == 0){
					
						Node<UserBNode>* node4 = node2;
						
						node2 = node2->next;		// move to the next node
														
						// Wallet-> bitcoin(list) -> DeleteNode();
						(node->data)->bitcoins->DeleteNode(node4->data);		// delete node4 (actually ex-node3)
							
						node4 = NULL;
							
						continue;
					}
					
					node3 = NULL;
				
				}else{	// node2->data->value < temp ==> node2->data->value will be 0
				
					node2->data->value = 0;
								
					Node<PTNode>* node3 = node2->data->current->head;	// traverse the PT List (pointers to tree nodes )
					
					while( (temp > 0) && ( node3 != NULL ) ){	
					
						node3->data->Transaction( &temp, node2->data->BitcoinID, node1->data, tlnode);	// we update the tree node, as well as create its children 
						// we also update the user2 (in the above function)
						
						
						if( node3->data->tnode == NULL){  // delete this, as the value of tree node was used up ( will always be true )
							
							Node<PTNode>* node4 = node3;
							
							node3 = node3->next;		// move to the next node
														
							// UserBNode-> current(PTNode list) -> DeleteNode();
							(node2->data)->current->DeleteNode(node4->data);		// delete node4 (actually ex-node3)
							
							node4 = NULL;
							
							continue;					
						}
										
						node3 = node3->next;
					
					}	//after this temp(transaction amount)  should still be != 0
										
					if( node3 != NULL){	// we expect node3 == NULL
					
						printf("SOMETHING IS WRONG (4) !!!!!!");
										
						delete senderHT;
						delete receiverHT;
			
						delete UserList;
						delete TransList;
						delete RootList;
			
						delete TransactionHT;
						
						return -1;
					}
					 
				
					/* this bitcoin has no value (used it all up) ===> DELETE*/
					if( node2->data->value == 0){		// will always be true
					
					Node<UserBNode>* node4 = node2;
						
					node2 = node2->next;		// move to the next node
														
					// Wallet-> bitcoin(list) -> DeleteNode();
					(node->data)->bitcoins->DeleteNode(node4->data);		// delete node4 (actually ex-node3)
							
					node4 = NULL;
							
					continue;
					}
					
					node3 = NULL;
					
				}
				
				
				node2 = node2->next;
			}
			// finished  updating the structures (moving to the next part)
			
			
			if( temp > 0){	// loop finished ( with node2 = NULL ) 
			
				printf("ERROR temp must be 0");
											
				delete senderHT;
				delete receiverHT;
			
				delete UserList;
				delete TransList;
				delete RootList;
			
				delete TransactionHT;		
				
				return -1;
			}
			
			
			/* Updated hashTables and wallets( UserBNodes and UserBNodes->PTNodes ) and Trees */
			printf("\n\nFinished the requested transaction \n\n");
			transaction->Print();
		
		
			transaction = NULL;
			tlnode = NULL;
			Utlnode = NULL;
			
			node2 = NULL;
			node1 = NULL;
			
//			printf("\n\nFinished the requested transaction \n\n");			
			continue;	// we are finished here
		}
		


























/******************************** /requestTransactions  ***********************************************************/	
		
		if( strcmp( token, "/requestTransactions") == 0){
		
		
			token = strtok(NULL,"\n");		// get the rest of line
			
			char sarg[50];
			char sarg1[50];
			
			char samount[10];
			int amount = 0;
			
			char date[11];		// DD-MM-YYYY 
			char tim[6];
		
			int assignments = 0;
		
			assignments = sscanf(token,"%s %s %s %s %s",sarg,sarg1,samount,date,tim);
			
			if( (assignments == 0 ) || ( assignments == 2 ) || (assignments == 4 ) ){
				
				printf("Problem with arguments, it should be:  args = 1,3,5 \n");
				continue;			
			}
			
			printf("assignments %d \n", assignments);
		
		
			if( ( assignments == 3) || ( assignments == 5) ){	// read line by line
			
				int ki = strlen(token);
				
				printf("token is: %s \n", token);
				
				printf(" token[ki] is: %c \n", token[ki-1]);
				
				if( token[ki-1] != ';'){			// -1 because we start from 0
				
					printf("You forgot the ';' at the end, try again \n\n");
					continue;				
				}
				
				
				if( assignments == 3){
				
					int k = strlen(samount) - 1;
				
					samount[k] = '\0';		// replace the ; with \0
				
				}
			
				amount = atoi( samount );
			
			
				/* Check if sender(sarg) ,receiver(sarg1) exist  */
		
		
				/* Check sender */
	
				if( UserExists(UserList,sarg) == false){	// user doesn't exist => exit program
		
					printf("User %s  doesn't exist ==> continue to next transaction \n", sarg);
					continue;
				}
		
				printf("\nFound senderUser \n\n");
		
				/* Find Node of sender */
				Node<Wallet>* node = UserList->head;
		
				while( node != NULL){
		
					if( strcmp( node->data->WalletID, sarg) == 0 ){
			
						printf(" Found our Node , breaking out of loop (3) \n");
				
						break;			
					}
		
					node = node->next;		
				}
			
				if( node == NULL){	// if  our node doesn't exist ===> ERROR
		
					printf(" ERROR , SOMETHING IS WRONG HERE !!!! (2)");
			
		
					delete senderHT;
					delete receiverHT;
			
					delete UserList;
					delete TransList;
					delete RootList;

					delete TransactionHT;
			
					return -1;	
			
				}
			
			
				/* Check if sender = receiver */
		
				if( strcmp(sarg,sarg1) == 0){	//check if same user
		
					printf(" Same User %s in this transaction => continue to next transaction \n", sarg);
					continue;
				}
		
		
		
		
		
				/* Check receiver */
	
				if( UserExists(UserList, sarg1) == false){	// user doesn't exist => exit program
			
					printf("User %s  doesn't exist ==> continue \n", sarg1);
					continue;
				}

				/* Find Node of receiver */
				Node<Wallet>* node1 = UserList->head;
		
				while( node1 != NULL){
			
					if( strcmp( node1->data->WalletID, sarg1) == 0 ){
			
						printf(" Found our Node , breaking out of loop (4) \n");
					
						break;			
					}
			
					node1 = node1->next;		
				}
		
				if( node1 == NULL){	// if  our node doesn't exist ===> ERROR
		
					printf(" ERROR , SOMETHING IS WRONG HERE !!!! (3)");
			
				
					delete senderHT;
					delete receiverHT;
			
					delete UserList;
					delete TransList;
					delete RootList;
			
					delete TransactionHT;
			
					return -1;	
		
				}
				
				/* Check if user1 has enough money, check the balance */
	
		
				if( amount <= 0){	// check if the transaction amount is invalid
		
					printf("The transaction amount is invalid ( <= 0) ===> continue to next transaction \n");
					continue;		
				}
	
	
				if( node->data->amount < amount){	// user1 can't pay
		
					printf("User1 can't complete this transaction , not enough $$$ \n");
					continue;		
				}
	
				// user1 has enough $$$ ====> OK


				/* Additional check : date not before the "last" transaction */ 

				time_t timestamp = 0;

				if( assignments == 5){	// date,time  given ==> create timestamp ==> have to check
			
					char* buffer = NULL;
				
				
				
					int k = strlen(tim);
					tim[k] ='\0';		//replace the ;
				
				
					int length1 = snprintf(buffer,0,"%s %s",date,tim) + 1;
					buffer = new char[length1];
					snprintf(buffer,length1,"%s %s",date,tim);
				
				
					struct tm vtm = {0};
	
					if( strptime(buffer, "%d-%m-%Y %H:%M",&vtm) != NULL){
	
						timestamp =	mktime(&vtm);	
			
					}else{ 
		
						printf(" Error in strptime() (2) \n\n"); 
			
			
						delete senderHT;
						delete receiverHT;
			
						delete UserList;
						delete TransList;
						delete RootList;
			
						delete TransactionHT;
			
						return -1;
					}
				
				
					delete[] buffer;
					buffer = NULL;
				
					//now check if it was after the last one
				
					if( timestamp < last_trans){		// before last transaction ==> IGNORE
				
						printf(" Current transaction is before last one ==> ignoring your request \n");
						continue;				
					}
					
					//else , OK
				
					last_trans = timestamp;		// updating last_transaction timestamp
			
				}else if( assignments == 3){	// date,time not given ==> timestamp = current time { time(NULL) } ==> no need to check
			
					timestamp = time(NULL);		// current date and time
					
					last_trans = timestamp;
				
				} 
			


		
		
				printf("\n\nPassed the transaction checks ====> creating transaction \n\n");
		
				/* We start by creating new TransactionID
					We have kept a maxID counter , so we can easily find new IDs */
				
				maxID = maxID + 1;
			
				int length1 = snprintf(NULL,0,"%d",maxID) +1;
				char transactionId[length1];
				snprintf(transactionId,length1,"%d", maxID);
				
				//Add new transactionId to the TransactionHT
				TransactionHT->AddRecord1(transactionId);
						
		
				/* Now we create the transaction */
		
				Transaction* transaction;
		
				/* Creating Transaction (and all associated nodes ) */
		
				transaction = new Transaction(transactionId, sarg, sarg1, amount, timestamp);
			
				TLNode* tlnode = new TLNode( transaction);
			
				UserTLNode* Utlnode = new UserTLNode( tlnode);
			
				/////////////////////////////////////////////////
						
				TransList->AddNode(tlnode);	//add the tlnode to the general transaction list
			
			
				/* Check if sender, receiver exist in the HashTables 
			
					If yes, update their contents */ 
			
				struct Records* record = new struct Records();
			
				senderHT->SearchRecord(sarg,&record);
			
				if( record == NULL){	// a record of user1 doesn't exist
			
					record = new struct Records();	//create it again (previous one was deleted )
			
					record->wallet = node->data;		
			
					record->list = new List<UserTLNode>();
			
					record->list->AddNode(Utlnode);
				
					// Add the record to HashTable
					senderHT->AddRecord(record);
				
					//we don't need record anymore
					
					record->wallet = NULL;
					record->list = NULL;
				
					delete record;				
					record = NULL;
				
				}else{	//a record of user1 exists
			
					record->list->AddNode(Utlnode);
				
					//Update/overwrite record in HashTable (don't need to)
					//senderHT->UpdateRecord(record);
				
				
					//we don't need record anymore
				
					record->wallet = NULL;
					record->list = NULL;
					
					delete record;				
					record = NULL;							
				}
			
				////////////////////////////////////			
			
				/* Now for receiver */
			
				printf("ReceiverHT \n");
			
				printf("receiver is %s , sender is %s \n", sarg1, sarg);
			
				record = new struct Records();
			
				receiverHT->SearchRecord(sarg1,&record);
			
				if( record == NULL){	// a record of user1 doesn't exist
			
					record = new struct Records();	// create it again (because it was previously deleted)
				
					record->wallet = node1->data;		
			
					record->list = new List<UserTLNode>();
			
					record->list->AddNode(Utlnode);
				
					// Add the record to HashTable
					receiverHT->AddRecord(record);
				
					record = NULL;
				
				}else{	//a record of user1 exists
			
					record->list->AddNode(Utlnode);
				
					record = NULL;
				
					//Update/overwrite record in HashTable
					//senderHT->UpdateRecord(record);			
				}
				
				/* HashTables are now updated */
			
			
			
			
				printf("Updated Hash tables ==> updating the rest \n");
			
			
				/* Need to update BitcoinTree(s) ,  Wallet->amount, Wallet->bitcoins(list) */
			
				/* Reminder: 
					node ==> user1/wallet node
					node1 ==> user2/wallet node
				
					node2 ==> node of user1 bitcoin list
					node3 ==> PTnode of user1(bitcoin node), PT = pointer to tree node => a node which contains a pointer to a tree node
				
				
				
				*/	
			
			
				/* Starting with Wallet  */
			
					/* user1/Wallet1 (node->data, was found previously) */
			
				node->data->amount = node->data->amount - amount;	//reducing overall balance of user1
			
				node1->data->amount = node1->data->amount + amount;	// increasing overall balance of user2
			
			
				int temp = amount;	//transaction amount
			
				Node<UserBNode>* node2 = node->data->bitcoins->head;
			
				while( (temp > 0 ) && ( node2 != NULL) ){	// traverse the bitcoin List of user1
			
			
					if( node2->data->value >= temp){	 // this bitcoin is more than enough to cover the amount (of transaction)
				
						node2->data->value = node2->data->value - temp;	//reducing the "value" of this bitcoin ( ex user1 now has 2$ in bitcoin 12 ) 
					
					
									
						Node<PTNode>* node3 = node2->data->current->head;	// traverse the PT List (pointers to tree nodes )
					
						while( (temp > 0) && ( node3 != NULL ) ){	
						
							printf("\n\n Calling Transaction() \n\n");
						
							if( node1->data == NULL) printf("\n\nERRROOOOORR !!!!!\n\n");
					
							node3->data->Transaction( &temp, node2->data->BitcoinID, node1->data, tlnode);	// we update the tree node, as well as create its children 
							// we also update the user2 (in the above function)
							
						
						
						
						
							if( node3->data->tnode == NULL){  // delete this as the value of tree node was used up
							
								Node<PTNode>* node4 = node3;
							
								node3 = node3->next;		// move to the next node
														
								// UserBNode-> current(PTNode list) -> DeleteNode();
								(node2->data)->current->DeleteNode(node4->data);		// delete node4 (actually ex-node3)
							
								node4 = NULL;
							
								continue;					
							}
							
										
							node3 = node3->next;
					
						}	//after this temp(transaction amount)  should be 0
										
						if( temp != 0){
					
							printf("SOMETHING IS WRONG (3) !!!!!!");
						
							delete senderHT;
							delete receiverHT;
			
							delete UserList;
							delete TransList;
							delete RootList;
				
							delete TransactionHT;
											
							return -1;
						}
					 
					
						/* If this bitcoin has no value (used it all up) */
						if( node2->data->value == 0){
					
							Node<UserBNode>* node4 = node2;
						
							node2 = node2->next;		// move to the next node
														
							// Wallet-> bitcoin(list) -> DeleteNode();
							(node->data)->bitcoins->DeleteNode(node4->data);		// delete node4 (actually ex-node2)
							
							node4 = NULL;
							
							continue;
						}
					
						node3 = NULL;
				
					}else{	// node2->data->value < temp ==> node2->data->value will be 0
				
						node2->data->value = 0;
								
						Node<PTNode>* node3 = node2->data->current->head;	// traverse the PT List (pointers to tree nodes )
					
						while( (temp > 0) && ( node3 != NULL ) ){	
					
							node3->data->Transaction( &temp, node2->data->BitcoinID, node1->data, tlnode);	// we update the tree node, as well as create its children 
							// we also update the user2 (in the above function)
							
						
							if( node3->data->tnode == NULL){  // delete this, as the value of tree node was used up ( will always be true )
							
								Node<PTNode>* node4 = node3;
							
								node3 = node3->next;		// move to the next node
														
								// UserBNode-> current(PTNode list) -> DeleteNode();
								(node2->data)->current->DeleteNode(node4->data);		// delete node4 (actually ex-node3)
							
								node4 = NULL;
							
								continue;					
							}
										
							node3 = node3->next;
					
						}	//after this temp(transaction amount)  should still be != 0
										
						if( node3 != NULL){	// we expect node3 == NULL
					
							printf("SOMETHING IS WRONG (4) !!!!!!");
										
							delete senderHT;
							delete receiverHT;
			
							delete UserList;
							delete TransList;
							delete RootList;
			
							delete TransactionHT;
						
							return -1;
						}
					 
					
						/* this bitcoin has no value (used it all up) ===> DELETE*/
						if( node2->data->value == 0){		// will always be true
					
							Node<UserBNode>* node4 = node2;
						
							node2 = node2->next;		// move to the next node
														
							// Wallet-> bitcoin(list) -> DeleteNode();
							(node->data)->bitcoins->DeleteNode(node4->data);		// delete node4 (actually ex-node3)
							
							node4 = NULL;
							
							continue;
						}
					
						node3 = NULL;
					
					}
				
				
					node2 = node2->next;
				}
				// finished  updating the structures (moving to the next part)
			
			
				if( temp > 0){	// loop finished ( with node2 = NULL ) 
				
					printf("ERROR temp must be 0");
											
					delete senderHT;
					delete receiverHT;
			
					delete UserList;
					delete TransList;
					delete RootList;
			
					delete TransactionHT;		
				
					return -1;
				}
			
			
				/* Updated hashTables and wallets( UserBNodes and UserBNodes->PTNodes ) and Trees */
				printf("\n\nFinished the requested transaction \n\n");
				transaction->Print();
		
		
				transaction = NULL;
				tlnode = NULL;
				Utlnode = NULL;
				
				node2 = NULL;
				node1 = NULL;
			
	//			printf("\n\nFinished the requested transaction \n\n");			
//				continue;	// we are finished here
				
				
				
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////		


				printf("From now on, you don't have to type /requestTransactions, just the type the arguments \n");
				
				printf("\nYou can exit this mode , by typing '/exit' \n\n");
				
			/* Now we will start reading the rest of the transactions form stdin */	
				
				printf("Please enter your transaction, or exit by typing '/exit' \n"); 	
				
					
				
				
				
				while( (read = getline(&line,&length, stdin) ) != -1){
	
					printf("\n\n Line is %s \n\n", line);
	
	
					token = strtok(line, " ,\n");	
		


					if( strcmp(token,"/exit") == 0 ) break;
					
					//else it is a transaction
					
					strcpy(sarg,token);		// sarg = user1, sarg1 = user2
					
					
					
					token = strtok(NULL,"\n");		// get the rest of line
			
//					char sarg[50];
//					char sarg1[50];
			
//					char samount[10];
					amount = 0;
			
//					char date[11];		// DD-MM-YYYY 
//					char tim[6];
		
					assignments = 0;
					
					
		
					assignments = sscanf(token,"%s %s %s %s",sarg1,samount,date,tim);
					
					printf(" assignments is %d \n", assignments);
			
					if( (assignments < 2) || (assignments == 3) ){
				
						printf("Problem with arguments, it should be:  args = 3,5 \n");
						continue;			
					}
		
		
					int ki = strlen(token) -1;
		
					if( token[ki] != ';'){
			
						printf("You forgot the ';' at the end, try again \n\n");
						continue;				
					}
				
				
					if( assignments == 2){
				
						int k = strlen(samount) -1;
				
						samount[k] = '\0';		// replace the ; with \0
				
					}
			
					amount = atoi( samount );
			
			
					/* Check if sender(sarg) ,receiver(sarg1) exist  */
		
		
					/* Check sender */
	
					if( UserExists(UserList,sarg) == false){	// user doesn't exist => exit program
		
						printf("User %s  doesn't exist ==> continue to next transaction \n", sarg);
						continue;
					}
		
					printf("\nFound senderUser \n\n");
		
					/* Find Node of sender */
					Node<Wallet>* node = UserList->head;
		
					while( node != NULL){
		
						if( strcmp( node->data->WalletID, sarg) == 0 ){
			
							printf(" Found our Node , breaking out of loop (3) \n");
				
							break;			
						}
		
						node = node->next;		
					}
			
					if( node == NULL){	// if  our node doesn't exist ===> ERROR
		
						printf(" ERROR , SOMETHING IS WRONG HERE !!!! (2)");
			
			
						delete senderHT;
						delete receiverHT;
			
						delete UserList;
						delete TransList;
						delete RootList;

						delete TransactionHT;
			
						return -1;	
			
					}
			
				
					/* Check if sender = receiver */
		
					if( strcmp(sarg,sarg1) == 0){	//check if same user
		
						printf(" Same User %s in this transaction => continue to next transaction \n", sarg);
						continue;
					}
			
		
		
		
		
					/* Check receiver */
	
					if( UserExists(UserList, sarg1) == false){	// user doesn't exist => exit program
			
						printf("User %s  doesn't exist ==> continue \n", sarg1);
						continue;
					}

					/* Find Node of receiver */
					Node<Wallet>* node1 = UserList->head;
		
					while( node1 != NULL){
			
						if( strcmp( node1->data->WalletID, sarg1) == 0 ){
			
							printf(" Found our Node , breaking out of loop (4) \n");
					
							break;			
						}
			
					node1 = node1->next;		
				}
		
				if( node1 == NULL){	// if  our node doesn't exist ===> ERROR
		
					printf(" ERROR , SOMETHING IS WRONG HERE !!!! (3)");
			
				
					delete senderHT;
					delete receiverHT;
			
					delete UserList;
					delete TransList;
					delete RootList;
			
					delete TransactionHT;
			
					return -1;	
		
				}
				
				/* Check if user1 has enough money, check the balance */
	
		
				if( amount <= 0){	// check if the transaction amount is invalid
		
					printf("The transaction amount is invalid ( <= 0) ===> continue to next transaction \n");
					continue;		
				}
	
	
				if( node->data->amount < amount){	// user1 can't pay
		
					printf("User1 can't complete this transaction , not enough $$$ \n");
					continue;		
				}
	
				// user1 has enough $$$ ====> OK


				/* Additional check : date not before the "last" transaction */ 

				time_t timestamp = 0;

				if( assignments == 4){	// date,time  given ==> create timestamp ==> have to check
			
					char* buffer = NULL;
				
				
				
					int k = strlen(tim);
					tim[k] ='\0';		//replace the ;
				
				
					int length1 = snprintf(buffer,0,"%s %s",date,tim) + 1;
					buffer = new char[length1];
					snprintf(buffer,length1,"%s %s",date,tim);
				
				
					struct tm vtm = {0};
	
					if( strptime(buffer, "%d-%m-%Y %H:%M",&vtm) != NULL){
	
						timestamp =	mktime(&vtm);	
			
					}else{ 
		
						printf(" Error in strptime() (2) \n\n"); 
			
			
						delete senderHT;
						delete receiverHT;
			
						delete UserList;
						delete TransList;
						delete RootList;
			
						delete TransactionHT;
			
						return -1;
					}
				
				
					delete[] buffer;
					buffer = NULL;
				
					//now check if it was after the last one
				
					if( timestamp < last_trans){		// before last transaction ==> IGNORE
				
						printf(" Current transaction is before last one ==> ignoring your request \n");
						continue;				
					}
					
					//else , OK
				
					last_trans = timestamp;		// updating last_transaction timestamp
			
				}else if( assignments == 2){	// date,time not given ==> timestamp = current time { time(NULL) } ==> no need to check
			
					timestamp = time(NULL);		// current date and time
					
					last_trans = timestamp;
				
				} 
			


		
		
				printf("\n\nPassed the transaction checks ====> creating transaction \n\n");
		
				/* We start by creating new TransactionID
					We have kept a maxID counter , so we can easily find new IDs */
				
				maxID = maxID + 1;
			
				int length1 = snprintf(NULL,0,"%d",maxID) +1;
				char transactionId[length1];
				snprintf(transactionId,length1,"%d", maxID);
				
				//Add the new transactionId to the transactionHT
				TransactionHT->AddRecord1(transactionId);
						
		
				/* Now we create the transaction */
		
				Transaction* transaction;
		
				/* Creating Transaction (and all associated nodes ) */
		
				transaction = new Transaction(transactionId, sarg, sarg1, amount, timestamp);
			
				TLNode* tlnode = new TLNode( transaction);
			
				UserTLNode* Utlnode = new UserTLNode( tlnode);
			
				/////////////////////////////////////////////////
						
				TransList->AddNode(tlnode);	//add the tlnode to the general transaction list
			
			
				/* Check if sender, receiver exist in the HashTables 
			
					If yes, update their contents */ 
			
				struct Records* record = new struct Records();
			
				senderHT->SearchRecord(sarg,&record);
			
				if( record == NULL){	// a record of user1 doesn't exist
			
					record = new struct Records();	//create it again (previous one was deleted )
			
					record->wallet = node->data;		
			
					record->list = new List<UserTLNode>();
			
					record->list->AddNode(Utlnode);
				
					// Add the record to HashTable
					senderHT->AddRecord(record);
				
					//we don't need record anymore
					
					record->wallet = NULL;
					record->list = NULL;
				
					delete record;				
					record = NULL;
				
				}else{	//a record of user1 exists
			
					record->list->AddNode(Utlnode);
				
					//Update/overwrite record in HashTable (don't need to)
					//senderHT->UpdateRecord(record);
				
				
					//we don't need record anymore
				
					record->wallet = NULL;
					record->list = NULL;
					
					delete record;				
					record = NULL;							
				}
			
				////////////////////////////////////			
			
				/* Now for receiver */
			
				printf("ReceiverHT \n");
			
				printf("receiver is %s , sender is %s \n", sarg1, sarg);
			
				record = new struct Records();
			
				receiverHT->SearchRecord(sarg1,&record);
			
				if( record == NULL){	// a record of user1 doesn't exist
			
					record = new struct Records();	// create it again (because it was previously deleted)
				
					record->wallet = node1->data;		
			
					record->list = new List<UserTLNode>();
			
					record->list->AddNode(Utlnode);
				
					// Add the record to HashTable
					receiverHT->AddRecord(record);
				
					record = NULL;
				
				}else{	//a record of user1 exists
			
					record->list->AddNode(Utlnode);
				
					record = NULL;
				
					//Update/overwrite record in HashTable
					//senderHT->UpdateRecord(record);			
				}
				
				/* HashTables are now updated */
			
			
			
			
				printf("Updated Hash tables ==> updating the rest \n");
			
			
				/* Need to update BitcoinTree(s) ,  Wallet->amount, Wallet->bitcoins(list) */
			
				/* Reminder: 
					node ==> user1/wallet node
					node1 ==> user2/wallet node
				
					node2 ==> node of user1 bitcoin list
					node3 ==> PTnode of user1(bitcoin node), PT = pointer to tree node => a node which contains a pointer to a tree node
				
				
				
				*/	
			
			
				/* Starting with Wallet  */
			
					/* user1/Wallet1 (node->data, was found previously) */
			
				node->data->amount = node->data->amount - amount;	//reducing overall balance of user1
			
				node1->data->amount = node1->data->amount + amount;	// increasing overall balance of user2
			
			
				int temp = amount;	//transaction amount
			
				Node<UserBNode>* node2 = node->data->bitcoins->head;
			
				while( (temp > 0 ) && ( node2 != NULL) ){	// traverse the bitcoin List of user1
			
			
					if( node2->data->value >= temp){	 // this bitcoin is more than enough to cover the amount (of transaction)
				
						node2->data->value = node2->data->value - temp;	//reducing the "value" of this bitcoin ( ex user1 now has 2$ in bitcoin 12 ) 
					
					
									
						Node<PTNode>* node3 = node2->data->current->head;	// traverse the PT List (pointers to tree nodes )
					
						while( (temp > 0) && ( node3 != NULL ) ){	
						
							printf("\n\n Calling Transaction() \n\n");
						
							if( node1->data == NULL) printf("\n\nERRROOOOORR !!!!!\n\n");
					
							node3->data->Transaction( &temp, node2->data->BitcoinID, node1->data, tlnode);	// we update the tree node, as well as create its children 
							// we also update the user2 (in the above function)
							
						
						
						
						
							if( node3->data->tnode == NULL){  // delete this as the value of tree node was used up
							
								Node<PTNode>* node4 = node3;
							
								node3 = node3->next;		// move to the next node
														
								// UserBNode-> current(PTNode list) -> DeleteNode();
								(node2->data)->current->DeleteNode(node4->data);		// delete node4 (actually ex-node3)
							
								node4 = NULL;
							
								continue;					
							}
							
										
							node3 = node3->next;
					
						}	//after this temp(transaction amount)  should be 0
										
						if( temp != 0){
					
							printf("SOMETHING IS WRONG (3) !!!!!!");
						
							delete senderHT;
							delete receiverHT;
			
							delete UserList;
							delete TransList;
							delete RootList;
				
							delete TransactionHT;
											
							return -1;
						}
					 
					
						/* If this bitcoin has no value (used it all up) */
						if( node2->data->value == 0){
					
							Node<UserBNode>* node4 = node2;
						
							node2 = node2->next;		// move to the next node
														
							// Wallet-> bitcoin(list) -> DeleteNode();
							(node->data)->bitcoins->DeleteNode(node4->data);		// delete node4 (actually ex-node3)
							
							node4 = NULL;
							
							continue;
						}
					
						node3 = NULL;
				
					}else{	// node2->data->value < temp ==> node2->data->value will be 0
				
						node2->data->value = 0;
								
						Node<PTNode>* node3 = node2->data->current->head;	// traverse the PT List (pointers to tree nodes )
					
						while( (temp > 0) && ( node3 != NULL ) ){	
					
							node3->data->Transaction( &temp, node2->data->BitcoinID, node1->data, tlnode);	// we update the tree node, as well as create its children 
							// we also update the user2 (in the above function)
							
						
							if( node3->data->tnode == NULL){  // delete this, as the value of tree node was used up ( will always be true )
							
								Node<PTNode>* node4 = node3;
							
								node3 = node3->next;		// move to the next node
														
								// UserBNode-> current(PTNode list) -> DeleteNode();
								(node2->data)->current->DeleteNode(node4->data);		// delete node4 (actually ex-node3)
							
								node4 = NULL;
							
								continue;					
							}
										
							node3 = node3->next;
					
						}	//after this temp(transaction amount)  should still be != 0
										
						if( node3 != NULL){	// we expect node3 == NULL
					
							printf("SOMETHING IS WRONG (4) !!!!!!");
										
							delete senderHT;
							delete receiverHT;
			
							delete UserList;
							delete TransList;
							delete RootList;
			
							delete TransactionHT;
						
							return -1;
						}
					 
					
						/* this bitcoin has no value (used it all up) ===> DELETE*/
						if( node2->data->value == 0){		// will always be true
					
							Node<UserBNode>* node4 = node2;
						
							node2 = node2->next;		// move to the next node
														
							// Wallet-> bitcoin(list) -> DeleteNode();
							(node->data)->bitcoins->DeleteNode(node4->data);		// delete node4 (actually ex-node3)
							
							node4 = NULL;
							
							continue;
						}
					
						node3 = NULL;
					
					}
				
				
					node2 = node2->next;
				}
				// finished  updating the structures (moving to the next part)
			
			
				if( temp > 0){	// loop finished ( with node2 = NULL ) 
				
					printf("ERROR temp must be 0");
											
					delete senderHT;
					delete receiverHT;
			
					delete UserList;
					delete TransList;
					delete RootList;
			
					delete TransactionHT;		
				
					return -1;
				}
			
			
				/* Updated hashTables and wallets( UserBNodes and UserBNodes->PTNodes ) and Trees */
				printf("\n\nFinished the requested transaction \n\n");
				transaction->Print();
		
		
				transaction = NULL;
				tlnode = NULL;
				Utlnode = NULL;
				
				node2 = NULL;
				node1 = NULL;
					
					
				continue;
			}			
			//exited the loop		
					
			printf("You have exited the inner loop, please enter new command \n");
			
			continue;


			
		}else if( assignments == 1){	// read file
		
		
			printf("Sarg is %s\n",sarg);
		
			/* Opening inputFile */
	
			fp = fopen(sarg,"r");
			if( fp == NULL){
	
				perror("Opening Input File: ");
				return -1;
	
			}
	
			/* Reading line by line */
	
			line = NULL;
			length = 0;
	
			token = NULL;

//			char transactionId[50];
//			char user1[50];		// IMPORTANT
//			char user2[50];

			amount = 0;	// transaction amount
	
//			char date[11];
//			char tim[6];


			assignments = 0;	// how many successful assignments after sscanf()
	
//			int maxID = 0;	// will be used later , to create new transactionID
	
//	time_t last_trans = 0;	//timestamp for last transaction

	
	
			while( (read = getline(&line,&length, fp) ) != -1){
	
				printf(" Line is: %s \n", line);
				
				token = strtok(line,"");
	
				assignments = sscanf(line,"%s %s %s %s %s",sarg,sarg1,samount,date,tim);
			
				if( ( assignments < 3 ) || (assignments == 4 ) ){
				
					printf("Problem with arguments, it should be:  args = 3,5 \n");
					continue;			
				}
		
		
				int ki = strlen(line);
				
				if( line[ki - 2] != ';'){		// -1 for '\n' and -1 because we count from 0
				
					printf("You forgot the ';' at the end, try again \n\n");
					continue;	
				}
				
				
				if( assignments == 3){
				
					int k = strlen(samount);
				
					samount[k-1] = '\0';		// replace the ; with \0
				
				}
			
				amount = atoi( samount );
			
			
				/* Check if sender(sarg) ,receiver(sarg1) exist  */
		
		
				/* Check sender */
	
				if( UserExists(UserList,sarg) == false){	// user doesn't exist => exit program
		
					printf("User %s  doesn't exist ==> continue to next transaction \n", sarg);
					continue;
				}
		
				printf("\nFound senderUser \n\n");
		
				/* Find Node of sender */
				Node<Wallet>* node = UserList->head;
		
				while( node != NULL){
		
					if( strcmp( node->data->WalletID, sarg) == 0 ){
			
						printf(" Found our Node , breaking out of loop (3) \n");
				
						break;			
					}
		
					node = node->next;		
				}
			
				if( node == NULL){	// if  our node doesn't exist ===> ERROR
		
					printf(" ERROR , SOMETHING IS WRONG HERE !!!! (2)");
			
		
					delete senderHT;
					delete receiverHT;
			
					delete UserList;
					delete TransList;
					delete RootList;

					delete TransactionHT;
			
					return -1;	
			
				}
			
			
				/* Check if sender = receiver */
		
				if( strcmp(sarg,sarg1) == 0){	//check if same user
		
					printf(" Same User %s in this transaction => continue to next transaction \n", sarg);
					continue;
				}
		
		
		
		
		
				/* Check receiver */
	
				if( UserExists(UserList, sarg1) == false){	// user doesn't exist => exit program
			
					printf("User %s  doesn't exist ==> continue \n", sarg1);
					continue;
				}

				/* Find Node of receiver */
				Node<Wallet>* node1 = UserList->head;
		
				while( node1 != NULL){
			
					if( strcmp( node1->data->WalletID, sarg1) == 0 ){
			
						printf(" Found our Node , breaking out of loop (4) \n");
					
						break;			
					}
			
					node1 = node1->next;		
				}
		
				if( node1 == NULL){	// if  our node doesn't exist ===> ERROR
		
					printf(" ERROR , SOMETHING IS WRONG HERE !!!! (3)");
			
				
					delete senderHT;
					delete receiverHT;
			
					delete UserList;
					delete TransList;
					delete RootList;
			
					delete TransactionHT;
			
					return -1;	
		
				}
				
				/* Check if user1 has enough money, check the balance */
	
		
				if( amount <= 0){	// check if the transaction amount is invalid
		
					printf("The transaction amount is invalid ( <= 0) ===> continue to next transaction \n");
					continue;		
				}
	
	
				if( node->data->amount < amount){	// user1 can't pay
		
					printf("User1 can't complete this transaction , not enough $$$ \n");
					continue;		
				}
	
				// user1 has enough $$$ ====> OK


				/* Additional check : date not before the "last" transaction */ 

				time_t timestamp = 0;

				if( assignments == 5){	// date,time  given ==> create timestamp ==> have to check
			
					char* buffer = NULL;
				
				
				
					int k = strlen(tim);
					tim[k] ='\0';		//replace the ;
				
				
					int length1 = snprintf(buffer,0,"%s %s",date,tim) + 1;
					buffer = new char[length1];
					snprintf(buffer,length1,"%s %s",date,tim);
				
				
					struct tm vtm = {0};
	
					if( strptime(buffer, "%d-%m-%Y %H:%M",&vtm) != NULL){
	
						timestamp =	mktime(&vtm);	
			
					}else{ 
		
						printf(" Error in strptime() (2) \n\n"); 
			
			
						delete senderHT;
						delete receiverHT;
			
						delete UserList;
						delete TransList;
						delete RootList;
			
						delete TransactionHT;
			
						return -1;
					}
				
				
					delete[] buffer;
					buffer = NULL;
				
					//now check if it was after the last one
				
					if( timestamp < last_trans){		// before last transaction ==> IGNORE
				
						printf(" Current transaction is before last one ==> ignoring your request \n");
						continue;				
					}
					
					//else , OK
				
					last_trans = timestamp;		// updating last_transaction timestamp
			
				}else if( assignments == 3){	// date,time not given ==> timestamp = current time { time(NULL) } ==> no need to check
			
					timestamp = time(NULL);		// current date and time
					
					last_trans = timestamp;
				
				} 
			


		
		
				printf("\n\nPassed the transaction checks ====> creating transaction \n\n");
		
				/* We start by creating new TransactionID
					We have kept a maxID counter , so we can easily find new IDs */
				
				maxID = maxID + 1;
			
				int length1 = snprintf(NULL,0,"%d",maxID) +1;
				char transactionId[length1];
				snprintf(transactionId,length1,"%d", maxID);
				
				//Add new transactionId to the TransactionHT
				TransactionHT->AddRecord1(transactionId);
						
		
				/* Now we create the transaction */
		
				Transaction* transaction;
		
				/* Creating Transaction (and all associated nodes ) */
		
				transaction = new Transaction(transactionId, sarg, sarg1, amount, timestamp);
			
				TLNode* tlnode = new TLNode( transaction);
			
				UserTLNode* Utlnode = new UserTLNode( tlnode);
			
				/////////////////////////////////////////////////
						
				TransList->AddNode(tlnode);	//add the tlnode to the general transaction list
			
			
				/* Check if sender, receiver exist in the HashTables 
			
					If yes, update their contents */ 
			
				struct Records* record = new struct Records();
			
				senderHT->SearchRecord(sarg,&record);
			
				if( record == NULL){	// a record of user1 doesn't exist
			
					record = new struct Records();	//create it again (previous one was deleted )
			
					record->wallet = node->data;		
			
					record->list = new List<UserTLNode>();
			
					record->list->AddNode(Utlnode);
				
					// Add the record to HashTable
					senderHT->AddRecord(record);
				
					//we don't need record anymore
					
					record->wallet = NULL;
					record->list = NULL;
				
					delete record;				
					record = NULL;
				
				}else{	//a record of user1 exists
			
					record->list->AddNode(Utlnode);
				
					//Update/overwrite record in HashTable (don't need to)
					//senderHT->UpdateRecord(record);
				
				
					//we don't need record anymore
				
					record->wallet = NULL;
					record->list = NULL;
					
					delete record;				
					record = NULL;							
				}
			
				////////////////////////////////////			
			
				/* Now for receiver */
			
				printf("ReceiverHT \n");
			
				printf("receiver is %s , sender is %s \n", sarg1, sarg);
			
				record = new struct Records();
			
				receiverHT->SearchRecord(sarg1,&record);
			
				if( record == NULL){	// a record of user1 doesn't exist
			
					record = new struct Records();	// create it again (because it was previously deleted)
				
					record->wallet = node1->data;		
			
					record->list = new List<UserTLNode>();
			
					record->list->AddNode(Utlnode);
				
					// Add the record to HashTable
					receiverHT->AddRecord(record);
				
					record = NULL;
				
				}else{	//a record of user1 exists
			
					record->list->AddNode(Utlnode);
				
					record = NULL;
				
					//Update/overwrite record in HashTable
					//senderHT->UpdateRecord(record);			
				}
				
				/* HashTables are now updated */
			
			
			
			
				printf("Updated Hash tables ==> updating the rest \n");
			
			
				/* Need to update BitcoinTree(s) ,  Wallet->amount, Wallet->bitcoins(list) */
			
				/* Reminder: 
					node ==> user1/wallet node
					node1 ==> user2/wallet node
				
					node2 ==> node of user1 bitcoin list
					node3 ==> PTnode of user1(bitcoin node), PT = pointer to tree node => a node which contains a pointer to a tree node
				
				
				
				*/	
			
			
				/* Starting with Wallet  */
			
					/* user1/Wallet1 (node->data, was found previously) */
			
				node->data->amount = node->data->amount - amount;	//reducing overall balance of user1
			
				node1->data->amount = node1->data->amount + amount;	// increasing overall balance of user2
			
			
				int temp = amount;	//transaction amount
			
				Node<UserBNode>* node2 = node->data->bitcoins->head;
			
				while( (temp > 0 ) && ( node2 != NULL) ){	// traverse the bitcoin List of user1
			
			
					if( node2->data->value >= temp){	 // this bitcoin is more than enough to cover the amount (of transaction)
				
						node2->data->value = node2->data->value - temp;	//reducing the "value" of this bitcoin ( ex user1 now has 2$ in bitcoin 12 ) 
					
					
									
						Node<PTNode>* node3 = node2->data->current->head;	// traverse the PT List (pointers to tree nodes )
					
						while( (temp > 0) && ( node3 != NULL ) ){	
						
							printf("\n\n Calling Transaction() \n\n");
						
							if( node1->data == NULL) printf("\n\nERRROOOOORR !!!!!\n\n");
					
							node3->data->Transaction( &temp, node2->data->BitcoinID, node1->data, tlnode);	// we update the tree node, as well as create its children 
							// we also update the user2 (in the above function)
							
						
						
						
						
							if( node3->data->tnode == NULL){  // delete this as the value of tree node was used up
							
								Node<PTNode>* node4 = node3;
							
								node3 = node3->next;		// move to the next node
														
								// UserBNode-> current(PTNode list) -> DeleteNode();
								(node2->data)->current->DeleteNode(node4->data);		// delete node4 (actually ex-node3)
							
								node4 = NULL;
							
								continue;					
							}
							
										
							node3 = node3->next;
					
						}	//after this temp(transaction amount)  should be 0
										
						if( temp != 0){
					
							printf("SOMETHING IS WRONG (3) !!!!!!");
						
							delete senderHT;
							delete receiverHT;
			
							delete UserList;
							delete TransList;
							delete RootList;
				
							delete TransactionHT;
											
							return -1;
						}
					 
					
						/* If this bitcoin has no value (used it all up) */
						if( node2->data->value == 0){
					
							Node<UserBNode>* node4 = node2;
						
							node2 = node2->next;		// move to the next node
														
							// Wallet-> bitcoin(list) -> DeleteNode();
							(node->data)->bitcoins->DeleteNode(node4->data);		// delete node4 (actually ex-node3)
							
							node4 = NULL;
							
							continue;
						}
					
						node3 = NULL;
				
					}else{	// node2->data->value < temp ==> node2->data->value will be 0
				
						node2->data->value = 0;
								
						Node<PTNode>* node3 = node2->data->current->head;	// traverse the PT List (pointers to tree nodes )
					
						while( (temp > 0) && ( node3 != NULL ) ){	
					
							node3->data->Transaction( &temp, node2->data->BitcoinID, node1->data, tlnode);	// we update the tree node, as well as create its children 
							// we also update the user2 (in the above function)
							
						
							if( node3->data->tnode == NULL){  // delete this, as the value of tree node was used up ( will always be true )
							
								Node<PTNode>* node4 = node3;
							
								node3 = node3->next;		// move to the next node
														
								// UserBNode-> current(PTNode list) -> DeleteNode();
								(node2->data)->current->DeleteNode(node4->data);		// delete node4 (actually ex-node3)
							
								node4 = NULL;
							
								continue;					
							}
										
							node3 = node3->next;
					
						}	//after this temp(transaction amount)  should still be != 0
										
						if( node3 != NULL){	// we expect node3 == NULL
					
							printf("SOMETHING IS WRONG (4) !!!!!!");
										
							delete senderHT;
							delete receiverHT;
			
							delete UserList;
							delete TransList;
							delete RootList;
			
							delete TransactionHT;
						
							return -1;
						}
					 
					
						/* this bitcoin has no value (used it all up) ===> DELETE*/
						if( node2->data->value == 0){		// will always be true
					
							Node<UserBNode>* node4 = node2;
						
							node2 = node2->next;		// move to the next node
														
							// Wallet-> bitcoin(list) -> DeleteNode();
							(node->data)->bitcoins->DeleteNode(node4->data);		// delete node4 (actually ex-node3)
							
							node4 = NULL;
							
							continue;
						}
					
						node3 = NULL;
					
					}
				
				
					node2 = node2->next;
				}
				// finished  updating the structures (moving to the next part)
			
			
				if( temp > 0){	// loop finished ( with node2 = NULL ) 
				
					printf("ERROR temp must be 0");
											
					delete senderHT;
					delete receiverHT;
			
					delete UserList;
					delete TransList;
					delete RootList;
			
					delete TransactionHT;		
				
					return -1;
				}
			
			
				/* Updated hashTables and wallets( UserBNodes and UserBNodes->PTNodes ) and Trees */
				printf("\n\nFinished the requested transaction \n\n");
				transaction->Print();
		
		
				transaction = NULL;
				tlnode = NULL;
				Utlnode = NULL;
				
				node2 = NULL;
				node1 = NULL;
			
			}//reached EOF
	
			printf("\nFinished reading inputfile \n");
	
			/* Closing bitcoinBalanceFile */
	
			if( fclose(fp) == -1){
	
				perror("Closing TransactionFile: ");
				return -1; 
			}
			
			continue;		
		}
		
	}	






















		
	/******************************** /findEarnings ***********************************************************/	
		
		if( strcmp( token, "/findEarnings") == 0){
		
			printf("Before before line is %s\n", line);
			
			token = strtok(NULL," ,\n");	// walletID
			
			
			
	//		line = token; 
			
			
			
			
			
			/* Find Node of WalletID */
			Node<Wallet>* node = UserList->head;
		
			while( node != NULL){
		
				if( strcmp( node->data->WalletID, token) == 0 ){
			
					printf(" Found our Node , breaking out of loop (5) \n");
				
					break;			
				}
			
				node = node->next;		
			}
			
		
			if( node == NULL){	// if  our node doesn't exist ===> continue to the next
		
				printf("User %s doesn't exist, can't complete /findEarnings \n", token);

				continue;	
		
			}
			
			
			token = strtok(NULL,"");	// no space as a delimiter , because we want the rest of the string as is
			printf("token is %s", token);
			
	//		if( token 
			
			/* 4 scenarios from here on out:
				a) time1 and time2 is given ( if time1 exists => time2 MUST exist [the same for date1 and date2]  ) => ALL DATES, this time period
			
				b) date1 and date2		// (hour:minutes is at midnight 00:00 )
				
				c) time1,time2 and date1,date2	( timestamp1 <= timestamp <= timestamp2 )
				
				d) none of the above  ( all of the transactions as a receiver )
				
			*/
			
//			printf("before line is %s\n", line);		
//			line = line + strlen(token) + 3;	// get past the 1st token (causes segv)	
			
//			printf("line is %s\n", line);
//			printf("%s token ",token);
			
			
//			char* token1 = NULL;
/*			if( ( token1 = strtok(NULL,"\n") ) == NULL ){
			
				printf("EEEEERRRRROOOOOR\n\n\n");
				
				
				delete senderHT;
				delete receiverHT;
			
				delete UserList;
				delete TransList;
				delete RootList;
			
				delete TransactionHT;
				
				return -1;
			
			}
	
*/			
			if(token != NULL){

				assignments = sscanf(token,"%s %s %s %s ", arg,arg1,arg2,arg3);	// sscanf return how many sucessful assignments we got
				
			}else{
			
				assignments = 0;
			
			}
			
			// the only accepted values of assignment is 0,2,4 (for the rest =>  Error)
			
			/* (A or B) */
			if( assignments == 2){
			
				// the first assignment(arg) would be either time1 or date1
			
				if( ( strlen(arg) == 5) && ( arg[2] == ':')  ){	// time1 as arg
			
					/* (A) */
					
					// if time1 exists then time2 must also exist					
					
					if( ( strlen(arg1) == 5) && ( arg[2] == ':')  ){	// time2 as arg1
					
						struct tm tm1 = {0};		// initialise the struct
						tm1.tm_mday= 1;
						
						time_t t0 = 0;
						
						
						// create timestamp0
						if( strptime(arg,"%H:%M",&tm1) != NULL){
						
							t0 = mktime(&tm1);
						
						
						}else{
						
							printf("Something is wrong with strptime() (2) \n");
							printf("Aborting.... \n");
							continue;						
						}
						
						//create timestamp1
						time_t t1 = 0;
						
						if( strptime(arg1,"%H:%M",&tm1) != NULL){
						
							t1 = mktime(&tm1);
						
						
						}else{
						
							printf("Something is wrong with strptime() (3) \n");
							printf("Aborting.... \n");
							continue;						
						}
						
					
						
						/* Created the timestamps => Search for the transactions */
						
						
						struct Records *record = new struct Records();
						
						receiverHT->SearchRecord(node->data->WalletID, &record);
						
						if( record == NULL){
						
							printf(" Didn't find User %s record, in receiverHT \n", node->data->WalletID);
							printf("Overall earnings = 0$ \n");
							
							printf("On to the next one \n\n");
							
							continue; 					
						}
						// else , record != NULL
						
						Node<UserTLNode>* node = record->list->head;
						
						time_t temp = 0;
						
						int amount1 = 0;
						
						while( node != NULL){
						
							temp = ( (node->data)->transaction)->transaction->HourMinutes();		// UserTLNode->TLNode->Transaction->function()
						
							if( ( temp >= t0) && (temp <= t1) ){
							
								node->data->transaction->transaction->Print();		// print the Transaction
								
								amount1 = amount1 + node->data->transaction->transaction->value;
							}
							node = node->next;					
						}
						//finished with the transactions, printing overall amount
					
						printf("Overall amount is %d\n",amount1);
						

						// don't record anymore
						record->wallet = NULL;
						record->list = NULL;
				
						delete record;
						record = NULL;
				
						node = NULL;						
					
						continue;
									
					}else{	//ERROR (time2)
					
						printf("You gave time1 as the first argument, however time2 (second argument) was not given correctly ===> ignoring your request \n");
						
						continue;					
					}
					
				
				}else if( ( strlen(arg) == 10) && ( arg[2] == '-') && ( arg[5] == '-') ){	// date1 as arg
				
				
				
					/* (B) */
					
					// if date1 exists then date2 must also exist					
					
					if( ( strlen(arg1) == 10) && ( arg[2] == '-') && ( arg[5] == '-') ){	// date2 as arg1
					
						struct tm tm1 = {0};		// initialise the struct
//						tm1.tm_mday= 1;
						
						time_t t0 = 0;
						
						
						// create timestamp0
						if( strptime(arg,"%d-%m-%Y",&tm1) != NULL){
						
							t0 = mktime(&tm1);
						
						
						}else{
						
							printf("Something is wrong with strptime() (4) \n");
							printf("Aborting.... \n");
							continue;						
						}
						
						//create timestamp1
						time_t t1 = 0;
						
						if( strptime(arg1,"%d-%m-%Y",&tm1) != NULL){
						
							t1 = mktime(&tm1);
						
						
						}else{
						
							printf("Something is wrong with strptime() (5) \n");
							printf("Aborting.... \n");
							continue;						
						}
						
					
						
						/* Created the timestamps => Search for the transactions */
						
						
						struct Records *record = new struct Records();
						
						receiverHT->SearchRecord(node->data->WalletID, &record);
						
						if( record == NULL){
						
							printf(" Didn't find User %s record, in receiverHT \n",node->data->WalletID);
							printf("Overall earnings = 0$ \n");
							
							printf("On to the next one \n\n");
							
							continue; 					
						}
						// else , record != NULL
						
						Node<UserTLNode>* node = record->list->head;
						
						time_t temp = 0;
						
						int amount1 = 0;
						
						while( node != NULL){
						
							temp = ( (node->data)->transaction)->transaction->timestamp;		// UserTLNode->TLNode->Transaction->timestamp
							
							
//							printf("Hiiiiiiiiiii\n");
							
//							printf("\n temp is %ld \n", (long) temp);
//							printf("t0 is %ld and t1 is %ld \n\n", (long) t0, (long) t1);
							
						
							if( ( temp >= t0) && (temp <= t1) ){
							
								node->data->transaction->transaction->Print();		// print the Transaction
								
								amount1 = amount1 + node->data->transaction->transaction->value;
							}
							node = node->next;					
						}
						//finished with the transactions, printing overall amount
					
						printf("Overall amount is %d\n",amount1);
						
						
						// don't record anymore
						record->wallet = NULL;
						record->list = NULL;
				
						delete record;
						record = NULL;
				
						node = NULL;

						continue;

									
					}else{	//ERROR (date2)
					
						printf("You gave date1 as the first argument, however date2 (second argument) was not given correctly ===> ignoring your request \n");
						
						continue;					
					}
				
				
				
				
				}else{	// ERROR
				
					printf("Either date or time was not given correctly ==> ignoring your request .... \n");
					
					continue;				
				}
			
			
			}else if( assignments == 4){		// given date1,2 and time1,2  ( [time1] [date1] [time2] [date2] )
			
				/* we assume: arg = time1, arg1 = date1, arg2 = time2, arg3 = date2 */
				struct tm tm1 = {0};
				
				struct tm tm2 = {0};
				
				time_t t0 = 0;
				
				char* date1 = NULL;
				char* date2 = NULL;
				
				// Create the 1st "date"
			
				int length1 = snprintf(NULL,0,"%s %s", arg1, arg) + 1;
				date1 = new char[length1];
				snprintf(date1,length1,"%s %s", arg1, arg);
				
				
				//create timestamp0
				if( strptime(date1,"%d-%m-%Y %H:%M",&tm1) != NULL){
						
					t0 = mktime(&tm1);
						
						
				}else{
					
					printf("Something is wrong with strptime() (6) \n");
					printf("Aborting.... \n");
					continue;						
				}
			
				delete[] date1;	//don't need it anymore
				date1 = NULL;
			
			
				// Create the 2nd "date"
			
				time_t t1 = 0;
			
				length1 = snprintf(NULL,0,"%s %s", arg3, arg2) + 1;
				date2 = new char[length1];
				snprintf(date2,length1,"%s %s", arg3, arg2);
				
				
				//create timestamp1
				if( strptime(date2,"%d-%m-%Y %H:%M",&tm2) != NULL){
						
					t1 = mktime(&tm2);
						
						
				}else{
					
					printf("Something is wrong with strptime() (7) \n");
					printf("Aborting.... \n");
					continue;						
				}
			
				delete[] date2; // don't need it anymore
				date2 = NULL;
			
				/* Created the timestamps => Search for the transactions */
						
						
				struct Records *record = new struct Records();
				
				receiverHT->SearchRecord(node->data->WalletID, &record);
						
				if( record == NULL){
				
					printf(" Didn't find User %s record, in receiverHT \n", node->data->WalletID);
					printf("Overall earnings = 0$ \n");
							
					printf("On to the next one \n\n");
							
					continue; 					
				}
				// else , record != NULL
					
				Node<UserTLNode>* node = record->list->head;
						
				time_t temp = 0;
						
				int amount1 = 0;
						
				while( node != NULL){
					
					temp = ( (node->data)->transaction)->transaction->timestamp;		// UserTLNode->TLNode->Transaction->timestamp
					
//					printf("Hiiiiiiiiiii\n");
							
//					printf("\n temp is %ld \n", (long) temp);	
//					printf("t0 is %ld and t1 is %ld \n\n", (long) t0, (long) t1);
					
					
					
					
					
					
					
					if( ( temp >= t0) && (temp <= t1) ){
							
						node->data->transaction->transaction->Print();		// print the Transaction
								
						amount1 = amount1 + node->data->transaction->transaction->value;
					}
					node = node->next;					
				}
				//finished with the transactions, printing overall amount
				
				printf("Overall amount is %d\n",amount1);
				
				// don't record anymore
				record->wallet = NULL;
				record->list = NULL;
				
				delete record;
				record = NULL;
				
				node = NULL;
				
				
						
				continue;
					
			}else if( assignments == 0){	// no date or time given
			
				/* Immediately search for user Record in receiverHT (we will just print all of the transactions) */
			
				struct Records *record = new struct Records();
				
//				printf("node->data is %s \n", node->data->WalletID);
				
				receiverHT->SearchRecord(node->data->WalletID, &record);
						
				if( record == NULL){
				
					printf(" Didn't find User %s record, in receiverHT \n",node->data->WalletID);
					printf("Overall earnings = 0$ \n");
							
					printf("On to the next one \n\n");
							
					continue; 					
				}
				// else , record != NULL
					
				Node<UserTLNode>* node = record->list->head;
						
				time_t temp = 0;
						
				int amount1 = 0;
						
				while( node != NULL){
					
					node->data->transaction->transaction->Print();		// print the Transaction
								
					amount1 = amount1 + node->data->transaction->transaction->value;
				
					node = node->next;					
				}
				//finished with the transactions, printing overall amount
				
				printf("Overall amount is %d\n",amount1);
				
				// don't record anymore
				record->wallet = NULL;
				record->list = NULL;
				
				delete record;
				record = NULL;
				
				node = NULL;
				
						
				continue;
			
			}else{
			
				printf(" ERROR, the number of accepted arguments ( after walletID is 0,2,4) \n");
				
				continue;
			}
		
		}
		
		
		
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
		
	







	/******************************** /findPayment  ***********************************************************/	
		
		// almost the same as /findEarnings ( this time we will use senderHT )
		
		if( strcmp( token, "/findPayments") == 0){
		
			printf("Before before line is %s\n", line);
			
			token = strtok(NULL," ,\n");	// walletID
			
			
			
	//		line = token; 
			
			
			
			
			
			/* Find Node of WalletID */
			Node<Wallet>* node = UserList->head;
		
			while( node != NULL){
		
				if( strcmp( node->data->WalletID, token) == 0 ){
			
					printf(" Found our Node , breaking out of loop (6) \n");
				
					break;			
				}
			
				node = node->next;		
			}
			
		
			if( node == NULL){	// if  our node doesn't exist ===> continue to the next
		
				printf("User %s doesn't exist, can't complete /findEarnings \n", token);

				continue;	
		
			}
			
			
			token = strtok(NULL,"");	// no space as a delimiter , because we want the rest of the string as is
			printf("token is %s", token);
			
	//		if( token 
			
			/* 4 scenarios from here on out:
				a) time1 and time2 is given ( if time1 exists => time2 MUST exist [the same for date1 and date2]  ) => ALL DATES, this time period
			
				b) date1 and date2		// (hour:minutes is at midnight 00:00 )
				
				c) time1,time2 and date1,date2	( timestamp1 <= timestamp <= timestamp2 )
				
				d) none of the above  ( all of the transactions as a receiver )
				
			*/
			
//			printf("before line is %s\n", line);		
//			line = line + strlen(token) + 3;	// get past the 1st token (causes segv)	
			
//			printf("line is %s\n", line);
//			printf("%s token ",token);
			
			
//			char* token1 = NULL;
/*			if( ( token1 = strtok(NULL,"\n") ) == NULL ){
			
				printf("EEEEERRRRROOOOOR\n\n\n");
				
				
				delete senderHT;
				delete receiverHT;
			
				delete UserList;
				delete TransList;
				delete RootList;
			
				delete TransactionHT;
				
				return -1;
			
			}
	
*/			
			if(token != NULL){

				assignments = sscanf(token,"%s %s %s %s ", arg,arg1,arg2,arg3);	// sscanf return how many sucessful assignments we got
				
			}else{
			
				assignments = 0;
			
			}
			
			// the only accepted values of assignment is 0,2,4 (for the rest =>  Error)
			
			/* (A or B) */
			if( assignments == 2){
			
				// the first assignment(arg) would be either time1 or date1
			
				if( ( strlen(arg) == 5) && ( arg[2] == ':')  ){	// time1 as arg
			
					/* (A) */
					
					// if time1 exists then time2 must also exist					
					
					if( ( strlen(arg1) == 5) && ( arg[2] == ':')  ){	// time2 as arg1
					
						struct tm tm1 = {0};		// initialise the struct
						tm1.tm_mday= 1;
						
						time_t t0 = 0;
						
						
						// create timestamp0
						if( strptime(arg,"%H:%M",&tm1) != NULL){
						
							t0 = mktime(&tm1);
						
						
						}else{
						
							printf("Something is wrong with strptime() (2) \n");
							printf("Aborting.... \n");
							continue;						
						}
						
						//create timestamp1
						time_t t1 = 0;
						
						if( strptime(arg1,"%H:%M",&tm1) != NULL){
						
							t1 = mktime(&tm1);
						
						
						}else{
						
							printf("Something is wrong with strptime() (3) \n");
							printf("Aborting.... \n");
							continue;						
						}
						
					
						
						/* Created the timestamps => Search for the transactions */
						
						
						struct Records *record = new struct Records();
						
						senderHT->SearchRecord(node->data->WalletID, &record);
						
						if( record == NULL){
						
							printf(" Didn't find User %s record, in senderHT \n", node->data->WalletID);
							printf("Overall earnings = 0$ \n");
							
							printf("On to the next one \n\n");
							
							continue; 					
						}
						// else , record != NULL
						
						Node<UserTLNode>* node = record->list->head;
						
						time_t temp = 0;
						
						int amount1 = 0;
						
						while( node != NULL){
						
							temp = ( (node->data)->transaction)->transaction->HourMinutes();		// UserTLNode->TLNode->Transaction->function()
						
							if( ( temp >= t0) && (temp <= t1) ){
							
								node->data->transaction->transaction->Print();		// print the Transaction
								
								amount1 = amount1 + node->data->transaction->transaction->value;
							}
							node = node->next;					
						}
						//finished with the transactions, printing overall amount
					
						printf("Overall amount is %d\n",amount1);
						

						// don't record anymore
						record->wallet = NULL;
						record->list = NULL;
				
						delete record;
						record = NULL;
				
						node = NULL;						
					
						continue;
									
					}else{	//ERROR (time2)
					
						printf("You gave time1 as the first argument, however time2 (second argument) was not given correctly ===> ignoring your request \n");
						
						continue;					
					}
					
				
				}else if( ( strlen(arg) == 10) && ( arg[2] == '-') && ( arg[5] == '-') ){	// date1 as arg
				
				
				
					/* (B) */
					
					// if date1 exists then date2 must also exist					
					
					if( ( strlen(arg1) == 10) && ( arg[2] == '-') && ( arg[5] == '-') ){	// date2 as arg1
					
						struct tm tm1 = {0};		// initialise the struct
//						tm1.tm_mday= 1;
						
						time_t t0 = 0;
						
						
						// create timestamp0
						if( strptime(arg,"%d-%m-%Y",&tm1) != NULL){
						
							t0 = mktime(&tm1);
						
						
						}else{
						
							printf("Something is wrong with strptime() (4) \n");
							printf("Aborting.... \n");
							continue;						
						}
						
						//create timestamp1
						time_t t1 = 0;
						
						if( strptime(arg1,"%d-%m-%Y",&tm1) != NULL){
						
							t1 = mktime(&tm1);
						
						
						}else{
						
							printf("Something is wrong with strptime() (5) \n");
							printf("Aborting.... \n");
							continue;						
						}
						
					
						
						/* Created the timestamps => Search for the transactions */
						
						
						struct Records *record = new struct Records();
						
						senderHT->SearchRecord(node->data->WalletID, &record);
						
						if( record == NULL){
						
							printf(" Didn't find User %s record, in senderHT \n",node->data->WalletID);
							printf("Overall earnings = 0$ \n");
							
							printf("On to the next one \n\n");
							
							continue; 					
						}
						// else , record != NULL
						
						Node<UserTLNode>* node = record->list->head;
						
						time_t temp = 0;
						
						int amount1 = 0;
						
						while( node != NULL){
						
							temp = ( (node->data)->transaction)->transaction->timestamp;		// UserTLNode->TLNode->Transaction->timestamp
							
							
//							printf("Hiiiiiiiiiii\n");
							
//							printf("\n temp is %ld \n", (long) temp);
//							printf("t0 is %ld and t1 is %ld \n\n", (long) t0, (long) t1);
							
						
							if( ( temp >= t0) && (temp <= t1) ){
							
								node->data->transaction->transaction->Print();		// print the Transaction
								
								amount1 = amount1 + node->data->transaction->transaction->value;
							}
							node = node->next;					
						}
						//finished with the transactions, printing overall amount
					
						printf("Overall amount is %d\n",amount1);
						
						
						// don't record anymore
						record->wallet = NULL;
						record->list = NULL;
				
						delete record;
						record = NULL;
				
						node = NULL;

						continue;

									
					}else{	//ERROR (date2)
					
						printf("You gave date1 as the first argument, however date2 (second argument) was not given correctly ===> ignoring your request \n");
						
						continue;					
					}
				
				
				
				
				}else{	// ERROR
				
					printf("Either date or time was not given correctly ==> ignoring your request .... \n");
					
					continue;				
				}
			
			
			}else if( assignments == 4){		// given date1,2 and time1,2  ( [time1] [date1] [time2] [date2] )
			
				/* we assume: arg = time1, arg1 = date1, arg2 = time2, arg3 = date2 */
				struct tm tm1 = {0};
				
				struct tm tm2 = {0};
				
				time_t t0 = 0;
				
				char* date1 = NULL;
				char* date2 = NULL;
				
				// Create the 1st "date"
			
				int length1 = snprintf(NULL,0,"%s %s", arg1, arg) + 1;
				date1 = new char[length1];
				snprintf(date1,length1,"%s %s", arg1, arg);
				
				
				//create timestamp0
				if( strptime(date1,"%d-%m-%Y %H:%M",&tm1) != NULL){
						
					t0 = mktime(&tm1);
						
						
				}else{
					
					printf("Something is wrong with strptime() (6) \n");
					printf("Aborting.... \n");
					continue;						
				}
			
				delete[] date1;	//don't need it anymore
				date1 = NULL;
			
			
				// Create the 2nd "date"
			
				time_t t1 = 0;
			
				length1 = snprintf(NULL,0,"%s %s", arg3, arg2) + 1;
				date2 = new char[length1];
				snprintf(date2,length1,"%s %s", arg3, arg2);
				
				
				//create timestamp1
				if( strptime(date2,"%d-%m-%Y %H:%M",&tm2) != NULL){
						
					t1 = mktime(&tm2);
						
						
				}else{
					
					printf("Something is wrong with strptime() (7) \n");
					printf("Aborting.... \n");
					continue;						
				}
			
				delete[] date2; // don't need it anymore
				date2 = NULL;
			
				/* Created the timestamps => Search for the transactions */
						
						
				struct Records *record = new struct Records();
				
				senderHT->SearchRecord(node->data->WalletID, &record);
						
				if( record == NULL){
				
					printf(" Didn't find User %s record, in senderHT \n", node->data->WalletID);
					printf("Overall earnings = 0$ \n");
							
					printf("On to the next one \n\n");
							
					continue; 					
				}
				// else , record != NULL
					
				Node<UserTLNode>* node = record->list->head;
						
				time_t temp = 0;
						
				int amount1 = 0;
						
				while( node != NULL){
					
					temp = ( (node->data)->transaction)->transaction->timestamp;		// UserTLNode->TLNode->Transaction->timestamp
					
//					printf("Hiiiiiiiiiii\n");
							
//					printf("\n temp is %ld \n", (long) temp);	
//					printf("t0 is %ld and t1 is %ld \n\n", (long) t0, (long) t1);
					
					
					
					
					
					
					
					if( ( temp >= t0) && (temp <= t1) ){
							
						node->data->transaction->transaction->Print();		// print the Transaction
								
						amount1 = amount1 + node->data->transaction->transaction->value;
					}
					node = node->next;					
				}
				//finished with the transactions, printing overall amount
				
				printf("Overall amount is %d\n",amount1);
				
				// don't record anymore
				record->wallet = NULL;
				record->list = NULL;
				
				delete record;
				record = NULL;
				
				node = NULL;
				
				
						
				continue;
					
			}else if( assignments == 0){	// no date or time given
			
				/* Immediately search for user Record in senderHT (we will just print all of the transactions) */
			
				struct Records *record = new struct Records();
				
//				printf("node->data is %s \n", node->data->WalletID);
				
				senderHT->SearchRecord(node->data->WalletID, &record);
						
				if( record == NULL){
				
					printf(" Didn't find User %s record, in senderHT \n",node->data->WalletID);
					printf("Overall earnings = 0$ \n");
							
					printf("On to the next one \n\n");
							
					continue; 					
				}
				// else , record != NULL
					
				Node<UserTLNode>* node = record->list->head;
						
				time_t temp = 0;
						
				int amount1 = 0;
						
				while( node != NULL){
					
					node->data->transaction->transaction->Print();		// print the Transaction
								
					amount1 = amount1 + node->data->transaction->transaction->value;
				
					node = node->next;					
				}
				//finished with the transactions, printing overall amount
				
				printf("Overall amount is %d\n",amount1);
				
				// don't record anymore
				record->wallet = NULL;
				record->list = NULL;
				
				delete record;
				record = NULL;
				
				node = NULL;
				
						
				continue;
			
			}else{
			
				printf(" ERROR, the number of accepted arguments ( after walletID is 0,2,4) \n");
				
				continue;
			}
		
		}


/*************************** /walletStatus ***********************************/

		if( strcmp( token, "/walletStatus") == 0){
			
			token = strtok(NULL," ,\n");
		
	
//			bool found = false;
		
			Node<Wallet> *node = UserList->head;
			
			while( node != NULL){
			
				if( strcmp(node->data->WalletID, token) == 0) break;
			
			
				node = node->next;
			}

			if( node == NULL){
			
				printf("User %s doesn't exist \n", token);
				continue;		
			}
			//else user exists
			
			printf("User %s has currently %d$ in his wallet \n\n", token, node->data->amount);
			
			continue;
		}




/*************************** /bitCoinStatus ***********************************/

		if( strcmp( token, "/bitCoinStatus") == 0){

			token = strtok(NULL," ,\n");
			
			Node<RLNode>* node = RootList->head;
			
			while( node != NULL){
			
				if( strcmp( node->data->name, token) == 0) break;
				
				node = node->next;			
			}
			
			if( node == NULL){
			
				printf("Bitcoin %s doesn't exist \n", token);
				
				continue;		
			}
			
			// Count Transactions and unspent amount
			
			int counter = 0;
			int unspent = 0;
			
			HashTable1* TransIdHT = new HashTable1(senderHTnum, bucketSize);
			
			node->data->tree->root->TransCount( &counter, TransIdHT, true);
			node->data->tree->root->Unspent(&unspent);		
			
			
			delete TransIdHT;		// we don't need this anymore
			TransIdHT = NULL;
	
			printf("Bitcoin: %s, Original_Value: %d, Transactions: %d, Unspent: %d \n\n", token, node->data->tree->root->amount, counter, unspent);
			
			continue;
		}




/*************************** /traceCoin ***********************************/

		if( strcmp( token, "/traceCoin") == 0){

			token = strtok(NULL," ,\n");
			
			Node<RLNode>* node = RootList->head;
			
			while( node != NULL){
			
				if( strcmp( node->data->name, token) == 0) break;
				
				node = node->next;			
			}
			
			if( node == NULL){
			
				printf("Bitcoin %s doesn't exist \n",token);
				
				continue;		
			}

			HashTable1* TransIdHT = new HashTable1(senderHTnum, bucketSize);
			
			node->data->tree->root->TraceCoin(TransIdHT, true);
			
			delete TransIdHT;		// we don't need this anymore
			TransIdHT = NULL;
			
			printf(" Finished tracecoin \n\n");
			
			continue;			
		}


/*************************** /exit ***********************************/

		if( strcmp( token, "/exit") == 0) break;
		
		
		


		// if the first token is none of our "functions"
		printf("Please retry, don't recognise your input \n\n");

	}


























	/* Finishing up => freeing memory */

	printf("\nDeleting SenderHT \n");						
	delete senderHT;
	printf("\n");
	
	
	printf("Deleting ReceiverHT \n");
	delete receiverHT;
	printf("\n");	
	
	printf("Deleting the rest \n");	
		
	delete UserList;
	delete TransList;
	delete RootList;
			
	delete TransactionHT;
	
	delete[] bFile;
	delete[] tFile;
	
	
	printf("Returning .... \n");

	return 0;
}
