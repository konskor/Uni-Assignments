#include <iostream>
#include <string>

#include <vector>

#include <utility>

#include "Vector.hpp"
#include "Curve.hpp"

#include "Utilities.hpp"
#include "Utilities2.hpp"
#include "Utilities3.hpp"


using namespace std;

void Remove_Duplicates(vector<Item*>* items){

	for( unsigned int i = 0; i < items->size(); i++){	// for each item
	
	
		for(unsigned int j = i+1; j < items->size(); j++){
		
			double distance = Item_Distance(items->at(i), items->at(j) );
			
			if( distance > 0) continue;
			
			//else (distance == 0) ==> we must remove it
			
			while(true){
			
				if( items->at(j) == items->back() ){	//last item

					items->pop_back();
					break;
				}
				
			
				items->at(j) = items->back();
				items->pop_back();
				
				if( Item_Distance( items->at(i), items->at(j) ) > 0) break;
				//Check if the "previous" back item, is also a duplicate
			}
			// j is ok, check the others
			
		}
	
	}


}










double Item_Distance(Item* x, Item* y){

	bool curve = x->isCurve();

	double distance = 0;
			
	if ( curve == false){
		
		Vector* v1 = x->getVector();
		Vector* v2 = y->getVector();	
	
		distance = Vector_distance(v1,v2,1);	// Utilities1
			
		v1 = NULL;
		v2 = NULL;
			
	}else{	// it is a curve ==> DTW
		
		vector< vector<double>* >* c1 = x->getCurve()->getP();
		vector< vector<double>* >* c2 = y->getCurve()->getP();
		
		distance = DTW(c1,c2);	//Utilities2
			
		c1 = NULL;
		c2 = NULL;
	}

	return distance;
}





int BinarySearch(vector<double>* P, double x, int start, int end){

// used in both, Initialization (2) and Check/Fix_Assignment()

	if( start == end) return start;	// cannot search anymore


	int index = (start + end ) /2;
	
	if( x > P->at(index) ){
	
		int index1 = BinarySearch(P,x,index+1,end);
		return index1;
	
	}else if( x < P->at(index) ){

		if( (index == 0) || ( x > P->at(index-1) ) ) return index;

		int index1 = BinarySearch(P,x,start,index-1);
		return index1;


	}else{	// x == P->at(index)

		return index;
	}

}




//Check again
void UpdateItems(vector<Item*>* items){

	// remove from vector , every centroid Item

	// We want to Reorder the vector, in a way that
	// all the items up to (but not including) Item i are Non-Centroids

	// From Item i and after we have centroids
	// So we pop_back() as many times as needed


	for( unsigned int i = 0; i < items->size(); i++){
	
		if( items->at(i)->isCentroid() == false) continue;	//ignore
		
		
		
		if( i == items->size() -1){	//last element
		
			items->pop_back();
			continue;
		}
		
		unsigned int j = 0;
		
		for(j = i+1; j < items->size(); j++){
		
			if( items->at(j)->isCentroid() == false) break;
		
		}
	
		if( j == items->size() ){	// all the items after (i) are centroids => pop_back
		
			unsigned int counter = j - i; 	// pop_back (i) and everything after
		
			for( unsigned int k = 0; k < counter; k++)	items->pop_back();		

		}else{
		//We found a non-centroid Item => swap
		
			Item* temp = items->at(i);	//centroid
			items->at(i) = items->at(j);
			items->at(j) = temp;
			
			temp = NULL;
		
		}
	
	}

		assert( items->size() > 0);
}



void Break_Assignments(vector<Cluster*>* clusters){

	/* We call Break_Assignments , if we we want to Redo the Assignment part
	
	 We call it in 2 scenarios:
	 a) After an Update
	 b) If after the assignment, we found outliers (fixed them, but that resulted into new centroids) ==> Assignment again

	*/

	for( unsigned int i = 0; i < clusters->size(); i++)	clusters->at(i)->EmptyCluster();

}



void Compute_2nd_Best(vector<Item*>* items, vector<Cluster*>* clusters){

	// this is a sanitation check ==> all items have 2nd best cluster


	for( unsigned int i = 0; i < items->size(); i++){
	
		Cluster* cluster = items->at(i)->getCluster();
		
		assert( cluster != NULL);
		
		Cluster* cluster_2 = items->at(i)->get_2nd_Cluster();
		
		
		if( cluster_2 != NULL){
			cluster = NULL;
			cluster_2 = NULL;	
			continue;
		}
		
		//else we need to find the 2nd best cluster
		
		double min_dist1 = items->at(i)->getDist();
	
		int min_index = -1;
		double min_dist = -1;

		for( unsigned int j = 0; j < clusters->size(); j++){ // Compute dist from all centroids

			double distance = Item_Distance(items->at(i),clusters->at(j)->getC() );

			if( distance > min_dist1){
			
				if( (min_dist < 0) || ( distance < min_dist ) ){
				
					min_dist = distance;
					min_index = j;				
				}
			}
		}

		if( min_index == -1){

			cluster_2 = cluster;
			min_dist = 0;
			cout << "Worst case scenario, compute_best_2nd\n";
		}else{
		
			cluster_2 = clusters->at(min_index);		
		}

		items->at(i)->Change_2nd( cluster_2, min_dist);
		cluster = NULL;
		cluster_2 = NULL;
	}
	
	
	for( unsigned int i = 0; i < clusters->size(); i++){
		assert( clusters->at(i) != NULL);
	}
	
	

}

















