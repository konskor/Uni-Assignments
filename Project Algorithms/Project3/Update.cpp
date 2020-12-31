#include <iostream>
#include <random>
#include <chrono>
#include <cmath>

#include <vector>
#include <utility>

#include <string>

#include <cassert>


#include "Update.hpp"

#include "Vector.hpp"
#include "Curve.hpp"


#include "Utilities.hpp"
#include "Utilities2.hpp"

#include "Utilities3.hpp"



bool Update_Lloyd(vector<Item*>* items, vector<Cluster*>* clusters){
//Update 1

	/******************************************************
		items1 contains every item (with centroid)

		items2 contains all the items in the cluster, except potential candidate
	***********************************************************/


	int original_size = items->size();


	bool change = false;

	int counter = 0;	// how many centroids were changed

	for( unsigned int i = 0; i < clusters->size(); i++){ // for each cluster
	
		assert(clusters->at(i)->getSize() >= 2);
		
		assert( clusters->at(i)->getInput() == true);

		Cluster* cluster = clusters->at(i);
		Item* old_centroid = cluster->getC();

		vector<Item*>* items1 = new vector<Item*>;
		cluster->getAllItems(items1);	// get all existing items inside the cluster 

		double min = -1;
		int min_index = -1;


		for( unsigned int j = 0; j < items1->size(); j++){	// for all items

			vector<Item*>* items2 = new vector<Item*>;
			*items2 = *items1;	// Shallow copy
			
			/**********************************************************
				Update items2 so that it contains every item in cluster
				except the potential candidate
			************************************************************/
			
			items2->at(j) = items2->back();	//overwrite
			items2->pop_back();
			
			/* Now compute the sum distances */
			
			double sum = 0;
			
			for( unsigned int k = 0; k < items2->size(); k++)
				sum = sum + Item_Distance(items1->at(j), items2->at(k) );


			// Computed the sum, check if minimum
			
			if( (min < 0) || ( sum < min) ){
				
				min = sum;
				min_index = j;
			}
			
			delete items2;
			items2 = NULL;
		}
		//Found the true minimum, check if different than old_centroid
	
		if( items1->at(min_index) != old_centroid){
		
			//Insert old centroid, back into items
//			items->push_back(old_centroid);

			// Delete old cluster ,
			delete cluster;
			cluster = NULL;

			/********************* IMPORTANT ***********************************
			
				Order of instruction matters
				1) Delete old cluster first 
				2) Then create new one
				
				Otherwise, it can create problems with ( cluster, 2nd_cluster) in neighbohr
			
			*/
			
			//Create new cluster
			Item* new_centroid = items1->at(min_index);
			Cluster* new_cluster = new Cluster(new_centroid,true);
			
			assert(new_cluster->getC()->getCluster() != NULL);
			
		
			//Replace in cluster vector<>
			clusters->at(i) = new_cluster;
			
			/* testing */
			bool test = false;
			for( unsigned int index1 = 0; index1 < items->size(); index1++){
			
				if( items->at(index1) == new_centroid ){
					 test = true;
					 
					 items->at(index1) = old_centroid; 
				}
			
			}

			
			assert( test == true);
			
			
			
			new_centroid = NULL;
			new_cluster = NULL;
			
			change = true;	// a cluster was changed

			counter++;	//increment counter
		}// else centroid didn't change ==> do nothing
		
		delete items1;
		items1 = NULL;

		cluster = NULL;
		old_centroid = NULL;

	}// updated all clusters => return
	
//	assert(counter <= 5);
	

	// testing again
	for( unsigned int i = 0; i < clusters->size(); i++){
	
		assert(clusters->at(i)->getC()->getCluster() != NULL);
	
	}


	
//	cout << "Items size is " << items->size() << endl;
	
	// Finished exit
	
	assert( items->size() == original_size);

	return change;
}




//Update2 for vectors ==> Mean Vector
bool Update_Vector(vector<Item*>* items, vector<Cluster*>* clusters){


	for( unsigned int i = 0; i < clusters->size(); i++){ // compute mean vector for all clusters
	
		vector<double>* coordinates = new vector<double>;
	
		Cluster* cluster = clusters->at(i);
		Item* centroid = cluster->getC();
		
		//Items1 contains all vectors in cluster (existing or not)
		vector<Item*>* items1 = new vector<Item*>;
		cluster->getAllItems(items1);	// will return centroid
//		items1->push_back(centroid);

		vector< vector<double>* >* points = new vector< vector<double>* >;
		
		for( unsigned int i = 0; i < items1->size(); i++){
		
			vector<double>* c1 = items1->at(i)->getVector()->getC();	//get coordinates
			points->push_back(c1);
			
			c1 = NULL;
		}
		
		Mean_Point(points,coordinates);	// compute mean Vector

		/***************************** IMPORTANT ***************************************
		

			ORDER MATTERS ==> Delete old cluster and then create new one 
		
		************************************************************************/

		//Deleting old cluster
		if( cluster->getInput() == true) items->push_back(centroid);//don't lose the existing item
		delete cluster;
		cluster = NULL;


		// Creating new vector
		string name = "a";
		Vector* v = new Vector(name,coordinates);

		// Creating the item
		Item* new_item = new Item(v);

		//Creating new cluster
		Cluster* new_cluster = new Cluster(new_item,false);
		
		//Replacing old cluster in vector<>
		clusters->at(i) = new_cluster;
		
		coordinates = NULL;
		cluster = NULL;
		centroid = NULL;
		
		delete items1;
		items1 = NULL;
		
		delete points;
		points = NULL;
	}
	
	cout << "\nItems size is " << items->size() << endl;
	
	
//	assert(items->size() == 456 );
	

	return true;

}







