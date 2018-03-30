/*
 * Cells.h
 *
 *  Created on: 27 ���� 2018
 *      Author: OR
 */

#ifndef EX1_CELL_H_
#define EX1_CELL_H_
#include <iostream>
#include <fstream>
#include <string>
#include <fstream>
#include "Constants.h"
using namespace std;

class Cell
{
	Cell(char piece , bool isJoker);
	Cell();

	char getPiece() const;
	bool getIsJoker() const;

	static void updateCell(Cell& cell, char piece, bool isJoker);

private:
	char piece;
	bool isJoker;
};



#endif /* EX1_CELL_H_ */
