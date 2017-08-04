#ifndef CAMERA_CPP
#define CAMERA_CPP

#include <sstream>
#include <math.h>
#include "glut.h"
#include "camera.h"
#include "fland.h"
#include "main.h"
#include "tyler.h"

using namespace std;

void Camera::setNearFarPlanes( double nearest, double farthest )
{
	//far = farthest * 1.00001;
	near = nearest * cos( ( viewAngle * PI_D_180 ) ) * 0.9999;
	far = near * farPercentage;
}

string Camera::toString( int depth = 0 )
{
	string ind = indent( depth );
	ostringstream out;
	out << ind << "Camera::toString()";
	out
		<< ind << "\nx " << x
		<< ind << "\ny " << y
		<< ind << "\nz " << z
		<< ind << "\nrotX " << rotX
		<< ind << "\nroty " << rotY
		<< ind << "\nrotZ " << rotZ
		<< ind << "\nviewAngle " << viewAngle
		<< ind << "\nnear " << near
		<< ind << "\nfar " << far
		<< ind << "\nspeed " << speed
		<< ind << "\nvertSpeed " << vertSpeed
		<< ind << "\nsideSpeed " << sideSpeed
		<< ind << endl;

	out << "\n\n";

	//for( int i = 0; i<4; i++ )
	//{
	//	for( int j=0; j<4; j++ )
	//	{
	//		out << ind << "\tFrustum[" << i << "][" << j << "] " << frustum[i][j] << endl;
	//	}
	//}

	#if( 0 )
	// print out the frustum stuff
   double   proj[16];
   double   modl[16];
   double   clip[16];
   double   t;    /* Get the current PROJECTION matrix from OpenGL */
   glGetDoublev( GL_PROJECTION_MATRIX, proj );    /* Get the current MODELVIEW matrix from OpenGL */
   glGetDoublev( GL_MODELVIEW_MATRIX, modl );    /* Combine the two matrices (multiply projection by modelview) */

	for( int i=0; i<16; i++ )
	{
		out << ind << "Projection[" << i << "] " << proj[i] << endl;
	}
	for( int i=0; i<16; i++ )
	{
		out << ind << "Modelview[" << i << "] " << modl[i] << endl;
	}
	#endif
}


// this function was written By Mark Morley, December 2000. markmorley.com
// THANKS MARK!!! YOU DA MAN!!!
// I (Tyler Patterson) changed the data types from "float" to "double".

  //A plane can be defined with four numbers: A, B, and C define the plane's
  //normal, and D defines its distance from the origin.

