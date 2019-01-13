# This makefile is based on the makefile provided by the instructor (adelis)

OBJS 	= mirr.o, Inode.o, NameTree.o
SOURCE	= mirr.cpp, Inode.cpp, NameTree.cpp
HEADER  = Mystructs.hpp, Inode.hpp, NameTree.hpp
OUT  	= mirr
CC	= g++
FLAGS   = -g -c
 
# -g option enables debugging mode 
# -c flag generates object code for separate files

all: mirr

mirr: mirr.o Inode.o NameTree.o
	$(CC) -g mirr.o Inode.o NameTree.o -o mirr

# create/compile the individual files >>separately<< 

INode.o: Inode.cpp NameTree.cpp
	$(CC) $(FLAGS) Inode.cpp

mirr.o: mirr.cpp  Inode.cpp NameTree.cpp
	$(CC) $(FLAGS) mirr.cpp

NameTree.o: NameTree.cpp Inode.cpp NameTree.cpp
	$(CC) $(FLAGS) NameTree.cpp


# clean house
clean:
	rm -f $(OBJS) $(OUT)

# do a bit of accounting
count:
	wc $(SOURCE) $(HEADER)
