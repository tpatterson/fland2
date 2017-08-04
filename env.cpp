#ifndef ENV_CPP
#define ENV_CPP
// Environment: This class is for keeping track of the
// environment that the program is running in.
// Things like framerate, current keys states, mousestate, etc.

using namespace std;

#include <iostream>
#include "env.h"
#include "glutenv.h"

Env::Env()
{
	mouseX = mouseY = mouseButton = 0;

	for( int i=0; i<256; i++ )
	{
		keys[i] = false;
	}
	timeSlice = 0.1; // has to start somewhere
	frameCount = 0; // the number of frames elapsed
	frameRate = 0.0;
	lastFrameTime = 0.0;
	// set these to something, will be set again later.
	screenWidth = 100;
	screenHeight = 100;
}

void Env::printVars()
{
	cout << "\nFrameRate: " << frameRate
		  << "\nTime Slice: " << timeSlice
		  << "\nFrame Count: " << frameCount
		  << "\nElapsed Time: " << lastFrameTime << endl;
}

void Env::recalculateFramerate()
{
	double currentTime = getElapsedTime();

	// Calculate the timeSlice
	timeSlice = currentTime - lastFrameTime;
	lastFrameTime = currentTime;

	// Calculate the framerate
	frameRate = 1.0 / timeSlice;

	// if the timeslice is more then 1 second, stick it to 1 second.
	if( timeSlice > 1.0 )
		timeSlice = 1.0;

	// increment the frameCount
	frameCount++;
}

#endif
