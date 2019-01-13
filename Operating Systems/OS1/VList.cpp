#include <iostream>
#include <string.h>

#include "MyClasses.hpp"

using namespace std;


	VList::VList() : head(NULL) , tail(NULL) { 


		////cout << "Created an empty Vertex List\n"; 

	}

	VList::~VList() {

////		cout << "Destroying the Vertex List\n";


		if( head != NULL){

			VNode* temp = NULL;

			while( head->getNext() != NULL){				// delete all the nodes , except the last one

				temp = head;
				head = head->getNext();

				delete temp;
				temp = NULL;



			}

			delete head;				//deleting the last one
			head = NULL;
			tail = NULL;
		}

////		cout <<"Vertex List destroyed\n";
	}

///////////////////////////////////////

	VNode* VList::getHead() const {return head;}
	VNode* VList::getTail() const {return tail;}

///////////////////////////////////////

	void VList::addVertex(Vertex* V){

		VNode* V1 = new VNode(V);				//Create a new VNode which points to Vertice V

		if(head == NULL){						//Empty List

			head = V1;

			tail = head;

			V1 = NULL;

			return;
		}

		tail->modNext(V1);					// Adding it , to the end of the list

		tail = V1;							// Tail now points to the NEW end of list

		V1 = NULL;

	}





	void VList::addVNode(VNode* pv){

		if(head == NULL){						//Empty List

			head = pv;

			tail = head;

			return;
		}

		tail->modNext(pv);					// Adding it , to the end of the list

		tail = pv;							// Tail now points to the NEW end of list

	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void VList::deleteVNode(VNode *pv){			//delete the edge node , which *pv points to

		if( pv == NULL){

////			cout << "pv is NULL, abort deleteVNode\n";

			return;
		}


		if( head == NULL){

////			cout << "VList is empty\n";
			return;
		}




		/* Check if *pv = head */

		if ( pv == head ){				// delete head of list

			if( head->getNext() == NULL){					// only 1 node

				delete head;



				head = NULL;
				tail = NULL;

				return;
			}

			//More than 1 node

			VNode* temp = head;

			head = head->getNext(); 				// move to the next one

			delete temp;						//delete the node

			temp = NULL;

			return;
		}


		/* Check if *pv = tail */

		VNode *previous = head;

		VNode *current = head->getNext() ;



		if(pv == tail){								// delete the last node

			while( current->getNext() != NULL ){					// we expect at the end : current = tail; , previous ->next = current

				previous = current;

				current = current->getNext();

			}



			tail = previous;			// new Tail

			previous = NULL;

			tail->modNext(NULL);			// cutting the last node from the list

			delete current;					//deleting Node

			current = NULL;



			return;

		}


		/* The node is between head and tail */


		while(  current->getNext()  != tail ){				// traverse every single one node, between head and tail 

			if(current == pv){

				previous->modNext( current->getNext() );			// passing over current

				delete current;							// the next= NULL is being done by the destructor

				current = NULL;
				previous = NULL;

				return;

			}

			previous = current;
			current = current->getNext();

		}

		current = NULL;
		previous = NULL;

////		cout << "Error"<< endl;

	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////


	void VList::findVNode(char* s, VNode** temp){

		*temp = head;

		while(*temp != NULL){

			if( strcmp( (*temp)->getVertex()->getName(), s) == 0){

////				cout << "Found the Vertex , FindVNode" <<endl;
				 return;

			}

			*temp = (*temp)->getNext();

		}
	}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void VList::updateVList(Vertex *pv){				//deletes VNodes without deleting vertices

		if( pv == NULL){

////			cout << "Pointer is NULL, abort updateVList" << endl;
			return;
		}

		/* Check if *pe = head */

		if ( pv == head->getVertex() ){				// NULL and then delete head of list

			if( head->getNext() == NULL){					// only 1 node

				head->modVertex(NULL);			// we dont want to delete the vertex
				delete head;

				head = NULL;
				tail = NULL;

				return;
			}

			//More than 1 node

			VNode* temp = head;

			head = head->getNext(); 				// move to the next one

			temp->modVertex(NULL);
			delete temp;						//delete the node

			temp = NULL;

			return;
		}


		/* Check if *pe = tail */

		VNode *previous = head;

		VNode *current = head->getNext() ;



		if(pv == tail->getVertex() ){								// delete the last node

			while( current->getNext() != NULL ){					// we expect at the end : current = tail; , previous ->next = current

				previous = current;

				current = current->getNext();

			}

			tail = previous;			// new Tail

			previous = NULL;

			tail->modNext(NULL);			// cutting the last node from the list

			current->modVertex(NULL);
			delete current;					//deleting Node

			current = NULL;

			return;

		}


		/* The node is between head and tail */


		while(  current->getNext()  != NULL ){				// traverse every single one node, between head and tail

			if(current->getVertex() == pv){

				previous->modNext( current->getNext() );			// passing over current

				current->modVertex(NULL);
				delete current;							// the next = NULL is being done by the destructor

				current = NULL;
				previous = NULL;

				return;

			}

			previous = current;
			current = current->getNext();

		}

		current = NULL;
		previous = NULL;


////		cout << "Didnt find the requested vertex"<< endl;

	}





//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool VList::Exist(Vertex* V){					//later used in DFS


		if( head == NULL) return false;

		VNode *temp = head;

		while( temp != NULL){

			if( temp->getVertex() == V) return true;

			temp = temp->getNext();

		}

			temp = NULL;

		return false;

	}






