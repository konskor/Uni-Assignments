OBJS 	= InfoItem.o LSH_HT.o Utilities.o Vector.o Vector_Node.o HyperCube.o Curve.o InfoCurve.o Curve_LSH.o Utilities2.o Relevant_Node.o Relevant_List.o Relevant_Table.o
SOURCE	= InfoItem.cpp LSH_HT.cpp Utilities.cpp Vector.cpp Vector_Node.cpp HyperCube.cpp Curve.cpp InfoCurve.cpp Curve_LSH.cpp Utilities2.cpp Relevant_Node.cpp Relevant_List.cpp Relevant_Table.cpp
HEADER  = InfoItem.hpp LSH_H.hpp Utilities.hpp Vector.hpp Vector_Node.hpp HyperCube.hpp Curve.hpp InfoCurve.hpp Curve_LSH.hpp Utilities2.hpp Relevant_Node.hpp Relevant_List.hpp Relevant_Table.hpp
OUT  	= lsh cube curve_grid_lsh
CC	= g++
FLAGS   = -g -c -Wall -std=c++11
 
# -g option enables debugging mode 
# -c flag generates object code for separate files

all: lsh cube curve_grid_lsh curve_grid_hypercube curve_projection_lsh curve_projection_hypercube


curve_projection_hypercube: $(OBJS) curve_projection_hypercube.o 
	$(CC) -g $(OBJS) curve_projection_hypercube.o -o curve_projection_hypercube

curve_projection_lsh: $(OBJS) curve_projection_lsh.o 
	$(CC) -g $(OBJS) curve_projection_lsh.o -o curve_projection_lsh


curve_grid_hypercube: $(OBJS) curve_grid_hypercube.o
	$(CC) -g $(OBJS) curve_grid_hypercube.o -o curve_grid_hypercube

curve_grid_lsh: $(OBJS) curve_grid_lsh.o 
	$(CC) -g $(OBJS) curve_grid_lsh.o -o curve_grid_lsh

cube: $(OBJS) HyperCube.o cube.o
	$(CC) -g $(OBJS) cube.o  -o cube

lsh: $(OBJS) lsh.o 
	$(CC) -g $(OBJS) lsh.o -o lsh

# create/compile the individual files >>separately<< 

curve_projection_hypercube.o: curve_projection_hypercube.cpp
	$(CC) $(FLAGS) curve_projection_hypercube.cpp

curve_projection_lsh.o: curve_projection_lsh.cpp
	$(CC) $(FLAGS) curve_projection_lsh.cpp

curve_grid_hypercube.o: curve_grid_hypercube.cpp
	$(CC) $(FLAGS) curve_grid_hypercube.cpp 

curve_grid_lsh.o: curve_grid_lsh.cpp
	$(CC) $(FLAGS) curve_grid_lsh.cpp


cube.o: cube.cpp
	$(CC) $(FLAGS) cube.cpp

lsh.o: lsh.cpp
	$(CC) $(FLAGS) lsh.cpp
	
		

InfoItem.o: InfoItem.cpp 
	$(CC) $(FLAGS) InfoItem.cpp
	
LSH_HT.o: LSH_HT.cpp 
	$(CC) $(FLAGS) LSH_HT.cpp	
	
Utilities.o: Utilities.cpp 
	$(CC) $(FLAGS) Utilities.cpp	

Vector.o: Vector.cpp 
	$(CC) $(FLAGS) Vector.cpp
	
Vector_Node.o: Vector_Node.cpp 
	$(CC) $(FLAGS) Vector_Node.cpp

HyperCube.o: HyperCube.cpp
	$(CC) $(FLAGS) HyperCube.cpp
	
Curve.o: Curve.cpp
	$(CC) $(FLAGS) Curve.cpp	

InfoCurve.o: InfoCurve.cpp
	$(CC) $(FLAGS) InfoCurve.cpp	
	
Curve_LSH.o: Curve_LSH.cpp
	$(CC) $(FLAGS) Curve_LSH.cpp
	
Utilities2.o: Utilities2.cpp
	$(CC) $(FLAGS) Utilities2.cpp	
	
	
Relevant_Table.o: Relevant_Table.cpp
	$(CC) $(FLAGS) Relevant_Table.cpp
	
Relevant_List.o: Relevant_List.cpp Relevant_Node.hpp
	$(CC) $(FLAGS) Relevant_List.cpp
	
Relevant_Node.o: Relevant_Node.cpp 
	$(CC) $(FLAGS) Relevant_Node.cpp	
	
			


# clean house
clean:
	rm -f $(OBJS) $(OUT)

# do a bit of accounting
count:
	wc $(SOURCE) $(HEADER)
