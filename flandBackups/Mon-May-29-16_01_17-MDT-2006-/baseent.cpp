// Every entity inherits from BaseEnt.
// BaseEnt is purely abstract. You can only inherit from it, not instantiate from it. --- ACTUALLY, you can. :)
// This is good so we can have a root entity.
#ifndef BASEENT_CPP
#define BASEENT_CPP

using namespace std;

#include <iostream>
#include <sstream>
#include "tyler.h"
#include "fland.h"
#include "camera.h"
#include "baseent.h"
#include "templates.h"
////////////////////////////////////////////////////////////////////////////
// The implementation of the BaseEnt class

Reuse< BaseEnt, BaseEnt > BaseEnt::reuse;

/*
// This is adam's singlton class. It creats only one instance of
// itself.
class Singlton
{
	private:
		static Singlton instance;

		Singlton();


	public:
		static Singlton getInstance()
		{
			return instance;
		}
};
*/

#if DEBUG
void BaseEnt::dump( ostream& out, int depth )
{
	out << this->toString( depth ) << endl << endl;

	for( int i=0; i<child.size(); i++ )
	{
		child[i]->dump( out, depth + 1 );
	}
}

string BaseEnt::toString( int indentNum )
{
	ostringstream out;
	//cout << "BaseEnt " << this << visibility <<endl;

	string ind = indent( indentNum );

	out << ind << "BaseEnt::" << endl;
	out << ind << "This              " << this << endl;
	out << ind << "Parent            " << parent << endl;
	out << ind << "Iteration Depth   " << iterationDepth << endl;
	out << ind << "isVisible         " << isVisible << endl;
	out << ind << "visibility        " << visibility << endl;
	out << ind << "createVisibility  " << createVisibility << endl;
	out << ind << "isSplit           " << isSplit << endl;
	#if DEBUG
	out << ind << "isDeleted         " << isDeleted << endl;
	if( isDeleted )
		out << ind << "********** THIS OBJECT HAS BEEN DELETED! IT SHOULDN'T BE BEING USED! **********" << endl;
	#endif
	for( int i=0; i<rules.size(); i++ )
		out << rules[i].toString( indentNum );
	out << ind << "child.size()      " << child.size() << endl;
	out << ind << "child:" << endl;
		for( int i=0; i<child.size(); ++i )
		{
			out << ind << "\t" << i << ": " << child[i] << "   " << child[i]->getType() << endl;
		}

	return out.str();
}

string RulePointer::toString( int indentNum )
{
	string ind = indent( indentNum );
	ostringstream out;

	if( rule )
		out << ind << "Rule: " << rule->fmlName << endl;

	return out.str();
}

void BaseEnt::printGeneology( int depth )
{
	for( int i=0; i<depth; i++ )
	{
		cout << ".";
	}
	cout << this->getType() << endl;
	for( int i=0; i<child.size(); i++ )
	{
		child[i]->printGeneology( depth + 1 );
	}
}

void BaseEnt::printType(){ cout << "BaseEnt" << endl; }
string BaseEnt::getType(){ return( "BaseEnt" ); }
#endif

// called by split()
// Take all the right rules from our parent.
// This function is called from each child entity of a rule that was just split.
// It is called before the children's rules are processed(duh, they dont exist until after this function)
// This function loops through each of the rules of the entity that was just split, runs and inserts
// the current entities rules into the new child.
void BaseEnt::takeRulesFromParent()
{
	for( int i=0; i < parent->rules.size(); i++ )
	{
		parent->rules[i].propogateRules( this );
	}
}

BaseEnt::~BaseEnt()
{

	//if( nextEnt ) delete nextEnt; // kill our kids
	// TODO: put the delete rules code here!
	//if( meRule && meRule->parent == this ) delete meRule; // kill the rules that made us
	//if( splitRule && splitRule->parent == this ) delete splitRule; // kill the rules that made our kids.
	//delete rule;
}


BaseEnt::BaseEnt()
{
	//meRule = new GroupR;
	//splitRule = new GroupR;
	BaseEnt::constructor( NULL );
}


int BaseEnt::constructor( BaseEnt *inParent )
{

	// Pass the object's parent to it.
	parent = inParent;

	bound.initalize();

	if( parent ) // if we aren't the root entity
	{
		iterationDepth = parent->iterationDepth + 1; // depth increases each iteration
		bound.radius = parent->bound.radius / 3.0; // TODO fix this to calculate more acurately the needed bounding sphere
		// If any rules change the bound radius, rerun this calulation!
		createVisibility = parent->bound.radius / bound.radius;
	}
	else // we are the root entity
	{
		createVisibility = 0.0; // we never get deleted.
		iterationDepth = 0;
	}
	seed = 0; // The random seed. this will have to be set directly by the parent.
	child.clear();
	viewDistance = 0.0;
	isSplit = false;

	rules.clear();

	#if DEBUG
	isDeleted = false;
	#endif

	return 0;
}

