#include <iostream>
#include <cstdlib>
#include <ctime>

#include "classes.h"



using namespace std;



int main(int argc,char *argv[]){

srand(time(NULL));

int N;		
	
if(argc == 1) {
	
	cout << "Please insert value N\n";	
	return -1;	
}

if(argc >1){

	N=atoi(argv[1]);
	cout << "The amount of stations are:  "<< N << endl;	
}


Train metro(N,3);


while(metro.operate() != false){

	int i=0;
	
	
	
	for(i=0;i<metro.get_totalW();i++){			//inStation
	
		metro.get_array()[i]->inStation();
		

	}
	 
	for(i=0;i<metro.get_totalW();i++){		//betweenStation
		
		metro.get_array()[i]->betweenStations();	
	
	}

	metro.reduce_station();				//one down, more to go
		
}


metro.printStatistics();

return 0;

}












