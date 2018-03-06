// Program Information /////////////////////////////////////////////////////////
/**
  * @file sim.cpp
  *
  * @brief implements simulation functions    
  * 
  * @details Performs command line instructions to create and drop db/tbls
  *
  * @version 1.01 Carli DeCapito, Sanya Gupta, Eugene Nelson
  *			 February 10, 2018 - DB create/drop impementation, create directories
  *
  *			 1.00 Carli DeCapito
  *			 February 8, 2018 -- Initial Setup, Create/Drop DB Implementation
  *
  * @note None
  */

#include <iostream>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include "Database.cpp"

using namespace std;

const string DATABASE_TYPE = "DATABASE";
const string TABLE_TYPE = "TABLE";

const string DROP = "DROP";
const string CREATE = "CREATE";
const string SELECT = "SELECT";
const string USE = "USE";
const string ALTER = "ALTER";
const string EXIT = ".EXIT";

const int ERROR_DB_EXISTS = -1;
const int ERROR_DB_NOT_EXISTS = -2;
const int ERROR_TBL_EXISTS = -3;
const int ERROR_TBL_NOT_EXISTS = -4;
const int ERROR_INCORRECT_COMMAND = -5;

void startSimulation( string currentWorkingDirectory );
bool removeSemiColon( string &input );
bool startEvent( string input, vector< Database> &dbms, string currentWorkingDirectory, string &currentDatabase );
string getNextWord( string &input );
bool databaseExists( vector<Database> dbms, Database dbInput, int &dbReturn );
void removeDatabase( vector< Database > &dbms, int index );
void removeTable( vector< Database > &dbms, int dbReturn, int tblReturn );
void handleError( int errorType, string commandError, string errorContainerName );
void convertToLC( string &input );


/**
 * @brief read_Directory method
 *
 * @details reads contents of a directory into a vector
 *
 * @param [in] string &name
 *             
 * @param [in] vector <string &v>
 *
 * @return bool
 *
 * @note Code from stack overflow
 */
bool read_directory(const std::string& name, vector< string >& v)
{
	struct stat buffer;
	if( !( stat( name.c_str(), &buffer ) == 0 ) )
		return false;

    DIR* dirp = opendir(name.c_str());
    struct dirent * dp;
    while ((dp = readdir(dirp)) != NULL) {
        v.push_back(dp->d_name);
    }
    closedir(dirp);

    return true;
}

/**
 * @brief startSimulation 
 *
 * @details takes string input and begins to parse
 *          
 * @pre None
 *
 * @post Program ends when .EXIT is inputted
 *
 * @par Algorithm 
 *      Loop until .EXIT is inputted from terminal
 *		Otherwise parse string to find out what action to take
 *      
 * @exception None
 *
 * @param [in] None
 *
 * @param [out] None
 *
 * @return None
 *
 * @note None
 */
void startSimulation( string currentWorkingDirectory )
{
	currentWorkingDirectory += "/DatabaseSystem";

	// Check if the database system directory exists
	struct stat buffer;
	if( !( stat( currentWorkingDirectory.c_str(), &buffer ) == 0 ) )
	{
		// if not, create it.
		system( ( "mkdir " + currentWorkingDirectory ).c_str() );
	}

	string input;
	string currentDatabase;
	vector< Database > dbms;

	// Retrieve all of the information about existing directories
	vector< string > directoryItems;
	if( read_directory( currentWorkingDirectory, directoryItems ) )
	{
		for( unsigned int i = 0; i < directoryItems.size(); i++ )
		{
			if(directoryItems[i] == "." || directoryItems[i] == "..")
			{
				directoryItems.erase(directoryItems.begin() + i);
				i--;
			}
			else
			{
				Database tempDatabase;
				tempDatabase.databaseName = directoryItems[i];

				vector< string > tableItems;
				Table tempTable;

				if( read_directory( currentWorkingDirectory + "/" + tempDatabase.databaseName, tableItems ) )
				{
					for( unsigned int j = 0; j < tableItems.size(); j++ )
					{
						if(tableItems[j] == "." || tableItems[j] == "..")
						{
							tableItems.erase(tableItems.begin() + j);
							j--;
						}
						else
						{
							tempTable.tableName = tableItems[i];

							tempDatabase.databaseTable.push_back(tempTable);
						}
					}
				}

				dbms.push_back(tempDatabase);
			}
		}
	}

	bool simulationEnd = false;

	do{
		cout << "> ";
		getline( cin, input );
		//helper function to remove semi colon
		if( removeSemiColon( input ) )
		{
			//call helper function to check if modifying db or tbl
			simulationEnd = startEvent( input, dbms, currentWorkingDirectory, currentDatabase );
		}

	}while( simulationEnd == false );

	cout << "All done. " << endl; 
}


