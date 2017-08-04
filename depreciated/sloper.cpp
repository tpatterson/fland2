#ifndef SlopeR_CPP
#define SlopeR_CPP
/*
	SlopeR
	- Applies to:
		- Verticies

	- Description:
		- Applies different types of terrain to the vertex.

	- Values it can recieve
		- Two child rules, terrainType1 and terrainType2.
			These are input (for now) exactly the same way as groupmembers.
			They MUST be input in order, and MUST be the first (and only!) groupmembers.
			-There is no checking to enforce this at the moment, so be nice!
		- two threshold values.

	- How it works
		- If the slope of a given vertex falls below threshold1, it will be terrainType1
		- If it falls between the threshold values, it will contain terrainType 1 and 2,
			interpolated.
		- If if falls above threshold2 it will be terrainType2.

*/

using namespace std;

#include <sstream>
#include <iostream>
#include "tyler.h"
#include "groupr.h"
#include "vertex.h"
#include "templates.h"
#include "sloper.h"





#if( DEBUG )
string SlopeR::toString( int depth = 0 )
{
	ostringstream out;
	string ind = indent( depth );
	out << ind << " SlopeR::\n";
	out << GroupR::toString( depth );
	return out.str();
}

string SlopeR::getType(){ return "SlopeR"; }

#endif

string SlopeR::assignValue( string valueType, istringstream *value, FmlParser* fPtr )
{
	if( valueType == "threshold1" )
	{
		 *value >> threshold1;
	}
	else if( valueType == "threshold2" )
	{
		 *value >> threshold2;
	}
	else
	{
		// NOTE: the terraintype1 ane terrainType 2 vars input just like any child rule. (see text above)
		return GroupR::assignValue( valueType, value, fPtr );
	}
	return "";
}

void SlopeR::destructor( bool willDelete )
{

	GroupR::destructor( willDelete );
}

// This rule is overloaded simply so that GroupR::goCalcMe doesn't get called.
// That would be bad because it would call goCalcMe() on both childrules, and we dont
// want to do that.
void SlopeR::goCalcMe( BaseEnt *inClient, intensityType runIntensity )
{

}

#endif

