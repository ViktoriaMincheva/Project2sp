#include "Operations.h"
#include "Cell.h"
#include "Spreadsheet.h"
#include <string>

const char* Operations::OPEN = "open";
const char* Operations::CLOSE = "close";
const char* Operations::PRINT = "print";
const char* Operations::EDIT = "edit";
const char* Operations::SAVE = "save";
const char* Operations::HELP = "help";
const char* Operations::EXIT = "exit";



void Operations::openFile(std::fstream& file, char* fileName)
{
	file.open(fileName, std::ios::in | std::ios::out);

	if (!file.is_open()) {

		std::cerr << "Soryy, I couldn't open the file! Try again or type in: help!\n";

	}
	else {
			std::cout << "Successfully opened " << fileName << std::endl;
	}
}

bool Operations::isFileOpen(std::fstream& file)
{
	if (!file.is_open()) {
		return false;
	}
	return true;
}

void Operations::closeFile(std::fstream& file, char* fileName)
{
	if (!file.is_open()) {
		std::cerr << "You can not close a file that it's not opened yet!" << std::endl;
	}
	else {
		file.close();
		std::cout << fileName << " is successfully closed!" << std::endl;
	}
}

void Operations::getRowsAndColumns(std::fstream& file, int* maxColumns, int* maxRows)
{
	*maxColumns = 0;
	*maxRows = 0;

	char line[255];

	file.clear();
	file.seekg(0, std::ios_base::beg);
	file.unsetf(std::ios::skipws);

	while (file) {
		file.getline(line, 255);
		int size = strlen(line);
		int currMaxColumns = 0;

		for (int i = 0; i < size; i++) {
			if (line[i] == ',') {
				currMaxColumns++;
			}

		}
		if (currMaxColumns > * maxColumns) {
			*maxColumns = currMaxColumns;
		}
		(*maxRows)++;
	}

	*maxRows -= 1;
	*maxColumns += 1;
}

void Operations::editCell(std::fstream& file, char* line, int* editRow, int* editColumn, Spreadsheet* s)
{
	if (!file.is_open()) {
		std::cerr << "The file is not opened yet!" << std::endl <<
			"If you want to use it, please open it first!" << std::endl;
	}
	else {
		int columns = 0, rows = 0;
		getRowsAndColumns(file, &columns, &rows);

		char* lineCopy = new char[strlen(line) + 1];
		strcpy(lineCopy, line);

		char* token = strtok(lineCopy, " ");
		int tokenCounter = 0;

		while (token != NULL) {
			tokenCounter++;
			token = strtok(NULL, " ");
		}
		if (tokenCounter < 3) {
			std::cout << "Invalid input! Edit function should look like this:\n edit R<N>C<M> <newValue>\n";
			return;
		}
		//del(token);
		//del(lineCopy);
		
		int size = 0;
		char** line_ = strSplit(line, ' ', &size);
		
		int sizeOftheSecondToken = strlen(line_[1]);
		int sizeOfTheNewValue = strlen(line_[2]);

		if (size > 3) {
			char* inputStr = new char[1];
			inputStr[0] = '\0';

			for (int i = 2; i < size; i++) {
				inputStr = strcat(inputStr, line_[i]);
				if (i + 1 != size) {
					inputStr = strcat(inputStr, " ");
				}
			}
			strcpy(line_[2], inputStr);
			//del(inputStr);
		}
		
		*editColumn = 0, *editRow = 0;
		switch (sizeOftheSecondToken) {
		  case 4: {
			  editCaseFour(line_[1], editColumn, editRow);
			  break;
		  }
		  case 5: {
			  editCaseFive(line_[1], editColumn, editRow);
			  break;
		  }
		  case 6: {
			  editCaseSix(line_[1], editColumn, editRow);
			  break;
		  }
		  default: {
			  std::cerr << "Error! Either invalid input or you've entered larger numbers (>99). Must be: R<N>C<M>!\n";
		  }
		}
		(*editColumn)--;
		(*editRow)--;
		

		if (*editColumn >= columns || *editRow >= rows || *editColumn < 0 || *editRow < 0) {
			std::cout << "The cell you want to change is out of the table!\n";
			return;
		}

		if (size > 3) {
			if (line_[2][0] == '=') {

				int count = 0;
				for (int i = 0; line_[2][i]; i++) {
					if (line_[2][i] != ' ') {
						line_[2][count] = line_[2][i];
						count++;
					}
				}
				line_[2][count] = '\0';

				char* newLine2 = new char[1];
				newLine2 = equal(line_[2], s);
				if (strlen(newLine2) == 0) {
					return;
				}
				s->setCell(*editRow, *editColumn, newLine2);
				std::cout << "Cell successfully edited!\n";
				//del(newLine2);
				return;
			}
		}
			s->setCell(*editRow, *editColumn, line_[2]);
			std::cout << "Cell successfully edited!\n";
			
	}
}

