#include <iostream>
#include <string.h>


class Vertex;
class VNode;

class Edge{

	Vertex* source;		// these two represent the direction of the Edge
	Vertex* dest;

	int weight;

	public:

		Edge(Vertex* s,Vertex* d,int w);

		~Edge();

////////////////////////////////////////////////////

		Vertex* getSource() const;				// accessors

		Vertex* getDest() const;

		int getWeight() const;

/////////////////////////////////////////////////

		void modWeight(int w);				//mutator


};

/********************************************************************/
/*******************************************************************/

class ENode{

	Edge* edge;
	ENode* next;

	public:

		ENode(Vertex* source, Vertex* dest,int weight);

		ENode(Edge* e);

		~ENode();

/////////////////////////////////////////////////////////////////

		Edge* getEdge() const;						//accessors
		ENode* getNext() const;

/////////////////////////////////////////////////////////////////

		void modNext(ENode *n);					// mutators
		void modEdge(Edge *n);

};


/************************************************************************/
/***********************************************************************/


class EList{

	ENode *head;
	ENode *tail;

	public:

		EList();

		~EList();

///////////////////////////////////////

		ENode* getHead() const;				//accessors
		ENode* getTail() const;

///////////////////////////////////////

		void addEdge(Vertex* v1, Vertex* v2, int w);

		void addENode( Edge* temp);
		void addENode( ENode* temp);				// same as addEdge , only difference is that we just link the EList with the ENode ( above we also have to create the ENode)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////


		void updateEList(Edge *pe); 			//deletes the ENode which points to the same edge as pe, but doesnt delete the edge

		void prepDelete(Vertex *pv);			//before deleting Inc/Out edges , we have to update the Elist of the other node

		void prepDelete1(Vertex *v1, Vertex* v2);		// complementary function of DeleteEdges()
														// same as above, but we dont search all the edges connected with V1 (or pv) , but only the ones which "connect" v1 and v2
														// In other words, both v1->...->v2 and v2->..->v1 (all the edges between the two)

		void deleteENode(Edge *pe);			//find and delete the  ENode which points to the same Edge as *pe, and also delete the Edge

////////////////////////////////////////////////////////////////////////////////////

		void findENode(VNode* v1,VNode* v2, int w, ENode **temp);				//find the ENode and put its address in temp, and if it doesnt exist *temp = NULL;

		void Print();				//Print List of Edges (EdgeNodes->Edges)

		void Print1();				//Print List of Edges (EdgeNodes->Edges), complementary function of circlefind

		bool Exist(Edge* pe);					//if a given Edge exists

};

/*************************************************************************************/
/*************************************************************************************/

class Vertex{

	char* name;

	EList* inc;
	EList* out;



	public:

		Vertex(char* s);


		~Vertex();

///////////////////////////////////////////////////////////////////////////////////////////////////

		char* getName() const;

		EList* getInc() const;
		EList* getOut() const;

/////////////////////////////////////////////////////////////////////////////////////////////////

		void modInc(EList* n);
		void modeOut(EList* n);

};

/************************************************************************************************/
/***********************************************************************************************/

class VNode{

	Vertex* vertex;
	VNode* next;

	public:

		VNode(char* name);

		VNode(Vertex* V);

		~VNode();

/////////////////////////////////////////////////////////////////

		Vertex* getVertex() const;				// accessors
		VNode* getNext() const;

/////////////////////////////////////////////////////////////////

		void modNext(VNode *n);					// mutators
		void modVertex(Vertex* v);



};

/************************************************************************************************/
/***********************************************************************************************/

class VList{

	VNode *head;
	VNode *tail;

	public:

		VList();

		~VList();

///////////////////////////////////////

		VNode* getHead() const;			// accessors
		VNode* getTail() const;

///////////////////////////////////////


		void addVNode(VNode* pv);
		void addVertex(Vertex* V);



		void deleteVNode(VNode *pv);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////


		void findVNode(char* s, VNode** temp);			// Search the VList using  VNode** temp, and in the end *temp will have the address of the VNode, or NULL 

		void updateVList(Vertex *pv);				//deletes VNodes without deleting vertices ( without calling the destructors )

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		bool Exist(Vertex* V);					//find if a Vertex V exists in the VList

};

/************************************************************************************************/
/***********************************************************************************************/

class Graph{

	VList *LV;				// LV -> list of pointers -> Vertices

	public:

		Graph();

		~Graph();

///////////////////////////////////////////////////////////

		VList* getLV() const;					// accessor


/////////////////////////////////////////////////////////

		void InsertVertex(char* s);

		void InsertEdge(char* s1, char* s2, int w);

/////////////////////////////////////////////////////////

		void DeleteVertex(char* s);


		void DeleteEdge(char *s1,char *s2, int w);
		void DeleteEdges(char *s1,char *s2);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		void ModEdgeW(char *s1, char *s2,int w, int nw);

		void Receiving(char *s);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void DFS(Vertex* start, Vertex* current, VList** visited, EList** edges,int* count);				//DFS used in circlefind( NO duplicate edges and no duplicate vertices)

		void DFS1(Vertex* start, Vertex* current, EList** edges, int k, int* count);						//DFS used in findcircles (no duplicate edges, duplicate vertices accepted EXCEPT start node )

		void DFS2(Vertex* start, Vertex* dest, Vertex* current, EList** edges, int* l, int* count);		// DFS used in traceflow, (no duplicate edges)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void findcircles(char *s, int k);

		void circlefind(char* s);

		void traceflow(char* s1, char* s2, int l);

};






