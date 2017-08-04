#ifndef _TYLER_CPP
#define _TYLER_CPP

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <string>
#include "tyler.h"

using namespace std;



// indent - pass an int, it returns a string of that many indents
string indent( int num )
{
	string s;
	for( int i=0; i<num; i++ )
		{ s += "."; }
	return s;
}

// PrintBool Pass a bool to this function before printing it
string pBool( bool inVal )
{
	if( inVal )
		return "true";
	else
		return "false";
}

// assign a string value to a bool
void aBool( string val, bool& b )
{
	if( val == "" || val == "0" || val == "false" )
		b = false;
	else
		b = true;
}


Color::Color()
{
	red = green = blue = 0.0;
}


// THIS STUFF WOULD GO IN THE .CPP FILE
Point3D::Point3D()
{
	x = 0;
	y = 0;
	z = 0;
}

Point3D::Point3D( double ix, double iy, double iz )
{
	x = ix;
	y = iy;
	z = iz;
}





void calcNormal(double x1, double y1, double z1, double x2, double y2, double z2,
				double x3, double y3, double z3, double &nx, double &ny, double &nz)
{
	double ax, ay, az, bx, by, bz;
	ax = x1 - x2;
	ay = y1 - y2;
	az = z1 - z2;

	bx = x1 - x3;
	by = y1 - y3;
	bz = z1 - z3;

	nx = ay*bz-by*az;
	ny = -ax*bz+bx*az;
	nz = ax*by-bx*ay;
	normalize( nx, ny, nz );

} // calcNormal


// For debug purposes
#include <ctime>

void pause( int x ){
	double startPauseTime=time( 0 );
	while( time( 0 ) < startPauseTime + x ){}
}


#endif
