#include <iostream>

#include "LSH_HT.hpp"


using namespace std;

LSH_HT::LSH_HT( int nbuckets, int k1, int d1, double w1, mt19937 generator, uniform_real_distribution<double> dis){

	numbuckets = nbuckets;
	k = k1;
	d = d1;
	
	w = w1;
	
	
	// Creating vector of pointers to buckets (all NULL)	
	int i = 0;

	
	buckets = new vector<Vector_Node*>*[numbuckets];	// vector of pointers -> vector(STL) of pointers => vectors(points of dataset)
	
	
	for( i = 0; i < numbuckets; i++) buckets[i] = NULL;
	
	// w will have already been computed (not in function below)
	
	this->Manhattan_LSH(w,generator,dis);
}



/* Destructor */
LSH_HT::~LSH_HT(){

	int i = 0;
	
	for( i = 0; i < numbuckets; i++){		
// remove the list wihout calling destructor for the Vector* pointers (will do that later)
		 
		 delete buckets[i];
		 buckets[i] = NULL;
	}
	delete[] buckets;
	buckets = NULL;
	
	for( i = 0; i < k; i++){
		delete s[i];
		s[i] = NULL;
	}	
	delete[] s;
	s = NULL;
}

/* Accessors */

int LSH_HT::getnumBuckets(){ return numbuckets;}

int LSH_HT::getK(){ return k;}

int LSH_HT::getD(){ return d;}

double** LSH_HT::getS(){ return s;}

// Testing
vector<Vector_Node*>** LSH_HT::getB(){ return buckets;}




void LSH_HT::Manhattan_LSH(double w, mt19937 generator, uniform_real_distribution<double> dis){

// w is already computed ( after exact Nearest neighbor )

	int i = 0;
	int j = 0;
	
	s = new double*[k];		// creating the array of vectors s ( k x d)

	for( i = 0; i < k; i++){		// Create k random real vectors of dimension d
	
		s[i] = new double[d];
	
		for( j = 0; j < d; j++){
	
			s[i][j] = dis(generator);	
			 
		}	
	}
}




/* find the g(x)*/

//string instead of int
string LSH_HT::Hash(Vector *v){

//	cout <<"Hashing vector with id " << v->getName() << endl;
//	cout << "s[0][0] is " << s[0][0] << " s[1][0] is " << s[1][0] << endl;
//	assert( s[0][0] != s[1][0] );


	string result;
	
	int *a = new int[d];
	
	int i = 0;
	int j = 0;
	
	vector<double>* vd = v->getC();		// the xi
	
	double x = 0.0;
	double temp = 0.0;
	
	unsigned int M = pow(2,32/k);	
	
	unsigned int* h = new unsigned int[k];
	
//	cout<< "D is " << d << endl;	
	
	for( i = 0; i< k; i++){		// for all hash functions hi(x)
	
		for( j = 0; j < d; j++){	//for all dimensions

				x = vd->at(j);
				temp = ( x - s[i][j] ) / w;
				
				a[j] = floor(temp);
		}
		// calculated all a[i]


//		for( int i = 0; i < d; i++){
		
//			cout<< "a[" << i << "] is " << a[i] << endl;
		
//		}
		int m = Max(a,d) + 1;		// find max element
		m = pow(2,31)-5;

		int temp = 0;
		int temp1 = 0;
		int sum = 0;
		

		
//		cout << "Calculating h[i]" << endl;
		
		for( j = 0; j < d; j++){	// Calculate the sum ==>h[i]
		
			temp = Mod(a[j],M);
			temp1 = alt_pow(m, d-1-j,M);
			
			sum = Mod( sum + Mod( temp1*temp, M) , M);
		}	
		
		h[i] = sum;
	}

	/* This is updated (2) */

	int size = 32/k;
	
	int sum = 0;
	string str;
	
	//Concatenate string
	for( i = 0 ; i < k; i++){ 
	
		int num = h[i];		
		int sh_amount =  size * (k - (i+1) ) ;		// 		

		int num1 = num;
		if( sh_amount > 0 ) num1 = num << sh_amount;
		
		sum = sum | num1;	// Logical OR
	}
	str.append(to_string(sum));

	delete[] h;
	delete[] a;

	return str;
}


/* Insert hash: g(x) and find bucket */
int LSH_HT::getBucket(string g){

	int bucket = stoull(g) % numbuckets;		//string to unsigned long int
	
	return bucket;
}