/**
 * @brief removeSemiColon
 *
 * @details removes semi colons from input string
 *          
 * @pre string exists
 *
 * @post string ; is removed
 *
 * @par Algorithm 
 *      using find and erase string functions
 *      
 * @exception None
 *
 * @param [in] none
 *
 * @param [out] input provides a string of the input command
 *
 * @return None
 *
 * @note None
 */
bool removeSemiColon( string &input )
{
	//CHECK IF SEMI EXISTS
	int semiIndex = 0;
	int strLen = input.length();
	bool semiExists = false;
	//find where ; exists if it does exist
	for ( int index = 0; index < strLen; index++ )
	{
		if( input[ index ] == ';' )
		{
			semiExists = true;
			semiIndex = index;
		}
	}
	//if semi colon does not exist or is not at the end
	if( semiExists == false || semiIndex != ( strLen - 1 ) )
	{
		//if input is exit then we are fine
		if( input == EXIT )
		{
			return true;
		}
		else
		{
			//otherwise incorrect syntax
			handleError( ERROR_INCORRECT_COMMAND, "NULL", input );
			return false;
		}

	}
	else
	{
		//if semi colon is at end then erase it
		input.erase( input.find( ';' ) );
		return true;
	}
	
}




/**
 * @brief startEvent
 *
 * @details Initiates action to take: create, use, drop, select, alter
 *          
 * @pre input and dbms exists
 *
 * @post action is done
 *
 * @par Algorithm 
 *      stores data into DB class and into tables
 *      
 * @exception None
 *
 * @param [in] input provides string of input command
 *
 * @param [out] dbms provides system of database to add databases and tables
 *
 * @return None
 *
 * @note None
 */
