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
	Cell(char piece , bool isJoker,int playerNumber);
	Cell();

	char getPiece() const;
	bool getIsJoker() const;
	int  getPlayerNumber() const;
	static void updateCell(Cell& cell, char piece, bool isJoker,int playerNumber);

private:
	char piece;
	bool isJoker;
	int playerNum;
};



#endif /* EX1_CELL_H_ */
