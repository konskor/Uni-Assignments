#include <iostream>
#include <ctime>
#include <cstdlib>

#include "classes.h"

using namespace std;


	/* Constructors-Destructors */



Waggon::Waggon(int c=20,int f=20,int f1=10 ){		//maxCapacity,ticket_fine,reduced_ticket_fine
	
	maxCapacity = c;
	pcount = 0;
		
	inspection=false;
	money=0;
	
	
	
	p_array= new person*[c];				//creating array of pointers to persons
	
	for(int i=0;i<c;i++){					//setting each one to NULL
		
		p_array[i]=NULL;
	}
	
	
	
	
	totalTrespassers=0;
	trespassers_caught=0;
	trespassers_escaped=0;
	
	ticket_fine=f;
	reduced_ticket_fine=f1;
	
	
	cout<<"A waggon with capacity for "<<maxCapacity <<"passengers, was created\n";
	
}

Waggon::~Waggon(){
	
	for(int i=0;i<maxCapacity;i++){
		
		delete p_array[i];	
		p_array[i]=NULL;
	}
	
	delete[] p_array;
	
	
	cout<<"A waggon was destroyed\n";
}


Train::Train(int N,int w=3){
	
	
	totalWaggons = w;
	
	
	w_array=new Waggon*[totalWaggons];	
		
	for(int i=0;i<totalWaggons;i++){
		
		w_array[i] = new Waggon(20,10,5);	
	}	

	stations = N;
	this->money = 0;
		
	cout<<"A metro train with "<< totalWaggons<< "waggons, was created\n";
}

Train::~Train(){
	
	cout<<"A metro train was destroyed\n";
	
	for(int i=0;i< totalWaggons;i++){
	
	 delete w_array[i];
	 w_array[i]=NULL;
	
	}
		
delete[] w_array;
	
}

Person::Person(){
	this->have_ticket= rand() %2;	//0 for false , 1 for true
	this->have_half  = rand() %2;
	this->deserve_half= rand() %2;
	
	cout << "Created a person\n";
};

Person::~Person(){
};



					/*Train Functions */

Waggon** Train::get_array(){
	
	return w_array;
	
}

int Train::get_totalW(){
	
	return this->totalWaggons;
}

void Train::reduce_station(){
	
	this->stations--;
}

bool Train::operate(){

	if(stations>0) return true;
		
return false;		
}


void Train::printStatistics(){


	for(int i=0;i<totalWaggons;i++){
		
		int j=0;
		
		w_array[i]->printStatistics();			//print statistics of each waggon
		
		w_array[i]->get_money(j);				//get money from each waggon
		money+= j;								//sum up all the money
	}
	
cout<<"The train has gained "<< this->money<<"money\n";	
}



					/*Waggon Functions*/
										

void Waggon::get_money(int& m) const{
	
	m = this->money;	
}



void Waggon::inStation(){
	
	
	if(this->pcount == 0) this->Entering();
	else{
		this->Leaving();
		this->Entering();
	}
	
	int rand_choice = rand() %2;
	
	if(rand_choice == 0) this->inspection = true;
	else this->inspection=false;
	

	return;
	

}




void Waggon::Leaving(){
	
	int people= rand() % (this->pcount+1);		// [0,all] will leave
	
	if(people == 0) return;
	if(people == pcount){			//all will leave
	 
	 pcount = 0;
	 
	 for(int i=0;i<maxCapacity;i++) {		//deleting the pointers
	 
	 delete p_array[i];
	 p_array[i] = NULL;
	 
	}
	 
	 
	}
	else{							//the people that will leave are : people=(0,all) 
									// we will delete the the nodes at the end 

		for(int i=0;i<people;i++){			//deleting from the end
			
			delete p_array[pcount-i];
			
	}
			
	
	pcount= pcount - people;			//reducing counter
	}
}



void Waggon::Entering(){
	
	int people = rand() % (maxCapacity+1);			// [0,maxCapacity]
	
	
	if(pcount == maxCapacity) return;							//no one can enter
	
	
															//some can enter
		
		if(pcount + people <= maxCapacity ){				// all the people will enter
		
			int i=0;
			
			while(people != 0 ){
				
				if(p_array[i] == NULL){				// if we find a vacant seat
					
					p_array[i] = new Person;		// the person sits
					
					people--;						//reducing the people searching for a seat
				}
				
				i++;								// increasing count
			}
			
		pcount+=people;	
				
		return;	
			
		}else{ 						//not all the people will enter
			
			int x = maxCapacity-pcount;		//how many people can enter
			   
			
			int i=0;
			
			while( x != 0 ){
				
				if(p_array[i] == NULL){				// if we find a vacant seat
					
					p_array[i] = new Person;		// the person sits
					
					x--;							//reducing the people searching for a seat
				}
				
				i++;								// increasing count
			}
			
			
			pcount = maxCapacity;
			
			return;
						
		}	
		
	}
			



void Waggon::betweenStations(){
	
	if(inspection == false){
		
		
		
		for(int i=0;i<maxCapacity;i++){
			
			if(p_array[i] == NULL ) continue;
			
			int j=0,k=0,l=0;
			

			
			p_array[i]->get(j,k,l);				//have_ticket, have reduced ticket, deserve reduced ticket
												// (1 = true, 0 = false)
			
			
			if( (j==0) || (k==1 && l==0 ) ){		//if they didnt have ticket or
				this->totalTrespassers++;			//had reduced one, but didnt deserve it
				this->trespassers_escaped++;						
			}	
		}
		
		
		

	}
	else{
		
		for(int i=0;i<maxCapacity;i++){
			
			if(p_array[i] == NULL ) continue;
			int j=0,k=0,l=0;
	
			
			p_array[i]->get(j,k,l);				//have_ticket, have reduced ticket, deserve reduced ticket
												// (1 = true, 0 = false)
			
			
			
			if(j==0){								//if they didnt have ticket
				this->totalTrespassers++;			
				this->trespassers_caught++;	
				
				this->money+= this->ticket_fine;	
				
				delete p_array[i];						//kicking the person out						
				p_array[i]=NULL;			
			}
			
			if(j==1 && k==0){					//or had reduced one, but didnt deserve it
			
				this->totalTrespassers++;			
				this->trespassers_caught++;
				
				this->money+= this->reduced_ticket_fine;		//this is the only part that changes
			
				delete p_array[i];						//kicking the person out						
				p_array[i]=NULL;		
			}
		}
			
	}
	
	
return;
	
}



void Waggon::printStatistics(){
	
	cout<< "We had a total of "<< this->totalTrespassers << " tresspassers\n";
	cout<< this->trespassers_caught <<" were caught\n";
	cout<< "However "<< this->trespassers_escaped << " escaped without paying the fine \n\n";
	
	return;
	
}		
		
				/* Person Functions */


void Person::get(int&a,int&b,int&c) const {
		a=have_ticket;
		b=have_half;
		c=deserve_half;
	}	


