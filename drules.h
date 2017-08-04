#ifndef D_RULES_H
#define D_RULES_H

#include "descriminatoryr.h"
#include <string>

#include "baserule.h"
#include "fmlparser.h"
#include "tyler.h"
#include "defines.h"
#include "perlinnoise.h"

/*
	NOTE - To add a new rule here do these things:

	- add the class definition here (copy and paste)
	- add the function in drules.cpp
	- go into fmlparser and add the rule to evaluateRule()
*/

// PatternR
class PatternR : public DescriminatoryR
{
    public:
    intensityType size;
    PerlinNoise noise;
    PatternR();
	virtual string assignValue( string valueType, istringstream *value, FmlParser* fPtr );
    virtual intensityType getSecondRegionIntensity( BaseEnt* inClient );
};

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