char * Operations::equal(char* line, Spreadsheet* s)
{
	int sizeOfline = strlen(line);
	int operators = 0;
	int operatorsPlace = 0;
	for (int i = 0; i < strlen(line); i++) {
		if (line[i] == '+' || line[i] == '-' || line[i] == '*' || line[i] == '/' || line[i] == '^') {
			operatorsPlace = i; 
			operators++;
		}
	}

	if (operators != 1) { 
		return line;
	}
	char error[1] = "";
	bool isFirstString = false;
	bool isFirstAddress = false; 

	char* firstValue = new char[1];
	

	int firstCounter = 0;
	for (int i = 1; i < operatorsPlace; i++) {
		firstValue[firstCounter] = line[i];
		firstCounter++;
	}
	firstValue[firstCounter] = '\0';

	bool isSecondString = false;
	bool isSecondAddress = false;

	char* secondValue = new char[1];
	int secondCounter = 0;
	for (int i = operatorsPlace + 1; i < sizeOfline; i++) {
		secondValue[secondCounter] = line[i];
		secondCounter++;
	}
	secondValue[secondCounter] = '\0';

	double d1 = 0;
	int i1 = 0;
	char* c1 = new char[1];
	int colu = -1;
	int ro = -1;

	if (isDouble(firstValue)) {
		d1 = std::stod(firstValue);
	}
	else if (isInt(firstValue)) {
		i1 = std::stoi(firstValue);
	}
	else {
		int sizeOfC = strlen(firstValue);

		switch (sizeOfC) {
		case 4: {
			editCaseFour(firstValue, &colu, &ro);
			isFirstAddress = true;
			break;
		}
		case 5: {
			editCaseFive(firstValue, &colu, &ro);
			isFirstAddress = true;
			break;
		}
		case 6: {
			editCaseSix(firstValue, &colu, &ro);
			isFirstAddress = true;
			break;
		}
		default: {
			isFirstString = true; }
		}
	}

	double d2 = 0;
	int i2 = 0;
	char* c2 = new char[1];
	int colu2 = -1;
	int ro2 = -1;

	if (isDouble(secondValue)) {
		d2 = std::stod(secondValue);
	}
	else if (isInt(secondValue)) {
		i2 = std::stoi(secondValue);
	}
	else {
		int sizeOfC = strlen(secondValue);

		switch (sizeOfC) {
		case 4: {
			editCaseFour(secondValue, &colu2, &ro2);
			isSecondAddress = true;
			break;
		}
		case 5: {
			editCaseFive(secondValue, &colu2, &ro2);
			isSecondAddress = true;
			break;
		}
		case 6: {
			editCaseSix(secondValue, &colu2, &ro2);
			isSecondAddress = true;
			break;
		}
		default: {
			i2 = 0;
			isSecondString = true;
			}
		}
	}

	int resultValue = 0;
	double resultValued = 0;
	int size;
	char* newLine = new char[1];

	if (line[operatorsPlace] == '+') {
		if (isInt(firstValue) && isInt(secondValue)) {
			resultValue = i1 + i2;
			size = sizeof(resultValue);
			snprintf(newLine, size, "%d", resultValue);
			return newLine;
		}
		else if (isInt(firstValue) && isDouble(secondValue)) {
			resultValued = i1 + d2;
			size = sizeof(resultValued);
			snprintf(newLine, size, "%g", resultValued);
			return newLine;
		}
		else if (isDouble(firstValue) && isInt(secondValue)) {
			resultValued = d1 + i2;
			size = sizeof(resultValued);
			snprintf(newLine, size, "%g", resultValued);
			return newLine;
		}
		else if (isDouble(firstValue) && isDouble(secondValue)) {
			resultValued = d1 + d2;
			size = sizeof(resultValued);
			snprintf(newLine, size, "%g", resultValued);
			return newLine;
		}
		else if ((isFirstString && isInt(secondValue)) || (isFirstAddress && isInt(secondValue))) {
			resultValue = 0 + i2;
			size = sizeof(resultValue);
			snprintf(newLine, size, "%d", resultValue);
			return newLine;
		}
		else if (((isInt(firstValue) && isSecondString)) || (isInt(firstValue) && isSecondAddress)) {
			resultValue = i1 + 0;
			size = sizeof(resultValue);
			snprintf(newLine, size, "%d", resultValue);
			return newLine;
		}
		else if ((isFirstString && isDouble(secondValue)) || (isFirstAddress && isDouble(secondValue))) {
			resultValued = 0 + d2;
			size = sizeof(resultValued);
			snprintf(newLine, size, "%g", resultValued);
			return newLine;
		}
		else if ((isDouble(firstValue) && isSecondString) || ((isDouble(firstValue) && isSecondAddress))) {
			resultValued = d1 + 0;
			size = sizeof(resultValued);
			snprintf(newLine, size, "%g", resultValued);
			return newLine;
		}
		else if ((isFirstAddress && isSecondAddress) || (isFirstString && isSecondString) || (isFirstString && isSecondAddress) || (isFirstAddress && isSecondString)) {
			resultValue = 0;
			size = sizeof(resultValue);
			snprintf(newLine, size, "%d", resultValue);
			return newLine;
		}
		else {
			std::cout << "Something went wrong! Can't fulfill this operation!\n";
			return error;
		}
	}
	else if (line[operatorsPlace] == '-') {
		if (isInt(firstValue) && isInt(secondValue)) {
			resultValue = i1 - i2;
			size = sizeof(resultValue);
			snprintf(newLine, size, "%d", resultValue);
			return newLine;
		}
		else if (isInt(firstValue) && isDouble(secondValue)) {
			resultValued = i1 - d2;
			size = sizeof(resultValued);
			snprintf(newLine, size, "%g", resultValued);
			return newLine;
		}
		else if (isDouble(firstValue) && isInt(secondValue)) {
			resultValued = d1 - i2;
			size = sizeof(resultValued);
			snprintf(newLine, size, "%g", resultValued);
			return newLine;
		}
		else if (isDouble(firstValue) && isDouble(secondValue)) {
			resultValued = d1 - d2;
			size = sizeof(resultValued);
			snprintf(newLine, size, "%g", resultValued);
			return newLine;
		}
		else if ((isFirstString && isInt(secondValue)) || (isFirstAddress && isInt(secondValue))) {
			resultValue = 0 - i2;
			size = sizeof(resultValue);
			snprintf(newLine, size, "%d", resultValue);
			return newLine;
		}
		else if (((isInt(firstValue) && isSecondString)) || (isInt(firstValue) && isSecondAddress)) {
			resultValue = i1 - 0;
			size = sizeof(resultValue);
			snprintf(newLine, size, "%d", resultValue);
			return newLine;
		}
		else if ((isFirstString && isDouble(secondValue)) || (isFirstAddress && isDouble(secondValue))) {
			resultValued = 0 - d2;
			size = sizeof(resultValued);
			snprintf(newLine, size, "%g", resultValued);
			return newLine;
		}
		else if ((isDouble(firstValue) && isSecondString) || ((isDouble(firstValue) && isSecondAddress))) {
			resultValued = d1 - 0;
			size = sizeof(resultValued);
			snprintf(newLine, size, "%g", resultValued);
			return newLine;
		}
		else if ((isFirstAddress && isSecondAddress) || (isFirstString && isSecondString) || (isFirstString && isSecondAddress) || (isFirstAddress && isSecondString)) {
			resultValue = 0;
			size = sizeof(resultValue);
			snprintf(newLine, size, "%d", resultValue);
			return newLine;
		}
		else {
			std::cout << "Something went wrong! Can't fulfill this operation!\n";
			return error;
		}
	}
	else if (line[operatorsPlace] == '*') {
		if (isInt(firstValue) && isInt(secondValue)) {
			resultValue = i1 * i2;
			size = sizeof(resultValue);
			snprintf(newLine, size, "%d", resultValue);
			return newLine;
		}
		else if (isInt(firstValue) && isDouble(secondValue)) {
			resultValued = i1 * d2;
			size = sizeof(resultValued);
			snprintf(newLine, size, "%g", resultValued);
			return newLine;
		}
		else if (isDouble(firstValue) && isInt(secondValue)) {
			resultValued = d1 * i2;
			size = sizeof(resultValued);
			snprintf(newLine, size, "%g", resultValued);
			return newLine;
		}
		else if (isDouble(firstValue) && isDouble(secondValue)) {
			resultValued = d1 * d2;
			size = sizeof(resultValued);
			snprintf(newLine, size, "%g", resultValued);
			return newLine;
		}
		else if ((isFirstString && isInt(secondValue)) || ((isInt(firstValue) && isSecondString)) || (isFirstAddress && isInt(secondValue)) 
			 || (isInt(firstValue) && isSecondAddress) || (isFirstAddress && isSecondAddress) || (isFirstString && isSecondString) 
			 || (isFirstString && isSecondAddress) || (isFirstAddress && isSecondString)) {
			   resultValue = 0;
			   size = sizeof(resultValue);
			   snprintf(newLine, size, "%d", resultValue);
			   return newLine;
		     }
		else if ((isFirstString && isDouble(secondValue)) || (isDouble(firstValue) && isSecondString) || (isFirstAddress && isDouble(secondValue)) 
			|| (isDouble(firstValue) && isSecondAddress)) {
			   resultValue = 0;
			   size = sizeof(resultValue);
			   snprintf(newLine, size, "%d", resultValue);
			   return newLine;
		     }
		else {
			std::cout << "Something went wrong! Can't fulfill this operation!\n";
			return error;
		}
	}
	else if (line[operatorsPlace] == '/') {
		if (isInt(firstValue) && isInt(secondValue)) {
			if (i2 == 0) {
			std::cout << "Dividing by 0 is not possible!\n";
			return error;
			}
			resultValued = i1 / i2;
			size = sizeof(resultValued);
			snprintf(newLine, size, "%g", resultValued);
			return newLine;
		}
		else if (isInt(firstValue) && isDouble(secondValue)) {
			if (d2 == 0) {
				std::cout << "Dividing by 0 is not possible!\n";
				return error;
			}
			resultValued = i1 / d2;
			size = sizeof(resultValued);
			snprintf(newLine, size, "%g", resultValued);
			return newLine;
		}
		else if (isDouble(firstValue) && isInt(secondValue)) {
			if (i2 == 0) {
				std::cout << "Dividing by 0 is not possible!\n";
				return error;
			}
			resultValued = d1 / i2;
			size = sizeof(resultValued);
			snprintf(newLine, size, "%g", resultValued);
			return newLine;
		}
		else if (isDouble(firstValue) && isDouble(secondValue)) {
			if (d2 == 0) {
				std::cout << "Dividing by 0 is not possible!\n";
				return error;
			}
			resultValued = d1 / d2;
			size = sizeof(resultValued);
			snprintf(newLine, size, "%g", resultValued);
			return newLine;
		}
		else if ((isFirstString && isInt(secondValue)) || (isFirstAddress && isInt(secondValue))) {
			if (i2 == 0) {
				std::cout << "Dividing by 0 is not possible!\n";
				return error;
			}
			resultValue = 0 / i2;
			size = sizeof(resultValue);
			snprintf(newLine, size, "%d", resultValue);
			return newLine;
		}
		else if ((isInt(firstValue) && isSecondString) || (isInt(firstValue) && isSecondAddress) || (isFirstAddress && isSecondAddress) 
			|| (isFirstString && isSecondString)  || (isFirstString && isSecondAddress) || (isFirstAddress && isSecondString)) {
			   std::cout << "Dividing by 0 is not possible!\n";
			   return error;
		     }
		else if ((isFirstString && isDouble(secondValue)) || (isFirstAddress && isDouble(secondValue))) {
			resultValue = 0;
			size = sizeof(resultValue);
			snprintf(newLine, size, "%d", resultValue);
			return newLine;
		}
		else if ((isDouble(firstValue) && isSecondString) || ((isDouble(firstValue) && isSecondAddress))) {
			std::cout << "Dividing by 0 is not possible!\n";
			return error;
		}
		else {
			std::cout << "Something went wrong! Can't fulfill this operation!\n";
			return error;
		}
	}
	else if (line[operatorsPlace] == '^') {
		if (isInt(firstValue) && isInt(secondValue)) {
			resultValued = pow (i1, i2);
			size = sizeof(resultValued);
			snprintf(newLine, size, "%g", resultValued);
			return newLine;
		}
		else if (isInt(firstValue) && isDouble(secondValue)) {
			resultValued = pow(i1, d2);
			size = sizeof(resultValue);
			snprintf(newLine, size, "%g", resultValue);
			return newLine;
		}
		else if (isDouble(firstValue) && isInt(secondValue)) {
			resultValued = pow(d1, i2);
			size = sizeof(resultValue);
			snprintf(newLine, size, "%g", resultValue);
			return newLine;
		}
		else if (isDouble(firstValue) && isDouble(secondValue)) {
			resultValued = pow(d1, d2);
			size = sizeof(resultValue);
			snprintf(newLine, size, "%g", resultValue);
			return newLine;
		}
		else if ((isFirstString && isInt(secondValue)) || (isFirstAddress && isInt(secondValue))) {
			resultValue = 0;
			size = sizeof(resultValue);
			snprintf(newLine, size, "%d", resultValue);
			return newLine;
		}
		else if (((isInt(firstValue) && isSecondString)) || (isInt(firstValue) && isSecondAddress)) {
			resultValue = pow(i1,0);
			size = sizeof(resultValue);
			snprintf(newLine, size, "%d", resultValue);
			return newLine;
		}
		else if ((isFirstString && isDouble(secondValue)) || (isFirstAddress && isDouble(secondValue)) ) {
			resultValue = 0;
			size = sizeof(resultValue);
			snprintf(newLine, size, "%d", resultValue);
			return newLine;
		}
		else if (((isDouble(firstValue) && isSecondString)) || ((isDouble(firstValue) && isSecondAddress))) {
			resultValued = d1;
			size = sizeof(resultValued);
			snprintf(newLine, size, "%g", resultValued);
			return newLine;
		}
		else if ((isFirstAddress && isSecondAddress) || (isFirstString && isSecondString) || (isFirstString && isSecondAddress) || (isFirstAddress && isSecondString)) {
			resultValue = 0;
			size = sizeof(resultValue);
			snprintf(newLine, size, "%d", resultValue);
			return newLine;
		}
		else {
			std::cout << "Something went wrong! Can't fulfill this operation!\n";
			return error;
		}
	}
	
}

