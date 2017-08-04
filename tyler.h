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
string indent( int num );
string pBool( bool inVal );
void aBool( string val, bool& b );

// some debug stuff
// output the file and Line Number
#define LN cout << " " << __FILE__ << " " << __LINE__ << endl;
#define WAIT( x ) double a=getElapsedTime(); while( getElapsedTime() < a+x ){}


template <class T>
int normalize(T &x, T &y, T &z)
{
	T length = sqrt(x*x + y*y + z*z);
	// if the length of each side is 0, its going to mess up anyway.
	if( !length )
	{
		length = 1.0;
		//cout << __FILE__ << " " << __LINE__ << " normalize calculation messed up." << endl;
		return 0;
	}
    T oneOverL = 1.0 / length;
	x = (x * oneOverL);
	y = (y * oneOverL);
	z = (z * oneOverL);
	return 1;
} //normalize

template <class T>
T clamp(T &in, T low, T high){
    if(in < low)
        in = low;
    else if(in > high)
        in = high;
}

template <class I, class O>
void calcNormal(I x1, I y1, I z1, I x2, I y2, I z2,
				I x3, I y3, I z3, O &nx, O &ny, O &nz)
{
	I ax, ay, az, bx, by, bz;
	ax = x1 - x2;
	ay = y1 - y2;
	az = z1 - z2;

	bx = x1 - x3;
	by = y1 - y3;
	bz = z1 - z3;

	nx = (O)(ay*bz-by*az);
	ny = (O)(-ax*bz+bx*az);
	nz = (O)(ax*by-bx*ay);
	normalize( nx, ny, nz );
} // calcNormal


// storing of colors
class Color
{
	public:
	float red, green, blue;
	Color();
    Color(float r, float g, float b);
    string toString(int depth);
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


// abstract in case we want to use a different algorithm later.
inline void tSrand( int seed )
{
	srand( seed );
}

// a speedy assert. This assert can be removed from production code.
//#define sassert( val ) assert( val );
// seg fault if assert fails. then bt in dgb will work.
#define sassert( val ) if( !(val) ){ cout << "ASSERTION FAILED " << endl;LN int* a=NULL; *a=0; }

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