bool startEvent( string input, vector< Database> &dbms, string currentWorkingDirectory, string &currentDatabase )
{
	bool exitProgram = false;
	bool errorExists = false;
	bool attrError = false;

	//bool tblExists;
	int dbReturn;
	int tblReturn;
	int errorType;
	string originalInput = input;
	string errorContainerName;
	string actionType = getNextWord( input );
	string containerType;

	if( actionType.compare( SELECT )  == 0 )
	{
		Database dbTemp;
		dbTemp.databaseName = currentDatabase;
		databaseExists( dbms, dbTemp, dbReturn );

		//get 
		string temp = getNextWord( input );
		temp = getNextWord( input );


		Table tblTemp;
		tblTemp.tableName = input;

		if( !(dbms[ dbReturn ].tableExists( tblTemp.tableName, tblReturn )) )
		{
			errorExists = true;
			errorType = ERROR_TBL_NOT_EXISTS;
			errorContainerName = tblTemp.tableName;		
		}
		else
		{
			tblTemp.tableSelect( currentWorkingDirectory, currentDatabase );
		}
	}
	else if( actionType.compare( USE ) == 0 )
	{
		
		Database dbTemp;
		dbTemp.databaseName = input;
		bool dbExists = databaseExists( dbms, dbTemp, dbReturn );
		
		//check if database exists
		if( dbExists )
		{
			//if it does then set current database as string
			currentDatabase = dbTemp.databaseName;
			dbTemp.databaseUse();
		}
		else
		{
			//if it does not then return error message
			errorExists = true;
			errorContainerName = dbTemp.databaseName;
			errorType = ERROR_DB_NOT_EXISTS; 
		}
	}
	else if( actionType.compare( CREATE ) == 0 ) 
	{
		//get string if we are modifying table or db
		containerType = getNextWord( input );
		//databse create
		if( containerType == DATABASE_TYPE )
		{
			Database dbTemp;
			//call Create db function
			dbTemp.databaseName = input;
			//check that db does not exist already
			bool dbExists = databaseExists( dbms, dbTemp, dbReturn );

			if( dbExists )
			{
				//if it does then return error message
				errorExists = true;
				errorContainerName = dbTemp.databaseName;
				errorType = ERROR_DB_EXISTS; 
			}
			else
			{
				//if it does not, return success message and push onto vector
				dbms.push_back( dbTemp );

				//create directory
				dbTemp.databaseCreate();
			}
		}
		//table create
		else if( containerType == TABLE_TYPE )
		{
			
			//call create tbl function
			Database dbTemp;
			dbTemp.databaseName = currentDatabase;
			//get dbReturn of database
			databaseExists( dbms, dbTemp, dbReturn );
			
			// make sure the input does not specify multiple tables 
			size_t pos = input.find(" (");
			string temp = input.substr(0, pos);
			if( temp.find(" ") != string::npos )
			{
				errorExists = true;
				errorType = ERROR_INCORRECT_COMMAND;
				errorContainerName = originalInput;
			}
			else
			{
				//get table name 
				Table tblTemp;
				tblTemp.tableName = getNextWord( input );

				//check that table exists
				if( !(dbms[ dbReturn ].tableExists( tblTemp.tableName, tblReturn )) )
				{
					//check that table attributes are not the same
					tblTemp.tableCreate( currentWorkingDirectory, currentDatabase, tblTemp.tableName, input, attrError );
					if( !attrError  )
					{
						//if it doesnt then push table onto database	
						dbms[ dbReturn ].databaseTable.push_back( tblTemp );
					}
				}
				else
				{
					//if it does than handle error
				 	errorExists = true;
				 	errorType = ERROR_TBL_EXISTS;
				 	errorContainerName = tblTemp.tableName;	
				}
			}
		}
		else
		{
			errorExists = true;
			errorType = ERROR_INCORRECT_COMMAND;
			errorContainerName = originalInput;	
		}

	}
	else if( actionType.compare( DROP ) == 0 )
	{
		containerType = getNextWord( input );
		if( containerType == DATABASE_TYPE )
		{
			//create temp db to be dropped
			Database dbTemp;
			dbTemp.databaseName = input;

			//check if database exists
			if( databaseExists( dbms, dbTemp, dbReturn ) != true )
			{
				//if it does not then return error message
				errorExists = true;
				errorContainerName = dbTemp.databaseName;
				errorType = ERROR_DB_NOT_EXISTS; 
			}
			else
			{
				//if it does, return success message and remove from dbReturn element
				removeDatabase( dbms, dbReturn );

				//remove directory
				dbTemp.databaseDrop(currentWorkingDirectory);
			}


		}
		else if( containerType == TABLE_TYPE )
		{
			//call drop tbl function
			Database dbTemp;
			dbTemp.databaseName = currentDatabase;
			databaseExists( dbms, dbTemp, dbReturn );

			Table tblTemp;
			tblTemp.tableName = getNextWord( input );

			//check if table exists
			if( !(dbms[ dbReturn ].tableExists( tblTemp.tableName, tblReturn )) )
			{
				//if it doesnt exist then return error
				errorExists = true;
				errorType = ERROR_TBL_NOT_EXISTS;
				errorContainerName = tblTemp.tableName;
			}
			else
			{
				//table exists and remove from database
				removeTable( dbms, dbReturn, tblReturn );

				//remove table/file
				tblTemp.tableDrop(currentWorkingDirectory, currentDatabase );
			}
		}
		else
		{
			errorExists = true;
			errorType = ERROR_INCORRECT_COMMAND;
			errorContainerName = originalInput;	
		}

	}
	else if( actionType.compare( ALTER ) == 0 ) 
	{
		containerType = getNextWord( input );
		if( containerType == TABLE_TYPE )
		{
			//call alter tbl function
			Database dbTemp;
			dbTemp.databaseName = currentDatabase;
			databaseExists( dbms, dbTemp, dbReturn );

			Table tblTemp;
			tblTemp.tableName = getNextWord( input );

			//check if table exists
			if( !(dbms[ dbReturn ].tableExists( tblTemp.tableName, tblReturn )) )
			{
				//if it doesnt exist then return error
				errorExists = true;
				errorType = ERROR_TBL_NOT_EXISTS;
				errorContainerName = tblTemp.tableName;
			}
			else
			{
				//remove table/file
				tblTemp.tableAlter( currentWorkingDirectory, currentDatabase, input, attrError );
			}


		}

	}
	else if( actionType.compare( EXIT ) == 0 )
	{
		exitProgram = true;
	}
	else
	{
		errorExists = true;
		errorType = ERROR_INCORRECT_COMMAND;
		errorContainerName = originalInput;
	}

	if( errorExists )
	{
		handleError( errorType, actionType, errorContainerName );
	}

	return exitProgram;
}