/* Insert Vector (inside HT) ==> for Vectors */
void LSH_HT::Insert( vector<Vector*> *v){

	unsigned int i = 0;
	
	for( i = 0; i < v->size(); i++){
	
		Vector* x = v->at(i);


		string g = this->Hash(x);
		int bucket = this->getBucket(g);	
		
//		cout << "With g = " << g << " we get bucket = " << bucket << endl;

		if( buckets[bucket] == NULL) buckets[bucket] = new vector<Vector_Node*>;		// create empty vector

		Vector_Node* V1 = new Vector_Node(x,g);
		buckets[bucket]->push_back(V1); 

		x = NULL;
		V1 = NULL;
	}
	
	// finished
}


/* Insert Vector (inside HT) ==> for Vectors */
void LSH_HT::Insert1( vector<Vector_Node*> *v){

	unsigned int i = 0;
	
	for( i = 0; i < v->size(); i++){	// for each grid vector
	
		Vector_Node* x = v->at(i);
		
//		Print_Vector(x->getV()); 

		string g = this->Hash(x->getV());
		int bucket = this->getBucket(g);

		x->setG(g);	
//		cout << "With g = " << g << " we get bucket = " << bucket << endl;
			
		if( buckets[bucket] == NULL) buckets[bucket] = new vector<Vector_Node*>;
		// create empty vector
		
		buckets[bucket]->push_back(x);
		x = NULL;	
	}
	// finished
}


/* Not needed in project2


void LSH_HT::Search_NN( Vector* query, InfoItem* info, int L){
	
	double min = info->getLSH_d();
	
	unsigned int j = 0;
	
	double temp = 0.0;
	
	string id;

	string g = this->Hash(query);
	unsigned int bucket = this->getBucket(g);
	
	int count = 0;		// count how many points we have checked
	
	if( buckets[bucket] == NULL){
		
//		info->setLSH_d(min);
//		info->setLSH_NN(id);		// approximate NN name
	
		cout << "No items found in this bucket , return from LSH_HT::Search_NN() \n";
		return;
	}
	
//	cout << "Buckets have " << buckets[bucket]->size() << " size\n";
			
	for( j = 0; j < buckets[bucket]->size(); j++){

		Vector_Node* v = buckets[bucket]->at(j);
		string g1 = v->getG();
		
		if( g != g1){
			v = NULL;
			continue;		// compare items with same g		
		}

		temp = Vector_distance( query, buckets[bucket]->at(j)->getV(), 1 );
		// 1 for Manhattan, 2 for Euclidean, etc.
	
		count++;	// increment counter
	
		if( (min < 0) || (temp < min) ){
			
			min = temp;
			id = ( buckets[bucket]->at(j)->getV() )->getName();	// (Vector*)->getName()	
		}
		
//		if( j == 3*L)  break;		// Searched 3*L + 1 points ==> enough


/* Try commenting / uncommenting *
//		if( count > 100*L) break;  
		
//		if( count > 150*L) break;

		if( count > 80*L) break;  // 100 is good and 80 
		
//		if( count > (buckets[bucket]->size()) /5) break;		// div 3 or div 5 is pretty good
		
		
	}
			
	// Edit the InfoItem
	double prev = info->getLSH_d();
		
	if(  (prev < 0) || (min < prev) ){		//found new min
			
		info->setLSH_d(min);
		info->setLSH_NN(id);		// approximate NN name	
	}
}

*/









