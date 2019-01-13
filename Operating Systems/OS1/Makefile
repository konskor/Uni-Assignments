
# This makefile is based on the makefile provided by the instructor (adelis)

OBJS 	= main.o Vertex.o VNode.o VList.o Edge.o ENode.o EList.o Graph.o 
SOURCE	= main.cpp Vertex.cpp VNode.cpp VList.cpp Edge.cpp ENode.cpp EList.cpp Graph.cpp
HEADER  = MyClasses.hpp
OUT  	= mygraph
CC	= g++
FLAGS   = -g -c
 
# -g option enables debugging mode 
# -c flag generates object code for separate files

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT)

# create/compile the individual files >>separately<< 

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp

Vertex.o: Vertex.cpp 
	$(CC) $(FLAGS) Vertex.cpp

VNode.o: VNode.cpp 
	$(CC) $(FLAGS) VNode.cpp

VList.o: VList.cpp 
	$(CC) $(FLAGS) VList.cpp

Edge.o: Edge.cpp 
	$(CC) $(FLAGS) Edge.cpp

ENode.o: ENode.cpp 
	$(CC) $(FLAGS) ENode.cpp

EList.o: EList.cpp 
	$(CC) $(FLAGS) EList.cpp

Graph.o: Graph.cpp 
	$(CC) $(FLAGS) Graph.cpp

# clean house
clean:
	rm -f $(OBJS) $(OUT)

# do a bit of accounting
count:
	wc $(SOURCE) $(HEADER)
