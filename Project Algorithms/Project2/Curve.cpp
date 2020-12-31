#include <iostream>
#include <string>

#include "Curve.hpp"

using namespace std;


/* Constructor/Destructor */


Curve::Curve(string name1, vector< vector<double>* >* p1){

	name = name1;
	p = p1;	
	d = (p->at(0) )->size();
	
}



Curve::~Curve(){

	// Delete all vector2 of curve
	for( unsigned int i = 0; i < p->size(); i ++) delete p->at(i);		
	delete p;	//delete p

	p = NULL;	
}


/* Functions/Accessors */

string Curve::getName(){ return name;}

int Curve::getD(){	return d;}


int Curve::getSize(){ return p->size(); }	//how many points


//vector<double>* Curve::getP(){ return p;}
vector< vector<double>* >* Curve::getP(){ return p;}



