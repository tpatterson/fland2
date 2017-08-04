#ifndef CAMERA_H
#define CAMERA_H

#include <string>
#include "tyler.h"

// This is the data class Fland uses to keep track of the camera location,
// and to test if a point is visible to the camera.
class Camera
{
	public:
	double x, y, z, rotY, rotX, rotZ;
	double viewAngle;
	double near, far;
	double farPercentage;
	double speed; // forward and backward speed
	double vertSpeed; // up and down speed
	double sideSpeed; // side to side
   double frustum[6][4]; // this is calculated in calcFrustum. Used in frustrum culling.
   float fogColor[4];

	Camera();
	void updateGlView();
	void updateLocation();
	void calcFrustum();
	bool pointInFrustum( double inX, double inY, double inZ );
	template< class T >
	inline bool sphereInFrustum( T x, T y, T z, T radius );
	double calcVisibility( double, double, double );
	void setNearFarPlanes();
	void resetView();
	void printFrustum();
	string toString( int );
	void setNearFarPlanes( double nearest, double farthest );

};
template< class T >
inline bool Camera::sphereInFrustum( T inX, T inY, T inZ, T radius )
{
	inX -= x;
	inY -= y;
	inZ -= z;
	// Dont check for the near clip planes.
   for( int p = 0; p < 5; p++ )
      if( frustum[p][0] * inX + frustum[p][1] * inY + frustum[p][2] * inZ + frustum[p][3] <= -radius )
         return false;
   return true;
}

#endif
