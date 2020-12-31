#include <iostream>
#include <string>
#include <cmath>

#include <random>

#include <vector>

#include <cassert>


#include "Curve_LSH.hpp"
#include "Curve.hpp"
#include "Utilities2.hpp"

#include "LSH_HT.hpp"
#include "HyperCube.hpp"


/* Constructor/Destructor */

Curve_LSH::Curve_LSH(int d1,double del,mt19937 generator, uniform_real_distribution<double> dis){

	d = d1;
	delta = del;
	
	t = new double[d];
	for(int i = 0; i < d; i++) t[i] = dis(generator);
	
	ht = NULL;
	htc = NULL;
	
	grid_curves = new vector<Curve*>;
	grid_points = new vector<Vector_Node*>;	
	
	vector_cleanup = new vector<Vector*>;	
	
	max_c = -1000000;
	max_d = 0;	
}




Curve_LSH::~Curve_LSH(){

	delete[] t;
	t = NULL;
	
	if(ht != NULL) delete ht;
	if( htc != NULL) delete htc;
	
	
	for(unsigned int i = 0; i < grid_curves->size(); i++) delete grid_curves->at(i);
	delete grid_curves;
	
	delete grid_points;	// the vector_nodes are already deleted in (delete ht/htc)
	
	for(unsigned int i = 0; i < vector_cleanup->size() ; i++) delete vector_cleanup->at(i);
	delete vector_cleanup;	
}



/* Accessors */
double* Curve_LSH::getT(){ return t; }
LSH_HT* Curve_LSH::getHT(){ return ht; }
HyperCube* Curve_LSH::getHTc(){ return htc; }

vector<Vector_Node*>* Curve_LSH::getGpoints(){ return grid_points; }




/* Modifiers */

void Curve_LSH::setLSH_HT(int k1, double w, mt19937 generator, uniform_real_distribution<double> dis){

	int numbuckets = grid_points->size()/8;
	int d1 = grid_points->at(0)->getV()->getC()->size();
	
//	double w = Compute_w_curve(grid_points);	// this is Computed in main	
//	double w = 4100.31;
	
	this->ht = new LSH_HT(numbuckets,k1,d1,w,generator,dis);
}




void Curve_LSH::setHTc(int k1,int bh_d1, int M1, int probes1,double w1){

	int d1 = grid_points->at(0)->getV()->getC()->size();

	this->htc = new HyperCube(k1,d1,bh_d1,M1,probes1,w1);
}










void Curve_LSH::Insert(){

	//Insert grid points into Hash Table
	
	if( ht != NULL){
	
		ht->Insert1(grid_points);
	
	
	}else if( htc != NULL){
	
		htc->Insert1(grid_points);
	
	
	}else{
	
		cout << "ERROR\n";	
	}
	
	


}















// Curve to Grid_Curve 
void Curve_LSH::Curve2Gcurve(vector<Curve*>* vc){


	double max = -100000;		//max coordinate of grid points

	for( unsigned int i = 0; i < vc->size(); i++){		// for all real curves
	
		// Create Grid curve 
		
		vector< vector<double>* >* points = vc->at(i)->getP();	// the points of our curve
		int numofpoints = points->size();

		double temp = 0;
		vector<vector<double>*>* a = new vector<vector<double>*>;// the grid points of our Gcurve
	
		for(int l = 0; l < numofpoints; l++){	// for each point in curve => find Grid point
		
			vector<double>* a1 = new vector<double>;	//coordinates of ai
	
			for( int j = 0; j < d; j++){		// Compute ai(j)
		
				double x = ( points->at(l) )->at(j);	// for point(i) get coordinate(j)
			
//				cout << "X is " << x << endl;
			
				temp =  round( (x - t[j] ) / delta );
//				temp = (x - t[j] ) / delta ;		// CHANGED
				
				double temp1 = temp + t[j];
											
//				a1->push_back(temp);
				a1->push_back(temp1);
			
				if( a1->at(j) > max) max = a1->at(j);
			}

			// Check if point p(i)' == p(i-1)' 
			if( l > 0){

				int size = a->size();
				bool check = Same_vectors(a->at(size-1),a1);	// check if same coordinates
			
				if( check == true){	//same point as previous ==> ignore
			
//					cout << "Same Grid points, continue \n";
					delete a1;
					a1 = NULL;
					continue;			
				}
				// Else a different point				
			}
		
			a->push_back(a1);	//insert Grid point into Grid curve	
			a1 = NULL;
	
		}// Found all the grid points ==> Creating Grid curve
	
		string ss = "a";	// dummy name
		 
		Curve* gcurve = new Curve(ss,a);		
		this->grid_curves->push_back(gcurve);

		a = NULL;
		points = NULL;
		gcurve = NULL;	
	}
	
//	cout << "Finished Curve2GCurve\n";
	
	
	this->max_c = max;	// setting max coordinates of this Grid
	
	this->Gcurve2Vector(vc);	// Create Grid Points
}



