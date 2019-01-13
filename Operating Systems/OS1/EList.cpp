#include <iostream>
#include <string.h>

#include "MyClasses.hpp"

using namespace std;


	EList::EList() : head(NULL) , tail(NULL) {

////		cout << "Created an empty Edge List\n";

	}

	EList::~EList() {

////		cout << "Destroying the Edge List\n";

		if( head != NULL){ 

			ENode* temp = NULL;

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

////		cout <<"Edge List destroyed\n";
	}

///////////////////////////////////////

	ENode* EList::getHead() const {return head;}				// accessors
	ENode* EList::getTail() const {return tail;}

///////////////////////////////////////

	void EList::addEdge(Vertex* v1, Vertex* v2, int w){

		ENode* temp = new ENode(v1,v2,w);				// Creating a new Node

		if(head == NULL){


			head = temp;						// head now points to the new node
			tail = head;						// because we have 1 node, head=tail

			temp = NULL;

////			cout << "Inserted	|"<< v1->getName() <<"| -> "<< w <<" -> |"<< v2->getName() <<"|"<< endl;

			return;
		}




		tail->modNext(temp);					// Adding it , to the end of the list

		tail = temp;							// Tail now points to the NEW end of list

		temp = NULL;							// we dont need temp anymore

////		cout << "Inserted	|"<< v1->getName() <<"| -> "<< w <<" -> |"<< v2->getName() <<"|"<< endl;


	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////




	void EList::addENode( Edge* temp){

		ENode* temp1 = new ENode(temp);

		if(head == NULL){


			head = temp1;						// head now points to the new node
			tail = head;						// because we have 1 node, head=tail

////			cout << "Inserted	|"<< temp1->getEdge()->getSource()->getName() <<"| -> "<< temp1->getEdge()->getWeight() <<" -> |"<< temp1->getEdge()->getDest()->getName() <<"|"<< endl;

			temp1 = NULL;

			return;
		}

		tail->modNext(temp1);					// Adding it , to the end of the list

		tail = temp1;							// Tail now points to the NEW end of list

////		cout << "Inserted	|"<< temp1->getEdge()->getSource()->getName() <<"| -> "<< temp1->getEdge()->getWeight() <<" -> |"<< temp1->getEdge()->getDest()->getName() <<"|"<< endl;

		temp1 = NULL;


	}

	void EList::addENode( ENode* temp){

		if(head == NULL){


			head = temp;						// head now points to the new node
			tail = head;						// because we have 1 node, head=tail

////			cout << "Inserted	|"<< temp->getEdge()->getSource()->getName() <<"| -> "<< temp->getEdge()->getWeight() <<" -> |"<< temp->getEdge()->getDest()->getName() <<"|"<< endl;

			return;
		}

		tail->modNext(temp);					// Adding it , to the end of the list

		tail = temp;							// Tail now points to the NEW end of list

////		cout << "Inserted	|"<< temp->getEdge()->getSource()->getName() <<"| -> "<< temp->getEdge()->getWeight() <<" -> |"<< temp->getEdge()->getDest()->getName() <<"|"<< endl;


	}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////


	void EList::updateEList(Edge *pe){				//complementary function used when Deleting an Edge or Vertex 

		/* Delete the ENode , without deleting the Edge */

		if( pe == NULL){

////			cout << "Pointer is NULL, abort updateEList" << endl;
			return;
		}

		//head->getEdge();

		//cout << "Del-vertex |" << pe->getSource()->getName() << "|-> " << pe->getWeight() << "-> |" << pe->getDest()->getName() << "|" << endl;



		/* Check if *pe = head */

		if( head == NULL){

////			cout << " List is empty return" <<endl;
			return;

		}


		if ( head->getEdge() == pe ){				// NULL and then delete head of list



			if( head->getNext() == NULL){					// only 1 node



				head->modEdge(NULL);			// we dont want to delete the edge
				delete head;



				head = NULL;
				tail = NULL;

				return;
			}

			//More than 1 node



			ENode* temp = head;

			head = head->getNext(); 				// move to the next one

			temp->modEdge(NULL);
			delete temp;						//delete the node

			temp = NULL;

			return;
		}


		/* Check if *pe = tail */

		ENode *previous = head;

		ENode *current = head->getNext() ;



		if(pe == tail->getEdge() ){								// delete the last node

			while( current->getNext() != NULL ){					// we expect at the end : current = tail; , previous->getNext() = current

				previous = current;

				current = current->getNext();

			}

			tail = previous;			// new Tail

			previous = NULL;

			tail->modNext(NULL);			// cutting the last node from the list

			current->modEdge(NULL);
			delete current;					//deleting Node

			current = NULL;

			return;
		}


		/* The node is between head and tail */


		while(  current->getNext()  != NULL ){				// traverse every single one node, between head and tail



			if(current->getEdge() == pe){

				previous->modNext( current->getNext() );			// passing over current

				current->modEdge(NULL);
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


////		cout << "Didnt find the requested edge"<< endl;

	}





	void EList::prepDelete(Vertex *pv){			//before deleting Inc/Out edges , we have to update the list of the other node

		/* Nulls the pointers (of the oposite vertex), who point to the same edge as pe, and then removes them from the list */


		// Example: V1->100->V2 , 	1)We access V1				2) Through Edge(100) go to V2
		//							3) Search V2->Inc() for the ENode who points to the Edge *pe 			4)Delete ENode without deleting the edge (yet)
		//							5) Go back to V1, find the edge *pe  points to 			5) Now We delete ENode and edge


		//Basically, updateEList does step 3 and 4


		ENode *temp  = head;




		/* Nulls the pointers (of the oposite vertex), who point to the same edge as pe, and then removes them from the list */

		while(temp != NULL){



			if( temp->getEdge()->getSource() == pv ){			// Outgoing Edges

				temp->getEdge()->getDest()->getInc()->updateEList( temp->getEdge() );					// Similar to DeleteENode, we find the ENode and instead of deleting both ENode and Edge
																										//we delete only the ENode,leaving the edge unaffected,and removing the Enode from the Elist

				temp = temp->getNext();

				continue;
			}

			// if it doesnt enter the if statement ==> Incoming Edge

			temp->getEdge()->getSource()->getOut()->updateEList( temp->getEdge() );						// Similar to DeleteENode, we find the ENode and instead of deleting both ENode and Edge
																										//we delete only the ENode,leaving the edge unaffected,and removing the Enode from the Elist

			temp = temp->getNext();

		}
	}







	void EList::prepDelete1(Vertex *v1, Vertex* v2){			//complementary function of DeleteEdges, same as above ( with a few modifications)

		/* Nulls the pointers (of the oposite vertex), who point to the same edge as pe, and then removes them from the list */


		// Example: V1->100->V2 , 	1)We access V1				2) Through Edge(100) go to V2
		//							3) Search V2->Inc() for the ENode who points to the Edge *pe 			4)Delete ENode without deleting the edge (yet)
		//							5) Go back to V1, find the edge *pe  points to 			5) Now We delete ENode and edge


		//Basically, updateEList does step 3 and 4


		ENode *temp  = head;



		/* Nulls the pointers (of the oposite vertex), who point to the same edge as pe, and then removes them from the list */

		while(temp != NULL){



			if( ( temp->getEdge()->getSource() == v1 ) && (temp->getEdge()->getDest() == v2 ) ) {			// Outgoing Edges, with dest = v2 ==> v1->..->v2

				temp->getEdge()->getDest()->getInc()->updateEList( temp->getEdge() );					// Similar to DeleteENode, we find the ENode and instead of deleting both ENode and Edge
																										//we delete only the ENode,leaving the edge unaffected,and removing the Enode from the Elist

				temp = temp->getNext();

				continue;
			}

			// if it doesnt enter the if statement ==> Incoming Edge

			if( ( temp->getEdge()->getSource() == v2 ) && (temp->getEdge()->getDest() == v1 ) ) {			// Incoming Edges, with dest = v1 ==> v2->..->v1

				temp->getEdge()->getDest()->getInc()->updateEList( temp->getEdge() );					// Similar to DeleteENode, we find the ENode and instead of deleting both ENode and Edge
																										//we delete only the ENode,leaving the edge unaffected,and removing the Enode from the Elist

				temp = temp->getNext();

				continue;
			}

			/* We ignore the edges that do not enter the above if statements */


			temp = temp->getNext();

		}
	}










//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void EList::deleteENode(Edge *pe){			//delete the edge node , which *pe points to

		if( pe == NULL){

////			cout << "Pointer is NULL, abort deleteNode" << endl;
			return;
		}


		//cout << "Del-vertex |" << pe->getSource()->getName() << "|-> " << pe->getWeight() << "-> |" << pe->getDest()->getName() << "|" << endl;




		/* Check if *pe = head */

		if ( pe == head->getEdge() ){				// delete head of list

			if( head->getNext() == NULL){					// only 1 node

				delete head;

				head = NULL;
				tail = NULL;

				return;
			}

			//More than 1 node

			ENode* temp = head;

			head = head->getNext(); 				// move to the next one

			delete temp;						//delete the node

			temp = NULL;

			return;
		}


		/* Check if *pe = tail */

		ENode *previous = head;

		ENode *current = head->getNext() ;



		if(pe == tail->getEdge() ){								// delete the last node

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


		while(  current->getNext()  != NULL ){				// traverse every single one node, between head and tail

			if(current->getEdge() == pe){

				previous->modNext( current->getNext() );			// passing over current

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


////		cout << "Didnt find the requested edge"<< endl;

	}

////////////////////////////////////////////////////////////////////////////////////

	void EList::findENode(VNode* v1,VNode* v2, int w, ENode **temp){

		*temp = head;

		while(*temp != NULL){

			if( ( (*temp)->getEdge()->getSource()  == v1->getVertex() ) && ( (*temp)->getEdge()->getDest() == v2->getVertex() ) && ( (*temp)->getEdge()->getWeight() == w ) ){

////				cout << "Found the ENode , FindENode" << endl;
				return;
			}

			*temp = (*temp)->getNext();

		}
	}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	void EList::Print(){				//Print List of Edges (EdgeNodes->Edges)

		ENode* temp = head;

		while(temp != NULL){

			cout << "|"<< temp->getEdge()->getSource()->getName() << "|->"<< temp->getEdge()->getWeight() << "->|" << temp->getEdge()->getDest()->getName() << "|" << endl<<"\t\t";

			temp = temp->getNext();

		}

		cout << endl;
	}


	void EList::Print1(){				//Print List of Edges (EdgeNodes->Edges), complementary function of circlefind

		ENode* temp = head;

		while(temp != NULL){

			cout << "|"<< temp->getEdge()->getSource()->getName() << "|->"<< temp->getEdge()->getWeight() << "->";


			if ( temp->getNext() == NULL){

				cout << "|"<< temp->getEdge()->getDest()->getName() << "|" << endl;

			}

			temp = temp->getNext();


		}

	}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool EList::Exist(Edge* pe){					//later used in DFS

		ENode *temp = head;

		while( temp != NULL){

			if( temp->getEdge() == pe) return true;

			temp = temp->getNext();

		}

		temp = NULL;

		return false;

	}


