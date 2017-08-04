#ifndef FMLPARSER_CPP
#define FMLPARSER_CPP
#include <sstream>
#include <iterator>
#include <map>

using namespace std;

#include "fmlparser.h"
#include "defines.h"
#include "tyler.h"
#include "baserule.h"

#include "groupr.h"
#include "averager.h"
#include "roughr.h"
#include "colorshiftr.h"
#include "drules.h"
#include "monor.h"
#include "main.h"



//using std::ios;
// Note: This code needs a lot of TLC. Its way messy (if I remember from before my mission right)
// it starts here with a filename passed in.
FmlParser::FmlParser( string inStartFile )
{
	cout << "Parsing " << inStartFile << endl;

	startFile = inStartFile;
	parseFile( startFile, FML_FOLDER );

	// check for a rule named "root"
	if( !nameMap[ ROOT_RULE_NAME ] )
		throw( FmlParserFatalError( "FML ERROR! Finished compiling FML files \n\tand there was no rule named " ROOT_RULE_NAME " found!" ) );
}

FmlParser::~FmlParser()
{
	// TODO: delete everything in the nameMap and picMap
	/* // Disable this because it has to do with pictures
	picMapType::iterator iter;
	for( iter = picMap.begin();
	iter != picMap.end();
	++iter )
	{
		//cout << iter->first << iter->second << endl;
		if( iter->second )
			delete iter->second;
	}
	*/
	nameMapType::iterator niter;
	for( niter = nameMap.begin();
	niter != nameMap.end();
	++niter )
	{
		//cout << niter->first << niter->second << endl;
		if( niter->second )
			delete niter->second;
	}
}

// this gets called on main.fml, and every file that is included
int FmlParser::parseFile( string inFile, string folder )
{
	TextFile file( inFile, folder );

	if( !file.loaded() )
	{
		return 1;
	}
	string ret;


	while( !file.eof() )
	{
		ret = evaluateLine( &file, NULL );
		//cout << file.pos() << endl;
	}
	return 0;
}

// this is called on every line of a file, one line after another.
// When called initially currentRule is NULL.
string FmlParser::evaluateLine( TextFile *file, BaseRule* currentRule )
{
		string fileLine = file->getLine();

		// clean out coments, leading and trailing whitespace.
		cleanLine( fileLine );

		string token;
		string rightOperand = "";
		string returnVal = "";

		// for checking for barewords
		int numTokens = 0;

		do
		{
			// strip off the rightmost token. The equals is the delimiter.
			// after this line fileLine will have one less token and equals on the right.
			token = tokenizer( fileLine, "=" );


			// this is genius. The rightmost token is striped off and passed in as 'token'.
			// Any processing is done on it (if it is some command like 'print' or 'include')
			// if there is no processing to do (like if it is an rvalue) it will just be returned.
			// Then the returned value is passed back in to be the rvalue of the next call (returnVal)
			returnVal = evaluateToken( token, returnVal, file, currentRule );

			// make sure there is a returnVal. /closing tags are allowed as barewords.
			if( returnVal != "" && returnVal.substr( 0, 1 ) != "/" ){ numTokens++; }
		}while( fileLine.length() > 0 );

		if( numTokens == 1 ) { file->pError( "Bareword '" + returnVal + "' found" ); }

		return returnVal;
}

// This is disabled at the moment until we fix the bitmap loader
/*
int FmlParser::processImage( string fileName )
{
	// if the image is already loaded and in the array, we're good.
	if( picMap[ fileName ] )
		return 0;

	string loadFile = PIC_FOLDER + fileName;

	// not loaded yet, load it.
	Bitmap* nBitmap;
	// THIS NEXT LINE is what causes the problem
	// it has nothing to do with the Bitmap class, Vertex does the same thing
	nBitmap = new Bitmap;
	nBitmap->loadBitmap( loadFile );

	// if there was an error, bail out now
	if( nBitmap->isGood == false )
	{
		delete nBitmap;
		return 1;
	}

	// there wasn't an error, put it in the picMap
	picMap[ fileName ] = nBitmap;

	return 0;

}
*/

