#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include "../perlinnoise.h"
#include <vector>


using namespace std;

inline double rnd()
{
	return( (double)rand() / (double)RAND_MAX );
}


struct SVertex
{
	GLfloat x,y,z;
	GLfloat r,g,b;
};

vector<SVertex> verticies;
vector<GLuint> indexVector;


PerlinNoise perlin;



int size = 200;
void doTest()
{   
    //glLoadIdentity();
    glTranslatef( 0.0, 0.0, 0.0 );

    int vertexNum = 0;
    verticies.resize( size * size );

    for( int x=0; x<size; x++ ){
            for( int y=0; y<size; y++ ){
                float rx = ( x / (float)size ) - 0.5;
                float ry = ( y / (float)size  ) - 0.5;
                double n;
                n = perlin.multipleNoise( 2.0, 5, (rx), (ry ), 0.0 ) + 0.5 ;
                verticies[vertexNum].x = rx;
                verticies[vertexNum].y = ry;
                verticies[vertexNum].z = 0.0;
                verticies[vertexNum].r = n;
                verticies[vertexNum].g = n;
                verticies[vertexNum].b = 1.0;
                //glColor3f( n, n, 1.0 );
                //glVertex3f( rx, ry, 0.0 );
                //cout << noise( rx, ry, 0.0 ) << p[x] << endl;
                ++vertexNum;
            }
    }

    indexVector.clear();
    for( int x=0; x<size-1; x++ ){
        for( int y=0; y<size-1; y++ ){
            indexVector.push_back((y)*size+(x)); 
            indexVector.push_back((y)*size+(x+1)); 
            indexVector.push_back((y+1)*size+(x+1)); 

            indexVector.push_back((y)*size+(x)); 
            indexVector.push_back((y+1)*size+(x+1)); 
            indexVector.push_back((y+1)*size+(x)); 
        }
    }

    cout << &verticies[0] << endl;
    cout << &(verticies[0].x) << endl;
    cout << &verticies[0].r << endl;

    glVertexPointer(    3,
                        GL_FLOAT,
                        sizeof(SVertex),
                        &verticies[0].x
                    );
    glColorPointer(    3,
                        GL_FLOAT,
                        sizeof(SVertex),
                        &verticies[0].r
                    );

    cout << "about to draw" << endl;
    // issue the "GO!" call
    glDrawElements( GL_TRIANGLES,
                    indexVector.size(),
                    GL_UNSIGNED_INT,
                    &indexVector[0]
            );
//    glDrawArrays(GL_POINTS,0,indexVector.size());
    cout << "drawn" << endl;

    glutSwapBuffers();
    cout << "test done." << endl;
}



// This function sets up the graphics.
void initGraphics(int argc, char** argv)
{

    glutInit(&argc, argv);
   glutInitDisplayMode ( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize ( 500, 500 ); // these vals are defined in defines.h   glutInitWindowPosition ( 100, 10 );
   glutCreateWindow ("Fractal Land");

   // enable vertex array stuff
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_COLOR_ARRAY );

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	//glShadeModel(GL_FLAT);
	glClearColor(0.0, 0.0, 0.0, 0.5);				// Black Background
	glClearDepth(1.0);									// Depth Buffer Setup
	//glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
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
	//glEnable(GL_LIGHT1);							// Enable Light One
	//glEnable(GL_LIGHTING);

	//glEnable( GL_FOG );
	//glFogi(GL_FOG_MODE, GL_LINEAR);
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
	//glHint( GL_FOG_HINT, GL_NICEST );

}

// When the window is resized.
void gReshape(int width, int height)
{
    if( height == 0 )						// Prevent A Divide By Zero If The Window Is Too Small 
        height = 1;

    glViewport(0, 0, width, height);		// Reset The Current Viewport And Perspective Transformation
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(0.0, float(width)/float(height), 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    doTest();
}

void gKeyboard(unsigned char key, int x, int y)
{
	switch( key )
	{
		case 'q':
		case 27:
            exit(0);
		break;
    }
}

// The outside program calls this function to initalize everything.
int initEnv(int argc, char** argv)
{
	// this is in this file
   initGraphics(argc, argv);
   glutReshapeFunc(gReshape);
   glutKeyboardFunc(gKeyboard);
   glutMainLoop();

   // THE PROGRAM NEVER EVEN GETS HERE!
	return 0;
}




int main(int argc, char** argv)
{
    initEnv(argc, argv);
    return 0;
}
