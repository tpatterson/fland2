#ifndef D_RULES_H
#define D_RULES_H

#include "descriminatoryr.h"

#include "tyler.h"
#include "defines.h"

/*
	NOTE - To add a new rule here do these things:

	- add the class definition here (copy and paste)
	- add the function in drules.cpp
	- go into fmlparser and add the rule to evaluateRule()
*/

//AltitudeR
class AltitudeR : public DescriminatoryR
{
	public:
	virtual intensityType getSecondRegionIntensity( BaseEnt* inClient );
};

//SlopeR
class SlopeR : public DescriminatoryR
{
	public:
	virtual intensityType getSecondRegionIntensity( BaseEnt* inClient );
};

// SlopeXAltitudeR
class SlopeXAltitudeR : public DescriminatoryR
{
	public:
	virtual intensityType getSecondRegionIntensity( BaseEnt* inClient );
};
#endif

