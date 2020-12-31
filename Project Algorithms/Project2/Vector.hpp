#ifndef VECTOR_H
#define VECTOR_H

#include <string>
#include <vector>

using namespace std;

class Vector{		// struct for our points

	private:
	
	string name;
	int d;			// dimension

	vector<double>* c;
	
	public:
	

	Vector(string name1, vector<double>* c1);

	~Vector();

	string getName();	// Accessors
	int getD();
	vector<double>* getC();
	
	void setC(vector<double>* c1);	// Modify Coordinates
	
	void setD(int d);
	
	
};

#endif
