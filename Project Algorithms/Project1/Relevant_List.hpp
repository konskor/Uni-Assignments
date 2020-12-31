#ifndef RELEVANT_LIST_H
#define RELEVANT_LIST_H

#include <iostream>
//#include <string>
#include <chrono>
#include <random>

#include <vector>
#include <utility>

#include "Vector.hpp"
#include "InfoCurve.hpp"
#include "Curve.hpp"

#include "Relevant_Node.hpp"


using namespace std;

class Relevant_List{

	private:

		int length_i;
		int length_j;
		
//		double** G;	// the "normal" array (not used here)
// pass it to Relevant_Node
	
		vector<vector< pair<int,int> >*>* relevant_traversals;	// contains all relevant traversals

		vector<Relevant_Node*>* nodes;	// vector of Relevant nodes
	
	public:
	
		Relevant_List(int length_i1,int length_j1, double** G1,int K,bool LSH);
		~Relevant_List();
		
		int All_RT(vector<pair<int,int>>* current, vector<int>* limit, int i, int j);
		
		void Main_RT(vector<pair<int,int>>* diagonal, int i, int j, double l);
		
		void Insert(vector<Curve*>* vc, int k, int L);	//for LSH
		void Insert1(vector<Curve*>* vc, int k, int M, int probes);	// for HyperCube

		void Search(Curve* query,InfoCurve* info);
};


#endif
