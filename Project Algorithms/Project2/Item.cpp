#include <iostream>

#include "Item.hpp"





using namespace std;

/* Constructor/Destructor */

Item::Item(Curve* curve2): curve1(true), c(curve2), v(NULL), cluster(NULL), dist(-1),cluster_2(NULL), dist_2(-1) , assigned(false){}

Item::Item(Vector* v1): curve1(false), c(NULL), v(v1), cluster(NULL), dist(-1), cluster_2(NULL), dist_2(-1), assigned(false){}


Item::~Item(){

//	v = NULL;		// don't delete them here 
//	curve = NULL;	// ( will be deleted from the original vector<> => main)

	if( v != NULL) delete v;
	if( c != NULL) delete c;
	
	v = NULL;
	c = NULL;

	cluster = NULL;
}


/* Accessors */

bool Item::isCurve(){ return curve1;}

Curve* Item::getCurve(){ return c; }
Vector* Item::getVector(){ return v;}

Cluster* Item::getCluster(){ return cluster;}

Cluster* Item::get_2nd_Cluster(){ return cluster_2; }




double Item::getDist(){ return dist;}	// distance from cluster

bool Item::isAssigned(){ return assigned; }

bool Item::isCentroid(){

	bool result = false;
	
	if( cluster == NULL) return result;
	
	Item* centroid = cluster->getC();
	
	if( centroid == this) result = true;	// this item is a centroid
	centroid = NULL;
	
	return result;
}


void Item::ChoosePoints(vector< vector<double>* >* C, int* array, int size){

	// copy the points ==> avoid having pointers to existing points of curve

	// if we dont want copies, comment  the 4 line of code and uncomment last one

	vector< vector<double>* >* points = c->getP();


	for(int i = 0; i < size; i++){
	
		int index = array[i];
		
		//comment these if needed
		vector<double>* coordinates = new vector<double>;
		*coordinates = *(points->at(index) );
		
		C->push_back( coordinates );
		coordinates = NULL;
		
//		C->push_back( points->at(index) );
	}

	points = NULL;
	


}




/* Functions */

//void Item::ChangeCluster(Cluster* c){ cluster = c; }

void Item::ChangeCluster(Cluster* c, double distance){ 

	
	if( distance == -1){
/*****************************************************
	If the cluster we are assigned to is :
		a) destroyed
		b) emptied ( break assignments)
******************************************************/


		assert(c == NULL);
		
		cluster = c;
		dist = distance;
		
		cluster_2 = NULL;
		dist_2 = -1;
		return;
	}
	
//	Else, we move the current cluster, to 2nd_best cluster field


	cluster_2 = this->cluster;
	dist_2 = dist;
	
	cluster = c;
	dist = distance;
}

void Item::Change_Assigned(bool c){ assigned = c; }



//Changed
void Item::Change_2nd(Cluster* c, double distance){

	cluster_2 = c;
	dist_2 = distance;
}



/*
void Item::ChangeCluster(Cluster* c, double distance){ 

	cluster = c; 
	dist = distance;
}
*/









