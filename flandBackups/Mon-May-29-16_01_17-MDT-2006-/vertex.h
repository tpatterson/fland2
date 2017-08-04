// every vertex in a mesh
#ifndef VERTEX_H
#define VERTEX_H

// NOTE: A vertex's children will be stored in the child vector, along with
// in the vChild array.

using namespace std;

#include <math.h>
#include "rendervertex.h"
#include "baseent.h"
#include "templates.h"
#include "main.h"

#define NUM_AFFECT_VERTICIES 4

//#define LEFT 0;
//#define RIGHT 1;

class Vertex : public BaseEnt
{
	public:

	// Pointer to each child this entity has. NULL is used if the children dont exist.
	//BaseEnt *vChild[9];
	Vertex *vChild[9];

	// Pointer to each neighbor. Ourself is included, just to make it easier to use.
	// NULL if not exist.
	Vertex *neighbor[9];

	// This keeps track of which child of our parent's we are.
	// from 0 to 8
	// -1 means we dont have a parent vertex - our parent is the mesh
	int vChildNum;

	RenderVertex trueVertex; // where the vertex really is
	RenderVertex startMorphVertex; // where we start morphing.
	RenderVertex morphVertex; // current morph location


	// This is for the rules. It keeps track of which neighboring verticies affect us
	// and by how much.
	BaseEnt* affectVertex[NUM_AFFECT_VERTICIES];
	double affectAmount[NUM_AFFECT_VERTICIES];

	// EXTRAS. These parameters dont apply to the actual workings of the vertex, but
	// how some rules (will) work with them.
	// Certain rules (will) use these values
	//QUARD_TYPE u, v;
	inline QUARD_TYPE getAltitude();
	inline float getSlope();
	//QUARD_TYPE slope;

	///////////////////////////////////////////////////////////////////////////
	// the functions

	// calculate a vertex's children's normals, based on the vertex's neighbors.
	void calcNormals();

	virtual int constructor( BaseEnt *parent );

	virtual void destructor( bool willDelete = false );

	// used in the destructor to clear neighbor's pointers to me.
	void clearNeighborPointersToMe();

	virtual void draw();

	virtual void split();

	// checks a vertex's neighbors.
	virtual bool specialUnSplitCheck();

	//virtual int calculateMe();

	virtual bool canUnSplit();

	virtual void calcMorph( double );

	#if( DEBUG )
	virtual void dump( ostream&, int );
	virtual string toString( int );
	virtual void printType();
	virtual string getType();
	void recursiveCheck();
	#endif
	static Reuse< BaseEnt, Vertex > reuse;

	// FROM MESH
	// The recursive function that draws the triangles.
	// returns 1 if something went wrong.
	// Takes three vertexes and tritype is either LEFT or RIGHT
	//int drawTri( BaseEnt*, BaseEnt*, BaseEnt*, int );
	int drawLeft( BaseEnt*, BaseEnt*, BaseEnt* );
	int drawRight( BaseEnt*, BaseEnt*, BaseEnt* );


	void teapot();
	private:
	bool isEdge; // if this vertex lies on the edge of a mesh.
					 // Different split logic applies
	bool isCorner; // if on a corner of more then one mesh. WAY different logic.

	// This makes sure that a vertex's neighbors are all split to at least the same
	// depth as she is.
	int assertNeighborsSplit();

	// This function creates the children and points us to them, and them to us.
	int createChildren();

	// points the children at thier neighbors.
	int pointChildrenAtNeighbors();

	// sets the affect verticies and how much affect they have on EVERY vChild.
	int calcAffectVerticies();

	// calculate the starting morph location, colors, etc...
	// of a vertex's newly created children
	void calcStartMorph();


};

QUARD_TYPE Vertex::getAltitude()
{
	return trueVertex.y;
}
float Vertex::getSlope()
{
	return acos( trueVertex.ny ) * ( 1.0 / ( PI / 2.0 ) );
}


#endif

