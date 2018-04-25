/*
 * RPSBoard.cpp
 *
 *  Created on: 24 ����� 2018
 *      Author: OR
 */
#include "RPSBoard.h"

RPSBoard::RPSBoard(){
	board(ROWS, vector<char>(COLS));
}
RPSBoard::~RPSBoard(){}

int RPSBoard::getPlayer(const Point& pos) const{
	int x = pos.getX();
	int y = pos.getY();
	char piece = board.at(x-1).at(y-1);
	if (piece == 0) return 0;
	else if (isupper(piece)) return 1;
	else return 2;
}

