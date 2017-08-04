#!/usr/bin/env python

#
# This code was created by Richard Campbell '99 (ported to Python/PyOpenGL by John Ferguson 2000)
#
# The port was based on the PyOpenGL tutorial module: dots.py  
#
# If you've found this code useful, please let me know (email John Ferguson at hakuin@voicenet.com).
#
# See original source and C based tutorial at http://nehe.gamedev.net
#
# Note:
# -----
# This code is not a good example of Python and using OO techniques.  It is a simple and direct
# exposition of how to use the Open GL API in Python via the PyOpenGL package.  It also uses GLUT,
# which in my opinion is a high quality library in that it makes my work simpler.  Due to using
# these APIs, this code is more like a C program using function based programming (which Python
# is in fact based upon, note the use of closures and lambda) than a "good" OO program.
#
# To run this code get and install OpenGL, GLUT, PyOpenGL (see http://www.python.org), and PyNumeric.
# Installing PyNumeric means having a C compiler that is configured properly, or so I found.  For 
# Win32 this assumes VC++, I poked through the setup.py for Numeric, and chased through disutils code
# and noticed what seemed to be hard coded preferences for VC++ in the case of a Win32 OS.  However,
# I am new to Python and know little about disutils, so I may just be not using it right.
#
# BTW, since this is Python make sure you use tabs or spaces to indent, I had numerous problems since I 
# was using editors that were not sensitive to Python.
#
# Modified on May 2nd,2004 by Travis Wells to fix some GLUT issues and missing import
from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *
import sys
import gtk
# Some api in the chain is translating the keystrokes to this octal string
# so instead of saying: ESCAPE = 27, we use the following.
ESCAPE = '\033'

window = 0
glutObject = 0
viewer = 0

# The function called when our window is resized (which shouldn't happen if you enable fullscreen, below)
def glutWindowReSizeGLScene(Width, Height):
    if Height == 0:						# Prevent A Divide By Zero If The Window Is Too Small 
        Height = 1

    glViewport(0, 0, Width, Height)		# Reset The Current Viewport And Perspective Transformation
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluPerspective(45.0, float(Width)/float(Height), 0.1, 100.0)
    glMatrixMode(GL_MODELVIEW)

# The main drawing function. 
def glutWindowDrawGLScene():
    glutObject.draw()

# The function called whenever a key is pressed. Note the use of Python tuples to pass in: (key, x, y)  
def glutWindowKeyPressed(*args):
    # If escape is pressed, kill everything.
    if args[0] == ESCAPE:
        glutDestroyWindow(window)
        sys.exit()

def glutWindowMouseDown( button, state, x, y ):
    if( button == 4 ):
        glutObject.viewDistance -= .1
        glutObject.draw()
        return
    if( button == 3 ):
        glutObject.viewDistance += .1
        glutObject.draw()
        return

    glutObject.mouseStartX = x
    glutObject.mouseStartY = y
    glutObject.mouseX = x
    glutObject.mouseY = y
    glutObject.startRotateZ = glutObject.rotateZ
    glutObject.startRotateX = glutObject.rotateX
    if( state == 0 ):
        glutObject.isMouseDown = 1
        glutObject.draw()
    else:
        glutObject.isMouseDown = 0


def glutWindowMouseMove( x, y ):
    glutObject.mouseX = x
    glutObject.mouseY = y




