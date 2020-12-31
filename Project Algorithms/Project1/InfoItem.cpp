#include <iostream>
#include <string>

#include "Vector.hpp"
#include "InfoItem.hpp"

/* Constructor/Destructor */

InfoItem::InfoItem(string q1, string NN1, double True_d1, double True_t1){

	q = q1;
	NN_name_True = NN1;
	
	True_d = True_d1;
	True_t = True_t1;
	
	LSH_t = -1;	// dummy values ==> will update (after running LSH)
	LSH_d = -1;	
}


/* Used only for big datasets , when I can't run brute force to compute w */
InfoItem::InfoItem(string q1){

	q = q1;
	
	True_d = -1;
	True_t = -1;
	
	LSH_t = -1;
	LSH_d = -1;
}



InfoItem::~InfoItem(){}	//nothing to do

/* Accessors */

string InfoItem::getQ(){ return q;}
string InfoItem::getNN(){ return NN_name;}
string InfoItem::getNN_True(){ return NN_name_True;}

double InfoItem::getLSH_d(){ return LSH_d;}
double InfoItem::getLSH_t(){ return LSH_t;}

double InfoItem::getTrue_d(){ return True_d;}
double InfoItem::getTrue_t(){ return True_t;}

/* Modifiers */

void InfoItem::setLSH_d(double d1){ LSH_d = d1;}
void InfoItem::setLSH_t(double t1){ LSH_t = t1;}

void InfoItem::setLSH_NN(string s){ NN_name = s;} 



