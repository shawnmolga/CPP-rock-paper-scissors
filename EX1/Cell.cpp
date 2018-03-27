/*
 * Cells.cpp
 *
 *  Created on: 27 במרץ 2018
 *      Author: OR
 */
#include "Cell.h"

Cell::Cell(char playerPiece , bool isPieceJoker) : piece(playerPiece), isJoker(isPieceJoker){}
Cell::Cell() : piece(0), isJoker(false){}

static void Cell::updateCell(Cell& cell, char piece, bool isJoker){
		delete cell;
		cell = new Cell(piece, isJoker);
}
char Cell::getPiece() const{
	return piece;
}
bool Cell::getIsJoker() const{
	return isJoker;
}


