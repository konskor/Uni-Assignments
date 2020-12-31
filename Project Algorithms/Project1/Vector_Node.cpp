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
}


/* For curve */
Vector_Node::Vector_Node(Vector* v1, Curve* r1, Curve* g1 ){

	v = v1;
	real = r1;
	
	grid = g1;
}





Vector_Node::~Vector_Node(){ 

	v = NULL;
	real = NULL;
	grid = NULL;
}


Vector* Vector_Node::getV(){ return v;}
string Vector_Node::getG(){ return g;}

Curve* Vector_Node::getReal(){ return real;}
Curve* Vector_Node::getGrid(){ return grid;}


void Vector_Node::setG(string g1){ g = g1;}