void Operations::save(std::fstream& file, Spreadsheet* s)
{
	if (!file.is_open()) {
		std::cerr << "The file is not opened yet!" << std::endl <<
			"If you want to use it, please open it first!" << std::endl;

	}
	else {
		int columns = 0, rows = 0;
		getRowsAndColumns(file, &columns, &rows);

		file.clear();
		file.seekg(0, std::ios_base::beg);
		file.unsetf(std::ios::skipws);

		std::fstream temp;
		temp.open("temp.txt", std::ios_base::out);

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				if (j == columns - 1) {
					s->getCell(i, j, temp);
				}
				else {
					s->getCell(i, j, temp);
					temp << ",";
				}
			}
			temp << "\n";
		}

		file.close();
		temp.close();
		
		remove("spreadsheet.txt");
		if (rename("temp.txt", "spreadsheet.txt") != 0) {
			std::cerr << "Copy fail!!" << std::endl;
		}
		else {
			file.open("spreadsheet.txt", std::ios::in | std::ios::out);
			std::cout << "Changes successfully saved!\n";
		}

	}
}

void Operations::saveas(std::fstream& file, char* newName, Spreadsheet* s)
{
	if (!file.is_open()) {
		std::cerr << "The file is not opened yet!" << std::endl <<
			"If you want to use it, please open it first!" << std::endl;
	}
	else {
		int columns = 0, rows = 0;
		getRowsAndColumns(file, &columns, &rows);

		file.clear();
		file.seekg(0, std::ios_base::beg);
		file.unsetf(std::ios::skipws);

		std::fstream newFile;
		newFile.open(newName, std::ios_base::out);

		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++) {
				if (j == columns - 1) {
					s->getCell(i, j, newFile);
				}
				else {
					s->getCell(i, j, newFile);
					newFile << ",";
				}
			}
			newFile << "\n";
		}
		

		std::cout << "Successfully saved as " << newName << std::endl;
		newFile.close();
	}
}

