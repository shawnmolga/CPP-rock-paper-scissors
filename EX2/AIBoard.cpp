#include "AIBoard.h"

AIBoard::AIBoard() {
	//board(ROWS, vector<AICell>(COLS)); //	board = new Cell *[ROWS];
	vector<vector<AICell>> board(COLS+1, vector<AICell>(ROWS+1));
	for (int i = 0; i <= COLS; i++)
	{
		vector<AICell> rowVector;
		for (int j = 0; j <= ROWS; j++)
		{
			rowVector.push_back(AICell(0, false));
		}
		board.push_back(rowVector);
	}
}
AIBoard::~AIBoard() {}

int AIBoard::getPlayer(const Point& pos) const {
	int x = pos.getX();
	int y = pos.getY();
	char piece = board.at(x).at(y).getPiece();
	if (piece == 0) return 0;
	else if (isupper(piece)) return 1;
	else return 2;
}