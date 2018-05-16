#include "AIBoard.h"

AIBoard::AIBoard() {
	board(ROWS, vector<AICell>(COLS)); //	board = new Cell *[ROWS];
}
AIBoard::~AIBoard() {}

int AIBoard::getPlayer(const Point& pos) const {
	int x = pos.getX();
	int y = pos.getY();
	char piece = board.at(x - 1).at(y - 1).getPiece();
	if (piece == 0) return 0;
	else if (isupper(piece)) return 1;
	else return 2;
}