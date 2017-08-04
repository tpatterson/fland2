// every vertex in a mesh
#ifndef VERTEX_CPP
#define VERTEX_CPP

// NOTE: A vertex's children will be stored in the child vector, along with
// in the vChild array.

using namespace std;

#include <sstream>
#include <GL/glut.h>
#include "tyler.h"
#include "rendervertex.h"
#include "baseent.h"
#include "templates.h"
#include "main.h"
#include "vertex.h"
#include "fland.h"


// debugging help
void Vertex::teapot()
{
	Camera *cam = getFlandPtr()->camera;

	glDisable( GL_DEPTH_TEST );

	glPushMatrix();
	glTranslatef( trueVertex.x - cam->x, trueVertex.y - cam->y, trueVertex.z - cam->z );
	glColor4f( 1.0, 0.5, 0.5, 0.5 );
	glutSolidSphere( bound.radius/ 10, 10, 5 );
	glPopMatrix(  );

	glPushMatrix();
	glTranslatef( startMorphVertex.x - cam->x, startMorphVertex.y - cam->y, startMorphVertex.z - cam->z );
	glColor4f( 0.5, 1.0, 0.5, 0.5 );
	glutSolidSphere( bound.radius/ 15, 10, 5 );
	glPopMatrix(  );

	glPushMatrix();
	glTranslatef( morphVertex.x - cam->x, morphVertex.y - cam->y, morphVertex.z - cam->z );
	glColor4f( 0.5, 0.5, 1.0, 0.5 );
	glutSolidSphere( bound.radius/ 20, 10, 5 );
	glPopMatrix(  );

	glEnable( GL_DEPTH_TEST );

	//glPushMatrix();
	//glTranslatef( morphVertex.x, morphVertex.y, morphVertex.z );
	//glColor4f( 0.5, 0.5, 1.0, 0.5 );
	//glDisable( GL_DEPTH_TEST );
	//glutSolidSphere( bound.radius/10, 10, 5 );
	//glEnable( GL_DEPTH_TEST );
	//glPopMatrix(  );
}

void Vertex::printType(){ cout << "Vertex" << endl;}
string Vertex::getType(){ return "Vertex"; }

#if DEBUG

void Vertex::recursiveCheck()
{
	if( morphVertex.x > .8 ||
		 morphVertex.x < -.8 ||
		 morphVertex.z > .8 ||
		 morphVertex.z < -.8 )
	{
		cout << toString( 0 );
	}
	for( int i=0; i<9; i++ )
	{
		vChild[i]->recursiveCheck();
	}
}

void Vertex::dump( ostream& out, int depth )
{
	out << indent( depth ) << "Vertex\n";
	//cout << "Neighbor[4] = " << neighbor[4];
	BaseEnt::dump( out, depth );
}

string Vertex::toString( int depth = 0 )
{
	ostringstream out;
	string ind = indent( depth );

	out << BaseEnt::toString( depth );

	out << ind << "Vertex::\n";
	for( int i=0; i<9; i++ )
	{
	out << ind << "vChild[" << i << "]         " << vChild[i] << endl;
	}
	for( int i=0; i<9; i++ )
	{
	out << ind << "neighbor[" << i << "]       " << neighbor[i] << endl;
	}
	out << ind << "vChildNum         " << vChildNum << endl;

	for( int i=0; i<NUM_AFFECT_VERTICIES; i++ )
	{
		out << ind << "Affect Vertex[" << i << "]   " << affectVertex[i] << endl;
		out << ind << "Affect Amount[" << i << "]   " << affectAmount[i] << endl;
	}
	out << ind << "trueVertex       " << trueVertex.toString() << endl;
	out << ind << "startMorphVertex " << startMorphVertex.toString()<< endl;
	out << ind << "morphVertex      " << morphVertex.toString()<< endl;

	return out.str();
}
#endif

Reuse< BaseEnt, Vertex > Vertex::reuse;

void Vertex::split()
{
	// if this vertex is already split, send the message to split our kids.
	if( isSplit )
	{
		// split our kids
		BaseEnt::split();
		// exit function
	}
	tSrand( seed );
	// Make sure all our neighbors exist. If they dont, this function will
	// split the aunt until they do exist.
	assertNeighborsSplit();
	createChildren();
	pointChildrenAtNeighbors();
	calcAffectVerticies();
	calcNormals();
	// This will create any other children.

	// (THESE have to be in this order or the "STAR" bug occurs)
	calcStartMorph();
	BaseEnt::split();
}

void Vertex::calcStartMorph()
{
	// children 1 through 7 are normal cases
	for( int i=0; i<9; i++ )
	{
		if( vChild[i] )
		{
			//vChild[i]->startMorphVertex = VERT(this)->trueVertex * ( 1.0 - vChild[i]->createVisibility );
			//vChild[i]->startMorphVertex += neighbor[i]->trueVertex * ( vChild[i]->createVisibility );
			vChild[i]->startMorphVertex = VERT(this)->trueVertex * ( 2.0 / 3.0 );
			vChild[i]->startMorphVertex += neighbor[i]->trueVertex * ( 1.0 / 3.0 );
		}
	}

	// 0 and 8 are special, due to the orientation of the triangles being rendered.
	if( vChild[0] )
	{
		vChild[0]->startMorphVertex = VERT(this)->trueVertex * ( vChild[0]->createVisibility );
		vChild[0]->startMorphVertex += neighbor[1]->trueVertex * ( vChild[0]->createVisibility );
		vChild[0]->startMorphVertex += neighbor[3]->trueVertex * ( vChild[0]->createVisibility );
	}

	if( vChild[8] )
	{
		vChild[8]->startMorphVertex = VERT(this)->trueVertex * ( vChild[8]->createVisibility );
		vChild[8]->startMorphVertex += neighbor[5]->trueVertex * ( vChild[8]->createVisibility );
		vChild[8]->startMorphVertex += neighbor[7]->trueVertex * ( vChild[8]->createVisibility );
	}


	//// DEBUG
	//for( int i=0; i<9; i++ )
	//{
	//	if( vChild[i] )
	//	{
	//		if( abs( vChild[i]->startMorphVertex.x ) > .8 || abs( vChild[i]->startMorphVertex.z ) > .8 )
	//		{
	//			cout << i << endl;
	//			cout << vChild[i]->startMorphVertex.toString(0) << endl;
	//			cout << vChild[i]->createVisibility;LN
	//			//cout << vChild[i]->toString(10);
	//			sassert( 0 == 1 );
	//		}
	//	}
	//}
}

