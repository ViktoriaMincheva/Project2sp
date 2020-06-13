#include "Spreadsheet.h"
#include "Operations.h"


Spreadsheet::Spreadsheet()
{
	Spreadsheet(1, 1);
}

Spreadsheet::Spreadsheet(int r, int c)
{
	rows = r;
	cols = c;
	spreadsheet = new Cell * [rows];
	for (int i = 0; i < rows; i++) {
		spreadsheet[i] = new Cell[cols];
	}
}

Spreadsheet::Spreadsheet(std::fstream& file)
{
	int columns = 0, rows = 0;
	Operations::getRowsAndColumns(file, &columns, &rows);
	this->rows = rows;
	this->cols = columns;

	spreadsheet = new Cell * [rows];
	for (int i = 0; i < rows; i++) {
		spreadsheet[i] = new Cell[columns];
	}

	file.clear();
	file.seekg(0, std::ios_base::beg);
	file.unsetf(std::ios::skipws);

	char line[255];
	int currRow = 0;

	while (file && currRow < rows) {
		file.getline(line, 255);
		int size = 0;

		bool findEmptyCells = false;

		for (int i = 0; i < strlen(line); i++) {
			if ((line[i] == ',') && (line[i + 1] == ',')) {
				findEmptyCells = true;
			}
		}

		if (strlen(line) == 0) {
			for (int i = 0; i < cols; i++) {
				spreadsheet[currRow][i].assignEmptyCell("\0");
			}
		}
		else if ((line[0] == ',') || (line[strlen(line) - 1] == ',') || findEmptyCells) { //,,23,hi,,
			
			bool findEmptyCells = false;
			bool passedTheFirstChar = false;
			bool lastCharReady = false;

			int spacesNeeded = 0;

			for (int i = 0; i < strlen(line); i++) {
				if ((line[i] == ',') && (line[i + 1] == ',')) {
					findEmptyCells = true;
					spacesNeeded++;
				}
				else if ((line[0] == ',') && passedTheFirstChar == false) {
					spacesNeeded++;
					passedTheFirstChar = true;
				}
				else if ((line[strlen(line) - 1] == ',') && lastCharReady == false) {
					spacesNeeded++;
					lastCharReady = true;
				}
			}

			//std::cout << "Needed spaces: " << spacesNeeded << "\n\n";

			char* lineCopy = new char[strlen(line) + 1];
			strcpy(lineCopy, line);
			char* lineCopy2 = new char[strlen(lineCopy) + 1 + spacesNeeded];
			strcpy(lineCopy2, lineCopy);

			int spacesPlaced = 0;

			for (int i = 0; i < strlen(lineCopy2); i++) {
				if ((lineCopy2[0] == ',') && i == 0) {
					lineCopy2[i] = ' ';
					for (int j = 1; j < strlen(lineCopy2) + 1; j++) {
						lineCopy2[j] = lineCopy[j - 1];
					}
					spacesPlaced++;
				}
				else if ((lineCopy2[i] == ',') && i == (strlen(lineCopy2) - 1)) {
					lineCopy2[i] = lineCopy[i - spacesPlaced];
					lineCopy2[i + 1] = ' ';
					lineCopy2[i + 2] = '\0';
					break;
				}
				else if ((lineCopy2[i] == ',') && (lineCopy2[i + 1] == ',')) {
					lineCopy2[i] = lineCopy[i - spacesPlaced];
					lineCopy2[i + 1] = ' ';
					spacesPlaced++;

					for (int j = i + 2; j < strlen(lineCopy2) + spacesPlaced; j++) {
						lineCopy2[j] = lineCopy[j - spacesPlaced];
					}
				}
			}
			char** splitLine = Operations::strSplit(lineCopy2, ',', &size);
			for (int i = 0; i < size; i++) {
				if (splitLine[i] == " ") {
					spreadsheet[currRow][i].assignEmptyCell("\0");
				}
				else {
					spreadsheet[currRow][i].assignCell(splitLine[i]);
				}
			}


		}
		else {
			char** splitLine = Operations::strSplit(line, ',', &size);

			for (int i = 0; i < size; i++) {
				spreadsheet[currRow][i].assignCell(splitLine[i]);
			}
		}
		currRow++;
	}
}

Spreadsheet::Spreadsheet(Spreadsheet& other)
{
	rows = other.rows;
	cols = other.cols;
	spreadsheet = new Cell * [rows];
	for (int i = 0; i < rows; i++) {
		spreadsheet[i] = new Cell[cols];
	}

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			spreadsheet[i][j] = other.spreadsheet[i][j];
		}
	}
}

Spreadsheet Spreadsheet::operator=(Spreadsheet& other)
{
	if (this != &other) {
		for (int i = 0; i < this->rows; i++) {
			delete[] spreadsheet[i];
		}
		delete[] spreadsheet;

		rows = other.rows;
		cols = other.cols;
		spreadsheet = new Cell * [rows];
		for (int i = 0; i < rows; i++) {
			spreadsheet[i] = new Cell[cols];
		}

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				spreadsheet[i][j] = other.spreadsheet[i][j];
			}
		}
	}

	return *this;
}

Spreadsheet::~Spreadsheet()
{
	for (int i = 0; i < this->rows; i++) {
		delete[] spreadsheet[i];
	}
	delete[] spreadsheet;
}

void Spreadsheet::printTable()
{
	int width = 20;
	int widthOfCell;
	for (int i = 0; i < rows; i++) {
		std::cout << "|";
		for (int j = 0; j < cols; j++) {
			spreadsheet[i][j].printCell();
			widthOfCell = spreadsheet[i][j].getSizeOfCell();
			std::cout << std::setw(width - widthOfCell) << "|";
		}
		std::cout << "\n";
	}
}


void Spreadsheet::setRows(int newRows)
{
	rows = newRows;
}

void Spreadsheet::setCols(int newCols)
{
	cols = newCols;
}

void Spreadsheet::setCell(int r, int c, char* value)
{
	spreadsheet[r][c].assignCell(value);
}

void Spreadsheet::getCell(int r, int c, std::fstream& file)
{ 
	spreadsheet[r][c].printCellToFile(file);
}

