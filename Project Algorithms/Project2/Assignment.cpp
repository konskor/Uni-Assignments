#include <iostream>
#include "Assignment.hpp"

using namespace std;


//Assignment1 ==> ALL OK
void Simple_Assignment(vector<Item*>* items, vector<Cluster*>* clusters){

	for(unsigned int i = 0; i < items->size(); i++){	// for each non centroid

		int min_index = -1;
		double min_dist = -1;
		
		
		int min_index_2 = -1;
		double min_dist_2 = -1; 
		
		int counter = 0; 
		for( unsigned int j = 0; j < clusters->size(); j++){ // Compute dist from all centroids

			double distance = Item_Distance(items->at(i),clusters->at(j)->getC() );

			assert( distance > 0);

			if( min_dist < 0 ){		// 1st assignment
			
				min_dist = distance;
				min_index = j;
				
//				j++;
				
				continue;
			}
			
			if( distance < min_dist){	//2nd assignment and onward
			
				// Found a better centroid
			
				min_index_2 = min_index;
				min_dist_2 = min_dist;
				
				min_dist = distance;
				min_index = j;
			}

//			}else if( ( distance >= min_dist) && ( distance < min_dist_2) ){
			
			if( distance > min_dist ){
			
				if( (min_dist_2 < 0) || (distance < min_dist_2) ){
			
					// better 2nd_best centroid
			
					min_index_2 = j;
					min_dist_2 = distance;
				}
			}
			
			counter++;
		}
		
		assert( (min_index >= 0) && (min_index_2 >= 0) );
		
		//Insert the item , into nearest cluster
		clusters->at(min_index)->Add_Item(items->at(i), min_dist);

		//Update 2nd best cluster field of Item (changed)
//		items->at(i)->Change_2nd(clusters->at(min_index_2),min_dist_2);

		clusters->at(min_index_2)->Add_Item2(items->at(i), min_dist_2);

	}
	// Computed the min and 2nd_min distances for all non centroid items
	
	
	
	// Lets take a look at the centroids ==> compute 2nd best for them
	
	for( unsigned int i = 0; i < clusters->size(); i++){
	
		if( clusters->at(i)->getInput() == false) continue;
		
		Item* centroid = clusters->at(i)->getC();
		
		// Clusters1 will contain all clusters, except the one centroid belongs
		vector<Cluster*>* clusters1 = new vector<Cluster*>;
		*clusters1 = *clusters;
		
		clusters1->at(i) = clusters1->back();
		clusters1->pop_back();
		
		double min = -1;
		double min_index = -1;
		
		for( unsigned int j = 0; j < clusters1->size(); j++){
		
//			if( centroid == clusters->at(j)->getC() ) continue;

			double distance = Item_Distance( centroid, clusters1->at(j)->getC() );

			if( (min < 0) || ( distance < min) ){
			
				min = distance;
				min_index = j;
			}
		
		}
		
		clusters1->at(min_index)->Add_Item2(centroid,min);	// centroid best neighbohr cluster
		
		delete clusters1;
		clusters1 = NULL;
	
		centroid = NULL;
	}

	
	cout << "Finished Assignment (1) \n";
}





