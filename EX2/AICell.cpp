#include "AICell.h"

AICell::AICell(char playerPiece, bool isPieceJoker) {
	Cell(playerPiece, isPieceJoker);
	init();
}
AICell::AICell() {
	Cell();
	init();
}

AICell::initAIFields() {
	this->flagProbability = FLAGS_NUM / TOTAL_PIECES_NUM;
	this->isJokerKnown = false;
	this->isMovingPieceKnown = false;
}

void AICell::updateCellKnowlage(AICell cell,AICell fromCell) {
	cell.flagProbability = fromCell.flagProbability;
	cell.isJokerKnown = fromCell.isJokerKnown;
	cell.isMovingPieceKnown = fromCell.isMovingPieceKnown;
	cell.isMovingPiece = fromCell.isMovingPiece;
}
