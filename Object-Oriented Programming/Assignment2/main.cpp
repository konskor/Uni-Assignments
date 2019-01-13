#include <iostream>
#include <fstream>

#include <cstdlib>


#include "Classes.h"


using namespace std;



int main(int argc,char *argv[]){
	
	
	
if(argc<2){
	
	cout << "Not enough arguments\n";
	
	return -1;	
}	
	
time_t forward= atoi(argv[1]);	
time_t now = 0;	
	
Airport Freedom(now,3);	

int f1,f2,f3;	//position of lights in terminals


ifstream infile;


					/* 1st Flight */
string s1;
time_t T1,T2;
int num1,num2;




infile.open("Flight1.txt");

infile >> s1;

infile >> T1;
infile >> T2;

infile>> num1;
infile>> num2;

f1 = Freedom.add_flight(s1,T1,T2,num1,num2);
//f1 = Freedom.add_flight(infile)


infile.close();


					/* 2nd Flight */

infile.open("Flight2.txt");

infile >> s1;

infile >> T1;
infile >> T2;

infile>> num1;
infile>> num2;

f2 = Freedom.add_flight(s1,T1,T2,num1,num2);

//f2 = Freedom.add_flight(infile)


infile.close();				
						
						/*3rd Flight */
						
						

infile.open("Flight3.txt");

infile >> s1;

infile >> T1;
infile >> T2;

infile>> num1;
infile>> num2;


f3 = Freedom.add_flight(s1,T1,T2,num1,num2);

//f3 = Freedom.add_flight(infile)


infile.close();						



				/* 1st Application */

string str2,str3,str4;
char ch;
				

infile.open("App1.txt");

infile >> s1;
infile >> str2;
infile >> str3;
infile >> str4;

infile >> T1;
infile >> T2;

infile >> ch;

Freedom.add_application(s1,str2,str3,str4,T1,T2,ch);

//Freedom.add_application(infile);

infile.close();


				/* 2nd Application */
				

infile.open("App2.txt");

infile >> s1;
infile >> str2;
infile >> str3;
infile >> str4;

infile >> T1;
infile >> T2;

infile >> ch;

Freedom.add_application(s1,str2,str3,str4,T1,T2,ch);


//Freedom.add_application(infile);

infile.close();				
				
						/* 3rd Application */
				

infile.open("App3.txt");

infile >> s1;
infile >> str2;
infile >> str3;
infile >> str4;

infile >> T1;
infile >> T2;

infile >> ch;

Freedom.add_application(s1,str2,str3,str4,T1,T2,ch);


//Freedom.add_application(infile);

infile.close();				
	
					/* 4th Application */
				

infile.open("App4.txt");

infile >> s1;
infile >> str2;
infile >> str3;
infile >> str4;

infile >> T1;
infile >> T2;

infile >> ch;

Freedom.add_application(s1,str2,str3,str4,T1,T2,ch);



//Freedom.add_application(infile);

infile.close();				

						/* 5th Application */
				

infile.open("App5.txt");

infile >> s1;
infile >> str2;
infile >> str3;
infile >> str4;

infile >> T1;
infile >> T2;

infile >> ch;

Freedom.add_application(s1,str2,str3,str4,T1,T2,ch);



//Freedom.add_application(infile);

infile.close();				


									/* 6th Application */
				

infile.open("App6.txt");

infile >> s1;
infile >> str2;
infile >> str3;
infile >> str4;

infile >> T1;
infile >> T2;

infile >> ch;

Freedom.add_application(s1,str2,str3,str4,T1,T2,ch);

//Freedom.add_application(infile);

infile.close();


								/*Current Situation */

Freedom.show_timetable();
Freedom.show_people_waiting();


Freedom.flow_time(forward);

								/*Fast forwarding */
								
Freedom.show_timetable();
Freedom.show_people_waiting();


return 0;
}
























