#ifndef COLORSHIFTR_H
#define COLORSHIFTR_H

using namespace std;

#include <string>
#include "baserule.h"
#include "fmlparser.h"

class ColorShiftR : public BaseRule
{
	public:
	float shiftAmount;
	// If you want a color to stay near a certain value, this is how hard a tendancy it has to stay near the val.
	// 0 means it can stray as far as it wants, 1.0 means it always stays within shiftAmount.
	float stayAmount;
	// keep track of the inverse of stayAmount, so it dosn't have to be calulated all the time
	float inverseStayAmount;
	// If the color is staying near a certain value, use these RGB values for where to stay next to
	float sr, sg, sb;
	virtual void goCalcMe( BaseEnt*, intensityType );
	ColorShiftR();

	// used in the parse functions. assigns value to valuetype.
	// returns an error, if there was one. Otherwise, returns blank ( "" )
	virtual string assignValue( string valueType, istringstream *value, FmlParser* fPtr );
	#if( DEBUG )
	virtual string getType();
	#endif

};


#endif

