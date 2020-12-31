#ifndef UTILITIES3_H
#define UTILITIES3_H

#include <iostream>
#include <vector>

#include <utility>

#include "Item.hpp"
#include "Cluster.hpp"

class Item;		// forward declarations again
class Cluster;

using namespace std;

void Remove_Duplicates(vector<Item*>* items);


double Item_Distance(Item* x, Item* y);

int BinarySearch(vector<double>* P, double x, int start, int end);

void UpdateItems(vector<Item*>* items);

void Break_Assignments(vector<Cluster*>* clusters);	//after each update or if we find outliers


void Compute_2nd_Best(vector<Item*>* items, vector<Cluster*>* clusters);


//void Generate_Random_Set(int lamda, int* array);
void Generate_Random_Subsequence(int lamda, int size, int* array);


void DTW1(vector<vector<double>*>* p,vector<vector<double>*>* q, vector< pair<int,int> >* result1);

void DTW_Backtrack( double** array, int i , int j, vector< pair<int,int> >* v);


#endif
