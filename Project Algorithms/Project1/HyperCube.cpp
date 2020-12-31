#include <iostream>
#include <string>
#include <chrono>
#include <random>

#include <vector>
#include <unordered_map>

#include <assert.h>

#include "Vector.hpp"
#include "InfoItem.hpp"
#include "Utilities.hpp"

#include "HyperCube.hpp" 
#include "Vector_Node.hpp"

#include "Curve.hpp"
#include "InfoCurve.hpp"

#include "Utilities2.hpp"





HyperCube::HyperCube(int k1,int d1,int bh_d1, int M1, int probes1, double w1){

	k = k1;
	d = d1;
	bh_d = bh_d1;
	M = M1;
	probes = probes1;
	w = w1;

	numbuckets = pow(2,bh_d);

	count0 = 0;
	count1 = 0;

//	cout << "bhd is " << bh_d << endl;

	/* Creating our random generator of double to fill our 3d table s */
	s = new double**[bh_d];
	
	for( int i = 0; i < bh_d; i++){	// for all g(x)
	
		random_device rd;
		mt19937 generator(rd());	//Standard mersenne_twister_engine seeded with rd()
		
		std::uniform_real_distribution<double> dis(0.0, w);
	
		s[i] = new double*[k];
	
		for(int j = 0; j < k; j++){	// for  all h(x) ( of each g(x) )
		
			s[i][j] = new double[d];
		
			for(int l = 0; l < d; l++) s[i][j][l] = dis(generator);	//fill the table
		}
	}
	
	/* Initialise the 0/1 generator */
	
//	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();	
//	dis = uniform_int_distribution<int>(0,1);
//	gen = mt19937(seed);
	
	std::random_device rd;
	gen = mt19937(rd());
	dis = uniform_int_distribution<int>(0,1);
	
	
	
	/* Initialise vector and unordered map */
	
	index = new unordered_map<string,int>;
	buckets = new vector<Vector_Node*>*[numbuckets];
	for( int i = 0; i < numbuckets; i++) buckets[i] = NULL;	

}



HyperCube::~HyperCube(){

	/* Deleting 3-dimensional table s */
	for( int i = 0; i < bh_d; i++){
	
		for(int j = 0; j < k; j++) delete s[i][j];
		
		delete s[i];	
	}
	delete[] s;	// finally delete s
	
	/* Deleting unordered map */
	delete index;	// finally delete unordered map
	
	/* Deleting buckets (without deleting */
	for( int i = 0; i < numbuckets; i++) delete buckets[i];
	delete[] buckets;	
}

		
int HyperCube::getnumBuckets(){ return numbuckets;}
int HyperCube::getD(){ return d;}
int HyperCube::getK(){ return k;}
int HyperCube::getBh_d(){ return bh_d;}

/* Testing */
double*** HyperCube::getS(){ return s;}
unordered_map<string,int>* HyperCube::getIndex(){ return index;}

//vector<Vector*>** HyperCube::getVector(){ return buckets;}
vector<Vector_Node*>** HyperCube::getVector(){ return buckets;}

/* 0/1 Generator */
int HyperCube::binary(){

	int b = dis(gen);	// create 0 or 1 (uniformly)
	
	if( b == 0) count0++;
	else if( b == 1) count1++;
	
	return b;
}



int HyperCube::getBucket(string g){ 

//	int b = stoi(g);
	
//	cout << "For g = " << g << " is (in int) " << b << endl;

	return stoi(g,0,2);

}







