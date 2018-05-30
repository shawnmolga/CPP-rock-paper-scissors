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
	this->flagProbability = FLAGS_NUM / TOTAL_PIECES_NUM;
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
