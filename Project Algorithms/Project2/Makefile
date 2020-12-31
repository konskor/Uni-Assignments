OBJS 	= Vector.o Item.o Curve.o Vector_Node.o Cluster.o LSH_HT.o Curve_LSH.o Initialization.o Assignment.o Update.o Evaluation.o Utilities.o Utilities2.o Utilities3.o 
SOURCE	= Vector.cpp Item.cpp Curve.cpp Vector_Node.cpp Cluster.cpp LSH_HT.cpp Curve_LSH.cpp Initialization.cpp Assignment.cpp Update.cpp Evaluation.cpp Utilities.cpp Utilities2.cpp Utilities3.cpp 
HEADER  = Vector.hpp Vector_Node.hpp Curve.hpp Item.hpp Cluster.hpp LSH_HT.hpp Curve_LSH.hpp Initialization.hpp Assignment.hpp Update.hpp Evaluation.hpp Utilities.hpp Utilities2.hpp Utilities3.hpp 
OUT  	= cluster
CC	= g++
FLAGS   = -g -c -Wall -std=c++11
 
# -g option enables debugging mode 
# -c flag generates object code for separate files

all: cluster 

cluster: $(OBJS) cluster.o 
	$(CC) -g $(OBJS) cluster.o -o cluster

# create/compile the individual files >>separately<< 


cluster.o: cluster.cpp
	$(CC) $(FLAGS) cluster.cpp
	
	

LSH_HT.o: LSH_HT.cpp 
	$(CC) $(FLAGS) LSH_HT.cpp	

Curve_LSH.o: Curve_LSH.cpp
	$(CC) $(FLAGS) Curve_LSH.cpp



Vector.o: Vector.cpp 
	$(CC) $(FLAGS) Vector.cpp
	
Vector_Node.o: Vector_Node.cpp 
	$(CC) $(FLAGS) Vector_Node.cpp

Curve.o: Curve.cpp
	$(CC) $(FLAGS) Curve.cpp	




Utilities.o: Utilities.cpp 
	$(CC) $(FLAGS) Utilities.cpp	

Utilities2.o: Utilities2.cpp
	$(CC) $(FLAGS) Utilities2.cpp	

Utilities3.o: Utilities3.cpp 
	$(CC) $(FLAGS) Utilities3.cpp	



Initialization.o: Initialization.cpp
	$(CC) $(FLAGS) Initialization.cpp	
	
Assignment.o: Assignment.cpp
	$(CC) $(FLAGS) Assignment.cpp	

Update.o: Update.cpp
	$(CC) $(FLAGS) Update.cpp	

Evaluation.o: Evaluation.cpp
	$(CC) $(FLAGS) Evaluation.cpp	





Item.o: Item.cpp
	$(CC) $(FLAGS) Item.cpp	

Cluster.o: Cluster.cpp
	$(CC) $(FLAGS) Cluster.cpp	


# clean house
clean:
	rm -f $(OBJS) $(OUT)

# do a bit of accounting
count:
	wc $(SOURCE) $(HEADER)
