#ifndef DESCRIMINATORYR_H
#define DESCRIMINATORYR_H

#include <string>
#include "groupr.h"
#include "tyler.h"
#include "defines.h"


/*
	Region0 is at the bottom, or is the default. region1 (second region) begins to be
	stronger when the altitude (or slope, or whatever) increases.
*/

class DescriminatoryR : public GroupR
{
	public:
	virtual string assignValue( string valueType, istringstream *value, FmlParser* fPtr );

	float threshold1;
	float threshold2;

	// every rule class overloads this function
	// the actual descriminatory work will be done here.
	// Returns the intensity of the second region.
	// The intensity of the first region will be a decreasing function of the second region.
	// Params:
	//	- The to whom this rule is applying.
	virtual intensityType getSecondRegionIntensity( BaseEnt* inClient ) = 0;

	virtual void goCalcMe( BaseEnt *inClient, intensityType );

	// returns by value the intensity each region should have.
	void getBothIntensities( BaseEnt* inClient, intensityType inIntensity,
		intensityType& region0, intensityType& region1 );

	virtual void propogateChildren( BaseEnt* newEnt, intensityType inIntensity );

	DescriminatoryR();
};


#endif