/**
 * @brief databaseExists
 *
 * @details checks whether dbInput exists in dbms
 *          
 * @pre dbms exists and dbInput exists
 *
 * @post returns true if dbExists, false otherwise
 *
 * @par Algorithm 
 *      loop through dbms return true if match is found
 *      
 * @exception None
 *
 * @param [in] dbms provides vector of dbs
 *
 * @param [in] dbInput provides db to be created
 *
 * @return bool
 *
 * @note None
 */
bool databaseExists( vector<Database> dbms, Database dbInput, int &dbReturn )
{
	int size = dbms.size();
	for( dbReturn = 0; dbReturn < size; dbReturn++ )
	{
		if( dbInput.databaseName == dbms[ dbReturn ].databaseName )
		{
			return true;
		}
	}
	return false;
}



void removeDatabase( vector< Database > &dbms, int index )
{
	dbms.erase( dbms.begin() + index );
}


void removeTable( vector< Database > &dbms, int dbReturn, int tblReturn )
{
	dbms[ dbReturn ].databaseTable.erase( dbms[ dbReturn ].databaseTable.begin() + tblReturn );
}

/**
 * @brief handleError
 *
 * @details takes an errorType and outputs an error based off of it
 *          
 * @pre errorType and errorContainerName exist
 *
 * @post outputs error to terminal
 *
 * @par Algorithm 
 *      checks errorTYpe and outputs accordingly
 *      
 * @exception None
 *
 * @param [in] errorType provides an int value of the appropriate error
 *
 * @param [in] errorContainerName provides error source
 *
 * @return None
 *
 * @note None
 */
void handleError( int errorType, string commandError, string errorContainerName )
{

	if( commandError == "SELECT" )
	{
		commandError = "query";
	}
	//convert to lowercase for use
	convertToLC( commandError );

	// if problem is that databse does exist (used for create db )
	if( errorType == ERROR_DB_EXISTS )
	{
		cout << "-- !Failed to " << commandError << " database " << errorContainerName;
		cout << " because it already exists." << endl;
	}
	//if problem is that database does not exist ( used for use, drop)
	else if( errorType == ERROR_DB_NOT_EXISTS )
	{
		cout << "-- !Failed to " << commandError << " database " << errorContainerName;
		cout << " because it does not exist." << endl;
	}
	//if problem is that table exists ( used for create table)
	else if( errorType == ERROR_TBL_EXISTS )
	{
		cout << "-- !Failed to " << commandError << " table " << errorContainerName;
		cout << " because it already exists." << endl;
	}
	//if problem is that table does not exist( used for alter, select, drop )
	else if( errorType == ERROR_TBL_NOT_EXISTS )
	{
		cout << "-- !Failed to " << commandError << " table " << errorContainerName;
		cout << " because it does not exist." << endl;
	}
	//if problem is that an unrecognized error occurs
	else if( errorType == ERROR_INCORRECT_COMMAND )
	{
		cout << "-- !Failed to complete command. "<< endl;
		cout << "-- !Incorrect instruction: " << errorContainerName << endl;
	}
}


/**
 * @brief convertToLC
 *
 * @details converts a string into lowercase
 *          
 * @pre Assumes stirng has uppercase values
 *
 * @post String is now in lowercase
 *
 * @par Algorithm 
 *      parse through the string and converts toLower using string lib function
 * 
 * @exception None
 *
 * @param [in] string &input
 *
 * @return None
 *
 * @note None
 */
void convertToLC( string &input )
{
	int size = input.size();
	for( int index = 0; index < size; index++ )
	{
		input[ index ] = tolower( input[ index ] );
	}
}

