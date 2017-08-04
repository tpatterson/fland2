#ifndef GROUPR_CPP
#define GROUPR_CPP


using namespace std;
#include <sstream>
#include <cassert>

#include "groupr.h"
#include "tyler.h"
#include "defines.h"
#include "fmlparser.h"
#include "baseent.h"


#if( DEBUG )
string GroupR::toString( int depth = 0 )
{
	string ind = indent( depth );
	ostringstream out;

	out
		<< ind << "(GroupR)" << endl
		<< BaseRule::toString( depth )
		<< ind << "GroupR::" << endl
		<< ind << "Size              " << this->size() << endl
		<< ind << "Members:" << endl
	;
	for( int i=0; i<this->size(); i++ )
	{
		out << groupMember[i]->toString( depth + 4 );
	}
	return out.str();
}


#endif


string GroupR::assignValue( string valueType, istringstream *value, FmlParser* fPtr )
{
	if( valueType == "groupmember" )
	{
		BaseRule* temp;
		temp = fPtr->nameMap[ value->str() ];
		addGroupMember( temp );
		//cout << fPtr->nameMap[ value->str() ];LN
	}
	else
	{
		return BaseRule::assignValue( valueType, value, fPtr );
	}
	return "";
}

void GroupR::addGroupMember( BaseRule* inPtr )
{
	addRule( inPtr );
	//push_back( inPtr );
}

// this function is called on a groupr when it's client is being split.
//It returns a pointer to the new rule that the new entity should have.
void GroupR::propogateChildren( BaseEnt* newEnt, intensityType inIntensity )
{
	if( childRule )
	{
		BaseRule::propogateChildren( newEnt, inIntensity );
	}
	else
	{
		// loop through each groupmember and insert her  or her child
		// depending on if the groupmember has a numPropogates > 0
		for( int i=0; i<size(); i++ )
		{
			groupMember[i]->propogateRules( newEnt, groupMember[i]->numPropogates, inIntensity );
		}
	}
}

void GroupR::destructor( bool willDelete )
{

	BaseRule::destructor( true );
}

void GroupR::goCalcMe( BaseEnt *inClient, intensityType runIntensity )
{
	sassert( inClient );

	sassert( (runIntensity >= 0.0 && runIntensity <= 1.01) );

	for( int i=0; i<size(); ++i )
	{
		groupMember[i]->goCalcMe( inClient, runIntensity * intensity );
	}
}


GroupR::GroupR():BaseRule()
{
	groupMember.clear();
}

#endif
