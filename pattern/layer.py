
import math
from numpy import *
from numpy.random import *
from numpy.lib import *
from numpy.core import *

seed( 1 )

class Layer:
    """
        A member of the layer class represents an image layer. 
        Many functions can be accessed.
    """
    def __init__( self, width, height, type=float ):
        """
            Creates a new pattern. All array values are zeros.
        """
        self.data = arange( width * height, dtype=type ).reshape( width, height )
        self.data.fill( 0 )
        self.width = width
        self.height = height
        self.type = type
        self.displayList = 0;

    def getVal( self, x, y ):
        """
            Returns an element from this layer. 
            Wraps at the edges back to the beginning!
        """
        while x < 0: 
            x += self.width
        while x >= self.width:
            x =- self.width
        while y < 0:
            y += self.height
        while y >= self.height:
            y -= self.height
        return self.data[x,y]

    def setVal( self, x, y, val ):
        """ 
            see getVal
        """
        while x < 0: 
            x += self.width
        while x >= self.width:
            x =- self.width
        while y < 0:
            y += self.height
        while y >= self.height:
            y -= self.height
        self.data[x,y] = val


    def addLayer( self, layer2 ):
        """
            adds the values of the array elements in layer2 into this layer.
        """
        pass

    def addNoise( self, frequency=1, amplitude=-1 ):
        """
            Adds 1 iteration of Perlin noise to the layer. 
            The layer has to be square power of 2.
            frequency must be a power of 2.
        """
        if( frequency > self.width ):
            print "FREQUENCY IS OUT OF RANGE!!!"
            return

        if( amplitude == -1 ):
            amplitude = 1.0 / frequency

        # stores normal vectors for each control point in the mesh.
        vector = arange( frequency ** 2 * 2, dtype=self.type ).reshape( frequency, frequency, 2 )
        for x in range(frequency):
            for y in range(frequency):
                direction = random() * pi * 2.0
                offset = 0.0
                if( amplitude > 0 ):
                    #offset = normal( 0, amplitude )
                    offset = amplitude * rand()

                #if( offset > 1.5 ):
                #    offset = 1.5
                vx = sin( offset ) * cos( direction )
                vy = sin( offset ) * sin( direction )
                vz = cos( offset )
                slopeX = vx / vz
                slopeY = vy / vz
                vector[x,y,0] = slopeX
                vector[x,y,1] = slopeY

        # width = 8
        # freq = 2
        step = self.width / frequency # 4
        for x in range( 0, self.width, step ): # 0, 4
            for y in range( 0, self.height, step ): # 0, 4
                xi = x/step # 0, 1
                xi2 = xi+1 # 1, 2
                if( xi2 >= frequency ): #makes xi2 be in front of xi by 1
                    xi2 = 0
                yi = y/step
                yi2 = yi+1
                if( yi2 >= frequency ):
                    yi2 = 0
                
                self._interpolate_noise( x, y, x+step, y+step, \
                vector[xi, yi, 0], vector[ xi, yi, 1 ], \
                vector[xi2, yi, 0], vector[ xi2, yi, 1 ], \
                vector[xi, yi2, 0], vector[ xi, yi2, 1 ], \
                vector[xi2, yi2, 0], vector[ xi2, yi2, 1 ] \
                )


    def _interpolate_noise( self, x1, y1, x2, y2,
    mx1, my1, mx2, my2, mx3, my3, mx4, my4 ):
        """
            pass it the x and y indicies and it will interpolate every value
            in between those square points. Used by addNoise().
            Does not do the far right and bottom rows, as these
            are done by the next iteration

            mx1, my1, mx2, my2, ... are the slope of surface
            at the four corners.

            # interpolates between two points in octave.
            x = 0:.01:1;
            m1 = -8; b1 = 2; m2 = 1; b2 = -1;
            y = ( b1 .+ m1 .* x ) .* (cos(x.*pi).*.5+.5)   .+   (b2 .+ m2.*(x.-1)) .* (cos((1.-x).*pi).*.5+.5);
            plot(x,b1.+m1.*x,x,(b2 .+ m2.*(x.-1)),x,y);
        """
        # scale is the size of one unit 
        scale = 1.0 / self.width
        # loop through every column, computing column specific stuff
        for x in range( x1, x2 ):
            xPercent = ( 0.0 + x - x1 ) / ( x2 - x1 )

            # using cosine 
            rightContribution = cos( (1.0-xPercent) * pi ) * .5 + .5

            # using Perlin's s-curve
            #rightContribution = xPercent * xPercent * xPercent * ( xPercent * (
            #xPercent * 6.0 - 15.0 ) + 10.0 )

            leftContribution = 1.0 - rightContribution

            zTop = ( mx1 * xPercent * scale ) * leftContribution + \
            ( mx2 * ( xPercent - 1 ) * scale ) * rightContribution
            zBot = ( mx3 * xPercent * scale ) * leftContribution + \
            ( mx4 * ( xPercent - 1 ) * scale ) * rightContribution
            
            myTop = my1 * leftContribution + my2 * rightContribution
            myBot = my3 * leftContribution + my4 * rightContribution
            
            for y in range( y1, y2 ):
                yPercent = (0.0 + y - y1 ) / ( y2 - y1 )

                # cosine curve
                botContribution = cos( (1.0-yPercent) * pi ) * .5 + .5

                # perlin's curve
                #botContribution = yPercent * yPercent * yPercent * ( yPercent * (
                #yPercent * 6.0 - 15.0 ) + 10.0 )

                topContribution = 1.0 - botContribution

                z = ( zTop + myTop * yPercent * scale ) * topContribution +\
                ( zBot + myBot * (yPercent-1.0) * scale ) * botContribution
                self.data[x, y] += z;
                

    def _interpolateBetweenXAxis( self, position, x1, y1, z1, vx1, vy1, vz1, x2, 
    y2, z2, vx2, vy2, vz2 ):
        """ 
            returns an in-between point
        """
        pass

    def getOctaveDataString( self ):
        """
            returns a string representation of the data
            in the pattern, almost like an image file.
        """                
        dataString = "# Created by fland layer.py.\n"
        dataString += "# name: data\n"
        dataString += "# type: matrix\n"
        dataString += "# rows: %s\n" % self.height
        dataString += "# columns: %s\n" % self.width
        for x in range( self.width ):
            for y in range( self.height ):
                dataString += " %f" % self.data[ x, y ]
            dataString += "\n"
        return dataString


    def setRange( self, min, max ):
        """
            updates the height of every point to be 
            within min and max
            Does not clip, but scales
        """
        oldMin = self.data.min()
        oldMax = self.data.max()
        oldHeight = oldMax - oldMin
        newHeight = max - min
        for x in range( self.width ):
            for y in range( self.height ):
                self.data[ x, y ] = ( self.data[ x, y ] - oldMin ) / oldHeight \
                * newHeight + min


    def getNormal( self, x, y ):
        """ 
            returns the normal to the plane at a point.
        """
        x1 = 1.0*x / self.width;
        y1 = 1.0*y / self.height;
        z1 = self.getVal( x, y ) * self.width;
        x2 = (1.0*x+1.0) / self.width;
        y2 = y1
        z2 = self.getVal( x+1, y ) * self.width;
        x3 = x1;
        y3 = (y*1.0+1.0) / self.height;
        z3 = self.getVal( x, y+1 ) * self.width;

        # turn the points into two vectors
        xv1 = x2 - x1
        yv1 = y2 - y1
        zv1 = z2 - z1
        xv2 = x3 - x1
        yv2 = y3 - y1
        zv2 = z3 - z1

        xr = yv1 * zv2 - zv1 * yv2;
        yr = zv1 * xv2 - xv1 * zv2;
        zr = xv1 * yv2 - yv1 * xv2;

        len = math.sqrt( xr*xr + yr*yr + zr*zr )
        if( len > 0 ):
            xr /= len
            yr /= len
            zr /= len
        else:
            xr = 0
            yr = 0
            zr = 0

        return( [xr, yr, zr] )

    def writeToFile( self, filename ):
        """
            writes the layer to a file of my own format. I dont feel like
            writing parsers for more complicated formats.
        """
        f = open( filename, 'w' )
        f.write( "# Saved pattern data. format is width, height, min, max, and type. Data is interpolated linearly between min and max.\n" )
        f.write( str( self.width ) + ' ' )
        f.write( str( self.height ) + ' ' )
        f.write( str( self.data.min() ) + ' ' )
        f.write( str( self.data.max() ) + ' ' )
        f.write( "BYTE\n" )

        multiplyBy = 1.0 / ( self.data.max() - self.data.min() )
        takeFrom = self.data.min()

        # write the data
        for x in range( self.width ):
            for y in range( self.height ):
                val = ( self.getVal( x, y ) - takeFrom ) * multiplyBy
                val = int( val * 256 )
                if( val > 255 ):
                    val = 255
                if( val < 0 ):
                    val = 0
                f.write( chr( val ) )
        f.close()



