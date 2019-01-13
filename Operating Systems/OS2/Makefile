# This makefile is based on the makefile provided by the instructor (adelis)

OBJS 	=  myfind.o, splitter.o, leaf.o, Functions.o
SOURCE	= myfind.cpp, splitter.cpp, leaf.cpp, Functions.cpp
HEADER  = MyStruct.hpp
OUT  	= myfind , splitter, leaf
CC	= g++
FLAGS   = -g -c
 
# -g option enables debugging mode 
# -c flag generates object code for separate files

all: myfind splitter leaf

myfind: myfind.o Functions.o
	$(CC) -g myfind.o Functions.o -o myfind

splitter: splitter.o Functions.o
	$(CC) -g splitter.o Functions.o -o splitter

leaf: leaf.o Functions.o
	$(CC) -g leaf.o Functions.o -o leaf

# create/compile the individual files >>separately<< 

Function.o: Functions.cpp
	$(CC) $(FLAGS) Functions.cpp

myfind.o: myfind.cpp
	$(CC) $(FLAGS) myfind.cpp

splitter.o: splitter.cpp
	$(CC) $(FLAGS) splitter.cpp

leaf.o: leaf.cpp
	$(CC) $(FLAGS) leaf.cpp


# clean house
clean:
	rm -f $(OBJS) $(OUT)

# do a bit of accounting
count:
	wc $(SOURCE) $(HEADER)
