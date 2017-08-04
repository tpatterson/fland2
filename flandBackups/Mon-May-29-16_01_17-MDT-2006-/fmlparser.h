#ifndef FMLPARSER_H
#define FMLPARSER_H

#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include "fmlutils.h"

using namespace std;



//using std::ios;

#define COMENT "//"

class BaseRule;

class FmlParser
{
	public:
	// this stores the names of all the rules that will be used more then once
	//typedef map< string, Bitmap* > picMapType;
	typedef map< string, BaseRule* > nameMapType;
	nameMapType nameMap;
	//picMapType picMap;

	FmlParser( string inStartFile = "" );

	~FmlParser();
	string startFile;

	// parses a file
	int parseFile( string fileName );

	// cleans out whitespace and coments
	void cleanLine( string& );

	// process an image, return error
	int processImage( string fileName );

	// evaluates a token, and returns a string.
	// if it is the token on the left of an assignment,
	// the second string is whatever the thing on the right evaluated to.
	string evaluateToken( string token, string inputVal, TextFile *file, BaseRule* currentRule );

	// evaluates a single line from a file
	// returns whatever the last token evaluated to
	string evaluateLine( TextFile *file, BaseRule* );

	// this function takes over from parseFile when
	// it gets to a rule tag
	// and returns the name of the rule.
	string evaluateRule( string inputVal, TextFile *file );

};



#endif
