#ifndef EVENTS_H
#define EVENTS_H

using namespace std;

// events handles all events from GLUT and sends them to the right place

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

enum MENU_OPTIONS { QUIT_PROGRAM, DUMP_SPECS, RESET_VIEW, DUMP_ENVIRONMENT,
						  RESET_FLAND, FULL_SCREEN, RANDOMIZE_LAND };

void gKeyboard(unsigned char, int, int);
void gMouse(int, int, int, int);
void gMouseMove( int, int );
void gKeyboardUp( unsigned char, int, int );
void gRepeatKeys();
void gDisplay();
void gReshape(int w, int h);
void gIdle();
void gPassiveMotionFunc(int x, int y);
//static void gMenu( int option );
void gMenu( int option );



#endif
