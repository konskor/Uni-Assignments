#ifndef UTILITIES_2_H
#define UTILITIES_2_H

#include <iostream>
#include <string>
#include <chrono>
#include <random>

#include <vector>

#include "Vector.hpp"
#include "InfoCurve.hpp"

#include "Vector_Node.hpp"
#include "Curve.hpp"

using namespace std;

double Compute_w_curve(vector<Vector_Node*>* points);

void Update(vector<Vector_Node*>* vn, double max_c, int max_d);

bool Same_Grid(Curve* c1, Curve* c2);

bool Same_vectors(vector<double>* a, vector<double>* b);


double Min(double x, double y, double z);

double Vector_Distance1(vector<double>* v1, vector<double>* v2, int k);


double DTW(vector<vector<double>*>* p, vector<vector<double>*>* q);


int Curve_NN_True(vector<Curve*>* curves, vector<Curve*>* queries, vector<InfoCurve*>* info);
int Curve_NN_True1(vector<Curve*>* curves, vector<Curve*>* queries, vector<InfoCurve*>* info);

void Print_Curve(vector<InfoCurve*> *info, string filename);


void Print_Vector(Vector* v);

#endif