//string instead of int
string HyperCube::Hash(Vector *v){

	string result;
	
	int *a = new int[d];
	
	int i = 0;
	int j = 0;
	int l = 0;
	
	vector<double>* vd = v->getC();		// the xi
	
	double x = 0.0;
	double temp = 0.0;
	
	unsigned int M = pow(2,32/k);
	
	int count12 = 0;
	
	unsigned int* h = new unsigned int[k];
	
	for( i = 0; i< this->bh_d; i++){		// for all g(x)
		
		for( j = 0; j < k; j++){	//for all h(x)


			for( l = 0; l < d; l++){ //for all dimensions
				x = vd->at(l);
				temp = ( x - s[i][j][l] ) / w;
									
				a[l] = floor(temp);		
			}	
								
			// calculated all a[i]	
		
			int m = Max(a,d) + 1;		// find max element
			
			m = pow(2,31)-5;	

			int temp = 0;
			int temp1 = 0;
			int sum = 0;
			
		
			for( l = 0; l < d; l++){	// Calculate the sum ==>h[i]
				
				temp = Mod(a[l],M);		
				temp1 = alt_pow(m, d-1-l,M);			
				sum = Mod( sum + Mod( temp1*temp, M) , M);		
			}	
		
			h[j] = sum;			
		}
	
		string str;
	

		/* This is updated */
		int size = 32/k;
	
		int sum = 0;
	
		//Concatenate string
		for(int j = 0 ; j < k; j++){ 
	
			int num = h[j];

			int sh_amount = size * (k - (j+1) ) ;		// 
		
			int num1 = num;
			if( sh_amount > 0 ) num1 = num << sh_amount;
		
			sum = sum | num1;
			
		}
		str.append(to_string(sum));

		/* G(x) is complete */

		/* Search if the string has been apointed a value (0 or 1) */
		
		int temp2 = -1;	//dummy value
		
//		int iter = index->find(str);

		auto iter = index->find(str);
		
		if( iter == index->end() ){	// (key = string) not found
		
			temp2 = this->binary();		//binary will return a 0 or 1
			
			index->insert(make_pair(str,temp2) );			

		}else{	//key exists
		
			temp2 = iter->second;

		}
		

		count12++;	
		
		result.append( to_string(temp2) );	// append to result	
		
		
	}
	// We have found the binary string 

	delete[] h;
	delete[] a;
	 
	v = NULL;
	
	return result;	// string
	
}



/* Insert Vector  */
void HyperCube::Insert( vector<Vector*> *v){

	unsigned int i = 0;
	
	for( i = 0; i < v->size(); i++){
	
		Vector* x = v->at(i);
		
		string b = this->Hash(x);
		int bucket = this->getBucket(b);

		if( buckets[bucket] == NULL) buckets[bucket] = new vector<Vector_Node*>;// create empty vector

		Vector_Node* v1 = new Vector_Node(x,b);
		buckets[bucket]->push_back(v1);
		
		x = NULL;
		v1 = NULL;	
	}	
	// finished
}




/* Insert Vector  */
void HyperCube::Insert1( vector<Vector_Node*> *v){

	unsigned int i = 0;
	
	for( i = 0; i < v->size(); i++){
	
		Vector* x = v->at(i)->getV();
		
		string b = this->Hash(x);
		int bucket = this->getBucket(b);

		if( buckets[bucket] == NULL) buckets[bucket] = new vector<Vector_Node*>;// create empty vector
		
		buckets[bucket]->push_back(v->at(i));
		
		x = NULL;
	}	
	// finished
}




void HyperCube::Hamming_Distance(vector<string>* vs, string str, int i, int remaining){

	if( remaining == 0){
		vs->push_back(str);
		return;	
	}
	if( i < 0) return;
	
	/* Check both possibilities: 
		a) Change i'th bit
		b) Don't change i'th bit
	*/
	
	string original = str;
	
	// Change the i-th bit	
	if( str[i] == '0'){
		
		str[i] = '1';
	
	}else{	// it is '1'
		str[i] = '0';
	}
	Hamming_Distance(vs,str,i-1,remaining-1);
	
	//Revert the changes
	str = original;		
	Hamming_Distance(vs,str,i-1,remaining);
}





