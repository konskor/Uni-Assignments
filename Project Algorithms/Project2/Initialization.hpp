#ifndef INITIALIZATION_H
#define INITIALIZATION_H

#include <vector>

#include "Item.hpp"
#include "Cluster.hpp"

/* ALL OK */

void Random_Initialize(int k, vector<Item*>* items, vector<Cluster*>* clusters);
// Initialization (1)

void Initialize(int k, vector<Item*>* items, vector<Cluster*>* clusters);
// Initialization++

//Moved to Utilities_3
//int BinarySearch(vector<double>* P, double x, int start, int end);


#endif
