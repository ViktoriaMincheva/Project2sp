#pragma once

#include <iostream>


enum CellType {
  i, d, str //int, double, char*
};

union CellValue {
	int i;
	double d;
	char* c;
};

class Cell
{
private:
	CellType type;
	CellValue value;

public:
	Cell();
	Cell(CellType, CellValue);
	Cell(Cell&);
	Cell operator=(Cell&);

	void assignCell(char*);
	void assignEmptyCell(const char*);
	CellType checkCellType(char *);
	void printCell();
	void printCellToFile(std::fstream&);

	int getSizeOfCell();
	CellType getType();
	CellValue getValue();

	template <typename T>
	void reassignCell(Cell* c, T value, CellType ct) {
		if (ct == i) {
			c->type = i;
			c->value.i = value;
		}
		else if (ct = d) {
			c->type = d;
			c->value.d = value;
		}
	}

	template <>
	void reassignCell(Cell* c, char* value, CellType ct) {
		c->type = str;
		c->value.c = new char[strlen(value) + 1];
		strcpy(c->value.c, value);
	}
};

