#include <stdio.h>
#include <sys/types.h>

#include <semaphore.h>

typedef struct Application_form{


/* want to enter or exit */

	bool enter;		// if true => want to enter, if false => want to exit

	pid_t pid;
	char type[2];			// what type of vessel
//	long offset_type;



	bool PQ;		// come from a PriorityQueue or not
// no real use, just to check if to increment a certain counter while entering

/* enter == true (want to enter) */

	/* written by vessel */

	bool upgrade;
	char utype[2];
//	long offset_utype;



//	double parkperiod;


		/* For Statistics */
	double arrive_queue;			// when did it arrive in queue


	/* written by port-master */

	bool approval;
//	char position_type[2];		//after being approved
//	char postype[2];


	long offset;		//helps get to the specific position, given by portmaster

		/* For Statistics */
//	double waiting_time;		// how much did the vessel wait, to get approved
	// waiting_time = now - arrive_queue



/* enter == false (want to leave) */

	/* written by vessel */

	double departure;
	double cost;

//	long offset;

		/* For Statistics */

//	double arrive_queue;			// when did it arrive in queue


	/* written by port-master */

//	bool approval;				// can leave or not

		/* For Statistics */

	//double waiting_time;		// how much did the vessel wait, to get approved
	//waiting_time = now - arrive_queue



}App;


typedef struct Port_position{

/* these offsets  current - base , where base = port_position (not start of SHM) */


	/* position info */
	bool available;

	char postype[2];		// position type

//	long offset_postype;
	int counter;			// 1st , 2nd , etc  position of postype( example : 2 of M)


	/* vessel info */

	pid_t pid;

	char type[2];

//	long offset_type;

	double arrival_time;			//in port, start_time for cost
//	double parkperiod;


//	double cost;			// final cost

}Position;



typedef struct SHMemory{

	/* everyone can read/write in shared memory */
	/* public ledger only written by port-master,  and read by anyone else */

//	App application;// only 1 vessel writes in this "application form", decided by port-master 

	long offset_application;



//	sem_t moving;				// moving = 1,  port-master checks if there is movement in port 


	sem_t check;		// check = 0, wait for vessel/application to arrive,

//increments when a vessel/application arrives, and port-master chooses from which queue, an application/vessel is going to wake up ( generic or one of the priority queue)

	sem_t queue;		// queue = 0, queue of application,
						// blocks the  corresponding vessel ,
						// until port-master decides otherwise

//	sem_t write_app		// write_app = 1, only 1 writes application struct,
// used by port-master to check if there is already a vessel who has written its application
// block anyone else from writing in this "application form"


	sem_t full;		// full = 0, used to inform port-master that there is an application to read

	sem_t in_evaluation;		// in_evaluation = 0,block vessel
								// wait for the port-master to read the app and his decision

	sem_t maneuver_pm;// maneuver_pm = 0, port master waits untill vessel finishes maneuvering
// This way, we guarantee that only one vessel will be moving in port



	/* Readers/Writers in memory */

	sem_t mutex;				//mutual exclusion between processes
	sem_t rw_mutex;			// read_write mutex
	int readers_count;


/* Priority Queues = 7 */

	// if the vessel can't enter the port, put it into the appropriate priority Queue 
	// In order to have " FCFS & Available", we also create a local linked list in "port-master", that shows the order of the "priority applications " (more on that later) 

//	int pr_count;			//for each failed application => priority_queue_member_count++; 
// a counter for all PQs


	/* Non-upgradable ships */

	sem_t pr_Small;
	sem_t pr_Medium;
	sem_t pr_Large;

	/* upgradable ships */

	sem_t pr_uSM;		// Small ship, willing to upgrade to Medium
	sem_t pr_uSL;		// Small ship, willing to upgrade to Large
	sem_t pr_uML;		// Medium ship, willing to upgrade to Large


//	sem_t pr_exit;		// for vessels, that couldn't leave
//	the above can't happen, since we can guarantee when the time comes the vessel will exit


/* Variables */

//	bool movement;			// movement = false, if there is movement in port

	bool accept_apps;		// false if we don't want any other vessel to "come" to port
// we finish our current jobs then exit


	int num_applications_serviced; 		// 2 * vessels ( 1 for enter , 1 for exit )

	int counter;			// applications to be serviced, +2 each time a vessel approaches port(1 to enter and 1 to exit )
// used as a complementary(not important) variable, to stop port from running endlessly



		/* Money */
	double total_money;			// total amount of money gained by all categories
	int number_vessels;		// number of vessels that came to the port

	double total_money_S;		// total money gained from Position type Small
	int number_S;				// how many times, Small Positions were reserved 

	double total_money_M;		// total money gained from Position type Medium
	int number_M;				// how many times, Medium Positions were reserved

	double total_money_L;				// total money gained from Position type Large
	int number_L;				// how many times, Large Positions were reserved


		/* Time ( use gettimeofday() ) */

	double start;		// our starting point in time (initialise in myport )


	double total_waiting;			// total amount of waiting for all vessels
// will use number_vessels, average_waiting = waiting_time/ number_vessels

	double waiting_S;		// total waiting_time
	double waiting_M;
	double waiting_L;


		/* Cost for each position and the time_period */

	double time_period;		// for calculating cost => 5$ per 30 minutes

	double cost_S;
	double cost_M;
	double cost_L;




	/* Public Ledger Record */

//	char PL_name[20];					//public ledger  name
	long offset_PLname;

	/* Log */
//	char log_name[10];
	long offset_logname;


	long offset_Small;					//offset to list of Small_Nodes
	int S_available;
	int S_total;

	long offset_Medium;				//offset to list of Medium_Nodes
	int M_available;
	int M_total;

	long offset_Large;					//offset to list of Large_Nodes
	int L_available;
	int L_total;			// total amount of Large Positions in port



}SHM;



