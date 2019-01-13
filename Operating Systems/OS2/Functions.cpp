

#include "MyStruct.hpp"
#include <cstdio>
#include <ctime>
#include <string.h>


using namespace std;





/* Node Functions */

	Node::Node() {}

	Node::Node(BRec rec1){

		rec.custid = rec1.custid;

		strcpy(rec.FirstName ,rec1.FirstName);
		strcpy(rec.LastName ,rec1.LastName);
		strcpy(rec.Street ,rec1.Street);

		rec.HouseID = rec1.HouseID;

		strcpy(rec.City ,rec1.City);
		strcpy(rec.postcode ,rec1.postcode);

		rec.amount = rec1.amount;

		next = NULL;




}


	Node::~Node(){

//		/* deleting the arrays */
//		delete[] rec.FirstName;
//		delete[] rec.LastName;
//		delete[] rec.Street;
//		delete[] rec.City;
//		delete[] rec.postcode;

		next = NULL;

	}

	BRec Node::getRecord(){ return rec; }
	Node* Node::getNext(){ return next; }

	void Node::ModNext(Node* new_next){

		next = new_next;
	}





/* List Functions */

	List::List() : head(NULL), tail(NULL){}

	List::~List(){

		Node* temp = head;

		while( head != NULL){

			head = head->getNext();
			delete temp;
			temp = head;
		}

		tail = NULL;

	}


	Node* List::getHead(){ return head; }
	Node* List::getTail(){ return tail; }



	void List::addRec(BRec rec){

		Node* temp = new Node(rec);

		if( head == NULL){

			head = temp;
			tail = head;
			temp = NULL;

		}else{

		tail->ModNext(temp);

		tail = temp;

		temp = NULL;
		}

	}











/* http://www.linux-mag.com/id/308/ */

int read_all(int fd, void* buffer, size_t count){

	int nbytes;
	int total = 0;

	char * buffer1 = (char *) buffer;	// void * ---> char * , and buffer1 contains address of buffer[0]

	while( count != 0 ) {

		nbytes = read(fd, buffer1, count);

		if( nbytes < 0){

			perror("Read Error in read_all() ");
			return -1;
		}

		buffer1 = buffer1 + nbytes;		//move the pointer
		count = count - nbytes;		// reduce count
		total = total + nbytes;		// increase the number of bytes read

	}

	return total;
}


int write_all(int fd, void* buffer, size_t count){

	int nbytes;
	int total = 0;

	char * buffer1 = (char *) buffer;	// void * ---> char * , and buffer1 contains address of buffer[0]

	while( count != 0 ) {

		nbytes = write(fd, buffer1, count);

		if( nbytes < 0){

			perror("Write Error in write_all() ");
			return -1;
		}

		buffer1 = buffer1 + nbytes;			//move the pointer
		count = count - nbytes;				//reduce count
		total = total + nbytes;				// increase the number of bytes written

	}
}






























