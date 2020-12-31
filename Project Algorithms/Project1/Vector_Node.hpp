#ifndef VECTOR_NODE_H
#define VECTOR_NODE_H

#include <iostream>
#include <string>

#include "Vector.hpp"
#include "Curve.hpp"

class Vector_Node{

	private:
	
	Vector* v;
	string g;
	
	Curve* real;		// used for LSH_curve, else NULL
	Curve* grid;
	
	public:
	
	Vector_Node(Vector* v1, string g1);
	Vector_Node(Vector* v1, Curve* r1, Curve* g1);
	
	~Vector_Node();
	
	Vector* getV();
	string getG();
	
	Curve* getReal();
	Curve* getGrid();
	
	
	void setG(string g1);
	
};

#endif
