#ifndef BASEOBJ_H
#define BASEOBJ_H

using namespace std;

#include <string>
#include "defines.h"

class BaseObj
{
	public:
	// because we are using the reuse classes, every class
	// has to know how to delete itself. (which reuse class
	// to tell to delete.)
	// This function also deletes the child's rules, and any children it has.

	// Note: willDelete is used to see if the leaf class has been overloaded
	// and will delete this object. If not, baseObj deletes it.
	virtual void destructor( bool willDelete = false );

	#if( DEBUG )
	virtual string getType();
	#endif
	virtual string toString( int ) = 0;
};

#endif
