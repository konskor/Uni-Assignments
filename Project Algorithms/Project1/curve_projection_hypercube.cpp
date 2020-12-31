#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include <string.h>
#include <stdlib.h>
#include <ctime>
#include <cmath>
#include <cassert>

#include <random>
#include <chrono>

#include <vector>

#include "Curve.hpp"
#include "Utilities.hpp"
#include "InfoCurve.hpp"
#include "LSH_HT.hpp"

#include "Utilities2.hpp"
#include "Curve_LSH.hpp"

#include "Relevant_Table.hpp"




using namespace std;


void Insert2Vector(vector<Curve*>* v, string filename);
void Max_fraction(vector<InfoCurve*> *info);
void AverageLSH_time(vector<double> *times, int L);
void CutInput( vector<Curve*>* curves, vector<Curve*>* cut );


int main(int argc,char** argv){

	if( argc < 7){
	
		cout << "Not enough arguments need 7\n";
		return -1;
	}
	
	
	string input_file,query_file,output_file;
	
	int M1 = 10;		// max points to check in Hypercube
//	int k = 4;		// how many h(x) for HyperCube
	int bh_d = 4;	// d' of hypecube
	int probes = 2;
		
	double e = 0.5; 	
		
	
		
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

		if( strcmp(argv[i] , "-k_hypercube") == 0 ){		// k for Hypercube (optional)
		
			if( i == argc-1){
				cout << " Something is wrong: -k_hypercube..... \n";
				return -1;		
			}
			
			bh_d = atoi( argv[i+1] );
			
			continue;	
		}


		
		if( strcmp(argv[i] , "-M") == 0 ){// how many points to check in HyperCube (optional)
		
			if( i == argc-1){
				cout << " Something is wrong: -M ..... \n";
				return -1;		
			}
			
			M1 = atoi( argv[i+1] );
			continue;
		}
		
		
		if( strcmp(argv[i] , "-probes") == 0 ){
		// how many buckets to check ib HyperCube (optional)
		
			if( i == argc-1){
				cout << " Something is wrong: -probes ..... \n";
				return -1;		
			}
			
			probes = atoi( argv[i+1] );
			continue;
		}
		
		
		
		
		if( strcmp(argv[i] , "-e") == 0 ){//  (optional)
		
			if( i == argc-1){
				cout << " Something is wrong: -e ..... \n";
				return -1;		
			}
			
			e = strtod( argv[i+1], NULL );
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
	vector<Curve*> *curves = new vector<Curve*>;	// create empty vectors
	vector<Curve*> *queries = new vector<Curve*>;
	vector<InfoCurve*> *info = new vector<InfoCurve*>;
	
	
	Insert2Vector(curves,input_file);
	cout << "Finished inserting curves into vector\n";
	
	Insert2Vector(queries,query_file);
	cout << "Finished inserting queries into vector\n"; 
	
	
	// Cut the input ( curves with max length M =10)
	vector<Curve*> *curves_cut = new vector<Curve*>;
	CutInput(curves,curves_cut);
	
	// Cut the input ( curves with max length M =10)
	vector<Curve*> *queries_cut = new vector<Curve*>;
	CutInput(queries,queries_cut);
	
	cout << "Curves_size is " << curves->size() << endl;
	cout << "Queries_size is " << queries->size() << endl;	

	
//	cout<< "Query 101 has " << queries->at(100)->getP()->size() << " points" << endl;
	
	
////////////////////////////////////////////////////////////////////

	cout << "Starting Curve_NN_True1" << endl;
	
	int d = curves->at(0)->getD();
	
	cout << "D is " << d << endl;
	
	
	/* This is the brute force method , comment if dataset is big */	
	
	int M = Curve_NN_True1(curves_cut,queries_cut,info);
//	int M = 100;
	assert(queries_cut->size() == info->size() );	
	
///////////////////////////////////////////////////////
/////////////////////////////////////////////////////
	
	
	/* Starting Projection */
	
	cout << " Creating necessary objects for Projection Curbe \n";
	

	/* Creating the Table */
	int window = 2;
	
	Relevant_Table* table = new Relevant_Table(M,d,window,e,false);
	
	cout << "Finished Creating the empty Relevant_Table\n";
	
	cout << "Started Inserting\n";
	
	table->Insert1(curves_cut,bh_d, M1, probes); 
	
	cout << "Finished Inserting\n";
	
	cout << "Now for the queries\n";
	
	vector<double>* times = new vector<double>;
	
	for( unsigned int i = 0; i < queries_cut->size(); i++){
	
		double t1 = Timestamp();
		table->Search(queries_cut->at(i),info->at(i));	
		
		double t2 = Timestamp();
		double duration = t2-t1;
		
		times->push_back(duration);	
	}
	
//	cout << "Finished with the queries, writing the Statistics \n";

	cout << "Finished  Searching for Nearest Neighbors (LSH)\n";
	
	Print_Curve(info,output_file);
	
	cout << "Finished writing the statistics to output ==> Cleaning up\n";	

//////////////////////////////////////////////////////////////////

	Max_fraction(info);
	AverageLSH_time(times,1);
	

/////////////////////////////////////////////////////////////////
	
	/* Cleaning up */
	
	unsigned int i = 0;
	for( i = 0; i < curves->size(); i++){
		delete curves->at(i); 
		curves->at(i) = NULL;		// deleting all the Vectors (dimension d)
	}
	delete curves;	// finally delete vector
	curves = NULL;
	
	delete curves_cut;
	
///////////////////////////////////////////////////////////////////////////////////
	
	for( i = 0; i < queries->size(); i++){
		delete queries->at(i); 
		queries->at(i) = NULL;		// deleting all the Vectors (dimension d)
	}
	delete queries;	// finally delete vector
	queries = NULL;
	
	delete queries_cut;
	
////////////////////////////////////////////////////////////////////////////////	
	
	for( i = 0; i < info->size(); i++){
		delete info->at(i); 
		info->at(i) = NULL;		// deleting all the Vectors (dimension d)
	}
	delete info;	// finally delete vector
	info = NULL;

////////////////////////////////////////////////////////////////////////////////

	delete table;
	table = NULL;
	
///////////////////////////////////////////////////////////////////////////////

	delete times;
	times = NULL;

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

}


	
	cout << "Finished cleaning up, exiting \n";
	
//	cout << "W is " << w << endl;
	
	return 0;
}
	
	
	
	
	
	

