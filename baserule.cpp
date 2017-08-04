// Rules govern what an object's attributes will be, sort of like DNA.
// different types of rules inherit from BaseRule.
#ifndef BASE_RULE_CPP
#define BASE_RULE_CPP

using namespace std;

#include <sstream>
#include <cassert>
#include "tyler.h"
#include "baserule.h"
#include "fmlparser.h"
#include "baseent.h"

// The -- client -- is the entity that is using the rule.

#if( DEBUG )
string BaseRule::toString( int depth )
{

	ostringstream out;
	string ind = indent( depth );
	out
		<< ind << "BaseRule::\n"
		<< ind << "childRule         " << childRule << endl
		<< ind << "numPropogates     " << numPropogates << endl
		<< ind << "fmlName           " << this->fmlName << endl
		//<< ind << "fmlRule        " << pBool( fmlRule ) << endl
		<< ind << "this              " << this << endl
;
	return out.str();
}


#endif

BaseRule::BaseRule()
{
	childRule = NULL;
	numPropogates = 0;
	intensity = 1.0;
}

// every derrived class of baserule has an assignValue function.
// if the value is not assigned to something inside it's assignValue,
// it calles the BaseRule assignvalue, which returns an error.
/*
When an entity is being split,
All the entity's rules check themselves, but only one level deep. In a globe, For
example, the sphereRule will go for a certain number of iterations, and the
globeRule will for a different number. But, the globe Rule wont check any of it's
groupMembers.
*/
// The param *file is here so that when returning errors we can return where the error occured.
string BaseRule::assignValue( string valueType, istringstream *value, FmlParser *fPtr )
{
	if( valueType == "numiterations" || valueType == "numpropogates" )
	{
			*value >> numPropogates;
	}
	else if( valueType == "intensity" )
	{
		*value >> intensity;
	}
	else if( valueType == "childrule" )
	{
		BaseRule* inRule = fPtr->nameMap[ value->str() ];

		// Make sure the rule is legitimate. If not, quit now.
		if( !inRule )
		{
			ostringstream out;
			out << "Bad Rule Name \"" << value->str() << "\"";
			throw FmlParserFatalError( out.str() );

			return( "ERROR" );	// This currently doesn't work.
		}
		childRule = inRule;
	}
	else
	{
		return( "Bad Type: " + valueType + " = " + value->str() );
	}
	return "";
}


void BaseRule::propogateRules( BaseEnt* newEnt, int remainingPropogates, intensityType nIntensity )
{
	sassert( newEnt );

	if( remainingPropogates > 0 )
	{
		newEnt->addAndRunRule( this, remainingPropogates - 1, nIntensity );
	}
	else
	{
		propogateChildren( newEnt, nIntensity );
	}

}

// this is the generic propogateChild. GroupR and descriminatoryR have their own.
void BaseRule::propogateChildren( BaseEnt* newEnt, intensityType nIntensity )
{
	if( childRule )
	{
		// the rule will be set to propogate this many times.
		int numPropogates = childRule->numPropogates;
		newEnt->addAndRunRule( childRule, numPropogates, nIntensity );
	}
	else
	{
		// this is a case that happens often.
		// Should we leave the behavior to happen this way?
		//cout << "no childrule! ";LN
	}
}

void BaseRule::destructor( bool willDelete )
{

}

void BaseRule::goCalcMe( BaseEnt *inClient, intensityType runIntensity )
{
	LN
}


BaseRule* BaseRule::startUse()
{ return this; }

void BaseRule::endUse(){}


#endif




#if( 0 )

/*

 NOTE!!!
 To create a new rule class, do these steps

 - create a new rule file and copy and paste the below code into it.
 - do a search and replace SOMETHING for the rule name inside the file
 - in defines.h put a line in with the other classes for quick dynamic casting
	example: #define ROUGHR( x ) dynamic_cast< RoughR* >( x )
 - in includes.cpp insert a line in the three places (near the end of the file)
 - in fmlparser.cpp function evaluateRule, insert code to create your class
 - IF your rule will extend anything other then BaseRule (for example GroupR), then do a search for
 	GroupR (or something else) in the code and put the parent's rules name where it applies.
 - Edit your new rule and start using it!
*/

// COPY AND PASTE THIS CODE WHEN CREATING A NEW RULE.
// Start copying here...

#ifndef SOMETHING_CPP // TODO: insert class name before _CPP
#define SOMETHING_CPP
/*
	SOMETHING
	- Applies to:
		eg. Verticies

	- Description:

	- Values it can recieve

	- How it works

*/

#include <iostream>
#include <sstream>
#include "tyler.h"
#include "templates.h"
#include "baserule.h"

class SOMETHING : public BaseRule
{
	public:
	SOMETHING();
	virtual SOMETHING();

	virtual void destructor( bool willDelete = false );

	// This makes the rule do it's thing. Call it and pass the entity that
	// is using the rule (client), and the rule will alter the appropriate attributes
	// of the client.
	virtual void goCalcMe( BaseEnt *inClient, intensityType runIntensity );

	// used in the parse functions. assigns value to valuetype.
	// returns an error, if there was one. Otherwise, returns blank ( "" )
	virtual string assignValue( string valueType, istringstream *value, FmlParser* fPtr );

	#if( DEBUG )
	virtual string toString( int );
	virtual string getType(){ return "SOMETHING"; }
	#endif
	static Reuse< BaseRule, SOMETHING > reuse;

};

// EVERYTHING AFTER THIS GOES IN THE .CPP
#if( DEBUG )
string SOMETHING::toString( int i )
{
	ostringstream out;
	ind = indent( i );
	out << ind << " SOMETHING::\n";
	out << BaseRule::toString( i );
	return out.str();
}
#endif

// mention the reuse rule class
Reuse< BaseRule, SOMETHING > SOMETHING::reuse;


string SOMETHING::assignValue( string valueType, istringstream *value, FmlParser* fPtr )
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

SOMETHING::SOMETHING()
	:BaseRule();
{

}

void SOMETHING::destructor( bool willDelete )
{

	BaseRule::destructor( willDelete );
}

void SOMETHING::goCalcMe( BaseEnt *inClient, intensityType runIntensity )
{
	cout << "need to overload goCalcMe still! "; LN
}

#endif

// THIS IS THE END OF WHAT YOU SHOULD COPY AND PASTE


#endif

