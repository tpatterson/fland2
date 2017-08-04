#ifndef GLUTENV_H
#define GLUTENV_H

using namespace std;
#include "glut.h"

#include "main.h"
#include "events.h"
#include "defines.h"

// These functions are to be used by the main program
// If you want to use a system other then GLUT you need to rewrite this file
// keeping these functions the same.
// ALSO: Make sure the callback functions have the right parameters.

int initEnv(int argc, char** argv);
int swapBuffers();

int setCallbacks();
int setupMenu();
int initGraphics(int argc, char** argv);
float getElapsedTime(); // returns the number of seconds elapsed since the prog started.
void drawSphere( double x, double y, double z, double r );


#endif

