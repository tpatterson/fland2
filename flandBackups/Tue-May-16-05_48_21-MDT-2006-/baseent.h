#ifndef BASEENT_H
#define BASEENT_H

using namespace std;

#include "defines.h"
#include "rendervertex.h"
#include "groupr.h"
#include "templates.h"

class BaseRule;
class RulePointer;
//class GroupR;
class BaseEnt
{
	public:
	// pointer back to our parent
	BaseEnt *parent;

	// how many generations we are removed from the great root entity.
	// root is one, each child is 2, grandchildren are 3, etc...
	int iterationDepth;

	// each entity has a random seed value, given at creation time.
	int seed;

	// Each entity has a list of children.
	vector< BaseEnt* > child;

	// Each entity has 2 vectors of rules. The meRules are run on a new entity as
	// soon as it has been created. the splitrules are run when it is split, to create
	// children.
	//vector< BaseRule* > meRule;
	//vector< BaseRule* > splitRule;
	//GroupR* meRule;
	//GroupR* splitRule;

	// Nope. We decided to give each entity one of these.
	vector< RulePointer > rules;

	// adds a rule to the vector of rules.
	void addRule( BaseRule*, int numPropogates, intensityType );
	// adds a rule and then runs it.
	void addAndRunRule( BaseRule*, int numPropogates, intensityType );

	// Whether or not this entity is split
	bool isSplit;

	// The distance from the viewer. Recalculated every frame.
	DIS_TYPE viewDistance;

	// recalculated when created. This is the visibility of the entity
	// when it is created. When the visibility gets down to this value,
	// the entity is deleted out of the world.
	double createVisibility;

	// These are recalculated every frame and are used to know who to split
	// and un-split
	bool isVisible;
	double visibility;


	// each entity gets a bounding sphere.
	// It and all its future should fit inside this sphere.
	BoundSphere bound;

	/////////////////////////////////////////////////////////////////////////////////
	// functions

	// PROBLEM: Since there are so many entities and rules being created and destroyed
	// every frame the program will have to dynamically create and destroy objects a whole lot.
	// SOLUTION: There is a stack of each type of entity and rule . When an entity or rule
	// is needed it checks if there is one on the stack. If so, it pulls it off and runs it's
	// "constructor". If there isn't one available, it creates it, and then calls it's "constructor".
	// Note that this is not the real constructor, it cant be the real one because you cant call
	// the real one more then once.
	virtual int constructor( BaseEnt *parent );

	// The real constructor.
	BaseEnt();

	// The destructor is called explicitly, It has to "delete" it's children.
	virtual void destructor( bool willDelete = false );

	// This is the "indescriminate kill". By using the actual destructor
	// you just kill and object and all it's kids, not worying about detaching
	// everything right, just making sure it all gets deleted.
	virtual ~BaseEnt();

	// Every entity has the function draw();
	virtual void draw();

	// This function recursively goes through each entity and calculates the
	// visibility of each one.
	void recursiveCalcVisibility();

	// This runs through each entity and splits it if necessary
	// You have to calculate the visibility of each object first!
	void recursiveSplit();

	// Calculates and stores the visibility and isVisible
	// for the entity
	// Is NOT recursive. calculates and sets if it is visible, the visibility level,
	// and the morph amount.
	void calcVisibility();

	// called when the object is created. Runs the 'me' rules.
	// This function calculates the unique attributes for each entity.
	// such as X, Y, Z, color, it's rules, etc.
	//virtual int calculateMe();

	// every entity has the function split(). This function creates it's children,
	// and calls calculateMe() for each one.
	// It first calls propogateRules to get the parent's rules, then it
	// uses those rules to determine it's attributes.
	virtual void split();

	// unsplits an entity. Donsen't check if this is allowed. Check that before calling this function.
	// Deletes all it's children.
	// NOTE: if a vertex, it will delete all neighbors that need to be deleted.
	virtual void unSplit();

	// goes through and un-splits the approprite entities.
	// Returns true if this entity can be deleted.
	bool recursiveUnSplit();

	// returns true if there aren't any neighbors depending on this entity
	// for existance.
	virtual bool canUnSplit();

	// Right before an entity is un-split,
	// it runs this function. A regular entity simply
	// returns true-can be un-split, but a vertex
	// will check it's neighbors.
	// Returns true if it can be un-split, and false if not.
	// NOTE: a vertex may run recursiveUnSplit on neighbors
	// from this function.
	virtual bool specialUnSplitCheck();

	// for a newly created child. Goes through the parent's rules and
	// takes the appropriate ones for itself.
	void takeRulesFromParent();

	// Counts recursively how many children this entity has.
	// Also gets the deepest iteration depth
	int getCount( int& deepestIteration );

	// returns true if the entity should be split. NOTE: uses the
	// visibility calculations from calcVisibility. Must run calcVisibility first.
	inline bool shouldSplit();
	// should this entity unsplit?
	inline bool shouldUnSplit();

	// This sets an entity and all it's posterity as visible or invisible.
	void setVisible( bool );

	// this is the first pass the calculate loop makes. The return
	// value is the number of entities gone through so far.
	int calcPass1();

	// after an entity has had it's visibility calulated, it runs this function.
	virtual void calcMorph( double );

	// writes each object's key attributes to the screen.
	#if DEBUG

	// When an entity is tDeleted it will set this to true.
	// This is to help make sure that there are no deleted entities still in the world.
	bool isDeleted;

	// print info about each entity
	virtual void dump( ostream&, int );

	virtual string toString( int );

	virtual void printType();
	virtual string getType();

	// Prints in a compact form every entity, in a way easy to see parent-children relationships.
	void printGeneology( int depth );
	#endif

	static Reuse< BaseEnt, BaseEnt > reuse;
};


// returns true if the entity should be split. NOTE: uses the
// visibility calculations from calcVisibility. Must run calcVisibility first.
inline bool BaseEnt::shouldSplit()
{
	//Fland *fl = getFlandPtr();
	if( isVisible && visibility > 1.0 )//fl->getSplitVisibility() )
		return true;
	else
		return false;
}

// should this entity unsplit? (based only on visibility, not on
// the status of it's neighbors.
inline bool BaseEnt::shouldUnSplit()
{
	//Fland *fl = getFlandPtr();
	if( !isVisible || visibility < 1.0 )//fl->getSplitVisibility() )
		return true;
	else
		return false;
}

// every entity has a vector of pointers to these.
class RulePointer
{
	private:
	BaseRule* rule;
	int remainingPropogates;
	intensityType intensity;

	public:
	inline BaseRule* getRule(){ return rule; }
	inline void setRule( BaseRule* r ){ rule = r; }
	inline intensityType getIntensity(){ return intensity; }
	inline void setIntensity( intensityType i ){ intensity = i; }
	inline int getRemainingPropogates(){ return remainingPropogates; }
	inline void setRemainingPropogates( int p ){ remainingPropogates = p; }
	string toString( int = 0 );

	RulePointer();

	//// tells the rule to do it's thing to the entity.
	//inline void goCalcMe( BaseEnt* newEnt )
	//{
	//	sassert( rule );
	//	sassert( newEnt );
	//	rule->goCalcMe( newEnt, intensity );
	//}

	inline void propogateRules( BaseEnt* newEnt )
	{
		sassert( newEnt );
		rule->propogateRules( newEnt, remainingPropogates, intensity );
	}

	string getFmlName()
	{
		return rule->fmlName;
	}

};

#endif
