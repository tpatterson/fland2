#ifndef FMLUTILS_CPP
#define FMLUTILS_CPP



#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <typeinfo>
#include "tyler.h"
#include "fmlutils.h"



using namespace std;


TextFile::TextFile( string inFileName, string inFolder )
{
	file = NULL;

    string newFolder = tokenize2( inFileName, "/" );
    folder = inFolder;
    if( newFolder != "" )
        folder = inFolder + newFolder + "/";

	line = 0;
	fileName = folder + inFileName + '\0';

	FILE *fp;
	fp = fopen(fileName.data(), "r");
	if( !fp )
	{
		cout << "Couldn't open " << fileName << endl;
	}
	else
	{
		string str;
		char chr;
		do
		{
			fread(&chr, sizeof(char), 1, fp);
			str = str + chr;
		}while( !feof( fp ) );
		fclose( fp );
		file = new istringstream( str );
	}
}

TextFile::~TextFile()
{
	if( file )
		delete file;
}


bool TextFile::loaded()
{
	if( file )
		return true;
	else
		return false;
}


string TextFile::getLine()
{
	string str = "";

	// if this char isn't initalized to something then get() returns garbage at the end of a file.
	// TODO: replace get with something else if this continues.
	char chr = ' ';

	do
	{
		file->get( chr );
		if( chr != '\n' )
			str = str + chr;
	}while( !file->eof() && chr != '\n' );
	if( chr == '\n' )
		line++;
	return str;
}

bool TextFile::eof()
{
	return file->eof();
}


// returns a string of the position in the file
string TextFile::pos()
{
	ostringstream temp;
	string sTemp = fileName;
	string fname = tokenize( sTemp, "." );
	temp << fname << (int)line;
	return temp.str();
}

void TextFile::pError( string error )
{
	if( error.length() > 0 )
		cout << "Parse Error " << fileName << " " << line << "\n\t" << error << endl;
}

FmlParserFatalError::FmlParserFatalError( string err )
{
	message = err;
}

// pass an error message and a position in the file
FmlParserFatalError::FmlParserFatalError( string err, string pos )
{
	message = err; file = pos;
}

FmlParserFatalError::FmlParserFatalError()
: message( "FML Parse Fatal Error Occured." )
{
	line = 0; file = "";
}

string FmlParserFatalError::what()
{
	return message + "\n\t" + file;
}

string getLine( ifstream *handle )
{
	string str = "";
	char chr;
	do
	{
		handle->get( chr );
		if( chr != '\n' )
		str = str + chr;
	}while( !handle->eof() && chr != '\n' );
	return str;
}

// takes a string, breaks off the first occurence of token, and returns the token.
// the origional string is changed
string tokenize( string &inString, string delimiter )
{

	int x = inString.find( delimiter );

	string ret = inString;

	if( x == string::npos )
	{
		inString = "";
	}
	else
	{
		ret = inString.substr( 0, x );
		int chop = x + delimiter.length();
		inString = inString.substr( chop, inString.length() - chop );
	}
	return ret;
}

// takes a string, breaks off the LAST occurence of token, and returns the token.
// the origional string is changed
string tokenizer( string &inString, string delimiter )
{

	int x = inString.rfind( delimiter );

	string ret = inString;

	if( x == string::npos )
	{
		inString = "";
	}
	else
	{
		x = x + delimiter.length();
		ret = inString.substr( x, inString.length() - x );
		int chop = ret.length() + delimiter.length();
		inString = inString.substr( 0, inString.length() - chop );
	}
	return ret;
}

string tokenize2( string &inString, string delimiter )
{

	int x = inString.find( delimiter );

	string ret;

	if( x == string::npos )
	{
		//inString = "";
	}
	else
	{
		ret = inString.substr( 0, x );
		int chop = x + delimiter.length();
		inString = inString.substr( chop, inString.length() - chop );
	}
	return ret;
}

string addQuotes( string inStr )
{
	return( "\"" + inStr + "\"" );
}

// The string cant have leading or trailing whitespace.
string stripQuotes( string instr )
{
	if( instr.substr( 0,1 ) == "\"" )
	{
		return( instr.substr( 1, instr.length() - 2 ) );
	}
	else
	{
		return instr;
	}
}


#endif