void Camera::calcFrustum()
{
   double   proj[16];  
   double   modl[16];
   double   clip[16];
   double   t;    /* Get the current PROJECTION matrix from OpenGL */
   glGetDoublev( GL_PROJECTION_MATRIX, proj );    /* Get the current MODELVIEW matrix from OpenGL */
   glGetDoublev( GL_MODELVIEW_MATRIX, modl );    /* Combine the two matrices (multiply projection by modelview) */

   clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
   clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
   clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
   clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];    clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
   clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
   clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
   clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];    clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
   clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
   clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
   clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];    clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
   clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
   clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
   clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];    /* Extract the numbers for the RIGHT plane */
   frustum[0][0] = clip[ 3] - clip[ 0];
   frustum[0][1] = clip[ 7] - clip[ 4];
   frustum[0][2] = clip[11] - clip[ 8];
   frustum[0][3] = clip[15] - clip[12];    /* Normalize the result */
   t = sqrt( frustum[0][0] * frustum[0][0] + frustum[0][1] * frustum[0][1] + frustum[0][2] * frustum[0][2] );
   frustum[0][0] /= t;
   frustum[0][1] /= t;
   frustum[0][2] /= t;
   frustum[0][3] /= t;    /* Extract the numbers for the LEFT plane */
   frustum[1][0] = clip[ 3] + clip[ 0];
   frustum[1][1] = clip[ 7] + clip[ 4];
   frustum[1][2] = clip[11] + clip[ 8];
   frustum[1][3] = clip[15] + clip[12];    /* Normalize the result */
   t = sqrt( frustum[1][0] * frustum[1][0] + frustum[1][1] * frustum[1][1] + frustum[1][2] * frustum[1][2] );
   frustum[1][0] /= t;
   frustum[1][1] /= t;
   frustum[1][2] /= t;
   frustum[1][3] /= t;    /* Extract the BOTTOM plane */
   frustum[2][0] = clip[ 3] + clip[ 1];
   frustum[2][1] = clip[ 7] + clip[ 5];
   frustum[2][2] = clip[11] + clip[ 9];
   frustum[2][3] = clip[15] + clip[13];    /* Normalize the result */
   t = sqrt( frustum[2][0] * frustum[2][0] + frustum[2][1] * frustum[2][1] + frustum[2][2] * frustum[2][2] );
   frustum[2][0] /= t;
   frustum[2][1] /= t;
   frustum[2][2] /= t;
   frustum[2][3] /= t;    /* Extract the TOP plane */
   frustum[3][0] = clip[ 3] - clip[ 1];
   frustum[3][1] = clip[ 7] - clip[ 5];
   frustum[3][2] = clip[11] - clip[ 9];
   frustum[3][3] = clip[15] - clip[13];    /* Normalize the result */
   t = sqrt( frustum[3][0] * frustum[3][0] + frustum[3][1] * frustum[3][1] + frustum[3][2] * frustum[3][2] );
   frustum[3][0] /= t;
   frustum[3][1] /= t;
   frustum[3][2] /= t;
   frustum[3][3] /= t;    /* Extract the FAR plane */
   frustum[4][0] = clip[ 3] - clip[ 2];
   frustum[4][1] = clip[ 7] - clip[ 6];
   frustum[4][2] = clip[11] - clip[10];
   frustum[4][3] = clip[15] - clip[14];    /* Normalize the result */
   t = sqrt( frustum[4][0] * frustum[4][0] + frustum[4][1] * frustum[4][1] + frustum[4][2] * frustum[4][2] );
   frustum[4][0] /= t;
   frustum[4][1] /= t;
   frustum[4][2] /= t;
   frustum[4][3] /= t;    /* Extract the NEAR plane */
   frustum[5][0] = clip[ 3] + clip[ 2];
   frustum[5][1] = clip[ 7] + clip[ 6];
   frustum[5][2] = clip[11] + clip[10];
   frustum[5][3] = clip[15] + clip[14];    /* Normalize the result */
   t = sqrt( frustum[5][0] * frustum[5][0] + frustum[5][1] * frustum[5][1] + frustum[5][2] * frustum[5][2] );
   frustum[5][0] /= t;
   frustum[5][1] /= t;
   frustum[5][2] /= t;
   frustum[5][3] /= t;
}

bool Camera::pointInFrustum( double inX, double inY, double inZ )
{
	// dont check for the near or far clip planes.
   for( int p = 0; p < 4; ++p )
      if( frustum[p][0] * inX + frustum[p][1] * inY + frustum[p][2] * inZ + frustum[p][3] <= 0.0 )
         return false;
   return true;
}

