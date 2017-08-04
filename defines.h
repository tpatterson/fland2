// compile time defines.
#ifndef DEFINES_H
#define DEFINES_H

using namespace std;


// INITALIZATION STUFF //
#define DEFULT_WINDOW_WIDTH 1100 / 2
#define DEFULT_WINDOW_HEIGHT 900 / 2


// spliting stuff
#define DEFAULT_SPLIT_VISIBILITY 0.0803755
#define DEFAULT_TARGET_FRAMERATE 50
// as the framerate drops, detail is pulled to bring it up.
// the detail isn't pulled all at once because then
// things can start to osilate in and out of existance.
// The higher this number, the less it osilates.
#define FRAME_CHANGE_DAMPENER 40.0

// The higher this number, ( up to 1.0 ) the longer it takes for an entity
// to morph into place. 0.4 means that for 40% if it's existance it will be
// morphing into place. The other 60% it will be in place.
#define MORPH_AMOUNT 0.8

// just to be sure
#ifndef NULL
#define NULL 0
#endif

// Whether or not this is the debug version.
// do not define DEBUG if this is not a debug version. Just comment out this line.
#define DEBUG 1

// The quardinate type of the entities( x, y, z ).
#define QUARD_TYPE double

// the type for rule intensity
#define intensityType float

// Other distance variables, such as approxamateSize, etc.
#define DIS_TYPE double

// the type of normals in this program
#define NORMAL_TYPE double

// some debug stuff
// output the file and Line Number
#define LN cout << " " << __FILE__ << " " << __LINE__ << endl;
#define WAIT( x ) double a=getElapsedTime(); while( getElapsedTime() < a+x ){}

// Just so I dont have to type the dynamic cast all the time
#define VERT( x ) dynamic_cast< Vertex* >( x )
#define MSEED( x ) dynamic_cast< MeshSeed* >( x )
#define BASE( x ) dynamic_cast< BaseEnt* >( x )
#define ROUGHR( x ) dynamic_cast< RoughR* >( x )
#define CSHIFTR( x ) dynamic_cast< ColorShiftR* >( x )
#define GROUPR( x ) dynamic_cast< GroupR* >( x )
#define PATCHR( x ) dynamic_cast< PatchR* >( x )
#define BASER( x ) dynamic_cast< BaseRule* >( x )
#define SLOPER( x ) dynamic_cast< SlopeR* >( x )

// Some camera stuff
// When you move the mouse, this is the number of degrees that the view changes
// for every one pixel of mouse movement.
#define CAMERA_LOOK_SPEED 0.3
#define CAMERA_MOVE_SPEED 0.9
#define CAMERA_SPEED_BREAK 10.0

#define FML_FOLDER "fml/"
#define PIC_FOLDER "pics/"
#define ROOT_RULE_NAME "root"

#define LOG_FILE_NAME "log.txt"
#define DUMP_FILE_NAME "dump.txt"

#define BUILD "fland compiled " __DATE__ " at " __TIME__

#define TEST_MESSAGE "this would be a test. -4-\n"


#endif

