#pragma once
#include <iostream>
#include <fstream>
#include <string.h>
#include <iomanip>
#include "Cell.h"


class Spreadsheet
{
private:
	Cell** spreadsheet;
	int rows;
	int cols;

 public:
	 Spreadsheet();
	 Spreadsheet(int, int);
	 Spreadsheet(std::fstream&);
	 Spreadsheet(Spreadsheet&);
	 Spreadsheet operator=(Spreadsheet&);
	 ~Spreadsheet();

	 void printTable();

	 void setRows(int newRows);
	 void setCols(int newCols);
	 void setCell(int, int, char*);
	 void getCell(int, int, std::fstream&);
};

