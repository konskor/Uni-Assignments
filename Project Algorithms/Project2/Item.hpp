#ifndef ITEM_H
#define ITEM_H

#include <iostream>
#include <string>

#include <cassert>
#include <vector>

#include "Vector.hpp"
#include "Curve.hpp"

#include "Cluster.hpp"

class Cluster;	// forward declaration because compiler is complaining again



using namespace std;

class Item{		// struct for the items of our future clusters ( vectors or curves)

	private:

	bool curve1;	// if the item is a Curve (all items must have same value)
	
	Curve* c;	// 1 of these must be NULL
	Vector* v;
	
	Cluster* cluster;	// where do we belong ( originally NULL)
	double dist;	// distance from cluster
	
	Cluster* cluster_2;	// for silhouette (2nd best cluster)
	double dist_2;
	
	
	// reverts to false, when a cluster is deleted or we have new clusters have appeared 
	bool assigned;	// "commit" that the item belongs to this cluster
	// mostly used by assignment 2

	public:
	
	/* Constructors/Destructor */
	Item(Curve* c);	// 2 constructors ( Vector or Curve)
	Item(Vector* v);
	
	~Item();

	/* Accessors */
	bool isCurve();

	Curve* getCurve();
	Vector* getVector();
	
	Cluster* getCluster();
	
	Cluster* get_2nd_Cluster();	// get 2nd best cluster
	
	double getDist();
	
	bool isAssigned();
	
	bool isCentroid();
	
	// Used in Update2 ==> Curves
	void ChoosePoints(vector< vector<double>* >* C, int* array, int size);
	
	

	/* Modifiers */
	
//	void ChangeCluster(Cluster* c);

	void ChangeCluster(Cluster* c,double dist);

	void Change_Assigned(bool c);
	
	void Change_2nd(Cluster* c, double dist);

};

#endif
