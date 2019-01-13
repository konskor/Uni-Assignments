#include <iostream>
#include <string.h>

#include "MyClasses.hpp"


using namespace std;


	VNode::VNode(char* name){

		////cout<<"Creating a new Vertex Node" << endl;

		vertex = new Vertex(name);
		next = NULL;
	}




	VNode::VNode(Vertex* V){

		////cout<<"Creating a new Vertex Node" << endl;

		vertex = V;
		next = NULL;
}


//////////////////////////////////////////////////////////////

	VNode::~VNode(){

		////cout << "Destroying Vertex Node \n";

		next = NULL;
		delete vertex;
	}

/////////////////////////////////////////////////////////////////

	Vertex* VNode::getVertex() const { return vertex; }					//accessors
	VNode* VNode::getNext() const { return next; }

/////////////////////////////////////////////////////////////////

	void VNode::modNext(VNode *n){ next = n; }					// mutators
	void VNode::modVertex(Vertex* v){ vertex = v; }



