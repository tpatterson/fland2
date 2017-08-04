#ifndef MESHSEED_CPP
#define MESHSEED_CPP

#include <cassert>
#include "meshseed.h"
#include "defines.h"
#include "tyler.h"
#include "defines.h"
#include "glutenv.h"
#include "fland.h"

Reuse< BaseEnt, MeshSeed >MeshSeed::reuse;

void MeshSeed::setMeshParams( QUARD_TYPE ix, QUARD_TYPE iy, QUARD_TYPE iz, DIS_TYPE isize )
{
	x = bound.x = ix;
	y = bound.y = iy;
	z = bound.z = iz;
	size = bound.radius = isize;
}


void MeshSeed::split()
{
	for( int i=0; i<4; i++ )
	{
		Vertex* vert = VERT( Vertex::reuse.tNew( this ) );
		vChild[i] = vert;
		child.push_back( vert );

		// so the bound radius will extend and overlap the opposite corner vertex.
		vert->bound.radius = size * 1.5;
		vert->trueVertex.nz = 1.0; // BUG: TODO: this normal should be positive? what is my quardinate system?

		// NOTE: this is where the inital properties are set.
		vert->trueVertex.red   = 1.0;
		vert->trueVertex.green = 1.0;
		vert->trueVertex.blue  = 1.0;
	}
	attachVertexNeighbors();

	// Set the quardinates of the verticies.
	VERT(vChild[0])->trueVertex.x = x - size * 0.5;
	VERT(vChild[0])->trueVertex.y = y;
	VERT(vChild[0])->trueVertex.z = z - size * 0.5;

	VERT(vChild[1])->trueVertex.x = x + size * 0.5;
	VERT(vChild[1])->trueVertex.y = y;
	VERT(vChild[1])->trueVertex.z = z - size * 0.5;

	VERT(vChild[2])->trueVertex.x = x - size * 0.5;
	VERT(vChild[2])->trueVertex.y = y;
	VERT(vChild[2])->trueVertex.z = z + size * 0.5;

	VERT(vChild[3])->trueVertex.x = x + size * 0.5;
	VERT(vChild[3])->trueVertex.y = y;
	VERT(vChild[3])->trueVertex.z = z + size * 0.5;

	VERT(vChild[0])->startMorphVertex = VERT(vChild[0])->trueVertex;
	VERT(vChild[1])->startMorphVertex = VERT(vChild[1])->trueVertex;
	VERT(vChild[2])->startMorphVertex = VERT(vChild[2])->trueVertex;
	VERT(vChild[3])->startMorphVertex = VERT(vChild[3])->trueVertex;

	BaseEnt::split();
}

int MeshSeed::constructor( BaseEnt *parent )
{
	for( int i=0; i<4; i++ )
		vChild[i] = NULL;
	x = y = z = 0.0;
	size = 0.0;
	BaseEnt::constructor( parent );
}

void MeshSeed::destructor( bool willDelete )
{
	for( int i=0; i<4; i++ )
		vChild[i] = NULL;
	// delete our children, rules and parent's references to us
	BaseEnt::destructor( true );

	// and finally, if we're not being deleted by a child class (if one
	// eventually exist) delete ourselves.
	if( !willDelete )
		reuse.tDelete( this );
}


bool MeshSeed::specialUnSplitCheck()
{
	// cant be unsplit (i think thats what false means)
	return false;
	//return true;
}

void MeshSeed::attachVertexNeighbors()
{
  // point the verticies at thier neighbors.
	vChild[0]->neighbor[4] = vChild[0];
	vChild[0]->neighbor[5] = vChild[1];
	vChild[0]->neighbor[7] = vChild[2];
	vChild[0]->neighbor[8] = vChild[3];

	vChild[1]->neighbor[4] = vChild[1];
	vChild[1]->neighbor[3] = vChild[0];
	vChild[1]->neighbor[6] = vChild[2];
	vChild[1]->neighbor[7] = vChild[3];

	vChild[2]->neighbor[4] = vChild[2];
	vChild[2]->neighbor[1] = vChild[0];
	vChild[2]->neighbor[2] = vChild[1];
	vChild[2]->neighbor[5] = vChild[3];

	vChild[3]->neighbor[4] = vChild[3];
	vChild[3]->neighbor[0] = vChild[0];
	vChild[3]->neighbor[1] = vChild[1];
	vChild[3]->neighbor[3] = vChild[2];
}

void MeshSeed::draw()
{
	//cout << "drawmesh";

	// make sure the verticies exist before we try to reference them.
	// to avoid seg faults.

	if( isSplit )
	{

		sassert( vChild[0] );
		sassert( vChild[1] );
		sassert( vChild[2] );
		sassert( vChild[3] );


		vChild[0]->drawLeft( vChild[2],
						vChild[0],
						vChild[1] );
		vChild[0]->drawRight( VERT( vChild[1] ),
					VERT( vChild[3] ),
					VERT( vChild[2] ) );

		//VERT( vChild[0] )->teapot();
		//VERT( vChild[1] )->teapot();
		//VERT( vChild[2] )->teapot();
		//VERT( vChild[3] )->teapot();

		// pass the draw message down the line.
		BaseEnt::draw();
	}
	else
	{
		//drawSphere( x - Fland::fland->camera->x, y - Fland::fland->camera->y, z - Fland::fland->camera->z, size );
	}

}

void MeshSeed::unSplit()
{
	BaseEnt::unSplit();
	for( int i=0; i<4; i++ )
		vChild[i] = NULL;
}

#endif

