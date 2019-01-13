#include <stdio.h>
#include <string.h>

#include "PriorityList.hpp"

/* Node */

Node::Node(char* s){

	strcpy(queue_type,s);
	next = NULL;

}

Node::~Node(){ next = NULL; }


/* PriorityList */

PriorityList::PriorityList(){

	head = NULL;
	tail = NULL;

	small = 0;		// members in priority Queue Small 
	medium = 0;		// members in priority Queue Medium
	large = 0;

	uSM = 0;		// members in priority Queue, of Small vessels willing to upgrade to Medium
	uSL = 0;
	uML = 0;

	uExit = 0;

}

PriorityList::~PriorityList(){

	Node* temp;

	while( head != NULL){

		temp = head;
		head = head->next;

		delete temp;

	}

	tail = NULL;
	temp = NULL;

}


void PriorityList::AddNode(char *s){

	Node *temp = new Node(s);

	if(head == NULL){

		head = temp;
		tail = head;
		temp = NULL;

	}else{	// head != NULL


		tail->next = temp;
		tail = temp;
		temp = NULL;

	}


}

void PriorityList::deleteNode(Node *n){

	if( n == NULL) return;


	if(head != NULL){

		Node *current = head->next;
		Node *previous = head;

		if( head == n){		//delete head

			head = current;
			delete previous;
			previous = NULL;
			current = NULL;

			return;

		}

		if( tail == n){		// delete tail

			while( current != tail){

				previous = current;
				current = current->next;

			}

			tail = previous;
			tail->next = NULL;

			delete current;
			current = NULL;
			previous = NULL;

			return;

		}

		// n is between head and tail

		while( current != n ){

			previous = current;
			current = current->next;
		}


		previous->next = current->next;
		current->next = NULL;
		delete current;
		current = NULL;
		previous = NULL;


	}
}









