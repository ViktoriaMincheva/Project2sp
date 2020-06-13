#include <iostream>
#include <fstream>
#include <string>
#include "Operations.h"
#include "Spreadsheet.h"

#define MAX_LINE 100

int main()
{
	char line[MAX_LINE];
	std::fstream file;
	Spreadsheet* s = new Spreadsheet;
	while (true) {
		std::cin.getline(line, MAX_LINE);
		char* lineCop = new char[strlen(line) + 1];
		strcpy(lineCop, line);
		char* word = strtok(line, " ");
		char* fileName = new char[1];
		
		if (word != NULL) {
			if (strcmp(word, Operations::OPEN) == 0) { //t.e. първата дума е open
				fileName = strtok(NULL, " ");
				if (fileName == nullptr) {
					std::cerr << "Please give name to the file you want to open!\n\n";
				}
				else {
					Operations::openFile(file, fileName);
					bool isFile = Operations::isFileOpen(file);
					if (isFile) {
						s = new Spreadsheet(file);
					}
					std::cout << "\n";
				}
			}
			else if (strcmp(word, Operations::CLOSE) == 0) {
				char* cfileName;
				cfileName = strtok(NULL, " ");
				if (cfileName == nullptr) {
					std::cerr << "Please give name to the file you want to close!\n\n";
				}
				else {
					Operations::closeFile(file, cfileName);
					delete[] fileName;
				}
				std::cout << "\n";
			}
			else if (strcmp(word, Operations::PRINT) == 0) {
				if (!file.is_open()) {
					std::cerr << "The file is not opened yet!" << std::endl <<
						"If you want to use it, please open it first!" << std::endl;
				}
				else {
					s->printTable();
				}
				std::cout << "\n"; 
			}
			else if (strcmp(word, Operations::EDIT) == 0) {
				int editRow, editColumn;
				Operations::editCell(file, lineCop, &editRow, &editColumn, s);
				std::cout << "\n";
			}
			else if (strcmp(word, Operations::SAVE) == 0) {
				
				char* checkIfAs;
				checkIfAs = strtok(NULL, " ");

				if (checkIfAs == NULL) {
					
					Operations::save(file, s);
				}
				else if (strcmp(checkIfAs, "as") == 0) {
					char* newName;
					newName = strtok(NULL, " ");
					Operations::saveas(file, newName, s);
				}
				else {
					std::cout << "Invalid command!" << std::endl;
				}
				std::cout << "\n";
			}
		    else if (strcmp(word, Operations::HELP) == 0) {
				Operations::help();
				std::cout << "\n";
			}
			else if (strcmp(word, Operations::EXIT) == 0) { //done
				file.close();
				std::cout << "Exit!" << std::endl;
				s->~Spreadsheet();
				return 0;
			}
		    else {
				std::cout << "Command not recognized! Try again or try the command <help>!" << std::endl;
			}
		}
	}

	return 0;
}