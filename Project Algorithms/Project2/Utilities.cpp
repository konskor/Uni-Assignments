#include <iostream>
#include <string>
#include <fstream>

#include <chrono>
#include <random>

#include <cmath>
#include <ctime>

#include <vector>

#include "Vector.hpp"
//#include "InfoItem.hpp"

using namespace std;

//not used
// Random generator of real(float numbers ==> maybe use double?
double FRandomGen(double low, double high, std::default_random_engine generator){

	//Create random (uniform distribution) float number	
	
	uniform_real_distribution<double> distribution (low,high);

	double k = distribution(generator);
	
	return k;
}




/* Simple timestamp function */
double Timestamp(){

	double t = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	return t;
}







/*
int Mod(int x, int y){

	int r = x % y; 		// remain
	
//	if( r < 0)	r = r + abs(y);
	
	if( r < 0){
	
		r = r - y;
	
	}else{
	
		r = r + y;
	}
	
	
	return r;
}

*/

int Mod(int x, int y){

	return ( x %y + y) % y;

}










double Vector_distance(Vector* v1, Vector* v2, int k){	


	int i = 0;
	double distance = 0.0;
	vector<double>* t1 = v1->getC();
	vector<double>* t2 = v2->getC();


	int d = v1->getD();		//dimension 
	
	double sub = 0.0;
	double pow1 = 0.0;

	double sum = 0.0;
	
	
	for( i = 0; i < d; i++){
		
		sub = abs( t1->at(i) - t2->at(i) );		//absolute value of sub
		pow1 = pow(sub, k);

		sum = sum + pow1;
	}
	
	distance = pow(sum,1.0/k);
	

	t1 = NULL;
	t2 = NULL;
	
	return distance;
}



double Compute_w(vector<Vector*>* points){
	
	double min = -1;
	
	unsigned int i = 0;
	unsigned int j = 0;
	
	string stemp;
	
	double temp = 0.0;
	
	double avg = 0.0;//Compute average distance of all NN for w (LSH point) ==> w = 3*avg or 4*avg
	double temp1 = 0.0;
	
	for( i = 0; i < points->size(); i++){
			
		for( j = 0 ; j < points->size(); j++){
		
			if( i == j) continue;	// don't do yourself
	
			temp = Vector_distance( points->at(i), points->at(j), 1 );
			// 1 for Manhattan, 2 for Euclidean, etc.
	
			if( (min < 0) || (temp < min) ){
			
				min = temp;
				stemp = ( points->at(j) )->getName();	// (Vector*)->getName()	
			}
		}
				
		temp1 = temp1 + min;

		min = -1;	//reset value

	}

	avg =  temp1 / (points->size() );
	
	double w = 3 * avg;	//3 or 4

	cout << "Finished computing w" << endl;
	return w;
}


int Max(int *a,int size){

	int max = -100000;
	
	for(int i = 0; i < size; i++){
	
		if( a[i] > max) max = a[i];
	}

	return max;
}



unsigned int alt_pow(int m, unsigned int d, unsigned int M){		// Used for LSH ==> m^d-1

	unsigned int p = 1;
	unsigned int i = 0;
	
	int m1 = Mod(m,M);
	
	
//	for( i = 0; i < d; i++) p =  Mod( Mod(p,M) * Mod(m,M) , M);

	for( i = 0; i < d; i++) p =  Mod( p * m1 , M);


	return p;
} 



/* Not needed in project2 
double NN_True(vector<Vector*> *points, vector<Vector*> *queries,  vector<InfoItem*> *info){

	double min = -1;
	
	unsigned int i = 0;
	unsigned int j = 0;
	
	string id;
	
	double temp = 0.0;
	double sum = 0.0;
		
	for( i = 0; i < queries->size(); i++){

		double t1 = Timestamp();
		
		for( j = 0; j < points->size(); j++){
	
			temp = Vector_distance( queries->at(i), points->at(j), 1 );
			// 1 for Manhattan, 2 for Euclidean, etc.
	
			if( (min < 0) || (temp < min) ){
			
				min = temp;
				id = ( points->at(j) )->getName();	// (Vector*)->getName()	
			}
		}
		
//		cout << "Min is " << min << endl;
//		cout << "NN is point with id " << id << endl; 
		
		double t2 = Timestamp();
		double duration = t2-t1;
		string q = (queries->at(i) )->getName();	//query name
		
		// query_name, NN_name, True_distance, duration
		InfoItem* item = new InfoItem( q, id, min, duration);
		// not all of info available ( will use modifiers later)		
		
		info->push_back(item);
		item = NULL;		// don't call destructor

		sum = sum + min;	// for w

		id.clear();		

		min = -1;	//reset value
	}
	
//	cout << "Sum is " << sum << endl;
//	cout << "Queries->size() " << queries->size() << endl; 

	double avg = sum / (queries->size());

//	double w = 4 * avg;
	
	double w = 10 * avg;
	
	cout << "Finished NN_True" << endl;
		
	return w;	
}

*/


/*	Not needed in project2
void Print_vec(vector<InfoItem*> *info, string filename){

	ofstream output;
	output.open(filename.c_str(), ios::out | ios::trunc );
	
	unsigned int i = 0;
	
	for( i = 0; i < info->size(); i++){
	
		output << "Query: " << info->at(i)->getQ() << endl;
		output << "Nearest neighbor: " << info->at(i)->getNN() << endl;
		
//		output << "True Nearest neighbor: " << info->at(i)->getNN_True() << endl;
		
		output << "distanceLSH: " << info->at(i)->getLSH_d() << endl;
		output << "distanceTrue: " << info->at(i)->getTrue_d() << endl;
		output << "tLSH: " << info->at(i)->getLSH_t()/1000 << endl;
		output << "tTrue: " << info->at(i)->getTrue_t()/1000 << endl;
		
		output << endl << endl;	
	}
	output.close();
}
*/


























