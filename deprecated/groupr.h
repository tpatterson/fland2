// This rule acts like a group of rules.
// It acts like one rule.
// When you say groupr1 = groupr2 it creates all the new children rules
// exactly the same for group2 as group1 has.

#ifndef GROUPR_H
#define GROUPR_H

using namespace std;

#include "baserule.h"
#include "templates.h"

class GroupR : public BaseRule
{
	public:

	// This is where we store all the rules that are in this group
	vector< BaseRule* > groupMember;

	GroupR();

	void addGroupMember( BaseRule* );

	virtual void destructor( bool willDelete = false );

	virtual void goCalcMe( BaseEnt *inClient, intensityType );

	virtual void propogateChildren( BaseEnt* newEnt, intensityType intensity );

	// used in FMLParse
	virtual string assignValue( string valueType, istringstream *value, FmlParser* fPtr );

	// return the number of members
	inline int size();

	// clears all the rules, dosn't ask any questions. Be careful of MEM leaks!
	inline void clear();

	// adds a rule. If the rule is NULL, it dosn't add it.
	inline void addRule( BaseRule* );

	#if( DEBUG )
	virtual string toString( int );
	virtual string getType(){ return "GroupR"; }
	#endif
};

inline int GroupR::size()
{
	return groupMember.size();
}

inline void GroupR::clear()
{
	groupMember.clear();
}

inline void GroupR::addRule( BaseRule* inRule )
{
	if( inRule ) // if it isn't NULL
	{
		groupMember.push_back( inRule );
	}
}


#endif
