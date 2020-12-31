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

#include "Relevant_Table.hpp"
#include "Relevant_List.hpp"

using namespace std;

Relevant_Table::Relevant_Table(int M1,int d,int window1,double e1,bool LSH){

	M = M1;
	window = window1;
	e = e1;		
	
	assert(window <= M);

	/* Creating G table */
	random_device rd;
	mt19937 gen(rd());
	
	normal_distribution<double> dis(0,1);
	
	double e2 = pow(e,2);	
	int K = (-1) * d * log(e)/e2;
	
//	cout << " K is " << K << endl;

	cout << " M is " << M << endl;
	
	assert( K > 0);
	
	
	G = new double*[K];
	
	for(int i = 0; i < K; i++){
	
		G[i] = new double[d];
		
		for( int j = 0; j < d; j++)	G[i][j] = dis(gen);	
	}
	
//	cout << "Hello from Relevant Table constructor\n";
	
	
	/* Create The Table ==> Fill Diagonally (Unfinished ) */
	
	table = new Relevant_List**[M];
	for(int i = 0; i < M; i++){
	
		table[i] = new Relevant_List*[M];
		for( int j = 0; j < M; j++){
//			table[i][j]	= new Relevant_List(i+1,j+1,G,K,LSH);
			table[i][j]	= NULL;
		}
		
		cout << "Finished table["<< i <<"]\n";
	}

	// Fill the main diagonal
	
	for(int i = 0; i < M; i++)	table[i][i] = new Relevant_List(i+1,i+1,G,K,LSH);

	// Fill upper diagonals
	
	for( int count = 1; count <= window; count++){
	
		for(int i = 0; i < M-count; i++) 
			table[i][i+count] = new Relevant_List(i+1,i+count+1,G,K,LSH);	
		
	}

	// Fill lower diagonals
	
	for( int count = 1; count <= window; count++){
	
		for(int i = 0; i < M-count; i++) 
			table[i+count][i] = new Relevant_List(i+count+1,i+1,G,K,LSH);	
		
	}


	cout << "Finished building the table\n";


}




Relevant_Table::~Relevant_Table(){

	for( int i = 0; i < K; i++)	delete G[i];
	delete[] G;
	
	
	for( int i = 0; i < M; i++){
	
		for(int j = 0; j < M; j++) delete table[i][j];	//delete Relevant_list	
		
		delete[] table[i];
	}
	delete[] table;

	
}


/* For LSH */
void Relevant_Table::Insert(vector<Curve*>* vc, int k, int L){

	vector<Curve*>** vec = new vector<Curve*>*[M];	// where M is max length of curves
	for(int i = 0; i< M; i++) vec[i] = NULL;
	
	this->Categorise(vc,vec);

	for(int i = 0; i < M; i++){
	
		if( vec[i] == NULL) continue;
		
		
		for(int j = 0; j < M; j++){
			
			if( table[i][j] == NULL) continue;
			
			table[i][j]->Insert(vec[i], k, L);
		}
	}

	cout << "almost finished RT_insert\n";

	/* Finished, delete vec */

	for(int i = 0; i < M; i++){
	
		if( vec[i] != NULL) delete vec[i];	//delete the sub-vector (not the Curve)
	}
	delete[] vec;	
}




/* For HyperCube */
void Relevant_Table::Insert1(vector<Curve*>* vc, int k, int M, int probes){

	vector<Curve*>** vec = new vector<Curve*>*[M];	// where M is max length of curves
	for(int i = 0; i< M; i++) vec[i] = NULL;
	
	this->Categorise(vc,vec);

	for(int i = 0; i < M; i++){
	
		if( vec[i] == NULL) continue;
		
		for(int j = 0; j < M; j++){

			if( table[i][j] == NULL) continue;

			table[i][j]->Insert1(vec[i], k, M, probes);	
	
		}
	}



	/* Finished, delete vec */

	for(int i = 0; i < M; i++){
	
		if( vec[i] != NULL) delete vec[i];	
	}
	delete[] vec;	
}











// Categorise the curves based on length, and store them in the array
void Relevant_Table::Categorise(vector<Curve*>* vc, vector<Curve*>** vec){

	for(unsigned int i = 0; i < vc->size(); i++){	// for all curves
	
		int length = vc->at(i)->getP()->size();
		
		assert(length <= M);
		
		if( vec[length-1] == NULL) vec[length-1] = new vector<Curve*>;
		
		vec[length-1]->push_back(vc->at(i));		
	}
}







void Relevant_Table::Search(Curve* query,InfoCurve* info){

	int j = query->getP()->size() -1;
	
	//For i, check up and down

	for( int i = j - window; i <= j + window; i++){
	
		if( ( i < 0) || (i > M) ) continue;	// out of bounds
	
		this->table[i][j]->Search(query,info);
	}



}
































