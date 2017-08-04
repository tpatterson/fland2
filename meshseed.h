#ifndef MESHSEED_H
#define MESHSEED_H

using namespace std;

#include "baseent.h"
#include "vertex.h"
#include "templates.h"
#include "defines.h"


// creates a new mesh as the child of this entity.
class MeshSeed : public BaseEnt
{
	private:
	QUARD_TYPE x, y, z;
	DIS_TYPE size;

	public:
	static Reuse< BaseEnt, MeshSeed >reuse;
	void setMeshParams( QUARD_TYPE, QUARD_TYPE, QUARD_TYPE, DIS_TYPE );

	// storing this may be unnecessary because the verticies are stored in the normal child list.
	Vertex *vChild[4];

	// after the constructor call this to create the mesh.
	virtual void split();
	virtual void unSplit();

	virtual int constructor( BaseEnt *parent );
	virtual void destructor( bool willDelete = false );
	virtual bool specialUnSplitCheck();

	virtual void draw();
	void attachVertexNeighbors();
};

#endif

