#ifndef CURVE_H
#define CURVE_H

#include <string>
#include <vector>

#include "Vector.hpp"

using namespace std;

class Curve{		// struct for our curves

	private:
	
	string name;
	int d;			// dimension of points
	
	vector< vector<double>* >* p;	// pointer to vector1 of pointers--> vector2<double>
	
	/* The above is a pointer to a vector1<pointers to vectors2> (vector1).
	   vector1 contains pointers to vectors2<double>
	   
	   p ---> vector1[][][][][][]
	   				 |
	   				 |
	   				 vector2
	  double<--------[]
	   				 []
	   				 []
	   				 
	   vector2 contains double (not pointer)
	*/
	
	
	
	public:
	Curve(string name1, vector< vector<double>* >* p1);
	~Curve();

	string getName();	// Accessors
	int getD();

	vector< vector<double>* >* getP();
};

#endif