void Vertex::calcMorph(double morphAmount )
{
	Fland* fl = getFlandPtr();

	if( fl->env.keys[' '] == false )
	{
		if( morphAmount == 1.0 ) // dont spend our time interpolating if we dont need to
		{
			morphVertex = trueVertex;
		}
		else
		{
			morphVertex.interpolate( startMorphVertex, trueVertex, morphAmount );
		}
	}
	else
	{
		morphVertex = trueVertex;
	}
}

void Vertex::calcNormals()
{
	// calcVertexNormal. Put the normal in v1.
#define CVN( v1, v2, v3, store ) calcNormal( v2->trueVertex.x, v2->trueVertex.y, v2->trueVertex.z, v1->trueVertex.x, v1->trueVertex.y, v1->trueVertex.z, v3->trueVertex.x, v3->trueVertex.y, v3->trueVertex.z, VERT(store)->trueVertex.nx, VERT(store)->trueVertex.ny, VERT(store)->trueVertex.nz );
	// Average 2 verticies and store the result.
#define AV( in1, in2, store ) store->trueVertex.nx = ( in1->trueVertex.nx + in2->trueVertex.nx ); \
											  store->trueVertex.ny = ( in1->trueVertex.ny + in2->trueVertex.ny ); \
											  store->trueVertex.nz = ( in1->trueVertex.nz + in2->trueVertex.nz ); \
											  normalize( store->trueVertex.nx, store->trueVertex.ny, store->trueVertex.nz );

	if( neighbor[0] )
	{
		CVN( neighbor[3], neighbor[0], neighbor[4], vChild[3] );
		CVN( neighbor[1], neighbor[4], neighbor[0], vChild[1] );
		AV( vChild[3], vChild[1], vChild[0] );
	}

	if( neighbor[2] )
	{
		CVN( neighbor[1], neighbor[2], neighbor[4], vChild[1] );
		CVN( neighbor[5], neighbor[4], neighbor[2], vChild[5] );
		AV( vChild[1], vChild[5], vChild[2] );
	}
	if( neighbor[6] )
	{
		CVN( neighbor[7], neighbor[6], neighbor[4], vChild[7] );
		CVN( neighbor[3], neighbor[4], neighbor[6], vChild[3] );
		AV( vChild[7], vChild[3], vChild[6] );
	}
	if( neighbor[8] )
	{
		CVN( neighbor[5], neighbor[7], neighbor[4], vChild[5] );
		CVN( neighbor[7], neighbor[4], neighbor[8], vChild[7] );
		AV( vChild[7], vChild[5], vChild[8] );
	}

	for( int i=0; i<9; i++ )
	{
		if( i == 4 )
			continue;
		if( vChild[i] )
		{
			vChild[4]->trueVertex.nx += vChild[i]->trueVertex.nx;
			vChild[4]->trueVertex.ny += vChild[i]->trueVertex.ny;
			vChild[4]->trueVertex.nz += vChild[i]->trueVertex.nz;
		}
	}
	normalize( vChild[4]->trueVertex.nx, vChild[4]->trueVertex.ny, vChild[4]->trueVertex.nz );

	// TODO: this block shouldn't be necessary, and it slows things down. Fix the problem.
	for( int i=0; i<9; i++ )
	{
		if( vChild[i] )
		{
			// This line messes up once if the file is loaded
			if( !normalize( vChild[i]->trueVertex.nx, vChild[i]->trueVertex.ny, vChild[i]->trueVertex.nz ) )
			{
				//cout << i << endl;;LN
				//vChild[i]->toString();
			}
		}
	}


}

