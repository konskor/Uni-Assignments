#include <stdio.h>
#include <stdlib.h>

#include "Functions.h"

int Mod(int x, int y){	//Utilities.hpp

	return ( x %y + y) % y;

}



double Min(double x, double y, double z){	//Utilities2.hpp

	double result = x;
	
	if( result > y) result = y;	
	if( result > z) result = z;
	
	return result;
}

double Max(double x, double y, double z){	//Utilities.hpp

	double result = x;
	
	if( result < y) result = y;	
	if( result < z) result = z;
	
	return result;
}




