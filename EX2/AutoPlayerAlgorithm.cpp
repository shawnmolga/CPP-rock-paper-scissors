/*
 * AutoPlayerAlgorithm.cpp
 *
 *  Created on: 2 ���� 2018
 *      Author: OR
 */



#include "AutoPlayerAlgorithm.h"

AutoPlayerAlgorithm::AutoPlayerAlgorithm()
{
	gameBoard = RPSBoard();
}

void AutoPlayerAlgorithm::getInitialPositions(int player,
		std::vector<unique_ptr<PiecePosition>> &vectorToFill)
{
	//positionRocks(vectorToFill);
	//positionPapers(vectorToFill);
	//positionScissors(vectorToFill);
	positionBombs(player, vectorToFill);
	positionFlags(player, vectorToFill);
	//positionJokers(vectorToFill);
}

void AutoPlayerAlgorithm::positionFlags(int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill) {
	std::random_device rd; // obtain a random number from hardware
	std::mt19937 eng(rd()); // seed the generator
	std::uniform_int_distribution<> colDistr(0, COLS-1); // define the range
	std::uniform_int_distribution<> rowDistr(0, ROWS-1); // define the range
	bool isCellTaken = true;
	bool hasOpponentNeighbor = true;
	char piece = (player == 1 ? 'F' : 'f');
	int col = colDistr(eng);
	int row = rowDistr(eng);

	for(int i=0; i<FLAGS_NUM; ++i){
		while (isCellTaken || hasOpponentNeighbor){
	    	if (gameBoard.board[row][col].getPiece() == 0){
	    		isCellTaken = false;
					if (!checkIsOpponentNeighbors){
						hasOpponentNeighbor = false;
						Cell::updateCell(gameBoard.board[row][col], piece, false);
						vectorToFill.push_back(make_unique<RPSPiecePosition>(RPSpoint(row, col), piece, '#'));
					}
	    			
	    	}
		}
		row = rowDistr(eng);
		col = colDistr(eng);
		isCellTaken = true;
		hasOpponentNeighbor = true;
	}
}

int getRandomNumInRange(int start, int end){
	std::random_device rd; // obtain a random number from hardware
	std::mt19937 eng(rd()); // seed the generator
	std::uniform_int_distribution<> colDistr(start, end); // define the range
	return colDistr(eng);
}


void AutoPlayerAlgorithm::positionBombs(int flagRow, int flagCol, int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill) {
	bool isCellTaken = true;
	bool hasOpponentNeighbor = true;
	char piece = (player == 1 ? 'F' : 'f');
	int col = getRandomNumInRange(0,COLS-1);
	int row = getRandomNumInRange(0,ROWS-1);

	for(int i=0; i<FLAGS_NUM; ++i){
		while (isCellTaken || hasOpponentNeighbor){
	    	if (gameBoard.board[row][col].getPiece() == 0){
	    		isCellTaken = false;
					if (!checkIsOpponentNeighbors){
						hasOpponentNeighbor = false;
						Cell::updateCell(gameBoard.board[row][col], piece, false);
						vectorToFill.push_back(make_unique<RPSPiecePosition>(RPSpoint(row, col), piece, '#'));
					}
	    			
	    	}
		} 
		col = getRandomNumInRange(0,COLS-1);
		row = getRandomNumInRange(0,ROWS-1);
		isCellTaken = true;
		hasOpponentNeighbor = true;
	}
}

bool isOpponentPiece(int myPlayerNum, char piece){
	if (myPlayerNum == 1){
		return islower(piece);
	}
	else{
		return isupper(piece);
	}
}

/*
Input  - relevant col and row
Output - true if this is a legal position and  false otherwise 
*/
// legal position on board starts from 0 to number of rows/cols - 1
bool AutoPlayerAlgorithm::isLegalPosition(int row, int col){
	if (row > ROWS-1 || col > COLS-1 || row < 0 || col < 0){
		return false;
	}
	return true;
}
/*
Input - current player num, row and col 
Output - true if an opponent nieghbour is located there and otherwise false 
*/
bool AutoPlayerAlgorithm::checkIsOpponentNeighbors(int player, int row, int col){
	//down neigbor
	char piece = gameBoard.board[row+1][col].getPiece();
	if (isLegalPosition(row+1, col) && isOpponentPiece(player, piece)){
			return true;
	}
	//right
	piece = gameBoard.board[row][col+1].getPiece();
	if (isLegalPosition(row, col+1) && isOpponentPiece(player, piece)){
			return true;
	}
	//up neighbor
	piece = gameBoard.board[row-1][col].getPiece();
	if (isLegalPosition(row-1, col) && isOpponentPiece(player, piece)){
			return true;
	}
	//left
	piece = gameBoard.board[row][col-1].getPiece();
	if (isLegalPosition(row, col-1) && isOpponentPiece(player, piece)){
			return true;
	}
	return false;
}

//will not be used in Auto player method
//<<<<<<< HEAD
//void AutoPlayerAlgorithm::notifyOnInitialBoard(const Board& b, const std::vector<unique_ptr<FightInfo>>& fights){}
//=======
void AutoPlayerAlgorithm::notifyOnInitialBoard(const Board &b,
		const std::vector<unique_ptr<FightInfo>> &fights)
{

}
