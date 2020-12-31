#ifndef INFO_ITEM_H
#define INFO_ITEM_H

//#include <iostream>
#include <string>


using namespace std;

class InfoItem{		//a struct used for our output 

	private:
	
	string q;		//query name
	
	string NN_name_True;	// True Nearest neighbor name
	string NN_name;			// Approximate
	
	
	double LSH_d;	// distance computed by LSH
	double True_d;	// distance computed by Brute force
	
	double LSH_t;	// LSH time	==> will use timestamps
	double True_t;	// True time ==> will use timestamps
	
	
	public:
	
	/* In main, we will first run the brute force procedure ==> will fill all except LSH_d, LSH_t
	   (they will have dummy value of  0)
	   They will later be modified when we run the LSH algorithm 
	*/
	InfoItem(string q1, string NN1, double True_d1, double True_t1);
	InfoItem(string q);
	
	~InfoItem();

	string getQ();	// Accessors
	
	string getNN();		// get NN_name (approximate)
	string getNN_True();
	
	double getLSH_d();
	double getTrue_d();
	double getLSH_t();
	double getTrue_t();
	
	void setLSH_d(double d1); // Modifiers (only these 2 are needed)
	void setLSH_t(double d2);
	void setLSH_NN(string s);
	
	
};

#endif
