// Program Information ////////////////////////////////////////////////////////
/**
 * @file Table.cpp
 * 
 * @brief Implementation file for Table class
 * 
 * @author Carli Decapito, Sanya Gupta, Eugene Nelson
 *
 * @details Implements all member methods of the Table class
 *
 * @Note Requires Table.h
 */
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include "Table.h"

using namespace std;

// Precompiler directives /////////////////////////////////////////////////////
#ifndef TABLE_CPP
#define TABLE_CPP

string ALL = "*";


/**
 * @brief getCommaCount
 *
 * @details counts how many commas are in the passed in string
 *          
 * @pre assumes there's a string that has commas in 
 *
 * @post returns the number of commas in given string
 *
 * @par Algorithm for loop that parses the the given string to check for commas
 *      
 * @param [in] string str
 *
 * @return int (the number of commas)
 *
 * @note None
 */
int getCommaCount( string str )
{
	int length = str.length();
	int count = 0;
	for( int index = 0; index < length; index++ )
	{
		//look for num of commas
		if( str[ index ] == ',' )
		{
			count ++;
		}
	}
	return count;
}

/**
 * @brief getNextWord
 *
 * @details Used to find the action word (command)
 *          
 * @pre assumes there is more to the string and erases
 *
 * @post action word is found and returned
 *
 * @par Algorithm parses through the string and erases till next space
 *
 * @param [in] string &input
 *      
 * @return string (the next word)
 *
 * @note None
 */
string getNextWord( string &input )
{

	string actionType;
	bool spaceExists = false;
	int inputSize = input.size();

	for( int index = 0; index < inputSize; index++ )
	{
		if( input[ index ] == ' ' )
		{
			spaceExists = true;
		}
	}
	if( !spaceExists )
	{
		actionType = input;
		input.clear();
		return actionType;
	}
	else
	{
		//take first word of input and set as action word
		actionType = input.substr( 0, input.find(" "));
		//erase word from original str to further parse
		input.erase( 0, input.find(" ") + 1 );

		return actionType;
	}
	
}

string getUntilTab( string &input )
{
	string attribute;
	bool tabExists = false;
	int inputSize = input.size();

	for ( int index = 0; index < inputSize; index++ )
	{
		if( input[ index ] == '\t' )
		{
			tabExists = true;
		}
	}
	if ( !tabExists )
	{
		attribute = input;
		input.clear();
		return attribute;
	}
	else
	{
		attribute = input.substr( 0, input.find("\t") );
		input.erase( 0, input.find( "\t" ) + 1 );
		return attribute;	
	}

}

/**
 * @brief removeLeadingWS
 *
 * @details Used to find the action word (command)
 *          
 * @pre assumes there is a string with potential whitespace in front of the word
 *
 * @post leading white space infront of a string is removed
 *
 * @par Algorithm parses through the string and moves index upto white space and deletes it through erase function
 *
 * @param [in] string &input
 *      
 * @return none
 *
 * @note None
 */
void removeLeadingWS( string &input )
{
	int index = 0;
	while( input[ index ] == ' ' || input[ index ] == '\t' )
	{ 
		index++;
	}

	input.erase( 0, index );
}


/**
 * @brief attributeNameExists
 *
 * @details checks vector of tables to see if attribute name already exists
 *          
 * @pre assumes there's an vector of attributes to search through
 *
 * @post action word is found and returned
 *
 * @par Algorithm parses through the string and erases till next space
 *
 * @param [in] vector <Attribute> attributeTable
 * 
 * @param [in] Attribute attr - the attribute stored locally
 *      
 * @return bool true if already in table, else false
 *
 * @note None
 */
bool attributeNameExists( vector< Attribute > attributeTable, Attribute attr )
{
	int size = attributeTable.size();
	for( int index = 0; index < size; index++ )
	{
		if( attributeTable[ index ].attributeName == attr.attributeName )
		{
			return true;
		}
	}
	return false;
}


/**
 * @brief table default constructor
 *
 * @details table class default constructor - needs to be implemented
 *
 * @note None
 */
Table::Table()
{

}


/**
 * @brief Table default destructor
 *
 * @detials Table class default destructor - needs to be implmented
 *
 * @notes Tables should be deleted
*/
Table::~Table()
{

}


/**
 * @brief tableCreate
 *
 * @details creates table and stores in disk otherwise handles errors too
 *          
 * @pre assumes there is more to the string and erases
 *
 * @post action word is found and returned
 *
 * @par Algorithm checks if table already exists in current directory, if not, then creates table in current database & directory
 *
 * @param [in] string currentWorkingDirectory
 *
 * @param [in] string currentDatabase
 *
 * @param [in] string tblName
 *
 * @param [in] string input
 *
 * @param [in] bool &errorCode
 *      
 * @return  none
 *
 * @note None
 */
