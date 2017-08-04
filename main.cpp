#ifndef MAIN_CPP
#define MAIN_CPP
// the tester program. Fland will have an interface and can be used in any program.
// compile it like this:
// tclear; g++ main.cpp -lGL -lGLU -lglut -ofland2
// NOTE: tclear is not necessary, it just clears the screen.

// NOTE: a perl script will insert the statements to include the other cpp files here.
// <START_CPP_INCLUES>
		// these files are now written to includecpps.cpp.
// <END_CPP_INCLUDES>

using namespace std;

#include <iostream>
#include <fstream>
// this must come before globals because globals uses it.
#include "defines.h"
#include "main.h"
#include "camera.h"
#include "perlinnoise.h"


// callback functions for event handling.
#include "events.h"
// the functions glut calls
#include "glutenv.h"

#include "fland.h"



// application enters here
int main(int argc, char** argv)
{
	// this is in glutenv.cpp
	initEnv(argc, argv);
	return 0;
}

// Called by the event system after the window and callback functions
// are all set up.
int initProgram()
{
    PerlinNoise::initalize();
	new Fland( 1 );
	return 0;
}

// Called by the event system each frame.
int mainLoop()
{
	gRepeatKeys(); // in events.h
	getFlandPtr()->fland->recalculateView();
	getFlandPtr()->fland->draw();

	return 0;
}

// called by the event system when the exit event gets called.
int closeProgram()
{
	delete Fland::fland;
	cout << "Done\n\n" << TEST_MESSAGE;
	exit( 0 );
	// the program never gets here.
	return 0;
}

void resetFland( int seed )
{
	Camera cam;
	cam = *(Fland::fland->camera);
	delete Fland::fland;
	new Fland( seed );
	*(Fland::fland->camera) = cam;

}


#endif

