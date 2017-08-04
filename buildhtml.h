#ifndef DIAGNOSE_H
#define DIAGNOSE_H
// this file contains fland diagnostic functions

using namespace std;

#include <iostream>
#include <sstream>
#include <string>
#include "fland.h"
#include "baseent.h"
#include "baserule.h"

class BuildDiagnosticHtml
{
	public:
	BuildDiagnosticHtml( Fland* );
	void buildHtml( Fland* );

	// this builds the individual files for this object.
	template< class T >
	string buildIndividualFile( T* );

	void recurseEntities( BaseEnt*, ostream& );

	//void recurseRules( BaseRule*, ostream& );

	protected:
	// these are to keep track of which row and column we are on when recursing through the entities
	// In order to make the table have the same number of rows and columns
	// it will be necessary to put dummy cells in.


	// this keeps track of the highest number of rows that will be displayed.
	int maxRow;
};

// this is the file that, when you click on a rule, it displays that rule's properties.
template< class T >
string BuildDiagnosticHtml::buildIndividualFile( T* obj )
{
	ostringstream fileName;
	string retString;
	fileName << "obj_" << obj << ".html";
	retString = fileName.str();
	ostringstream fullFile;
	fullFile << "html/" << retString;

	ofstream ofile( fullFile.str().c_str() );

	ofile << "<html><body>\n";
	ofile << "<pre>\n";
	ofile << obj->toString( 0 );
	ofile << "</pre>\n";
	ofile << "</body></html>\n";

	ofile.close();

	return retString;
}

#endif

