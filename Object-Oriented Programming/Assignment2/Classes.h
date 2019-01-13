#include <iostream>
#include <string>
#include <deque>

using namespace std;

class Flights;


class Application{

	
	
	string id;
	string First_Name;
	string Last_Name;
	string Destination;
	time_t	ETA_Airport;						//time to get to airport
	time_t	Destination_time;					//intended time to reach destination
	char	Class_seat;							// Economic A or Luxury B
	
public: 
		Application(const string&,const string&,const string&,const string&,const time_t&,const time_t&,const char&);
		Application(ifstream&);																							//reference to a file name
			
	   ~Application();
	   
	   bool matches(const Flights&A) const;			//if application and flight match
	   bool is_luxury_class() const;
	   
	   string get_id() const;
	   string get_name() const;
	   	
	   time_t arrived_by() const;
	   time_t get_ETA()const{return ETA_Airport;};
	   
	   Application& operator=(const Application& a);
				
};

class Flights{
	
	string Destination;
	time_t   Departure;
	time_t   Length;
	
	
	
	
	
	int maxA;					//Economic max Capacity
	int bookedA;
	
	int maxB;					//Luxury max capacity
	int bookedB;			 
	
	Application **bookings;					//List of bookings

public:
	
	Flights(const string&,const time_t&,const time_t&,const int&,const int&);
	Flights(ifstream&);
	
   ~Flights();

    void add_passenger(Application&);				
    void cancel_reservation(const string&);					// string = id of passenger

	time_t departs_at() const {return Departure;};
	time_t arrives_at() const {return Departure+Length;};
	
	string get_destination() const;
	int get_available() const { return (maxA-bookedA)+(maxB-bookedB); };
	
	Application** get_bookings() const;
	
	
	void getMaxA_B(int& a,int& b) const{a = maxA; b=maxB;}  
		
	void printInfo();
	
	
};


class Airport{
	
	time_t  now_time;
	
	int totalTerminals;
	int activeTerminals;
	Flights** Terminals;
	
	deque <Application* > Queue;
	
	int failedApp;
	
public:

		Airport(const time_t&,const int&);
	   
	   ~Airport();
	   
	   time_t get_time() const {return now_time; };							//get current time;
	   	
	   	void add_application(const string&,const string&,const string&,const string&,const time_t&,const time_t&,const char&);
	    void add_application(ifstream&);
		
		void cancel_application(const string&);
	   	
	   int add_flight(const string&,const time_t&,const time_t&,const int&,const int&);
	   int add_flight(ifstream& file1);
	   
	   void cancel_flight(const int&);
	   
	   int failed_applications();
	   
	   void show_timetable();
	   void show_people_waiting();

	   void flow_time(const time_t &);
	
};

