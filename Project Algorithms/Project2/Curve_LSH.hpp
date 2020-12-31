#ifndef CURVE_LSH_H
#define CURVE_LSH_H


#include <iostream>
#include <string>
#include <cmath>

#include <random>

#include <vector>

#include <cassert>

#include "Vector.hpp"
#include "Curve.hpp"


#include "LSH_HT.hpp"
//#include "HyperCube.hpp"
#include "Vector_Node.hpp"

#include "Item.hpp"


/* This is the revised one, for the 2nd Programming exercise */


using namespace std;

class Curve_LSH{		// struct for our curves

	/* This is just a grid */

	private:
	
	int d;
	double delta;
		
	double* t;	// uniformly generated in R (dimension d) => [0,d)
	
	
	LSH_HT* ht;		// only one of them not NULL ( will be created after Constructor)
//	HyperCube* htc;
	
	vector<Curve*>* grid_curves;
	vector<Vector_Node*>* grid_points;
	
	vector<Vector*>* vector_cleanup;	// used to cleanup garbage vectors
	
	double max_c;
	double max_d;	
		
	public:
	Curve_LSH(int d,double del, mt19937 generator, uniform_real_distribution<double> dis);
	~Curve_LSH();
	
	/* Accessors */
	double* getT();
	LSH_HT* getHT();
//	HyperCube* getHTc();
	
	vector<Vector_Node*>* getGpoints();
	
	
	
	/* Modifiers */
	void setLSH_HT(int k1, double w1, mt19937 generator, uniform_real_distribution<double> dis);	// Create LSH Hash Table
	
//	void setHTc(int k1,int bh_d1, int M1, int probes1,double w1);	// Create HyperCube	
	
	/* Functions */
	
	// this is for input
	void Curve2Gcurve(vector<Curve*>* c);	// Done
	void Gcurve2Vector(vector<Curve*>* c);	// Done
	void Update();		// Done
	
	void Insert();
	
	/*For clustring */
	void UpdateVector_Node(vector<Item*>* items);
	
	
	
	
	//this is for queries	
	void Curve2Gcurve1(Curve* c, Curve** gr);
	// Real Curve --> Polygonal Curve
	
	void Gcurve2Vector1(Curve* gc, Vector** v);
	void Update1(Vector* v);
	
};

#endif
