/*
 * RPSBoard.cpp
 *
 *  Created on: 24 ����� 2018
 *      Author: OR
 */
#include "RPSBoard.h"

RPSBoard::RPSBoard()
{
	vector<vector<Cell>> newboard(COLS+1, vector<Cell>(ROWS+1));
	for (int i = 0; i <= COLS; i++)
	{
		vector<Cell> rowVector;
		for (int j = 0; j <= ROWS; j++)
		{
			rowVector.push_back(Cell(0, false));
		}
		newboard.push_back(rowVector);
	}
	board = newboard;
}
RPSBoard::~RPSBoard() {}

int RPSBoard::getPlayer(const Point &pos) const
{
	int x = pos.getX();
	int y = pos.getY();
	char piece = board.at(x).at(y).getPiece();
	if (piece == 0)
		return 0;
	else if (isupper(piece))
		return 1;
	else
		return 2;
}
