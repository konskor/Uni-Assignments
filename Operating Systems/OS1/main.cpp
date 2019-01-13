//#include <iostream>
#include <string.h>

#include <stdio.h>
#include <cstdlib>

#include "MyClasses.hpp"

using namespace std;

int main(int argc, char* argv[] ){

	bool input = false ;
	int ipos = 0;						//position of  -i flag

	bool output = false ;
	int opos = 0;						// position of -o flag

	int i = 0; 

	for( i = 0; i < argc; i++){

		if( strcmp(argv[i] , "-i") == 0 ){

			 input = true;
			 ipos = i+1;

		}

		if( strcmp(argv[i], "-o")  == 0 ){

			 output = true;
			 opos = i+1;

		}
	}

	Graph* G = new Graph();					// Creating a new empty Graph

	size_t buffersize=32;
	char* buffer= new char[buffersize];

	if ( input == true ){

		FILE* pf;

		/* Open and read the file line by line , and insert the edges to the Graph ( if the Vertices dont exist create them) */

		printf( " %s \n",argv[ipos] );

		pf = fopen( argv[ipos] , "r" );

		if (pf == NULL){

			printf("Error opening file\n");
			return -1;
		}


		char* source = NULL;
		char* dest = NULL;

		char* w = NULL;
		int weight = 0;


		while( getline(&buffer,&buffersize, pf) != EOF ){				

			source = strtok(buffer," ,\t");
			dest  =  strtok( NULL, " ,\t");
			w = strtok( NULL, " ,\t");

			weight = atoi(w);

			G->InsertEdge(source,dest,weight);					// add Edge,and if the Vertices dont exist create them

			source = NULL;				// just in case
			dest = NULL;
			w = NULL;
			weight = 0;

		}

		/* Closing the file and freeing up memory */

		fclose(pf);				// close file

	}

	/* From here on out, the user will input the commands */

	char* instruction = NULL;


	do{

		printf("Enter your command:");
		getline( &buffer, &buffersize, stdin );

		if( buffer[buffersize - 1] == '\n'){

			buffer[buffersize - 1] = '\0';
			buffersize--;
		}

		printf("\n");

		instruction = strtok(buffer," ,\t,\n");

		if( strcmp(instruction, "i") == 0 ){		// Insert Vertex

			char* vertex = strtok(NULL," ,\t,\n");

			if ( vertex != NULL ){

				G->InsertVertex(vertex);

				vertex = NULL;

			}
			else{

				printf(" A problem has occured in the InsertVertex bracket, incorrect input\n");

				//printf("Incorrect Input \n");

				return -1;

			}

		}


		if( strcmp(instruction, "n") == 0 ){		// Insert Edge

			char* v1 = strtok( NULL," ,\t,\n");
			char* v2 = strtok( NULL, " ,\t,\n");
			char* w = strtok( NULL, " ,\t,\n");

			if ( (v1 != NULL) && ( v2 != NULL) && ( w != NULL) ){

				int weight = atoi(w);

				G->InsertEdge( v1 , v2, weight);

				v1 = NULL;
				v2 = NULL;
				weight = 0;

			}
			else{

				printf(" A problem has occured in the InsertEdge bracket, incorrect input\n");
				return -1;

			}
		}


		if( strcmp(instruction, "d") == 0 ){		// DeleteVertex

			char* vertex = strtok( NULL," ,\t.\n");

			if ( vertex != NULL ){

				G->DeleteVertex( vertex );

				vertex = NULL;
			}
			else{

				printf(" A problem has occured in the DeleteVertex bracket, incorrect input\n");
				return -1;

			}
		}


		if( strcmp(instruction, "l") == 0 ){		// DeleteEdge or DeleteEdges

			char* v1 = strtok( NULL," ,\t,\n");
			char* v2 = strtok( NULL, " ,\t,\n");
			char* w = strtok( NULL, " ,\t,\n");


			if ( (v1 != NULL ) && (v2 != NULL) ) {

				if( w != NULL){				//DeleteEdge

					int weight = atoi(w);

					G->DeleteEdge( v1, v2 , weight);

				}
				else{

				G->DeleteEdges( v1, v2);

				}

				v1 = NULL;
				v2 = NULL;
				w = NULL;


			}
			else{

				printf(" A problem has occured in the DeleteEdges bracket, incorrect input\n");
				return -1;

			}
		}

		if( strcmp(instruction, "m") == 0 ){		// Mod EdgeWeight

			char* v1 = strtok( NULL," ,\t,\n");
			char* v2 = strtok( NULL, " ,\t,\n");
			char* w = strtok( NULL, " ,\t,\n");
			char* nw = strtok( NULL, " ,\t,\n");

			if ( (v1 != NULL)  && (v2 != NULL ) && (w != NULL) && ( nw != NULL)){

				int weight = atoi(w);
				int nweight = atoi(nw);

				G->ModEdgeW( v1, v2, weight, nweight );

				v1 = NULL;
				v2 = NULL;
				w = NULL;
				nw = NULL;

			}
			else{

				printf(" A problem has occured in the ModEdgeWeight bracket, incorrect input\n");
				return -1;

			}
		}


		if( strcmp(instruction, "r") == 0 ){			//Receiving

			char* vertex = strtok( NULL," ,\t,\n");

			if( vertex != NULL){

				G->Receiving(vertex);

				vertex = NULL;
			}
			else{

				printf(" A problem has occured in the Receiving bracket, incorrect input\n");
				return -1;

			}
		}


		if( strcmp(instruction, "c") == 0 ){			//CircleFind

			char* vertex = strtok( NULL," ,\t,\n");

			if( vertex != NULL){

				G->circlefind(vertex);

				vertex = NULL;
			}
			else{

				printf(" A problem has occured in the circlefind bracket, incorrect input\n");
				return -1;

			}
		}




		if( strcmp(instruction, "f") == 0 ){			// FindCircles

			char* vertex = strtok( NULL," ,\t,\n");

			char* ck = strtok( NULL, " ,\t,\n");				// char* k

			if( ( vertex != NULL) && ( ck != NULL) ){

				int k = atoi(ck);

				G->findcircles(vertex, k);

				vertex = NULL;
				ck = NULL;
			}
			else{

				printf(" A problem has occured in the findcircles bracket, incorrect input\n");
				return -1;

			}
		}



		if( strcmp(instruction, "t") == 0 ){			// FindCircles

			char* start = strtok( NULL," ,\t,\n");
			char* dest = strtok( NULL," ,\t,\n");

			char* cl = strtok( NULL, " ,\t,\n");


			if( (start != NULL) && (dest != NULL) && (cl != NULL) ){

				int l = atoi(cl);

				G->traceflow( start, dest , l );

				start = NULL;
				dest = NULL;
				cl = NULL;

			}
			else{

				printf(" A problem has occured in the traceflow bracket, incorrect input\n");
				return -1;

			}

			if ( strcmp(instruction,"e") == 0 ) break;



		}

	}while( strcmp( instruction ,"e") != 0 );

/* Exit() : */

	cout << " exit program" << endl;



	if ( output  == true){

		FILE* pf;

		pf = fopen( argv[opos] , "w");

		if (pf == NULL){

			printf("Error opening file\n");
			return -1;
		}

		VNode* vh = G->getLV()->getHead();

		ENode* eh = NULL;

		while ( vh != NULL){

			fprintf(pf, "|%s|\n", vh->getVertex()->getName() );


			eh = vh->getVertex()->getOut()->getHead();

			while( eh != NULL){

				fprintf(pf, "\t\t");

				fprintf( pf ," %d -> |%s| \n",  eh->getEdge()->getWeight(), eh->getEdge()->getDest()->getName() );

				eh = eh->getNext();
			}

		vh = vh->getNext();

		}
	}


/* freeing up memory */


	delete[] buffer;				// deleting buffer
	buffer = NULL;

	delete G;						//delete graph , chain calling destructors  : Graph -> VList ->VNode ->Vertex -> EList (Inc and Out) -> ENode -> Edge 
	G = NULL; 
}
























