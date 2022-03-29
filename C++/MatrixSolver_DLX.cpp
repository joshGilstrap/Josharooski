#include <iostream>
#include <random> //for RNG engine/distribution
#include "Node.h"

const int ROWS = 8;
const int COLS = 8;
const double RANDOM_SD = 20.0; //Standard deviation for RNG
const double RANDOM_MEAN = 49.5; //Mean for RNG 

void createBoolTable(bool boolTable[][COLS]);
void create_DLX_Matrix(bool boolTable[][COLS], Node Table[][COLS], Node*& headPtr);
void printBoolTable(bool boolTable[][COLS]);
void printTable(Node Table[][COLS], Node* headPtr);

int main() {
	Node* headPtr = new Node();
	bool(*boolTable)[COLS] = new bool[ROWS][COLS];
	Node(*Table)[COLS] = new Node[ROWS][COLS];

	createBoolTable(boolTable);
	printBoolTable(boolTable);
	create_DLX_Matrix(boolTable, Table, headPtr);

	std::cout << std::endl;
	printTable(Table, headPtr);
}

void createBoolTable(bool boolTable[][COLS]) {
	std::default_random_engine engine;
	std::normal_distribution<double> norm(RANDOM_MEAN, RANDOM_SD);

	for (int row = 0; row <=ROWS; row++) {
		for (int col = 0; col <COLS; col++) {
			if (row == 0) {
				boolTable[row][col] = true;
			}
			else {
				int num = norm(engine);
				if (num < 0) {
					num *= -1;
				}
				if(num < 75 && num > 25){
					boolTable[row][col] = false;
				}
				else {
					boolTable[row][col] = true;
				}
			}
		}
	}
}

void create_DLX_Matrix(bool boolTable[][COLS], Node Table[][COLS], Node*& headPtr) {

	for (int row = 0; row <=ROWS; row++) {
		for (int col = 0; col <COLS; col++) {
			if (boolTable[row][col]) {
				if (row) {
					Table[0][col].numNodes++;
				}
				
				Table[row][col].setColumn(&Table[0][col]);
				Table[row][col].inRow = row;
				Table[row][col].inCol = col;

				int rowCheck = row;
				int colCheck = col;
				do {
					colCheck = Table[row][colCheck].getLeftIndex(colCheck);
				} while (!boolTable[rowCheck][colCheck] && colCheck != col);
				Table[row][col].setLeft(&Table[row][colCheck]);

				rowCheck = row;
				colCheck = col;
				do {
					colCheck = Table[row][colCheck].getRightIndex(colCheck);
				} while (!boolTable[rowCheck][colCheck] && colCheck != col);
				Table[row][col].setRight(&Table[row][colCheck]);

				rowCheck = row;
				colCheck = col;
				do {
					rowCheck = Table[rowCheck][col].getUpIndex(rowCheck);
				} while (!boolTable[rowCheck][colCheck] && rowCheck != row);
				Table[row][col].setUp(&Table[rowCheck][col]);

				rowCheck = row;
				colCheck = col;
				do {
					rowCheck = Table[rowCheck][col].getDownIndex(rowCheck);
				} while (!boolTable[rowCheck][colCheck] && rowCheck != row);
				Table[row][col].setDown(&Table[rowCheck][col]);
			}
		}
	}

	headPtr->setRight(&Table[0][0]);
	headPtr->setLeft(&Table[0][COLS - 1]);

	Table[0][0].setLeft(headPtr);
	Table[0][COLS - 1].setRight(headPtr);
}

void printBoolTable(bool boolTable[][COLS]) {
	std::cout << "Bool Table:" << std::endl;
	for (int row = 0; row <= ROWS; row++) {
		for (int col = 0; col < COLS; col++) {
			if (boolTable[row][col]) {
				std::cout << "1 ";
			}
			else {
				std::cout << "0 ";
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void printTable(Node Table[][COLS], Node* headPtr) {
	std::cout << "Node Coordinates: " << std::endl;

	Node* rowPtr;
	Node* colPtr;
	Node* ptrInPlace;
	rowPtr = headPtr->getRight();
	colPtr = rowPtr->getDown();
	ptrInPlace = rowPtr;

	do { 
		do {
			if(colPtr->inRow){
				std::cout << '[' << colPtr->inCol + 1 << ',';
				std::cout << colPtr->inRow << "] ";
			}
			colPtr = colPtr->getDown();
		} while (colPtr != ptrInPlace);
		std::cout << std::endl;
		rowPtr = rowPtr->getRight();
		colPtr = rowPtr->getDown();
		ptrInPlace = rowPtr;
	} while (rowPtr != headPtr);
	std::cout << std::endl;

	rowPtr = nullptr;
	colPtr = nullptr;
	ptrInPlace = nullptr;
}