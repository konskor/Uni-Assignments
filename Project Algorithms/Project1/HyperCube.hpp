#ifndef HYPER_CUBE_H
#define HYPER_CUBE_H

#include <iostream>
#include <string>
#include <chrono>
#include <random>

#include <vector>
#include <unordered_map>

#include "Vector.hpp"
#include "InfoItem.hpp"
#include "Vector_Node.hpp"

#include "InfoCurve.hpp"

using namespace std;

class HyperCube{

	private:
	
	int numbuckets;
	
	int d;		// dimension of vectors
	int k;		// how many h(x)
	
	int bh_d;	// 	d' = bh_d ==> dimension of hypercube
	
	int M;		// max items to be searched inside bucket 
	int probes;	// check neighbour buckets (hamming distance = 1)
	
	double w;
	
//	int count0;
//	int count1;
	
	
	// The s vector needed for each g(x)
	double*** s;	// s[i][j][l], ( 0 <= i <= d'-1, 0 <= j <= k-1, 0 <= l <= d-1)
	

	unordered_map<string,int>* index;	// Here we store our pairs of <string, 0 or 1>
	vector<Vector_Node*>** buckets;	// array of pointers to vectors(STL) ==> which have pointers to Vectors(class)
	

	/* For our random generator of 0 and 1 */
	mt19937 gen;
	uniform_int_distribution<int> dis;


	public:
	
		HyperCube(int k1,int d1,int bh_d1, int M1, int probes1, double w1);
		~HyperCube();
		
		int getnumBuckets();	//Accessors
		int getD();
		int getK();
		int getBh_d();
		
//		double*** getS();	//testing
//		unordered_map<string,int>* getIndex();	//testing
		
		
		int binary();	// create 0 or 1 (uniformly)
		

		/* Functions */		
		string Hash(Vector* x);		// find bucket
	
		void Insert(vector<Vector*> *v);
		void Insert1(vector<Vector_Node*> *v);	// for curves	
			
		
		int getBucket(string g);	
	
	
		void Hamming_Distance(vector<string>* vs, string str, int i, int remaining);
		void Search( Vector* query, InfoItem* info);
		void Search1( Vector_Node* query, InfoCurve* info);	//for Curves		
	
		
		
		/* Testing */
		int count0;	//testing
		int count1;
		
		double*** getS();	//testing
		unordered_map<string,int>* getIndex();	//testing
		vector<Vector_Node*>** getVector();
		
};




















#endif