void BaseEnt::draw()
{

	// tell all our children to draw themselves.
	for( int i=0; i<child.size(); i++ )
	{
		child[i]->draw();
	}

}

//int BaseEnt::calculateMe()
//{
//	// loop through and run all the me-rules.
//	tSrand( seed );
//	for( int i=0; i<rules.size(); i++ )
//	{
//		rules[i].goCalcMe( this );
//	}
//
//	return 0;
//}

// When the entity needs to be split into smaller pieces.
void BaseEnt::split()
{
	if( !isSplit ) // make sure not already split
	{ // We are NOT split yet
		isSplit = true;

		// Propogate the parent's rules, and calculate run calculateMe on each child.
		for( int i=0; i<child.size(); i++ )
		{
			child[i]->takeRulesFromParent();
			//child[i]->calculateMe();
			child[i]->calcVisibility();
			//child[i]->recursiveSplit(); // if necessary, split the new child.
		}

	} // not isSplit
	else // We are split. This should actually never be called because recursiveSplit() checkes if the entity is already split.
	{
		cout << "Shouldn't be here! "; LN
		// loop through and split all our kids.
		for( int i=0; i<child.size(); i++ )
		{
			//if( rnd() < 0.8 )
				child[i]->split();
		}
	}
}



int BaseEnt::calcPass1()
{
	return 0;
}

void BaseEnt::calcVisibility()
{
	Fland *fl = getFlandPtr();
	assert( fl );
	Camera *cam = fl->camera;

		// find the distance, and compute the visibility for the entity
		DIS_TYPE dist = distance( cam->x, bound.x, cam->y, bound.y, cam->z, bound.z );
		viewDistance = dist;

	// check if the object is anywhere inside the view frustum
	if( fl->camera->sphereInFrustum( bound.x, bound.y, bound.z, bound.radius ) )
	{
		// set isVisibile to TRUE
		isVisible = true;
		visibility = bound.radius / dist;
		visibility *= fl->getInverseSplitVisibility();

		// This will be used in setting the near and far clip plane for openGL.
		// to determine how close and far the viewer should be able to see.
		if( dist < fl->closestRenderDistance )
			fl->closestRenderDistance = dist;
		if( dist > fl->farthestRenderDistance )
			fl->farthestRenderDistance = dist;

		if( !isSplit )
		{
			double morphAmount = visibility;

			morphAmount = morphAmount - createVisibility;
			morphAmount *= ( 1.0 / (1.0 - createVisibility) );
			morphAmount *= ( 1.0 / fl->morphPercentage );
			if( morphAmount > 1.0 )
				morphAmount = 1.0;
			if( morphAmount < 0.0 )
				morphAmount = 0.0;

			calcMorph( morphAmount );
		}
	}
	else
	{
		// the object is not inside the view frustum
		isVisible = false;
		calcMorph( (double)isSplit );

	}
}

// the baseEnt version does nothing.
void BaseEnt::calcMorph(double morphAmount ){}

// goes through each entity and calculates the visibility for it.
void BaseEnt::recursiveCalcVisibility()
{
	calcVisibility();
	if( isVisible ) // if this entity is not visible, we know that none of
	{// it's children are either. Dont bother calculating them.
		for( int i=0; i<child.size(); i++ )
		{
			child[i]->recursiveCalcVisibility();
		}
	}
	else
	{
		// We aren't visible, so none of our children are either.
		// set them all to invisible.
		setVisible( false );
	}
}

void BaseEnt::recursiveSplit()
{
	Fland *fl = getFlandPtr();
	if( isSplit )
	{
		for( int i=0; i<child.size(); i++ )
		{
			child[i]->recursiveSplit();
		}
	}
	else
	{
		if( shouldSplit() )
		{
			split();
			// now check his children to see if they should split too.
			for( int i=0; i<child.size(); i++ )
			{
				// split() calculates the visibility, all we have to do is tell
				// it to split now.
				child[i]->recursiveSplit();
			}
		}
	}
}