void Operations::help()
{
	    std::cout << "\nYou can only use the following commands: \n"
		<< "open <fileName>                                     opens <file>\n"
		<< "print                                               prints the current information in the file you opened\n"
		<< "save                                                saves the currently open file\n"
		<< "save as <newFileName>                               saves the currently open file in <newFileName>\n"
		<< "close                                               closes currently opened file\n"
		<< "= <RnumberCnumber> operationType <RnumberCnumber>   equations with data from the spreadsheet\n"
		<< "help                                                prints this information\n"
		<< "exit                                                exists the program\n";
}

char** Operations::strSplit(char* word, char delimiter, int* size)
{
	int delimiters = 0;

	for (size_t i = 0; i < strlen(word); i++) {
		if (word[i] == delimiter) {
			delimiters++;
		}
	}

	*size = delimiters + 1;

	char** result = new char* [delimiters];
	char* token = strtok(word, &delimiter);

	int i = 0;
	while (token != NULL) {
		result[i] = new char[strlen(token) + 1];
		strcpy(result[i], token);
	    //}
		i++;

		token = strtok(NULL, &delimiter);
	}
	return result;
}

bool Operations::isInt(char* input)
{
	if (strlen(input) == 0) {
		return false;
	}

	int negative = 0;
	int sizeOfInput = strlen(input);

	for (int i = 0; i < sizeOfInput; i++) {
		if (input[i] == '-') {
			negative++;
		}
		else if (input[i] < '0' || input[i] > '9') {
			return false;
		}
	}

	if (negative > 1) {
		return false;
	}
	if (negative == 1 && input[0] != '-') {
		return false;
	}

	return true;
}

