#include "AIBoard.h"

AIBoard::AIBoard() {
	vector<vector<AICell>> newboard(COLS, vector<AICell>(ROWS));
	for (int i = 0; i <= COLS; i++)
	{
		vector<AICell> rowVector;
		for (int j = 0; j <= ROWS; j++)
		{
			rowVector.push_back(AICell(0, false));
		}
		newboard.push_back(rowVector);
	}
	board = newboard;

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
