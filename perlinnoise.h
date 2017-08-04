#ifndef PERLINNOISE_H
#define PERLINNOISE_H

#include <math.h>
#include <iostream>

using namespace std;

//*
// IMPLEMENTATION OF IMPROVED NOISE - COPYRIGHT 2002 KEN PERLIN.
class PerlinNoise
{
    public:

    static int p[512];
//    static int permutation[256];

    static void initalize();

   inline double fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); }
   inline double lerp(double t, double a, double b) { return a + t * (b - a); }
   inline double grad(int hash, double x, double y, double z) {
      int h = hash & 15;                      // CONVERT LO 4 BITS OF HASH CODE
      double u = h<8 ? x : y;                 // INTO 12 GRADIENT DIRECTIONS.
      double v = h<4 ? y : h==12||h==14 ? x : z;
      return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
   }
   double noise(double x, double y, double z) {
      int X = (int)floor(x) & 255;                  // FIND UNIT CUBE THAT
      int Y = (int)floor(y) & 255;                  // CONTAINS POINT.
      int  Z = (int)floor(z) & 255;
      x -= floor(x);                                // FIND RELATIVE X,Y,Z
      y -= floor(y);                                // OF POINT IN CUBE.
      z -= floor(z);
      double u = fade(x);                                // COMPUTE FADE CURVES
      double v = fade(y);                                // FOR EACH OF X,Y,Z.
      double w = fade(z);
      int A = p[X  ]+Y, AA = p[A]+Z, AB = p[A+1]+Z;      // HASH COORDINATES OF
      int B = p[X+1]+Y, BA = p[B]+Z, BB = p[B+1]+Z;      // THE 8 CUBE CORNERS,

      return lerp(w, lerp(v, lerp(u, grad(p[AA  ], x  , y  , z   ),  // AND ADD
                                     grad(p[BA  ], x-1, y  , z   )), // BLENDED
                             lerp(u, grad(p[AB  ], x  , y-1, z   ),  // RESULTS
                                     grad(p[BB  ], x-1, y-1, z   ))),// FROM  8
                     lerp(v, lerp(u, grad(p[AA+1], x  , y  , z-1 ),  // CORNERS
                                     grad(p[BA+1], x-1, y  , z-1 )), // OF CUBE
                             lerp(u, grad(p[AB+1], x  , y-1, z-1 ),
                                     grad(p[BB+1], x-1, y-1, z-1 ))));
   }

    // Runs the noise function recursively
    // PARAMS:
    //   - initialSize: The frequency of the initial pass. 
    //   - iterations: the number of times to do this. Each iteration
    //     the frequency doubles
    //   - x, y, z: The quardinates of this point
    //   - amplitudeFalloff: Each iteration the amplitude normally is about
    //     half of what the last iteration was. Half is the default.
    // RETURNS:
    //   - the noise value of this point. Between -1 and 1.
    double multipleNoise( double initialFrequency, int iterations, double x, double y, double z, double amplitudeFalloff = 0.5 )
    {
        double retVal = 0.0;
        double amplitude = 1.0;
        double freq = initialFrequency;

        for( int i=0; i<iterations; ++i )
        {
            double n = noise( x * freq, y * freq, z * freq ) * amplitude;
            retVal += n;

            amplitude *= amplitudeFalloff;
            freq *= 2.0;
        }
        return( retVal );
    }
};
//*/



#endif 

