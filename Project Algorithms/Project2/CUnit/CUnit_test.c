#include "CUnit/Basic.h"

//#include "Utilities.hpp"
//#include "Utilities2.hpp"

#include "Functions.h"


/* Initializer/Celaner */

int init(){ return 0; }
int clean(){ return 0; }


/* Test Functions */


void testMax(){

	CU_ASSERT( Max(1,2,14) == 14);

	CU_ASSERT( Max(-1,-5,7) == 7);
	
	CU_ASSERT( Max(-2, 10,100) == 100);
}





void testMin(){

	CU_ASSERT( Min(1,2,14) == 1);

	CU_ASSERT( Min(-1,-5,7) == -5);
	
	CU_ASSERT( Min(-2, 10,100) == -2);
}


void testMod(){

	CU_ASSERT( Mod(1,10) == 1);
	
	CU_ASSERT( Mod(2,3) == 2);
	
	CU_ASSERT( Mod(2,2) == 0);

	CU_ASSERT( Mod( 4,3) == 1);
}







int main(){

	CU_pSuite pSuite = NULL;

	/* initialize the CUnit test registry */
	if( CU_initialize_registry() != CUE_SUCCESS ) return CU_get_error();


	/* Add a suite to the registry */
	pSuite = CU_add_suite("Suite_1", init, clean);
	
	if( pSuite == NULL){

		CU_cleanup_registry();
		return CU_get_error();
	}


	/* Add the tests to the suite */
	if( CU_add_test(pSuite, "test of Max() in Utilities.hpp", testMax) == NULL){
	
		CU_cleanup_registry();
		return CU_get_error();
	}


	if( CU_add_test(pSuite, "test of Min() in Utilities2.hpp", testMin) == NULL){
	
		CU_cleanup_registry();
		return CU_get_error();
	}
	
	if( CU_add_test(pSuite, "test of Mod() in Utilities.hpp", testMod) == NULL){
	
		CU_cleanup_registry();
		return CU_get_error();
	}



	CU_basic_run_tests();
	
	CU_cleanup_registry();
	return CU_get_error();

}



