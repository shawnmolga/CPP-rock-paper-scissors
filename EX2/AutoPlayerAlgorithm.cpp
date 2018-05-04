/*
 * AutoPlayerAlgorithm.cpp
 *
 *  Created on: 2 במאי 2018
 *      Author: OR
 */



#include "AutoPlayerAlgorithm.h"

AutoPlayerAlgorithm::AutoPlayerAlgorithm(const string &posAuto,
		const string &moveAuto) : positionAuto(posAuto), movesAuto(moveAuto), gameBoard()
{
}

void AutoPlayerAlgorithm::getInitialPositions(int player,
		std::vector<unique_ptr<PiecePosition>> &vectorToFill)
{
	positionRocks(vectorToFill);
	positionPapers(vectorToFill);
	positionScissors(vectorToFill);
	positionBombs(vectorToFill);
	positionFlags(vectorToFill);
	positionJokers(vectorToFill);
}

void AutoPlayerAlgorithm::initFlags(int player){
	std::random_device rd; // obtain a random number from hardware
	std::mt19937 eng(rd()); // seed the generator
	std::uniform_int_distribution<> distr(0, COLS-1); // define the range
	bool isCellTaken = true;
	char piece = (player == 1 ? 'F' : 'f');
	int col = distr(eng);

	for(int i=0; i<FLAGS_NUM; ++i){
		while (isCellTaken){
			col = distr(eng) // generate numbers
	    	if (gameBoard.board[ROWS-1][col].getPiece() == 0){
	    		isCellTaken = false;
	    		gameBoard.board[ROWS-1][col] = Cell::updateCell(board[ROWS-1][col], piece, false);
	    	}

		}
		isCellTaken = true;
	}
}

//will not be used in Auto player method
//<<<<<<< HEAD
//void AutoPlayerAlgorithm::notifyOnInitialBoard(const Board& b, const std::vector<unique_ptr<FightInfo>>& fights){}
//=======
void AutoPlayerAlgorithm::notifyOnInitialBoard(const Board &b,
		const std::vector<unique_ptr<FightInfo>> &fights)
{

}
