#ifndef ENV_H
#define ENV_H
// Environment: This class is for keeping track of the
// environment that the program is running in.
// Things like framerate, current keys states, mousestate, etc.

using namespace std;


class Env
{
	public:
	int mouseX, mouseY, mouseButton;
	bool keys[256];

	// Store the current framerate
	double frameRate;
	// the time elapsed in the last frame
	double timeSlice;
	// Keep track of the last frame
	double lastFrameTime;
	//the number of frames elapsed since we started
	long frameCount;

	// Store the screen width and height.
	int screenWidth;
	int screenHeight;

	Env();
	// couts the variables to the console
	void printVars();

	void recalculateFramerate();

};
#endif