// CHECK AGAIN

bool Update_Curve(vector<Item*>* items, vector<Cluster*>* clusters){


	for( unsigned int i = 0; i < clusters->size(); i++){	//for each cluster
	
		vector<Item*>* items1 = new vector<Item*>;
		clusters->at(i)->getAllItems(items1);
		
//		vector<Item*>* items1 = clusters->at(i)->getItems();
		
		/* Find lamda */
		
		unsigned int sum = 0;
		
		for(unsigned int j = 0; j < items1->size(); j++) 
			sum = sum + items1->at(j)->getCurve()->getSize();
	
		unsigned int lamda = sum / items1->size();
		
		cout << "Compute lamda = " << lamda << endl;
	
		/* Find the Curves which have (>= lamda) points */
		vector<Item*>* items2 = new vector<Item*>;
		
		for( unsigned int j = 0; j < items1->size(); j++){	// for each item in cluster
		
			unsigned int size = items1->at(j)->getCurve()->getSize();

			if( size >= lamda) items2->push_back(items1->at(j));
		}
		
		assert( items2->size() > 0 );
		
		cout << "Found all the curves with length >= lamda \n";
		
		// Found all the curves => pick a random curve
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<int> dis(0, items2->size() - 1);		// [0, size - 1]
	
		int index = dis(gen);
		
		Item* So = items2->at(index);	//the chosen item/curve
		
		int* array = new int[lamda];
		Generate_Random_Subsequence(lamda, So->getCurve()->getSize(), array);
	
		cout << "Generated ranodm subsequence \n";
	
		vector< vector<double>* >* C = new vector< vector<double>* >;
		So->ChoosePoints(C,array,lamda);
		
		//Choose points, create the C with copies of the points of So
		// The points are chosen
		
		cout << "Created our initial C\n";
		
		// Now we have created our initial C
	
		vector< vector<double>* >* previous_C = new vector< vector<double>* >;
		
		/******************* The main part of the function *****************/
	
		int max_iterations = 5;
		for( int j = 0; j < max_iterations; j++){

			if( previous_C->size() != 0){
			
				unsigned int size = previous_C->size();
				for(unsigned int k = 0; k < size; k++){
				
					delete previous_C->back();
					previous_C->pop_back();
				}			
			
			}
		
			*previous_C = *C;
		
			vector< vector< vector<double>* >* >* A = new vector< vector< vector<double>* >* >;
			// This is a vector of vectors of points ==> vector of Curves/sequences
			
			// Initialize A 
			for( unsigned int k = 0; k < lamda; k++){

				vector< vector<double>* >* temp = new vector< vector<double>* >;
//				A->at(k) = new vector< vector<double>* >;

				A->push_back(temp);
				temp = NULL;
			}
			
			// Now to fill A
			for( unsigned int k = 0; k < items1->size(); k++){
			
				vector< vector<double>* >* points = items1->at(k)->getCurve()->getP();
				//points of the curve k
			
				vector< pair<int,int> >* IPairs = new vector< pair<int,int> >;
				//index pairs of best-traversals
				
				DTW1(C,points,IPairs);	//INCOMPLETE
				
				for(unsigned int k1 = 0; k1 < IPairs->size(); k1++){
				
					pair<int,int> temp = IPairs->at(k1);
					
					A->at( temp.first)->push_back( points->at(temp.second) );				
				}
				// Filled the A vector<>
			
				delete IPairs;
				IPairs = NULL;
				points = NULL;
			}
			// time to compute the new C
			
			assert( A->size() == lamda);
			
			for( int k = 0; k < lamda; k++){
			
				vector<double>* temp = new vector<double>;
				Mean_Point(A->at(k),temp);
			
				C->at(k) = temp;
				temp = NULL;
			}
			// the new C was created
			
			//Erase A
			for( unsigned int k = 0; k < A->size(); k++) delete A->at(k);
			delete A;
			A = NULL;
			
			/* Compare to old one? ==> nope (check again) */
		}
		//The new centroid C was created
		
		unsigned int size = previous_C->size();
		for(unsigned int k = 0; k < size; k++){
				
			delete previous_C->back();
			previous_C->pop_back();
		}

		delete previous_C;
		previous_C = NULL;
		
		//Create Cluster with centroid C 
		
		string name = "a";
		Curve* new_curve = new Curve(name,C);
		Item* new_Item = new Item(new_curve);
		
		Cluster* new_cluster = new Cluster(new_Item,false);
		
		// If centroid is an existing item, push back into vector<> items
		if( clusters->at(i)->getInput() == true){
		
			Item* centroid = clusters->at(i)->getC();
			items->push_back(centroid);
			centroid = NULL;
		}
		
		// Delete old cluster
		delete clusters->at(i);
		clusters->at(i) = NULL;
		
		// Insert new cluster
		clusters->at(i) = new_cluster;
		
		new_curve = NULL;
		new_Item = NULL;
		new_cluster = NULL;
		
		C = NULL;
		
		delete[] array;
		array = NULL;
		
		delete items2;
		items2 = NULL;
		
		delete items1;
		items1 = NULL;
	}
	
	cout << "Finished Mean Curve\n";

	//finished updating all clusters => exit

	return true;
}









// Used in both Update2_Vector() and Update2_Curve()
void Mean_Point(vector< vector<double>* >* points, vector<double>* result){

	int d = points->at(0)->size();
	
//	assert( d == 2);

	for( int i = 0; i < d; i++){	// construct each coordinate
		
		double sum = 0;
			
		for( unsigned int j = 0; j < points->size(); j++) sum = sum + points->at(j)->at(i);
			
		double total = sum / points->size();
		result->push_back(total);
	}
	
	assert( result->size() == d);

}
















