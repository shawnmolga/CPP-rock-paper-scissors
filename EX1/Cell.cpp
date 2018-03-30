/*
 * Cells.cpp
 *
 *  Created on: 27 ���� 2018
 *      Author: OR
 */
#include "Cell.h"

Cell::Cell(char playerPiece , bool isPieceJoker, int playerNumber) : piece(playerPiece), isJoker(isPieceJoker), playerNum(playerNumber){};
Cell::Cell() : piece(0), isJoker(false),playerNum(-1){}

static void updateCell(Cell& cell, char piece, bool isJoker,int playerNumber){
		delete &cell;
		cell = new Cell(piece, isJoker,playerNumber);
}
char Cell::getPiece() const{
	return piece;
}
bool Cell::getIsJoker() const{
	return isJoker;
}
int Cell::getPlayerNumber() const{

	return playerNum;
}


