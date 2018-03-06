# cs457Hw1
Authors: Carli DeCapito, Sanya Gupta, Eugene Nelson CS457: Database Management Systems Programming Assignment #1 February 22, 2018

//////////////////////////////////////////////////////////////////////////////// About: This programming assignment required the group to implement the basic functionality of a SQLite Database Management System. The program designed by the group implements basic functionality of both databases and tables. The program can create and drop databases and tables. Databases are also able to utilize a “use” function, which allow it to “create”, “drop”, and “alter” tables within the database that calls the use function. As previously mentioned, tables within databases are also able to be created, deleted, and altered.

The program works similar to a terminal (similar to SQLite), where it is continuously requesting input. Once input is typed and entered into the terminal, the program parses that command and performs the necessary actions. The program will continue to ask for input until the command “.EXIT” is inputted.

//////////////////////////////////////////////////////////////////////////////// Commands: Database Commands Create- CREATE DATABASE ; Drop- DROP DATABASE ; Use- Use ;

Table Commands
        Create- CREATE TABLE <table name> (<attr name> <attr type>...);
        Drop-   DROP TABLE <table name>;
        Alter-  ALTER TABLE <table name> ADD <attr name> <attr type>;
        Select- SELECT * FROM <table name>;
//////////////////////////////////////////////////////////////////////////////// How to Run and Compile The user must first navigate to where the program is stored on the computer through terminal. A makefile is provided to compile this program so all the user has to do is type:

make
./main
The program should now be running. The user should now be prompted by a “>” symbol, which indicates that it is expecting user input.

//////////////////////////////////////////////////////////////////////////////// Special Circumstances The program requires that all SQL commands and keywords must be in caps: eg. CREATE, DROP, USE, ALTER, SELECT * FROM, .EXIT, DATABASE, TABLE, ADD Likewise, each line should end with a semicolon except for when exiting the program. If the user wishes to terminate the program, .EXIT must be type and does not include a semicolon at the end. Spacing also matters. This program follows the spacing convention that is displayed in the provided SQL test file.
# cs457pa2
# cs457pa2
