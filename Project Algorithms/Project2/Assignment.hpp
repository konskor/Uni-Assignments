#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include <iostream>
#include <random>
#include <chrono>
#include <cmath>

#include <vector>

#include <string>

#include <cassert>

#include "Vector.hpp"
#include "Curve.hpp"


#include "Utilities.hpp"
#include "Utilities2.hpp"

#include "Utilities3.hpp"

#include "LSH_HT.hpp"
#include "Curve_LSH.hpp"

#include "Item.hpp"
#include "Cluster.hpp"



void Simple_Assignment(vector<Item*>* items, vector<Cluster*>* clusters);
// 1st Assignment


void Reverse_Assignment(vector<Item*>* items, vector<Cluster*>* clusters,void** hash_tables,int L);
// 2nd Assignment


void Commit_Assignmens_Items(vector<Item*>* items, vector<Cluster*>* clusters);



void Initialize_Query_Vector(vector<Cluster*>* clusters, vector< vector<string>* >* gx, LSH_HT** ht, int L);


void Initialize_Query_Curve(vector<Cluster*>* clusters, vector< vector<string>* >* gx, Curve_LSH** grids, int L);


void Initialize_LSH_Vector(vector<Item*>* items, int k, int L, LSH_HT** ht);

void Initialize_LSH_Curve(vector<Item*>* items, int k, int L, Curve_LSH** grids);

bool Check_Assignment(vector<Item*>* items,vector<Cluster*>* clusters);

void Fix_Assignment(vector<Cluster*>* problem_c, Cluster* target, vector<Cluster*>* result);


#endif