void Camera::updateLocation()
{
	Fland *fl = getFlandPtr();

	// slow down the camera
	double speedBreak = CAMERA_SPEED_BREAK * fl->env.timeSlice;
	speed /= ( 1.0 + speedBreak );
	vertSpeed /= ( 1.0 + speedBreak );
	sideSpeed /= ( 1.0 + speedBreak );

	// scale the speed to the timeslice
	double thisSpeed = speed * fl->env.timeSlice;
	double thisVertSpeed = vertSpeed * fl->env.timeSlice;
	double thisSideSpeed = sideSpeed * fl->env.timeSlice;

	// scale the speed to the nearest rendered object
	thisSpeed *= fl->closestRenderDistance;
	thisVertSpeed *= fl->closestRenderDistance;
	thisSideSpeed *= fl->closestRenderDistance;

	double rx = rotX * PI_D_180;
	//double ry = rotY * PI_D_180;
	double ry = rotY * PI_D_180;
	x += thisSpeed * cos( rx ) * sin( ry );
	y -= thisSpeed * sin( rx );
	z -= thisSpeed * cos( rx ) * cos( ry ); // subtract from Z because the Z axis points at your face initally

	// vertical movement
	x += thisVertSpeed * sin( rx ) * sin( ry );
	y += thisVertSpeed * cos( rx );
	z -= thisVertSpeed * sin( rx ) * cos( ry );

	// Side to side movement
	x -= thisSideSpeed * cos( ry );
	//y += thisSideSpeed * cos( rz + PI / 2.0 );
	z -= thisSideSpeed * sin( ry );

	updateGlView();


	//cout << x << "\n" << y << "\n" << z << "\n";
	//cout << "\t" << rotX << "\n\t" << rotY << "\n" << endl;
}

// opengL uses the RIGHT HAND RULE for rotation
void Camera::updateGlView()
{
	glLoadIdentity();

	//glRotatef(rotZ, 0.0, 0.0, 1.0);
	glRotatef(rotX, 1.0, 0.0, 0.0);
	glRotatef(rotY, 0.0, 1.0, 0.0);



	//glPushMatrix();
	//glTranslatef(x, y, z);
	GLfloat nDis = near + ((far - near)*0.1);
	GLfloat fDis = far;
	glFogf( GL_FOG_START, nDis );
	glFogf( GL_FOG_END, fDis );
	glFogf( GL_FOG_DENSITY, 2.0 / fDis );
	glFogfv( GL_FOG_COLOR, fogColor );
	glClearColor( fogColor[0], fogColor[1], fogColor[2], fogColor[3] );
	calcFrustum();
	//glPopMatrix();


	//printFrustum();
	//glFrustum(-1.0, 1.0, -h, h, 3.0, 600.0);
}


inline double Camera::calcVisibility( double inX, double inY, double inZ )
{
	return distance( inX, x, inY, y, inZ, z );
}

Camera::Camera()
{
	resetView();
}

void Camera::resetView()
{
	x = y = z = rotX = rotZ = speed = 0.0;
	farPercentage = 200;
	viewAngle = 1.5;
	near = 0.0001; // just for now
	far = 600; // ditto
	speed = 0.0;
	vertSpeed = 0;
	sideSpeed = 0; 
//	rotX = 2.1;
	rotZ = 0.0;
	rotY = 44.0;
//	x = 0.0;
	y = 0.1;
//	z = 2.0;
	fogColor[0] =  0.3; // red
	fogColor[1] =  0.85; // green
	fogColor[2] =  0.85; // blue
	fogColor[3] =  1.0; // alpha?
}

void Camera::setNearFarPlanes( )
{
	Fland* fl = getFlandPtr();
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //glFrustum(-1.0, 1.0, -env.screenHeight, env.screenHeight, near, far);

  //cout << "x: " << x << " y: " << y << " z: " << z;
  //cout << "\trotX: " << rotX << " rotY: " << rotY << " rotZ: " << rotZ << endl;

  gluPerspective(viewAngle, (GLdouble)fl->env.screenWidth / (GLdouble)fl->env.screenHeight, near, far );
  glMatrixMode(GL_MODELVIEW);
  //glTranslatef( 0, 0, -0.2 );
}

void Camera::printFrustum()
{
	Fland* fl = getFlandPtr();
	sassert( fl );
	glMatrixMode(GL_PROJECTION);
	gluPerspective(viewAngle, (GLdouble)fl->env.screenWidth/(GLdouble)fl->env.screenHeight, near, far );
	glMatrixMode(GL_MODELVIEW);
}



#endif
