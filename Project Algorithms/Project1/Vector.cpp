#include <iostream>
#include <string>

#include "Vector.hpp"

/* Constructor/Destructor */


Vector::Vector(string name1, vector<double>* c1){

	name = name1;
	c = c1;
	
	d = c->size();
}



Vector::~Vector(){

	delete c;	//call destructor for vector	
	c = NULL;
}

/* Functions/Accessors */

string Vector::getName(){ return name;}

int Vector::getD(){	return d;}

vector<double>* Vector::getC(){ return c;}

void Vector::setC(vector<double>* c1){ c = c1; }

// Used in LSH_curve, when we need to pad
void Vector::setD(int d1){ d = d1;}


