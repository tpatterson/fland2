#ifndef DESCRIMINATORYR_CPP
#define DESCRIMINATORYR_CPP

#include <string>
#include <sstream>
#include "groupr.h"
#include "tyler.h"
#include "defines.h"
#include "descriminatoryr.h"


#define REGION_CUTOFF 0.01

DescriminatoryR::DescriminatoryR()
	:GroupR()
{
	threshold1 = threshold2 = 0.0;
}

string DescriminatoryR::assignValue( string valueType, istringstream *value, FmlParser* fPtr )
{

	// an alias for adding groupmembers
	if( valueType == "threshold1" )
	{
		*value >> threshold1;
	}
	else if(valueType == "threshold2" )
	{
		*value >> threshold2;
	}
	else
	{
		return GroupR::assignValue( valueType, value, fPtr );
	}
	return "";
}

void DescriminatoryR::propogateChildren( BaseEnt* newEnt, intensityType inIntensity )
{
	intensityType region0, region1; // the intensities of both regions
	getBothIntensities( newEnt, inIntensity, region0, region1 );

	sassert( groupMember[0] );
	sassert( groupMember[1] );


	if( region0 > REGION_CUTOFF )
		groupMember[0]->propogateChildren( newEnt, region0 );

	if( region1 > REGION_CUTOFF )
		groupMember[1]->propogateChildren( newEnt, region1 );

	//cout << "region0: " << region0 << " region1: " << region1;LN
	//cout << groupMember[0]->toString(0);
}

void DescriminatoryR::goCalcMe( BaseEnt *inClient, intensityType inIntensity )
{
	intensityType region0, region1; // the intensities of both regions
	getBothIntensities( inClient, inIntensity, region0, region1 );

	sassert( groupMember[0] );
	sassert( groupMember[1] );
	if( region0 > REGION_CUTOFF )
		groupMember[0]->goCalcMe( inClient, region0 );
// here is our segfault
	if( region1 > REGION_CUTOFF )
		groupMember[1]->goCalcMe( inClient, region1 );
}


void DescriminatoryR::getBothIntensities( BaseEnt* inClient, intensityType inIntensity,
	intensityType& region0, intensityType& region1 )
{
	region1 = getSecondRegionIntensity( inClient );
	region0 = 1.0 - region1;

	region0 *= inIntensity;
	region1 *= inIntensity;
}

#endif

