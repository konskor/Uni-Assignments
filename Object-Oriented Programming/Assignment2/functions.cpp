#include "Classes.h"
#include <fstream>



/* Application	*/
		
Application::Application(const string& ids= " ",const string& first= " ",const string& last= " ",const string& dest= " ",const time_t& T1=0,const time_t& T2=0, const char& c=' '){
	
	id = ids;				//strings
	First_Name = first;
	Last_Name = last;
	Destination = dest;
	
	ETA_Airport = T1;			//Time
	Destination_time = T2;
	
	Class_seat = c;				//Char
	
}		

/*
Application::Application(ifstream& file1){
	
	file1 >> id ;				//strings
	file1 >>  First_Name; 
	file1 >> Last_Name ;
	file1 >> Destination;
	
	file1 >> ETA_Airport ;			//Time
	file1 >> Destination_time; 
	
	file1 >> Class_seat;				//Char
	
/*cout << id << endl;
cout << First_Name <<endl;
cout << Last_Name <<endl;
cout << Destination <<endl;

cout << ETA_Airport<<endl;
cout << Destination_time<<endl;	

cout << Class_seat<<endl;	
}
*/
		
		
Application::~Application(){
	
	
cout << "An application has been destroyed\n";		
}		

bool Application::matches(const Flights& A)const{

	if(A.get_destination() == Destination){					
	
	//	cout << "Same destination \n\n";
	
	
		if( A.departs_at() >= ETA_Airport){				//if the person can reach the plane on time
		
		//	cout << "Reaches the plane on time \n\n";
		
			if( A.arrives_at() <= Destination_time ){			//if the plane reaches the destination in the desired time
				
		//		cout << " The plane arrives at a good time (at destination) \n\n";
				
				if(A.get_available() > 0) {
			//		cout << "There are available seats \n\n";
					
					return true	;		
				
				}
			return false;
			}
		
		return false;	
		}
	return false;	
	}
return false;	
}

bool Application::is_luxury_class() const{
	
	if( Class_seat == 'B' ) return true;
	
	return false;
	
}


string Application::get_name() const{
	
	string result1 = First_Name + " " + Last_Name;
	
	
return result1;	
}

string Application::get_id() const{	
	
	return id;
			
}

time_t Application::arrived_by() const{
	
	return Destination_time;
	
}

/*
Application& Application::operator=(const Application& A){
	
	
	if(this!= &A){				// checks not to assign to itself 
		
		id.assign(A.id);							//replacing the contents of each member
		First_Name.assign(A.First_Name); 
		Last_Name.assign(A.Last_Name);
		Destination.assign(A.Destination);
		
		ETA_Airport = A.ETA_Airport;
		Destination_time =  A.Destination_time;
		
		Class_seat= A.Class_seat;
		
		cout<< "\nI just performed an application ASSIGNMENT  \n\n";
	}
	
return *this;	
}
*/


																		/* Flights */
					
Flights::Flights(const string& s,const time_t& t1,const time_t& t2,const int&A,const int&B){
	
	
	Destination = s;			
	
	Departure = t1;
	Length	= t2;
	
	maxA = A;				//Initialising Economic
	bookedA = 0;
	
	maxB = B;				//Initialising Luxury
	bookedB = 0;
	
	bookings = new Application*[maxA+maxB];			//allocating memory for  array of pointers
	
	for(int i=0 ;i < maxA+maxB ;i++){
		
		bookings[i] = NULL;							//initialising each pointer
		
	}	
	
}
/*
Flights::Flights(ifstream& file1){
	
	
	file1 >> Destination;			
	
	file1 >> Departure;
	file1 >> Length;
	
	file1 >> maxA;				//Initialising Economic
	bookedA = 0;
	
	file1 >> maxB;				//Initialising Luxury
	bookedB = 0;
	
	bookings = new Application*[maxA+maxB];			//allocating memory for  array of pointers
	
	for(int i=0 ;i < maxA+maxB ;i++){
		
		bookings[i] = NULL;							//initialising each pointer
		
	}
	
/*cout << Destination << endl;
cout << Departure<<endl;
cout << Length <<endl;
cout << maxA <<endl;

cout << maxB<<endl;	
}
*/
				
				
Flights::~Flights(){
		
	//delete[] Destination;			//string will be deleted automatically, when it goes out of scope
		
	for(int i =0 ;i< maxA+maxB ;i++){
		
		delete bookings[i];					//calling destructors of applications 
		bookings[i] = NULL;					//and setting pointer to NULL
	}
	
	delete[]  bookings;
	
}