void Reverse_Assignment(vector<Item*>* items,vector<Cluster*>* clusters,void** hash_tables,int L){

	// In our range search, we will only need (Item*, g(x) for each hash table)

	vector< vector<string>* >* gx = new vector< vector<string>* >;
	
	
	LSH_HT** ht = NULL;
	Curve_LSH** grids = NULL;
	
	bool curve = items->at(0)->isCurve();
	
	if( curve == false){

		ht = (LSH_HT**) hash_tables;	// L hash_tables

		Initialize_Query_Vector(clusters,gx,ht,L);

	}else{

		grids = (Curve_LSH**) hash_tables;	// L grids
		
		Initialize_Query_Curve(clusters,gx,grids,L);
	}
	
	assert( gx->size() == clusters->size() ); 

	//Initialized the queries 
	
	// Computing min distance of centers ==> Initial Range r

	double min = -1;

	for( unsigned int i = 0; i < clusters->size(); i++){
	
		double dist = 0;
	
		for( unsigned int j = i+1; j < clusters->size(); j++){
		
			dist = Item_Distance(clusters->at(i)->getC(), clusters->at(j)->getC() );
		
			if( (min < 0) || ( dist < min) ) min = dist;
		}
	}

	double initial_radius = min;	//initial radius


//	vector<Item*>* copy_items = new vector<Item*>;
//	*copy_items = *items;	// shallow copy => just copy the addresses

	int max_iterations = 10;
//	int counter = 0;

	// If we have less than 10% of items not assigned ==> break, use Simple Assignment
//	int num_of_items = items->size();	// used for break off 

	double remaining = 0;

	
	for( int i = 0; i < max_iterations; i++){	// after (2^9) * initial_radius => stop
	
		double range = pow(2,i) * initial_radius;
	
		for( unsigned int j = 0; j < clusters->size(); j++){ // for each cluster
		
			for( int k = 0; k < L; k++){ // Range_Search each LSH_HT or LSH_curve/grid
			
				if( curve == false){
					assert( ht != NULL);
					ht[k]->RangeSearch(clusters->at(j),gx->at(j)->at(k),range);

				}else{	// we have curves
				
					assert( grids != NULL);
					grids[k]->getHT()->RangeSearch(clusters->at(j),gx->at(j)->at(k),range);
				}
				//finished with this HT, moving to the next one
			}
			// finished with cluster, moving to next one		
		}
		
		double assigned_items = 0;	// how many items are assigned
	
		// Try this one
		Commit_Assignmens_Items(items,clusters);
		
		for( unsigned int j = 0; j < clusters->size(); j++)
			assigned_items+= clusters->at(j)->getSize();
	

		// breakoff point 5% ==> maybe use counter?

		assert(assigned_items <= items->size() );
		
//		remaining = (items->size() - assigned_items) / 100;
		remaining = items->size() - assigned_items;

		double remaining1 = remaining/ items->size();

		
		if( remaining1 < 0.05){	// less than 5% remaining
		
			cout << "Less than 5% remaining in Assignment2 ";
			cout << "==> use simple Assignment for these (Assignment1) \n";
			break;
		}

	}
	//finished Assignment2
	
	assert( remaining >= 0);
	
	// 2 possible scenarios, remaining > 0 or remaining = 0
	// remaining = 0; ======> Nothing
	// remaining > 0 ========> Simple Assignment (Assignment1)
	
	
	if( remaining > 0){	// there are still unassigned points
	
//		cout << "There are still " << remaining << " items \n";
	
		vector<Item*>* items1 = new vector<Item*>;
	
		for( unsigned int i = 0; i < items->size(); i++){

			if( items->at(i)->isAssigned() == false) items1->push_back(items->at(i) );
		}
		
		Simple_Assignment(items1,clusters);
		
		delete items1;	//delete container , not the contents
		items1 = NULL;
	}

	/* Reminder: Check that all the clusters have >= 3 size ( 1 centroid and >= 2 items)
	
		If not , we will have undefined behavior in Silhouette!!!!
		
		If a cluster does have a problem ==> AssignmentUndefined()
		===> pick the furthest point as the new centroid
		
		Doesn't work ==> Found a new way ==> Check_Assignment() and Fix_Assignment()
	*/


	// Cleaning up
	ht = NULL;
	grids = NULL;
	
	for( unsigned int i = 0; i < gx->size(); i++){
	
		delete gx->at(i);
		gx->at(i) = NULL;
	}
	delete gx;

	
	cout << "Finished Range Search Assignment \n";
}




