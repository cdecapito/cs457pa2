// Program Information ////////////////////////////////////////////////////////
/**
 * @file Table.h
 *
 * @brief Definition file for Table class
 * 
 * @details Specifies all member methods of the Table class
 *
 * @Note None
 */

#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Precompiler directives /////////////////////////////////////////////////////
#ifndef TABLE_H
#define TABLE_H

struct Attribute{
	string attributeName;
	string attributeType;
};


class Table{
	public: 
		string tableName;

		Table();
		~Table();
		void tableCreate( string currentWorkingDirectory, string currentDatabase, string tblName, string input, bool &errorCode );
		void tableDrop( string currentWorkingDirectory, string dbName );
		void tableAlter( string currentWorkingDirectory, string currentDatabase, string input, bool &errorCode );
		void tableSelect( string currentWorkingDirectory, string currentDatabase, string input, string queryType );
		void tableInsert( string currentWorkingDirectory, string currentDatabase, string tblName, string input, bool &errorCode );
};

// Terminating precompiler directives  ////////////////////////////////////////
#endif
