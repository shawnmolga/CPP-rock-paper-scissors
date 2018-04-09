/*
 * RCPgame.cpp
 *
 *  Created on: 27 ���� 2018
 *      Author: OR
 */

#include "RCPgame.h"

RCPgame::RCPgame() :playerOne(Player(1)), playerTwo(Player(2)), isGameOver(false)
{
	board = new Cell *[ROWS];
	for (int i = 1; i <= ROWS; ++i)
	{
		board[i] = new Cell[COLS];
	}
}

RCPgame::~RCPgame()
{
	cout << "in delete game" << endl;
	for (int i = 1; i <= ROWS; ++i)
	{
		delete[] board[i];
	}
	delete[] board;

	//shawn removed - playerOne playerTwo were not created with "new" therefore on stack
	//delete &playerOne;
	//delete &playerTwo;
}

Player RCPgame::getPlayerOne() const{
	return playerOne;
}

Player RCPgame::getPlayerTwo() const{
	return playerTwo;
}

string RCPgame::ToString(GAME_OVER_TYPE typeGame)
{
	switch (typeGame)
	{
	case ALL_FLAGS_CAPTURED:
		return "All flags of the opponent are captured";
	case ALL_PIECES_EATEN:
		return "All moving PIECEs of the opponent are eaten";
	case WRONG_FILE_FORMAT_ONE:
		return "Bad Positioning input file for player 1 - line "; //TODO : line
	case WRONG_FILE_FORMAT_TWO:
		return "Bad Positioning input file for player 2 - line "; //TODO : line
	case WRONG_FILE_FORMAT_BOTH:
		return "Bad Positioning input file for both players - player 1: line <X>, player 2: line <Y>";
	case TIE_NO_WINNER:
		return "A tie - both Moves input files done without a winner";
	case TIE_ALL_FLAGS_EATEN:
		return "A tie - all flags are eaten by both players in the position files";
	case WRONG_MOVE_FILE_FORMAT_TWO://added by shawn TODO
		return "Bad Move File input file for player 2 - line ";
	case WRONG_MOVE_FILE_FORMAT_ONE://added by shawn TODO
		return "Bad Move File input file for player 1 - line ";
	default:
		return "[Unknown GAME_OVER_TYPE]";
	}
}



bool RCPgame::fight(bool isPlayerOneTurn,int row, int col, char currPiece, bool isCurrPieceJoker)
{

	//TODO: ask if we can position flag on flag and who is loosing
	Player *currPlayer = &playerOne;
	Player *nextPlayer = &playerTwo;
	if (!isPlayerOneTurn)
	{
		currPlayer = &playerTwo;
		nextPlayer = &playerOne;
	}

	char currPlayerPiece = toupper(currPiece);
	char nextPlayerPiece = toupper(board[row][col].getPiece());

	//Case 1: 2 players in the same type.
	if (nextPlayerPiece == currPlayerPiece)
	{
		cout<<"case1"<<endl;
		if (board[row][col].getIsJoker())
		{
			currPlayer->numOfPieces[4]--;
			nextPlayer->numOfPieces[4]--;
		}
		else
		{
			switch (nextPlayerPiece)
			{
			case FLAG:
				currPlayer->numOfPieces[5]--;
				nextPlayer->numOfPieces[5]--;
			case ROCK:
				break;
				currPlayer->numOfPieces[0]--;
				nextPlayer->numOfPieces[0]--;
				break;
			case PAPER:
				currPlayer->numOfPieces[1]--;
				nextPlayer->numOfPieces[1]--;
				break;
			case SCISSOR:
				currPlayer->numOfPieces[2]--;
				nextPlayer->numOfPieces[2]--;
				break;
			case BOMB:
				currPlayer->numOfPieces[3]--;
				nextPlayer->numOfPieces[3]--;
				break;
			}
		}
		Cell::updateCell(board[row][col], 0, false);
	}

	//Case 2: player 1 is flag and player 2 another piece
	else if (nextPlayerPiece == FLAG)
	{
		cout<<"case2"<<endl;
		if (board[row][col].getIsJoker())
		{
			nextPlayer->numOfPieces[4]--;
		}
		else
		{
			nextPlayer->numOfPieces[5]--;
		}
		Cell::updateCell(board[row][col], currPiece, isCurrPieceJoker);
	}

	//case 3: player 2 is flag and player 1 another piece
	else if (currPlayerPiece == FLAG)
	{
		cout<<"case3"<<endl;
		if (board[row][col].getIsJoker())
		{
			currPlayer->numOfPieces[4]--;
		}
		else{
			currPlayer->numOfPieces[5]--;
		}
	}

	//case 4: nextPlayer is bomb and player 2 another piece
	else if (nextPlayerPiece == BOMB)
	{
		cout<<"case4"<<endl;
		if (isCurrPieceJoker)
		{
			currPlayer->numOfPieces[4]--;
		}
		else
		{
			switch (currPlayerPiece)
			{
			case ROCK:
				currPlayer->numOfPieces[0] = currPlayer->numOfPieces[0] -1 ;
				break;
			case PAPER:
				currPlayer->numOfPieces[1] = currPlayer->numOfPieces[1] -1;
				break;
			case SCISSOR:
				currPlayer->numOfPieces[2] =currPlayer->numOfPieces[2] -1;
				break;
			}
		}
		
	}

	//case 5: player 2 is bomb and player 1 another piece
	else if (currPlayerPiece == BOMB)
	{
		cout<<"case5"<<endl;
		if (isCurrPieceJoker)
		{
			nextPlayer->numOfPieces[4]--;
		}
		else
		{
			switch (nextPlayerPiece)
			{
			case ROCK:
				nextPlayer->numOfPieces[0]--;
				break;
			case PAPER:
				nextPlayer->numOfPieces[1]--;
				break;
			case SCISSOR:
				nextPlayer->numOfPieces[2]--;
				break;
			}
		}
		Cell::updateCell(board[row][col], currPiece, isCurrPieceJoker);
	}

	//case 6: player 1 is PAPER and player 2 another piece
	else if (nextPlayerPiece == PAPER)
	{
		cout<<"case6"<<endl;
		if (currPlayerPiece == ROCK)
		{
			if (isCurrPieceJoker)
			{
				currPlayer->numOfPieces[4]--;
			}
			else
			{
				currPlayer->numOfPieces[0]--;
			}
		}
		else if (currPlayerPiece == SCISSOR)
		{
			if (board[row][col].getIsJoker())
			{
				nextPlayer->numOfPieces[4]--;
			}
			else
			{
				nextPlayer->numOfPieces[1]--;
			}
			Cell::updateCell(board[row][col], currPiece, isCurrPieceJoker);
		}
	}
	//case 7: nextPlayer is ROCK and currPlayer another piece
	else if (nextPlayerPiece == ROCK)
	{
		cout<<"case7"<<endl;
		if (currPlayerPiece == PAPER)
		{
			if (board[row][col].getIsJoker())
			{
				nextPlayer->numOfPieces[4]--;
			}
			else
			{
				nextPlayer->numOfPieces[0]--;
			}
			Cell::updateCell(board[row][col], currPiece, isCurrPieceJoker);
		}
		else if (currPlayerPiece == SCISSOR)
		{
			if (isCurrPieceJoker)
			{
				currPlayer->numOfPieces[4]--;
			}
			else
			{
				currPlayer->numOfPieces[2]--;
			}
		}
	}
	//case 8: player 1 is ROCK and player 2 another piece
	else if (nextPlayerPiece == SCISSOR)
	{
		cout<<"case8"<<endl;
		if (currPlayerPiece == PAPER)
		{
			currPlayer->numOfPieces[1]--;
		}
		else if (currPlayerPiece == ROCK)
		{
			if (board[row][col].getIsJoker())
			{
				nextPlayer->numOfPieces[4]--;
			}
			else
			{
				nextPlayer->numOfPieces[2]--;
			}
			Cell::updateCell(board[row][col], currPiece, isCurrPieceJoker);
		}
	}
	return checkGameOver(isPlayerOneTurn);
}

