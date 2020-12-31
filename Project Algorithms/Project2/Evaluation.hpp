#ifndef EVALUATION_H
#define EVALUATION_H

#include <iostream>
#include <vector>

#include <utility>

#include "Item.hpp"
#include "Cluster.hpp"

void Silhouette(vector<Cluster*>* clusters, vector<double>* s,  unsigned int size);

double Compute_Average_Distance(Item* item,vector<Item*>* items1, bool same, int index);

#endif
