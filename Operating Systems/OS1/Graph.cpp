#include <iostream>
#include <string.h>

#include "MyClasses.hpp"

using namespace std;




	Graph::Graph(){

		LV = new VList();

		cout << "Created an empty Graph" << endl;

}

///////////////////////////////////////////////////////////

	Graph::~Graph() { 


		delete LV;								// this will start a chain reaction
		LV = NULL;								// it will start calling the destructors, calling each destructor downwards,
												// when we hit botom, starts deleitng upwards

//  Ex:  Calls :  LV->(each) VNode->Vertex 
//		 Deleting: Vertex ->(each) VNode -> LV

		cout << "Graph deleted" << endl;

	}

///////////////////////////////////////////////////////////

	VList* Graph::getLV() const { return LV; } 					// Get LV pointer


/////////////////////////////////////////////////////////

	void Graph::InsertVertex(char* s){

		VNode* V1 = NULL;


		LV->findVNode(s,&V1);				// at the end V1 = vertex with name s, or V1 = NULL if the vertex doesnt exist

		if( V1 != NULL){

			cout << "Node |"<< s <<"| exists;" << endl;
			return;

		}

		// V1 is NULL right now

		V1 = new VNode(s);				// creating a new VNode


		LV->addVNode(V1);
		V1 = NULL;



		cout << "Inserted |" << s <<"|";
		cout << endl;

	}


/////////////////////////////////////////////////////


	void Graph::InsertEdge(char* s1, char* s2, int w){


		VNode* V1 = NULL;
		LV->findVNode(s1,&V1);

		if( V1 == NULL){

			V1 = new VNode(s1);						// V1 with name s1, doesnt exist , so we create it
			LV->addVNode(V1);

			cout << "Inserted |" << s1 <<"|"<< endl;


		}


		VNode *V2 = NULL;
		LV->findVNode(s2,&V2);

		if( V2 == NULL){

			V2 = new VNode(s2);						// V2 with name s2, doesnt exist , so we create it
			LV->addVNode(V2);						//add VNode to the list of V

			cout << "Inserted |" << s2 <<"|" << endl;

		}

		/* We need to add the edge , in the INCOMING EList of V2 and the OUTGOING EList of V1 */

		Edge* temp = new Edge(V1->getVertex() ,V2->getVertex() ,w);


		V1->getVertex()->getOut()->addENode(temp);			// getOut() returns a pointer to an EList , which has pointers of all the outgoing Edges

		V2->getVertex()->getInc()->addENode(temp);			// getInc() returns a pointer to an EList , which has pointers to all the incoming Edges




		cout << "Inserted	|"<< V1->getVertex()->getName() <<"| -> "<< w <<" -> |"<< V2->getVertex()->getName() <<"|"<< endl;


		V1 = NULL;
		V2 = NULL;			//We dont need V1 and V2 anymore (no need to delete anything)

	}


