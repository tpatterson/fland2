#ifndef _TEMPLATE_H
#define _TEMPLATE_H

using namespace std;


#include <iostream>
#include <vector>


// Reuse is used to create a stack of dead objects so we dont have to keep
// dynamically creating and deleting them.
// it makes an auto growing stack and puts the dead object pointers on it.
// then when an object is needed, it pops its pointer off the stack.

// B is the base class object type,
// D is the derived class type. They could be the same thing.
#define START_STACK_SIZE 1000

class BaseEnt;
template< class B, class D >
class Reuse
{
	public:
	Reuse( int = START_STACK_SIZE );
	~Reuse();
	void empty();

	B* tNew( BaseEnt* = NULL ); // create a new object. Optionally pass the parent in.
	void tDelete( B* );

	private:
	vector< B* > stack;
};

template< class B, class D >
void Reuse< B, D >::empty()
{
	for( int i=0; i<stack.size(); i++ )
	{
		delete stack[i];
	}
	stack.clear();
}

template< class B, class D >
Reuse< B, D >::~Reuse()
{
	empty();
}

template< class B, class D >
Reuse< B, D >::Reuse( int inSize )
{
	//stack.resize( inSize );
	for( int i=0; i< START_STACK_SIZE; i++ )
	{
		stack.push_back( new D );
	}
}


template< class B, class D >
void Reuse< B, D >::tDelete( B *inVal )
{
	// THIS IS THE REAL FUNCTION
	stack.push_back( inVal );
	// this is for debugging
	//delete inVal;
}

template< class B, class D >
B* Reuse< B, D >::tNew( BaseEnt *inParent )
{
	B *ptr;
	if( stack.size() > 0 )
	{
		ptr = stack[ stack.size() - 1 ];
		stack.pop_back();
	}
	else
	{
		ptr = new D;
		if( ptr == NULL ) // if the allocation failed
		{
			cout << "Couldn't allocate more memory" << endl;
			exit( 0 );
		}
	}
	ptr->constructor( inParent );
	return( ptr );
}

#endif
