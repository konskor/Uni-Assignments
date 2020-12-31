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
#include "Utilities.hpp"
#include "Utilities2.hpp"
#include "Curve.hpp"

#include "Relevant_Node.hpp"
#include "Relevant_List.hpp"

using namespace std;

Relevant_List::Relevant_List(int length_i1,int length_j1,double** G,int K,bool LSH){

	length_i = length_i1;
	length_j = length_j1;
	
	
//	cout << "Hello from Relevant List constructor\n";
	
	this->relevant_traversals = new vector<vector< pair<int,int> >*>;
	this->nodes = new vector<Relevant_Node*>;
		
	/* Create all relevant traversals through a recursive function */
	
	
	vector<pair<int,int>>* diagonal = new vector<pair<int,int>>;
	double l = 1.0 * length_j / length_i;
	

//	cout << "Length_i is " << length_i << " Length_j  is " << length_j <<endl;

	this->Main_RT(diagonal,0,0,l);
	
	vector<int>* limit = new vector<int>;

	assert( diagonal->size() > 0);

	int previous_i = -1;
	int previous_j = -1;
	
	
	for( unsigned int i = 0; i < diagonal->size(); i++){
	
		pair<int,int> temp = diagonal->at(i);
		
//		cout << temp.second << endl;

		if( temp.second > previous_j){	// changed column
//			cout<< "Inserting " << temp.first + 1 << endl;
	
//			limit->push_back(temp.first-1);
			limit->push_back(temp.first + 1);
			previous_j = temp.second;
		}
	
	}
	// Now limit has the max value (i) can take for each (j)	
	
	assert( limit->size() > 0);
	
	vector<pair<int,int>>* current = new vector<pair<int,int>>;
	this->All_RT(current,limit,0,0);
	// the results will be in (this->relevant_traversals), aka a vector

	delete current;	// no longer need this vector
	current = NULL;

	delete limit;
	limit = NULL;

	cout << "Relevant_Traversals are : " << relevant_traversals->size() << endl;

	/* Now that we found all RTs, create the Relevant Nodes */

	this->nodes->reserve(relevant_traversals->size());

	for(unsigned int i = 0; i < this->relevant_traversals->size(); i++){
	//For each RT ==> create a Relevant_Node , with the RT as an "ID"
	
		vector< pair<int,int> >* rt = this->relevant_traversals->at(i);	
		
		Relevant_Node* Rnode = new Relevant_Node(rt,G,K,LSH);
		
		nodes->push_back(Rnode);	//insert into vector
		
		Rnode = NULL;
		rt = NULL;
	}
	
	cout << " Nodes size is " << nodes->size() << endl;
	
	delete relevant_traversals;		// we don't need this anymore
	relevant_traversals = NULL;
		
}















Relevant_List::~Relevant_List(){

	for(unsigned int i = 0; i < this->nodes->size(); i++) delete nodes->at(i);
	delete nodes;

//	delete relevant_traversals;
	// the items inside vector will be deleted by the Relevant_node destructor
	
}




int Relevant_List::All_RT(vector<pair<int,int>>* current, vector<int>* limit, int i, int j){

	if( (i == this->length_i) || (j == this->length_j) ) return -1;
	// go back a step (out of bounds)
	 
	if( abs(i-j) > 2) return -1; // not a relevant traversal( need <= 2)
	
	if( i > limit->at(j) ) return -1;	// we don't want cells under the diagonal
	
	// So valid step has been made ==> create the pair
	pair<int,int> temp(i,j);	
	current->push_back(temp);	
	
	
	if( (i == length_i-1) && ( j == length_j-1) ){	// reached our destination
		//Create a copy of current vector
	
		vector<pair<int,int>>* temp1 = new vector<pair<int,int>>;
		temp1->reserve(current->size());
		
		*temp1 = *current;	// copy contents of current(the pairs) into temp1

		assert( temp1->at(0) == current->at(0) );

		this->relevant_traversals->push_back(temp1);	
		// a new relevant traversal has been created
		
		temp1=NULL;		
		return 0;		
	} 
	// Haven't reached our destination yet
	// move forward: a) i+1   b) j+1 
	
	int value = 0;

	value = this->All_RT(current,limit,i+1,j);	//move up	
	if( value == 0)	current->pop_back();	// remove the last pair

	value = this->All_RT(current,limit,i+1,j+1);	//move diagonally
	if( value == 0)	current->pop_back();	// remove the last pair

	value = this->All_RT(current,limit,i,j+1);	//move right
	if( value == 0)	current->pop_back();	// remove the last pair
		
	return 0;
}






//Find Orange Line (recursively)       (i,j) start from (0,0)
void Relevant_List::Main_RT(vector<pair<int,int>>* main_rt, int i, int j, double l){

//	cout << " i is " << i << " j is " << j << " and l is " << l << endl;

	assert( (i != length_i) && ( j != length_j) );


	// create the pair
	pair<int,int> temp(i,j);	
	main_rt->push_back(temp);	
	
	if( (i == length_i-1) && ( j == length_j-1) ){	// reached our destination

		return;		
	} 
	
	int yp = j+1;
	int xp = i+1;

	double y = l * xp;
	
//	if( y < yp ) this->Main_RT(main_rt,i+1,j,l);
//	else if( y > yp) this->Main_RT(main_rt,i,j+1,l);
	if( y < yp ) this->Main_RT(main_rt,i+1,j,l);
	else if( y > yp) this->Main_RT(main_rt,i,j+1,l);
	else if( y == yp) this->Main_RT(main_rt,i+1,j+1,l);
}


























void Relevant_List::Insert(vector<Curve*>* vc, int k, int L){	//for LSH

	for(unsigned int i = 0; i < nodes->size(); i++) nodes->at(i)->Insert(vc,k,L);

	cout << "Insert RL_Insert\n";

}



void Relevant_List::Insert1(vector<Curve*>* vc, int k, int M,int probes){	//for LSH

	for(unsigned int i = 0; i < nodes->size(); i++) nodes->at(i)->Insert1(vc,k,M,probes);
}








void Relevant_List::Search(Curve* query,InfoCurve* info){	

	for(unsigned int i = 0; i < nodes->size(); i++) nodes->at(i)->Search(query,info);

}

























