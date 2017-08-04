#ifndef SlopeR_H
#define SlopeR_H


using namespace std;

#include "groupr.h"
#include "vertex.h"
#include "templates.h"

class SlopeR : public GroupR
{
	public:

	float threshold1;
	float threshold2;

	SlopeR():BaseRule(){}

	virtual void destructor( bool willDelete = false );

	virtual void goCalcMe( BaseEnt *inClient, intensityType );

	// used in the parse functions. assigns value to valuetype.
	// returns an error, if there was one. Otherwise, returns blank ( "" )
	virtual string assignValue( string valueType, istringstream *value, FmlParser* fPtr );

	#if( DEBUG )
	virtual string toString( int );
	virtual string getType();
	#endif

};



#endif

