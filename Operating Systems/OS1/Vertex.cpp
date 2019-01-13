#include <iostream>
#include <string.h>

#include "MyClasses.hpp"

using namespace std;


	Vertex::Vertex(char* s){

		int length = strlen(s);

		name = new char[length+1];			// +1 because strlen() doesnt count the '\0' character

		strcpy(name,s);										// name  <- s
		inc = new EList();
		out = new EList();


////	cout << "Created a Vertex with name " << name;
		cout<< endl;

	}

/////////////////////////////////////////////////////////////////////////////////////////////////////

	Vertex::~Vertex() {

////		cout << "Deleting Vertex with name " << name << endl;

		delete[] name;

		delete inc;			//calling destructors of list
		delete out;



		name= NULL;
		inc =NULL;
		out = NULL;



////		cout << "Deleted Vertex with name" << name << endl;
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	char* Vertex::getName() const { return name; }			//accessors

	EList* Vertex::getInc() const { return inc; }
	EList* Vertex::getOut() const { return out; }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void Vertex::modInc(EList* n) { inc = n; }				//mutators
	void Vertex::modeOut(EList* n) { out = n; }



