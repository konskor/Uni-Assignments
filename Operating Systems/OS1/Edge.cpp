#include <iostream>
#include <string.h>



#include "MyClasses.hpp"

using namespace std;


	Edge::Edge(Vertex* s,Vertex* d,int w) : source(s), dest(d), weight(w) {}

/////////////////////////////////////////////////////

	Edge::~Edge(){
		////cout << "Edge destroyed " << endl;

		source = NULL;
		dest = NULL;

	}

////////////////////////////////////////////////////

	Vertex* Edge::getSource() const {return source;}			//accessors

	Vertex* Edge::getDest() const {return dest;}

	int Edge::getWeight() const{ return weight;}

/////////////////////////////////////////////////

	void Edge::modWeight(int w){ weight = w; }				//mutators
