
#include "RPSgame.h"

//RPSgame::RPSgame() : playerOne(Player(1)), playerTwo(Player(2)), isGameOver(false)
RPSgame::RPSgame() : playerOne(Player(1)), playerTwo(Player(2)), isGameOver(false)
{
	board = new Cell *[ROWS];
	for (int i = 1; i <= ROWS; ++i)
	{
		board[i] = new Cell[COLS];
	}
}

RPSgame::~RPSgame()
{
	for (int i = 1; i <= ROWS; ++i)
	{
		delete[] board[i];
	}
	delete[] board;
}

Player RPSgame::getPlayerOne() const
{
	return playerOne;
}

Player RPSgame::getPlayerTwo() const
{
	return playerTwo;
}

string RPSgame::ToString(GAME_OVER_TYPE typeGame, int indexErrorPosOne, int indexErrorPosTwo, int indexErrorMoveOne, int indexErrorMoveTwo)
{
	switch (typeGame)
	{
	case ALL_FLAGS_CAPTURED:
		return "All flags of the opponent are captured";
	case ALL_PIECES_EATEN:
		return "All moving PIECEs of the opponent are eaten";
	case WRONG_FILE_FORMAT_ONE:
		return "Bad Positioning input file for player 1 - line " + to_string(indexErrorPosOne); //TODO : line
	case WRONG_FILE_FORMAT_TWO:
		return "Bad Positioning input file for player 2 - line " + to_string(indexErrorPosTwo); //TODO : line
	case WRONG_FILE_FORMAT_BOTH:
		return "Bad Positioning input file for both players - player 1: line " + to_string(indexErrorPosOne) + ", player 2: line " + to_string(indexErrorPosTwo);
	case TIE_NO_WINNER:
		return "A tie - both Moves input files done without a winner";
	case TIE_ALL_FLAGS_EATEN:
		return "A tie - all flags are eaten by both players in the position files";
	case WRONG_MOVE_FILE_FORMAT_TWO:
		return "Bad Move File input file for player 2 - line " + to_string(indexErrorMoveTwo);
	case WRONG_MOVE_FILE_FORMAT_ONE:
		return "Bad Move File input file for player 1 - line " + to_string(indexErrorMoveOne);
		;
	default:
		return "[Unknown GAME_OVER_TYPE]";
	}
}

bool RPSgame::fight(bool isPlayerOneTurn, int row, int col, char currPiece, bool isCurrPieceJoker)
{
	Player *currPlayer = &playerOne;
	Player *nextPlayer = &playerTwo;

	if (!isPlayerOneTurn)
	{
		currPlayer = &playerTwo;
		nextPlayer = &playerOne;
	}

	char currPlayerPiece = toupper(currPiece);
	char nextPlayerPiece = toupper(board[row][col].getPiece());

	//Case 1: 2 players in the same type - both should be eaten
	if (nextPlayerPiece == currPlayerPiece)
	{
		switch (nextPlayerPiece)
		{
		case FLAG:
			currPlayer->numOfPieces[5]--;
			nextPlayer->numOfPieces[5]--;
			break;
		case ROCK:
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

		if (currPlayerPiece == BOMB)
		{
			//bomb exploded!
			currPlayer->numOfPieces[3]--;
			Cell::updateCell(board[row][col], 0, false);
		}
		else
		{
			Cell::updateCell(board[row][col], currPiece, isCurrPieceJoker);
		}
	}

	//case 3: current player's piece is flag and there is another piece on board
	else if (currPlayerPiece == FLAG)
	{
		currPlayer->numOfPieces[5]--;

		if (nextPlayerPiece == BOMB)
		{
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
		//bomb won and exploded so cell is empty now
		Cell::updateCell(board[row][col], 0, false);
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
			nextPlayer->numOfPieces[2]--;

			Cell::updateCell(board[row][col], currPiece, isCurrPieceJoker);
		}
	}
	return checkGameOver(false, isPlayerOneTurn);
}

void RPSgame::setGameOver(int winnerNumber, GAME_OVER_TYPE type)
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

bool RPSgame::checkGameOver(bool isBeforeMove, bool isPlayerOneTurn)
{
	Player *currPlayer = &playerOne;
	Player *nextPlayer = &playerTwo;
	if (!isPlayerOneTurn)
	{
		currPlayer = &playerTwo;
		nextPlayer = &playerOne;
	}
	if (currPlayer->numOfPieces[5] == 0 && nextPlayer->numOfPieces[5] == 0)
	{
		isGameOver = true;
		currPlayer->setIsWinner(false);
		nextPlayer->setIsWinner(false);
		gameOverReason = TIE_ALL_FLAGS_EATEN;
		return true;
	}
	//check if all of player one's flags are taken
	if (currPlayer->numOfPieces[5] == 0)
	{
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
		currPlayer->setIsWinner(true);
		nextPlayer->setIsWinner(false);
		currPlayer->setScore(currPlayer->getScore() + 1);
		isGameOver = true;
		gameOverReason = ALL_FLAGS_CAPTURED;
		return true;
	}
	//check if all of player one's moving pieces are eaten
	if (isBeforeMove && !currPlayer->isLeftMovingPieces())
	{
		nextPlayer->setIsWinner(true);
		currPlayer->setIsWinner(false);
		nextPlayer->setScore(nextPlayer->getScore() + 1);
		isGameOver = true;
		gameOverReason = ALL_PIECES_EATEN;
		return true;
	}
	return false;
}

bool RPSgame::getIsGameOver() const
{
	return isGameOver;
}
void RPSgame::resetGameResults()
{
	cout<<"Game was reseted"<<endl;
	//reset game result after fights that have done due to positioning files
	playerOne.setIsWinner(false);
	playerTwo.setIsWinner(false);
	playerOne.setScore(0);
	playerTwo.setScore(0);
	isGameOver = false;
}

GAME_OVER_TYPE RPSgame::getGameOverReason() const
{
	return gameOverReason;
}