int Vertex::calcAffectVerticies()
{
	/*
	for( int c=0; c<9; c++ )
	{
		if( vChild[c] )
		{
			vChild[c]->affectVertex[0] = this;
				vChild[c]->affectAmount[0] = 2.0 / 3.0;
			vChild[c]->affectVertex[1] = neighbor[c];
				vChild[c]->affectAmount[1] = 1.0 / 3.0;
		}
	}
	return 0;
	*/
	// the old way
	if( vChild[0] )
	{
		VERT(vChild[0])->affectVertex[0] = this;
			VERT(vChild[0])->affectAmount[0] = 4.0 / 9.0;
		VERT(vChild[0])->affectVertex[1] = neighbor[3];
			VERT(vChild[0])->affectAmount[1] = 2.0 / 9.0;
		VERT(vChild[0])->affectVertex[2] = neighbor[0];
			VERT(vChild[0])->affectAmount[2] = 1.0 / 9.0;
		VERT(vChild[0])->affectVertex[3] = neighbor[1];
			VERT(vChild[0])->affectAmount[3] = 2.0 / 9.0;
	}

	if( vChild[1] )
	{
		VERT(vChild[1])->affectVertex[0] = this;
			VERT(vChild[1])->affectAmount[0] = 6.0 / 9.0;
		VERT(vChild[1])->affectVertex[1] = neighbor[1];
			VERT(vChild[1])->affectAmount[1] = 3.0 / 9.0;
	}

	if( vChild[2] )
	{
		VERT(vChild[2])->affectVertex[0] = this;
			VERT(vChild[2])->affectAmount[0] = 4.0 / 9.0;
		VERT(vChild[2])->affectVertex[1] = neighbor[1];
			VERT(vChild[2])->affectAmount[1] = 2.0 / 9.0;
		VERT(vChild[2])->affectVertex[2] = neighbor[2];
			VERT(vChild[2])->affectAmount[2] = 1.0 / 9.0;
		VERT(vChild[2])->affectVertex[3] = neighbor[5];
			VERT(vChild[2])->affectAmount[3] = 2.0 / 9.0;
	}

	if( vChild[3] )
	{
		VERT(vChild[3])->affectVertex[0] = this;
			VERT(vChild[3])->affectAmount[0] = 6.0 / 9.0;
		VERT(vChild[3])->affectVertex[1] = neighbor[3];
			VERT(vChild[3])->affectAmount[1] = 3.0 / 9.0;
	}

	// vChild[4] will always exist
	VERT(vChild[4])->affectVertex[0] = this;
		VERT(vChild[4])->affectAmount[0] = 9.0 / 9.0;


	if( vChild[5] )
	{
		VERT(vChild[5])->affectVertex[0] = this;
			VERT(vChild[5])->affectAmount[0] = 6.0 / 9.0;
		VERT(vChild[5])->affectVertex[1] = neighbor[5];
			VERT(vChild[5])->affectAmount[1] = 3.0 / 9.0;
	}

	if( vChild[6] )
	{
		VERT(vChild[6])->affectVertex[0] = this;
			VERT(vChild[6])->affectAmount[0] = 4.0 / 9.0;
		VERT(vChild[6])->affectVertex[1] = neighbor[3];
			VERT(vChild[6])->affectAmount[1] = 2.0 / 9.0;
		VERT(vChild[6])->affectVertex[2] = neighbor[6];
			VERT(vChild[6])->affectAmount[2] = 1.0 / 9.0;
		VERT(vChild[6])->affectVertex[3] = neighbor[7];
			VERT(vChild[6])->affectAmount[3] = 2.0 / 9.0;
	}

	if( vChild[7] )
	{
		VERT(vChild[7])->affectVertex[0] = this;
			VERT(vChild[7])->affectAmount[0] = 6.0 / 9.0;
		VERT(vChild[7])->affectVertex[1] = neighbor[7];
			VERT(vChild[7])->affectAmount[1] = 3.0 / 9.0;
	}

	if( vChild[8] )
	{
		VERT(vChild[8])->affectVertex[0] = this;
			VERT(vChild[8])->affectAmount[0] = 4.0 / 9.0;
		VERT(vChild[8])->affectVertex[1] = neighbor[5];
			VERT(vChild[8])->affectAmount[1] = 2.0 / 9.0;
		VERT(vChild[8])->affectVertex[2] = neighbor[8];
			VERT(vChild[8])->affectAmount[2] = 1.0 / 9.0;
		VERT(vChild[8])->affectVertex[3] = neighbor[7];
			VERT(vChild[8])->affectAmount[3] = 2.0 / 9.0;
	}
	return 0;
}

int Vertex::createChildren()
{
	// loop through and create each vChild.
	for( int i=0; i<9; i++ )
	{
		// Make sure the mesh extends in the direction of the vChild being created.
		// This insures we dont create children that will be off the edge of the mesh.
		if( neighbor[i] )
		{
			// create the vChild object
			vChild[i] = VERT(reuse.tNew( this ));

			// Set the bound radius to be a third of the parent's
			vChild[i]->bound.radius = bound.radius / 3.0;
			// If any rules change the bound radius, rerun this calulation!
			vChild[i]->createVisibility = bound.radius / parent->bound.radius;

			// add it to the children list
			child.push_back( vChild[i] );

			// set the vChildNum
			VERT(vChild[i])->vChildNum = i;
			// use the next random number for the child's seed.
			vChild[i]->seed = tRand();
		}
	}
	return 0;
}

