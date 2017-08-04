#ifndef DIAGNOSE_CPP
#define DIAGNOSE_CPP
// this file contains fland diagnostic functions

using namespace std;
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
// this is needed to see if a rule is a groupr
#include <typeinfo>
#include "fland.h"
#include "baseent.h"
#include "baserule.h"
#include "buildhtml.h"
#include "groupr.h"
#include "defines.h"

// constructor
BuildDiagnosticHtml::BuildDiagnosticHtml( Fland* fland )
{
	if( fland == NULL )
	{
		cout << "Error. No fland pointer given in BuildDiagnosticHtml constructor"; LN
	}
	else
	{
		buildHtml( fland );
	}
}

// loop through each entity.
void BuildDiagnosticHtml::recurseEntities( BaseEnt* entity, ostream& out )
{
	// put this entity and the rules that will follow into a table
	// the table will have two or two columns, one for this rule and one for it's children.

	// this will indent the html nicely
	string ind;
	for( int i=0; i<entity->iterationDepth; i++ ){ ind = ind + " "; }

	// create the individual file
	string individualFileName;
	individualFileName = buildIndividualFile( entity );

	// this table encloses the entity
	out << "<!-- BEGIN ENTITY -->\n";
	out << ind << "<table border=0 cellpadding=0 cellspacing=0><tr><td valign=top>";

		// this puts a table and border around the entity and it's rules.
		out << "<table border=1 cellpadding=0 cellspacing=0><tr><td valign=top>";

			// This table puts the entity in it's own table row, with it's rules in another.
			out << "<!-- THIS TABLE SEPERATES THE ENTITY AND IT'S RULES -->";
			out << "<table border=0 cellpadding=0 cellspacing=0><tr><td valign=top>";
				// this prints out the hyperlink to the individual file
				out << "<a href='" << individualFileName << "' target='_blank'>";
				out << entity->iterationDepth ;
				out << "</a>&nbsp;";
			out << "</td>";
				if( entity->rules.size() > 0 )
				{
			out << "<td valign=top>";
			out << "\n<!-- BEGIN RULES -->";
					for( int i=0; i<entity->rules.size(); i++ )
					{
						out << entity->rules[i].getFmlName() << "-" << entity->rules[i].getIntensity() << "<br>\n";
						//recurseRules( entity->rule.groupMember[i], out );
					}
			out << "<!-- END RULES -->";
			out << "</td>";
				}
			out << "</tr></table>\n";
			out << "<!-- END SEPERATE ENTITY AND IT'S RULES -->";
		out << "</tr></table>\n";

	out << "</td>";

		// call this entities children
		if( entity->child.size() > 0 )
		{
			out << "<td valign='top'>\n";
			for( int i=0; i<entity->child.size(); i++ )
			{
				recurseEntities( entity->child[i], out );
			}
			out << "</td>\n";
		}
	out << ind << "</tr></table>\n";
}

//// loop through each rule that belongs to an entity
//void BuildDiagnosticHtml::recurseRules( BaseRule* rule, ostream& out )
//{
//	string fileName;
//	fileName = buildIndividualFile( rule );
//
//	bool hasGroupMembers = false;
//	if( typeid( GroupR ).name() == typeid( *rule ).name() ){ hasGroupMembers = true; }
//
//	if( hasGroupMembers ){ out << "<table border=0 cellpadding=0 cellspacing=0><tr><td valign=top>"; }
//
//	out << "<a href='" << fileName << "' target='_blank'>";
//	// Print the visible index string
//	out << rule->fmlName << "(" << rule->intensity << ")";
//	string test;
//	out << "</a>&nbsp;\n";
//	if( !hasGroupMembers ){ out << "<br>"; } // if this text doesn't go in a table, print a break
//
//	// check to see if the rule is a groupRule. If so, call this function on it's groupmembers
//	if( hasGroupMembers )
//	{
//		out << "</td>";
//		GroupR *thisRule = GROUPR( rule );
//		out << "<td>";
//		for( int i=0; i<thisRule->size(); i++ )
//		{
//			out << "<!-- BEGIN GROUPMEMBER -->\n";
//			recurseRules( thisRule->groupMember[i], out );
//			out << "\n<!-- END GROUPMEMBER -->";
//		}
//		out << "</td>";
//		out << "</tr></table>\n";
//	}
//}


// a pointer to fland, and a string filename. This builds the main index file.
void BuildDiagnosticHtml::buildHtml( Fland* fland )
{
	cout << "outputting index.html..." << endl;
	ofstream ifile( "html/index.html" );

	ifile << setprecision( 2 );

	ifile << "\n\n<html>\n<head>"
	<< "<script language='javascript' src='html/functions.js'></script>"
	<< "<title>Fland Diagnostic Output</title></head>\n<body>\n";
	ifile << "<b>Fland Diagnostic Output</b><br>\n";
    ifile << "iteration ruleName-intensity<br>\n";
	maxRow = 0;
	int numEntities = fland->firstEnt->getCount( maxRow );
	ifile << numEntities << " Entities<br>\n" << maxRow << " Iterations Deep<br>\n";

	recurseEntities( fland->firstEnt, ifile );

	ifile.close();
	cout << "done" << endl;
}




#endif