// Curve to Grid_Curve
void Curve_LSH::Gcurve2Vector(vector<Curve*>* vc){

	int maxd1 = 0;

	for( unsigned int i = 0; i < grid_curves->size(); i++){	// for all grid curves
	
		vector< vector<double>* >* v1 = grid_curves->at(i)->getP();	// get grid points
	
		vector<double>* v2 = new vector<double>;	// create new coordinates
	
		for(unsigned int l = 0; l < v1->size(); l++ ){	// for all points in Grid curve
	
			vector<double>* temp = v1->at(l);	// coordinates of grid point(l)	
	
			for( unsigned int j = 0; j < temp->size(); j++){// for all coordinates of each point
		
				double x = temp->at(j);
				v2->push_back(x);		// Add coordinate to vector		
		
//				cout << "X is " << x << endl;
			}
			temp = NULL;
		}
		// Finished ==> Creating the vector
		
		string ss = "a";	//dummy name
		Vector* vec = new Vector(ss,v2);	
		Vector_Node* vec1 = new Vector_Node(vec,vc->at(i),grid_curves->at(i));
	
		this->vector_cleanup->push_back(vec);
		this->grid_points->push_back(vec1);
	
		int dimension = v2->size();
		
		if( dimension > maxd1) maxd1 = dimension;

		vec = NULL;
		vec1 = NULL;

		v2 = NULL;
		v1 = NULL;
	}
	
//	cout << "Finished GCurve2Vector\n";
	
	this->max_d = maxd1;	//set max dimension
	
	this->Update();		// Pad the coordinates (if needed)
}













/* Used in LSH_Curve ==> Its the padding step */
void Curve_LSH::Update(){

	/* All vectors must be the same dimension -> max_d
	   If not, add in the coordinates max_c, as much as we have to
	 */

	for(unsigned int i = 0; i < this->grid_points->size(); i++){	// for all Grid vectors
	
		vector<double>* c = this->grid_points->at(i)->getV()->getC();//get coordinates of grid vector(i)
		
		int d = c->size();
		
		assert( d <= this->max_d);	// if not => STOP
		
		if( d == this->max_d){// don't do anything ( no need to pad)
		
			c = NULL;
			continue;
		}
		//Else, fix the problem
		int difference = this->max_d - d;
		
//		for(int j = 0; j < difference; j++)	c->push_back(this->max_c);
		
		for(int j = 0; j < difference; j++)	c->push_back(this->max_c + 1);
		
		this->grid_points->at(i)->getV()->setD(this->max_d);		// update dimension
		c = NULL;	
	}
	//Checked all vectors => return
	
//	cout << "Finished Update\n";
}













/**************************************************************
***************************************************************
***************************************************************
***************************************************************/

// This is for queries 

// Curve to Grid_Curve
void Curve_LSH::Curve2Gcurve1(Curve* vc, Curve** gc){

	// Create Grid curve 
		
	vector< vector<double>* >* points = vc->getP();	// the points of our curve
	int numofpoints = points->size();
	
	double temp = 0;
	
	vector<vector<double>*>* a = new vector<vector<double>*>;// the grid points of our Gcurve
	
	
	for(int i = 0; i < numofpoints; i++){	// for each point in curve => find Grid point
		
		vector<double>* a1 = new vector<double>;	//coordinates of ai
	
		for( int j = 0; j < d; j++){		// Compute ai(j)
		
			assert( j < 2);
		
			double x = ( points->at(i) )->at(j);	// for point(i) get coordinate(j)
			
//			cout << "X is " << x << endl;
			
			temp =  round( (x - t[j] ) / delta );
//			temp = (x - t[j] ) / delta ;	// CHANGED
			
			double temp1 = temp + t[j];
			
						
//			a1->push_back(temp);
			a1->push_back(temp1);
		}

		// Check if point p(i)' == p(i-1)' 

		if( i > 0){
		
			int size = a->size();
			bool check = Same_vectors(a->at(size-1),a1);	// check if same coordinates
			
			if( check == true){	//same point as previous ==> ignore
			
//				cout << "Same Grid points, continue \n";
				delete a1;
				a1 = NULL;
				continue;			
			}
			// Else a different point				
		}
		
		a->push_back(a1);	//insert Grid point into Grid curve	
		a1 = NULL;
	
	}// Found all the grid points ==> Creating Grid curve
	
	string ss = "a"; 
	
	*gc = new Curve(ss,a);		

	a = NULL;
	points = NULL;
}





// Curve to Grid_Curve
void Curve_LSH::Gcurve2Vector1(Curve* gc, Vector** v){

	/* Return dimension of vector */
	
	vector< vector<double>* >* v1 = gc->getP();	// get grid points
	
	vector<double>* v2 = new vector<double>;	// create new coordinates
	
	for(unsigned int i = 0; i < v1->size(); i++ ){	// for all points in Grid curve
	
		vector<double>* temp = v1->at(i);	// coordinates of grid point	
	
		for(unsigned int j = 0; j < temp->size(); j++){	// for all coordinates of each point
		
			double x = temp->at(j);
			v2->push_back(x);		// Add coordinate to vector		
		}
		temp = NULL;
	}
	// Finished ==> Creating the vector
	string ss = "a";	//dummy name
	*v = new Vector(ss,v2);

	v2 = NULL;
}






/* Used in LSH_Curve ==> Its the padding step (for queries) */
void Curve_LSH::Update1(Vector* v){

	/* All vectors must be the same dimension -> max_d
	   If not, add in the coordinates max_c, as much as we have to
	 */
	
	vector<double>* c = v->getC();//get coordinates of grid vector(i)
	int d = c->size();
		
//	assert( d <= this->max_d);	// if not => STOP

	if( d == this->max_d){// don't do anything ( no need to pad)
		
		c = NULL;
		return;
	}

	
	if( d > this->max_d){	// this query has more coordinates than permitted=> remove them
	
		int difference = d - this->max_d;
		
		for(int j = 0; j < difference; j++)	c->pop_back();
		v->setD(this->max_d);		// update dimension
		c = NULL;
		return;
	} 
	//Else, We have less coordinates than permitted
	int difference = this->max_d - d;
		
	for(int j = 0; j < difference; j++)	c->push_back(this->max_c + 1);
		
	v->setD(this->max_d);		// update dimension
	c = NULL;	
	
	
//	cout << "Finished Update\n";
}

