int Vertex::pointChildrenAtNeighbors()
{

	// first point the vChild at himself, otherwise things get messed up.
	VERT(vChild[4])->neighbor[4] = vChild[4];

	// point vChildren at their siblings
	// NOTE: These setpointer commands are based on the corner verticies.
	// if each corner exist it connects all the pointers in it's quadrant.
	// some pointers will be connected twice, but this way is very simple.
	if( vChild[0] ) // If we're on an edge, this vertex may not exist.
	{
		// corner vertex
		VERT(vChild[0])->neighbor[5] = vChild[1];
			VERT(vChild[1])->neighbor[3] = vChild[0];
		VERT(vChild[0])->neighbor[7] = vChild[3];
			VERT(vChild[3])->neighbor[1] = vChild[0];
		VERT(vChild[0])->neighbor[8] = vChild[4];
			VERT(vChild[4])->neighbor[0] = vChild[0];
		// middle verticies
		VERT(vChild[1])->neighbor[7] = vChild[4];
			VERT(vChild[4])->neighbor[1] = vChild[1];
		VERT(vChild[3])->neighbor[5] = vChild[4];
			VERT(vChild[4])->neighbor[3] = vChild[3];
		// diagnals
		VERT(vChild[3])->neighbor[2] = vChild[1];
			VERT(vChild[1])->neighbor[6] = vChild[3];
	}

	if( vChild[2] ) // If we're on an edge, this vertex may not exist.
	{
		// corner vertex
		VERT(vChild[2])->neighbor[3] = vChild[1];
			VERT(vChild[1])->neighbor[5] = vChild[2];
		VERT(vChild[2])->neighbor[7] = vChild[5];
			VERT(vChild[5])->neighbor[1] = vChild[2];
		VERT(vChild[2])->neighbor[6] = vChild[4];
			VERT(vChild[4])->neighbor[2] = vChild[2];
		// middle verticies
		VERT(vChild[1])->neighbor[7] = vChild[4];
			VERT(vChild[4])->neighbor[1] = vChild[1];
		VERT(vChild[5])->neighbor[3] = vChild[4];
			VERT(vChild[4])->neighbor[5] = vChild[5];
		// diagnals
		VERT(vChild[5])->neighbor[0] = vChild[1];
			VERT(vChild[1])->neighbor[8] = vChild[5];
	}

	if( vChild[6] ) // If we're on an edge, this vertex may not exist.
	{
		// corner vertex
		VERT(vChild[6])->neighbor[5] = vChild[7];
			VERT(vChild[7])->neighbor[3] = vChild[6];
		VERT(vChild[6])->neighbor[1] = vChild[3];
			VERT(vChild[3])->neighbor[7] = vChild[6];
		VERT(vChild[6])->neighbor[2] = vChild[4];
			VERT(vChild[4])->neighbor[6] = vChild[6];
		// middle verticies
		VERT(vChild[3])->neighbor[5] = vChild[4];
			VERT(vChild[4])->neighbor[3] = vChild[3];
		VERT(vChild[7])->neighbor[1] = vChild[4];
			VERT(vChild[4])->neighbor[7] = vChild[7];
		// diagnals
		VERT(vChild[3])->neighbor[8] = vChild[7];
			VERT(vChild[7])->neighbor[0] = vChild[3];
	}

	if( vChild[8] ) // If we're on an edge, this vertex may not exist.
	{
		// corner vertex
		VERT(vChild[8])->neighbor[3] = vChild[7];
			VERT(vChild[7])->neighbor[5] = vChild[8];

		VERT(vChild[8])->neighbor[1] = vChild[5];
			VERT(vChild[5])->neighbor[7] = vChild[8];
		VERT(vChild[8])->neighbor[0] = vChild[4];
			VERT(vChild[4])->neighbor[8] = vChild[8];
		// middle verticies
		VERT(vChild[5])->neighbor[3] = vChild[4];
			VERT(vChild[4])->neighbor[5] = vChild[5];
		VERT(vChild[7])->neighbor[1] = vChild[4];
			VERT(vChild[4])->neighbor[7] = vChild[7];
		// diagnals
		VERT(vChild[5])->neighbor[6] = vChild[7];
			VERT(vChild[7])->neighbor[2] = vChild[5];
	}


	// point vChildren at thier cousins
	// must check that cousins are split, and that we dont lie on an edge (some
	// verticies dont exist).

	// neighbor 0, a diagnal
	if( neighbor[0] && neighbor[0]->isSplit )
	{
		VERT(vChild[0])->neighbor[0] = VERT(neighbor[0])->vChild[8];
			VERT(VERT(neighbor[0])->vChild[8])->neighbor[8] = vChild[0];
	}

	// neighbor 1
	if( neighbor[1] && neighbor[1]->isSplit )
	{
		// The straight edge.
		VERT(vChild[1])->neighbor[1] = VERT(neighbor[1])->vChild[7];
			VERT(VERT(neighbor[1])->vChild[7])->neighbor[7] = vChild[1];
		// if the corner vChild exists, then we know we aren't hanging over an edge
		if( vChild[0] )
		{
			VERT(vChild[0])->neighbor[1] = VERT(neighbor[1])->vChild[6];
				VERT(VERT(neighbor[1])->vChild[6])->neighbor[7] = vChild[0];
			VERT(vChild[0])->neighbor[2] = VERT(neighbor[1])->vChild[7];
				VERT(VERT(neighbor[1])->vChild[7])->neighbor[6] = vChild[0];

			VERT(vChild[1])->neighbor[0] = VERT(neighbor[1])->vChild[6];
				VERT(VERT(neighbor[1])->vChild[6])->neighbor[8] = vChild[1];
		}

		if( vChild[2] )
		{
			VERT(vChild[2])->neighbor[1] = VERT(neighbor[1])->vChild[8];
				VERT(VERT(neighbor[1])->vChild[8])->neighbor[7] = vChild[2];
			VERT(vChild[2])->neighbor[0] = VERT(neighbor[1])->vChild[7];
				VERT(VERT(neighbor[1])->vChild[7])->neighbor[8] = vChild[2];

			VERT(vChild[1])->neighbor[2] = VERT(neighbor[1])->vChild[8];
				VERT(VERT(neighbor[1])->vChild[8])->neighbor[6] = vChild[1];
		}
	}

	// neighbor 2, a diagnal
	if( neighbor[2] && neighbor[2]->isSplit )
	{
		VERT(vChild[2])->neighbor[2] = VERT(neighbor[2])->vChild[6];
			VERT(VERT(neighbor[2])->vChild[6])->neighbor[6] = vChild[2];
	}

	// neighbor 3
	if( neighbor[3] && neighbor[3]->isSplit )
	{
		// The straight edge.
		VERT(vChild[3])->neighbor[3] = VERT(neighbor[3])->vChild[5];
			VERT(VERT(neighbor[3])->vChild[5])->neighbor[5] = vChild[3];
		// if the corner vChild exists, then we know we aren't hanging over an edge
		if( vChild[6] )
		{
			VERT(vChild[6])->neighbor[3] = VERT(neighbor[3])->vChild[8];
				VERT(VERT(neighbor[3])->vChild[8])->neighbor[5] = vChild[6];
			VERT(vChild[6])->neighbor[0] = VERT(neighbor[3])->vChild[5];
				VERT(VERT(neighbor[3])->vChild[5])->neighbor[8] = vChild[6];

			VERT(vChild[3])->neighbor[6] = VERT(neighbor[3])->vChild[8];
				VERT(VERT(neighbor[3])->vChild[8])->neighbor[2] = vChild[3];
		}

		if( vChild[0] )
		{
			VERT(vChild[0])->neighbor[3] = VERT(neighbor[3])->vChild[2];
				VERT(VERT(neighbor[3])->vChild[2])->neighbor[5] = vChild[0];
			VERT(vChild[0])->neighbor[6] = VERT(neighbor[3])->vChild[5];
				VERT(VERT(neighbor[3])->vChild[5])->neighbor[2] = vChild[0];

			VERT(vChild[3])->neighbor[0] = VERT(neighbor[3])->vChild[2];
				VERT(VERT(neighbor[3])->vChild[2])->neighbor[8] = vChild[3];
		}
	}

	// neighbor 5
	if( neighbor[5] && neighbor[5]->isSplit )
	{
		// The straight edge.
		VERT(vChild[5])->neighbor[5] = VERT(neighbor[5])->vChild[3];
			VERT(VERT(neighbor[5])->vChild[3])->neighbor[3] = vChild[5];

		// if the corner vChild exists, then we know we aren't hanging over an edge
		if( vChild[2] )
		{

			VERT(vChild[2])->neighbor[5] = VERT(neighbor[5])->vChild[0];
				VERT(VERT(neighbor[5])->vChild[0])->neighbor[3] = vChild[2];
			VERT(vChild[2])->neighbor[8] = VERT(neighbor[5])->vChild[3];
				VERT(VERT(neighbor[5])->vChild[3])->neighbor[0] = vChild[2];

			VERT(vChild[5])->neighbor[2] = VERT(neighbor[5])->vChild[0];
				VERT(VERT(neighbor[5])->vChild[0])->neighbor[6] = vChild[5];
		}

		if( vChild[8] )
		{
			VERT(vChild[8])->neighbor[5] = VERT(neighbor[5])->vChild[6];
				VERT(VERT(neighbor[5])->vChild[6])->neighbor[3] = vChild[8];
			VERT(vChild[8])->neighbor[2] = VERT(neighbor[5])->vChild[3];

				VERT(VERT(neighbor[5])->vChild[3])->neighbor[6] = vChild[8];
			VERT(vChild[5])->neighbor[8] = VERT(neighbor[5])->vChild[6];
				VERT(VERT(neighbor[5])->vChild[6])->neighbor[0] = vChild[5];
		}
	}


	// neighbor 6, a diagnal
	if( neighbor[6] && neighbor[6]->isSplit )
	{
		VERT(vChild[6])->neighbor[6] = VERT(neighbor[6])->vChild[2];
			VERT(VERT(neighbor[6])->vChild[2])->neighbor[2] = vChild[6];
	}


	// neighbor 7
	if( neighbor[7] && neighbor[7]->isSplit )
	{
		// The straight edge.
		VERT(vChild[7])->neighbor[7] = VERT(neighbor[7])->vChild[1];
			VERT(VERT(neighbor[7])->vChild[1])->neighbor[1] = vChild[7];
		// if the corner vChild exists, then we know we aren't hanging over an edge
		if( vChild[8] )
		{
			VERT(vChild[8])->neighbor[7] = VERT(neighbor[7])->vChild[2];
				VERT(VERT(neighbor[7])->vChild[2])->neighbor[1] = vChild[8];
			VERT(vChild[8])->neighbor[6] = VERT(neighbor[7])->vChild[1];
				VERT(VERT(neighbor[7])->vChild[1])->neighbor[2] = vChild[8];

			VERT(vChild[7])->neighbor[8] = VERT(neighbor[7])->vChild[2];
				VERT(VERT(neighbor[7])->vChild[2])->neighbor[0] = vChild[7];
		}

		if( vChild[6] )
		{
			VERT(vChild[6])->neighbor[7] = VERT(neighbor[7])->vChild[0];
				VERT(VERT(neighbor[7])->vChild[0])->neighbor[1] = vChild[6];
			VERT(vChild[6])->neighbor[8] = VERT(neighbor[7])->vChild[1];
				VERT(VERT(neighbor[7])->vChild[1])->neighbor[0] = vChild[6];

			VERT(vChild[7])->neighbor[6] = VERT(neighbor[7])->vChild[0];
				VERT(VERT(neighbor[7])->vChild[0])->neighbor[2] = vChild[7];
		}
	}
	// neighbor 8, a diagnal
	if( neighbor[8] && neighbor[8]->isSplit )
	{
		VERT(vChild[8])->neighbor[8] = VERT(neighbor[8])->vChild[0];
			//BUG!!! This line sometimes causes seg faults! (may be fixed)
			VERT(VERT(neighbor[8])->vChild[0])->neighbor[0] = vChild[8];

	}


	return 0;
}

