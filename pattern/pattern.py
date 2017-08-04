#!/usr/bin/python2.4

import os
import re
import sys
import md5

from numpy import *
from layer import *

class Pattern:
    def __init__( self ):
        self.hasReturned = 0
        self.retLayer = 0
        self.commandString = ''
        
    def run( self, commandString = '', fileName = 'pattern/temp', returnType = 'file' ):
        """
            Loads and processes a single pattern by evaling the command string.
        """
        # the retLayer is the layer that gets returned from the layer code
        self.commandString = commandString
        if( commandString == '' ):
            commandString = self.getCommandString( "pattern/temppatterninfo/" )
        self.execCommandString( commandString )
        if( self.hasReturned == 0 ):
            print "Pattern.run(): The commandString did not call returnLayer! We dont have any layer data to return!"
        if( returnType == 'return' ):
            return self.retLayer
        else:
            # TODO: save the data off as a file
            layerData = self.retLayer.getOctaveDataString()




    def getCommandString( self, path ):
        """
            Returns the command string. As of now it reads the first
            non - hidden file in the path passed in and evals the contents.
        """
        for fileName in os.listdir( path ):
            # skip hidden files
            if re.compile( '\.' ).match( fileName ):
                continue
            # assume the first file read is the one we want.
            print "Scanning " + path + fileName
            f = open( path + fileName )
            commandString = f.read()
            f.close()
            break # we only want one file at a time.
            return commandString


    def execCommandString( self, commandString ):
        """ 
            Executes the command string. The eval'ed code
            has access to all members of this class.
            This has to call returnLayer at the end!
        """
        exec( commandString )

    def returnLayer( self, layer ):
        self.retLayer = layer
        self.hasReturned = 1
            
    def returnUniqueIdentifier( self ):
        num = 0
        return( md5.new( self.commandString ) )
            

    def saveString( self, fileName, dataString ):
        """
            saves the string data to a file
        """
        f = open( fileName, 'w' )
        f.write( dataString )
        f.flush()
        f.close()


