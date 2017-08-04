#ifndef ROUGHR_H
#define ROUGHR_H

// This rule applies to verticies. Moves them up or down to make
// a rough, mountainous terrain.
// a meRule

using namespace std;

#include "vertex.h"
#include "baserule.h"
#include "templates.h"

class RoughR : public BaseRule
{
	public:
	RoughR();
	virtual void goCalcMe( BaseEnt*, intensityType );
	 // between 0 and 1. 0 means a vertex is offset none,
	 // and 1 means it could randomly be up to 1 or -1 times the
	 // boundRadius.
	double roughness;
    double upPull;
	string assignValue( string valueType, istringstream *value, FmlParser* fPtr );
	#if( DEBUG )
	virtual string getType(){ return "RoughR"; }
	#endif
};

#endif

