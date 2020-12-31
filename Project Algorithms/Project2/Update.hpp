#ifndef UPDATE_H
#define UPDATE_H

#include <iostream>
#include <vector>

#include <utility>

#include "Item.hpp"
#include "Cluster.hpp"



bool Update_Lloyd(vector<Item*>* items, vector<Cluster*>* clusters);
//Update1

bool Update_Vector(vector<Item*>* items, vector<Cluster*>* clusters);
//Update2 ==> Mean Vector

bool Update_Curve(vector<Item*>* items, vector<Cluster*>* clusters);
//Update2 ==> Mean Curve

void Mean_Point(vector< vector<double>* >* points, vector<double>* result);


#endif
