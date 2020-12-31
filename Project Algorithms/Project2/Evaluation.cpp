#include <iostream>
#include <random>
#include <chrono>
#include <cmath>

#include <vector>

#include "Evaluation.hpp"

#include "Vector.hpp"
#include "Curve.hpp"

#include "Utilities3.hpp"




void Silhouette(vector<Cluster*>* clusters, vector<double>* s,  unsigned int size){

	// (size) is the number of data points 

	double s_total_sum = 0;

	for( unsigned int i = 0; i < clusters->size(); i++){	// for each cluster
	
		double s_sum = 0;	// average of si
	
		vector<Item*>* items1 = new vector<Item*>;
		clusters->at(i)->getAllItems(items1);
		
		assert( items1->size() >= 2);
		
		for( unsigned int j = 0; j < items1->size(); j++){	// for each object in cluster

			double a = Compute_Average_Distance(items1->at(j),items1,true,j);
			
			Cluster* cluster = items1->at(i)->getCluster();
		
			assert( cluster == clusters->at(i) );
		
			Cluster* cluster_2 = items1->at(j)->get_2nd_Cluster();

			// This a pretty rare case
			if( cluster_2 == clusters->at(i) ){

				// if best and 2nd best are the same clusters, then that means
				// that (probably) during range search,
				// this item was found only by the farthest cluster
				
				//Very very rare but it can happen
//				cout << "Worst case scenario, silhouette\n";
				
				
				cluster_2 = NULL;
				
				//si = 0;
				
//				delete items1;
//				items1 = NULL;
				
				continue;
			}
			
			//else, ALL good
			
			
			if( cluster_2 == NULL){
			
				vector<Item*>* temp1 = new vector<Item*>;
				
				temp1->push_back(items1->at(j));
				
				Compute_2nd_Best(temp1,clusters);
			}
			
			assert( cluster_2 != NULL);
			
			vector<Item*>* items2 = new vector<Item*>;
			cluster_2->getAllItems(items2);

			double b = Compute_Average_Distance( items1->at(j), items2, false, 0);
			
			double s_item = 0;
			
			if( a < b)	s_item = 1 - (a/b);
			else if( a > b) s_item = (b/a) - 1;
			
			s_sum = s_sum + s_item;
			
			delete items2;
			items2 = NULL;
		}
		// Computed the sum of all s(i) in this cluster
	
		double s_cluster = s_sum / items1->size();	// the si
		
//		s_total_sum = s_total_sum + s_cluster; 

		s_total_sum = s_total_sum + s_sum;
		s->push_back(s_cluster);
		
		delete items1;
		items1 = NULL;
	}

//	double stotal = s_total_sum / clusters->size();
	double stotal = s_total_sum / size;
	s->push_back(stotal);

	// Done ==> return
}




double Compute_Average_Distance(Item* item,vector<Item*>* items1, bool same,int index){


	// Create Copy of items
	vector<Item*>* items2 = new vector<Item*>;
	*items2 = *items1;	//shallow copy

	if( same == true){	// if item exists  in the same cluster, with other items  

		// remove current object, from copy
		items2->at(index) = items2->back();
		items2->pop_back();
	}

	double sum = 0;
		
	//Compute distance from other objects, in SAME cluster
	for( unsigned int k = 0; k < items2->size(); k++){
		
		double distance = Item_Distance(item, items2->at(k) );

		sum = sum + distance;			
	}
	// Computed Distance with all the other items, in Same cluster
	
	double result = sum / items2->size();	
	
	delete items2;
	items2 = NULL;


	return result;

}





