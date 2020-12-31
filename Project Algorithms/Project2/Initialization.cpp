#include <iostream>
#include <random>
#include <chrono>
#include <cmath>

#include <vector>

#include "Initialization.hpp"

#include "Vector.hpp"
#include "Curve.hpp"

#include "Utilities3.hpp"


using namespace std;

//ALL OK
void Random_Initialize(int k,vector<Item*>* items, vector<Cluster*>* clusters){

// k = how many clusters do we need

	for(int i = 0; i < k; i++){
	
		int l = items->size() - 1;
	
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<int> dis(0,l);
	
	
		int index = dis(gen);

		Item* centroid = items->at(index);	//our new centroid
		Cluster* cluster = new Cluster(centroid,true);
		clusters->push_back(cluster);
		
		// Removing the centroid from the vector<Items>
		Item* temp =  items->back();	//access the last element
		items->at(index) = temp;	//overwrite the value of the previout item
		items->pop_back();
		
		centroid = NULL;
		cluster = NULL;
		temp = NULL;
	}
	
//	assert ( k == 5);
	
	cout << "Finished Initialization (1) \n";
}























//Initialization++
void Initialize(int k, vector<Item*>* items, vector<Cluster*>* clusters){

	/* Choose the first centroid (uniformly at random) */ 

	int l = items->size() - 1;

	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(0,l);		// [0, size - 1]

	int index = dis(gen);

	Item* centroid = items->at(index);	//our 1st centroid
	Cluster* cluster = new Cluster(centroid,true);
	clusters->push_back(cluster);
	
	cout << "Our 1st cluster => OK\n";

	// Removing the centroid from the vector<Items>
	Item* temp =  items->back();	//access the last element
	items->at(index) = temp;	//overwrite the value of the previous item
	items->pop_back();


	/* Compute the distances of each item from the 1st cluster */

	// This vector will hold the minimum distance, of each Item to some cluster
	vector<double>* D = new vector<double>;
	
	double max = -1;

	for(unsigned int i = 0; i < items->size(); i++){	// Compute the D(i)
		
		double distance = Item_Distance(items->at(i), centroid);	//Utilities3.hpp

		D->push_back(distance);
		
		if( distance > max) max = distance;
	}
	
	
		/* Normalizing the D(i)'s */
	
	vector<double>* D1 = new vector<double>;
	for( unsigned int i = 0; i < D->size(); i++){

		double normD = ( D->at(i) / max);
		D1->push_back(normD);
	}

		/* Compute the P(r) = P(r-1) + D(r)^2 */

	vector<double>* P = new vector<double>;
		
	cout << "D1 size is " << D1->size() << endl;
	
	double dr = pow(D1->at(0),2);	// D(r)^2
	P->push_back(dr);
		
	for( unsigned int i = 1; i < items->size(); i++){
	
		dr = pow(D1->at(i),2);
		
		double Pr = P->at(i-1) + dr;
		P->push_back(Pr);
	}

	/* Pick a float x in [0,P->back()] */

	random_device rd1;
	mt19937 gen1(rd1());
	uniform_real_distribution<double> dis1(0,P->back());

	double x = dis1(gen1);
	int r = BinarySearch(P,x,0,P->size()-1);	//Searching for our new centroid

	delete D1;
	D1 = NULL;
	
	delete P;
	P = NULL;


	/* Found our 2nd centroid */


	Item* centroid1 = items->at(r);	//our new centroid
	Cluster* cluster1 = new Cluster(centroid1,true);
	clusters->push_back(cluster1);

	// Removing the centroid from the vector<Items>
	temp =  items->back();	//access the last element
	items->at(r) = temp;	//overwrite the value of the previous item
	items->pop_back();

	cout << "Our 2nd centroid ==> OK\n";

	// Removing the centroid from the vector<> D
	double temp1 =  D->back();	//access the last element
	D->at(r) = temp1;	//overwrite the value of the previous item
	D->pop_back();
	
	
	// Don't need these anymore
	centroid = NULL;
	cluster = NULL;
	temp = NULL;
	
	centroid1 = NULL;
	cluster1 = NULL;


// For the rest:
// We only compute the distances to the recently found centroid
// If new_D(i) < D(i) ==> D(i) = new_D(i)

// Again start with max = -1, and find maxD(i) ==> normalize ==> vector<> D1
// then Compute P() and find r

// More code, but each distance is computed only once


//	cout << "Before for loop\n";

	/* Now for the rest (INCOMPLETE => check again) */

	//1 or 2 for t ==> 2
	for( int t = 2; t < k; t++){	// compute the remaining centroids 
	
		max = -1;
		
		centroid1 = clusters->back()->getC();	// get the last computed centroid

		assert(centroid1 != NULL);

		for(unsigned int i = 0; i < items->size(); i++){	// Compute the D(i)
			
			double distance = Item_Distance(items->at(i), centroid1);

			if( distance > max) max = distance;	//new max
		
			if( distance < D->at(i) ) D->at(i) = distance;	// new min

		}//Computed all The D(i)
	
		/* Normalizing the D(i) */
	
		vector<double>* D2 = new vector<double>;
		for( unsigned int i = 0; i < D->size(); i++){

			double normD = ( D->at(i) / max);
			D2->push_back(normD);
		}

		/* Compute the P(r) = P(r-1) + D(r)^2 */

		vector<double>* P1 = new vector<double>;
	
		double dr1 = pow(D2->at(0),2);
		P1->push_back(dr1);
	
		for( unsigned int i = 1; i < items->size(); i++){
	
			dr1 = pow(D2->at(i),2);
		
			double Pr1 = P1->at(i-1) + dr1;
			P1->push_back(Pr1);
		}

		/* Pick a float x in [0,P->back()] */

		random_device rd2;
		mt19937 gen2(rd2());
		uniform_real_distribution<double> dis2(0,P1->back());

		double x1 = dis2(gen2);
		int r1 = BinarySearch(P1,x1,0,P1->size()-1);	//Searching for our new centroid

		delete D2;
		D2 = NULL;
	
		delete P1;
		P = NULL;

		/* Found our new centroid */

		Item* centroid2 = items->at(r1);	//our new centroid
		Cluster* cluster2 = new Cluster(centroid2,true);
		clusters->push_back(cluster2);

		// Removing the centroid from the vector<Items>
		temp =  items->back();	//access the last element
		items->at(r1) = temp;	//overwrite the value of the previous item
		items->pop_back();


		// Removing the centroid from the vector<> D
		double temp2 =  D->back();	//access the last element
		D->at(r1) = temp2;	//overwrite the value of the previous item
		D->pop_back();
		
		centroid2 = NULL;
		cluster2 = NULL;

	}	// found all the centroids ==> Exiting

	delete D;
	D = NULL;
	
	centroid = NULL;
	cluster = NULL;
	temp = NULL;
	
	
	cout << "Finished Initialization++ \n";
}


