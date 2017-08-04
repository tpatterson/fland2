#ifndef BASERULE_H
#define BASERULE_H

using namespace std;

#include "defines.h"
#include <string>

class BaseEnt;
class FmlParser;
class GroupR;
class BaseRule
{
	public:

	// The number of iterations deep this rule applies, until it spawns into
	// a new rule type, or goes away.
	int numPropogates;

	// The intensity of a rule. When 1.0 this rule's effects will apply 100%.
	// When the intensity of a rule gets to zero, the rule should no longer propogate.
	intensityType intensity;

	// When this rule is done propogating, a copy of one of these will be propogated.
	// this rule comes from the FmlParser, and is coded in with FML.
	BaseRule* childRule;

	virtual void destructor( bool willDelete = false );

	// These makes the rule do it's thing. Call it and pass the entity that
	// is using the rule (client), and the rule will alter the appropriate attributes
	// of the client.
	virtual void goCalcMe( BaseEnt *inClient, intensityType );

	// This is for possible future use, a 'stud hanging out of the wall'.
	// If I ever have a rule that needs to create a copy of itself instead of just using the
	// FML version, it should return a pointer to the new rule in this function.
	// All normal rules just return a pointer to themselves.
	virtual BaseRule* startUse();
	// this rule goes with the one above. If this rule had created a copy of itself,
	// this function will delete that copy.
	virtual void endUse();

	// this function is called from a rule who's entity is being split. It is called once for each of
	// that entity's children.
	// It inserts the pointers to rules into the new entity's list of rules.
	// it inserts either the same rule, or the new child rules.
	// remainingPropogates is the number of propogates the PARENT has left. the new ent will subtrsct 1 from this.
	void propogateRules( BaseEnt* newEnt, int remainingPropogates, intensityType nIntensity );

	// When a rule is done propogating and it's child(ren, in the case of descriminatory rules)
	// should be passed on, this rule inserts the new child(ren) into the newEnt's rule list.
	virtual void propogateChildren( BaseEnt* newEnt, intensityType nIntensity );

	// assigns attributes from the fmlparse functions
	// returns an error string, if there is an error
	// NOTE: the FmlParser pointer is for certain rule types, such as groupr,
	// where we are assigning a pointer out of the nameMap hash. It needs access
	// to the nameMap hash.
	virtual string assignValue( string valueType, istringstream *value, FmlParser* fPtr );

	BaseRule();

	virtual string getType(){};

	#if( DEBUG )
	virtual string toString( int );
	string fmlName; // the name of the rule in the fml nameMap
	bool fmlRule; // was I created by the fml parser, or through a makeMe?
	#endif
};


#endif