bool Operations::isDouble(char* input)
{
	if (strlen(input) == 0) {
		return false;
	}

	int negative = 0;
	int points = 0;
	int sizeOfInput = strlen(input);
	for (int i = 0; i < sizeOfInput; i++) {
		if (input[i] == '-') {
			negative++;
		} else if (input[i] == '.') {
			points++;
		} else if (input[i] < '0' || input[i] > '9') {
			return false;
		}
	}

	if (negative > 1) {
		return false;
	}
	if (negative == 1 && input[0] != '-') {
		return false;
	}
	if (points != 1) {
		return false;
	}
	if (points == 1 && negative == 0 && input[0] == '.') {
		return false;
	} 
	if (points == 1 && negative == 1 && input[1] == '.') {
		return false;
	}

	return true;
}

bool Operations::checkIfHasOnlyDigits(char token)
{
	if (!isdigit(token)) {
			return false;
	}
	return true;
}

void Operations::editCaseFour(char* line, int* column, int* row)
{
	if (line[0] != 'R') {
		std::cout << "Invalid input! Must be: R<N>C<M>!\n";
	} else {
		if (checkIfHasOnlyDigits(line[1]) == false) {
			std::cout << "Invalid input! Must be: R<N>C<M>!\n";
			return;
		} else {
			if (line[2] != 'C') {
				std::cout << "Invalid input! Must be: R<N>C<M>!\n";
				return;
			} else {
				if (checkIfHasOnlyDigits(line[3]) == false) {
					std::cout << "Invalid input! Must be: R<N>C<M>!\n";
					return;
				}
			   *row = line[1] - '0'; //std::stoi 
			   *column = line[3] - '0';
			}
		}
	}
}

