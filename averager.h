#ifndef AVERAGER_H
#define AVERAGER_H
// This rule takes a vertex and sticks it between it's parent and it's aunts.
// in all attributes that apply.
// (position, color)

using namespace std;

#include <string>
#include "templates.h"
#include "baserule.h"

class AverageR : public BaseRule
{
	public:
	AverageR();
	virtual void goCalcMe( BaseEnt*, intensityType );
	// used in the parse functions. assigns value to valuetype.
	// returns an error, if there was one. Otherwise, returns blank ( "" )
	virtual string assignValue( string valueType, istringstream *value, FmlParser* fPtr );
	#if( DEBUG )
	virtual string getType();
	virtual string toString( int ind=0 );
	#endif

};

#endif

