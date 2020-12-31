
OBJS 	= main.o  Functions.o
SOURCE	= main.cpp Functions.cpp
HEADER  = header.hpp
OUT  	= bitcoin
CC	= g++
FLAGS   = -g -c
 
# -g option enables debugging mode 
# -c flag generates object code for separate files

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT)

# create/compile the individual files >>separately<< 

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp

Functions.o: Functions.cpp 
	$(CC) $(FLAGS) Functions.cpp

# clean house
clean:
	rm -f $(OBJS) $(OUT)

# do a bit of accounting
count:
	wc $(SOURCE) $(HEADER)
