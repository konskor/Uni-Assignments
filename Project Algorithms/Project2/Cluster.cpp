#include <iostream>
#include <vector>

#include "Cluster.hpp"

using namespace std;


/* Constructor/Destructor */


//input ==> if centroid is made up or existing point
Cluster::Cluster(Item* c, bool i) : centroid(c), input(i){
	items = new vector<Item*>;
	
	items_2 = new vector<Item*>;	//maybe remove
	
	if( input == true) size = 1;
	else size = 0;
	
	c->Change_Assigned(true);	// the centroid will always count as assigned

	c->ChangeCluster(this,0);
}


Cluster::~Cluster(){

	// If a cluster is to be destroyed, modify the cluster field of each item to NULL 
	for(unsigned int i = 0; i < items->size(); i++){

		items->at(i)->ChangeCluster(NULL,-1);
		items->at(i)->Change_Assigned(false);	// Item is no longer assigned
	}

	for(unsigned int i = 0; i < items_2->size(); i++){

		items_2->at(i)->Change_2nd(NULL,-1);
	}

	delete items_2;


//	this->EmptyCluster();
	delete items;	// the vector<>, the Items will be deleted later
	
	// Don't delete 
//	delete centroid;	// the Centroid will be deleted here
//	centroid = NULL;
	
	centroid->Change_Assigned(false);
	centroid->ChangeCluster(NULL,-1);
	
	if( input == false) delete centroid;	//made up item ==> delete
	centroid = NULL;
}

/* Accessors */

Item* Cluster::getC(){ return centroid;}

int Cluster::getSize(){ 

	int result = size;

	return result;

}



vector<Item*>* Cluster::getItems(){ return items; }


void Cluster::getAllItems(vector<Item*>* result){

	vector<Item*>* items1 = new vector<Item*>;

	*items1 = *items;	// shallow copy

	if( input == true)	items1->push_back(centroid);

//	items1->push_back(centroid);

	*result = *items1;	// shallow copy

	delete items1;
	items1 = NULL;
}





bool Cluster::getInput(){ return input; }


/* Functions */

void Cluster::Add_Item(Item* item,double dist){	//Our default add Item (mostly use this)

	this->size++;
	this->items->push_back(item);
	
	item->ChangeCluster(this,dist);	// modify the cluster field of Item
}


void Cluster::Add_Item1(Item* item){

	//Used by Commit_Assigments_Items() ==> Assigment.hpp
	// At the end of a round ( before increasing the range), we add the items
	// without modifying their filed ( pre-modified during Range_search)


	this->size++;
	this->items->push_back(item);
}




// t
void Cluster::Add_Item2(Item* item,double dist){


	this->items_2->push_back(item);
	
	item->Change_2nd(this,dist);
}









// Removes Item from cluster, and modifies their cluster field
void Cluster::Delete_Item(Item* item){

//	this->size--;

	unsigned int i = 0;
	bool find = false;

	for(i = 0; i < this->items->size(); i++){
	
		if( this->items->at(i) == item){
		
			item->ChangeCluster(NULL,-1);	// modify the cluster filed of Item
		
			Item* temp = items->back();
			items->at(i) = temp;
			items->pop_back();
			
			find = true;
			
			temp = NULL;
			break;
		}
	}

	// Item doesn't exist in this cluster
//	if( i == items->size() ) cout << "ERROR in Cluster::DeleteItem() \n"

//	if( find == true ) cout << "ERROR in Cluster::DeleteItem() \n";
	
	this->size--;
}


// Didn't use
void Cluster::Commit_Assignments(){

// this function , modifies the (bool assigned) field of each item
// Basically we commit the changes => mark as assigned (Item).

	for(unsigned int i = 0; i < items->size(); i++) items->at(i)->Change_Assigned(true);
}



// Didn't use == > Commit_Assignments() in Assignment.cpp
void Cluster::Commit_Assignments1(vector<Cluster*>* clusters){

/* Commit Assignments, but also 
	check if all the items in cluster habe their "2nd best cluster" field ==> filled
*/


	for(unsigned int i = 0; i < items->size(); i++){
	
		Item* item = items->at(i);
		item->Change_Assigned(true);
		
		if( item->get_2nd_Cluster() != NULL){
			item = NULL;
			continue;
		}

//		double distance = 0;
		
		double min = -1;
		int min_index = -1;
		
		double dist = item->getDist();	// dist from assigned cluster
		for( unsigned int j = 0; j < clusters->size(); j++){
		
			if( clusters->at(j) == this) continue;
			
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
				
//			cout << "Worst case scenario, in Commit_Assignment1() \n";
				
			min = dist;

			item->Change_2nd(this,min);
				
			continue;
		}
		
		item->Change_2nd( clusters->at(min_index), min);

		// 1 item down, more to go
		item = NULL;
	}
	// Finished with all the Items => return

//	cout << "Done with Commit_Assignments1() \n";

}







void Cluster::EmptyCluster(){

// We want to empty this cluster ,
// Seperate each item from this cluster

/*
	if( items->size() == 0){
	
		//this can only happen if after an Update()/Fix_Assignment(), this cluster was created
		cout << "Nothing to empty, in EmptyCluster() \n";
		return;
	}
*/

	cout << "(In Cluster::EmptyCluster) Items size is " << items->size() << endl;

/*
	for( int i = items->size() - 1; i >= 0; i--){
	
		// Change assignment = false, cluster = NULL
		items->at(i)->ChangeCluster(NULL,-1);
		items->at(i)->Change_Assigned(false);
		
		items->pop_back();	// remove from items assigned to cluster
	}
*/


	for( int i = 0; i < items->size(); i++){
	
		// Change assignment = false, cluster = NULL
		items->at(i)->ChangeCluster(NULL,-1);
		items->at(i)->Change_Assigned(false);	
	}
	
	for( int i = 0; i < items_2->size(); i++){
	
		items_2->at(i)->Change_2nd(NULL,-1);	
	}
	

	delete items;
	items = NULL;
	items = new vector<Item*>;
	
	delete items_2;
	items_2 = NULL;
	items_2 = new vector<Item*>;
	

	if(input == true) size = 1;	//there is still the centroid
	else size = 0;	// no items here
}



















