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
	//TODO: ask if we can position flag on flag and who is loosing
	Player *currPlayer = &playerOne;
	Player *nextPlayer = &playerTwo;

	if (!playerOne.getIsPlayerTurn())
	{
		currPlayer = &playerTwo;
		nextPlayer = &playerOne;
	}

	char currPlayerPiece = toupper(currPiece);
	char nextPlayerPiece = toupper(board[row][col].getPiece());
	//Case 1: 2 players in the same type.
	if (nextPlayerPiece == currPlayerPiece)
	{
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
		if (board[row][col].getIsJoker())
		{
			playerTwo.numOfPieces[4]--;
		}
		playerTwo.numOfPieces[5]--;
	}

	//case 4: player 1 is bomb and player 2 another piece
	else if (nextPlayerPiece == BOMB)
	{
		if (isCurrPieceJoker)
		{
			playerTwo.numOfPieces[4]--;
		}
		else
		{
			switch (currPlayerPiece)
			{
			case ROCK:
				playerTwo.numOfPieces[0]--;
				break;
			case PAPER:
				playerTwo.numOfPieces[1]--;
				break;
			case SCISSOR:
				playerTwo.numOfPieces[2]--;
				break;
			}
		}
	}

	//case 5: player 2 is bomb and player 1 another piece
	else if (currPlayerPiece == BOMB)
	{
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
		if (currPlayerPiece == ROCK)
		{
			if (isCurrPieceJoker)
			{
				playerTwo.numOfPieces[4]--;
			}
			else
			{
				playerTwo.numOfPieces[0]--;
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
	//case 7: player 1 is ROCK and player 2 another piece
	else if (nextPlayerPiece == ROCK)
	{
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
				playerTwo.numOfPieces[4]--;
			}
			else
			{
				playerTwo.numOfPieces[2]--;
			}
		}
	}

	else if (nextPlayerPiece == SCISSOR)
	{
		if (currPlayerPiece == PAPER)
		{
			playerTwo.numOfPieces[1]--;
		}
		else if (currPlayerPiece == ROCK)
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
	}
	return checkGameOver();
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
