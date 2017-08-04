
#ifndef MonoRule_CPP // TODO: insert class name before _CPP
#define MonoRule_CPP
/*
	MonoRule
	- Applies to:
		eg. Verticies

	- Description:

	- Values it can recieve

	- How it works

*/

#include <iostream>
#include <sstream>
#include "tyler.h"
#include "monor.h"
#include "templates.h"
#include "baserule.h"

#if( DEBUG )
string MonoRule::toString( int i )
{
	ostringstream out;
	ind = indent( i );
	out << ind << " MonoRule::\n";
	out << BaseRule::toString( i );
	return out.str();
}
#endif

// mention the reuse rule class
Reuse< BaseRule, MonoRule > MonoRule::reuse;


string MonoRule::assignValue( string valueType, istringstream *value, FmlParser* fPtr )
{
	if( valueType == "test1" )
	{
		// *value >> test1;
	}
	else
	{
		return BaseRule::assignValue( valueType, value, fPtr );
	}
	return "";
}

MonoRule::MonoRule()
	:BaseRule();
{
    average_temp = 0.25;
    rainfall_inches = 20.0;
    rockiness = 0.2;
    sandiness = 0.2;
    topsoil_depth = 0.1;
}

void MonoRule::destructor( bool willDelete )
{

	BaseRule::destructor( willDelete );
}

void MonoRule::goCalcMe( BaseEnt *inClient, intensityType runIntensity )
{
	cout << "need to overload goCalcMe still! "; LN
}

#endif
