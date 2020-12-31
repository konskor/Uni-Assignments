#include <iostream>
#include <string>

#include "Vector.hpp"
#include "InfoCurve.hpp"

/* Constructor/Destructor */

InfoCurve::InfoCurve(string q1, string NN1, double True_d1){

	q = q1;
	NN_name_True = NN1;
	
	True_d = True_d1;
//	True_t = True_t1;
	
//	LSH_t = -1;	// dummy values ==> will update (after running LSH)
	LSH_d = -1;	
}


/* Used only for big datasets , when I can't run brute force to compute NN */
InfoCurve::InfoCurve(string q1){

	q = q1;
	
	True_d = -1;
//	True_t = -1;
	
//	LSH_t = -1;
	LSH_d = -1;
}



InfoCurve::~InfoCurve(){}	//nothing to do

/* Accessors */

string InfoCurve::getQ(){ return q;}
string InfoCurve::getNN(){ return NN_name;}
string InfoCurve::getNN_True(){ return NN_name_True;}

//string InfoCurve::getMethod(){ return method;}
//string InfoCurve::getHF(){ return HF;}

double InfoCurve::getLSH_d(){ return LSH_d;}
//double InfoCurve::getLSH_t(){ return LSH_t;}

double InfoCurve::getTrue_d(){ return True_d;}
//double InfoCurve::getTrue_t(){ return True_t;}

/* Modifiers */

void InfoCurve::setLSH_d(double d1){ LSH_d = d1;}
//void InfoCurve::setLSH_t(double t1){ LSH_t = t1;}

void InfoCurve::setLSH_NN(string s){ NN_name = s;} 