void Table::tableCreate( string currentWorkingDirectory, string currentDatabase, string tblName, string input, bool &errorCode )
{
	vector< Attribute> tblAttributes;
	Attribute attr;
	string temp;
	int commaCount;

	//get filepath, Database name + table name
	string filePath = "/" + currentDatabase + "/" + tblName;
	//output to file using ofstream operator
	ofstream fout( ( currentWorkingDirectory + filePath ).c_str() );

	//parse input str
		//remove beginning and end ()'s
		//get first open paren
	input.erase( 0, input.find( "(" ) + 1 );
	input.erase( input.find_last_of( ")" ), input.length()-1 );

	commaCount = getCommaCount( input );

	for( int index = 0; index < commaCount; index++ )
	{
		//remove beginning parameter
		temp = input.substr( 0, input.find( "," ));
		input.erase( 0, input.find(",") + 1 );
		//remove leading white space
		removeLeadingWS( temp );
		//parse, get next two words
		attr.attributeName = getNextWord( temp );
		attr.attributeType = getNextWord( temp );

		//check that variable name does not already exist
		if( attributeNameExists( tblAttributes, attr ) )
		{
			errorCode = true;
			cout << "-- !Failed to create table " << tblName << " because there are multiple ";
			cout << attr.attributeName << " variables." << endl;
			system( ( "rm " +  filePath ).c_str() ) ;
			fout.close();
			return;
		}

		//push attribute onto file
		tblAttributes.push_back( attr );

		//output to file
		fout << attr.attributeName << " ";
		fout << attr.attributeType << "\t";
	}
	
	//remove leading WS from input
	removeLeadingWS( input );
	//parse next two words
	attr.attributeName = getNextWord( input );
	//type is remaining string
	attr.attributeType = input;
	if( attributeNameExists( tblAttributes, attr ) )
	{
		errorCode = true;
		cout << "-- !Failed to create table " << tblName << " because there are multiple ";
		cout << attr.attributeName << " variables." << endl;
		system( ( "rm " +  currentDatabase + "/" + tblName ).c_str() ) ;
		fout.close();
		return;
	}

	//push onto vecotr
	tblAttributes.push_back( attr );
	
	//output to file
	fout << attr.attributeName << " ";
	fout << attr.attributeType;
	fout.close();

	cout << "-- Table " << tblName << " created." << endl;
}



/**
 * @brief tableDrop
 *
 * @details Used to delete a table from current database
 *          
 * @pre assumes table exists in current database
 *
 * @post table no longer exists
 *
 * @par Algorithm uses sys library to run linux terminal commands to delete table
 *
 * @param [in] string dbName - the database currently in 
 *      
 * @return None
 *
 * @note None
 */
void Table::tableDrop( string currentWorkingDirectory, string dbName )
{
	system( ( "rm " + currentWorkingDirectory + "/" + dbName + "/" + tableName ).c_str() ) ;
	cout << "-- Table " << tableName << " deleted." << endl;
}


/**
 * @brief tableAlter method 
 *
 * @details used to add attributes to a specified table
 *          
 * @pre assumes table exists and attribute name and type are specified
 *
 * @post attribute(s) are added to the table
 *
 * @par Algorithm checks if table exists in current directory, and if so, adds the parsed attribute name & type
 *
 * @param [in] string currentWorkingDirectory
 *
 * @param [in] string currentDatabase
 *
 * @param [in] string input
 *
 * @param [in] bool &errorCode
 *      
 * @return none
 *
 * @note None
 */
