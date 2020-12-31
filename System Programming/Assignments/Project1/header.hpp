#include <stdio.h>
#include <time.h>

bool valid_transaction(char* transaction);		//INCOMPLETE

bool valid_Date_Time(char*);

int HashFunction(char*,int);

/* Bucket Functions( for class HashTable) */

void SearchBucket(unsigned char**, int, int, char*, struct Records**);

void Add2Bucket(unsigned char**, int, int, struct Records* );

void ClearBucket(char**, int, int);

//void UpdateBucket(char**,int, int, struct Records*);


/* Bucket Functions( for class HashTable1 ) */

void SearchBucket1(char**, int, int, char*, char** result);

void Add2Bucket1(char**, int, int, char* result);

void ClearBucket1(char**, int, int);


/* Extra functions */

bool isNumber(char*);



/* Extra List Functions *

bool UserExists( List<Wallet>*, char*);
bool BitcoinExists( List<RLNode>*, char*);

bool TransactionExists();	// INCOMPLETE

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



class Transaction{

	public:
	
	char* transactionID;
	
	char* senderWalletID;
	char* receiverWalletID;
	
	int value;
	
	/* maybe char* */
//	char* date;		// DD-MM-YY (day-month-year)
//	char* time;		// HH:MM ( hour : minutes )
	
	time_t timestamp;	//timestamp;
	
	/* Functions */
	
	Transaction(char*, char*, char*, int);		// without date and time
	
	Transaction(char*, char*, char*, int, char*, char*);		// with date and time(as strings)
	
	Transaction(char*, char*, char*, int, time_t );		// with date and time (as time_t)
	
	~Transaction();
	
	/////////////////////////////////////////
	
	void Print();	// Print Transaction
	
	time_t HourMinutes(); 	// return timestamp but only for Hour:Minute ==> 1/1/1970 HH:MM
	

};

/********** LIST OF ALL TRANSACTIONS *************************/


class TLNode{	// Node of Transaction List, this list contains ALL Transactions

	public: 
	
	/* Variables */
	
	Transaction* transaction;
	
//	TLNode* next;

	/* Functions */
	
	TLNode(Transaction*);
	~TLNode();

};






/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////


class HashTable1;		//needed?







class TNode{		// Bitcoin Tree Node

	public:
	
	/* Variables */
	char* user;
	
	int amount;
	
	TNode* left;	// receiver
	
	TNode* right;	// user with the new amount ( or NULL if we sent the whole amount)
	
	TLNode* node;	// NULL for all non-left children (root is also NULL)
	// this is a pointer to a node which contains the transaction we need
	
	
	/* IMPORTANT (implemented) */
	
	// idea: for each internal node( specifically left child node) , as well as the root, will have a pointer to a TLNode (Transaction List Node)
	// Each of these Nodes(with their children),  represent a transaction
	


	/* Functions */
	
	TNode(char*, int);				// constructor for everybody else
	TNode(char*,int,TLNode*);		// constructor for all the left children 
	
	~TNode();
	
	//void BitcoinStatus(int* , int*);
	
	void TransCount(int*, HashTable1*, bool);			// count how many transactions
	void Unspent(int*);
	
	void TraceCoin(HashTable1*, bool);
	
};





class Tree{

	public:
	
	/* Variables */
	
	TNode* root;
	
	/* Functions */
	
//	Tree(TNode*);

		
	Tree();		//empty Tree
	Tree(TNode*);		
	
	~Tree();
	
	// more functions later
	
};


/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////


/* Single Linked List Template and related Nodes */

template <class T>
class Node{

	public:
	
	/* Variables */
	
	T* data;
	Node* next;

	/* Functions */

	Node(T*);
	~Node();


};



template <class T>
class List{

	public:
	
	/* Variables */
	
//	T* head;
//	T* tail;

	Node<T>* head;
	Node<T>* tail;

	/* Functions */
	
	List();
	~List();
	
	void AddNode(T* temp);
	void DeleteNode(T* temp);	
	
};





/*

class PTNode{

	public:
	
	/* Variables *
	
	TNode* tnode;
	
//	PTNode* next;
	
	/* Functions *
	
	PTNode(TNode*);
	~PTNode();
	
	
//	void Transaction(int, UserBNode* , Transaction* );	
	void Transaction(int*, char*,  Wallet* , TLNode* );	
	// follow the pointer to the tree node and add the newly created Tree Node(left child) (as a result from a Transaction)
	// into user2->currentList (user2 = receiver) ( !user2->current! List of pointers to tree nodes )
	
	// also connect the left child with the assosciated transaction(TLNode)
	
	//Node<Transaction>* or Transaction* (same thing for UserBNode)
};

*/



















class PTNode;



























/* INCOMPLETE(check again) */

class UserBNode{	// Node of Bitcoin list ( only for wallet , for now)  

public:

	/* variables */

	char* BitcoinID;
	int value;			// current value of Bitcoin (might be lowered/increased after transaction) ===> a mini wallet
	
//	UserBNode* next;	// NULL this when destructor is called





	// might use a pointer to the Bitcoin Tree (maybe) => see what transactions the user has done with a certain Bitcoin
	
	//Tree* tree;		// might be the original tree or a sub-tree
	// doesn't really help us , we can do the same with hash table
	
	
	//2nd try
//	TNode* current;			// where we are currently in the Bitcoin Tree
							//we can easily add new TNodes, if a new transaction occurs
	// NULL this when destructor is called
	
	
	
