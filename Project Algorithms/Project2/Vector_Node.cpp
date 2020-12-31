#include <iostream>
#include <string>

#include "Vector.hpp"
#include "Vector_Node.hpp"

/* For points */
Vector_Node::Vector_Node(Vector* v1, string g1){

	v = v1;
	g = g1;
	
	real = NULL;
	grid = NULL;
	
	item = NULL;
}


/* For curve */
Vector_Node::Vector_Node(Vector* v1, Curve* r1, Curve* g1 ){

	v = v1;
	real = r1;
	
	grid = g1;
	item = NULL;
}


/* Clustering */

Vector_Node::Vector_Node(Vector* v1, Curve* r1, Curve* g1, Item* item1 ){

	v = v1;
	real = r1;

	// only one of them is NOT NULL
//	if( (r1 != NULL) && (v1 != NULL) ) cout << "Error in constructor Vector_Node \n";

	grid = g1;
	item = item1;
}




Vector_Node::~Vector_Node(){ 

	v = NULL;
	real = NULL;
	grid = NULL;
	
	item = NULL;
}


Vector* Vector_Node::getV(){ return v;}
string Vector_Node::getG(){ return g;}

Curve* Vector_Node::getReal(){ return real;}
Curve* Vector_Node::getGrid(){ return grid;}

Item* Vector_Node::getItem(){ return item;}



void Vector_Node::setG(string g1){ g = g1;}

void Vector_Node::setItem(Item* item1){ item = item1;}


