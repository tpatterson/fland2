#ifndef COLORSHIFTR_CPP
#define COLORSHIFTR_CPP

using namespace std;

#include <sstream>
#include "baserule.h"
#include "templates.h"
#include "tyler.h"
#include "vertex.h"
#include "colorshiftr.h"

// a meRule.

#if( DEBUG )
string ColorShiftR::getType(){ return "ColorShiftR"; }
#endif


ColorShiftR::ColorShiftR()
	:BaseRule()
{
	shiftAmount = 0.1;
	stayAmount = 0.0;
	inverseStayAmount = 1.0;
	sr = sg = sb = 1.0;
}

string ColorShiftR::assignValue( string valueType, istringstream *value, FmlParser* fPtr )
{
	if( valueType == "shiftamount" )
	{
		*value >> shiftAmount;
	}
	else if( valueType == "stayamount" )
	{
		*value >> stayAmount;
		inverseStayAmount = 1.0 - stayAmount;
	}
	else if( valueType == "sr" || valueType == "stayred" || valueType == "red" )
	{
		*value >> sr;
	}
	else if( valueType == "sg" || valueType == "staygreen" || valueType == "green" )
	{
		*value >> sg;
	}
	else if( valueType == "sb" || valueType == "stayblue" || valueType == "blue" )
	{
		*value >> sb;
	}
	else
	{
		return BaseRule::assignValue( valueType, value, fPtr );
	}
	return "";
}


void ColorShiftR::goCalcMe( BaseEnt* inClient, intensityType runIntensity )
{
	float startRed = VERT(inClient)->trueVertex.red;
	float startGreen = VERT(inClient)->trueVertex.green;
	float startBlue = VERT(inClient)->trueVertex.blue;


	float rndRed = ( rnd() - 0.5 ) * 2.0 * shiftAmount;
	float targetRed = startRed * inverseStayAmount;
	targetRed += sr * stayAmount;
	targetRed += rndRed;
	if( targetRed > 1.0 )
		targetRed = 1.0;
	if( targetRed < 0.0 )
		targetRed = 0.0;

	float rndGreen = ( rnd() - 0.5 ) * 2.0 * shiftAmount;
	float targetGreen = startGreen * inverseStayAmount;
	targetGreen += sg * stayAmount;
	targetGreen += rndGreen;
	if( targetGreen > 1.0 )
		targetGreen = 1.0;
	if( targetGreen < 0.0 )
		targetGreen = 0.0;

	float rndBlue = ( rnd() - 0.5 ) * 2.0 * shiftAmount;
	float targetBlue = startBlue * inverseStayAmount;
	targetBlue += sb * stayAmount;
	targetBlue += rndBlue;
	if( targetBlue > 1.0 )
		targetBlue = 1.0;
	if( targetBlue < 0.0 )
		targetBlue = 0.0;

	runIntensity *= intensity;
	float newRed = ( targetRed - startRed ) * runIntensity + startRed;
	float newGreen = ( targetGreen - startGreen ) * runIntensity + startGreen;
	float newBlue = ( targetBlue - startBlue ) * runIntensity + startBlue;

	VERT(inClient)->trueVertex.red = newRed;
	VERT(inClient)->trueVertex.green = newGreen;
	VERT(inClient)->trueVertex.blue = newBlue;
}

#endif
