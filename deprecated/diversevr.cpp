#ifndef DIVERSEVR_CPP // TODO: insert class name before _CPP
#define DIVERSEVR_CPP
/*
	Diverse Vertex Rule.
	This rule applies to a vertex, looks at certain parameters ( altitude, slope, etc... )
	and gives it different sets of rules, depending on the parameters.

	While there are different functions that determine how the diverseVR rule functions,
	there is only one type of diverseVR. A diverseVR rule has a pointer to a function that does the
	actual diversifacation calculations.
	For example, one diverseVR will have a function that applies to the slope of a piece of terrain.
	When goCalcMe is called it calls the function referenced by the slope pointer. Another
	function might be based on altitude.
*/

using namespace std;
#include "diversevrfunctions.cpp" // the actual functions that do the work.

class DiverseVR : public GroupR
{
	public:
	DiverseVR();
	virtual int constructor( BaseEnt *inParent );

	virtual void destructor( bool willDelete = false );

	// This makes the rule do it's thing. Call it and pass the entity that
	// is using the rule (client), and the rule will alter the appropriate attributes
	// of the client.
	virtual int goSplit( BaseEnt *inClient );
	virtual int goCalcMe( BaseEnt *inClient );

	// This function returns a rule that the entity's children will have when
	// theyasdfasdf are created.
	// inClient is the entity being split,
	// baseRule
	virtual BaseRule* propogateRule( BaseEnt* inClient );

	// Creates a new rule of the same type that this function is called on.
	virtual BaseRule* makeOneOfMe( BaseEnt* inParent );

	// Calls the correct reuse class to delete this object.
	virtual void deleteMe( BaseRule* );

	// used in the parse functions. assigns value to valuetype.
	// returns an error, if there was one. Otherwise, returns blank ( "" )
	virtual string assignValue( string valueType, istringstream *value, FmlParser* fPtr );

	#if( DEBUG )
	virtual void toString();
	virtual string getType(){ return "DiverseVR"; }
	#endif

	////////////////////////////////////////////////////////////////////////////////////////////
	// The special stuff

	// A pointer to the function that will do the work.
	// This could look at altitude, slope, etc.
	// The function this points to will set the attribs.
	void (*functionPointer)(DiverseVR*, Vertex*);

	// The generic vars.
	float var1, var2, var3, var4;

};

#if( DEBUG )
void DiverseVR::toString()
{
	cout << " DiverseVR::\n";
	GroupR::toString();
}
#endif


string DiverseVR::assignValue( string valueType, istringstream *value, FmlParser* fPtr )
{
	// function types
	// Each different function type has to have a row here.
	// The parser will assign the function pointer to the correct function.
	if( valueType == "function" )
	{
		if( (*value).str() == "slope1vrfunction" || (*value).str() == "slope1" )
		{
			functionPointer = SLOPE1_VR;
			LN
			Vertex* mee;
			functionPointer( this, mee );
		}
	}
	else if( valueType == "slopecutoff" )
	{
		*value >> SLOPE_CUTOFF;
	}
	else if( valueType == "slopestarttransition" )
	{
		*value >> SLOPE_START_TRANSITION;
	}
	else if( valueType == "slopeendtransition" )
	{
		*value >> SLOPE_END_TRANSITION;
	}


	// altitude rule (TODO)

	else
	{
		return GroupR::assignValue( valueType, value, fPtr );
	}
	return "";
}

BaseRule* DiverseVR::makeOneOfMe( BaseEnt* inParent )
{
	// create the object
	BaseRule* nObj = reuseDiverseVR.tNew( NULL );

	// TODO: insert code to set the new object's variables equal to the calling object.
	#define set( X ) dynamic_cast< DiverseVR* >( nObj )->X = X;

	#undef set
	// now return the pointer to the object
	GroupR::makeOneOfMe( inParent );
	return nObj;
}

void DiverseVR::deleteMe( BaseRule* inClient )
{
	delete inClient;
}

DiverseVR::DiverseVR()
{
	constructor( NULL );
}

int DiverseVR::constructor( BaseEnt *inParent )
{
	// TODO: Assign default values to parameters here
	var1 = var2 = var3 = var4 = 0.0;
	functionPointer = NULL;

	GroupR::constructor( inParent );
	return 0;
}

BaseRule* DiverseVR::propogateRule( BaseEnt* inClient )
{
	// propogate forever
	return this;
}

void DiverseVR::destructor( bool willDelete )
{

	GroupR::destructor( willDelete );
}

int DiverseVR::goSplit( BaseEnt *inClient )
{

	return 0;
}
int DiverseVR::goCalcMe( BaseEnt *inClient )
{

	return 0;
}

#endif