// If our neighbors dont exist, split the appropriate aunt until they do.
int Vertex::assertNeighborsSplit()
{
	switch( vChildNum )
	{
		case 0:
			// corner verticies check 3 neighbors.
			if( VERT(parent)->neighbor[3] && !VERT(parent)->neighbor[3]->isSplit )
				VERT(parent)->neighbor[3]->split();
			if( VERT(parent)->neighbor[0] && !VERT(parent)->neighbor[0]->isSplit )
				VERT(parent)->neighbor[0]->split();
			if( VERT(parent)->neighbor[1] && !VERT(parent)->neighbor[1]->isSplit )
				VERT(parent)->neighbor[1]->split();
		break;

		case 1:
			// side verticies only need to check 1 vertex.
			if( VERT(parent)->neighbor[1] && !VERT(parent)->neighbor[1]->isSplit )
				VERT(parent)->neighbor[1]->split();
		break;

		case 2:
			if( VERT(parent)->neighbor[1] && !VERT(parent)->neighbor[1]->isSplit )
				VERT(parent)->neighbor[1]->split();
			if( VERT(parent)->neighbor[2] && !VERT(parent)->neighbor[2]->isSplit )
				VERT(parent)->neighbor[2]->split();
			if( VERT(parent)->neighbor[5] && !VERT(parent)->neighbor[5]->isSplit )
				VERT(parent)->neighbor[5]->split();
		break;

		case 3:
			if( VERT(parent)->neighbor[3] && !VERT(parent)->neighbor[3]->isSplit )
				VERT(parent)->neighbor[3]->split();
		break;

		case 4:
			// We're the middle vChild and all our neighbor are guarenteed to exist.
			// Dont need to bother checking!
		break;

		case 5:
			if( VERT(parent)->neighbor[5] && !VERT(parent)->neighbor[5]->isSplit )
				VERT(parent)->neighbor[5]->split();
		break;

		case 6:
			if( VERT(parent)->neighbor[3] && !VERT(parent)->neighbor[3]->isSplit )
				VERT(parent)->neighbor[3]->split();
			if( VERT(parent)->neighbor[6] && !VERT(parent)->neighbor[6]->isSplit )
				VERT(parent)->neighbor[6]->split();
			if( VERT(parent)->neighbor[7] && !VERT(parent)->neighbor[7]->isSplit )
				VERT(parent)->neighbor[7]->split();
		break;

		case 7:
			if( VERT(parent)->neighbor[7] && !VERT(parent)->neighbor[7]->isSplit )
				VERT(parent)->neighbor[7]->split();
		break;

		case 8:
			if( VERT(parent)->neighbor[5] && !VERT(parent)->neighbor[5]->isSplit )
				VERT(parent)->neighbor[5]->split();
			if( VERT(parent)->neighbor[7] && !VERT(parent)->neighbor[7]->isSplit )
				VERT(parent)->neighbor[7]->split();
			if( VERT(parent)->neighbor[8] && !VERT(parent)->neighbor[8]->isSplit )
				VERT(parent)->neighbor[8]->split();
		break;

		case -1:
			// We're the root vertex in the mesh. No need to split at this time.
		break;

		default:
			cout << "vChildnum isn't from -1 to 7!" << endl;
		break;

	}

	return 0;
}

