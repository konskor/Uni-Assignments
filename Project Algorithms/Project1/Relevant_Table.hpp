#ifndef RELEVANT_TABLE_H
#define RELEVANT_TABLE_H

#include <iostream>
#include <chrono>
#include <random>

#include <vector>

#include "Vector.hpp"
#include "InfoCurve.hpp"

#include "Relevant_List.hpp"
#include "Curve.hpp"


using namespace std;

class Relevant_Table{

	private:
	
		int M;
		Relevant_List*** table;	// MxM table with pointers to relevant_list objects (not lists)
	
		double** G;	// created from normal distribution
		// Create it here and then pass it to Relevant Nodes (objects)
		int K;

		double e;	// used to build G and search for neighbors

		int window;	// when query comes check nearby cells (up and down)

	public:
	
		Relevant_Table(int M,int d,int window,double e,bool LSH);	// bool ->Use LSH or HyperCube
		~Relevant_Table();
		
		void Insert(vector<Curve*>* vc, int k, int L);	//for LSH
		void Insert1(vector<Curve*>* vc, int k, int M, int probes);	// for HyperCube
		
		void Search(Curve* query,InfoCurve* info);
		
		
		void Categorise(vector<Curve*>* vc,  vector<Curve*>** result);
		// result is an array , each cell(i) points to a vector with Curvves of length(i)

};

#endif
