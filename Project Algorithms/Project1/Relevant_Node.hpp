#ifndef RELEVANT_NODE_H
#define RELEVANT_NODE_H


#include <iostream>
#include <chrono>
#include <random>

#include <vector>
#include <utility>

#include "Vector.hpp"
#include "InfoCurve.hpp"

#include "Curve.hpp"
#include "Vector_Node.hpp"


#include "LSH_HT.hpp"
#include "HyperCube.hpp"



using namespace std;

class Relevant_Node{

	private:
	
		vector<pair<int,int>>* rt;	//relevant traversal

		int K;			// rows of G
		double** G;

		vector<Vector*>* cleanup;	// used for cleaning up
		vector<Vector_Node*>* vv;  // the Vector_Nodes that will be inserted into the Hash Table


		int HT_number;	// only for LSH 
		LSH_HT** ht; // an array of LSH hash Tables
		
		HyperCube* htc;	// only one of them not NULL
		
		bool LSH;

	public:
	
		Relevant_Node(vector<pair<int,int>>* rt1, double** G1, int K1, bool LSH);
		// bool ->Use LSH or HyperCube
		~Relevant_Node();
								
		void Insert(vector<Curve*>* vc, int k, int L);	//for LSH
		void Insert1(vector<Curve*>* vc, int k, int M, int probes);	// for HyperCube		
		
		void Search(Curve* query,InfoCurve* info);
		
		void CreateX(vector<Curve*>* vc);	// for input
		
		void CreateY(Curve* query,Vector_Node** result);	// for query
		
		void Multiply_Arrays(vector<double>* point, Vector** v);		
};

#endif
