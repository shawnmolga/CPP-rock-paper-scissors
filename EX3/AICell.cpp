#include "AICell.h"

AICell::AICell(char playerPiece, bool isPieceJoker) {
	Cell(playerPiece, isPieceJoker);
	initAIFields();
}
AICell::AICell() {
	Cell();
	initAIFields();
}

void AICell::initAIFields() {
	this->flagProbability = (double)FLAGS_NUM / (double)TOTAL_PIECES_NUM;
	this->paperProbability = (double)(PAPERS_NUM + JOKERS_NUM) / (double)TOTAL_PIECES_NUM;
	this->scissorsProbability = (double)(SCISSORS_NUM + JOKERS_NUM) / (double)TOTAL_PIECES_NUM;
	this->rockProbability = (double)(ROCKS_NUM + JOKERS_NUM) / (double)TOTAL_PIECES_NUM;
	this->jokerProbability = (double)JOKERS_NUM / (double)TOTAL_PIECES_NUM;
	this->isJokerKnown = false;
	this->isMovingPieceKnown = false;
}

void AICell::updateCellKnowlage(AICell & cell,const AICell & fromCell) {
	cell.flagProbability = fromCell.flagProbability;
	cell.isJokerKnown = fromCell.isJokerKnown;
	cell.isMovingPieceKnown = fromCell.isMovingPieceKnown;
	cell.isMovingPiece = fromCell.isMovingPiece;
}




bool AICell::isMyPiece(int myPlayerNum){
	char piece = getPiece();
	if (piece == 0 || piece == '#')
		return false;
	return myPlayerNum == 1 ? isupper(piece) : islower(piece);
}

void AICell::resetKnowlage() {
	isJokerKnown = false;
	isMovingPieceKnown = false;
	isMovingPiece = false;
}
