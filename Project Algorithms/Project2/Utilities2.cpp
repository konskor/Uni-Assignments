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
//#include "InfoItem.hpp"
#include "Utilities.hpp"
#include "Utilities2.hpp"
#include "Curve.hpp"


using namespace std;





double Compute_delta(vector<Curve*>* curves){

//	double sum = 0;	// sum of distances (of consecutive points in a  curve)

	double total = 0;	// sum of all the averages

	for(unsigned int i = 0; i < curves->size(); i++){	// for each curve
	
		vector< vector<double>* >* points = curves->at(i)->getP();
		
		double sum = 0;
		
		int counter = 0;
		
		for( unsigned int j = 0; j < points->size() -1; j++){
		// Euclidean because of DTW (as instructed)
		//https://eclass.uoa.gr/modules/forum/viewtopic.php?course=DI352&topic=22406&forum=33439

			double dist = Vector_Distance1(points->at(j), points->at(j+1),2);

			sum = sum + dist;
			counter++;
		}
	
		if( counter > 0){
			double avg = sum/(points->size()-1);	// n-1 distances
			total = total + avg;
		}
	}

	double delta = total / curves->size();

	if( delta == 0 ){
		cout << "\n\nUnderflow in delta\n\n";
		delta = 0.001;	// if underflow
	}
	
	
	return delta;
}











double Compute_w_curve(vector<Vector_Node*>* points){
	
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
			

	
			temp = Vector_distance( points->at(i)->getV(), points->at(j)->getV(), 1 );
			// 1 for Manhattan, 2 for Euclidean, etc.
	
	
			if( (min < 0) || (temp < min) ){
			
				min = temp;
				stemp = ( points->at(j)->getV() )->getName();	// (Vector*)->getName()	
			}
		}
				
		temp1 = temp1 + min;

		min = -1;	//reset value

	}

	avg =  temp1 / (points->size() );
	
	double w = 4 * avg;

	cout << "Finished computing w" << endl;
	return w;
}







// Same grid Curves
bool Same_Grid(Curve* v1, Curve* v2){

	vector< vector<double>* >* p1 = v1->getP();
	vector< vector<double>* >* p2 = v2->getP();
	
	if( p1->size() != p2->size() ){	 //different number of points
		
//		cout << "The grid curves have different # of points\n";

		p1 = NULL;
		p2 = NULL;	
		return false;
	}
	
	for(int i = 0; i < p1->size(); i++){
	
		if( Same_vectors(p1->at(i), p2->at(i) ) == false){
			return false;	
		}
	}
	
	p1 = NULL;
	p2 = NULL;
	
	return true;
}




bool Same_vectors(vector<double>* a, vector<double>* b){

	int size = a->size();
	bool result = true;
	
	
//	cout << "A->size() is " << a->size() << " and b->size() is " << b->size() << endl;
	
//	if( a->size() != b->size() ) return false;	//
	
	assert( a->size() == b->size() );
	
//	cout << "A->size() is " << a->size() << endl;
//	cout << "B->size() is " << b->size() << endl;
	

	for( int i = 0; i < size; i++){
	
		if( a->at(i) != b->at(i) ){
		
//			cout << "A("<<i<<") is "<< a->at(i) << " and b("<<i<<") is " << b->at(i) << endl; 
			result = false;
			break;	
		}
	}

	return result;
}






double Min(double x, double y, double z){

	double result = x;
	
//	if( result < y) result = y;	
//	if( result < z) result = z;
	
	if( result > y) result = y;	
	if( result > z) result = z;
	
	return result;
}

// Similar with Vector_Distance, but with vectors(STL) instead of Vectors(objects)
double Vector_Distance1(vector<double>* v1, vector<double>* v2, int k){

	int i = 0;
	double distance = 0.0;

	int d = v1->size();
	
	double sub = 0.0;
	double pow1 = 0.0;

	double sum = 0.0;
	
	for( i = 0; i < d; i++){
	
		sub = abs( v1->at(i) - v2->at(i) );		//absolute value of sub
		pow1 = pow(sub, k);
		
//		cout << "pow1 for "<< i << " is " << pow1 << endl;
		
		sum = sum + pow1;
	}
	
	distance = pow(sum,1.0/k);

	
	return distance;
}



double DTW(vector<vector<double>*>* p, vector<vector<double>*>* q){

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
	

	
	// Filled the table ==> DTW is the lowest right cell 
	double result1 = result[m1-1][m2-1];
	
	// Finished cleaning up
	for( int i = 0; i < m1; i++) delete result[i];	
	delete[] result;
	
	
	return result1;
}