void Commit_Assignmens_Items(vector<Item*>* items, vector<Cluster*>* clusters){

	for(unsigned int i = 0; i < items->size(); i++){
	
	
		// This item is assigned ==> ignore
		if( items->at(i)->isAssigned() == true) continue;
		
		// this item wasn't caught in our range search ( assigned later with simple assignment)
		if( items->at(i)->getCluster() == NULL) continue;
		
		Item* item = items->at(i);
		Cluster* cluster = item->getCluster();
//		double dist1 = item->getDist();
	
		// Now add this item to cluster
		item->Change_Assigned(true);
		cluster->Add_Item1(item);		// Add the item into cluster
		
		
		if( item->get_2nd_Cluster() != NULL){
			item = NULL;
			cluster = NULL;
			continue;
		}
		
		/* Compute 2nd best cluster */

//		double distance = 0;
		
		double min = -1;
		int min_index = -1;
		
		double dist = item->getDist();	// dist from assigned cluster
		for( unsigned int j = 0; j < clusters->size(); j++){
		
			if( clusters->at(j) == cluster) continue;
			
			double distance = Item_Distance(item,clusters->at(j)->getC());	// Utilities3.hpp
			
			
			if( (distance >= dist) && ( (min < 0) || ( distance < min) ) ){
			
				// we must find a centroid which is farther than the cluster which we are assigned
			
				min = distance;
				min_index = j;
			}
		}
		
		if( min < 0){
			
			// Unfortunately (maybe due to range search), 
			// the item was assigned to the farthest cluster
			// the 2nd best ==> is itself
				
			// This is THE worst case scenario, it should almost never happen
				
//			cout << "Worst case scenario, in Commit_Assignment_Items() \n";
				
			min = dist;

			item->Change_2nd(cluster,min);
				
			continue;
		}
		
		item->Change_2nd( clusters->at(min_index), min);

		// 1 item down, more to go
		item = NULL;
		cluster = NULL;
	}
	// Finished with all the Items => return

//	cout << "Done with Commit_Assignments1() \n";

}











void Initialize_Query_Vector(vector<Cluster*>* clusters, vector< vector<string>* >* gx,LSH_HT** ht, int L){

// For each cluster/centroid , find the gx of all Hash tables


	for( unsigned int i = 0; i < clusters->size(); i++){	// for each cluster/centroid
	
		Item* centroid = clusters->at(i)->getC();
		
		Vector* v = centroid->getVector();
		
		
		vector<string>* str_g = new vector<string>;
		
		
		for( int j = 0; j < L; j++){	//Hash the vector in each hash Table
		
			string g = ht[j]->Hash(v);

			str_g->push_back(g);
		}
		// str_g is filled ==> push_back() into  vector<>gx
		
		gx->push_back(str_g);
		str_g = NULL;
		
		v = NULL;
		centroid = NULL;
	}

	cout << "Finished Initializing Query Vectors \n"; 


}




void Initialize_Query_Curve(vector<Cluster*>* clusters, vector< vector<string>* >* gx, Curve_LSH** grids, int L){


	for( unsigned int i = 0; i < clusters->size(); i++){

		Item* centroid = clusters->at(i)->getC();
		Curve* c = centroid->getCurve();
		
		vector<string>* str_g = new vector<string>;

		for( int j = 0; j < L; j++){	//for all Grids/HashTables

			Curve* g = NULL;
			grids[j]->Curve2Gcurve1(c, &g);	// Creating the Grid curve
			
			Vector* v = NULL;
			grids[j]->Gcurve2Vector1(g,&v);	 //Creating	the vector 
			
			grids[j]->Update1(v);	// update the query

			string gx1 = grids[j]->getHT()->Hash(v);

			str_g->push_back(gx1);

			delete v;
			delete g;
			v = NULL;
			g = NULL;
		}
		//str_g is filled ==> push_back() into vector<> gx
		
		gx->push_back(str_g);
		str_g = NULL;
		
		centroid = NULL;
		c = NULL;
	}

}