// The return parameter is whether or not this entity can be deleted.
bool BaseEnt::recursiveUnSplit()
{

				// debug
			if( this->getType() == "BaseEnt" )
				{LN}
	// can we be unsplit?
	// only if all our children are ready to be deleted.
	bool retValue = true;
	// go out to the leaves first, then un-split on your way back in.
	for( int i=child.size() - 1; i>=0; i-- )
	{
		// even if we find one child that cant be un-split, still do the rest, to get
		// them all as un-split as possible.
		if( child[i]->recursiveUnSplit() == false )
				retValue = false;
	}

			// debug
			if( this->getType() == "BaseEnt" )
				{cout << retValue; LN}

	// if our children cant all be deleted, bail out now.
	if( !retValue )
		return false;
	// NOTE: if we get to here and retValue is still true, then
	// all are children are un-split and are able to be deleted.

	// Now check if WE are able to be deleted.
	// Check this entity's visibility
	// if we're visible and shouldn't be un-split based on visiblity
	// then bail out now.
	if( !shouldUnSplit() )
		return false;

			// debug
			if( this->getType() == "BaseEnt" )
				{cout << retValue; LN}

	// if we are a leaf (dont have any children), skip all this and return true.
	if( isSplit )
	{
		// If we cant be deleted because the special check returns false, bail out.
		if( this->specialUnSplitCheck() == false )
			return false;

		// If everything checks out, we can be un-split!
		this->unSplit();
	}

	return true;
}

bool BaseEnt::specialUnSplitCheck()
{
	return true;
}

// call this function on an entity to see if it can be un-split
// IE - so all it's children are all destroyed.

// This check is necessary because the unsplit function destroys all the
// children, and they may be visible.

// NOTE: it calls itself on entities that are not split yet, and then
// all it does is check if they are visible or not. Still works the same.
bool BaseEnt::canUnSplit()
{
	// first check if it should un-split.
	// This means that if we un-split the entity,
	// it would only be resplit the next time.
	if( shouldUnSplit() )
		return false;

	// Now check our children (if we have any)
	// if not, then we are good for unspliting!
	if( isSplit )
	{
		// loop through and check all the children.
		for( int i=0; i<child.size(); i++ )
		{
			// if the child cannot be unsplit, then neither can we.
			if( child[i]->canUnSplit() == false )
				return false;
		}
	}

	return true;
}

int BaseEnt::getCount( int& deepestIteration )
{
	// childrenCount starts at one because this entity is a child (of it's parent)
	// if it was zero the final count would also be zero.
	int childrenCount = 1;
	for( int i=0; i<child.size(); i++ )
	{
		childrenCount += child[i]->getCount( deepestIteration );
	}
	if( iterationDepth > deepestIteration )
	{
		deepestIteration = iterationDepth;
	}
	return childrenCount;
}


// Sets the visibility of an entity and all it's children.
// used where the visibility is recursively calulated.
void BaseEnt::setVisible( bool inVisibility )
{
	// set the visibility
	visibility = inVisibility;
	// now set the visibility of the children
	for( int i=0; i<child.size(); i++ )
		child[i]->setVisible( inVisibility );
}

// kill all children.
void BaseEnt::unSplit()
{
	// loop through children

	for( int i=0; i<child.size(); i++ )
	{
		//cout << i << endl;
		// delete the child
		sassert( child[i] );
		child[i]->destructor();
	}
	// clean out the child array
	child.clear();
	// set us back to un-split
	isSplit = false;
	// Now that we arent split anymore, calculate our visibility.
	calcVisibility();
}

void BaseEnt::addRule( BaseRule* r, int p, intensityType i )
{
	sassert( r );
	sassert( i > 0 && i <= 1.0001 );

	RulePointer nRule;
	nRule.setRule( r );
	nRule.setIntensity( i );
	nRule.setRemainingPropogates( p );
	rules.push_back( nRule );
}

void BaseEnt::addAndRunRule( BaseRule* r, int p, intensityType  i)
{
	addRule( r, p, i );
	r->goCalcMe( this, i );
}


void BaseEnt::destructor( bool willDelete )
{

	int i;
	// delete all the children
	for( i=0; i<child.size(); i++ )
	{
		child[i]->destructor();
	}

	// this is for possible future use.
	//for( i=0; i<rules.size(); i++ )
	//	if( rules[i]->parent == this )
	//		rules[i].getRule()->endUse(); // this function doesnt do anything


	if( parent )
	{
		// clear our parent's pointer to us
		//remove( parent->child.begin(), parent->child.end(), this );
	}
	// This checks and if the object isn't being deleted somewhere else, it deletes it.
	#if DEBUG
	isDeleted = true;
	#endif
}

RulePointer::RulePointer()
{
	rule = NULL;
	remainingPropogates = 0;
}

#endif
