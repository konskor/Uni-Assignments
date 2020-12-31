#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include <string.h>
#include <stdlib.h>
#include <ctime>

#include <random>
#include <chrono>

#include <vector>

#include "Vector.hpp"
#include "Utilities.hpp"
#include "InfoItem.hpp"
#include "LSH_HT.hpp"



using namespace std;


void Insert2Vector(vector<Vector*>* v, string filename);
void Max_fraction(vector<InfoItem*> *info);
void AverageLSH_time(vector<InfoItem*> *info);




int main(int argc,char** argv){

	if( argc < 7){
	
		cout << "Not enough arguments need 7\n";
		return -1;
	}
	
	
	string input_file,query_file,output_file;
	int k = 4;
	int L = 5;
	
	for(int i = 0; i < argc; i++){

		if( strcmp(argv[i] , "-d") == 0 ){		// Input file
		
			if( i == argc-1){
				cout << " Something is wrong: -d ..... \n";
				return -1;		
			}
		
			input_file = argv[i+1];			
			continue;
		}

		if( strcmp(argv[i] , "-q") == 0 ){		// Query File
		
			if( i == argc-1){
				cout << " Something is wrong: -q ..... \n";
				return -1;		
			}
		
			query_file = argv[i+1];			
			continue;
		}
		
		if( strcmp(argv[i] , "-k") == 0 ){		// number of hi (optional)
		
			if( i == argc-1){
				cout << " Something is wrong: -k ..... \n";
				return -1;		
			}
			
			k = atoi( argv[i+1] );
			continue;	
		}
		
		if( strcmp(argv[i] , "-L") == 0 ){		// number of hash Tables (optional)
		
			if( i == argc-1){
				cout << " Something is wrong: -L ..... \n";
				return -1;		
			}
			
			L = atoi( argv[i+1] );
			continue;
		}		


		if( strcmp(argv[i] , "-o") == 0 ){		// Output File
		
			if( i == argc-1){
				cout << " Something is wrong: -o ..... \n";
				return -1;		
			}
		
			output_file = argv[i+1];
			continue;
		}

	}
	
	bool end_flag = false;


while( end_flag == false){
	
	/* Creating the required vectors */
	vector<Vector*> *points = new vector<Vector*>;	// create empty vectors
	vector<Vector*> *queries = new vector<Vector*>;
	vector<InfoItem*> *info = new vector<InfoItem*>;
	
	
	Insert2Vector(points,input_file);
	cout << "Finished inserting points into vector\n";
	
	Insert2Vector(queries,query_file);
	cout << "Finished inserting queries into vector\n"; 
	
////////////////////////////////////////////////////////////////////

	cout << "Starting Compute_w" << endl;
	
	/* This is the brute force method , comment if dataset is big */	
	
	double w = NN_True(points,queries,info);		
	w = Compute_w(points);
	
	/* Remove comments if dataset is big and  brute force is too slow ( we put w mannually)
	
		unsigned int p = 0;
		
		for( p = 0; p < queries->size(); p++){
		
			InfoItem* ift = new InfoItem(queries->at(p)->getName());		
		
			info->push_back(ift);
			ift = NULL;
		}
		
		double w = 4100,31;	
	*/
	
	/* w is computed by Compute_w => average dist NN of all points (by Xamodrakas) */
//	w = 4 * (3492.28/3);			/* IMPORTANT  dont forget to comment (testing for now*/
//	w = 4100.31;
//	w = 4*(w/10);	//because in w
//	w = Compute_w2(points);
	
	cout << "Finished Compute_w ==> w is " << w << endl;

	/* Starting LSH */
	
	cout << " Creating necessary objects for LSH \n";
	
	int numbuckets = (points->size())/ 8;	// n/8

//	int numbuckets = (points->size())/ 16;	// n/16

	
	vector<double>* c = points->at(0)->getC();
	int d = c->size();
	c = NULL;
	
	
	LSH_HT** ht = new LSH_HT*[L];
	
	for(int i = 0; i < L; i++){
	
		random_device rd;
		mt19937 generator(rd());	//Standard mersenne_twister_engine seeded with rd()
		
		std::uniform_real_distribution<double> dis(0.0, w);
	
		ht[i] = new LSH_HT(numbuckets,k,d,w,generator,dis);		
	}
	
	cout << "Built our new empty LSH_HT\n";	
	
	/* Inserting points */
	
	cout << "Inserting the points \n";
	
	for(int i = 0; i < L; i++) ht[i]->Insert(points);		// Insert the points
	 
	cout << "Finished inserting \n";
	cout << "Started Searching \n";
	
	unsigned int i = 0;
	int j = 0;
	
	/* Searching apporixmate neigbor using LSH */
	
	for(i = 0; i < queries->size(); i++){	// for all queries

		double t1 = Timestamp();

		for( j = 0; j < L; j++){	// Search all HashTables
			ht[j]->Search_NN(queries->at(i),info->at(i), L);	
		}

		double t2 = Timestamp();
		double duration = t2 - t1;
		
		info->at(i)->setLSH_t(duration);	
	}
	
	
	cout << "Finished  Searching for Nearest Neighbors (LSH)\n";
	
	
	Print_vec(info,output_file);
	
	cout << "Finished writing the statistics to output ==> Cleaning up\n";	
	
//////////////////////////////////////////////////////////////////

	Max_fraction(info);
	AverageLSH_time(info);

/////////////////////////////////////////////////////////////////
	
	/* Cleaning up */
	
//	unsigned int i = 0;
	for( i = 0; i < points->size(); i++){
		delete points->at(i); 
		points->at(i) = NULL;		// deleting all the Vectors (dimension d)
	}
	delete points;	// finally delete vector
	
///////////////////////////////////////////////////////////////////////////////////
	
	for( i = 0; i < queries->size(); i++){
		delete queries->at(i); 
		queries->at(i) = NULL;		// deleting all the Vectors (dimension d)
	}
	delete queries;	// finally delete vector
	
////////////////////////////////////////////////////////////////////////////////	
	
	for( i = 0; i < info->size(); i++){
		delete info->at(i); 
		info->at(i) = NULL;		// deleting all the Vectors (dimension d)
	}
	delete info;	// finally delete vector
	
	
	for( i = 0; i < L; i++) delete ht[i];
	delete[] ht;
	
	/* Check user input */
	string s1;
	bool another_input = false;
	
	string s2;
	bool another_query = false;
	
	cout << "Would you like to try with a diffent dataset? (yes/no)\n";
	cin >> s1;
	if( another_input == true){
	
		cout << "Please enter input filename of input (one word)\n";
		cin >> input_file;	
	}
	
	cout << "Would you like to try with a diffent query set? (yes/no)\n";
	cin >> s1;
	if( another_query == true){
	
		cout << "Please enter input filename of query (one word)\n";
		cin >> query_file;	
	}
	
	if( (another_input == false) && (another_query == false) ) end_flag = true;
	
	
}// end of function =>exit		
	
	cout << "Finished cleaning up, exiting \n";
	
//	cout << "W is " << w << endl;
	
	return 0;
}
	
	
	
	
	
	

