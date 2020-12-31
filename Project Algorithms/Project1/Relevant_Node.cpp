#include <iostream>
#include <string>
#include <fstream>

#include <chrono>
#include <random>

#include <cmath>
#include <ctime>
#include <cassert>

#include <vector>
#include <utility>

#include "Vector.hpp"
#include "InfoItem.hpp"
#include "Utilities.hpp"
#include "Utilities2.hpp"
#include "Curve.hpp"

#include "Relevant_Node.hpp"

using namespace std;

Relevant_Node::Relevant_Node(vector<pair<int,int>>* rt1, double** G1,int K1, bool LSH1){

	rt = rt1;
	
	K = K1;
	G = G1;
	
	LSH = LSH1;

	vv = new vector<Vector_Node*>;
	cleanup = new vector<Vector*>;
}




Relevant_Node::~Relevant_Node(){

	delete rt;
	G = NULL;
	
	if( vv != NULL){
		for( unsigned int i = 0; i < vv->size(); i++) delete vv->at(i);
		delete vv;	
	}
	
	if( cleanup != NULL){
		for( unsigned  int i = 0; i < cleanup->size(); i++) delete cleanup->at(i);
		delete cleanup;	
	}
	
	if( ht != NULL){
		for( int i = 0; i< HT_number; i++) delete ht[i];
		delete[] ht;	
	}
	
	if( htc != NULL) delete htc;
}



void Relevant_Node::Insert(vector<Curve*>* vc, int k, int L){	//for LSH

	/* Create the x Vectors */	
	HT_number = L;
	
	this->CreateX(vc);
	assert( vc->size() == vv->size() );
	
	
	double w = Compute_w_curve(vv);
	int numbuckets = vv->size()/8;
	
	int d = vv->at(0)->getV()->getC()->size();
	
	
	/* Create LSH_HT and insert the vv */
	ht = new LSH_HT*[L];	
	for(int i = 0; i < L; i++){
	
		random_device rd;
		mt19937 gen(rd());
	
		uniform_real_distribution<double> dis(0,w);
		ht[i] = new LSH_HT(numbuckets,k,d,w,gen,dis);
		
		ht[i]->Insert1(vv);	
		
	}

}




void Relevant_Node::Insert1(vector<Curve*>* vc, int bh_d, int M, int probes){// for HyperCube		

	/* Create the x Vectors */
	this->CreateX(vc);
	
	assert( vc->size() == vv->size() );
	double w = Compute_w_curve(vv);

	int d = vv->at(0)->getV()->getC()->size();
	
	int k = 4;	// how many h(x)
	
	/* Create LSH_HT and insert the vv */
	
	htc = new HyperCube(k,d,bh_d,M,probes,w);
	
	htc->Insert1(vv);
}




void Relevant_Node::Search(Curve* query, InfoCurve* info){

	Vector_Node* v = NULL;
	this->CreateY(query,&v);
	
	
	if( ht != NULL){
	
		for(int i = 0; i < HT_number; i++) ht[i]->Search_NN1(v,info,HT_number);
		
	}else if( htc != NULL){
	
		htc->Search1(v,info);
	
	}else{
	
		cout << "ERROR in Relevant_Node::Search()\n";
	}

	delete v;
	v = NULL;

}


















void Relevant_Node::CreateX(vector<Curve*>* vc){

	for(unsigned int i = 0; i < vc->size(); i++){	// for all Curves
	
		vector<Vector*>* vv1 = new vector<Vector*>;

		// for all pairs(Relevant Traversal) ==> Create vector
		for(unsigned int j = 0; j < rt->size(); j++){
	
			int index = 0;
			index = rt->at(j).first;
			
			Vector* v = NULL;
			
			// Multiply the arrays result is in NULL
			this->Multiply_Arrays(vc->at(i)->getP()->at(index),&v);// result in v
			
			assert(v != NULL);
			
			vv1->push_back(v);
			v = NULL;
		}
		
		// Extract coordinates into a single vector
		
		vector<double>* c = new vector<double>;	//coordinates of X
		
		for( unsigned int j = 0; j < vv1->size(); j++){	// for all resulting vectors
		
			assert( vv1->size() > 0);
			assert(vv1->at(j) != NULL);
		
			vector<double>* c1 = vv1->at(j)->getC();
		
			// concatenate coordinates of the resulting vetors into X
			for(unsigned int l = 0; l < c1->size(); l++) c->push_back(c1->at(l));		
		
		}	// Finished concatenating => Create Vector
		
		string s = "a";
		Vector* v = new Vector(s,c);
		Vector_Node* v1 = new Vector_Node(v,vc->at(i),NULL);	// NULL for no Grid Curve
		
		
		
		vv->push_back(v1);
		cleanup->push_back(v);	// to delete later
		
		
		/* Delete the resulting vectors/points from Multiply_Array() */ 
		for(unsigned int i = 0; i < vv1->size() ; i++) delete vv1->at(i);
		delete vv1;
		vv1 = NULL;		
		
		c = NULL;
		v = NULL;
		v1 = NULL;		
	}
}




/* Almost same as CreateX() */
void Relevant_Node::CreateY(Curve* query, Vector_Node** result){
	
	vector<Vector*>* vv1 = new vector<Vector*>;
	
	// for all pairs(Relevant Traversal) ==> Create vector
	for(unsigned int j = 0; j < rt->size(); j++){
	
		int index = 0;
		index = rt->at(j).second;
			
		Vector* v = NULL;
		this->Multiply_Arrays(query->getP()->at(index),&v);
			
		vv1->push_back(v);
			
		v = NULL;
	}
	// Extract coordinates into a single vector
		
	vector<double>* c = new vector<double>;	//coordinates of Y
		
	for( unsigned int j = 0; j < vv1->size(); j++){	// for all resulting vectors
		
		vector<double>* c1 = vv1->at(j)->getC();
		
		// concatenate coordinates of the resulting vetors into Y
		for( unsigned int l = 0; l < c1->size(); l++) c->push_back(c1->at(l));		
		
	}	// Finished concatenating => Create Vector
		
	string s = "a";
	Vector* v = new Vector(s,c);
	*result = new Vector_Node(v,query,NULL);	// NULL for no Grid Curve

	cleanup->push_back(v);	// to delete later
		
		
	/* Delete the resulting vectors/points from Multiply_Array() */ 
	for(unsigned int i = 0; i < vv1->size() ; i++) delete vv1->at(i);
	delete vv1;
	vv1 = NULL;		
		
	c = NULL;
	v = NULL;	
}



void Relevant_Node::Multiply_Arrays(vector<double>* point, Vector** v){

	vector<double>* c = new vector<double>;	// the coordinates of new Vector v
	
	int d = point->size();
	
	assert(d == 2);

	for(int i = 0; i < K; i++){		// for each row(i) of array G
	
		double sum = 0;
	
		for( int j = 0; j < d; j++){ // pick all elements
		
			double temp = G[i][j] * point->at(j);
			sum = sum + temp;		
		}
	
		c->push_back(sum);	
	}

	//Finished with the coordinates create Vector
	string s = "a";
	*v = new Vector(s,c);
	
	c = NULL;
}



















