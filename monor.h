
#ifndef MONOR_H
#define MONOR_H

using namespace std;

#include "baserule.h"
#include "defines.h"

/*
Rules:
An entity stores a list of rules that control how it looks, in color altitude difference compared to it's neighbors,  etc.
When a rule splits it's children get rules from their parent, and (NEED TO ADD) their neighbors rules. This should work as follows:
   Each new child calls takeRulesFromParent which should be renamed takeRulesFromParentAndNeighbors. It will have an algorithm that combines like rules, because most of the time they will be the same. This algorithm will have to calculate the combined weight of  each rule.

   RULE GENERATION:
A new type of rule will be created, called monoRule (because it is monolithic). It will be a groupR and handle discrimination based on slope, noise, and maybe other factors. It's child rules will be generated if they don't already exist.

   MonoRule Child Generation:
An algorithm will create new monoRules based on the parent rule's scale and other parameters. The algorithm will probably have three scales, BEFORE_VEGETATION, VEGETATION, and SUB_VEGETATION. Each scale will have a different sub algorithm that applies, and may even use different parameters, such as "rainfall", "sunlight", "rockiness", etc. Perhaps each of these parameters will apply at all levels. Note that these params are meta-data and do not apply when the rule is being applied to an entity, but only when the monoRule is generating it's child rules.

    Another possible approach: instead of using a monolithic rule for rule generation, perhaps I should use a generation rule that is a groupr. It would just hold pointers to it's rules that actually do the work. NEVERMIND, the cons outweigh the pros. See below.

 PROS:
-Less coding because rules that do the work are already done
-Code is cleaner because it is more seperated
  CONS:
-Slower because more pointer math going on
-Difficult to access properties of sub rules (color, ruoghness, etc).
-If organized well the monorule could be cleaner than this method because it removes all the extra stuff.

(from iphone notes)
Tyler Patterson
*/

class MonoR : public BaseRule
{
	public:

    // Parameters that effect how the entity that possesses this rule
    // will look
    ColorClass colorObj;
    RoughnessClass roughnessClass;

    // parameters akin to DiscriminatoryRules - ones that 
    // affect, when splitting, which childRule the newly
    // created entities will take:
    DiscriminatoryClass discriminatoryClass;

    
    
    // parameters that affect how the child rules will
    // be generated.
    
    // The scale is in meters, and represents the approximate size of entities
    // that this rule aplies to. QUESTION: Should there be a starting scale 
    // and an ending scale? 
    DIS_TYPE scale;

    // some parameters that define the living conditions
    // that exist where this rule applies. Used to determine
    // the colors, and later vegetation that applies to 
    // entities that take this rule
    float average_temp; // in celcius
    float rainfall_inches; // inches of rain (or equivilant in snow) per year
    float rockiness; // 1 is rocky cliffs, 0 is baby bottom
    float sandiness; // 0-1
    float topsoil_depth; // in meters


	MonoR();

	virtual void destructor( bool willDelete = false );

	virtual void goCalcMe( BaseEnt *inClient, intensityType );

	virtual void propogateChildren( BaseEnt* newEnt, intensityType intensity );

	// used in FMLParse
	virtual string assignValue( string valueType, istringstream *value, FmlParser* fPtr );


	#if( DEBUG )
	virtual string toString( int );
	virtual string getType(){ return "GroupR"; }
	#endif

	// adds a rule. If the rule is NULL, it dosn't add it.
//    inline void addRule( BaseRule* );
};

//inline void GroupR::addRule( BaseRule* inRule )
//{
//    if( inRule ) // if it isn't NULL
//    {
//        groupMember.push_back( inRule );
//    }
//}


#endif
