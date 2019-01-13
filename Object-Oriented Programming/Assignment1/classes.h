class Person{
	
	int have_ticket;
	int have_half;
	int deserve_half;
	
public: 
	Person();
	~Person();
	   
	void get(int&i,int&j,int&k) const;		
	

};

typedef class Person person;





class Waggon{

int maxCapacity;
int pcount;

person **p_array;	

int totalTrespassers;

int trespassers_caught;
int trespassers_escaped;


bool inspection;

int money;
int  ticket_fine;
int	 reduced_ticket_fine;



public:
	
	Waggon(int,int,int);			//maxCapacity,ticketFine,reduced ticket_fine
   ~Waggon();
   
  void printStatistics();		
  
  void inStation();				
  void Entering();
  void Leaving();
  
  void betweenStations();		
  
  void get_money(int&) const;
};


typedef class Waggon waggon;

class Train{
	
int totalWaggons;
Waggon **w_array;

int money;
int stations;


public:

	Train(int,int);			// totalWaggons, N= stations
   ~Train();
   
	bool operate();					
	void  printStatistics();	
	
	Waggon** get_array();
	int get_totalW();
	void reduce_station();	 
};

typedef class Train train;

