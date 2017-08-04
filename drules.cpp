#ifndef D_RULES_CPP
#define D_RULES_CPP


#include "baseent.h"
#include "vertex.h"
#include "tyler.h"

#include "drules.h"

//////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// PATTERN //////////////////////////////////////
// inClient HAS to be a vertex!
intensityType PatternR::getSecondRegionIntensity( BaseEnt* inClient )
{
	intensityType n;
    n = noise.multipleNoise( size, 3, 
            VERT( inClient )->getX(), 
            VERT( inClient )->getY(), 
            VERT( inClient )->getZ(),
            0.3 );
	
    return( convertRange1ToRange2( n, threshold1, threshold2, (intensityType)0.0, (intensityType)1.0 )  );
}

PatternR::PatternR()
	:DescriminatoryR()
{
    size = 1.0;
}

string PatternR::assignValue( string valueType, istringstream *value, FmlParser* fPtr )
{
	if( valueType == "size" )
	{
		*value >> size;
	}
	else
	{
		return DescriminatoryR::assignValue( valueType, value, fPtr );
	}
	return "";
}

//////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// ALTITUDE //////////////////////////////////////
// inClient HAS to be a vertex!
intensityType AltitudeR::getSecondRegionIntensity( BaseEnt* inClient )
{
	intensityType alt = VERT( inClient )->getAltitude();

	return( convertRange1ToRange2( alt, threshold1, threshold2, (intensityType)0.0, (intensityType)1.0 ) );
}

//////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// SLOPE /////////////////////////////////////////
// inClient HAS to be a vertex!
intensityType SlopeR::getSecondRegionIntensity( BaseEnt* inClient )
{
	intensityType slope = VERT( inClient )->getSlope();
//cout << VERT( inClient )->trueVertex.nz;LN
	return( convertRange1ToRange2( slope, threshold1, threshold2, (intensityType)0.0, (intensityType)1.0 ) );
}

//////////////////////////////////////////////////////////////////////////////////
////////////////////////////// SLOPE_X_ALTITUDE //////////////////////////////////
// inClient HAS to be a vertex!
intensityType SlopeXAltitudeR::getSecondRegionIntensity( BaseEnt* inClient )
{
	intensityType slope = VERT( inClient )->getSlope() * VERT( inClient )->getAltitude();

	return( convertRange1ToRange2( slope, threshold1, threshold2, (intensityType)0.0, (intensityType)1.0 ) );
}

#endif