// Items will contain ALL input (points and centroids)
void Initialize_LSH_Vector(vector<Item*>* items, int k, int L, LSH_HT** ht){

	// k is number of h(x)

	vector<Vector*>* Vectors = new vector<Vector*>;
//	Item2Vector(items,Vectors);
	
	for(unsigned int i = 0; i < items->size(); i++){	// Items ==> Vectors
	
		Vector* temp = items->at(i)->getVector();
		Vectors->push_back(temp);
	
		temp = NULL;
	}
	double w = Compute_w(Vectors);
	
	cout << "Finished Computing w in Initialize LSH_Vector, w is " << w << endl;
	
	/* Now create the empty hash Tables */
	
//	int numbuckets = Vectors->size()/8;
	int numbuckets = Vectors->size()/16;
	
	vector<double>* c = items->at(0)->getVector()->getC();
	int d = c->size();
	c = NULL;
	
	for(int i = 0; i < L; i++){
	
		random_device rd;
		mt19937 generator(rd());	//Standard mersenne_twister_engine seeded with rd()
		
		std::uniform_real_distribution<double> dis(0.0, w);
	
		ht[i] = new LSH_HT(numbuckets,k,d,w,generator,dis);		
	}
	//Created the empty LSH hash tables


	// Don't forget to "link" the Vector_Node and the Item*
	vector<Vector_Node*>* nodes = new vector<Vector_Node*>;
	for( unsigned int i = 0; i < items->size(); i++){	// Create the vector_Nodes
	
		Vector_Node* temp_node = new Vector_Node(Vectors->at(i),NULL,NULL,items->at(i) );
	
		nodes->push_back(temp_node);

		temp_node = NULL;
	}
	
	//Inserting the points into the LSH
	for(int i = 0; i < L; i++) ht[i]->Insert1(nodes);

	delete Vectors;	// delete the container
	Vectors = NULL;	// the actual Vectors will be deleted when the equivalent Items get destroyed

	delete nodes;	// deleting the vector
	nodes = NULL;	// the actual Nodes will be deleted when we destroy the Hash Tables

	cout << "Finished Initialize_LSH_Vector \n";
}










//ALL OK
void Initialize_LSH_Curve(vector<Item*>* items, int k, int L, Curve_LSH** grids){


	vector<Curve*>* Curves = new vector<Curve*>;
	
	for(unsigned int i = 0; i < items->size(); i++){	// Items ==> Curves
	
		Curve* temp = items->at(i)->getCurve();
		Curves->push_back(temp);
	
		temp = NULL;
	}
	double delta = Compute_delta(Curves);	//Utilities2.cpp

	int d = Curves->at(0)->getD();

	for( int i = 0; i < L; i++){
	
		random_device rd;
		mt19937 generator(rd());	//Standard mersenne_twister_engine seeded with rd()
		
		std::uniform_real_distribution<double> dis(0.0, delta);	//delta , not d
		grids[i] = new Curve_LSH(d,delta,generator,dis);				
	}
	cout << "Built our new empty LSH_Curves/grids\n";	

	/* Creating our Grid points and padding them ( if needed) */
	
	// does everything (Curve->GCurve->Vector->Vector_Node and the padding)	
	for(int i = 0; i < L; i++)	grids[i]->Curve2Gcurve(Curves);	

	// also update the "Item" field of each Vector_Node
	for(int i = 0; i < L; i++)	grids[i]->UpdateVector_Node(items);


	// now Create the LSH_HT of each grid

	for(int i = 0; i < L; i++){
	
		random_device rd;
		mt19937 generator(rd());	//Standard mersenne_twister_engine seeded with rd()
		
		vector<Vector_Node*>* vn = grids[i]->getGpoints();
		
		double w = Compute_w_curve(vn);		

		cout<<"W is " << w << endl;	
		
		std::uniform_real_distribution<double> dis(0.0, w);
	
		grids[i]->setLSH_HT(k,w,generator,dis);	// Creating the hasH Table	
		vn = NULL;
	}

	// Finally Insert the Vector_Nodes into the LSH_HT
	for(int i = 0; i < L; i++) grids[i]->Insert();
	
	delete Curves;
	Curves = NULL;

	cout << "Finished Initialize_LSH_Curve \n";
}







