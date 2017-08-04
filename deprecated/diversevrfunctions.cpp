#ifndef DIVERSEVRFUNCTIONS_CPP
#define DIVERSEVRFUNCTIONS_CPP
#include "vertex.cpp"



// These are the functions that DiverseVR will call to do the work.


// These are the different functions, and thier parameters
#define SLOPE1_VR slope1VRFunction
	// slopeSmooth
	#define SLOPE_START_TRANSITION var1
	#define SLOPE_END_TRANSITION var2
	// slopeDefiniteGradient
	#define SLOPE_CUTOFF var1




/*
	WHEN ADDING A NEW FUNCTION, UPDATE THESE PLACES:
	1. The top of this function, add #defines for the specific vars you need
	2. diversevr.cpp assignValue - the vars the function will use
	3.     ''        insert function name so the rules will have a pointer to it.
	4. write the function
*/


void slope1VRFunction( DiverseVR* rule, Vertex* vertex )
{
	LN
}



#endif