	//3rd try
	
	List<PTNode> *current;		// a list of pointers to Tree Nodes	 ==> PTNode = Node which contains Pointer to a Tree Node (= TNode**)	
	
	//List<TNode*> *current;		// which nodes can be "broken" into two
	
	// "Active" ( for future use) Tree Nodes asossiated with  User's bitcoin
	
	// where we are currently in the Bitcoin Tree
	//we can easily add new TNodes, if a new transaction occurs
	// NULL this when destructor is called
	
	
	
	
	
	/* functions */
	
	UserBNode(char*,int,PTNode*);
	~UserBNode();

};




class UserTLNode{	// Node of Transaction list assosciated with User ( transaction list of bucket)

	public:
	
	/* Variables */
	
	TLNode* transaction;	// pointer to Transaction List Node which cointains all the information about this transaction
	
//	List<PTNode> *nodes;	// List of pointers to Tree Nodes , assosciated in this transaction
	
//	UserTLNode* next;
	
	
	
	/* Functions */
	
	//UserTLNode(TLNode*, List<PTNode> *);
	
	UserTLNode(TLNode*);
	
	~UserTLNode();

};




class RLNode{		// Root List Node => list of (name,pointers) to the root of each Bitcoin Tree 

	public:
	
	/* Variables */
	
	char* name;
	
	//TNode* tnode;
	
	Tree* tree;
	
//	RLNode* next;
	
	/* Functions */
	
	RLNode(char*,TNode*);
	
	~RLNode();
};






/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////


class Wallet{

public:

	/* Variables */
	
	char* WalletID;				// userID

	int amount;					// overall amount in wallet, amount = numOfBitcoins * BitcoinValue
	
	List<UserBNode> *bitcoins;			// list of bitcoins => Node will have ( char* , Node* next)
	
	int numOfBitcoins;			// how many bitcoins (currently)
	
	
	int BitcoinValue;			// value of a single Bitcoin (might not need)
	
//	Wallet* next;


	
	/* Functions */
	
	//Wallet(char*, BList*, int value );		// constructor
	
		
	Wallet(char*, List<UserBNode>* , int value );		// constructor => wallet has something inside ( Don't know if i will use this )
	
	Wallet(char*, int);		// constructor => EMPTY wallet
	
	
	
	~Wallet();								//destructor
	
};


/********** LIST OF WALLETS/USERS *************************/




class PTNode{

	public:
	
	/* Variables */
	
	TNode* tnode;
	
//	PTNode* next;
	
	/* Functions */
	
	PTNode(TNode*);
	~PTNode();
	
	
//	void Transaction(int, UserBNode* , Transaction* );	
	void Transaction(int*, char*,  Wallet* , TLNode* );	
	// follow the pointer to the tree node and add the newly created Tree Node(left child) (as a result from a Transaction)
	// into user2->currentList (user2 = receiver) ( !user2->current! List of pointers to tree nodes )
	
	// also connect the left child with the assosciated transaction(TLNode)
	
	//Node<Transaction>* or Transaction* (same thing for UserBNode)
};

















/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////


struct Records{		// structure inside a Bucket ====> 16 bytes ( sizeof pointer =  8 )

	/* REMEMBER: the last 8 bytes(of bucket)  are pointer to the next bucket ( if needed)
	
		The pointer to next bucket is (char*)
		
	 */

	/* Variables */
	Wallet* wallet;		// pointer to the wallet of user id
	
	List<UserTLNode>* list;		// list of "transactions" by user ( more on this later) 
	
	//UserTL* list;		// list of "transactions" by user ( more on this later) 

	
};




/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////













class HashTable{	// HT for senderHT and receiverHT

	public:

	/* Variables */	
	unsigned char** bucket;		// an array of pointers to byte array (bucket/block)

	int numOfBuckets;
	
	int numOfRecords;		// number of Records inside bucket
	
	int bucket_size;

	/* Functions */
	
	HashTable(int,int);
	~HashTable();
	
	void AddRecord(struct Records*);
	void SearchRecord(char*, struct Records**);
	
	void Print();
	
//	void UpdateRecord(struct Records*);
	
	
	
	

};







class HashTable1{	// HT for transactionID availability

	public:

	/* Variables */	
	unsigned char** bucket;		// an array of pointers to byte array (bucket/block)

	int numOfBuckets;
	
	int numOfRecords;		// number of Records inside bucket
	
	int bucket_size;

	/* Functions */
	
	HashTable1(int,int);
	~HashTable1();
	
	void AddRecord1(char*);
	void SearchRecord1(char*, char**);

};

























/*
class Records{		// structure inside a Bucket ====> 16 bytes ( sizeof pointer =  8 )

	/* REMEMBER: the last 8 bytes(of bucket)  are pointer to the next bucket ( if needed)
	
		The pointer to next bucket is (char*)
		
	 *

	public:

	/* Variables *
	Wallet* wallet;		// pointer to the wallet of user id
	
	List<UserTLNode>* list;		// list of "transactions" by user ( more on this later) 
	
	//UserTL* list;		// list of "transactions" by user ( more on this later) 
	
	/* Functions *
	Records(Wallet*);
	~Records();
	
};
*/









/* Extra List Functions */

bool UserExists( List<Wallet>*, char*);
bool BitcoinExists( List<RLNode>*, char*);

bool TransactionExists();	// INCOMPLETE



























