/*
 * RCPgame.cpp
 *
 *  Created on: 27 ���� 2018
 *      Author: OR
 */

#include "RCPgame.h"

RCPgame::RCPgame() : isGameOver(false), playerOne(Player(1)), playerTwo(Player(2))
{
	board = new Cell *[ROWS];
	for (int i = 0; i < ROWS; ++i)
	{
		board[i] = new Cell[COLS];
	}
}

RCPgame::~RCPgame()
{
	for (int i = 0; i < ROWS; ++i)
	{
		delete[] board[i];
	}
	delete[] board;
	delete &playerOne;
	delete &playerTwo;
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
	default:
		return "[Unknown GAME_OVER_TYPE]";
	}
}
bool RCPgame::fight(int row, int col, char currPiece, bool isCurrPieceJoker)
{
	Player *currPlayer = &playerOne;
	Player *nextPlayer = &playerTwo;

	if (!playerOne.getIsPlayerTurn())
	{
		currPlayer = &playerTwo;
		nextPlayer = &playerOne;
	}

	char currPlayerPiece = toupper(currPiece);
	char nextPlayerPiece = toupper(board[row][col].getPiece());

	//Case 1: 2 players in the same type - both should be eaten
	if (nextPlayerPiece == currPlayerPiece)
	{
		//TODO: dont know if we should update jokers number - this is not relevat anymore when we started to play the game

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

		Cell::updateCell(board[row][col], 0, false);
	}

	//Case 2: there is flag and current player has another piece
	else if (nextPlayerPiece == FLAG)
	{
		nextPlayer->numOfPieces[5]--;

		if (currPlayerPiece == BOMB){
			//bomb exploded!
			currPlayer->numOfPieces[3]--;
			Cell::updateCell(board[row][col], 0, false);
		}
		else{
			Cell::updateCell(board[row][col], currPiece, isCurrPieceJoker);
		}
	}

	//case 3: current player's piece is flag and there is another piece on board
	else if (currPlayerPiece == FLAG)
	{
		currPlayer->numOfPieces[5]--;

		if (nextPlayerPiece == BOMB){
			//bomb exploded!
			nextPlayer->numOfPieces[3]--;
			Cell::updateCell(board[row][col], 0, false);
		}
	}
	//case 4: there is bomb and current player has another piece
	else if (nextPlayerPiece == BOMB)
	{
		//bomb exploded!
		nextPlayer->numOfPieces[3]--;
		//piece is eaten
		switch (currPlayerPiece)
		{
		case ROCK:
			currPlayer->numOfPieces[0]--;
			break;
		case PAPER:
			currPlayer->numOfPieces[1]--;
			break;
		case SCISSOR:
			currPlayer->numOfPieces[2]--;
			break;
		}
	}

	//case 5: current player piece is bomb and on board there is another piece
	else if (currPlayerPiece == BOMB)
	{
		//bomb exploded!
		currPlayer->numOfPieces[3]--;

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
		//bomb won and exploded so cell is empty now
		Cell::updateCell(board[row][col], 0, false);
	}

	//case 6: there is PAPER and current player has another piece
	else if (nextPlayerPiece == PAPER)
	{
		if (currPlayerPiece == ROCK)
		{
			currPlayer->numOfPieces[0]--;

		}
		else if (currPlayerPiece == SCISSOR)
		{
			nextPlayer->numOfPieces[1]--;
			Cell::updateCell(board[row][col], currPiece, isCurrPieceJoker);
		}
	}
	//case 7: player 1 is ROCK and player 2 another piece
	else if (nextPlayerPiece == ROCK)
	{
		if (currPlayerPiece == PAPER)
		{
			nextPlayer->numOfPieces[0]--;

			Cell::updateCell(board[row][col], currPiece, isCurrPieceJoker);
		}
		else if (currPlayerPiece == SCISSOR)
		{
			currPlayer->numOfPieces[2]--;

		}
	}

	else if (nextPlayerPiece == SCISSOR)
	{
		if (currPlayerPiece == PAPER)
		{
			currPlayer->numOfPieces[1]--;
		}
		else if (currPlayerPiece == ROCK)
		{
			nextPlayer->numOfPieces[0]--;

			Cell::updateCell(board[row][col], currPiece, isCurrPieceJoker);
		}
	}

	return checkGameOver();
}

void RCPgame::setGameOver(int winnerNumber, GAME_OVER_TYPE type)
{
	isGameOver = true;
	gameOverReason = type;
	if (winnerNumber == 0)
	{
		playerTwo.setIsWinner(false);
		playerOne.setIsWinner(false);
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
//TODO: there will be a problem in scoring if game is potentially over several times when game starts - because of positioning. so need to be done outside
bool RCPgame::checkGameOver()
{
	if (playerOne.numOfPieces[5] == 0 && playerTwo.numOfPieces[5] == 0)
	{
		isGameOver = true;
		gameOverReason = TIE_ALL_FLAGS_EATEN;
		return true;
	}
	//check if all of player one's flags are taken
	if (playerOne.numOfPieces[5] == 0)
	{
		playerTwo.setIsWinner(true);
		playerTwo.setScore(playerTwo.getScore() + 1);
		isGameOver = true;
		gameOverReason = ALL_FLAGS_CAPTURED;
	}
	//check if all of player one's moving pieces are eaten
	if (!playerOne.isLeftMovingPieces())
	{
		playerTwo.setIsWinner(true);
		playerTwo.setScore(playerTwo.getScore() + 1);
		isGameOver = true;
		gameOverReason = ALL_PIECES_EATEN;
	}
	//check if all of player two's flags are taken
	if (playerTwo.numOfPieces[5] == 0)
	{
		playerOne.setIsWinner(true);
		playerOne.setScore(playerOne.getScore() + 1);
		isGameOver = true;
		gameOverReason = ALL_FLAGS_CAPTURED;
	}

	//check if all of player two's moving pieces are eaten
	if (!playerTwo.isLeftMovingPieces())
	{
		playerOne.setIsWinner(true);
		playerOne.setScore(playerOne.getScore() + 1);
		isGameOver = true;
		gameOverReason = ALL_PIECES_EATEN;
	}

	return false;
}

Player RCPgame::getPlayerOne() const
{
	return playerOne;
}

Player RCPgame::getPlayerTwo() const
{
	return playerTwo;
}

bool RCPgame::getIsGameOver() const
{
	return isGameOver;
}

GAME_OVER_TYPE RCPgame::getGameOverReason() const
{
	return gameOverReason;
}
