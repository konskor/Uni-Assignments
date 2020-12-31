OBJS 	= mirror_client.o  Functions.o sender.o receiver.o
SOURCE	= main.cpp Functions.cpp sender.cpp receiver.cpp
HEADER  = header.hpp
OUT  	= bitcoin
CC	= g++
FLAGS   = -g -c
 
# -g option enables debugging mode 
# -c flag generates object code for separate files

all: mirror_client sender receiver

mirror_client: mirror_client.o Functions.o
	$(CC) -g mirror_client.o Functions.o -o mirror_client

sender: sender.o Functions.o
	$(CC) -g sender.o Functions.o -o sender

receiver: receiver.o Functions.o
	$(CC) -g receiver.o Functions.o -o receiver



# create/compile the individual files >>separately<< 

mirror_client.o: mirror_client.cpp
	$(CC) $(FLAGS) mirror_client.cpp

Functions.o: Functions.cpp 
	$(CC) $(FLAGS) Functions.cpp

sender.o: sender.cpp 
	$(CC) $(FLAGS) sender.cpp
	
receiver.o: receiver.cpp 
	$(CC) $(FLAGS) receiver.cpp	



# clean house
clean:
	rm -f $(OBJS) $(OUT)

# do a bit of accounting
count:
	wc $(SOURCE) $(HEADER)