/* Not needed in project2


void LSH_HT::Search_NN1(Vector_Node* query, InfoCurve* info, int L){

	
	
	double min = info->getLSH_d();
	unsigned int j = 0;
	
	double temp = 0.0;
	string id;
	string g = this->Hash(query->getV());
	
	unsigned int bucket = this->getBucket(g);
	query->setG(g);
	
	
//	cout << "Bucket is " << bucket << endl;
	
	int count = 0;		// count how many points we have checked
	int count1 = 0;
	
	if( buckets[bucket] == NULL){
		
//		info->setLSH_d(min);
//		info->setLSH_NN(id);		// approximate NN name
	
		cout << "No items found in this bucket , return from LSH_HT::Search_NN1() \n";
		return;
	}
	
//	cout << "Buckets have " << buckets[bucket]->size() << " size\n";	
	
	assert( buckets[bucket]->size() > 0);
			
	for( j = 0; j < buckets[bucket]->size(); j++){

		Vector_Node* v = buckets[bucket]->at(j);
		string g1 = v->getG();
		
		if( g != g1){	// compare items with same g(x)	
		
//			cout << "Different g(x) , continue\n";

			v = NULL;			
			continue;		
		}
		
		count++;
		
		/* We have the same g(x)... but what about grid Curves? => Don't care *		

		Curve* c1 = query->getGrid();
		Curve* c2 = v->getGrid();
		
		if( Same_Grid(c1,c2) == false){	// compare items with same g(x) and Grid curves
		
//			cout << "Different Grid curves, continue\n";
		
			v = NULL;
			c1 = NULL;
			c2 = NULL;
			continue;		
		}
		// Same Grid Curves
		*
		
		Curve* c1 = query->getReal();
		Curve* c2 = v->getReal();		
		
		
		temp = DTW(c1->getP(), c2->getP());
	
		count1++;	// increment counter
	
		if( (min < 0) || (temp < min) ){
			
			min = temp;
			id = ( buckets[bucket]->at(j)->getReal() )->getName();	// (Curve*)->getName()	
		}

		c1 = NULL;
		c2 = NULL;
		


/* Try commenting / uncommenting *
//		if( count > 120*L) break;  
		if( count > 100*L) break;  
//		if( count > 150*L) break;  
//		if( count > (buckets[bucket]->size()) /5) break;		// div 3 or div 5 is pretty good
		
		
	}
	
	/* Finally Done */
			
/*  Edit the InfoItem *
	double prev = info->getLSH_d();
		
	if(  (prev < 0) || (min < prev) ){		//found new min
			
		info->setLSH_d(min);
		info->setLSH_NN(id);		// approximate NN name	
	}


}

*/








// ALL OK
void LSH_HT::RangeSearch(Cluster* cluster, string gx, double r){

	Item* query = cluster->getC();	// query -> the centroid of cluster 

	unsigned int bucket = this->getBucket(gx);

	bool curve = query->isCurve();

	if( buckets[bucket] == NULL){
	
		cout << "No items found in this bucket , return from LSH_HT::RangeSearch() \n";
		return;
	}

	assert( buckets[bucket]->size() > 0);
			
	for( unsigned int j = 0; j < buckets[bucket]->size(); j++){

		Vector_Node* v = buckets[bucket]->at(j);
		string g1 = v->getG();
		
		if( gx != g1){	// compare items with same g(x)	
//			cout << "Different g(x) , continue\n";

			v = NULL;			
			continue;		
		}
		
		
		//changed
		// WAIT, maybe it is a centroid ==> ignore it
		
		if( v->getItem()->isCentroid() == true){
			//might not be needed, centroids count as assigned
		
			v = NULL;
			continue;
		}
		
		
		// this item is already assigned into a cluster
		if( v->getItem()->isAssigned() == true){
		
			v = NULL;
			continue;
		}
		
		// This item is not assigned ==> take it
		
		
		double dist = 0;
		
		if( curve == false)	dist = Vector_distance(query->getVector(), v->getV() ,1);
		else dist = DTW(query->getCurve()->getP(), v->getReal()->getP());
		
		if( dist >= r){	// not in our range ==> ignore
			v = NULL;
			continue;
		}
		
		// We are in range, check if this item is in 2 or more range balls
		
		/*********************** IMPORTANT ********************************
		
			In other words, in the same iteration (same radius)
			another centroid has claimed (not assigned, this is important) this item.
			
			An item gets ASSIGNED, ONLY at the END of each iteration.
			Meaning when the last centroid finishes its range search (from all the hash tables)
		****************************************************************************************/
		
		Item* item = v->getItem();

		double dist1 = item->getDist();	//distance of item from current cluster
		// -1 , if its not currently in any cluster
		// > 0 , if it is


		// Item is claimed by a cluster, and our query is NOT closer this item => continue
		if( (dist1 > 0) && ( dist > dist1 ) ){
		
			v = NULL;
			item = NULL;
			continue;

		}
		
		// (Last case) Item is claimed by a cluster, and our query IS closer this item => continue

		/*
		if( (dist1 > 0) && (dist < dist1) ){
			// Remove from previous cluster
			Cluster* previous = item->getCluster();
			previous->Delete_Item(item);
			
			previous = NULL;
		}
		
		cluster->Add_Item(item,dist);
		*/
		
		item->ChangeCluster(cluster,dist);
		
		item = NULL;
		v = NULL;
	}
	// Searched all the Items in the bucket, exit	
	query = NULL;

}



