void Flights::add_passenger( Application& A){
	
	if(A.is_luxury_class() == true ){
		
		if(bookedB >= maxB ){
			
			cout << "Luxury class seats are full, applicant cant enter \n";
			return;
			
		}
		else{
			
			int i=0;
			
			//while( bookings[i] != NULL ) i++;			// searching for null ptr
			
			for(i=0;i<maxA+maxB;i++) {					//searching for a NULL ptr
				
				if(bookings[i] == NULL) break;
			}
			
			
		//	Application B ;								//creating an object, using default constructor
			
		//	B = A;										//calling (user-defined) assignment operator =
			
			bookings[i] = &A;							// booking[i] points to  application A
			
			bookedA++;
			
			//remember to assign deque[i]=NULL if he was waiting , else nothing
			// and at the end of main deallocate memory  of all deque members
			
		}
		
	}else{
		
		
		if(bookedA >= maxA ){
			
			cout << "Economic class seats are full, applicant cant enter \n";
			
			
			return;
			
		} 
		else{
			int i=0;
			
			//while( bookings[i] != NULL ) i++;			// searching for null ptr
			
			
			for(i=0;i<maxA+maxB;i++) {					// searching for NULL ptr
				
				if(bookings[i] == NULL) break;
			}
			
			
			
			
		//	Application B ;								//creating an object, using default constructor
			
		//	B = A;										//calling (user-defined) assignment operator =
			
			bookings[i] = &A;							// booking[i] points to  application A
			
			bookedB++;
			
			//remember to assign deque[i]=NULL if he was waiting , else nothing
			// and at the end of main deallocate memory  of all deque members
			
		}
		
		
		
	}
	
	
	
}


void Flights::cancel_reservation(const string& s){
	
	for(int i=0; i < maxA+maxB ; i++){
		
		if(bookings[i] == NULL) continue;			//if seat is free => no reason to check
		
		if(bookings[i]->get_id() == s){
			
			if(bookings[i]->is_luxury_class() == true) bookedB--;				//reducing the count
			else{ bookedA--;}
			
			delete bookings[i];				// calling the destructor of Application
			bookings[i] = NULL;
			
			cout<< "Found the application " << bookings[i]->get_id() << "in a Flight and now deleting it \n";
			
		}
		
	}	
}




string Flights::get_destination() const{				

		return Destination;
}


Application** Flights::get_bookings()const{


return this->bookings;

}


void Flights::printInfo(){
	
	cout << "\n\n\n\n";
	
	cout << "Destination : "<< Destination << endl;
	
	cout << "Departs at : "<< Departure<<endl;
	cout << "Length of the flight = " << Length<<endl;
	cout << "Arrival at destination : " << arrives_at()<<endl;
	
	cout<< "\n\n";
	
	cout << "Available seats = " << get_available()<<endl;
	cout << "Available Economic class seats = " << maxA-bookedA<<endl;
	cout << "Available Luxury class seats = " << maxB-bookedB<<endl;
	
	
	cout << "\n\n\n\n";
	
}


																		/* Airport */
									
Airport::Airport(const time_t& T1,const int& x){
	
	now_time = T1;									//setting values
	totalTerminals = x;
	
	this->failedApp =0;
	this->activeTerminals = 0;
	
	Terminals = new Flights*[totalTerminals];		//allocating memory for  array of pointers
	for(int i=0;i<totalTerminals;i++){				//and setting said pointers to NULL
		
		Terminals[i] = NULL;			
	}
														
														//what to do about vectors
		
}									
									
Airport::~Airport(){
	
	for(int i =0;i<totalTerminals;i++){			// deleting flights and setting pointers to NULL
	
	 delete Terminals[i];
	 Terminals[i]=NULL;
	}
	
	delete[] Terminals;					// deleting array of pointers
	
	
	for(int i =0;i< Queue.size() ; i++){
		
		delete Queue[i];
		Queue[i] = NULL;
	}
	
	//delete Queue;
}

