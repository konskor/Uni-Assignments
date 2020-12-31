#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include <string.h>
#include <stdlib.h>
#include <ctime>

#include <random>
#include <chrono>

#include <cassert>

#include <vector>

#include "Vector.hpp"
#include "Curve.hpp"

#include "Utilities.hpp"

#include "LSH_HT.hpp"
#include "Curve_LSH.hpp"

#include "Initialization.hpp"
#include "Assignment.hpp"
#include "Update.hpp"
#include "Evaluation.hpp"



using namespace std;




bool Read_Input(string filename, vector<Item*>* result);
void Input_Vector(ifstream* stream, vector<Item*>* items);

void Input_Curve(ifstream* stream, vector<Item*>* items);
void Input_Curve1(ifstream* stream, vector<Item*>* items);

void Read_Config(string filename, int* k, int* num_grids, int* vec_L, int* vec_k);


void Print_curve1(Curve* curve, ofstream* output);
void Print_vector1(Vector* v, ofstream* output);


//void Print_output(string combination,vector<Cluster*>* clusters, bool complete);
void Print_output( string filename, string combination, vector<Cluster*>* clusters, double clustering_time, vector<double>* s, bool complete);


int main(int argc,char** argv){

	if( argc < 9){
	
		cout << "Not enough arguments need 9 \n";
		cout << "-i, -c, -o,  ( -combi ==> combination , 122,111, etc) \n";
		return -1;
	}
	
	
	string input_file,config_file,output_file;
	
	string combination;	// this my idea ==> 
	// insert from terminal which combination you want to execute
	// 111, 112, 121, 122, 211, 212, 221, 222
	
	string initialization,assignment,update;
	
	bool complete = false;

//	int k = 4;
//	int L = 5;
	
	for(int i = 0; i < argc; i++){

		if( strcmp(argv[i] , "-i") == 0 ){		// Input file
		
			if( i == argc-1){
				cout << " Something is wrong: -i..... \n";
				return -1;		
			}
		
			input_file = argv[i+1];			
			continue;
		}

		if( strcmp(argv[i] , "-c") == 0 ){		// Config File
		
			if( i == argc-1){
				cout << " Something is wrong: -c ..... \n";
				return -1;		
			}
		
			config_file = argv[i+1];			
			continue;
		}
		
		if( strcmp(argv[i] , "-combi") == 0 ){		// Combination
		
			if( i == argc-1){
				cout << " Something is wrong: -combi ..... \n";
				return -1;		
			}
			
			combination = argv[i+1];
			
			assert( combination.length() == 3 );
			
			initialization = combination[0];
			assignment = combination[1];
			update = combination[2];
			
			assert( (initialization == "1") || (initialization == "2") );
			assert( (assignment == "1") || (assignment  == "2") );
			assert( (update == "1") || (update  == "2") );

			continue;
		}
		
		if( strcmp(argv[i] , "-complete") == 0 ){		// print contents of cluster?
			
			complete = true;
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
	

	/* Read config file */
	
	int k = 0;	// number of clusters
	
	int num_grids = 0;	// number of grids (Curve
	int num_HT = 0;	// num of Hash Tables ( Vector )
	int k_vec = 0;	// num of h(x) ( Vector)
	
	Read_Config(config_file,&k,&num_grids,&num_HT,&k_vec);	// INCOMPLETE
	
	assert( (k > 0) && ( k_vec > 0) && (num_grids > 0) && (num_HT > 0) );
	
	/* Creating the necessary vectors<> */

	vector<Item*>* original_items = new vector<Item*>;	//contains all items
	vector<Cluster*>* clusters = new vector<Cluster*>;


	/* Create Items, from input file*/
	
	cout << "Started Reading Input file\n";
	
	bool curve = Read_Input(input_file,original_items);	// INCOMPLETE
	
	cout<< "Finished Reading Input file\n";
	
	
	
	// This is a small check for k 
	if ( k > original_items->size()/2 ){
		// will create clusters with (size < 2) ==> ERROR
		
		cout << "K is too big ----->  k > num_data_points/2 \n";
		cout << "This will create clusters with size < 2 ==> problem in Silhouette \n";
		
		delete clusters;
		clusters = NULL;
		
		for(unsigned int i = 0; i < original_items->size(); i++) delete original_items->at(i);
		delete original_items;
		original_items = NULL;
	
		return -1;	
	}
	
	/* Check for duplicates  ==> IMPORTANT
	
		This is mostly for  """ input_projection6.csv """
		
		There are items which we have  { Item_Distance(item1,item2) = 0 } !!!!!!
	
		Comment Remove_Duplicates(), if you are sure that there are no such items	
	*/
	
	cout << "\n\nOriginal size is " << original_items->size() << endl;
	
	Remove_Duplicates(original_items);
	
	cout << "\n\nAfter removing duplicates " << original_items->size() << endl;
	
	
	
	// Copy of original_items
	vector<Item*>* items = new vector<Item*>;
	*items = *original_items;

	//testing
	int original_size = original_items->size() - k;
	

	/* Our LSH data structures */

	LSH_HT** ht = NULL;
	Curve_LSH** grids = NULL;
	
//	void** Hash_tables = NULL;

	// Initialize the structures if we want range Search
	if( assignment == "2" ){	// Range Search with LSH ==> Initialize HashTables
	
		if( curve == false){
		
			cout << "Initializing LSH_Vector \n";
			
			ht = new LSH_HT*[num_HT];
			Initialize_LSH_Vector(original_items,k_vec,num_HT,ht);	//Assignment.hpp
		
//			Hash_tables = (void**) ht;
		
			cout << "Finished Initializing LSH_Vector \n";		
		}else{
		
			cout << "Initializing Curve_LSH \n";
			
			grids = new Curve_LSH*[num_grids];
			Initialize_LSH_Curve(original_items,k_vec,num_grids,grids);	//Assignment.hpp
		
//			Hash_tables = (void**) grids;
		
			cout << "Finished Initializing Curve_LSH \n";		
		}
	}
	
	
	/************************** Start timer **********************/
	
	double start = Timestamp();	//Utilities.cpp
	

	/*************************** Starting Initialization ********************************/
	if( initialization == "1"){
	
		cout << "Started Initialization (1) \n";
	
		Random_Initialize(k,items,clusters);
		
		
//		cout << endl << original_items->size() << " and " << items->size() << endl;
		
		assert( (items->size() == original_items->size() - k) && ( clusters->size() == k) );
	
		cout << "Finished Initialization (1) ==> main\n";
	
	}else{	//initialization == "2"
	
		cout << "Started Initialization++ \n";
	
		Initialize(k,items,clusters);	//Initialization++
		assert( (items->size() == original_items->size() - k) && ( clusters->size() == k) );
		
		cout << "Finished Initialization++ \n";
	}
	
	// items , now contains non-centroids
	
	
	assert(items->size() == original_size );

	/****************** 1st Assignment ***************************/

	while(true){

		if( assignment == "1"){
	
			cout << "Started 1st Assignment (1) \n";
	
			Simple_Assignment(items,clusters);
	
			cout << "Finished 1st Assignment (1) \n";
	
		}else{
	
			cout << "Started 1st Assignment (2) \n";
	
			void** Hash_tables = NULL; 
	
			if( curve == false){

				Hash_tables = (void**) ht;
			
				Reverse_Assignment(items,clusters, Hash_tables,num_HT);		
		
			}else{
		
				Hash_tables = (void**) grids;
			
				Reverse_Assignment(items,clusters, Hash_tables,num_grids);		
			}
			cout << "Finished 1st Assignment (2) \n";
		}

		bool check = true;
		check = Check_Assignment(items, clusters);
		
		if( check == false) break; // no problems encountered
		// Every cluster has at least 2 items
		
		// Break current assignments , because new centroids have appeared
		Break_Assignments(clusters);	//Utilities3.hpp
		
		cout << "Trying again (1) \n";
		
		//Else try again
	}
	

	cout << "Out of the 1st loop \n";
	
	/* A lot of Testing ==> Asserts */
	for( unsigned int i = 0; i < clusters->size(); i++){
	
		assert( clusters->at(i)->getSize() >= 2  );
	}
	
	assert(items->size() == original_size );
	
	
	for( unsigned int i = 0; i < items->size();i++){
		
		assert(items->at(i)->getCluster() != NULL);

		assert(items->at(i)->get_2nd_Cluster() != NULL);
	}
	
	for( unsigned int i = 0; i < clusters->size();i++){
	
//		cout << "i is " << i << endl;
		
		assert( clusters->at(i)->getC()->get_2nd_Cluster() != NULL);
		
		assert(clusters->at(i)->getC()->getCluster() != NULL);
		// the above is == NULL ===> ERROR

//		clusters->at(i)->getC()->ChangeCluster( clusters->at(i), 0);
	}
	
	/* End of Testing */
	
	

	/* Starting Update->Assign loop */

	int max_iterations = 7;
	
	for( int i = 0; i < max_iterations; i++){
	
	
		/************************ Updating **************************/
		
		bool change = false;
		
		if( update == "1"){	// k-medoid ==> PAM ala Lloyd's
		
			cout << "Starting Update(1) \n";
		
			change = Update_Lloyd(items,clusters);
			assert( (items->size() == original_size) && ( clusters->size() == k) );
			
			cout << "Finished Update(1) \n";
		
		
		}else{	// k-means
		
			if( curve == false){
			
//				change = Update_Vector(items,clusters);
				//maybe check the distance of old Centroid and new Centroid?

				cout << "Starting Update(2) \n";	

				Update_Vector(items,clusters);
				change = true;
				
				cout << "Finished Update(2) \n";
				
				assert(items->size() == original_items->size() );
			
			}else{
			
				cout << "Starting Update(2) \n";				
			
				Update_Curve(items,clusters);
				change = true;
				
				cout << "Finished Update(2) \n";
				
				assert(items->size() == original_items->size() );
			}
		}
		
	/* Testing
	for( unsigned int i = 0; i < items->size();i++){
		
		assert(items->at(i)->getCluster() == NULL);

		assert(items->at(i)->get_2nd_Cluster() == NULL);
	}
	
	for( unsigned int i = 0; i < clusters->size();i++){
	
		cout << "i is " << i << endl;
		
		assert( clusters->at(i)->getC()->get_2nd_Cluster() == NULL);
		
		assert(clusters->at(i)->getC()->getCluster() != NULL);
		// the above is == NULL ===> ERROR
		

		
//		clusters->at(i)->getC()->ChangeCluster( clusters->at(i), 0);
	}
	
	// end of Testing
	*/
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		if( change == false){
			cout << "No significant changes in Update ==> breaking out of loop\n";
			break;	//exit the loop	
		}
	
	
	
	
		//Revert the flag "assigned" to false ==> flag only used/checked in Range_Search
//		if( assignment == "2" ) Break_Assignments(clusters);	//Utilities3.hpp

//		assert(items->size() == original_size );

		Break_Assignments(clusters);	//Utilities3.hpp

//		assert(items->size() == original_size );






		/********************* 2nd Assignment **************************/
		
		while(true){

			if( assignment == "1"){
	
				cout << "Started 2nd Assignment (1) \n";
	
				Simple_Assignment(items,clusters);
	
				cout << "Finished 2nd Assignment (1) \n";
	
			}else{
	
				cout << "Started 2nd Assignment (2) \n";
	
				void** Hash_tables = NULL; 
	
				if( curve == false){

					Hash_tables = (void**) ht;
			
					Reverse_Assignment(items,clusters, Hash_tables,num_HT);		
		
				}else{
		
					Hash_tables = (void**) grids;
			
					Reverse_Assignment(items,clusters, Hash_tables,num_grids);		
				}
				cout << "Finished 2nd Assignment (2) \n";
			}
			
//			assert(items->size() == original_size );

			bool check = true;
			check = Check_Assignment(items, clusters);
			
//			assert(items->size() == original_size );
			
		
			if( check == false) break; // no problems encountered
			// Every cluster has at least 2 items
		
			// Break current assignments , because new centroids have appeared
			Break_Assignments(clusters);	//Utilities3.hpp
			
//			assert(items->size() == original_size );
		
			cout << "Trying again (2) \n";
		
			//Else try again
		}
		
		cout << "Out of the 2nd loop \n";
		
//		assert(items->size() == original_size );
	}
	
	
	
	
	double end = Timestamp();
	
	double clustering_time = ( end - start) / 1000;	// milliseconds ==> seconds

	cout << "Finished Updating, check Silhouette \n";
	
	
	/* Testing gain == > Assert */
	for( unsigned int i = 0; i < items->size();i++){
		
		assert(items->at(i)->getCluster() != NULL);

		assert(items->at(i)->get_2nd_Cluster() != NULL);
	}
	
	for( unsigned int i = 0; i < clusters->size();i++){
	
		if( clusters->at(i)->getInput() == false) continue;
	
		cout << "i is " << i << endl;
		
		assert( clusters->at(i)->getC()->get_2nd_Cluster() != NULL);
		
		assert(clusters->at(i)->getC()->getCluster() != NULL);
		// the above is == NULL ===> ERROR
		

		
//		clusters->at(i)->getC()->ChangeCluster( clusters->at(i), 0);
	}
	
	/* end of Testing */
	
	
	/************** One last check ***********************/
	Compute_2nd_Best(items,clusters);	// Sanitation check
	
	for( unsigned int i = 0; i < items->size();i++){

		assert(items->at(i)->get_2nd_Cluster() != NULL);
	}
	
	
	/************************ Silhouette ******************************/
	
	vector<double>* s = new vector<double>;

	Silhouette( clusters, s, items->size() );

	cout << "Finished Silhouette, print Statistics \n";

	/************************ Print Statistics ***********************/

	Print_output(output_file,combination,clusters, clustering_time, s, complete);
	
	
	/* Cleaning up */
	
	for( unsigned int i = 0; i < clusters->size(); i++) delete clusters->at(i);
	delete clusters;
	clusters = NULL;
	
	for( unsigned int i = 0; i < original_items->size(); i++) delete original_items->at(i);
	delete original_items;
	original_items = NULL;
	
	delete items;	// this was originally a copy  ==> contents already deleted
	items = NULL;
	
	delete s;
	s = NULL;

	return 0;
}





bool Read_Input(string filename, vector<Item*>* result){

	// return true or false, if we are dealing with Curves or Vectors

	ifstream myfile;
	myfile.open(filename.c_str());
	
	string line;
	
	bool result1 = false;
	
	getline( myfile, line);	//read 1st line of input file ==> vector or curve?
	
//	cout << "Length of line is " << line.length() << endl;
//	cout << "Line is " << line << endl;
	
	
	if( line[0] == 'v'){
	
		Input_Vector(&myfile,result);
		result1 = false;
	
	
	}else if( line[0] == 'c'){
	
		Input_Curve(&myfile,result);	// check for duplicate points in curve
//		Input_Curve1(&myfile,result);	// don't check for duplicate coordinates
		
		result1 = true;
	
	}else{
	
		cout << "Error while reading input\n";	
	
	}

	myfile.close();

	return result1;
}




void Input_Vector(ifstream* myfile, vector<Item*>* items){
	
	
	/* Handling input_file */
		
	string line;
	string token;	
	
	while(getline(*myfile,line)){	//read file line by line
	
		stringstream line1(line);

		vector<double>* coordinates = new vector<double>;
		string id;
		
		
		/* Get ID */
		
		getline(line1,token,'\t');
		id = token;

		while( getline(line1,token,'\t') ){	// read each coordinate
		
			double c1 = stod(token,NULL);
			coordinates->push_back(c1);	
			
			// next coordinate of vector
		}
		// finished with all the coordinates of this Vector ==> creating object
		
		Vector* v = new Vector(id,coordinates);
		Item* item = new Item(v);

		items->push_back(item);
		
		v = NULL;
		item = NULL;
		coordinates = NULL;
	}
}







//OK
void Input_Curve(ifstream* myfile, vector<Item*>* items){

	// Also, check for duplicate points in a curve
	// Don't add them

	/* Handling input_file */
		
	string line;
	string token;	
	
	while(getline(*myfile,line)){	//read file line by line (Curve)
	
		stringstream line1(line);

		vector< vector<double>* >* points = new vector< vector<double>* >;
		string id;
		
		/* Get ID */
		
		getline(line1,token,'\t');
		id = token;
		
		/* Get number of points */
		getline(line1,token,'\t');
		
		vector<double>* previous = NULL;

		while( getline(line1,token,'(') ){	// encountered a point
		
			// We have encountered a new point 			
			vector<double>* coord = new vector<double>;
								
			getline(line1,token,')');// now we have the token as the coordinates of single point
						
			stringstream coordinates(token);
			string token1;
			while( getline(coordinates,token1,' ') ){	// Coordinates
			
				int t = token1.length();
				
				if( (t == 1) && (token1[0] = ' ') ){
			
					continue;
				}
				
				if( token1[t-1] == ',') token1.pop_back();	// if the last char is ',', remove it
				
				
				
//				cout << "Token1 is " << token1 << endl;
//				cout << "Token1 length is " << token1.length() << endl;
				
				
				double c1 = stod(token1,NULL);
				coord->push_back(c1);
			}//ok , get next coordinate
			
			//finished with coordinates/vector inserting into curve_vector


			if( coord->size() == 0){
			
				// Probably ,a trailing space
				previous = NULL;

				delete coord;
				coord = NULL;

				continue;
			}


			if( (previous != NULL) && ( Same_vectors(previous,coord) == true) ){
			
				// this new point is the same as previous one
				// Don't add to the curve
			
				delete coord;
				coord = NULL;
				continue;	
			}
			//else all ok
			
			assert( coord->size() > 0);

			previous = coord;	// update previous
			
			points->push_back(coord);
			coord = NULL;					
		}	// next vector of curve
		
		// finished with all the vectors of curve ==> creating object
		
		assert(points->size() > 0);
		
		Curve* cv = new Curve(id,points);
		Item* item = new Item(cv);
		
		items->push_back(item);
		
		item = NULL;
		cv = NULL;
		points = NULL;
		
		previous = NULL;
	}

}







void Input_Curve1(ifstream* myfile, vector<Item*>* items){

	// Doesn't check for duplicate coordinates

	/* Handling input_file */
		
	string line;
	string token;	
	
	while(getline(*myfile,line)){	//read file line by line (Curve)
	
		stringstream line1(line);

		vector< vector<double>* >* points = new vector< vector<double>* >;
		string id;
		
		/* Get ID */
		
		getline(line1,token,'\t');
		id = token;
		
		/* Get number of points */
		getline(line1,token,'\t');
		
//		vector<double>* previous = NULL;

		while( getline(line1,token,'(') ){	// encountered a point
		
			// We have encountered a new point 			
			vector<double>* coord = new vector<double>;
								
			getline(line1,token,')');// now we have the token as the coordinates of single point
						
			stringstream coordinates(token);
			string token1;
			while( getline(coordinates,token1,' ') ){	// Coordinates
			
				int t = token1.length();
				
				if( (t == 1) && (token1[0] = ' ') ){
				
					continue;
				}
				
				if( token1[t-1] == ',') token1.pop_back();	// if the last char is ',', remove it
				
				
				
//				cout << "Token1 is " << token1 << endl;
//				cout << "Token1 length is " << token1.length() << endl;
				
				
				double c1 = stod(token1,NULL);
				coord->push_back(c1);
			}//ok , get next coordinate
			
			//finished with coordinates/vector inserting into curve_vector


			if( coord->size() == 0){
			
				// Probably ,a trailing space
//				previous = NULL;

				delete coord;
				coord = NULL;

				continue;
			}

			assert( coord->size() > 0);
			
			points->push_back(coord);
			coord = NULL;					
		}	// next vector of curve
		
		// finished with all the vectors of curve ==> creating object
		
		assert(points->size() > 0);
		
		Curve* cv = new Curve(id,points);
		Item* item = new Item(cv);
		
		items->push_back(item);
		
		item = NULL;
		cv = NULL;
		points = NULL;
	}

}












void Read_Config(string filename, int* k, int* num_grids, int *vec_L, int *vec_k){

	/* Default values */
	*num_grids = 2;	// num_grids/hash_tables ==> Curve
	*vec_L = 3;	// number of LSH hash_Tables ==> Vector


	ifstream myfile;
	myfile.open(filename.c_str());
	
	string line;
	
	while(getline(myfile,line)){	//read file line by line 
	
		stringstream line1(line);
	
		string token;
		getline(line1,token,' ');	// get string
		
		string token1;
		getline(line1,token1,' ');	//get value
		
		if(token == "number_of_clusters:"){
		
			*k = stoi(token1,NULL);
			assert( *k > 0);
			
//			check = true;
		
		}else if(token == "number_of_grids"){
		
			*num_grids = stoi(token1,NULL);
			assert( *num_grids > 0);
		
		}else if(token == "number_of_vector_hash_tables:"){
		
			*vec_L = stoi(token1,NULL);
			assert( *vec_L > 0);
		
		
		}else if(token == "number_of_vector_hash_functions:"){
		
			*vec_k = stoi(token1,NULL);
			assert( *vec_k > 0);
			
//			check1 = true;
		}
	
	}
	
	
	myfile.close();
}



void Print_output( string filename, string combination, vector<Cluster*>* clusters, double clustering_time, vector<double>* s, bool complete){

	bool curve = clusters->at(0)->getC()->isCurve();

	ofstream output;
	output.open(filename.c_str(), ios::out | ios::trunc );
	
	output<< "Algorithm: " << combination[0] <<"x" <<combination[1]<<"x"<< combination[2] << endl;
	
	for( unsigned int i = 0; i < clusters->size(); i++){

		output << "CLUSTER-" << i+1 <<  " {size:" << clusters->at(i)->getSize() << ", centroid: ";
		
		if( clusters->at(i)->getInput() == false){	//kmeans
		
			if( curve == true){
			
				Print_curve1( clusters->at(i)->getC()->getCurve(), &output);
			
			}else{
			
				Print_vector1( clusters->at(i)->getC()->getVector(), &output);
			
			}
			// printed the centroid (kmeans)
		
		}else{	// print item_id
		
			if( curve == true){
			
				output << clusters->at(i)->getC()->getCurve()->getName();
			
			}else{
			
				output <<  clusters->at(i)->getC()->getVector()->getName();		
			
			}
		}
		// finished printing the centroid
		
		output << " } " << endl;
	}
	// finished printing all clusters
	
	output << endl;
	
	// Print clustering time
	output << "clustering_time: " << clustering_time << endl;
	
	//Print silhouette
	output << "Silhouette: [ ";
	for( unsigned int i = 0; i < s->size(); i++){
		
		output << s->at(i);
			
		if( i != s->size() -1) output << ", ";
	}
	output << " ] \n";
	
	
	
	
	if( complete == true){	//Print items inside each cluster
	
		output << endl << endl;
		
		for( unsigned int i = 0; i < clusters->size(); i++){

			output << "CLUSTER-" << i+1 << " { ";
	
			vector<Item*>* items = new vector<Item*>;
			clusters->at(i)->getAllItems(items);
			
			for( unsigned int j = 0; j < items->size(); j++){
			
				if( curve == true){
			
					output << items->at(j)->getCurve()->getName();
				}else{
			
					output <<  items->at(j)->getVector()->getName();
				}
			
				if( i != items->size() -1) output << ", ";
			}
			
			output << " } \n";
			
			delete items;
			items = NULL;
		}
	}
		
	output.close();
}









void Print_curve1(Curve* curve, ofstream* output){

	vector< vector<double>* >* points = curve->getP();
	
	*output << " { ";
	
	for( unsigned int i = 0; i < points->size(); i++){
	
		*output << "[ ";
	
		for( unsigned int j = 0; j < points->at(i)->size(); j++){
		
			*output << points->at(i)->at(j);
			
			if( j != points->size() -1) *output << ", ";
			
		}
	
		*output << " ] ";
	
	}
	
	*output << " } ";
	
	points = NULL;
}


void Print_vector1(Vector* v, ofstream* output){

	vector<double>* coordinates = v->getC();

	*output << " [ ";

	for( unsigned int i = 0; i < coordinates->size(); i++){
		
		*output << coordinates->at(i);
			
		if( i != coordinates->size() -1) *output << ", ";
			
	}

	*output << " ] ";

	coordinates = NULL;
}

