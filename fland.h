// The main interface file. Any program can include this file and have access to all the
// fland interface and functionality.
// fland is a class
#ifndef FLAND_H
#define FLAND_H

using namespace std;


// The includes were looking messy so I put them in this file.
#include <string>
#include "defines.h"
#include "env.h"
class FmlParser;
class BaseEnt;
class Camera;

class Fland
{
	public:
	// the seed function is momentarily broken
	Fland( int inSeed );
	~Fland();

	double targetFrameRate;
	int maxCreates;

	// The actual morph ammount being used at any given moment
	double morphPercentage;

	// this must be called before draw();
	int recalculateView();
	// renders the whole landscape. Must call recalculateView first!
	int draw();
	void dump( ostream&, int ); // each entity writes it's attributes to cout.

	// This is the clostest vertex or entity that is being drawn this frame.
	// The near clipping plane should be this value.
	DIS_TYPE closestRenderDistance;
	// The farthest vertex being rendered.
	DIS_TYPE farthestRenderDistance;

	// pointer to the beginning!
	BaseEnt *firstEnt;
	Camera *camera;
	FmlParser *fmlParser;

	// this keeps track of fland's environment stuff
	// the camera uses it a lot.
	Env env;

	inline void setSplitVisibility( DIS_TYPE );
	inline DIS_TYPE getSplitVisibility();
	inline DIS_TYPE getInverseSplitVisibility();

	// This function builds the html files to view the rules graphically.
	// All files are stored in the folder 'html' in the current directory.
	// It first builds a single index.html file with a broad layout of the
	// entities and thier rules.
	// Then it creates a single simple html file for every each rule and
	// entity that will be displayed when the user clicks on one of the
	// links in index.html
	void buildRuleLayout();

	// this dumps a full listing of every object and it's rules to a file.
	// called by pressing 'o' while fland is running.
	void dumpObjectsToFile( string fileName );

	static Fland* fland;



	private:
	DIS_TYPE splitVisibility;
	DIS_TYPE inverseSplitVisibility;

};

inline void Fland::setSplitVisibility( DIS_TYPE inVal )
{
	splitVisibility = inVal;
	inverseSplitVisibility = 1.0 / inVal;
}

inline DIS_TYPE Fland::getSplitVisibility()
{
	return splitVisibility;
}
inline DIS_TYPE Fland::getInverseSplitVisibility()
{
	return inverseSplitVisibility;
}


inline Fland* getFlandPtr(){ return Fland::fland; }
void resetFland(); // defined in main.cpp, used in events.cpp

#endif
