#/usr/bin/python2.4 

import time
import thread
import gtk.glade
from numpy import *

import sys
sys.path.append( "pattern" )

from glutwindow import *
from pattern import * 

import gobject
gobject.threads_init()  

"""
        import gtk.glade 
        # 'Window' in the next line is unneeded if you
        # just want to import all widgets in the prog. You
        # would only add it if you wanted to import only
        # one window in a many window utility.
        xml = gtk.glade.XML("foo.glade", 'Window') 
        
        window = xml.get_widget('foo') 
        button = xml.get_widget('button') 
        
        button.connect('clicked', somefunction) 

        window.show_all()
        gtk.main()

        pygtk.org
"""


class Viewer:
    """
        the main viewer class.
    """

    def __init__( self ):
        xml = gtk.glade.XML( "pattern/viewer.glade" )
        self.isGlutWindowStarted = 0
        self.currentFile = "pattern/temppatterninfo/test1.py"
        
        # window 
        self.window = xml.get_widget( 'window1' )
        self.window.connect( 'destroy', self.close )
        #self.window.connect( 'activateMe', self.startGlutWindow )

        # goButton
        self.goButton = xml.get_widget( 'goButton' )
        self.goButton.connect( 'clicked', self.go )

        # textArea
        self.textArea = xml.get_widget( 'textview1' )

        #save button
        self.saveButton = xml.get_widget( 'buttonSave' )
        self.saveButton.connect( 'clicked', self.saveText )

        # wireframe button
        self.checkWireframe = xml.get_widget( 'checkWireframe' )
        self.checkWireframe.connect( 'clicked', self.viewStateChanged )
        
        # threshold button
        self.checkThreshold = xml.get_widget( 'checkThreshold' )
        self.checkThreshold.connect( 'clicked', self.viewStateChanged )
        # threshold slider
        self.scaleThreshold = xml.get_widget( 'scaleThreshold' )
        self.scaleThreshold.connect( 'button-release-event', self.viewStateChanged )

        #redraw button
        self.redrawButton = xml.get_widget( 'redrawButton' )
        self.redrawButton.connect( 'clicked', self.viewStateChanged )

        # load text into the textarea from a file
        f = open( self.currentFile )
        commandString = f.read()
        f.close()
        self.textArea.get_buffer().set_text( commandString )
        

        self.window.show_all()


        self.glutWindow = glutWindow()
        gtk.main()

    
    def close( self, temp = '' ):
        gtk.main_quit()

    def viewStateChanged( self, extra1 = '', extra2 = '', extra3 = '', extra4 = '' ):
        #print self.checkWireframe.get_active() 
        if( self.glutWindow.isReady ):
            #print "values: %s %s %s %s" %( extra1, extra2, extra3, extra4 )
            self.glutWindow.draw( reset = 1 )
        

    def go( self, temp = '' ):
        if( not self.isGlutWindowStarted == 1 ):
            self.isGlutWindowStarted = 1
            # TODO: emit a clicked signal here so that the first click 
            # to 'go' doesnt go un-answered
            self.glutWindow.run( self )

        # this stuff only gets executed after the first time the button is
        # pressed. The first time the stuff above takes over.
        text = self.textArea.get_buffer().get_text( self.textArea.get_buffer().get_start_iter(), self.textArea.get_buffer().get_end_iter() )
        print "Running text:\n%s" % text
        p = pattern.Pattern()
        lay = p.run( text, '', 'return' )
        self.glutWindow.layer = lay

    def file_open( self, temp = '' ):
        pass

    def saveText( self, temp = '' ):
        f = open( self.currentFile, 'w' )
        f.write( self.textArea.get_buffer().get_text( self.textArea.get_buffer().get_start_iter(), self.textArea.get_buffer().get_end_iter() ) )
        f.close()

view = Viewer()
print "done!"