/* Search for vector */
void HyperCube::Search( Vector* query, InfoItem* info){
	
//	double min = info->getLSH_d();
	double min = -1;	
	unsigned int j = 0;
	double temp = 0.0;
	
	string id;

	string b = this->Hash(query);	
	
	
	/* Generating all the required hash codes (Hamming distance) */
	string original = b;
	vector<string>* vec = new vector<string>;
	vec->push_back(original);
	
	int length = original.length();
	int max = original.length();
	
	string modified = original;
	for(int i = 0; i < max; i++){
	
		this->Hamming_Distance(vec,modified,length-1,i);	
	}	

	/* Now vec has all the hash codes we need
		a)Starting from original 
		b) To hash codes with hamming distance = 1,2,3...
		Progressively.
	*/	
	


	int count = 0;	// count how many points we have checked

	int amount = probes;
	
	if( probes > vec->size() ) amount = vec->size();	// to avoid any problems

	/* Now start searching */	
	for( int i = 0; i < amount; i++){
	
		string hash_c = vec->at(i);
		unsigned int bucket = this->getBucket(hash_c);

		if( buckets[bucket] == NULL) continue;	// nothing here search nearby
		
		for( j = 0; j < buckets[bucket]->size(); j++){

			temp = Vector_distance( query, buckets[bucket]->at(j)->getV(), 1 );
			// 1 for Manhattan, 2 for Euclidean, etc.

			count++;	// increment counter
	
			if( (min < 0) || (temp < min) ){
			
				min = temp;
				id = ( buckets[bucket]->at(j)->getV() )->getName();	// (Vector*)->getName()	
			}


//Test commenting/uncommenting
			if( count == M) break;  // Searched M points ==> enough		


		}
		
		if( count == M) break;
		
				
	}
	//Edit the info
	info->setLSH_d(min);
	info->setLSH_NN(id);		// approximate NN name
	
	delete vec;
}
























/* Search for vector */
void HyperCube::Search1( Vector_Node* query, InfoCurve* info){
	
	double min = info->getLSH_d();
	
//	double min = -1;	
	unsigned int j = 0;
	double temp = 0.0;
	
	string id;

	string b = this->Hash(query->getV());	
	
//	cout << "Hello" <<endl;
	
	/* Generating all the required hash codes (Hamming distance) */
	string original = b;
	vector<string>* vec = new vector<string>;
	vec->push_back(original);
	
	int length = original.length();
	int max = original.length();
	
	string modified = original;
	for(int i = 0; i < max; i++){
	
		this->Hamming_Distance(vec,modified,length-1,i);	
	}	

	/* Now vec has all the hash codes we need
		a)Starting from original 
		b) To hash codes with hamming distance = 1,2,3...
		Progressively.
	*/	
	

//	cout << "Hello1" <<endl;


	int amount = probes;
	
	if( probes > vec->size() ) amount = vec->size();	// to avoid any problems

	/* Now start searching */
//	for( int i = 0; i < probes; i++){

//	cout << "HELLO2\n";
	
	int count = 0;
	
	for( int i = 0; i < amount; i++){
	
//		cout << "HELLO3\n";
	
		string hash_c = vec->at(i);
		unsigned int bucket = this->getBucket(hash_c);

		if( buckets[bucket] == NULL) continue;	// nothing here search nearby
		
//		int count = 0;		// count how many points we have checked
		
		assert( buckets[bucket]->size() != 0 );
		
		for( j = 0; j < buckets[bucket]->size(); j++){

//			cout << "Hello4\n";
			Vector_Node* v = buckets[bucket]->at(j);
			
			/* check if they have the same grid curve
			
			Curve* c1 = query->getGrid();
			Curve* c2 = v->getGrid();
			
			if( Same_Grid(c1,c2) == false){	// compare items with same g(x) and Grid curves
		
				cout << "Different Grid curves, continue\n";
		
				v = NULL;
				c1 = NULL;
				c2 = NULL;
				continue;		
			}
			// Same Grid Curves
			*/
		
			Curve* c1 = query->getReal();
			Curve* c2 = v->getReal();		
					
			temp = DTW(c1->getP(), c2->getP());
			
			count++;
			
//			cout << "HELLO5s\n";
	
			if( (min < 0) || (temp < min) ){
			
				min = temp;
//				id = ( buckets[bucket]->at(j) )->getName();	// (Vector*)->getName()	
				id = ( buckets[bucket]->at(j)->getV() )->getName();	// (Vector*)->getName()	
			}
			
			c1 = NULL;
			c2 = NULL;
			v = NULL;


//Test commenting/uncommenting
			if( count == M) break;  // Searched M points ==> enough		


		}
		
		
		if(count == M) break; 	// maximum number of vectors to check (all buckets)
		
				
	}
	
//	cout << "Hello2\n";

	//Edit the info
//	info->setLSH_d(min);
//	info->setLSH_NN(id);		// approximate NN name
	
	/*  Edit the InfoItem */
	double prev = info->getLSH_d();
		
	if(  (prev < 0) || (min < prev) ){		//found new min
			
		info->setLSH_d(min);
		info->setLSH_NN(id);		// approximate NN name	
	}
	
	delete vec;
	
//	delete[] str;
//	str = NULL;	
}








