int Vertex::constructor( BaseEnt *inParent )
{
	isEdge = false;
	isCorner = false;
	vChildNum = -1;

	for( int i=0; i<9; i++ )
	{
		vChild[i] = NULL;
		neighbor[i] = NULL;
	}

	for( int i=0; i<NUM_AFFECT_VERTICIES; i++ )
	{
		affectVertex[i] = NULL;
		affectAmount[i] = 0.0;
	}
	neighbor[4] = this;

	// this morph vertex shouldn't really need to be initalized
	morphVertex.initalize();
	trueVertex.initalize();
	startMorphVertex.initalize();

	BaseEnt::constructor( inParent );
	return 0;
}

void Vertex::draw()
{
#if( 0 )
	if( !isSplit )
		teapot();
		//morphVertex.drawNormal( bound.radius );
#endif
	BaseEnt::draw();

}

bool Vertex::canUnSplit()
{
	// do the normal checks first, if this dosn't work,
	// there is no need to check further.
	if( BaseEnt::canUnSplit() == false )
		return false;

	return true;

}

// This checks our neighbors to see if they are keeping us from un-spliting.
bool Vertex::specialUnSplitCheck()
{
	// Macro to check a single vertex
	#define CHECK_VERT( n, vc ) \
		/* does his child keep us from spliting? */ \
		if( VERT(neighbor[n])->vChild[vc] && VERT(neighbor[n])->vChild[vc]->isSplit ) \
		{ \
			/* try to un-split the child, */ \
			VERT(neighbor[n])->vChild[vc]->recursiveUnSplit(); \
			/* if it didn't work, the special check bails out with false. */ \
			if( VERT(neighbor[n])->vChild[vc]->isSplit ) \
				return false; \
		}

	// Make a macro to check a corner vertex
	#define CHECK_CORNER( n, vc )	\
		/* neighbor 0 */  \
		if( neighbor[n] && neighbor[n]->isSplit ) \
		{ \
			CHECK_VERT( n, vc ) \
		}

	// Check a side. ( all three verticies )
	#define CHECK_SIDE( n, vc1, vc2, vc3 ) \
		if( neighbor[n] && neighbor[n]->isSplit )\
		{\
			if( VERT(neighbor[n])->vChild[vc1] ){ \
				CHECK_VERT( n, vc1 ) }\
			CHECK_VERT( n, vc2 )\
			if( VERT(neighbor[n])->vChild[vc3] ){ \
				CHECK_VERT( n, vc3 )}\
		}

	CHECK_CORNER( 0, 8 )
	CHECK_CORNER( 2, 6 )
	CHECK_CORNER( 6, 2 )
	CHECK_CORNER( 8, 0 )

	CHECK_SIDE( 1, 6, 7, 8 )
	CHECK_SIDE( 5, 0, 3, 6 )
	CHECK_SIDE( 7, 0, 1, 2 )
	CHECK_SIDE( 3, 2, 5, 8 )

	#undef CHECK_SIDE
	#undef CHECK_CORNER
	#undef CHECK_VERT
	return true;
}

void Vertex::destructor( bool willDelete )
{

	// delete our children, rules and parent's references to us
	BaseEnt::destructor( true );
	// clear our neighbor's pointers to me

	clearNeighborPointersToMe();

	// and finally, if we're not being deleted by a child class (if one
	// eventually exist) delete ourselves.
	if( !willDelete )
		reuse.tDelete( this );
}