/*
void Airport::add_application(ifstream& file1){
	
	
	Application *A = new Application(file1);				// creating the application
	
	int count=0;
	
	for(int i=0;i<totalTerminals;i++){											//checking if  application matches with any current flights
	
		if( Terminals[i]==NULL )	continue;
		if( A->matches(*Terminals[i]) == true ){
		 Terminals[i]->add_passenger(*A);
		 return;
		}
	}
								//none of the flights match the parameters of application
		
	cout<<"No matches for current application with id " << A->get_id() << "\n";
	cout<<"Adding it to the Queue (deque) \n";
	
    Queue.push_back(A);						

}
*/



void Airport::add_application(const string& ids= " ",const string& first= " ",const string& last= " ",const string& dest= " ",const time_t& T1=0,const time_t& T2=0, const char& c=' '){
	
	
	Application *A = new Application(ids,first,last,dest,T1,T2,c);				// creating the application
	
	
	for(int i=0;i<totalTerminals;i++){											//checking if  application matches with any current flights
	
		if( Terminals[i]==NULL )	continue;			
		if( A->matches(*Terminals[i]) == true ){
		 Terminals[i]->add_passenger(*A);
		 return;
		}
	}
			
	cout<<"No matches for current application with id " << A->get_id() << "\n";
	cout<<"Adding it to the Queue (deque) \n";
	
    Queue.push_back(A);						
	
}


void Airport::cancel_application(const string& s){
	
	for(int i=0;i<Queue.size();i++){		// check if application is in Queue
		
		if(Queue[i] == NULL) continue;
		if(Queue[i]->get_id() == s){
				
			
		 	this->failedApp++;
		 	cout<< "Found the application "<< Queue[i]->get_id() <<" in Queue and deleted it \n";
		 	
			//delete Queue[i];
			Queue[i] = NULL;
			 
			 return;
		}		
	}
	
	
	
	for(int i=0;i<totalTerminals;i++){			//check if application is in a Flight
		
		if(Terminals[i]==NULL) continue;
		
		Terminals[i]->cancel_reservation(s);
	}
	
failedApp++;	
	
			
}


int Airport::add_flight(const string& s,const time_t& t1,const time_t& t2,const int& A,const int& B){
	
	int i=0; 
	
	//Flights *F = new Flights(s,t1,t2,A,B);
	
	if(activeTerminals == totalTerminals){
		
		cout<< "No available Terminal \n ";
		return -1;
		
	}
	
	
	Flights *F = new Flights(s,t1,t2,A,B);
	
	//while(Terminals[i] != NULL ) i++;			//find a free terminal
	
	for(i=0;i<totalTerminals;i++){
		
		if(Terminals[i] == NULL ){
			
			Terminals[i] = F;
			activeTerminals++;
			
			break;		
		}
			
	}
	
return i;	
	
	
}
/*
int Airport::add_flight(ifstream& file1){
	
	int i=0; 
	
	//Flights *F = new Flights(s,t1,t2,A,B);
	
	if(activeTerminals == totalTerminals){
		
		cout<< "No available Terminal \n ";
		return -1;
		
	}
	
	
	Flights *F = new Flights(file1);
	
	//while(Terminals[i] != NULL ) i++;			//find a free terminal
	
	for(i=0;i<totalTerminals;i++){
		
		if(Terminals[i] == NULL ){
			
			Terminals[i] = F;
			activeTerminals++;
			
			break;		
		}
			
	}
	
return i;	
	
	
}
*/

















void Airport::cancel_flight(const int& x){
	

	//for(int j=0;j<totalTerminals;j++){								//star
		
		if(Terminals[x] ==NULL){
			
			cout << "Terminal is free, no flight found \n";
			return;
		}
			
			int k=0,l=0;
			
			Terminals[x]->getMaxA_B(k,l);
			
			
			for(int i=0;i < k+l ;i++ ){													//	maxA = k, maxB = l;
				
				if(Terminals[x]->get_bookings()[i] == NULL) continue;				//searching for booked seats
				
				Queue.push_front(Terminals[x]->get_bookings()[i]);					// putting Airport.bookings[i]  in front of the Queue
				
				Terminals[x]->get_bookings()[i] = NULL;								// setting bookings[i] = NULL  to avoid memory leaks later
				
			}
			
delete Terminals[x];			// deleting the flight			
}