/* Not needed in project2
int Curve_NN_True(vector<Curve*>* curves, vector<Curve*>* queries, vector<InfoCurve*>* info){

	/* Brute force, dist with DTW 
	   Also compute delta = min(m1,m2,m3,...,mk) of all input-queries
	*

	double min = -1;		 
	unsigned int m = curves->at(0)->getP()->size();	 // m will be minimum number of points in dataset

	
	unsigned int i = 0;
	unsigned int j = 0;
	
	string id;
	
	double temp = 0.0;

		
	for( i = 0; i < queries->size(); i++){

		
		// for delta
		if( m > queries->at(i)->getP()->size() ) m = queries->at(i)->getP()->size();

		
		for( j = 0; j < curves->size(); j++){


//			cout << "Comparing Query " << i <<" with input " << j << endl;
	
			temp = DTW( queries->at(i)->getP(), curves->at(j)->getP());
			

	
			if( (min < 0) || (temp < min) ){
			
				min = temp;
				id = ( curves->at(j) )->getName();	// (Curve*)->getName()	
			}
			
			// for delta
			if( m > curves->at(j)->getP()->size() ) m = curves->at(j)->getP()->size();
			
//			cout << "Curve " << j <<" has " << curves->at(j)->getP()->size() << endl;
		
		}
		
//		cout << "Min is " << min << endl;
//		cout << "NN is curve with id " << id << endl; 
		

		string q = (queries->at(i) )->getName();	//query name
		
		// query_name, NN_name, True_distance, duration
		InfoCurve* item = new InfoCurve( q, id, min);
		// not all of info available ( will use modifiers later)		
		
		info->push_back(item);
		item = NULL;		// don't call destructor


		id.clear();		

		min = -1;	//reset value
	}

//	cout << "Queries->size() " << queries->size() << endl; 
	
	cout << "Finished Curve_NN_True" << endl;
		
	return m;	
}

*/






/* Not needed in project2
int Curve_NN_True1(vector<Curve*>* curves, vector<Curve*>* queries, vector<InfoCurve*>* info){

	/* Brute force, dist with DTW 
	   Also compute M = max(m1,m2,m3,...,mk) of all input-queries
	*

	double min = -1;		 
	unsigned int M = curves->at(0)->getP()->size();	 // m will be minimum number of points in dataset

	
	unsigned int i = 0;
	unsigned int j = 0;
	
	string id;
	
	double temp = 0.0;

		
	for( i = 0; i < queries->size(); i++){

//		cout << "Query " << i <<" has " << queries->at(i)->getP()->size() <<" points and m is " << m <<" \n";
		
		
		// for M (max length of curves )
		if( M < queries->at(i)->getP()->size() ) M = queries->at(i)->getP()->size();

		
		for( j = 0; j < curves->size(); j++){


//			cout << "Comparing Query " << i <<" with input " << j << endl;
	
			temp = DTW( queries->at(i)->getP(), curves->at(j)->getP());
			

	
			if( (min < 0) || (temp < min) ){
			
				min = temp;
				id = ( curves->at(j) )->getName();	// (Curve*)->getName()	
			}
			
			// for delta
			if( M < curves->at(j)->getP()->size() ) M = curves->at(j)->getP()->size();
			
//			cout << "Curve " << j <<" has " << curves->at(j)->getP()->size() << endl;
		
		}
		
//		cout << "Min is " << min << endl;
//		cout << "NN is curve with id " << id << endl; 
		
		string q = (queries->at(i) )->getName();	//query name
		
		// query_name, NN_name, True_distance, duration
		InfoCurve* item = new InfoCurve( q, id, min);
		// not all of info available ( will use modifiers later)		
		
		info->push_back(item);
		item = NULL;		// don't call destructor


		id.clear();		

		min = -1;	//reset value
	}

//	cout << "Queries->size() " << queries->size() << endl; 
	
	cout << "Finished Curve_NN_True" << endl;
		
	return M;	
}

*/





/* Not needed in project2

void Print_Curve(vector<InfoCurve*> *info, string filename){

	ofstream output;
	output.open(filename.c_str(), ios::out | ios::trunc );
	
	unsigned int i = 0;
	
	for( i = 0; i < info->size(); i++){
	
		output << "Query: " << info->at(i)->getQ() << endl;
		output << "Method: LSH" << endl;
		output << "Hashfunction: LSH" << endl;
		output << "Nearest neighbor: " << info->at(i)->getNN() << endl;
		output << "True Nearest neighbor: " << info->at(i)->getNN_True() << endl;
		output << "distanceFound: " << info->at(i)->getLSH_d() << endl;
		output << "distanceTrue: " << info->at(i)->getTrue_d() << endl;		
		output << endl << endl;	
	}
	output.close();
}

*/



// Testing 
void Print_Vector(Vector* v){

	vector<double>* c = v->getC();
	
	
	for( int i = 0; i < c->size(); i++){
	
		cout << "v[" << i <<"] is "<< c->at(i) <<" ";			
	}

	cout << endl <<endl;

}





















