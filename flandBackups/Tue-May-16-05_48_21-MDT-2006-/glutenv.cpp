#ifndef GLUTENV_CPP
#define GLUTENV_CPP

using namespace std;
#include <GL/glut.h>

#include "main.h"
#include "events.h"
#include "defines.h"
#include "glutenv.h"

// These functions are to be used by the main program
// If you want to use a system other then GLUT you need to rewrite this file
// keeping these functions the same.
// ALSO: Make sure the callback functions have the right parameters.


// These functions use the GLUT environment.

float getElapsedTime()
{
	return glutGet( GLUT_ELAPSED_TIME ) / 1000.0;
}

// abstract the glutSwapBuffers()
int swapBuffers()
{
	glutSwapBuffers();
	return 0;
}

// The outside program calls this function to initalize everything.
int initEnv(int argc, char** argv)
{
	// this is in this file
   initGraphics(argc, argv);
   initProgram();

   glutMainLoop();

   // THE PROGRAM NEVER EVEN GETS HERE!
	return 0;
}

// sets the callback functions for event processing.
int setCallbacks()
{
	glutMouseFunc( gMouse );
	glutMotionFunc( gMouseMove );
   glutDisplayFunc(gDisplay);
   glutIdleFunc(gIdle);
   glutReshapeFunc(gReshape);
   glutKeyboardFunc(gKeyboard);
   glutKeyboardUpFunc(gKeyboardUp );
	glutPassiveMotionFunc(gPassiveMotionFunc);

	return 0;
}

int setupMenu()
{
	// in events.h is the enum of the options
	glutCreateMenu(gMenu);
	//glutAddSubMenu("Camera", 90);
	glutAddMenuEntry("Full Screen", FULL_SCREEN);
	glutAddMenuEntry("Reset View", RESET_VIEW);
	glutAddMenuEntry("Dump Object Parameters", DUMP_SPECS);
	glutAddMenuEntry("Dump Environment Specs", DUMP_ENVIRONMENT);
	glutAddMenuEntry("Reset Fland", RESET_FLAND );
	glutAddMenuEntry("Randomize Fland", RANDOMIZE_LAND);
	glutAddMenuEntry("Quit", QUIT_PROGRAM);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	return 0;
}

// This function sets up the graphics.
int initGraphics(int argc, char** argv)
{

	glutInit(&argc, argv);
   glutInitDisplayMode ( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (DEFULT_WINDOW_WIDTH, DEFULT_WINDOW_HEIGHT); // these vals are defined in defines.h
   glutInitWindowPosition ( 100, 10 );
   glutCreateWindow ("Fractal Land");

   setCallbacks();
	setupMenu();

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	//glShadeModel(GL_FLAT);
	glClearColor(0.0, 0.0, 0.0, 0.5);				// Black Background
	glClearDepth(1.0);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	glEnable(GL_COLOR_MATERIAL);    // this is necessary. Dont ask why
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);					// Set The Blending Function For Translucency
	glEnable(GL_BLEND);

	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);	// Really fast Perspective Calculations

   GLfloat LightSpecular[]= { 1.0f, 1.0f, 1.0f, 1.0f }; 				// Ambient Light Values
   GLfloat LightShininess[]= { 50.0 }; 				// Ambient Light Values
   GLfloat LightAmbient[]= { 0.2f, 0.2f, 0.2f, 1.0f }; 				// Ambient Light Values
	GLfloat LightDiffuse[]= { 0.7f, 0.7f, 0.7f, 1.0f };				 // Diffuse Light Values
	GLfloat LightPosition[]= { 0.0f, 0.0f, 0.0f, 1.0f };				 // Light Position

	//glLightfv(GL_LIGHT1, GL_SPECULAR, LightAmbient);
	//glLightfv(GL_LIGHT1, GL_SHININESS, LightShininess );
   // glMaterialfv(GL_FRONT, GL_SPECULAR, LightSpecular);
   // glMaterialfv(GL_FRONT, GL_SHININESS, LightShininess);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);				// Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);				// Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);			// Position The Light
	glEnable(GL_LIGHT1);							// Enable Light One
	glEnable(GL_LIGHTING);

	glEnable( GL_FOG );
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
	glHint( GL_FOG_HINT, GL_NICEST );

   return 0;
}

void drawSphere( double x, double y, double z, double r )
{
	glPushMatrix();
	glTranslatef( x, y, z );
	glColor4f( 1.0, 0.5, 0.5, 0.5 );
	glDisable( GL_DEPTH_TEST );
	glutSolidSphere( r, 10, 5 );
	glEnable( GL_DEPTH_TEST );
	glPopMatrix();
}


#endif
