#ifndef EVENTS_CPP
#define EVENTS_CPP
// This file has all the callback functions for event handling.

#include "glut.h"
#include <fstream>
#include <iostream>
#include <string>

#include "events.h"
#include "main.h"
#include "fland.h"
#include "camera.h"
#include "baseent.h"
#include "buildhtml.h"
#include "glutenv.h"

using namespace std;



// When the window is resized.
void gReshape(int width, int height)
{
	Fland *fl = getFlandPtr();
	fl->env.screenWidth = width;
	fl->env.screenHeight = height;
	glViewport(0, 0, (GLint) width, (GLint) height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glFrustum(-1.0, 1.0, -h, h, 3.0, 600.0);

	gluPerspective(25.0,(GLdouble)width/(GLdouble)height, 0.000003, 600.0);
	glMatrixMode(GL_MODELVIEW);
}

void gIdle()
{
	mainLoop();
	//glutPostRedisplay();
}

void gDisplay()
{

}

void gKeyboard(unsigned char key, int x, int y)
{

	Fland* fl = getFlandPtr();
	if( fl )
		fl->env.keys[ key ] = true; // set the key variable
	switch( key )
	{
		case 'q':
		case 27:
			closeProgram();
		break;

		case 'e':
			fl->env.printVars();
			break;

		case 't':
			//cout << fl->camera->toString( 1 );
			resetFland( (int)(getElapsedTime() * 1000) );
			break;

		case '=':
			fl->setSplitVisibility( fl->getSplitVisibility() / 1.2 );
			cout << "splitVisibility: " << fl->getSplitVisibility() << endl;
			break;

		case '-':
			fl->setSplitVisibility( fl->getSplitVisibility() * 1.2 );
			cout << "splitVisibility: " << fl->getSplitVisibility() << endl;
			break;

		case ' ':
			glDisable( GL_FOG );
			break;

		case '[':
			fl->morphPercentage -= .1;
			if( fl->morphPercentage < 0.0 )
				fl->morphPercentage = 0.0;
			cout << "morphPercentage " << fl->morphPercentage << endl;
			break;

		case ']':
			fl->morphPercentage += .1;
			if( fl->morphPercentage > 1.0 )
				fl->morphPercentage = 1.0;
			cout << "morphPercentage " << fl->morphPercentage << endl;
			break;

		case 'g':
			fl->firstEnt->printGeneology( 0 );
			break;

		case 'o': // dump the envirnment specs
			fl->dumpObjectsToFile( DUMP_FILE_NAME );
			break;

		case 'h':
			fl->buildRuleLayout();
			break;

		case 'p': // take a picture
			double saveSplit = fl->getSplitVisibility();
			fl->setSplitVisibility( saveSplit / 5.0 ); // make the visibility this much higher
			fl->recalculateView();
			//fl->dump( cout, 0 );
			fl->draw();
			fl->setSplitVisibility( saveSplit );
			pause( 4 );
			break;
	}
}

void gMouse(int button, int state, int x, int y)
{
	Fland* fl = getFlandPtr();
	fl->env.mouseX = x;
	fl->env.mouseY = y;
	fl->env.mouseButton = button;
}

void gMouseMove( int x, int y)
{
	Fland* fl = getFlandPtr();
	int moveX = x - fl->env.mouseX;
	int moveY = y - fl->env.mouseY;

	// This determines how the camera moves when you move the mouse.
	fl->camera->rotY += moveX * CAMERA_LOOK_SPEED;
	fl->camera->rotX -= moveY * CAMERA_LOOK_SPEED;


	fl->env.mouseX = x;
	fl->env.mouseY = y;
}

void gRepeatKeys()
{
	Fland* fl = getFlandPtr();
	// move forward
	if( fl->env.keys['w'] )
	{
		fl->camera->speed += CAMERA_MOVE_SPEED;
	}

	if( fl->env.keys['s'] )
	{
		fl->camera->speed -= CAMERA_MOVE_SPEED;
	}

	// move up and down
	if( fl->env.keys['r'] )
	{
		fl->camera->vertSpeed += CAMERA_MOVE_SPEED;
	}

	if( fl->env.keys['f'] )
	{
		fl->camera->vertSpeed -= CAMERA_MOVE_SPEED;
	}

	// move Side to side
	if( fl->env.keys['a'] )
	{
		fl->camera->sideSpeed += CAMERA_MOVE_SPEED;
	}

	if( fl->env.keys['d'] )
	{
		fl->camera->sideSpeed -= CAMERA_MOVE_SPEED;
	}
}

void gKeyboardUp( unsigned char key, int x, int y )
{
	Fland* fl = getFlandPtr();
	fl->env.keys[key] = false;

	// turn fog back on when the spacebar comes up
	if( key == ' ' )
		glEnable( GL_FOG );
}
void gPassiveMotionFunc(int x, int y)
{

}
//static void gMenu( int option )
//{
void gMenu( int option )
{
	Fland* fl = getFlandPtr();
	switch( option )
	{
		case DUMP_SPECS:
		{
			//ofstream logFile( LOG_FILE_NAME );
			fl->dump( cout, 0 );
			//logFile.close();
		}
			break;

		case QUIT_PROGRAM:
			closeProgram();
			break;

		case RESET_VIEW:
			fl->camera->resetView();
			break;

		case DUMP_ENVIRONMENT:
			fl->env.printVars();
			break;

		case RESET_FLAND:
			resetFland( 1 );
			break;

		case FULL_SCREEN:
			glutFullScreen();
			break;

		case RANDOMIZE_LAND:
			resetFland( fl->firstEnt->seed + 1 );
			break;

		default:
			cout << "Invalid menu option " << option << endl;
			LN
			break;
	}
}

#endif