void Insert2Vector( vector<Vector*>* v, string filename){

	ifstream myfile;
	myfile.open(filename.c_str());
	
	/* Handling input_file */
		
	string line;
	string token;	
	
	while(getline(myfile,line)){	//read file line by line
	
		stringstream line1(line);

		vector<double>* coordinates = new vector<double>;
		string id;
		
		int i = 0;	
		
		while( getline(line1,token,' ') ){
		
			if(token.find_first_not_of("abcdefghijklmnopqrstuvwxyz.0123456789") != string::npos){
				continue;	// not the characters we want
			}
		
//			cout << "Token is "<< token << endl;	
			if( i == 0){		//first one is the ID of Vector
				id = token;
				i++;
				continue;			
			}
			
			//else we have coordinates
						
			double c = stod(token,NULL);
			coordinates->push_back(c);
			
			token.clear();		
		}		
		
		Vector* v1 = new Vector(id, coordinates);
		v->push_back(v1);
		
		v1 = NULL;	// don't call destructor
		coordinates = NULL;	
	}
	
	myfile.close();
	
//	cout << "Finished reading input file" << endl;

}	
	
	
	
	
	
void Max_fraction(vector<InfoItem*> *info){

	unsigned int i = 0;
	
	double lsh_d = 0.0;
	double true_d = 0.0;
	double f = 0.0;
	
	
	double max = -1.0;
	double sum = 0.0;
	
//	vector<double>* v = new vector<double>;
	
	
	for( i = 0; i < info->size(); i++){
	
		lsh_d = info->at(i)->getLSH_d();
		true_d = info->at(i)->getTrue_d();
		
		f = lsh_d / true_d;
		
		sum = sum + f;
		
//		v->push_back(f);
		
		if( f > max) max = f;	
	}

	cout << "Max{lsh_d/true_d} is " << max << endl;
	
	
	sum = sum/ (info->size());
	
	cout<<" Average of lsh_d/true is " << sum << endl;	
}	
	
	
	
	
	
void AverageLSH_time(vector<InfoItem*> *info){

	unsigned int i = 0;

	double temp = 0.0;
	double sum = 0.0;
	
	double total = 0.0;
	
	for( i = 0; i < info->size(); i++){
	
		temp = info->at(i)->getLSH_t();
		sum = sum + temp;	
	}
	
	total = sum/(info->size());
	
	cout << "Average searching time is " << total/1000 << endl;
}	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