void Vertex::clearNeighborPointersToMe()
{
	// TODO: this function could be optimized for speed by
	// hard coding all the checks into it.
	// loop through neighbors
	for( int n=0; n<9; n++ )
	{
		// if the neighbor exist and he's not our sibling
		if( neighbor[n] && neighbor[n]->parent != parent )
		{
			// loop through each of the neighbor's neighbors.
			for( int nn=0; nn<9; nn++ )
			{
				if( VERT(neighbor[n])->neighbor[nn] == this )
					VERT(neighbor[n])->neighbor[nn] = NULL;
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////// THE MESH STUFF ///////////////////////////////////////////////////


// unused, remove.
//int Vertex::drawTri( BaseEnt *b1, BaseEnt *b2, BaseEnt *b3, int triType )
//{
//	// This function draws the triangles in a triangle mesh recursively.
//	// It is passed three verticies.
//	// It is recursive, so if the verticies are split it will call itself
//	// for each smaller triangle.
//
//	// tritype is either LEFT or RIGHT. LEFT is the top-left oriented triangle,
//	// RIGHT is the bottom-right. This has to be specified because the child
//	// indexes are different for left and right triangles.
//
//	if( triType == LEFT )
//	{
//		drawLeft( b1, b2, b3 );
//	}
//	if( triType == RIGHT )
//	{
//		drawRight( b1, b2, b3 );
//	}
//	return 0;
//}

//#define SPLIT_COLORS

int Vertex::drawLeft( BaseEnt* b1, BaseEnt* b2, BaseEnt* b3 )
{
	// Make baseEnt pointers and cast the verticies to them. For future use.
	Vertex *v1, *v2, *v3;
	v1 = dynamic_cast< Vertex* >( b1 );
	v2 = dynamic_cast< Vertex* >( b2 );
	v3 = dynamic_cast< Vertex* >( b3 );

	// Note: there are 8 possible combinations of split and un-split vertexes.
	int combination;
	// Create an int with the correct combination number.
	combination = (int)v1->isSplit + (int)v2->isSplit*2 + (int)v3->isSplit*4;
	switch( combination )
	{
		case 0:
			// white
			#ifdef SPLIT_COLORS
			glColor3f( 1.0, 1.0, 1.0 );
			#endif
			glBegin( GL_TRIANGLES );
			v1->morphVertex.printVertex();
			v2->morphVertex.printVertex();
			v3->morphVertex.printVertex();
			glEnd();
		break;

		case 1:
			// red
			#ifdef SPLIT_COLORS
			glColor3f( 1.0, 0.0, 0.0 );
			#endif

			glBegin( GL_TRIANGLE_FAN );
				v1->vChild[2]->morphVertex.printVertex();
				v1->vChild[1]->morphVertex.printVertex();
				v2->morphVertex.printVertex();
				v3->morphVertex.printVertex();
				v1->vChild[2]->morphVertex.printVertex();
			glEnd();
			drawLeft( v1->vChild[4], v1->vChild[1], v1->vChild[2] );
		break;

		case 2:
			// orange
			#ifdef SPLIT_COLORS
			glColor3f( 1.0, 0.5, 0.0 );
			#endif

			glBegin( GL_TRIANGLE_FAN );
				v2->vChild[8]->morphVertex.printVertex();
				v2->vChild[5]->morphVertex.printVertex();
				v3->morphVertex.printVertex();
				v1->morphVertex.printVertex();
				v2->vChild[7]->morphVertex.printVertex();
				v2->vChild[5]->morphVertex.printVertex();
			glEnd();

			// call the recursive function
			drawLeft( v2->vChild[7], v2->vChild[4], v2->vChild[5] );
		break;

		case 3:
			// yellow
			#ifdef SPLIT_COLORS
			glColor3f( 1.0, 1.0, 0.0 );
			#endif

			glBegin( GL_TRIANGLE_FAN );
				v2->vChild[8]->morphVertex.printVertex();
				v2->vChild[5]->morphVertex.printVertex();
				v3->morphVertex.printVertex();
				v1->vChild[2]->morphVertex.printVertex();
				v2->vChild[8]->morphVertex.printVertex();
			glEnd();

			drawLeft( v1->vChild[4], v1->vChild[1], v1->vChild[2] );
			drawLeft( v1->vChild[1], v2->vChild[7], v2->vChild[8] );
			drawLeft( v2->vChild[7], v2->vChild[4], v2->vChild[5] );

			drawRight( v2->vChild[8], v1->vChild[2], v1->vChild[1] );
			drawRight( v2->vChild[5], v2->vChild[8], v2->vChild[7] );
		break;

		case 4:
			// green
			#ifdef SPLIT_COLORS
			glColor3f( 0.0, 1.0, 0.0 );
			#endif

			glBegin( GL_TRIANGLE_FAN );
				v3->vChild[6]->morphVertex.printVertex();
				v1->morphVertex.printVertex();
				v2->morphVertex.printVertex();
				v3->vChild[3]->morphVertex.printVertex();
				v3->vChild[6]->morphVertex.printVertex();
			glEnd();
			drawLeft( v3->vChild[6], v3->vChild[3], v3->vChild[4] );
		break;

		case 5:
			// blue
			#ifdef SPLIT_COLORS
			glColor3f( 0.0, 0.0, 1.0 );
			#endif

			glBegin( GL_TRIANGLE_FAN );
				v2->morphVertex.printVertex();
				v3->vChild[3]->morphVertex.printVertex();
				v3->vChild[6]->morphVertex.printVertex();
				v1->vChild[2]->morphVertex.printVertex();
				v1->vChild[1]->morphVertex.printVertex();
			glEnd();
			drawLeft( v1->vChild[4], v1->vChild[1], v1->vChild[2] );
			drawLeft( v3->vChild[6], v3->vChild[3], v3->vChild[4] );
		break;

		case 6:
			// purple
			#ifdef SPLIT_COLORS
			glColor3f( 0.7, 0.1, 1.0 );
			#endif

			glBegin( GL_TRIANGLE_FAN );
				v2->vChild[8]->morphVertex.printVertex();
				v3->vChild[6]->morphVertex.printVertex();
				v1->morphVertex.printVertex();
				v2->vChild[7]->morphVertex.printVertex();
				v2->vChild[8]->morphVertex.printVertex();
			glEnd();

			drawLeft( v2->vChild[7], v2->vChild[4], v2->vChild[5] );
			drawLeft( v2->vChild[8], v2->vChild[5], v3->vChild[3] );
			drawLeft( v3->vChild[6], v3->vChild[3], v3->vChild[4] );

			drawRight( v2->vChild[5], v2->vChild[8], v2->vChild[7] );
			drawRight( v3->vChild[3], v3->vChild[6], v2->vChild[8] );
		break;

		case 7:
			// None are drawn here, call the recursive functions!
			// draw the left ones
			drawLeft( v1->vChild[4], v1->vChild[1], v1->vChild[2] );
			drawLeft( v1->vChild[1], v2->vChild[7], v2->vChild[8] );
			drawLeft( v2->vChild[7], v2->vChild[4], v2->vChild[5] );
			drawLeft( v1->vChild[2], v2->vChild[8], v3->vChild[6] );
			drawLeft( v2->vChild[8], v2->vChild[5], v3->vChild[3] );
			drawLeft( v3->vChild[6], v3->vChild[3], v3->vChild[4] );

			// draw the right ones
			drawRight( v2->vChild[8], v1->vChild[2], v1->vChild[1] );
			drawRight( v2->vChild[5], v2->vChild[8], v2->vChild[7] );
			drawRight( v3->vChild[3], v3->vChild[6], v2->vChild[8] );

		break;

		default:
			cout << "Combination chooser is broken! Switch statement defaulted!" << endl;
			LN
			break;
	}

	return 0;
}

int Vertex::drawRight( BaseEnt* b1, BaseEnt* b2, BaseEnt* b3 )
{
	// Make baseEnt pointers and cast the verticies to them. For future use.
	Vertex *v1, *v2, *v3;
	v1 = dynamic_cast< Vertex* >( b1 );
	v2 = dynamic_cast< Vertex* >( b2 );
	v3 = dynamic_cast< Vertex* >( b3 );

	// Note: there are 8 possible combinations of split and un-split vertexes.
	int combination;
	// Create an int with the correct combination number.
	combination = (int)v1->isSplit + (int)v2->isSplit*2 + (int)v3->isSplit*4;

	switch( combination )
	{
	case 0:
			// white
			#ifdef SPLIT_COLORS
			glColor3f( 1.0 / 2.0, 1.0 / 2.0, 1.0 / 2.0 );
			#endif

			glBegin( GL_TRIANGLES );
			v1->morphVertex.printVertex();
			v2->morphVertex.printVertex();
			v3->morphVertex.printVertex();
			glEnd();
		break;

		case 1:
			// red
			#ifdef SPLIT_COLORS
			glColor3f( 1.0 / 2.0, 0.0 / 2.0, 0.0 / 2.0 );
			#endif

			glBegin( GL_TRIANGLE_FAN );
				v1->vChild[6]->morphVertex.printVertex();
				v1->vChild[7]->morphVertex.printVertex();
				v2->morphVertex.printVertex();
				v3->morphVertex.printVertex();
				v1->vChild[6]->morphVertex.printVertex();
			glEnd();
			drawRight( v1->vChild[4], v1->vChild[7], v1->vChild[6] );
		break;

		case 2:
			// orange
			#ifdef SPLIT_COLORS
			glColor3f( 1.0 / 2.0, 0.5 / 2.0, 0.0 / 2.0 );
			#endif

			glBegin( GL_TRIANGLE_FAN );
				v2->vChild[0]->morphVertex.printVertex();
				v2->vChild[3]->morphVertex.printVertex();
				v3->morphVertex.printVertex();
				v1->morphVertex.printVertex();
				v2->vChild[1]->morphVertex.printVertex();
				v2->vChild[3]->morphVertex.printVertex();
			glEnd();

			// call the recursive function
			drawRight( v2->vChild[1], v2->vChild[4], v2->vChild[3] );
		break;

		case 3:
			// yellow
			#ifdef SPLIT_COLORS
			glColor3f( 1.0 / 2.0, 1.0 / 2.0, 0.0 / 2.0 );
			#endif

			glBegin( GL_TRIANGLE_FAN );
				v2->vChild[0]->morphVertex.printVertex();
				v2->vChild[3]->morphVertex.printVertex();
				v3->morphVertex.printVertex();
				v1->vChild[6]->morphVertex.printVertex();
				v2->vChild[0]->morphVertex.printVertex();
			glEnd();

			drawRight( v1->vChild[4], v1->vChild[7], v1->vChild[6] );
			drawRight( v1->vChild[7], v2->vChild[1], v2->vChild[0] );
			drawRight( v2->vChild[1], v2->vChild[4], v2->vChild[3] );

			drawLeft( v2->vChild[0], v1->vChild[6], v1->vChild[7] );
			drawLeft( v2->vChild[3], v2->vChild[0], v2->vChild[1] );
		break;

		case 4:
			// green
			#ifdef SPLIT_COLORS
			glColor3f( 0.0 / 2.0, 1.0 / 2.0, 0.0 / 2.0 );
			#endif

			glBegin( GL_TRIANGLE_FAN );
				v3->vChild[2]->morphVertex.printVertex();
				v1->morphVertex.printVertex();
				v2->morphVertex.printVertex();
				v3->vChild[5]->morphVertex.printVertex();
				v3->vChild[2]->morphVertex.printVertex();
			glEnd();
			drawRight( v3->vChild[2], v3->vChild[5], v3->vChild[4] );
		break;

		case 5:
			// blue
			#ifdef SPLIT_COLORS
			glColor3f( 0.0 / 2.0, 0.0 / 2.0, 1.0 / 2.0 );
			#endif

			glBegin( GL_TRIANGLE_FAN );
				v2->morphVertex.printVertex();
				v3->vChild[5]->morphVertex.printVertex();
				v3->vChild[2]->morphVertex.printVertex();
				v1->vChild[6]->morphVertex.printVertex();
				v1->vChild[7]->morphVertex.printVertex();
			glEnd();
			drawRight( v1->vChild[4], v1->vChild[7], v1->vChild[6] );
			drawRight( v3->vChild[2], v3->vChild[5], v3->vChild[4] );
		break;

		case 6:
			// purple
			#ifdef SPLIT_COLORS
			glColor3f( 0.0 / 7.0, 0.0 / 2.0, 1.0 / 2.0 );
			#endif

			glBegin( GL_TRIANGLE_FAN );
				v2->vChild[0]->morphVertex.printVertex();
				v3->vChild[2]->morphVertex.printVertex();
				v1->morphVertex.printVertex();
				v2->vChild[1]->morphVertex.printVertex();
				v2->vChild[0]->morphVertex.printVertex();
			glEnd();

			drawRight( v3->vChild[2], v3->vChild[5], v3->vChild[4] );
			drawRight( v2->vChild[0], v2->vChild[3], v3->vChild[5] );
			drawRight( v2->vChild[1], v2->vChild[4], v2->vChild[3] );

			drawLeft( v3->vChild[5], v3->vChild[2], v2->vChild[0] );
			drawLeft( v2->vChild[3], v2->vChild[0], v2->vChild[1] );
		break;

		case 7:
			// None are drawn here, call the recursive functions!
			// draw the left ones
			drawRight( v1->vChild[4], v1->vChild[7], v1->vChild[6] );
			drawRight( v1->vChild[7], v2->vChild[1], v2->vChild[0] );
			drawRight( v2->vChild[1], v2->vChild[4], v2->vChild[3] );
			drawRight( v1->vChild[6], v2->vChild[0], v3->vChild[2] );
			drawRight( v2->vChild[0], v2->vChild[3], v3->vChild[5] );
			drawRight( v3->vChild[2], v3->vChild[5], v3->vChild[4] );

			// draw the right ones
			drawLeft( v2->vChild[0], v1->vChild[6], v1->vChild[7] );
			drawLeft( v2->vChild[3], v2->vChild[0], v2->vChild[1] );
			drawLeft( v3->vChild[5], v3->vChild[2], v2->vChild[0] );
		break;

		default:
			cout << "Combination chooser is broken! Switch statement defaulted!" << endl;
			LN
			break;
	}
	return 0;
}

#endif
