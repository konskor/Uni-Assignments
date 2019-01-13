#include <iostream>
#include <string.h>

#include "MyClasses.hpp"

using namespace std;

	ENode::ENode(Vertex* source, Vertex* dest,int weight){

		edge = new Edge(source,dest,weight);
		next = NULL;
	}

	ENode::ENode(Edge* e){

		edge = e;
		next = NULL;

}


//////////////////////////////////////////////////////////////

	ENode::~ENode(){

////		cout << "Destroying Edge Node\n";

		next = NULL;

		delete edge;				// call destructor of edge

	}

/////////////////////////////////////////////////////////////////

	Edge* ENode::getEdge() const { return edge; }				//accessors
	ENode* ENode::getNext() const { return next; }

/////////////////////////////////////////////////////////////////

	void ENode::modNext(ENode *n){ next = n; }					// mutators
	void ENode::modEdge(Edge *n){ edge = n; }
