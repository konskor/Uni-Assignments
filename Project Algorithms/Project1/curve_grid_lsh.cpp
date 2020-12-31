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


using namespace std;


void Insert2Vector(vector<Curve*>* v, string filename);
void Max_fraction(vector<InfoCurve*> *info);
void AverageLSH_time(vector<double> *times, int L);


int main(int argc,char** argv){

	if( argc < 7){
	
		cout << "Not enough arguments need 7\n";
		return -1;
	}
	
	
	string input_file,query_file,output_file;
	
	int L = 4;		// how many grids for LSH_curve
	int k = 4;		// how many h(x) for LSH_vec
		
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

		if( strcmp(argv[i] , "-k_vec") == 0 ){		// k for LSH_vec (optional)
		
			if( i == argc-1){
				cout << " Something is wrong: -k_vec ..... \n";
				return -1;		
			}
			
			k = atoi( argv[i+1] );
			
			continue;	
		}


		if( strcmp(argv[i] , "-L_grid") == 0 ){		// how many grids for LSH_curve (optional)
		
			if( i == argc-1){
				cout << " Something is wrong: -L_grid ..... \n";
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
	vector<Curve*> *curves = new vector<Curve*>;	// create empty vectors
	vector<Curve*> *queries = new vector<Curve*>;
	vector<InfoCurve*> *info = new vector<InfoCurve*>;
	
	
	Insert2Vector(curves,input_file);
	cout << "Finished inserting curves into vector\n";
	
	Insert2Vector(queries,query_file);
	cout << "Finished inserting queries into vector\n"; 
	
	cout << "Curves_size is " << curves->size() << endl;
	cout << "Queries_size is " << queries->size() << endl;	
	
//	cout<< "Query 101 has " << queries->at(100)->getP()->size() << " points" << endl;
	
	
////////////////////////////////////////////////////////////////////

	cout << "Starting Curve_NN_True" << endl;
	
	int d = curves->at(0)->getD();
	
	cout << "D is " << d << endl;
	
	
	/* This is the brute force method , comment if dataset is big */	
	
	int m = Curve_NN_True(curves,queries,info);
	
	assert(queries->size() == info->size() );	
	
	double delta = (4 * d * m)/10;
	
//	delta = 0.3;
	
	cout << "Delta is "<< delta <<" and m is " << m << endl;
	
///////////////////////////////////////////////////////
/////////////////////////////////////////////////////
	
	
	/* Starting LSH_Curve */
	
	cout << " Creating necessary objects for LSH_Curve \n";
	

	/* Creating the Grids */

	Curve_LSH** grids = new Curve_LSH*[L];
	for( int i = 0; i < L; i++){
	
		random_device rd;
		mt19937 generator(rd());	//Standard mersenne_twister_engine seeded with rd()
		
		std::uniform_real_distribution<double> dis(0.0, d);
		grids[i] = new Curve_LSH(d,delta,generator,dis);				
	}
	cout << "Built our new empty LSH_Curves/grids\n";	
	
		
	/* Creating our Grid points and padding them ( if needed) */
	for(int i = 0; i < L; i++){
	
		grids[i]->Curve2Gcurve(curves);	
		// does everything (Curve->GCurve->Vector->Vector_Node and the padding)	
	}

	cout << "Created and padded all our curves\n";
	
	for(int i = 0; i < L; i++){
	
		random_device rd;
		mt19937 generator(rd());	//Standard mersenne_twister_engine seeded with rd()
		
		vector<Vector_Node*>* vn = grids[i]->getGpoints();
		
		double w = Compute_w_curve(vn);		

//		double w = 4100.31;
	
//		double w = 10;
	
//		if( w == 0) w = 10;
		
		cout<<"W is " << w << endl;	
		
		std::uniform_real_distribution<double> dis(0.0, w);
	
		grids[i]->setLSH_HT(k,w,generator,dis);	// Creating the hasH Table	
		vn = NULL;
	}
	 
	cout << "Created all the hash tables \n";
	
	cout <<"Started Inserting grid_points to HashTable\n";
	
	for(int i = 0; i < L; i++) grids[i]->Insert();
	
	cout << "Finished Inserting\n";
	
	cout << "Now for the queries \n";
	
	vector<double>* times = new vector<double>;
	
	for( unsigned int i = 0; i < queries->size(); i++){	// for all Queries
		
//		double t1 = Timestamp();
		double sum = 0;		
		
		for( int j = 0; j < L; j++){	//for all Grids/HashTables
		
			Curve* g = NULL;
			grids[j]->Curve2Gcurve1(queries->at(i), &g);	// Creating the Grid curve
			
			Vector* v = NULL;
			grids[j]->Gcurve2Vector1(g,&v);	 //Creating	the vector (Incomplete)
			
			grids[j]->Update1(v);	// update the query

			Vector_Node* v1 = new Vector_Node(v,queries->at(i),g);		

			double t1 = Timestamp();	

			grids[j]->getHT()->Search_NN1(v1,info->at(i),1);	//Search NN , L_vec=1

			double t2 = Timestamp();
			
			double duration = t2 - t1;
			sum = sum + duration;

			delete v1;
			delete v;
			delete g;
			v1 = NULL;
			v = NULL;
			g = NULL;
		}
		
		times->push_back(sum);	
		
		
//		double t2 = Timestamp();
//		double duration = t2 - t1;
//		times->push_back(duration);					
	}

	cout << "Finished  Searching for Nearest Neighbors (LSH)\n";

	Print_Curve(info,output_file);
	
	cout << "Finished writing the statistics to output ==> Cleaning up\n";	
	
//////////////////////////////////////////////////////////////////

	Max_fraction(info);
	AverageLSH_time(times,L);
	

/////////////////////////////////////////////////////////////////
	
	/* Cleaning up */
	
	unsigned int i = 0;
	for( i = 0; i < curves->size(); i++){
		delete curves->at(i); 
		curves->at(i) = NULL;		// deleting all the Vectors (dimension d)
	}
	delete curves;	// finally delete vector
	curves = NULL;
	
///////////////////////////////////////////////////////////////////////////////////
	
	for( i = 0; i < queries->size(); i++){
		delete queries->at(i); 
		queries->at(i) = NULL;		// deleting all the Vectors (dimension d)
	}
	delete queries;	// finally delete vector
	queries = NULL;
	
////////////////////////////////////////////////////////////////////////////////	
	
	for( i = 0; i < info->size(); i++){
		delete info->at(i); 
		info->at(i) = NULL;		// deleting all the Vectors (dimension d)
	}
	delete info;	// finally delete vector
	info = NULL;
	
////////////////////////////////////////////////////////////////////////////////

	for(int i = 0; i < L; i++)	delete grids[i];
	delete[] grids;
	grids = NULL;

	delete times;


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
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
