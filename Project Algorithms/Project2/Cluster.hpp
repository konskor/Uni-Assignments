#ifndef CLUSTER_H
#define CLUSTER_H

#include <string>
#include <vector>

#include "Item.hpp"
#include "Vector.hpp"

#include "Utilities3.hpp"


class Item;	// forward declartion , because compiler was whining

using namespace std;

class Cluster{		// struct for our clusters

	private:

	Item* centroid;		// center of cluster
	int size;			// how many items inside cluster

	bool input;		// if centroid is existing point, or was created

	vector<Item*>* items;	//the Items that exist inside cluster

	vector<Item*>* items_2;	// items_2

	public:
	Cluster(Item* c, bool i);
	~Cluster();

	// Accessors
	Item* getC();	// get centroid
	int getSize();

	vector<Item*>* getItems();	// get all items (without centroid)
	
	void getAllItems(vector<Item*>* result);	// all the items (with centroid)
	
	bool getInput();	//if the centroid is an existing item of our input
	
	
	//Modifiers
	void Add_Item(Item* item,double distance);
	void Add_Item1(Item* item);
	void Add_Item2(Item* item,double distance);
	
	void Delete_Item(Item* item);
	
	void Commit_Assignments();
	void Commit_Assignments1(vector<Cluster*>* clusters);
	
	void EmptyCluster();
};

#endif
