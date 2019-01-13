
#include <cstdio>
#include <ctime>
#include <unistd.h>

using namespace std;

int read_all(int fd, void* buffer, size_t count);
int write_all(int fd, void* buffer, size_t count);


typedef struct BinRecord{
	long  	custid;
	char 	FirstName[20];
	char 	LastName[20];
	char	Street[20];
	int 	HouseID;
	char	City[20];
	char	postcode[6];
	float  	amount;
} BRec;


typedef struct StatRec{	// contains for searchers

		/* Searchers */
		double search_min;		// double or long

		double search_max;

		double search_average;

		/* Mergers_splitters */

		double split_min;
		double split_max;
		double split_average;

} SRec;

class Node{


	BRec rec;
	Node* next;

	public:

	Node();
	Node(BRec rec1);

	~Node();

	BRec getRecord();
	Node* getNext();

	void ModNext(Node* new_next);

};



class List{

	Node* head;
	Node* tail;

	public:

	List();
	~List();

	void addRec(BRec rec);
	Node* getHead();
	Node* getTail();

};







