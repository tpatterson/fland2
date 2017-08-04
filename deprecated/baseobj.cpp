#ifndef BASEOBJ_CPP
#define BASEOBJ_CPP

using namespace std;

#include <iostream>
#include "baseobj.h"

#if( DEBUG )
string BaseObj::getType()
{
	return "getType() not overloaded!";
}
#endif


void BaseObj::destructor( bool willDelete )
{

	if( !willDelete )
	{
		cout << "destructor is not overloaded in a leaf class of type:\n\t"
			  << getType()
			  << "\n\tCalled in BaseObj.\n\t"
			  << "Aborting." << endl;


			  // purposely seg fault so gdb can give us a stack trace
			  int* a = NULL;
			  *a = 0;
		delete this;
	}
}



#endif
