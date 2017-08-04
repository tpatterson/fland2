// Render Vertex. An object that is drawn onscreen.
#ifndef RENDER_VERTEX_CPP
#define RENDER_VERTEX_CPP

using namespace std;

#include <GL/glut.h>
#include <math.h>
#include <iomanip>
#include <sstream>
#include <math.h>
#include "tyler.h"
#include "camera.h"
#include "fland.h"
#include "rendervertex.h"


BoundSphere::BoundSphere()
{
	initalize();
}

void RenderVertex::printVertex()
{
	Camera *cam = getFlandPtr()->camera;
	glColor3f( red, green, blue );
	glNormal3f( nx, ny, nz );
	glVertex3d( x - cam->x, y - cam->y, z - cam->z );

	//if( abs(x) > .7 || abs(z) > .7 )
	//{
	//	cout << toString(0);LN
	//}
}

string RenderVertex::toString( int depth )
{
	ostringstream out;
	string ind = indent( depth );
		//cout.setf( ios::fixed );
		out
		<< ind << "  RenderVertex::"
		<< ind << "\tx: " << x
		<< ind << "\ty: " << y
		<< ind << "\tz: " << z
		//<< ind << "\n\t  nx      " << nx
		//<< ind << "\n\t  ny      " << ny
		//<< ind << "\n\t  nz      " << nz
		//<< ind << "\n\t  red     " << red
		//<< ind << "\n\t  green   " << green
		//<< ind << "\n\t  blue    " << blue
		//<< ind << endl
		;
	return out.str();
}

RenderVertex::RenderVertex()
{
	initalize();
};

//void RenderVertex::operator=( const RenderVertex& ir )
//{
//	x = ir.x;
//	y = ir.y;
//	z = ir.z;
//	red = ir.red;
//	green = ir.green;
//	blue = ir.blue;
//	nx = ir.nx;
//	ny = ir.ny;
//	nz = ir.nz;
//
//	if( abs( x ) > 0.7 || abs( z ) > 0.7 )
//		{ cout << toString(0);LN }
//}



#endif
