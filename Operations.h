#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

class Spreadsheet;

class Operations
{
public:
	static const char* OPEN;
	static const char* PRINT;
	static const char* EDIT;
	static const char* CLOSE;
	static const char* SAVE;
	static const char* HELP;
	static const char* EXIT;


	static void openFile(std::fstream&, char*);
	static bool isFileOpen(std::fstream&);
	static void closeFile(std::fstream&, char*);
	static void getRowsAndColumns(std::fstream&, int*, int*);
	static void editCell(std::fstream&, char*, int*, int*, Spreadsheet *);
	static char* equal(char*, Spreadsheet*);
	static void save(std::fstream&, Spreadsheet*);
	static void saveas(std::fstream&, char*, Spreadsheet*);
	static void help();
	static char** strSplit(char*, char, int*);
	static bool isInt(char*);
	static bool isDouble(char*);
	static bool checkIfHasOnlyDigits(char);
	static void editCaseFour(char *, int*, int*);
	static void editCaseFive(char*, int*, int*);
	static void editCaseSix(char*, int*, int*);
	//static void del(char*);

};