////////////////////////////////////////////////////


	void Graph::DeleteVertex(char* s){

		VNode* V1 = NULL;

		LV->findVNode(s,&V1);

		if( V1 == NULL){

			cout << " Node |" << s <<"| doesn't exist	abort-d"<< endl;
			return;
		}

		/* Updating pointers of other lists before deleting */

		if( V1->getVertex()->getInc()->getHead() != NULL){



			V1->getVertex()->getInc()->prepDelete( V1->getVertex() );											// go to the OutgoingLists of the other nodes, and delete the ENodes 
																												// which point to Edges, that have source = V1
		}




		if( V1->getVertex()->getOut()->getHead() != NULL){



			V1->getVertex()->getOut()->prepDelete( V1->getVertex() );											// go to the IncomingLists of the other nodes, and delete the ENodes 
																												// which point to Edges, that have dest = V1
		}



		/* Deleting the node now */

		LV->deleteVNode(V1);				//calls the destructor of V1-> destructor of EList* Inc and Elist* Out 

		cout <<"Deleted |"<< s <<"|"<<endl;


		V1 = NULL;


	}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		void Graph::DeleteEdge(char *s1,char *s2, int w){

		VNode* V1 = NULL;
		LV->findVNode(s1,&V1);					// V1 will point to the VNode(if it exists) , or NULL


		if( V1 == NULL){													//V1 with name s1 , doesnt exist 

			cout << "|"<< s1 << "| does not exist - abort-l;" << endl;
			return;
		}


		VNode* V2 = NULL;
		LV->findVNode(s2,&V2);


		if( V2 == NULL){													//V2 with name s2 , doesnt exist 

			cout << "|"<< s2 << "| does not exist - abort-l;" << endl;
			return;
		}

		ENode* pe = NULL;
		V1->getVertex()->getOut()->findENode(V1,V2,w,&pe);								// pe will point to the ENode(if it exists) , or NULL

		if( pe == NULL){

			cout <<"|"<< s1 << "|->"<< w << "->|"<< s2 << "| does not exist - abort-l;" << endl;
			return;
		}


		/* First we have to  remove the Incoming ENode from the list (of V2) */

		V2->getVertex()->getInc()->updateEList( pe->getEdge() );						//find the ENode of V2 which points to this edge, NULL the Edge pointer, and remove it from List
																						//this way we wont delete the edge (yet)


		/* Now we delete the EdgeNode */


		cout << "Del-vertex |" << pe->getEdge()->getSource()->getName() << "|-> " << pe->getEdge()->getWeight() << "-> |" << pe->getEdge()->getDest()->getName() << "|" << endl;

		V1->getVertex()->getOut()->deleteENode( pe->getEdge() );



		V1 = NULL;
		V2 = NULL;				// we dont want the vertices to be deleted (out of scope -> call destructor)

		//cout << "Del-vertex |" << s1 << "|-> " << w << "-> |" << s2 << "|" << endl;

	}


		void Graph::DeleteEdges(char *s1,char *s2){

		VNode* V1 = NULL;
		LV->findVNode(s1,&V1);					// V1 will point to the VNode(if it exists) , or NULL


		if( V1 == NULL){													//V1 with name s1 , doesnt exist 

			cout << "|"<< s1 << "| does not exist - abort-l;" << endl;
			return;
		}


		VNode* V2 = NULL;
		LV->findVNode(s2,&V2);


		if( V2 == NULL){													//V2 with name s2 , doesnt exist 

			cout << "|"<< s2 << "| does not exist - abort-l;" << endl;
			return;
		}

		/* First we need to update Inc EList and Out EList of V2 (delete the correspondin ENodes without deleting the edges ) */

		V1->getVertex()->getOut()->prepDelete1( V1->getVertex(), V2->getVertex() );			// All the outgoing ENodes->Edges that connect V1->..->v2 have been removed from the INC Elist of V2
		V1->getVertex()->getInc()->prepDelete1( V1->getVertex(), V2->getVertex() );			// All the inc ENodes->Edges that connect V1->..->v2 have been removed from the OUT Elist of V2


		/* Now we have to delete both the ENodes ( and the Edges) that connect V1 and V2 */
		/* Starting with Out EList */



		ENode* temp = V1->getVertex()->getOut()->getHead();
		ENode* temp1 = NULL;


		while( temp != NULL){

			if(temp->getEdge()->getDest() == V2->getVertex() ){			// find all the outgoing edges pointing to V2

				temp1 = temp;
				temp = temp->getNext();

		//		cout << "Del-vertex |" << temp1->getEdge()->getSource()->getName() << "|-> " << temp1->getEdge()->getWeight() << "-> |" << temp1->getEdge()->getDest()->getName() << "|" << endl;


				V1->getVertex()->getOut()->deleteENode(temp1->getEdge() );				// delete both the ENode and Edge

				temp1 = NULL;

				continue;
			}

			temp = temp->getNext();

		}		/* Deleted all the Outgoing edges pointing to V2 */

		/* Now for the Inc EList */

		temp = V1->getVertex()->getInc()->getHead();
		temp1 = NULL;


		while( temp != NULL){

			if(temp->getEdge()->getSource() == V2->getVertex() ){			// find all the incoming edges from V2

				temp1 = temp;
				temp = temp->getNext();

			//	cout << "Del-vertex |" << temp1->getEdge()->getSource()->getName() << "|-> " << temp1->getEdge()->getWeight() << "-> |" << temp1->getEdge()->getDest()->getName() << "|" << endl;


				V1->getVertex()->getInc()->deleteENode(temp1->getEdge() );				// delete both the ENode and Edge

				temp1 = NULL;

				continue;
			}

			temp = temp->getNext();

		}		/* Deleted all the Incoming edges from V2 */

		cout << " All edges between " << s1 << " and " << s2 << " have been deleted " << endl;


		V1 = NULL;
		V2 = NULL;				// we dont want the vertices to be deleted (out of scope -> call destructor)

	}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	void Graph::ModEdgeW(char *s1, char *s2,int w, int nw){

		VNode* V1 = NULL;
		LV->findVNode(s1,&V1);					// V1 will point to the VNode(if it exists) , or NULL


		if( V1 == NULL){													//V1 with name s1 , doesnt exist 

			cout << "|"<< s1 << "| does not exist - abort-m;" << endl;
			return;
		}


		VNode* V2 = NULL;
		LV->findVNode(s2,&V2);


		if( V2 == NULL){													//V2 with name s2 , doesnt exist 

			cout << "|"<< s2 << "| does not exist - abort-m;" << endl;
			return;
		}

		ENode* pe = NULL;
		V1->getVertex()->getOut()->findENode(V1,V2,w,&pe);								// pe will point to the ENode(if it exists) , or NULL


		if( pe == NULL){

			cout <<"|"<< s1 << "|->"<< w <<"->|"<< s2 << "| does not exist - abort-m;" << endl;
			return;
		}


		pe->getEdge()->modWeight(nw);					// pe->getEdge() = Edge* ==> Edge->modWeight(nw) : replaces weight with new weight

		cout << "Mod-vertex |"<< s1 <<"|->"<< pe->getEdge()->getWeight() << "->|" << s2 << "|" << endl;

		pe = NULL;

	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void Graph::Receiving(char *s){

		VNode* V1 = NULL;
		LV->findVNode(s,&V1);					// V1 will point to the VNode(if it exists) , or NULL


		if( V1 == NULL){						//V1 with name s1 , doesnt exist 

			cout << "|"<< s << "| does not exist - abort-r;" << endl;
			return;
		}

		if( V1->getVertex()->getInc()->getHead() == NULL){

			cout<< "No-rec-edges " << s << endl;
			return;
		}


		cout << "Rec Edges	";
		V1->getVertex()->getInc()->Print();

	}



//////////////////////////////////////////////////////////////////////////////////////////////////////


	void Graph::DFS(Vertex* start, Vertex* current, VList** visited, EList** edges,int* count){				//DFS used in circlefind( NO duplicate edges and no duplicate vertices)

		if( (*visited)->Exist( current ) == true ){			// check if we have visited the vertex

			if( current == start){

				cout << "Cir-found	";
				(*edges)->Print1();
				cout<< endl;

				(*count)++;					// a counter to check how many circles

			}

			return;
		}

		/* we haven't visited this node */

		(*visited)->addVertex(current);					// current is now visited

		ENode* temp = current->getOut()->getHead();

		while (temp != NULL){							// check all the outgoing edges

			if( (*edges)->Exist( temp->getEdge() ) == false){					// we haven't passed through this edge, (we are forbidden to pass through the same edge twice)

				(*edges)->addENode( temp->getEdge() );					// add the edgeNode to the list

				DFS(start, temp->getEdge()->getDest(), visited, edges, count);


				(*edges)->updateEList(temp->getEdge() );					//Delete the ENode, not the edge

				/* Example:  In the given graph of our assignment, take only these 3 nodes and their edges : vdf, kji,kfc

							1)Starting DFS from |vdf|, we visit this node --> add it to the VList
							2) we add |vdf|->1200->|kji| to the EList 

							3) Recursive call of DFS ======> now we visit |kji| , adding it to the visited List
							4) Let's say the first edge encountered (and added to the EList ) is : |kji|->2200->|vdf|

							5) Recursive call of DFS ======> now we go to |vdf|
							6) We have already visited vdf and because vdf = start ==> Print the edge List

							7) Returning to |kji| , we remove the ENode pointing to |kji|->2200->|vdf| (without deleting the edge)  
							8) Go to the next edge (ENode)
							9) Repeat the process	
				*/

				/* tl;dr	1) We check if we have passed from the current vertex ( if they exist in the VList)

								We are not allowed to pass through the same vertex twice

							2) We check  if we have passed through the edge (if they exist in the EList)

								We are not allowed to pass through the same edge twice */


			}

			temp = temp->getNext();
		}

		(*visited)->updateVList(current);			// remove from the visited List

		return;
	}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void Graph::DFS1(Vertex* start, Vertex* current, EList** edges, int k, int* count){				//DFS used in findcircles (no duplicate edges, duplicate vertices accepted EXCEPT start node )
																									// VNode* start, only shows up in the beginning and end of the path

		if( (*edges)->getHead() != NULL ){				// if the edge list is empty, then there hasn't been path yet ( we are still at the start)

			if( current == start){

				cout << "Cir-found	";
				(*edges)->Print1();
				cout<< endl;

				(*count)++;					// a counter to check how many circles
				return;

			}

//			return;
		}

		ENode* temp = current->getOut()->getHead();

		while (temp != NULL){							// check all the outgoing edges


			if( temp->getEdge()->getWeight() < k) {					// ignore the edges who have weight < k

				temp = temp->getNext();
				continue;
			}



			if( (*edges)->Exist( temp->getEdge() ) == false){					// we haven't passed through this edge, (we are forbidden to pass through the same edge twice)

				(*edges)->addENode(temp->getEdge() );					// add the edgeNode to the list

				DFS1(start, temp->getEdge()->getDest(), edges, k, count);

				(*edges)->updateEList(temp->getEdge() );					//Delete the ENode, not the edge

				/* Example:  In the given graph of our assignment, take only these 3 nodes and their edges : vdf, kji,kfc

							1)Starting DFS from |vdf|, we check if the outgoing edges satisfy : weight >= k (else we ignore them)
							   for now lets say k = 0

							2) we add |vdf|->1200->|kji| to the EList  ( 1200 >0 )

							3) Recursive call of DFS ======> now we visit |kji|,
							4) Let's say the first edge encountered (and added to the EList ) is : |kji|->2200->|vdf| ( 2200 >0 )

							5) Recursive call of DFS ======> now we go to |vdf|
							6) vdf = start ==> Print the edge List

							7) Returning to |kji| , we remove the ENode pointing to |kji|->2200->|vdf| (without deleting the edge)  
							8) Go to the next edge (ENode)
							9) Repeat the process	
				*/

				/* tl;dr We check two things about the outgoing edges : a) weight >= k, b)if we have passed through them (if they exist in the EList)

								We are not allowed to pass through the same edge twice */

			}

			temp = temp->getNext();
		}

		return;
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void Graph::DFS2(Vertex* start, Vertex* dest, Vertex* current, EList** edges, int* l, int* count){				// check if right


		if( (*l) == 0 ){

			if( (*edges)->getHead() != NULL ){				// if the edge list is empty, then there hasn't been path yet ( we are still at the start)

				if( current == dest ){

					cout << "Tra-found	";
					(*edges)->Print1();
					cout << endl;

					(*count)++;

					return;

				}

/* If we also want to print the circles
				if( current == start ){

					cout << "Cir-found	";
					(*edges)->Print1();
					cout<< endl;

					return;

				}
*/

			return;
			}
		}

		ENode* temp = current->getOut()->getHead();

		while (temp != NULL){							// check all the outgoing edges


			if( (*edges)->Exist( temp->getEdge() ) == false){					// we haven't passed through this edge, (we are forbidden to pass through the same edge twice)


				(*edges)->addENode(temp->getEdge() );					// add the edgeNode to the list
				(*l)--;										//since we added an edge to our path , we have to reduce the l = max number of edges that we can traverse


				DFS2(start, dest, temp->getEdge()->getDest(), edges, l, count);

				(*edges)->updateEList(temp->getEdge() );					//Delete the ENode, not the edge

				(*l)++;														//since we removed an edge from the path, we increment l


				/* Example:  In the given graph of our assignment, take only these 3 nodes and their edges ( ignore edges that go to other vertices except these) : vdf, kji,kfc

							1)Starting DFS2 from |vdf|, 

							2) we add |vdf|->1200->|kji| to the EList  and reduce l (max number of edges to pass through)

							3) Recursive call of DFS2 ======> now we visit |kji|,
							4) Check if l = 0, and if it is  ==> we cant go anywhere else, so we check if the current vertex is destination or return to the previous
								For now lets say that l = 9999 (really big) , so we search the edges of |kji|

							5) Let's say the first edge encountered (and added to the EList ) is : |kji|->2200->|vdf|,    l--

							6) Recursive call of DFS2 ======> now we go to |vdf|
							7) l is != 0, but all the outgoing edges of |vdf| ( of our example ) have been passed

							8) Dead End, so we check if this node is the destination ==> Print EList 
							8a) if not then Return

							9) vdf = start ==> Print the edge List

							10) Returning to |kji| , we remove the ENode pointing to |kji|->2200->|vdf| (without deleting the edge)  
							11) Go to the next edge (ENode)
							12) Repeat the process	
				*/

				/* tl;dr We check one thing about the outgoing edges : if we have passed through them (if they exist in the EList)

								We are not allowed to pass through the same edge twice */

			}

			temp = temp->getNext();
		}

		/* At this point, current Node can't go anywhere else, ( passed through all the outgoing edges) 
			So we are currently in a dead end,and we check if this node is the destination, or we will return to the previous Vertex */



		if( current == dest ){

			cout << "Tra-found	";
			(*edges)->Print1();
			cout << endl;

			(*count)++;

			return;

		}

/*
		if( current == start ){

			cout << "Cir-found	";
			(*edges)->Print1();
			cout<< endl;

			return;

		}
*/

		return;
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	void Graph::circlefind(char* s){

		VNode* V1 = NULL;
		LV->findVNode(s,&V1);					// V1 will point to the VNode(if it exists) , or NULL


		if( V1 == NULL){						//V1 with name s1 , doesnt exist 

			cout << "|"<< s << "| does not exist - abort-c;" << endl;
			return;
		}

		VList*  vlist = new VList();
		EList*  elist = new EList();
		int count = 0;


		this->DFS(V1->getVertex() ,V1->getVertex() ,&vlist,&elist,&count);

		if( count == 0 ) cout << "No-circle-found |"<< s << "|" << endl;

		delete vlist;
		vlist = NULL;

		delete elist;
		elist = NULL;


	} 


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	void Graph::findcircles(char *s, int k){


		VNode* V1 = NULL;
		LV->findVNode(s,&V1);					// V1 will point to the VNode(if it exists) , or NULL


		if( V1 == NULL){						//V1 with name s1 , doesnt exist 

			cout << "|"<< s << "| does not exist - abort-f;" << endl;
			return;
		}

		EList*  elist = new EList();
		int count = 0;


		this->DFS1(V1->getVertex() ,V1->getVertex() ,&elist,k,&count);

		if( count == 0 ) cout << "No-circle-found involving |"<< s << "| " << k << endl;

		delete elist;
		elist = NULL;

	} 


//////////////////////////////////////////////////////////////////////////////////////////////////////////////


		void Graph::traceflow(char* s1, char* s2, int l){

		VNode* V1 = NULL;
		LV->findVNode(s1,&V1);					// V1 will point to the VNode(if it exists) , or NULL


		if( V1 == NULL){						//V1 with name s1 , doesnt exist 

			cout << "|"<< s1 << "| does not exist - abort-t;" << endl;
			return;
		}

		VNode* V2 = NULL;
		LV->findVNode(s2,&V2);					// V1 will point to the VNode(if it exists) , or NULL


		if( V2 == NULL){						//V1 with name s1 , doesnt exist 

			cout << "|"<< s2 << "| does not exist - abort-t;" << endl;
			return;
		}

		int count = 0;
		EList* elist = new EList();

		this->DFS2(V1->getVertex() ,V2->getVertex() ,V1->getVertex() ,&elist, &l, &count);

		if( count == 0 ) cout << "No-trace from |"<< s1 <<"| to |" << s2 << "|" << endl;

		delete elist;
		elist = NULL;

	}


////////////////////////////////////////////////////////////////////////////////////////////////////////