bool Check_Assignment(vector<Item*>* items,vector<Cluster*>* clusters){

// We check after assignment, if a cluster has less than 2 items
// If true ===> UNDEFINED BEHAVIOUR in Silhouette

// Return true , if a cluster does have a problem ==> we fix it, but we have to do the assignment again

	bool result = false;

	vector<Cluster*>* problem_c = new vector<Cluster*>;	// size < 2
	
	vector<Cluster*>* good_c = new vector<Cluster*>;	// size >=2




	for( unsigned int i = 0; i < clusters->size(); i++){	// for each cluster, check the size
	
		if( clusters->at(i)->getSize() >= 2){	// nothing wrong
		
			good_c->push_back( clusters->at(i) );
			continue;
		}
	
		cout << "A cluster was found with size < 2, in Check_Assignment \n";
		result = true;
		
		problem_c->push_back( clusters->at(i) );
	}
	
	if( result == false){	// Nothing is wrong ==> return
		cout << "Assignment is OK\n";
		
		delete problem_c;
		problem_c = NULL;
		
		delete good_c;
		good_c = NULL; 
		
		return result;
	}




	cout << "Assignment is NOT OK \n\n";


	// Found all the problem clusters ==> size < 2

	/* Reorder the good clusters ==> Non-Decreasing order of sizes ==> Insertion Sort */
	
	vector<Cluster*>* good_c1 = new vector<Cluster*>;
	
	unsigned int size1 = good_c->size();
	for( unsigned int i = 0; i < size1; i++){	// same size as good_c
	
		int max_size = 0;
		int max_index = -1;
		
		for( unsigned int j = 0; j < good_c->size(); j++){	//find max
		
			int size = good_c->at(j)->getSize();
			if( size > max_size ){
			
				max_size = size;
				max_index = j;
			}
		}
		// found the max_size
		
		good_c1->push_back( good_c->at(max_index) );
		
		// Remove good_c->at(j)
		good_c->at(max_index) = good_c->back();	// overwrite and delete 
		good_c->pop_back();
	}
	// Now good_c1 is ordered version of good_c (by size)

	// Also good_c is empty
	delete good_c;
	good_c = NULL;





	/* For each good_c1 (Cluster) check how many new centroids can we fit */
	
	vector<Cluster*>* final_clusters = new vector<Cluster*>;
	// This will contain ALL of the clusters (our new state of the problem)
	
	unsigned int num_problems = problem_c->size();	//how many outliers
	
	
	for( unsigned int i = 0; i < good_c1->size(); i++){	//for each potential cluster
	
		int size = good_c1->at(i)->getSize();
		assert( size > 1);
		
//		int num_centroids = (size/2) -1;
		unsigned int fit = (size/2) - 1;	//how many new centroids can we fit in this cluster
	
		assert( fit >= 1);
		// because we have sorted them in non-decreasing order 
		// ==> Error if we can't fit outliers anywhere
	
	
	
		/*********************************************************
			problem_c1 Clusters , will be the new/extra centroids 
		 	inserted into the good_c1->at(i) (Cluster)
		***********************************************************/
		
		vector<Cluster*>* problem_c1 = new vector<Cluster*>;
	
		int counter = 0;
	
		if( fit < num_problems){	// more outliers than we can fit
			counter = fit;
		}else{	// fit >= num_problems
			counter = num_problems;
		}
	
//		for( int j = 0; j < num_centroids; j++){

		for( int j = 0; j < counter; j++){
		
//			if( num_problems == 0) break;	// no more outliers
			
			problem_c1->push_back( problem_c->back() );
			problem_c->pop_back();
			
			num_problems--;
		}
		
		/***********************************************************
			Problem_c1 now contains the outliers(clusters) 
			who will enter the target cluster  ==> good_c1->at(i)
		************************************************************/
	
//		vector<Item*>* items1 = good_c1->at(i)->getItems;	//items of potential cluster
		vector<Cluster*>* result = new vector<Cluster*>;	// the new centroids
		
		Fix_Assignment(problem_c1, good_c1->at(i),result);
		
//		int k = problem_c1->size();
//		Fix_Assignment(k, good_c1->at(i),result);	// INCOMPLETE
		
		cout << "Fix_Assignment is done \n";

		/* Insert the new clusters into our solution */ 
		for( unsigned int j = 0; j < result->size(); j++)	// insert into final vector<> 
			final_clusters->push_back( result->at(j) );
			
		
		/*********************************************************************************
			Delete the old problematic clusters
			insert their centroids back to vector<>* items, if they are items of Input file 
			else delete them
		************************************************************************************/
		

//		cout << problem_c1->size() << endl;
		
		for( int j = problem_c1->size() - 1; j >= 0 ; j--){
		
			Item* centroid1 = problem_c1->at(j)->getC();
			
			bool input = problem_c1->at(j)->getInput();
			
			delete problem_c1->at(j);
			problem_c1->at(j) = NULL;
		
//			if( problem_c1->at(j)->getInput() == true){	// insert back the centroid of cluster


			if( input == true){	// insert back the centroid of cluster	
				items->push_back(centroid1);
			}
		
			centroid1 = NULL;
		}

		delete result;
		result = NULL;


		delete problem_c1;
		problem_c1 = NULL;
//		items1 = NULL;

		
		if( num_problems == 0) break;	//fixed all the outliers
	}
	//Finished with the problematic outliers, one last thing
	
	delete problem_c;
	problem_c = NULL;
	
	cout << "Hello2\n";
	
	assert( good_c1 != NULL);
	
	/* Insert into vector<>* final_clusters, the original good clusters */
	for(unsigned int i = 0; i < good_c1->size(); i++)
		final_clusters->push_back(good_c1->at(i) );
		
	/* Copy the contents of (final_clusters) into (vector<> clusters) ==> SHALLOW COPY
		===> New state of problem
	*/

	*clusters = *final_clusters;
	
	/* Check your vector<>* items, so it only contains non-centroid Items */
	
	UpdateItems(items);
	
	cout << "Finished Check_Assignment \n";
	
	// Last Deletes

//	delete good_c;
//	good_c = NULL;

	delete good_c1;
	good_c1 = NULL;
	
	delete final_clusters;
	final_clusters = NULL;
	

	return true;
}







