#ifndef AVERAGER_CPP
#define AVERAGER_CPP
// This rule takes a vertex and sticks it between it's parent and it's aunts.
// in all attributes that apply.
// (position, color)

using namespace std;

#include "baserule.h"
#include "defines.h"
#include "tyler.h"
#include "vertex.h"
#include "templates.h"
#include "averager.h"
#include <typeinfo>

#if( DEBUG )
string AverageR::getType(){ return "AverageR"; }
string AverageR::toString( int ind ){ return indent( ind ) + "AverageR::\n" + BaseRule::toString( ind ); }
#endif

string AverageR::assignValue( string valueType, istringstream *value, FmlParser* fPtr )
{
	if( valueType == "test1" )
	{
		// *value >> test1;
	}
	else
	{
		return BaseRule::assignValue( valueType, value, fPtr );
	}
	return "";
}

AverageR::AverageR( )
	:BaseRule()
{
}



void AverageR::goCalcMe( BaseEnt* inClient, intensityType runIntensity )
{
	Vertex* inC; // just make a casted alias to inClient, so we dont have to cast it
					 // every time.
	sassert( inClient );
	inC = VERT( inClient );

	// These are the attributes that will be averaged.
	QUARD_TYPE ax, ay, az;
//	QUARD_TYPE au, av;
	//NORMAL_TYPE anx, any, anz; // normals
	ax = ay = az = 0.0;
//	au = av = 0.0;
	//anx = any = anz = 0.0;
	Color col;

	// Loop through each affect vertex and add all the attributes
	// (scaled by affectAmount) to the averaged attributes.
	int i=0;
	// if the prog seg-faults on the next line, chances are you are calling this
	// rule on a non-vertex entity!
	for( i=0; i<=3; i++ ) // while there is another affectVertex
	{
		if( inC->affectVertex[i] == NULL ){ break; }
		ax += inC->affectAmount[i] * VERT(inC->affectVertex[i])->trueVertex.x;
		ay += inC->affectAmount[i] * VERT(inC->affectVertex[i])->trueVertex.y;
		az += inC->affectAmount[i] * VERT(inC->affectVertex[i])->trueVertex.z;
	//	au += inC->affectAmount[i] * VERT(inC->affectVertex[i])->u;
	//	av += inC->affectAmount[i] * VERT(inC->affectVertex[i])->v;

		col.red += inC->affectAmount[i] * VERT(inC->affectVertex[i])->trueVertex.red;
		col.green += inC->affectAmount[i] * VERT(inC->affectVertex[i])->trueVertex.green;
		col.blue += inC->affectAmount[i] * VERT(inC->affectVertex[i])->trueVertex.blue;


	}
	// now set the client attributes to the averaged attributes.
	if( i > 0 ) // but only if there were any affect verticies.
	{
		VERT(inC)->trueVertex.x = ax;
		VERT(inC)->trueVertex.y = ay;
		VERT(inC)->trueVertex.z = az;

		VERT(inC)->trueVertex.red = col.red;
		VERT(inC)->trueVertex.green = col.green;
		VERT(inC)->trueVertex.blue = col.blue;
	//	VERT(inC)->u = au;
	//	VERT(inC)->v = av;

		inC->bound.x = ax;
		inC->bound.y = ay;
		inC->bound.z = az;

		//VERT(inC)->startMorphVertex = VERT(inC)->trueVertex;
		//VERT(inC)->startMorphVertex.nx = anx;
		//VERT(inC)->startMorphVertex.ny = any;
		//VERT(inC)->startMorphVertex.nz = anz;
	}
}

#endif

