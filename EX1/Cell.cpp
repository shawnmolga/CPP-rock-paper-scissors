/*
 * Cells.cpp
 *
 *  Created on: 27 ���� 2018
 *      Author: OR
 */
#include "Cell.h"

Cell::Cell(char playerPiece, bool isPieceJoker) : piece(playerPiece), isJoker(isPieceJoker) {}
Cell::Cell() : piece(0), isJoker(false) {}

void Cell::updateCell(Cell &cell, char piece, bool isJoker)
{
	cell.piece = piece;
	cell.isJoker = isJoker;
}
void Cell::cleanCell(Cell &cell)
{
	updateCell(cell, 0, false);
}
char Cell::getPiece() const
{
	return piece;
}
bool Cell::getIsJoker() const
{
	return isJoker;
}
bool Cell::isPlayerOnePiece(Cell &cell)
{
	if(cell.getPiece() == isupper(cell.getPiece())){
		return true;
	}
	return false;
}
