// Render Vertex. An object that is drawn onscreen.
#ifndef RENDER_VERTEX_H
#define RENDER_VERTEX_H

using namespace std;

#include <string>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "defines.h"
#include "camera.h"
#include "fland.h"

class BoundSphere
{
	public:
	QUARD_TYPE x, y, z;

	// a bounding sphere, this is the radius. The center is the location, above.
	DIS_TYPE radius;
	inline void initalize();
	BoundSphere();
};
inline void BoundSphere::initalize()
{
	x = y = z = 0.0;
	radius = 1.0;
}

class RenderVertex
{
	public:
	GLfloat x, y, z;
	GLfloat red, green, blue;
	GLfloat nx, ny, nz; // normals
	//bool isDeleted; // debug

	RenderVertex();
	//void operator=( const RenderVertex& );
	inline void initalize();

//	void printVertex();
	inline void interpolate( const RenderVertex &start, const RenderVertex &end, double amount);
	inline RenderVertex operator*( double );
	inline void operator+=( const RenderVertex& );
	inline void drawNormal( float );
	string toString( int = 0 );
};



inline void RenderVertex::interpolate( const RenderVertex &start, const RenderVertex &end, double amount )
{
	// set up an inverse amount.
	double invAmount = (double)1.0 - amount;

	x = start.x * invAmount + end.x * amount;
	y = start.y * invAmount + end.y * amount;
	z = start.z * invAmount + end.z * amount;

	nx = start.nx * invAmount + end.nx * amount;
	ny = start.ny * invAmount + end.ny * amount;
	nz = start.nz * invAmount + end.nz * amount;

	red = start.red * invAmount + end.red * amount;
	blue = start.blue * invAmount + end.blue * amount;
	green = start.green * invAmount + end.green * amount;
}

inline void RenderVertex::initalize()
{
	x = y = z = 0.0;
	nx = ny = nz = 0.0;
	red = green = blue = 0.0;
}

inline void RenderVertex::drawNormal( float length )
{
	glNormal3f( 0.0, 0.0, 0.0 );
	glColor3f( 1.0, 1.0, 1.0 );
	glBegin( GL_LINES );
		glVertex3f( x, y, z );
		glVertex3f( x+(nx*length), y+(ny*length), z+(nz*length) );
	glEnd();
}

inline void RenderVertex::operator+=( const RenderVertex &addVert )
{
	x += addVert.x;
	y += addVert.y;
	z += addVert.z;

	nx += addVert.nx;
	ny += addVert.ny;
	nz += addVert.nz;

	red += addVert.red;
	green += addVert.green;
	blue += addVert.blue;
}

inline RenderVertex RenderVertex::operator*( double mulBy )
{
	RenderVertex ret;
	ret.x = x * mulBy;
	ret.y = y * mulBy;
	ret.z = z * mulBy;

	ret.nx = nx * mulBy;
	ret.ny = ny * mulBy;
	ret.nz = nz * mulBy;

	ret.red = red * mulBy;
	ret.green = green * mulBy;
	ret.blue = blue * mulBy;

	return ret;
}


#endif

