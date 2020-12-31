#ifndef LSH_HT_H
#define LSH_HT_H

#include <iostream>
#include <string>
#include <chrono>
#include <random>

//#include <list>
#include <vector>

#include "Vector.hpp"
#include "InfoItem.hpp"
#include "Vector_Node.hpp"
#include "InfoCurve.hpp"



using namespace std;

//double or float?


/* Hash Table, that can be used for both LSH and hypercube */

class LSH_HT{		// Hash Table for LSH

	// num of buckets?? do we choose? ==> n/4 or n/8 or n/16 ( we choose) ==> n/8 finally
	// actually 2^32 (really big???) => n/8

	private:
	
	int numbuckets;
	
//	list or vector? ==> vector

//	vector<Vector*> **buckets; // array of pointers -> vector of pointers to vectors(points)
	
	vector<Vector_Node*> **buckets;
	// array of pointers -> vector of pointers to vector_Nodes(points)
	

	int k;		// number of "secondary" hash functions
	int d;		// dimension
	

	/* For our hash function --> hi(x) */
	
	double w;
//	int w;			//for the Euclidean space		


	// s will be our "t" in Euclidean Space (or maybe not)
	double **s;		// array of float, size = k x d ==> [s11,s12,...,s1d]
					//										...
					//								  [sk1,sk2,....,skd]
					
//	double *v;		// the normal distributed vector ==> Euclidean space(later)

	public:	
	LSH_HT( int nbuckets, int k1, int d1, double w1, mt19937 generator, uniform_real_distribution<double> dis);
	
	~LSH_HT();

	int getnumBuckets();
	int getK();
	int getD();
	
	double** getS();	//might delete later
	
//	void Manhattan_LSH(double w, std::default_random_engine generator, uniform_real_distribution<double> dis );	
	void Manhattan_LSH(double w, mt19937 generator, uniform_real_distribution<double> dis);

//	int getBucket(Vector x);		// our "g(x)" ==> which bucket

//	void Insert(Vector* x);		// Insert vector to our hash table
	void Insert(vector<Vector*> *v);
	void Insert1(vector<Vector_Node*> *v);	// for Curve


//	int Hash(Vector* x);		// our "g(x)" ==> which bucket
	string Hash(Vector* x);		// our "g(x)" (not the bucket)

	int getBucket(string g);
	
//	void Search_NN( vector<Vector*>* queries, vector<InfoItem*>* item);
	void Search_NN( Vector* query, InfoItem* info, int L );	
	void Search_NN1( Vector_Node* query, InfoCurve* info, int L );	// for curves
	
	//Testing
	vector<Vector_Node*>** getB();
	
	

};

#endif