void Insert2Vector( vector<Curve*>* v, string filename){

	ifstream myfile;
	myfile.open(filename.c_str());

	
	/* Handling input_file */
		
	string line;
	string token;	
	
	while(getline(myfile,line)){	//read file line by line (Curve)
	
		stringstream line1(line);

		vector< vector<double>* >* points = new vector< vector<double>* >;
		string id;
		
		/* Get ID */
		
		getline(line1,token,'\t');
		id = token;
		
		/* Get number of points */
		getline(line1,token,'\t');

		
		
		while( getline(line1,token,'(') ){	// Vector
		
			// We have encountered a new point 			
			vector<double>* coord = new vector<double>;
								
			getline(line1,token,')');// now we have the token as the coordinates of single point
						
			stringstream coordinates(token);
			string token1;
			while( getline(coordinates,token1,' ') ){	// Coordinates
			
				int t = token1.length();
				
				if( token1[t-1] == ',') token1.pop_back();	// if the last char is ',', remove it
				
				double c1 = stod(token1,NULL);
				
				coord->push_back(c1);
			}//ok , get next coordinate
			
			//finished with coordinates/vector inserting into curve_vector

			points->push_back(coord);
			coord = NULL;					
		}	// next vector of curve
		
		// finished with all the vectors of curve ==> creating object
		
		Curve* cv = new Curve(id,points);
		v->push_back(cv);
		
		cv = NULL;
		points = NULL;
	}
	
	myfile.close();
	
//	cout << "Finished reading input file" << endl;

}	
	
	
	
	
	
void Max_fraction(vector<InfoCurve*> *info){

	unsigned int i = 0;
	
	double lsh_d = 0.0;
	double true_d = 0.0;
	double f = 0.0;
	
	
	double max = -1.0;
	double sum = 0.0;

	
	for( i = 0; i < info->size(); i++){
	
		lsh_d = info->at(i)->getLSH_d();
		true_d = info->at(i)->getTrue_d();
		
		f = lsh_d / true_d;
		
		sum = sum + f;

		if( f > max) max = f;	
	}

	cout << "Max{lsh_d/true_d} is " << max << endl;
		
	sum = sum/ (info->size());
		
	cout<<" Average of lsh_d/true is " << sum << endl;	
}	
	
	
	
	
	
void AverageLSH_time(vector<double> *times,int L){

	unsigned int i = 0;

	double temp = 0.0;
	double sum = 0.0;
	
	double total = 0.0;
	
	for( i = 0; i < times->size(); i++){
	
		temp = times->at(i);
		sum = sum + temp;	
	}
	
	total = sum/(times->size());
	
//	total = sum/ ( L * times->size() );
	
	cout << "Average searching time is " << total/1000 << endl;
}	
	
	
	
	
	
void CutInput( vector<Curve*>* curves, vector<Curve*>* cut ){
	
	int maxM = 10;
	
	for( unsigned int i = 0; i < curves->size(); i++){
		
		if( curves->at(i)->getP()->size() <= maxM) cut->push_back(curves->at(i) );
		
		
	}
	
	
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
