#ifndef ROUGHR_CPP
#define ROUGHR_CPP

// This rule applies to verticies. Moves them up or down to make
// a rough, mountainous terrain.
// a meRule

using namespace std;

#include <sstream>
#include "tyler.h"
#include "vertex.h"
#include "baserule.h"
#include "templates.h"
#include "roughr.h"


string RoughR::assignValue( string valueType, istringstream *value, FmlParser* fPtr )
{
	if( valueType == "roughness" )
	{
		 *value >> roughness;
	}
	else
	{
		return BaseRule::assignValue( valueType, value, fPtr );
	}
	return "";
}

RoughR::RoughR():BaseRule()
{
	roughness = 0.3;
}

void RoughR::goCalcMe( BaseEnt* inClient, intensityType runIntensity )
{

	runIntensity *= intensity;

	double randAmount = ( roughness * 2.0 * ( rnd() - 0.5 ) ) * VERT(inClient)->bound.radius * runIntensity;

	VERT(inClient)->trueVertex.x += ( randAmount * VERT(inClient)->trueVertex.nx );
	VERT(inClient)->trueVertex.y += ( randAmount * VERT(inClient)->trueVertex.ny );
	VERT(inClient)->trueVertex.z += ( randAmount * VERT(inClient)->trueVertex.nz );

}




#endif
