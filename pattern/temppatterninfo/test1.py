
size = 32

l1 = Layer( size, size )
l1.addNoise( 1 )
#l1.addNoise( 2 )
l1.addNoise( 4 )
l1.addNoise( 8 )
l1.addNoise( 16 )
l1.addNoise( 32 )
#l1.addNoise( 64 )
#l1.addNoise( 128 )
#l1.addNoise( 256 )
#l1.addNoise( 512 )


l1.writeToFile( 'temp.data' )

self.returnLayer( l1 )
