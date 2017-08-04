#ifndef _TYLER_CPP
#define _TYLER_CPP

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <string>
#include "tyler.h"
#include <sstream>

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

Color::Color(float r, float g, float b)
{
    red = r;
    green = g;
    blue = b;
}

string Color::toString(int depth){
    ostringstream out;
    string ind = indent( depth );
    out
        << ind << "red      " << red << endl
        << ind << "green    " << green << endl
        << ind << "blue     " << blue << endl
    ;
    return out.str();
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

// For debug purposes
#include <ctime>

void pause( int x ){
	double startPauseTime=time( 0 );
	while( time( 0 ) < startPauseTime + x ){}
}


#endif