void RCPgame::setGameOver(int winnerNumber, GAME_OVER_TYPE type)
{
	isGameOver = true;
	gameOverReason = type;
	if (winnerNumber == 0)
	{
		return;
	}

	else if (winnerNumber == playerOne.getPlayerNum())
	{
		playerOne.setIsWinner(true);
		playerOne.setScore(playerOne.getScore() + 1);
		return;
	}
	else
	{
		playerTwo.setIsWinner(true);
		playerTwo.setScore(playerTwo.getScore() + 1);
	}

	return;
}

bool RCPgame::checkGameOver(bool isPlayerOneTurn)
{
	Player *currPlayer = &playerOne;
	Player *nextPlayer = &playerTwo;
	if(!isPlayerOneTurn){
		currPlayer=&playerTwo;
		nextPlayer=&playerOne;
	}
	if (currPlayer->numOfPieces[5] == 0 && nextPlayer->numOfPieces[5] == 0)
	{
		cout<<"1?"<<endl;
		isGameOver = true;
		currPlayer->setIsWinner(false);
		nextPlayer->setIsWinner(false);
		gameOverReason = TIE_ALL_FLAGS_EATEN;
		return true;
	}
	//check if all of player one's flags are taken
	if (currPlayer->numOfPieces[5] == 0)
	{
		cout<<"2?"<<endl;
		nextPlayer->setIsWinner(true);
		currPlayer->setIsWinner(false);
		nextPlayer->setScore(nextPlayer->getScore() + 1);
		isGameOver = true;
		gameOverReason = ALL_FLAGS_CAPTURED;
		return true;
	}
	//check if all of player two's flags are taken
	if (nextPlayer->numOfPieces[5] == 0)
	{
		cout<<"4?"<<endl;
		currPlayer->setIsWinner(true);
		nextPlayer->setIsWinner(false);
		currPlayer->setScore(currPlayer->getScore() + 1);
		isGameOver = true;
		gameOverReason = ALL_FLAGS_CAPTURED;
		return true;
	}
	//check if all of player one's moving pieces are eaten
	if (!currPlayer->isLeftMovingPieces())
	{
		cout<<"3?"<<endl;
		nextPlayer->setIsWinner(true);
		currPlayer->setIsWinner(false);
		nextPlayer->setScore(nextPlayer->getScore() + 1);
		isGameOver = true;
		gameOverReason = ALL_PIECES_EATEN;
		return true;
	}

	//check if all of player two's moving pieces are eaten
	if (!nextPlayer->isLeftMovingPieces())
	{
		cout<<"5?"<<endl;
		currPlayer->setIsWinner(true);
		nextPlayer->setIsWinner(false);
		currPlayer->setScore(currPlayer->getScore() + 1);
		isGameOver = true;
		gameOverReason = ALL_PIECES_EATEN;
		return true;
	}
	return false;
}

bool RCPgame::getIsGameOver() const
{
	return isGameOver;
}

GAME_OVER_TYPE RCPgame::getGameOverReason() const
{
	return gameOverReason;
}
