#include "Cell.h"
#include "Operations.h"

Cell::Cell() 
{
	type = str;
	value.c = new char[1];
	strcpy(value.c, "\0");
}

Cell::Cell(CellType type, CellValue value)
{
	this->type = type;

	if (type == i || type == d) {
		this->value = value;
	}
	else {
		this->value.c = new char[strlen(value.c) + 1];
		strcpy(this->value.c, value.c);
	}
}

Cell::Cell(Cell& other)
{
	type = other.type;
	value = other.value;
}

Cell Cell::operator=(Cell& other)
{
	if (this != &other) {
		type = other.type;
		value = other.value;
	}

	return *this;
}


void Cell::assignCell(char* input)
{
	if (Operations::isDouble(input)) {
		this->type = d;
		this->value.d = std::stod(input);
	}
	else if (Operations::isInt(input)) {
		this->type = i;
		this->value.i = std::stoi(input);
	}
	else {
		this->type = str;
		this->value.c = new char[strlen(input) + 1];
		strcpy(this->value.c, input);
	}
}

void Cell::assignEmptyCell(const char * input)
{
	this->type = str;
	this->value.c = new char[1];
	strcpy(this->value.c, input);
}

CellType Cell::checkCellType(char* input)
{
	if (Operations::isDouble(input)) {
		return i;
	}
	else if (Operations::isInt(input)) {
		return d;
	}
	else {
		return str;
	}

}

void Cell::printCell()
{
	if (this->type == d) {
		std::cout << this->value.d; 
	}
	else if (this->type == i) {
		std::cout << this->value.i;
	}
	else {
		std::cout << this->value.c;
	}
}

void Cell::printCellToFile(std::fstream& file)
{
	if (this->type == d) {
		file << this->value.d;
	}
	else if (this->type == i) {
		file << this->value.i;
	}
	else {
		file << this->value.c;
	}
}

int Cell::getSizeOfCell()
{
	int sizeToReturn;
	int size;
	char* newLine = new char[1];
	if (this->type == d) {
		size = sizeof(this->value.d);
		snprintf(newLine, size, "%g", this->value.d);
		sizeToReturn = strlen(newLine);
	}
	else if (this->type == i) {
		size = sizeof(this->value.i);
		snprintf(newLine, size, "%d", this->value.i);
		sizeToReturn = strlen(newLine);
	}
	else {
		sizeToReturn = strlen (this->value.c);
	}

	return sizeToReturn;
}

CellType Cell::getType()
{
	return type;
}

CellValue Cell::getValue()
{
	return value;
}