class glutWindow(object):
    """
        creates a GLUT window and stores and renders its data.
    """

    def initView( self ):
        # used in mouse rotation stuff
        self.rotateZ = 0.0
        self.rotateX = 0.0
        self.startRotateZ = 0.0
        self.startRotateX = 0.0
        self.mouseX = 0.0
        self.mouseY = 0.0
        self.mouseStartX = 0.0
        self.mouseStartY = 0.0
        self.isMouseDown = 0

        self.viewDistance = -2.0

        # precompute some stuff
        self.scale = 0.0
        self.halfWidth = 0.0
        self.halfHeight = 0.0       

    # A general OpenGL initialization function.  Sets all of the initial parameters. 
    def InitGL(self, Width, Height):				# We call this right after our OpenGL window is created.
        
        self.initView()

        glClearColor(0.0, 0.0, 0.0, 0.0)	# This Will Clear The Background Color To Black
        glClearDepth(1.0)					# Enables Clearing Of The Depth Buffer
        glDepthFunc(GL_LEQUAL)				# The Type Of Depth Test To Do
        glEnable(GL_DEPTH_TEST)				# Enables Depth Testing
        glShadeModel(GL_SMOOTH)				# Enables Smooth Color Shading
        #glShadeModel(GL_FLAT)				# Enables Smooth Color Shading
        
    	glEnable(GL_COLOR_MATERIAL);        # this is necessary. Dont ask why
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()					# Reset The Projection Matrix
                                            # Calculate The Aspect Ratio Of The Window
        gluPerspective(45.0, float(Width)/float(Height), 0.1, 100.0)

        glMatrixMode(GL_MODELVIEW)


        LightSpecular= [ 1.0, 1.0, 1.0, 1.0 ]; 				# Ambient Light Values
        LightShininess= [ 50.0 ]; 				                # Ambient Light Values
        LightAmbient= [ 0.2, 0.2, 0.2, 1.0 ]; 				# Ambient Light Values
        LightDiffuse= [ 0.7, 0.7, 0.7, 1.0 ];				    # Diffuse Light Values
        LightPosition= [ -1.0, 0.0, 0.0, 1.0 ];				# Light Position

        glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);				# Setup The Diffuse Light
        glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);				# Setup The Ambient Light
        glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);			# Position The Light
        glEnable(GL_LIGHT1);							            # Enable Light One
        glEnable(GL_LIGHTING);



    def __init__( self ):
        self.gtkInitalized = 0
        self._layer = 0
        self.isReady = 0
    
    def getLayer( self ):
        return self._layer

    def setLayer( self, var ):
        self._layer = var
        self.scale = 1.0 / self.layer.width
        self.halfWidth = self.layer.width / 2.0
        self.halfHeight = self.layer.height / 2.0
        self.draw()
        print "The layer has been set yo-digity!!!!"

    layer = property( getLayer, setLayer )

    def draw( self, reset = 0 ):
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        glLoadIdentity()					# Reset The View 

        # Move Left 1.5 units and into the screen 6.0 units.
        glTranslatef(0.0, 0.0, self.viewDistance )
        glRotatef( self.rotateX, 1.0, 0.0, 0.0 )
        glRotatef( self.rotateZ, 0.0, 0.0, 1.0 )

        glTranslatef( 0.0, 0.0, 0.0 )

        layer = self.layer
        if( not layer == 0 ):

            if( layer.displayList > 0 and reset == 0 ):
                glCallList( layer.displayList )
            else:
                layer.displayList = glGenLists(1)
                glNewList( layer.displayList, GL_COMPILE )

                # calculate the scale stuff
                self.viewThreshold = -1
                if( viewer.checkThreshold.get_active() ):
                    self.viewThreshold = ( viewer.scaleThreshold.get_value() * 0.01 ) *\
                    ( layer.data.max() - layer.data.min() ) * layer.width + layer.data.min() \
                    * layer.width

                scale = 1.0 / layer.width
                for x in range( layer.width - 1 ):
                    glBegin( GL_TRIANGLE_STRIP )
                    for y in range( layer.height ):
                        xPos = self.getRenderX( x )
                        yPos = self.getRenderY( y )
                        zPos = layer.getVal( x, y ) * layer.width
                        self.printColor( zPos )
                        n = layer.getNormal( x, y )
                        glNormal3f( n[0], n[1], n[2] )
                        glVertex3f( xPos, yPos, zPos )
                        
                        xPos = self.getRenderX( x+1 )
                        zPos = layer.getVal(x+1, y) * layer.width
                        self.printColor( zPos )
                        n = layer.getNormal( x+1, y )
                        glNormal3f( n[0], n[1], n[2] )
                        glVertex3f( xPos, yPos, zPos )
                    glEnd()
                
                
                if( viewer.checkWireframe.get_active() ):
                    above = 0.005

                    glColor3f( 0.0, 0.0, 0.0 )
                    #glDisable( GL_DEPTH_TEST )
                    glBegin( GL_LINES )
                    for x in range( layer.width - 1 ):
                        for y in range( layer.height - 1 ):
                            # wireframe
                            if( 0 ):
                                glColor3f( 0, 0, 0 )
    #                            glVertex3f( self.getRenderX( x ), self.getRenderY( y ),
    #                            self._layer.getVal( x, y ) + above )
    #                            glVertex3f( self.getRenderX( x+1 ), self.getRenderY( y ),
    #                            self._layer.getVal( x+1, y ) + above )
                                glVertex3f( self.getRenderX( x ), self.getRenderY( y ),
                                self._layer.getVal( x, y ) * layer.width + above )
                                glVertex3f( self.getRenderX( x ), self.getRenderY( y+1 ),
                                self._layer.getVal( x, y+1 ) * layer.width + above )

                            # the normals
                            if( 1 ):
                                glColor3f( 1, 0, 0 )
                                glVertex3f( self.getRenderX( x ), self.getRenderY( y ), self._layer.getVal(x, y) * layer.width )
                                n = layer.getNormal( x, y )
                                glNormal3f( n[0], n[1], n[2] )
                                glVertex3f( self.getRenderX( x ) + n[0] * .03,
                                self.getRenderY( y ) + n[1] * .03,
                                self._layer.getVal(x, y) * layer.width + n[2] * .03 )


                    glEnd()

                glEndList()
                glCallList( layer.displayList )
                #glEnable( GL_DEPTH_TEST )
                    
        #  since this is double buffered, swap the buffers to display what just got drawn. 
        glutSwapBuffers()

    def getRenderX( self, x ):
        return( x - self.halfWidth ) * self.scale

    def getRenderY( self, y ):
        return( y - self.halfHeight ) * self.scale

    def printColor( self, z ):
        """
            pass the render quardinates and this
            will return color to render it in.
        """
        if( not self.viewThreshold == -1 ):
            if( z < self.viewThreshold ):
                glColor3f( 0.0, 1.0 , z+0.5 )
            else:
                glColor3f( 1.0, 0.0, z+0.5 )
        else:
            glColor3f( z+0.5, z+0.5, 1.0 )
            

    # Starts the GLUT window. GLUT gets control of the opperation thread
    # pass it the viewer object.
    def run( self, viewerInput ):
        print "Starting GLUT window... 123"
        print "viewerInput %s" %viewerInput

        #return()
        global window
        global glutObject
        global viewer

        viewer = viewerInput
        glutObject = self
        # For now we just pass glutInit one empty argument. I wasn't sure what should or could be passed in (tuple, list, ...)
        # Once I find out the right stuff based on reading the PyOpenGL source, I'll address this.
        glutInit(())

        # Select type of Display mode:   
        #  Double buffer 
        #  RGBA color
        # Alpha components supported 
        # Depth buffer
        glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH)
        
        # get a 640 x 480 window 
        glutInitWindowSize(640, 480)
        
        # the window starts at the upper left corner of the screen 
        glutInitWindowPosition(500, 200)
        
        # Okay, like the C version we retain the window id to use when closing, but for those of you new
        # to Python (like myself), remember this assignment would make the variable local and not global
        # if it weren't for the global declaration at the start of main.
        self.window = glutCreateWindow("Fland Pattern Viewer")

        # Register the drawing function with glut, BUT in Python land, at least using PyOpenGL, we need to
        # set the function pointer and invoke a function to actually register the callback, otherwise it
        # would be very much like the C version of the code.	
        glutDisplayFunc( glutWindowDrawGLScene )
        
        # Uncomment this line to get full screen.
        #glutFullScreen()
        glutMouseFunc( glutWindowMouseDown );
        glutMotionFunc( glutWindowMouseMove );

        # When we are doing nothing, redraw the scene.
        glutIdleFunc( self.idleFunction )
        
        # Register the function called when our window is resized.
        glutReshapeFunc( glutWindowReSizeGLScene)
        
        # Register the function called when the keyboard is pressed.  
        glutKeyboardFunc( glutWindowKeyPressed )

        # Initialize our window. 
        self.InitGL(640, 480)

        self.isReady = 1

        # Start Event Processing Engine	
        glutMainLoop()

    def idleFunction( self ):
        if( glutObject.isMouseDown ):
            glutObject.rotateZ = glutObject.startRotateZ + ( glutObject.mouseX - glutObject.mouseStartX ) * 1.0
            glutObject.rotateX = glutObject.startRotateX + ( glutObject.mouseY - glutObject.mouseStartY ) * 1.0
            self.draw()
        else:
            gtk.main_iteration()

