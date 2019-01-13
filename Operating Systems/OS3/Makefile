# This makefile is based on the makefile provided by the instructor (adelis)

OBJS 	= myport.o, port-master.o, vessel.o, monitor.o, PriorityList.o, 
SOURCE	= myport.cpp, port-master.cpp, vessel.cpp, monitor.cpp, PriorityList.cpp
HEADER  = MyStructs.hpp, PriorityList.hpp
OUT  	= myport , port-master, vessel, monitor
CC	= g++
FLAGS   = -g -c -lpthread
 
# -g option enables debugging mode 
# -c flag generates object code for separate files

all: myport port-master vessel monitor

myport: myport.o 
	$(CC) -g  myport.o -o myport -lpthread

port-master: port-master.o PriorityList.o
	$(CC) -g  port-master.o PriorityList.o -o port-master -lpthread

vessel: vessel.o 
	$(CC) -g vessel.o -o vessel -lpthread

monitor: monitor.o
	$(CC) -g monitor.o -o monitor -lpthread

# create/compile the individual files >>separately<< 

PriorityList.o: PriorityList.cpp
	$(CC) $(FLAGS) PriorityList.cpp

myport.o: myport.cpp
	$(CC) $(FLAGS) myport.cpp

port-master.o: port-master.cpp
	$(CC) $(FLAGS) port-master.cpp

vessel.o: vessel.cpp
	$(CC) $(FLAGS) vessel.cpp

monitor.o: monitor.cpp
	$(CC) $(FLAGS) monitor.cpp


# clean house
clean:
	rm -f $(OBJS) $(OUT)

# do a bit of accounting
count:
	wc $(SOURCE) $(HEADER)
