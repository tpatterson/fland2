#ifndef FMLUTILS_H
#define FMLUTILS_H


using namespace std;

// TextFile reads in a file from disk and reads it line by line.
class TextFile
{
	public:
	istringstream *file;
	int line;
	string fileName;

	TextFile( string inFileName = "" );

	~TextFile();

	bool loaded();

	string getLine();

	bool eof();

	// returns a string of the position in the file
	string pos();

	void pError( string error );
};

// An object of this type will be thrown when a fatal error occures.
class FmlParserFatalError
{
	public:
	FmlParserFatalError( string err );

	// pass an error message and a position in the file
	FmlParserFatalError( string err, string pos );

	FmlParserFatalError();

	string what();

	string message;
	int line;
	string file;
};

// These are some utility function I use in the file processing
// reads one line from a file.
// if eof is encountered, then fh.eof() is true
// must be called like this:
//		fileLine = getLine( &handle );
string getLine( ifstream *handle );

// takes a string, breaks off the first occurence of token, and returns the token.
// the origional string is changed
string tokenize( string &inString, string delimiter );

// takes a string, breaks off the LAST occurence of token, and returns the token.
// the origional string is changed
string tokenizer( string &inString, string delimiter );

string addQuotes( string inStr );

// The string cant have leading or trailing whitespace.
string stripQuotes( string instr );

#endif

