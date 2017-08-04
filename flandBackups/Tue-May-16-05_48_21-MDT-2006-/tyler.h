#ifndef _TYLER_H
#define _TYLER_H

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <string>
#include <cassert>

using namespace std;

const double pi = 3.14159265;
const double PI = pi;
const double PI2 = PI * 2.0;
const double PI_D_2 = PI / 2.0;
const double PI_D_180 = PI / 180.0;

inline double distance( double x1, double x2, double y1, double y2, double z1, double z2 );
inline double distance( double x, double y, double z );
int normalize(double &x, double &y, double &z);
void calcNormal(double x1, double y1, double z1, double x2, double y2, double z2,
				double x3, double y3, double z3, double &nx, double &ny, double &nz);
string indent( int num );
string pBool( bool inVal );
void aBool( string val, bool& b );

// storing of colors
class Color
{
	public:
	float red, green, blue;
	Color();
};

// storing 3d points
class Point3D
{
	public:
	double x, y, z;
	Point3D();
	Point3D( double ix, double iy, double iz );
};

inline int tRand();
inline void tSrand( int seed );
inline double rnd();
void pause( int x );


inline double rnd()
{
	return( (double)tRand() / (double)RAND_MAX );
}
// Abstract in case we want to use a different algorithm later.
inline int tRand()
{
	return( rand() );
}



inline double distance( double x, double y, double z )
{
	return( sqrt( x*x + y*y + z*z ) );
}

inline double distance( double x1, double x2, double y1, double y2, double z1, double z2 )
{
	return( distance( x1 - x2, y1 - y2, z1 - z2 ) );
}

inline int normalize(double &x, double &y, double &z)
{
	double length = sqrt(x*x + y*y + z*z);
	// if the length of each side is 0, its going to mess up anyway.
	if( !length )
	{
		length = 1.0;
		//cout << __FILE__ << " " << __LINE__ << " normalize calculation messed up." << endl;
		return 0;
	}
	x = (x / length);
	y = (y / length);
	z = (z / length);
	return 1;
} //normalize


// abstract in case we want to use a different algorithm later.
inline void tSrand( int seed )
{
	srand( seed );
}

// a speedy assert. This assert can be removed from production code.
//#define sassert( val ) assert( val );
// seg fault if assert fails. then bt in dgb will work.
#define sassert( val ) if( !(val) ){ cout << "ASSERTION FAILED ";LN int* a=NULL; *a=0; }

// NOTE: T has to be a float or double, if an int this function wont work.
template< class T >
T convertRange1ToRange2( T range1, T inMin, T inMax, T outMin, T outMax )
{
	// clamp the value
	if( range1 <= inMin )
		return outMin;
	else if( range1 >= inMax )
		return outMax;

	T sizeIn = inMax - inMin;
	T sizeOut = outMax - outMin;
	T outRatio = sizeOut / sizeIn;

	return( ( range1 - inMin ) * outRatio + outMin );
}

#endif