int Airport::failed_applications(){
	
	return this->failedApp;
	
}

void Airport::show_timetable(){
	
	if(this->activeTerminals == 0) {
		
		cout << "Nothing to show , no flights\n"; 
		return;
	}
	
	
	
	
					/* Sorting */
	
	
	
	Flights *temp1[totalTerminals];
	Flights	*temp2[totalTerminals];
	
	for(int i=0; i<totalTerminals ; i++) temp1[i] = Terminals[i];		//initialising temp1 array
	for(int i=0; i<totalTerminals ; i++) temp2[i] = NULL;				//initialising temp2 array

	
	
	for(int j=0;j<totalTerminals;j++){
	
		int count=0;
		
		int min_index=0;
		
		while(temp1[min_index] == NULL ) min_index++;		// find a random element that isnt NULL, and set it as minimum
	
	
	
				/* Sorting-Comparing */
				
		for(int i=0;i<totalTerminals;i++){									//seach to find the flight that leaves sooner than minimum
		
			if(temp1[i] == NULL) continue;
		
			if( temp1[i]->departs_at() < temp1[min_index]->departs_at() ) min_index = i;			// compare departure times
		}
	
	
	
	
	temp2[j]= temp1[min_index];			// temp2 is temp1 sorted by departure time , soon -> later 
										
	
	
	temp1[min_index] = NULL;			// to avoid putting the same min value in the whole array
	
		
		for(int k=0;k<totalTerminals;k++){			//check if all of temp elements are NULL
			
			if(temp1[k] == NULL) count++;	
		}
		
	
	if(count == totalTerminals) break; 			//if true then exit
	
	}									// after this procedure , temp1 members are all null




						/* Printing */

cout << "\nTimetable : \n";
	if(this->activeTerminals == 0 ){
		
		cout << "All the planes have left , nothing to show  \n";
		return;
		
	}
	
	
	
	for(int i=0;i<totalTerminals;i++){			//now to finally print timetable
		
		if(temp2[i] == NULL) continue;				// stop when u encounter NULL, since temp2 elements from 0 til k(random number) are all !=NULL 
		
		temp2[i]->printInfo();
		
		temp2[i] = NULL;
			
	}
	
	// now both  arrays' elements are NULL
		
}


void Airport::show_people_waiting(){
	
	cout <<  "\n\n\n";
	
	cout << "People waiting: \n";
	
	int count = 0;
	
	for(int i=0;i < Queue.size(); i++){
		
		if(Queue[i]==NULL) continue;
				
		cout << Queue[i]->get_name() <<endl;
		count++;
	}
	
	if(count == 0) cout << "~EMPTY~";				// no one is waitng
	
	cout<<"\n\n\n";
	
	
}


void Airport::flow_time(const time_t& T1){
	
	now_time+= T1;
	
								/* Terminals' change */
	
	
	cout<< "\n\n\n";
	
	for(int i=0;i<totalTerminals;i++){				//check for any change in the terminals
		
		if(Terminals[i] == NULL) continue;						//Terminal is empty , nothing changes
		
		if(now_time >= Terminals[i]->departs_at() ){			//departure time! , time to go 
		
		 	cout << "Flight with Destination : " << Terminals[i]->get_destination() << " and passengers : \n";
		 
		 	int x=0,y=0;
		 	Terminals[i]->getMaxA_B(x,y);
		 	
		 	for(int j=0;j<x+y;j++){											// printing the passengers			
			 	
			 	if(Terminals[i]->get_bookings()[j] == NULL) continue;
				 
				 cout << Terminals[i]->get_bookings()[j]->get_name() << endl;
		
			}
			cout << " is leaving, destroying  plane (freeing terminal) ... \n";
			
			delete Terminals[i];
			Terminals[i] = NULL;
			
			this->activeTerminals--;
			
			cout << "\n\n\n\n";
		}
	}
	
	
							/* Queue(Applications) change */
							
							
for(int i=0;i<Queue.size();i++){


	if(Queue[i] == NULL) continue;
	
	if(Queue[i]->arrived_by() <= now_time){ 						// cancelling
		
		string s = Queue[i]->get_id();
		
		this->cancel_application(s);
		
	}
	
}
	
}