// ALL OK
void Fix_Assignment(vector<Cluster*>* problem_c, Cluster* target, vector<Cluster*>* result){

	/* We try to fit (as new centroids) the (ex-outliers) into target cluster
		Basically, we partition the original target cluster into smaller clusters
	
	In order to choose , which items will be the new centroidsm, we will follow Initialization++
	The only difference is that the 1st centroid is already chosen (don't have to pick at random)
	*/
	
	// (items1) ==> contains all the non centroids
	
	
	
	/***** IMPORTANT: Same-y code with Initialization++ **********/
	
	
	
	vector<Item*>* items = target->getItems();	//all non-centroid items of Cluster target


	/* The 1st centroid is the existing one in target cluster */
	
	Item* centroid = target->getC();

	/* Compute the distances of each item from the 1st cluster */

	// This vector will hold the minimum distance, of each Item to some cluster
	vector<double>* D = new vector<double>;
	
	double max = -1;

	for(unsigned int i = 0; i < items->size(); i++){	// Compute the D(i)
		
		double distance = Item_Distance(items->at(i), centroid);
		
		D->push_back(distance);

		if( distance > max) max = distance;
		
	}
	
		/* Normalizing the D(i)'s */
	
	vector<double>* D1 = new vector<double>;
	for( unsigned int i = 0; i < D->size(); i++){

		double normD = ( D->at(i) / max);
		D1->push_back(normD);
	}

	/* Compute the P(r) = P(r-1) + D(r)^2 */

	vector<double>* P = new vector<double>;
	
	double dr = pow(D1->at(0),2);	// D(r)^2
	P->push_back(dr);
	
	for( unsigned int i = 1; i < items->size(); i++){
	
		dr = pow(D1->at(i),2);
		
		double Pr = P->at(i-1) + dr;
		P->push_back(Pr);
	}

	/* Pick a float x in [0,P->back()] */

	random_device rd1;
	mt19937 gen1(rd1());
	uniform_real_distribution<double> dis1(0,P->back());

	double x = dis1(gen1);
	int r = BinarySearch(P,x,0,P->size()-1);	//Searching for our new centroid


	delete D1;
	D1 = NULL;
	
	delete P;
	P = NULL;


	/* Found our 2nd centroid */


	Item* centroid1 = items->at(r);	//our new centroid
	Cluster* cluster1 = new Cluster(centroid1,true);
	result->push_back(cluster1);

	// Removing the centroid from the vector<Items> items1
	Item* temp =  items->back();	//access the last element
	items->at(r) = temp;	//overwrite the value of the previous item
	items->pop_back();


	// Removing the centroid from the vector<> D
	double temp1 =  D->back();	//access the last element
	D->at(r) = temp1;	//overwrite the value of the previous item
	D->pop_back();
	
	
	// Don't need these anymore
	centroid = NULL;
	temp = NULL;
	
	centroid1 = NULL;
	cluster1 = NULL;



// For the rest:
// We only compute the distances to the recently found centroid
// If new_D(i) < D(i) ==> D(i) = new_D(i)

// Again start with max = -1, and find maxD(i) ==> normalize ==> vector<> D1
// then Compute P() and find r

// More code, but each distance is computed only once


	/* Now for the rest */


	int k =  problem_c->size() + 1;
	// we want to fit in this cluster ===> problem_c->size() + 1  , clusters
	// + 1 , because we take into account (the outliers + existing centroid)
	
	// Now we have fitted (existing centroid + 1 outlier) ==> check the rest 
	 
	for( int t = 2; t < k; t++){	// compute the remaining centroids 
	
		max = -1;
		
		centroid1 = result->back()->getC();	// get the last computed centroid

		for(unsigned int i = 0; i < items->size(); i++){	// Compute the D(i)
			
			double distance = Item_Distance(items->at(i), centroid1);
		
			if( distance > max) max = distance;	//new max
		
			if( distance < D->at(i) ) D->at(i) = distance;	// new min

		}//Computed all The D(i)
	
		/* Normalizing the D(i) */
	
		vector<double>* D2 = new vector<double>;
		for( unsigned int i = 0; i < D->size(); i++){

			double normD = ( D->at(i) / max);
			D2->push_back(normD);
		}

		/* Compute the P(r) = P(r-1) + D(r)^2 */

		vector<double>* P1 = new vector<double>;
	
		double dr1 = pow(D2->at(0),2);
		P1->push_back(dr1);
	
		for( unsigned int i = 1; i < items->size(); i++){
	
			dr1 = pow(D2->at(i),2);
		
			double Pr1 = P1->at(i-1) + dr1;
			P1->push_back(Pr1);
		}

		/* Pick a float x in [0,P->back()] */

		random_device rd2;
		mt19937 gen2(rd2());
		uniform_real_distribution<double> dis2(0,P1->back());

		double x1 = dis2(gen2);
		int r1 = BinarySearch(P1,x1,0,P1->size()-1);	//Searching for our new centroid

		delete D2;
		D2 = NULL;
	
		delete P1;
		P = NULL;
		
		
		/* Found our new centroid */

		Item* centroid2 = items->at(r1);	//our new centroid
		Cluster* cluster2 = new Cluster(centroid2,true);
		result->push_back(cluster2);

		// Removing the centroid from the vector<Items>
		temp =  items->back();	//access the last element
		items->at(r1) = temp;	//overwrite the value of the previous item
		items->pop_back();


		// Removing the centroid from the vector<> D
		double temp2 =  D->back();	//access the last element
		D->at(r1) = temp2;	//overwrite the value of the previous item
		D->pop_back();
		
		centroid2 = NULL;
		cluster2 = NULL;

	}	// found all the centroids ==> Exiting

	delete D;
	D = NULL;
	
	centroid = NULL;
//	cluster = NULL;
	temp = NULL;
	
	items = NULL;
}

