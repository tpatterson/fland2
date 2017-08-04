#ifndef FLAND_CPP
#define FLAND_CPP

using namespace std;
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#include "glutenv.h"
#include "fland.h"
#include "defines.h"
#include "baseent.h"
#include "fmlparser.h"
#include "vertex.h"
#include "meshseed.h"
#include "buildhtml.h"
#include "camera.h"
#include "defines.h"

Fland* Fland::fland;

void Fland::dumpObjectsToFile( string fileName )
{
	ofstream logFile( fileName.c_str() );
	getFlandPtr()->dump( logFile, 0 );
	logFile.close();
}

void Fland::dump( ostream& out, int depth )
{
	#if DEBUG
	if( firstEnt )
	{
		out << "\nDumping every object in fland! elapsed time: " << getElapsedTime() << endl;
		firstEnt->dump( out, depth );
	}
	#endif
	int deepest = 0;
	out << "\nClosest Render Distance: " << closestRenderDistance
		  << "\nFarthest Render Distance: " << farthestRenderDistance
		  << "\nNumber of entities: " << firstEnt->getCount( deepest )
		  << "\nDeepest Entity Depth: " << deepest
		  << "\nSplit Visibility: " << splitVisibility << endl;
}


Fland::Fland( int inSeed )
{
	maxCreates = 100;
	static int numTimes = 0;

	fland = this;

	//if( numTimes == 0 )
	//{
		cout << "\n\n\n\nRealtime Fractal Landscape Generator\n";
		cout << "By Tyler Patterson\n";
		cout << "Compiled on " __DATE__ " at " __TIME__ << endl;
		cout << "Using seed " << inSeed << endl;
	//}
	numTimes++;
	cout << endl;

	morphPercentage = MORPH_AMOUNT;

	// First parse the rules
	// Do it in a try block to make sure everything is OK.
	try
	{
		fmlParser = new FmlParser( "main.fml" );
	}
	catch( FmlParserFatalError ex )
	{
		cout << ex.what() << endl;
		// Dont clean anything up, just rethrow the error and let the
		// system call all the destructors as it exits.
		throw;
	}

	setSplitVisibility( DEFAULT_SPLIT_VISIBILITY );
	closestRenderDistance = 1.0; // set it for fun, it will be set later.
	farthestRenderDistance = 10.0; // dito.
	targetFrameRate = DEFAULT_TARGET_FRAMERATE; // the goal.

	// Create the camera;
	camera = new Camera;
	// The first entity has to be set by hand.


	firstEnt = MeshSeed::reuse.tNew( NULL );
	MSEED(firstEnt)->setMeshParams( 0.0, 0.0, 0.0, 1.0 );

	// This is the split rule for the firstEnt.
	//BaseRule* fRule = reusePlaneR.tNew( firstEnt );
	// Set the firstent rule
	firstEnt->seed = inSeed;
	//firstEnt->splitRule = fRule;

	if( fmlParser->nameMap[ ROOT_RULE_NAME ] )
	{
		// create a pointer to the first rule
		BaseRule* fRule;
		// Create the first rule of the root entity firstEnt.
		fRule = fmlParser->nameMap[ ROOT_RULE_NAME ];
		assert( fRule );
		firstEnt->addRule( fRule, fRule->numPropogates, 1.0 );
	}
	else
	{
		cout << "No rule named " << ROOT_RULE_NAME << " found in FML files!\n";
		LN
	}

}

Fland::~Fland()
{
	// empty out the reuse objects
	MeshSeed::reuse.empty();
	Vertex::reuse.empty();
	//reuseBaseEnt.empty();

	// Kill everything
	if( firstEnt )
	{
		delete firstEnt;
	}
	delete camera;
	delete fmlParser;
	fland = NULL;
}

int Fland::recalculateView()
{
	Fland* fl = getFlandPtr();
	double frameDiscrepancy = targetFrameRate / fl->env.frameRate;
//	splitVisibility = ( splitVisibility + splitVisibility * frameDiscrepancy ) / 2.0;
//	if( frameDiscrepancy > 1.0 ) // if we are too detailed
//		splitVisibility *= frameDiscrepancy; // take detail away quick
//	else // add detail slowly
//	splitVisibility *= ( frameDiscrepancy - 1.0 ) / FRAME_CHANGE_DAMPENER + 1.0;



	//if( splitVisibility > 0.2 )
	//	splitVisibility = 0.2;
	//if( splitVisibility < 0.0001 )
	//	splitVisibility = 0.0001;

	//camera->viewAngle = 15;
	camera->updateLocation();
	if( firstEnt )
	{
		// This is the distance to the clostest rendered entity.
		// start it out high so it will be guranteed to be
		// set to the correct value in BaseEnt::calcVisibility
		closestRenderDistance = 99999999999.9;
		farthestRenderDistance = 0.0;


		firstEnt->recursiveCalcVisibility();

		// a little debug thing. Stop spliting and un-spliting while spacebar is pressed.
		if( fl->env.keys[' '] == false )
		{
			firstEnt->recursiveSplit();
			// tell firstEnt's children to unsplit. We dont want to un-split the base
			// entity or the whole world will go away!

			// This makes it impossible for the root entity to be unsplit.
			// It was causing the mesh to never split farther and become 'stuck'
			for( int i=0; i<firstEnt->child.size(); i++ )
				firstEnt->child[i]->recursiveUnSplit();

			// This makes it so that even the root entity can be unsplit
			// and the whole thing goes away.
			//firstEnt->recursiveUnSplit();

		}
	}
	fl->env.recalculateFramerate();
	// set the near and far planes.
	// Make them a hair larger then they need to be, just so things dont
	// lie exactly on the line and cause flickering.
	//camera->near = closestRenderDistance * 0.99;
	//camera->far = farthestRenderDistance * 1.01;
	camera->setNearFarPlanes( closestRenderDistance, farthestRenderDistance );
	//camera->printFrustum();
	return 0;
}

int Fland::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);

	camera->viewAngle = 30.0;
	//camera->near = 0.001;
	//camera->far = 10;
	camera->printFrustum();

	if( firstEnt )
	{
		firstEnt->draw();
	}
	//glTranslatef( 0, 1, 0 );
	//glRotatef( fl->env.frameCount, 0, 0, 1 );
	//glutSolidTeapot( .1 );
	swapBuffers();
	return 0;
}

void Fland::buildRuleLayout()
{
	BuildDiagnosticHtml( this );

}


#endif
