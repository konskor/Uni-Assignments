#ifndef VECTOR_NODE_H
#define VECTOR_NODE_H

#include <iostream>
#include <string>

#include "Vector.hpp"
#include "Curve.hpp"


// From clustering
#include "Item.hpp"


class Vector_Node{

	private:
	
	Vector* v;
	string g;
	
	Curve* real;		// used for LSH_curve, else NULL
	Curve* grid;
	
	//This is an extra  (added for clustering)
	
	Item* item;
	
	public:
	
	Vector_Node(Vector* v1, string g1);
	Vector_Node(Vector* v1, Curve* r1, Curve* g1);
	
	// Constructor for Clustering
	Vector_Node(Vector* v1, Curve* r1, Curve* g1, Item* item1 );
	
	~Vector_Node();
	
	/* Accessors */
	Vector* getV();
	string getG();
	
	Curve* getReal();
	Curve* getGrid();
	
	Item* getItem();
	
	
	/* Modifiers */
	void setG(string g1);
	
	void setItem(Item* item);
	
};

#endif
