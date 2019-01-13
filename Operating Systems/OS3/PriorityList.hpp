#include <stdio.h>
#include <string.h>


class Node{

	public:

	char queue_type[6];		// which queue to wake up
	Node* next;

	Node(char *s);
	~Node();


};

class PriorityList{

/* This PriorityList is used to keep the "FCFS and available" , while handling the priority/failed applications */

/* Whenever an application doesn't get approved, the vessel blocks in its own SIZE priority queue

	priority queue = semaphore


	Example: A Small-NonUpgradable vessel will block in the "Small Priority Queue"
			A Medium-Upgradable vessel will block in the "uML Priority Queue"

Because we scatter the applications into multiple priority queues, we need to keep somewhere the order of the priority applications

And this Priority List, created locally in "port-master" will solve our problems


*/



	public:

	Node* head;
	Node* tail;

	int small;		// members in priority Queue Small 
	int medium;		// members in priority Queue Medium
	int large;

	int uSM;		// members in priority Queue, of Small vessels willing to upgrade to Medium
	int uSL;
	int uML;

	int uExit;

	/* Functions */

	PriorityList();
	~PriorityList();

	void AddNode(char*);
	void deleteNode(Node *);


};


