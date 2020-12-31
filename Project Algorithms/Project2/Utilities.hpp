#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <chrono>
#include <random>

#include <vector>

#include "Vector.hpp"
//#include "InfoItem.hpp"


using namespace std;


double Compute_w(vector<Vector*>* points);	// we use this one
//double Compute_w2(vector<Vector*>* points);


double Timestamp();

//void Manhattan_LSH(float w, float** s, std::default_random_engine generator);
// pass by reference

int Mod(int x, int y);

int Max(int *a,int size);

unsigned int alt_pow(int m, unsigned int d, unsigned int M);

double Vector_distance(Vector* v1, Vector* v2, int k);

//double Compute_w(vector<Vector*>* points);	// Compute w for LSH
//double Compute_w1(vector<Vector*>* points, vector<Vector*>* queries);
//double Compute_w2(vector<Vector*>* points);	// IGNORE


//double NN_True( vector<Vector*> *points, vector<Vector*> *queries,  vector<InfoItem*> *info);
// return average true NN distance

//void Print_vec(vector<InfoItem*> *info, string filename);		// write to output file

#endif
