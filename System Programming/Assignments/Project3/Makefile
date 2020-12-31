OBJS 	= dropbox_client.o, dropbox_server.o, Functions.o
SOURCE	= dropbox_client.cpp, dropbox_server.cpp, Functions.cpp
HEADER  = header.hpp
OUT  	= dropbox_server , dropbox_client
CC	= g++
FLAGS   = -g -c
 
# -g option enables debugging mode 
# -c flag generates object code for separate files

all: dropbox_server  dropbox_client

dropbox_server: dropbox_server.o Functions.o
	$(CC) -g dropbox_server.o Functions.o -o dropbox_server

dropbox_client: dropbox_client.o Functions.o
	$(CC) -g dropbox_client.o Functions.o -o dropbox_client -pthread

# create/compile the individual files >>separately<< 

Functions.o: Functions.cpp
	$(CC) $(FLAGS) Functions.cpp -pthread

dropbox_server.o: dropbox_server.cpp
	$(CC) $(FLAGS) dropbox_server.cpp

dropbox_client.o: dropbox_client.cpp
	$(CC) $(FLAGS) dropbox_client.cpp -pthread


# clean house
clean:
	rm -f $(OBJS) $(OUT)

# do a bit of accounting
count:
	wc $(SOURCE) $(HEADER)