// token is the rule being processed. inputVal is the argument to the rule. For example
// token = inputVal
// ret is the same as token if token isn't a /rule tag and inputVal is null,
// or ret is the value of a new rule.
string FmlParser::evaluateToken( string token, string inputVal, TextFile* file, BaseRule* currentRule )
{
	cleanLine( inputVal );
	cleanLine( token );

	token = stripQuotes( token );
	string ret = token; // by default, return the token, striped of quotes

	// bail out if no rvalue is given.
	if( inputVal == "" && token.substr( 0, 1 ) != "/" )
	{
		return ret;
	}

	// first, check if it is an image. If so, load and put in picMap
	if( token == "image" )
	{
		//if( processImage( inputVal ) )
		//	file->pError( "Couldn't load image " + inputVal );
	}

	if( token == "print" )
	{
		cout << inputVal << "\n" << flush;
	}
	else if( token == "include" )
	{
		parseFile( inputVal, file->folder );
	}
	else if( token == "rule" )
	{
		ret = evaluateRule( inputVal, file );
	}
	else if( token == "/rule" && currentRule == NULL )
	{
		file->pError( "There are more /rule then there are rule tags!" );
	}
	else if( token == "printnames" )
	{
		cout << "Printing the entire name map\n";
		nameMapType::iterator niter;
		for( niter = nameMap.begin();
		niter != nameMap.end();
		++niter )
		{
			cout << "\t" << BASER(niter->second)->getType() << " " << niter->first << " " << niter->second << endl;
		}
	}
	else if( token == "name" )
	{
	// insert a new name into the name map
		if( nameMap[ inputVal ] )
		{
			file->pError( "you are trying to create rule " + addQuotes( inputVal ) + " twice!" );
		}
		nameMap[ inputVal ] = currentRule;
		// return the name of the new rule, with a flag so we know it is for us
		ret = "name=" + inputVal;
	}
	else
	{
		// this is one of the many individual parameters in a rule.
		if( currentRule && inputVal != "" )
		{
			// Clean the inputVal for certain conditions
			if( inputVal == "true" )
				inputVal = "1";
			if( inputVal == "false" )
				inputVal = "0";
			if( inputVal == "infinity" )
				inputVal = "99999999";

			// create the stream, and send the parameters to the virtual function of the rule they
			// go to
			istringstream sVal( inputVal );
			string rError;
			rError = currentRule->assignValue( token, &sVal, this );
			// TODO: make sure assignValue returns a value, at least empty string.
			// if it doesn't, then this next line can seg-fault.
			if( rError != "" )
			{
				if( rError == "ERROR" ) // These are fatal errors
					{ throw FmlParserFatalError( rError, file->pos() ); }
				else
					file->pError( rError );
			}
		}
		else if( !currentRule && inputVal != "" ) // This command of text is not recognised.
		{
			file->pError( "Command \"" + inputVal + "\" not recognised" );
		}
	}

	return ret;
}

// inputVal is the name of the rule type to create (like averager or groupr). If left
// blank a unique rule name will be given.
// this function will create a new rule of the type specified, put it's pointer into the
// nameMap hash and return the string name that references it.
// this function is called when the parser encounters a rule tag. Then this function
// evaluates lines inside the file until reaching it's /rule tag.
string FmlParser::evaluateRule( string inputVal, TextFile *file )
{
	// the filename and position in the file.
	// name is the name of the rule that will be put in the namemap.
	// Because some rules are not given names, the filename and position is given as a unique name.
	// If the rule IS given a specific name, this name parameter will be overridden.
	string name = file->pos();

	string type = "";
	BaseRule* rptr = NULL;
	// first create the rule
	if( inputVal == "groupr" )
	{
		rptr = new GroupR;
	}
	else if( inputVal == "colorshiftr" )
	{
		rptr = new ColorShiftR;
	}
	else if( inputVal == "roughr" )
	{
		rptr = new RoughR;
	}
	else if( inputVal == "averager" )
	{
		rptr = new AverageR;
	}
	else if( inputVal == "patternr" )
	{
		rptr = new PatternR;
	}
	else if( inputVal == "altituder" )
	{
		rptr = new AltitudeR;
	}
	else if( inputVal == "sloper" )
	{
		rptr = new SlopeR;
	}
    else if( inputVal == "monor" )
    {
        rptr = new MonoR;
    }
	else if( inputVal == "altitudexslope" || inputVal == "slopexaltitude" )
	{
		rptr = new SlopeXAltitudeR;
	}
	else
	{
		throw FmlParserFatalError( "Undefined rule type " + inputVal + " Bailing out! ", file->pos() );
	}
	string ret = "";

	bool didName = false;
	// parse one line at a time through the FML file and evaluate the rule parameters.
	do
	{

		if( file->eof() ){ break; }

		ret = evaluateLine( file, rptr );

		// find the fml name and store it.
		if( ret.find( "name=" ) != string::npos )
		{
			name = tokenizer( ret, "name=" );
			didName = true;
		}
	}while( ret != "/rule" );

	if( !didName )
	{
		ret = evaluateToken( "name", name, file, rptr );
		if( ret == "ERROR" )
			{ LN return ret; }
	}

	#if( DEBUG )
		rptr->fmlName = name;
		rptr->fmlRule = true;
	#endif
	return name;
}

// cleans out whitespace and coments
// Any internal whitespace will remain.
void FmlParser::cleanLine( string &newLine )
{
	// first clean out the leading whitespace
	int i;
	for( i=0; i<newLine.length(); i++ )
	{
		if( newLine.substr( i, 1 ) == " " ||
			 newLine.substr( i, 1 ) == "\r" ||
			 newLine.substr( i, 1 ) == "\n" ||
			 newLine.substr( i, 1 ) == "\t" ||
			 newLine.substr( i, 1 ) == "\0" )
			continue;
		break;
	}
	newLine = newLine.substr( i, newLine.length() - i );


	// a trick to strip off the coments.
	newLine = tokenize( newLine, COMENT );

	// now clean out the trailing whitespace
	for( i=newLine.length()-1; i>=0; i-- )
	{
		if( newLine.substr( i, 1 ) == " " ||
			 newLine.substr( i, 1 ) == "\n" ||
			 newLine.substr( i, 1 ) == "\r" ||
			 newLine.substr( i, 1 ) == "\t" ||
			 newLine.substr( i, 1 ) == "\0" )
			continue;
		break;
	}
	newLine = newLine.substr( 0, i+1 );


}

// ParseRule stuff

#endif