void Operations::editCaseFive(char* line, int* column, int* row)
{
	if (line[0] != 'R') {
		std::cout << "Invalid input! Must be: R<N>C<M>!\n";
		return;
	}
	else {
		if (checkIfHasOnlyDigits(line[1]) == false) {
			std::cout << "Invalid input! Must be: R<N>C<M>!\n";
			return;
		}
		else {
			if (line[2] != 'C' && checkIfHasOnlyDigits(line[2]) == false) {
		 	    std::cout << "Invalid input! Must be: R<N>C<M>!\n";
				return;
			}
			else if (checkIfHasOnlyDigits(line[2]) == true) {
				if (line[3] != 'C') {
					std::cout << "Invalid input! Must be: R<N>C<M>!\n";
					return;
				}
				else {
					if (checkIfHasOnlyDigits(line[4]) == false) {
						std::cout << "Invalid input! Must be: R<N>C<M>!\n";
						return;
					}
					else {
						*row = (line[1] - '0') * 10 + (line[2] - '0');
						*column = line[4] - '0';
						std::cout << "row: " << *row << " column: " << *column << "\n";
					}
				}
			}
			else { // ako si e C line[2] = C
				if (checkIfHasOnlyDigits(line[3]) == true && checkIfHasOnlyDigits(line[4]) == true) {
					*row = line[1] - '0';
					*column = (line[3] - '0') * 10 + (line[4] - '0');
					std::cout << "row: " << *row << " column: " << *column << "\n";
				}
				else {
					std::cout << "Invalid input! Must be: R<N>C<M>!\n";
					return;
				}
			}
		}
	}
}

void Operations::editCaseSix(char* line, int* column, int* row)
{
	if (line[0] != 'R' || line[3] != 'C') {
		std::cout << "Invalid input! Must be: R<N>C<M>!\n";
		return;
	}
	else if (checkIfHasOnlyDigits(line[1]) == false || checkIfHasOnlyDigits(line[2]) == false || checkIfHasOnlyDigits(line[4]) == false || checkIfHasOnlyDigits(line[5]) == false) {
		std::cout << "Invalid input! Must be: R<N>C<M>!\n";
		return;
	}
	else {
		*row = (line[1] - '0') * 10 + (line[2] - '0');
		*column = (line[4] - '0') * 10 + (line[5] - '0');
		std::cout << "row: " << *row << " column: " << *column << "\n";
	}
}

/*void Operations::del(char* input)
{
	delete input;
}*/