//Used in Update2 Curve
void Generate_Random_Subsequence(int lamda, int size, int* array){

	// Basically, we have a[] which values 0->size-1, corresponding to the points of curve

	// We shuffle the a[] ==> get non-reapeated sequence of length = (size)

	// get the first (lamda * elements) of a[] in b[]
	
	// Insertion sort into (int* array) ==> non-descending order


	int* a = new int[size];
	
	for( int i = 0; i < size; i++) a[i] = i;
	// a[] will have { 0, 1, 2, ... , size-1}

	// shuffle a
	for( int i = size-1; i > 0; i--){
	
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<int> dis(0, i-1);
	
		int j = dis(gen);
	
		// swap a[i] and a[j]
		int temp = a[i];
		a[i] = a[j];
		a[j] = temp;
	} 

//	cout << "Hello 1 \n";

	vector<int>* b = new vector<int>;
	for(int i = 0; i < lamda; i++) b->push_back(a[i]);
	
	// Insertion sort
	
	for( int i = 0; i < lamda; i++){
	
		int min = -1;
		int min_index = -1;	
	
		for( unsigned int j = 0; j < b->size(); j++){
	
			if( (min < 0) || ( b->at(j) < min) ){

				min = b->at(j);
				min_index = j;
			}
		}
		// Found the min
		array[i] = min;
		
		//Erase min from vector<>
		b->at(min_index) = b->back();
		b->pop_back();
	}
	
	cout << "Finished\n";

	// cleaning up
	delete b;
	b = NULL;
	
	delete[] a;
	a = NULL;
}



// INCOMPLETE
void DTW1(vector<vector<double>*>* p, vector<vector<double>*>* q, vector< pair<int,int> >* result1){
	/* Best Traversal is the reverse procedure, we must find the "path"
	   to result[m1][m2] 
	*/

	// We create a table for DTW ( m1 x m2) ==> ( p->size() x q->size() )
	int m1 = p->size();
	int m2 = q->size();

	double **result = new double*[m1];

	for( int i = 0; i < m1; i++) result[i] = new double[m2];	
	
	// Filling  [0,0]
	result[0][0] = Vector_Distance1( p->at(0), q->at(0), 2); 	
	

	
	// Filling the first row
	for( int i = 1; i < m2; i++){
		result[0][i] = Vector_Distance1( p->at(0), q->at(i),2) + result[0][i-1];	  
	}
	

	
	// Filling the first collumn
	for( int i = 1; i < m1; i++){
		result[i][0] = Vector_Distance1( p->at(i), q->at(0),2) + result[i-1][0];
	}


	// Fill the rest row by row
	
	for( int i = 1; i < m1; i++){
	
		for( int j = 1; j < m2; j++){
		
			double x = result[i-1][j];
			double y = result[i-1][j-1];
			double z = result[i][j-1];
		
			result[i][j] = Vector_Distance1( p->at(i), q->at(j),2) + Min(x,y,z);		
		}	
	} 
	
	// Creating the pairs

//	cout << "Creating Index_pairs \n";

	DTW_Backtrack(result,m1-1,m2-1,result1);
//	cout << "Done with Index Pairs \n";

	
	// Filled the table ==> DTW is the lowest right cell 
//	double result1 = result[m1-1][m2-1];
	
	// Finished cleaning up
	for( int i = 0; i < m1; i++) delete result[i];	
	delete[] result;
	
	
//	return result1;

}




void DTW_Backtrack( double** array, int i , int j, vector< pair<int,int> >* v){
	
	if( (i == 0) && ( j == 0) ){
	
		pair<int,int> temp;
		temp.first = i;
		temp.second = j;
		
		v->push_back(temp);
		return;
	}
	
//	double sum = total;
	
	if( ( i - 1 >= 0) && ( j - 1 >= 0) ){

		// All 3 cells are available => find minimum

		double x = array[i-1][j];		// (i - 1)
		double y = array[i][j-1];		// (j - 1)
		double z = array[i-1][j-1];	// (i - 1) && (j - 1)
		
		// choice == 1 ==> choose (i+1)
		// choice == 2 ==> choose (j+1)
		// choice == 3 ==> choose (i+1) && (j+1)
			
		// find the minimum of the 3
		double min = 0;
		int choice = 0;
	
		if( x < y){
			min = x;
			choice = 1;
			
		}else{
			min = y;
			choice = 2;
		}
	
		if( z < min){
			min = z;
			choice = 3;
		}
		//Found the minimum

		// Decrement indexes
		if( choice == 1){
			// i--; ==> go up
			
			DTW_Backtrack(array,i-1,j,v);
			
		}else if( choice == 2){
			//j--; ===> go left

			DTW_Backtrack(array,i,j-1,v);

		}else{
			// (i--) && (j--) ==> diagonally
			DTW_Backtrack(array,i-1,j-1,v);
		}


	//else reached wall ,go up or left
	}else if( i - 1 >= 0){
		// cannot go left
		
		DTW_Backtrack(array,i-1,j,v);
		 
	}else if( j - 1 >= 0){	// i + 1 >= m1 ====> ERROR
		// cannot go up
		
		DTW_Backtrack(array,i,j-1,v);
	}else{
	
		cout << "Error in DTW backtrack() \n";	
	}

	// Now create current pair ( after returning from recursion)
	pair<int,int> temp;
	temp.first = i;
	temp.second = j;

	v->push_back(temp);
	return;
}