void Table::tableAlter( string currentWorkingDirectory, string currentDatabase, string input, bool &errorCode )
{
	vector < Attribute > tableAttributes;
	vector < string > fileContents;
	string attrLine;
	Attribute attr;
	Attribute tempAttr;
	int commaCount = 0;
	string temp;
	int originalNumOfAttr = 0;
	int newNumOfAttr = 0;
	//create filepath  to read from file
	string filePath = "/" + currentDatabase + "/" + tableName;
	ifstream fin( ( currentWorkingDirectory + filePath ).c_str() );

	string action = getNextWord( input );

	if( action == "ADD" )
	{
		while( !fin.eof() )
		{
			//get attribute line
			getline( fin, attrLine );

			while( !attrLine.empty() )
			{
				string attribute = getUntilTab( attrLine );
				//parse attribute further
				tempAttr.attributeName = getNextWord( attribute );
				tempAttr.attributeType = attribute;

				tableAttributes.push_back( tempAttr );
			}
			//check again that it is not end of file
			if( !fin.eof() )
			{
				getline( fin, temp );
				fileContents.push_back( temp );
			}	
		}

		commaCount = getCommaCount( input );

		//get number of attributes
		originalNumOfAttr = tableAttributes.size();

		ofstream fout( ( currentWorkingDirectory + filePath ).c_str() );
		//get additional attributes
		for( int index = 0; index < commaCount; index++ )
		{
			//remove beginning parameter
			temp = input.substr( 0, input.find( "," ));
			input.erase( 0, input.find(",") + 1 );
			//remove leading white space
			removeLeadingWS( temp );
			//parse, get next two words
			attr.attributeName = getNextWord( temp );
			attr.attributeType = getNextWord( temp );

			//check that variable name does not already exist
			if( attributeNameExists( tableAttributes, attr ) )
			{
				errorCode = true;
				cout << "-- !Failed to modify table " << tableName << " because there are multiple ";
				cout << attr.attributeName << " variables." << endl;
				system( ( "rm " +  filePath ).c_str() ) ;
				return;
			}

			//push attribute onto file
			tableAttributes.push_back( attr );
		}
	
		//remove leading WS from input
		removeLeadingWS( input );
		//parse next two words
		attr.attributeName = getNextWord( input );
		//type is remaining string
		attr.attributeType = input;
		if( attributeNameExists( tableAttributes, attr ) )
		{
			errorCode = true;
			cout << "-- !Failed to modify table " << tableName << " because there are multiple ";
			cout << attr.attributeName << " variables." << endl;
			return;
		}
		//push onto vecotr
		tableAttributes.push_back( attr );

		int tableSize = tableAttributes.size();
		newNumOfAttr = tableSize - originalNumOfAttr;
		for( int index = 0; index < tableSize; index++ )
		{
			if( index != tableSize - 1 )
			{
				fout << tableAttributes[ index ].attributeName << " ";		
				fout << tableAttributes[ index ].attributeType << "\t";	
			}
			else
			{
				fout << tableAttributes[ index ].attributeName << " ";		
				fout << tableAttributes[ index ].attributeType;	
			}

		}

		int contentSize = fileContents.size();
		for( int index = 0; index < contentSize; index++ )
		{
			fout << endl << fileContents[ index ];
			for( int newAttr = 0; newAttr < newNumOfAttr; newAttr++ )
			{
				fout << "\tnull";
			}
		}
		fin.close();
		fout.close();
		cout << "-- Table " << tableName << " modified." << endl;
	}
	else
	{
		return;
	}
}


/**
 * @brief tableSelect method
 *
 * @details  displays the attributes from queried table
 *          
 * @pre assumes table specified is in the current directory
 *
 * @post attributes stored in the directory are displayed 
 *
 * @par Algorithm goes to the filepath, and displays info from the file
 *
 * @param [in] string currentWorkingDirectory
 *
 * @param [in] string currentDatabase
 *   
 * @return None
 *
 * @note None
 */
void Table::tableSelect( string currentWorkingDirectory, string currentDatabase, string input, string queryType )
{
	vector< string > attributes;
	vector < string > fileContent;
	string filePath = "/" + currentDatabase + "/" + tableName;
	string temp;
	ifstream fin( ( currentWorkingDirectory + filePath ).c_str() );

	//check type of query
	if( queryType == ALL )
	{
		//get until end of line
		getline( fin, temp);

		while( !temp.empty() )
		{
			string attribute = getUntilTab( temp );
			attributes.push_back( attribute );
		}
		
		cout << "-- ";
		int size = attributes.size();
		for( int index = 0; index < size; index++ )
		{
			cout << attributes[ index ];
			if( index != size - 1 )
			{
				cout << "|";
			}
		}
		cout << endl;

		while( !fin.eof() )
		{
			getline( fin, temp);
			cout << "-- ";
			while( !temp.empty() )
			{
				string content = getUntilTab( temp );
				cout << content << "|";
			}
			cout << "\b \b";
			cout << endl;
		}
	}
	else
	{
		cout << "not correct query type " << endl;
	}

}

void Table::tableInsert( string currentWorkingDirectory, string currentDatabase, string tblName, string input, bool &errorCode )
{
	string contentStr = "\n";
	int commaCount;
	string filePath = "/" + currentDatabase + "/" + tableName;
	string temp;

	commaCount = getCommaCount( input );
	for( int index = 0; index < commaCount; index++ )
	{
		//remove beginning parameter
		temp = input.substr( 0, input.find( "," ));
		input.erase( 0, input.find(",") + 1 );

		//remove leading white space
		removeLeadingWS( temp );

		//concat temp to content str
		contentStr = contentStr + temp + '\t';

	}
	
	//remove leading WS from input
	removeLeadingWS( input );
	contentStr = contentStr + input;

	ofstream fout;
	fout.open( ( currentWorkingDirectory + filePath ).c_str(), ofstream::out | ofstream::app );
	fout << contentStr;

	fout.close();

	cout << "-- 1 new record inserted." << endl;
}
// Terminating precompiler directives  ////////////////////////////////////////
#endif
