#include "RPSGame.h"
RPSGame::RPSGame() : indexErrorPosOne(0), indexErrorPosTwo(0), indexErrorMoveOne(0), indexErrorMoveTwo(0),isGameOver(false),playerOne(Player(1)), playerTwo(Player(2))
{
	gameBoard = RPSBoard();
	playerAlgoOne = NULL;
	playerAlgoTwo = NULL;
}
RPSGame::~RPSGame()
{
	delete (playerAlgoOne);
	delete (playerAlgoTwo);
}

/*
This function changes joker representation
Input - previous jokerRep, new jokerRep and inidication for current player
*/
void RPSGame::updateJokerChange(char prevJokerRep, char newRep,
								bool isPlayerOne)
{
	//remove one piece from number of prev piece
	switch (prevJokerRep)
	{
	case ROCK:
		isPlayerOne ? playerOne.setNumOfPieces(0,
											   playerOne.numOfPieces[0] - 1)
					: playerTwo.setNumOfPieces(0,
											   playerTwo.numOfPieces[0] - 1);
		break;
	case PAPER:
		isPlayerOne ? playerOne.setNumOfPieces(1,
											   playerOne.numOfPieces[1] - 1)
					: playerTwo.setNumOfPieces(1,
											   playerTwo.numOfPieces[1] - 1);
		break;
	case SCISSOR:
		isPlayerOne ? playerOne.setNumOfPieces(2,
											   playerOne.numOfPieces[2] - 1)
					: playerTwo.setNumOfPieces(2,
											   playerTwo.numOfPieces[2] - 1);
		break;
	case BOMB:
		isPlayerOne ? playerOne.setNumOfPieces(3,
											   playerOne.numOfPieces[3] - 1)
					: playerTwo.setNumOfPieces(3,
											   playerTwo.numOfPieces[3] - 1);
		break;
		break;
	}
	//add new piece to number of curr joker piece
	switch (newRep)
	{
	case ROCK:
		isPlayerOne ? playerOne.setNumOfPieces(0,
											   playerOne.numOfPieces[0] + 1)
					: playerTwo.setNumOfPieces(0,
											   playerTwo.numOfPieces[0] + 1);
		;
		break;
	case PAPER:
		isPlayerOne ? playerOne.setNumOfPieces(1,
											   playerOne.numOfPieces[1] + 1)
					: playerTwo.setNumOfPieces(1,
											   playerTwo.numOfPieces[1] + 1);
		break;
	case SCISSOR:
		isPlayerOne ? playerOne.setNumOfPieces(2,
											   playerOne.numOfPieces[2] + 1)
					: playerTwo.setNumOfPieces(2,
											   playerTwo.numOfPieces[2] + 1);
		break;
	case BOMB:
		isPlayerOne ? playerOne.setNumOfPieces(3,
											   playerOne.numOfPieces[3] + 1)
					: playerTwo.setNumOfPieces(3,
											   playerTwo.numOfPieces[3] + 1);
		break;
		break;
	}
}

/*
Input- move object, joker change object and players turn
Output- true if the move was done, otherwise false
*/
//returns true if gameOver, false otherwise
bool RPSGame::movePiece(unique_ptr<Move> &move, unique_ptr<JokerChange> &playerJokerChange,
						bool isPlayerOneTurn)
{
	int from_x = move->getFrom().getX();
	int from_y = move->getFrom().getY();
	int to_x = move->getTo().getX();
	int to_y = move->getTo().getY();
	if (isLegalMove(move, isPlayerOneTurn))
		return true;
	//do move
	if (gameBoard.board[to_x][to_y].getPiece() == 0)
	{
		Cell::updateCell(gameBoard.board[to_x][to_y], gameBoard.board[from_x][from_y].getPiece(),
						 gameBoard.board[from_x][from_y].getIsJoker());
		Cell::cleanCell(gameBoard.board[from_x][from_y]);
	}
	else //fight!
	{
		RPSFight fights = RPSFight();
		std::vector<unique_ptr<FightInfo>> initFights;
		//create position fromvectors
		RPSpoint Pos1(from_x, from_y);
		RPSpoint Pos2(to_x, to_y);
		if (!isPlayerOneTurn)
		{
			RPSpoint Pos1(to_x, to_y);
			RPSpoint Pos2(from_x, from_y);
		}
		isGameOver = fight(isPlayerOneTurn, to_x, to_y,
						   gameBoard.board[from_x][from_y].getPiece(),
						   gameBoard.board[from_x][from_y].getIsJoker(), fights, initFights, Pos1, Pos2); //need to add paramerrerd to fight!!
		Cell::cleanCell(gameBoard.board[from_x][from_y]);
		/*
		For the player who just moved:
		notifyFightResult(const FightInfo& fightInfo); // only if there was a fight
		getJokerChange();--->shawn do we have to add this??	
		*/
		isPlayerOneTurn ? playerAlgoOne->notifyFightResult(fights) : playerAlgoTwo->notifyFightResult(fights);
		/*
		For the other player:
		void notifyOnOpponentMove(const Move& move);
		void notifyFightResult(const FightInfo& fightInfo); // only if there was a fight
		=> go back to [A TURN] for this player
		*/

		isPlayerOneTurn ? playerAlgoTwo->notifyOnOpponentMove(*move) : playerAlgoOne->notifyOnOpponentMove(*move);
		isPlayerOneTurn ? playerAlgoOne->notifyFightResult(fights) : playerAlgoTwo->notifyFightResult(fights);
	}

	//no joker change
	if (playerJokerChange != NULL)
	{
		//RPSpoint  jokerPoint = playerJokerChange->getJokerChangePosition();
		int x_joker = playerJokerChange->getJokerChangePosition().getX();
		int y_joker = playerJokerChange->getJokerChangePosition().getY();
		char new_rep = playerJokerChange->getJokerNewRep();
		if (new_rep == 'E')
		{
			if (isPlayerOneTurn)
				return true;
		}
		else if (!gameBoard.board[x_joker][y_joker].getIsJoker())
		{ //if the original peice is not a joker
			cout << "Error: Piece specified is not joker" << endl;
			return true;
		}
		if (new_rep != ROCK && new_rep != PAPER && new_rep != SCISSOR && new_rep != BOMB)
		{
			cout << "Error: illegal NEW_REP " << endl;
			return true;
		}
		updateJokerChange(toupper(gameBoard.board[x_joker][y_joker].getPiece()), toupper(new_rep), isPlayerOneTurn);
		Cell::updateCell(gameBoard.board[x_joker][y_joker],
						 isPlayerOneTurn ? new_rep : tolower(new_rep), true);
	}
	return false;
}

int RPSGame::makeMove()
{
	unique_ptr<Move> move1 = std::move(playerAlgoOne->getMove());
	unique_ptr<Move> move2 = std::move(playerAlgoTwo->getMove());
	int xPiecePlayerOne = move1->getFrom().getX();
	int xPiecePlayerTwo = move2->getFrom().getX();

	unique_ptr<JokerChange> playerOneJokerChange;
	unique_ptr<JokerChange> playerTwoJokerChange;
	bool isPlayerOneTurn = true;
	bool isGameOverInternal = false;
	while (xPiecePlayerOne != -2 && xPiecePlayerOne != -3)
	{
		unique_ptr<JokerChange> playerOneJokerChange =
			playerAlgoOne->getJokerChange();
		unique_ptr<JokerChange> playerTwoJokerChange =
			playerAlgoTwo->getJokerChange();
		isGameOverInternal = false;
		if (xPiecePlayerOne != 0)
		{
			isPlayerOneTurn = true;
			isGameOverInternal = movePiece(move1, playerOneJokerChange,
										  isPlayerOneTurn);
			if (isGameOverInternal)
			{
				cout << "isGameOVerInternal = true" << endl;
				setGameOver(2, WRONG_MOVE_FILE_FORMAT_ONE);
				//lines??
				break;
			}
		}
		if (xPiecePlayerTwo != -2 && xPiecePlayerTwo != -3)
		{
			if (xPiecePlayerTwo != 0)
			{
				isPlayerOneTurn = false;
				isGameOverInternal = movePiece(move2, playerTwoJokerChange,
											  isPlayerOneTurn);
				if (isGameOverInternal)
				{
					setGameOver(1, WRONG_MOVE_FILE_FORMAT_ONE);
					//lines??
					break;
				}
			}
		}
		else if (xPiecePlayerTwo == -3)
		{
			cout << "Error while reading move file. Exiting game" << endl;
			//we need to remeber to close the stream!!!
			return -1;
		}
		else
		{
			break;
		}

		move1 = playerAlgoOne->getMove();
		move2 = playerAlgoTwo->getMove();
		xPiecePlayerOne = move1->getFrom().getX();
		xPiecePlayerTwo = move2->getFrom().getX();
	} //while
	//game stopeed
	if (!isGameOver)
	{
		if (xPiecePlayerOne == -2) //EOFcase
		{
			move2 = playerAlgoTwo->getMove();
			xPiecePlayerTwo = move2->getFrom().getX();
			while (xPiecePlayerTwo != -2 && xPiecePlayerTwo != -3)
			{
				if (xPiecePlayerTwo != 0)
				{
					isPlayerOneTurn = true;
					isGameOver = movePiece(move1, playerTwoJokerChange, isPlayerOneTurn);
					if (isGameOver)
					{
						//lines??
						break;
					}
				}
				move2 = playerAlgoTwo->getMove();
				xPiecePlayerTwo = move2->getFrom().getX();
			}
			if (xPiecePlayerTwo != -2)
			{
				cout << "Error while reading move file. Exiting game" << endl;
				//we need to remeber to close the stream!!!
				return -1;
			}
		}
		else if (xPiecePlayerTwo == -2) //player 2 eof
		{
			move1 = playerAlgoOne->getMove();
			xPiecePlayerOne = move1->getFrom().getX();
			while (xPiecePlayerOne != -2 && xPiecePlayerOne != -3)
			{
				if (xPiecePlayerOne != 0)
				{
					isPlayerOneTurn = true;
					isGameOver = movePiece(move1, playerTwoJokerChange, isPlayerOneTurn);
					if (isGameOver)
					{
						//lines??
						break;
					}
				}
				move1 = playerAlgoOne->getMove();
				xPiecePlayerOne = move1->getFrom().getX();
			}
			if (xPiecePlayerOne != -2)
			{
				cout << "Error while reading move file. Exiting game" << endl;
				//we need to remeber to close the stream!!!
				return -1;
			}
		}

		//TODO : close the stream
		isPlayerOneTurn=!isPlayerOneTurn; 
		bool doesGameOver = checkGameOver(true, isPlayerOneTurn);
		if (!isGameOverInternal && !isGameOver && !doesGameOver)
		{
			cout << "enter isGameOverInternal move" << endl;
			if (isPlayerOneTurn)
			{
				if (!playerTwo.isLeftMovingPieces())
				{
					setGameOver(1, ALL_PIECES_EATEN);
					return 0;
				}
			}
			else
			{
				if (!playerOne.isLeftMovingPieces())
				{
					setGameOver(2, ALL_PIECES_EATEN);
					return 0;
				}
			}
			setGameOver(0, TIE_NO_WINNER);
		}
	}
	return 0;
}

/*
 Return -1 if there is bad format error or -2 if there is getline error
 and 0 if everything is ok
 */

int RPSGame::startGame()
{
	// std::vector<unique_ptr<PiecePosition>> vectorToFill1;
	// std::vector<unique_ptr<PiecePosition>> vectorToFill2;
	//meand bad format err

	int initResult = checkBadFormat();
	if (initResult == -1)
	{
		return -1;
	}
	else if (initResult == -2)
	{
		cout << "Error: could not read line in file. exiting..." << endl;
		return -2;
	}
	// printing count numPieces
	cout << "Player 1 numbers:" << endl;
	for (int j = 0; j < 6; j++)
	{
		cout << playerOne.numOfPieces[j] << " " << endl;
	}
	cout << "Player 2 numbers:" << endl;
	for (int j = 0; j < 6; j++)
	{
		cout << playerTwo.numOfPieces[j] << " " << endl;
	}
	bool isPlayerOneTurn = true;
	bool isAboutToMove = true;
	if (checkGameOver(isAboutToMove, isPlayerOneTurn))
	{
		cout << getGameOverReason() << endl;
		return 0;
	}
	PrintBoardToConsole();
	//If we got here the board is initialized! now we need to make a move.
	makeMove();
	return 0;
}

bool RPSGame::isLegalMove(unique_ptr<Move> &move, bool isPlayer1)
{
	int from_x = move->getFrom().getX();
	int from_y = move->getFrom().getY();
	int to_x = move->getTo().getX();
	int to_y = move->getTo().getY();
	cout << "from_x = " << from_x << " from_y = " << from_y << " to_x = " << to_x << " to_y = " << to_y << endl;
	if ((from_x < 1 || from_x > ROWS) || (to_x < 1 || to_x > ROWS) || (from_y < 1 || from_y > COLS) || (to_y < 1 || to_y > COLS))
	{
		cout << "Error: illegal location on board" << endl;
		return false;
	}

	if (from_x == to_x && from_y == to_y)
	{
		cout << "Error: user MUST move one piece" << endl;
		return false;
	}
	//noy CHANGED!!!! need to consult with shawn
	if (gameBoard.board.at(from_x - 1).at(from_y - 1).getPiece() == 0)
	{
		cout << "Error: there is no piece in this position" << endl;
		return false;
	}
	else if ((isPlayer1 && islower(gameBoard.board.at(from_x - 1).at(from_y - 1).getPiece())) || (!isPlayer1 && isupper(gameBoard.board[from_x - 1][from_y - 1].getPiece())))
	{
		cout << "Error: trying to move the opponent piece" << endl;
		return false;
	}

	if (toupper(gameBoard.board.at(from_x - 1).at(from_y - 1).getPiece()) == BOMB || toupper(gameBoard.board[from_x - 1][from_y - 1].getPiece()) == FLAG)
	{
		cout << "Error: flag/bomb piece is not allowed to move" << endl;
		return false;
	}

	if (to_x == from_x + 1 || to_x == from_x - 1)
	{
		if (to_y != from_y)
		{
			cout
				<< "Error: illegal move - can move only one cell up/down/left/right "
				<< endl;
			return false;
		}
	}
	else if (to_y == from_y + 1 || to_y == from_y - 1)
	{
		if (to_x != from_x)
		{
			cout
				<< "Error: illegal move - can move only one cell up/down/left/right"
				<< endl;
			return false;
		}
	}
	else
	{
		cout
			<< "Error: illegal move - can move only one cell up/down/left/right"
			<< endl;
		return false;
	}

	if (gameBoard.board.at(to_x - 1).at(to_y - 1).getPiece() != 0)
	{
		if (isPlayer1)
		{
			if (isupper(gameBoard.board.at(to_x - 1).at(to_y - 1).getPiece()))
			{
				cout
					<< "Error: you are trying to move to a cell taken by your own piece"
					<< endl;
				return false;
			}
		}
		else
		{
			if (islower(gameBoard.board.at(to_x - 1).at(to_y - 1).getPiece()))
			{
				cout
					<< "Error: you are trying to move to a cell taken by your own piece"
					<< endl;
				return false;
			}
		}
	}

	return true;
}

/*
 Update array with relevant piece
 Input- playerNum and counter array, and piece
 output- updated array with relevant piece
 */
void RPSGame::countNumOfPieces(const int playerNum, int numOfPositionedPieces[],
							   const int piece)
{
	switch (piece)
	{
	case ROCK:
		numOfPositionedPieces[0]++;
		playerNum == 1 ? playerOne.setNumOfPieces(0, playerOne.numOfPieces[0] + 1) : playerTwo.setNumOfPieces(0, playerTwo.numOfPieces[0] + 1);
		break;
	case PAPER:
		numOfPositionedPieces[1]++;
		playerNum == 1 ? playerOne.setNumOfPieces(1, playerOne.numOfPieces[1] + 1) : playerTwo.setNumOfPieces(1, playerTwo.numOfPieces[1] + 1);
		break;
	case SCISSOR:
		numOfPositionedPieces[2]++;
		playerNum == 1 ? playerOne.setNumOfPieces(2, playerOne.numOfPieces[2] + 1) : playerTwo.setNumOfPieces(2, playerTwo.numOfPieces[2] + 1);
		break;
	case BOMB:
		numOfPositionedPieces[3]++;
		playerNum == 1 ? playerOne.setNumOfPieces(3, playerOne.numOfPieces[3] + 1) : playerTwo.setNumOfPieces(3, playerTwo.numOfPieces[3] + 1);
		break;
	case JOKER:
		numOfPositionedPieces[4]++;
		playerNum == 1 ? playerOne.setNumOfPieces(4, playerOne.numOfPieces[4] + 1) : playerTwo.setNumOfPieces(4, playerTwo.numOfPieces[4] + 1);
		break;
	case FLAG:
		numOfPositionedPieces[5]++;
		playerNum == 1 ? playerOne.setNumOfPieces(5, playerOne.numOfPieces[5] + 1) : playerTwo.setNumOfPieces(5, playerTwo.numOfPieces[5] + 1);
		break;
	default:
		cout << "Error: Bad format - illegal piece" << endl;
	}
}
/*
 Input - array of pieces
 Output - check piece overflow 
 */
bool RPSGame::checkPieceOverflow(int numOfPieces[])
{

	if (numOfPieces[0] > ROCKS_NUM || numOfPieces[1] > PAPERS_NUM || numOfPieces[2] > SCISSORS_NUM || numOfPieces[3] > BOMBS_NUM || numOfPieces[4] > JOKERS_NUM || numOfPieces[5] > FLAGS_NUM)
	{

		cout << "Error: a piece type appears in file more than its number"
			 << endl;
		return true;
	}
	return false;
}

/*
 This function check initial position and locate players on the board.
 Input - two boolean fields that indicate if there was a problen in players position format
 Output -  -1 if there is bad format or -2 if there is a problem with getline func
 */
int RPSGame::checkPositionOnBoard(bool &isPlayerOneLegalFormat,
								  bool &isPlayerTwoLegalFormat,
								  RPSFight &fights, std::vector<unique_ptr<FightInfo>> &initFights)
{
	std::vector<unique_ptr<PiecePosition>> vectorToFillPlayerOne;
	std::vector<unique_ptr<PiecePosition>> vectorToFillPlayerTwo;

	playerAlgoOne->getInitialPositions(1, vectorToFillPlayerOne);
	playerAlgoTwo->getInitialPositions(2, vectorToFillPlayerTwo);
	//resetGameResults();
	// <vector to fill> contains position(pos), piece(kind), jokerRep(rep)
	//Iterate player 1 vector and position on the board
	int row;
	int col;
	bool isJoker = false;
	int numOfPositionedPieces[6] = {0};
	int flagCnt = 0;
	//bool isPlayerOneLegalFormat = true;

	int vectorSize = (int)vectorToFillPlayerOne.size();
	for (int i = 0; i < vectorSize; i++)
	{
		isJoker = false;
		int inputPiece = vectorToFillPlayerOne[i]->getPiece();
		// In case the line if bad forrmated
		if (inputPiece == -1)
		{
			isPlayerOneLegalFormat = false;
			return -1;
		}

		else if (inputPiece == -2)
		{
			isPlayerOneLegalFormat = false;
			return -2;
		}
		else
		{
			//check position was already done there! (maybe we need to trasform here)
			row = vectorToFillPlayerOne[i]->getPosition().getX();
			col = vectorToFillPlayerOne[i]->getPosition().getY();

			if (vectorToFillPlayerOne[i]->getJokerRep() != '#')
			{
				isJoker = true;
			}
			countNumOfPieces(1, numOfPositionedPieces, inputPiece);

			if (toupper(inputPiece) == FLAG)
			{

				flagCnt++;
			}

			if (gameBoard.board.at(row).at(col).getPiece() != 0)
			{
				cout
					<< "Error: two or more pieces are positioned on the same location"
					<< endl;
				isPlayerOneLegalFormat = false;
				return -1;
			}
			else
			{
				Cell::updateCell(gameBoard.board.at(row).at(col), inputPiece, isJoker);
			}
		}
	}

	//check if there are too many pieces positioned on board
	if (checkPieceOverflow(numOfPositionedPieces))
	{
		isPlayerOneLegalFormat = false;
		return -1;
	}
	//check if there are not enough flags positioned on board
	if (flagCnt < FLAGS_NUM)
	{
		isPlayerOneLegalFormat = false;
		cout << "Error: Missing flags - not all flags are positioned on board"
			 << endl;
		return -1;
	}

	//Iterate player 2 vector and position on the board
	flagCnt = 0;

	//numOfPositionedPieces[6] = {0};
	memset(numOfPositionedPieces, 0, sizeof(numOfPositionedPieces)); // for automatically-allocated arrays

	for (int i = 0; i < (int)vectorToFillPlayerTwo.size(); i++)
	{
		isJoker = false;
		int inputPiece = vectorToFillPlayerTwo[i]->getPiece();
		// In case the line if bad forrmated
		if (inputPiece == -1)
		{
			isPlayerTwoLegalFormat = false;
			return -1;
		}
		// In case the line could no be read
		else if (inputPiece == -2)
		{
			isPlayerTwoLegalFormat = false;
			return -1;
		}
		else
		{
			//check position was already done there! (maybe we need to trasform here)
			row = vectorToFillPlayerTwo[i]->getPosition().getX();
			col = vectorToFillPlayerTwo[i]->getPosition().getY();

			if (vectorToFillPlayerTwo[i]->getJokerRep() != '#')
			{
				isJoker = true;
			}
			countNumOfPieces(2, numOfPositionedPieces, inputPiece);

			if (toupper(inputPiece) == FLAG)
			{
				flagCnt++;
			}
			if (gameBoard.board.at(row).at(col).getPiece() != 0)
			{
				if (gameBoard.board.at(row).at(col).getPiece() == islower(gameBoard.board.at(row).at(col).getPiece()))
				{
					isPlayerTwoLegalFormat = false;
					cout
						<< "Error: two or more pieces are positioned on the same location"
						<< endl;
					return -1;
				}
				else
				{
					RPSpoint player1Pos(vectorToFillPlayerOne[i]->getPosition().getX(), vectorToFillPlayerOne[i]->getPosition().getY());
					RPSpoint player2Pos(vectorToFillPlayerTwo[i]->getPosition().getX(), vectorToFillPlayerTwo[i]->getPosition().getY());
					fight(false, row, col, tolower(inputPiece), isJoker, fights, initFights,
						  player1Pos, player2Pos);
					playerAlgoOne->notifyFightResult(fights);
					playerAlgoTwo->notifyFightResult(fights);
				}
			}
			else
			{
				Cell::updateCell(gameBoard.board.at(row).at(col), tolower(inputPiece),
								 isJoker);
			}
		}
	}
	//check if there are too many pieces positioned on board
	if (checkPieceOverflow(numOfPositionedPieces))
	{
		isPlayerTwoLegalFormat = false;
		return -1;
	}
	if (flagCnt < FLAGS_NUM)
	{
		isPlayerTwoLegalFormat = false;
		cout << "Error: Missing flags - not all flags are positioned on board"
			 << endl;
		return -1;
	}
	return 0;
}

/*
 Reset game result: set player's scroe to 0,  isWinner to false and isGame over to false;
 */
void RPSGame::resetGameResults()
{
	//reset game result after fights that have done due to positioning files
	playerOne.setIsWinner(false);
	playerTwo.setIsWinner(false);
	playerOne.setScore(0);
	playerTwo.setScore(0);
	isGameOver = false;
}
/*
 Set GameOver relevant fields;
 Input - winner number and game over reason
 Output - update player's winning type and score
 */
void RPSGame::setGameOver(int winnerNumber, GAME_OVER_TYPE type)
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

int RPSGame::checkBadFormat()
{
	bool isPlayerOneLegalFormat = true;
	bool isPlayerTwoLegalFormat = true;
	RPSFight fightInfo = RPSFight();

	std::vector<unique_ptr<FightInfo>> initFights;
	int isLegalFormat = checkPositionOnBoard(isPlayerOneLegalFormat,
											 isPlayerTwoLegalFormat, fightInfo, initFights);
	resetGameResults(); //reset the game result as we did in ex1!
	if (!isPlayerOneLegalFormat && !isPlayerTwoLegalFormat)
	{
		setGameOver(0, WRONG_FILE_FORMAT_BOTH);

		return isLegalFormat;
	}
	else if (!isPlayerOneLegalFormat)
	{
		setGameOver(2, WRONG_FILE_FORMAT_ONE);
		return isLegalFormat;
	}
	else if (!isPlayerTwoLegalFormat)
	{
		setGameOver(1, WRONG_FILE_FORMAT_TWO);
		return isLegalFormat;
	}
	const RPSBoard gameBoardConst = gameBoard;
	playerAlgoOne->notifyOnInitialBoard(gameBoardConst, initFights);
	playerAlgoTwo->notifyOnInitialBoard(gameBoardConst, initFights);
	return isLegalFormat;
}

/*
 parse enum of typeGame and returns
 Input - enum winning type, index line pos of error of player 1, index line pos of error of player 2,index line move of player 1 , index line move of player 2
 output - string that represent winning reason.
 */
string RPSGame::ToString(GAME_OVER_TYPE typeGame, int indexErrorPosOne,
						 int indexErrorPosTwo, int indexErrorMoveOne, int indexErrorMoveTwo)
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
	default:
		return "[Unknown GAME_OVER_TYPE]";
	}
}
/*
 Verify that all game files exists
 */
bool RPSGame::isAllGameFilesExists(bool isPlayerOneUseFile, bool isPlayerTwoUseFile)
{
	bool PositionP1Exists = false, PositioP2Exists = false,
		 moveP1Exists = false, moveP2Exists = false;
	std::string currentFilename;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(".")) != NULL)
	{
		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL)
		{
			currentFilename = ent->d_name;
			if (!PositionP1Exists && currentFilename.compare(PLAYER_ONE_POSITION_FILENAME) == 0)
			{
				PositionP1Exists = true;
			}
			else if (!PositioP2Exists && currentFilename.compare(PLAYER_TWO_POSITION_FILENAME) == 0)
			{
				PositioP2Exists = true;
			}
			else if (!moveP1Exists && currentFilename.compare(PLAYER_ONE_MOVE_FILENAME) == 0)
			{
				moveP1Exists = true;
			}
			else if (!moveP2Exists && currentFilename.compare(PLAYER_TWO_MOVE_FILENAME) == 0)
			{
				moveP2Exists = true;
			}
		} //while
		closedir(dir);
		if (isPlayerOneUseFile && (!PositionP1Exists || !moveP1Exists))
		{
			std::cout
				<< "Error reading temp dirList file in working directory, Exit from Game."
				<< std::endl;
			return false;
		}
		if (isPlayerTwoUseFile && (!PositioP2Exists || !moveP2Exists))
		{
			std::cout
				<< "Error reading temp dirList file in working directory, Exit from Game."
				<< std::endl;
			return false;
		}
		return true;
	}
	else
	{
		std::cout
			<< "Unable to open temp dirList file in working directory, Exit from Game."
			<< std::endl;
		return false;
	}
	return false;
}
/*
 Return true if game is overm otherwise false
 */
bool RPSGame::checkGameOver(bool isBeforeMove, bool isPlayerOneTurn)
{
	cout << "in checkGameOver" << endl;
	Player *currPlayer = &playerOne;
	Player *nextPlayer = &playerTwo;
	if (!isPlayerOneTurn)
	{
		currPlayer = &playerTwo;
		nextPlayer = &playerOne;
	}
	if (currPlayer->numOfPieces[5] == 0 && nextPlayer->numOfPieces[5] == 0)
	{
		cout << "GameOver1" << endl;
		isGameOver = true;
		currPlayer->setIsWinner(false);
		nextPlayer->setIsWinner(false);
		gameOverReason = TIE_ALL_FLAGS_EATEN;
		return true;
	}
	//check if all of player one's flags are taken
	if (currPlayer->numOfPieces[5] == 0)
	{
		cout << "GameOver2" << endl;
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
		cout << "GameOver3" << endl;
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
		cout << "GameOver4" << endl;
		nextPlayer->setIsWinner(true);
		currPlayer->setIsWinner(false);
		nextPlayer->setScore(nextPlayer->getScore() + 1);
		isGameOver = true;
		gameOverReason = ALL_PIECES_EATEN;
		return true;
	}
	return false;
}

/*
Input - boolean that represents players turn, pieces row and col of next player , 
curr piece player,  

*/
//oid FilePlayerAlgorithm::notifyFightResult(const FightInfo& fightInfo)
bool RPSGame::fight(bool isPlayerOneTurn, int row, int col, char currPiece,
					bool isCurrPieceJoker, RPSFight &fights, std::vector<unique_ptr<FightInfo>> &initFights,
					RPSpoint posOne, RPSpoint posTwo)
{
	Player *currPlayer = &playerOne;
	Player *nextPlayer = &playerTwo;
	RPSpoint *currPos = &posOne;
	RPSpoint *NextPlayerPos = &posTwo;
	int currPlayerNum = 1;
	int nextPlayerNum = 2;
	RPSFight *ptr = new RPSFight(); //check this - might be bad practice
	if (!isPlayerOneTurn)
	{
		currPlayer = &playerTwo;
		nextPlayer = &playerOne;
		*currPos = posTwo;
		*NextPlayerPos = posOne;
		currPlayerNum = 2;
		nextPlayerNum = 1;
	}
	char currPlayerPiece = toupper(currPiece);
	char nextPlayerPiece = toupper(gameBoard.board.at(row).at(col).getPiece());
	fights.setPosition(*currPos);
	fights.setOpponentPiece(nextPlayerPiece);
	fights.setCurrPiece(currPiece);
	cout << "fight information" << endl;
	cout << "next player" << nextPlayerNum << endl;
	cout << "curr player" << currPlayerNum << endl;
	//Case 1: 2 players in the same type - both should be eaten
	if (nextPlayerPiece == currPlayerPiece)
	{
		cout << "case1" << endl;
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
		fights.setWinner(0);
		ptr->setPosition(*currPos);
		ptr->setOpponentPiece(gameBoard.board.at(row).at(col).getPiece());
		ptr->setWinner(0);
		initFights.push_back(make_unique<RPSFight>(*ptr));

		Cell::updateCell(gameBoard.board.at(row).at(col), 0, false);
	}

	//Case 2: there is flag and current player has another piece
	else if (nextPlayerPiece == FLAG)
	{
		cout << "case2" << endl;
		nextPlayer->numOfPieces[5]--;
		if (currPlayerPiece == BOMB)
		{
			//bomb exploded!
			currPlayer->numOfPieces[3]--;
			Cell::updateCell(gameBoard.board.at(row).at(col), 0, false);
		}
		else
		{
			Cell::updateCell(gameBoard.board.at(row).at(col), currPiece,
							 isCurrPieceJoker);
		}
		//initFights.push_back(
		// 	make_unique<RPSFight>(currPos, gameBoard.board.at(row).at(col).getPiece(), currPlayerNum));

		ptr->setPosition(*currPos);
		ptr->setOpponentPiece(gameBoard.board.at(row).at(col).getPiece());
		ptr->setWinner(0);
		initFights.push_back(make_unique<RPSFight>(*ptr));
	}

	//case 3: current player's piece is flag and there is another piece on board
	else if (currPlayerPiece == FLAG)
	{
		cout << "case3" << endl;
		currPlayer->numOfPieces[5]--;

		if (nextPlayerPiece == BOMB)
		{
			//bomb exploded!
			nextPlayer->numOfPieces[3]--;
			Cell::updateCell(gameBoard.board.at(row).at(col), 0, false);
		}
		fights.setWinner(nextPlayerNum);
		//initFights.push_back(
		// 	make_unique<RPSFight>(currPos, gameBoard.board.at(row).at(col).getPiece(), nextPlayerNum));

		ptr->setPosition(*currPos);
		ptr->setOpponentPiece(gameBoard.board.at(row).at(col).getPiece());
		ptr->setWinner(0);
		initFights.push_back(make_unique<RPSFight>(*ptr));
	}
	//case 4: there is bomb and current player has another piece
	else if (nextPlayerPiece == BOMB)
	{
		cout << "case4" << endl;
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
		Cell::updateCell(gameBoard.board.at(row).at(col), 0, false);
		fights.setWinner(nextPlayerNum);
		//initFights.push_back(
		//	make_unique<RPSFight>(currPos, gameBoard.board.at(row).at(col).getPiece(), nextPlayerNum));

		ptr->setPosition(*currPos);
		ptr->setOpponentPiece(gameBoard.board.at(row).at(col).getPiece());
		ptr->setWinner(0);
		initFights.push_back(make_unique<RPSFight>(*ptr));
	}

	//case 5: current player piece is bomb and on board there is another piece
	else if (currPlayerPiece == BOMB)
	{
		cout << "case5" << endl;
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
		Cell::updateCell(gameBoard.board.at(row).at(col), 0, false);
		fights.setWinner(nextPlayerNum);
		//initFights.push_back(
		//	make_unique<RPSFight>(currPos, gameBoard.board.at(row).at(col).getPiece(), nextPlayerNum));

		ptr->setPosition(*currPos);
		ptr->setOpponentPiece(gameBoard.board.at(row).at(col).getPiece());
		ptr->setWinner(0);
		initFights.push_back(make_unique<RPSFight>(*ptr));
	}

	//case 6: there is PAPER and current player has another piece
	else if (nextPlayerPiece == PAPER)
	{
		cout << "case6" << endl;
		if (currPlayerPiece == ROCK)
		{
			fights.setPosition(*currPos);
			fights.setWinner(nextPlayerNum);
			//initFights.push_back(
			//	make_unique<RPSFight>(currPos, gameBoard.board.at(row).at(col).getPiece(), nextPlayerNum));

			ptr->setPosition(*currPos);
			ptr->setOpponentPiece(gameBoard.board.at(row).at(col).getPiece());
			ptr->setWinner(0);
			initFights.push_back(make_unique<RPSFight>(*ptr));

			currPlayer->numOfPieces[0]--;
		}
		else if (currPlayerPiece == SCISSOR)
		{
			nextPlayer->numOfPieces[1]--;
			fights.setWinner(currPlayerNum);
			//initFights.push_back(
			//make_unique<RPSFight>(currPos, gameBoard.board.at(row).at(col).getPiece(), currPlayerNum));

			ptr->setPosition(*currPos);
			ptr->setOpponentPiece(gameBoard.board.at(row).at(col).getPiece());
			ptr->setWinner(0);
			initFights.push_back(make_unique<RPSFight>(*ptr));

			Cell::updateCell(gameBoard.board.at(row).at(col), currPiece,
							 isCurrPieceJoker);
		}
	}
	//case 7: player 1 is ROCK and player 2 another piece
	else if (nextPlayerPiece == ROCK)
	{
		cout << "case7" << endl;
		if (currPlayerPiece == PAPER)
		{
			nextPlayer->numOfPieces[0]--;
			fights.setWinner(currPlayerNum);
			//initFights.push_back(
			//	make_unique<RPSFight>(currPos, gameBoard.board.at(row).at(col).getPiece(), currPlayerNum));

			ptr->setPosition(*currPos);
			ptr->setOpponentPiece(gameBoard.board.at(row).at(col).getPiece());
			ptr->setWinner(0);
			initFights.push_back(make_unique<RPSFight>(*ptr));

			Cell::updateCell(gameBoard.board.at(row).at(col), currPiece,
							 isCurrPieceJoker);
		}
		else if (currPlayerPiece == SCISSOR)
		{
			currPlayer->numOfPieces[2]--;
			fights.setOpponentPiece(gameBoard.board.at(row).at(col).getPiece());
			fights.setWinner(nextPlayerNum);
			//initFights.push_back(
			// 	make_unique<RPSFight>(currPos, gameBoard.board.at(row).at(col).getPiece(), nextPlayerNum));

			ptr->setPosition(*currPos);
			ptr->setOpponentPiece(gameBoard.board.at(row).at(col).getPiece());
			ptr->setWinner(0);
			initFights.push_back(make_unique<RPSFight>(*ptr));
		}
	}

	else if (nextPlayerPiece == SCISSOR)
	{
		cout << "case8" << endl;
		if (currPlayerPiece == PAPER)
		{
			fights.setPosition(*currPos);
			fights.setWinner(nextPlayerNum);
			//initFights.push_back(
			// 	make_unique<RPSFight>(currPos, gameBoard.board.at(row).at(col).getPiece(), nextPlayerNum));
			ptr->setPosition(*currPos);
			ptr->setOpponentPiece(gameBoard.board.at(row).at(col).getPiece());
			ptr->setWinner(0);
			initFights.push_back(make_unique<RPSFight>(*ptr));
			currPlayer->numOfPieces[1]--;
		}
		else if (currPlayerPiece == ROCK)
		{
			fights.setWinner(currPlayerNum);
			//initFights.push_back(
			// 	make_unique<RPSFight>(currPos, gameBoard.board.at(row).at(col).getPiece(), currPlayerNum));

			ptr->setPosition(*currPos);
			ptr->setOpponentPiece(gameBoard.board.at(row).at(col).getPiece());
			ptr->setWinner(0);
			initFights.push_back(make_unique<RPSFight>(*ptr));

			nextPlayer->numOfPieces[2]--;

			Cell::updateCell(gameBoard.board.at(row).at(col), currPiece,
							 isCurrPieceJoker);
		}
	}
	return checkGameOver(false, isPlayerOneTurn);
}

/*
 Input - ofstream of output file
 Output - print the board to the file
 */
void RPSGame::printBoardToFile(ofstream &output)
{
	for (int i = 1; i <= ROWS; i++)
	{
		for (int j = 1; j <= COLS; j++)
		{
			if (gameBoard.board[i][j].getIsJoker())
			{
				if (Cell::isPlayerOnePiece(gameBoard.board[i][j]))
				{
					output << "J";
				}
				else
				{
					output << "j";
				}
			}
			else if (gameBoard.board[i][j].getPiece() == 0)
			{
				output << " ";
			}
			else
			{
				output << gameBoard.board[i][j].getPiece();
			}
		}
		output << endl;
	}
	output.ofstream::close();
	return;
}

/*
This function prints the board to the file.
Input - output file name
*/
void RPSGame::printOutputFile(const string &outputFile)
{
	ofstream output;
	output.open(outputFile, ios::trunc);
	if (output.fail())
	{
		cout << "Error while opening output file. Exiting game" << endl;
		return;
	}
	//player 1 is thw winner
	if (playerOne.getIsWinner())
	{
		output << "Winner: 1" << endl;
	}
	//player 2 is the winner
	else if (playerTwo.getIsWinner())
	{
		output << "Winner: 2" << endl;
	}
	//tie
	else
	{
		output << "Winner: 0" << endl;
	}
	output << "Reason: "
		   << ToString(getGameOverReason(), indexErrorPosOne, indexErrorPosTwo,
					   indexErrorMoveOne, indexErrorMoveTwo)
		   << endl;
	output << endl; // the third line must be en empty line!
	printBoardToFile(output);
	output.close();
	return;
}

/*
Input - game method.
Output- true if we successfuly init playerAlogrithms or false otherwise;
*/
bool RPSGame::initGameMethod(string gameMethod)
{
	bool isPlayerOneUseFile = false;
	bool isPlayerTwoUseFile = false;
	if (gameMethod.compare("auto-vs-file") == 0)
	{
		isPlayerTwoUseFile = true;
	}
	else if (gameMethod.compare("file-vs-auto") == 0)
	{
		isPlayerOneUseFile = true;
	}
	else if (gameMethod.compare("auto-vs-auto") == 0)
	{
		return initPlayersAlgo(isPlayerOneUseFile, isPlayerTwoUseFile);
	}
	else if (gameMethod.compare("file-vs-file") == 0)
	{
		isPlayerOneUseFile = true;
		isPlayerTwoUseFile = true;
	}
	else
	{
		cout << "Error: Illegal game method." << endl;
		return false;
	}
	return initPlayersAlgo(isPlayerOneUseFile, isPlayerTwoUseFile);
}

/*
Input - two boolean parameters that indicated if the players are using file play mode
Output- true if we successfuly init playerAlogrithms or false otherwise;
*/
bool RPSGame::initPlayersAlgo(bool isPlayerOneUseFile, bool isPlayerTwoUseFile)
{
	if (!isAllGameFilesExists(isPlayerOneUseFile, isPlayerTwoUseFile))
		return false;

	if (isPlayerOneUseFile)
	{
		playerAlgoOne = new FilePlayerAlgorithm(PLAYER_ONE_POSITION_FILENAME, PLAYER_ONE_MOVE_FILENAME);
		cout << PLAYER_ONE_POSITION_FILENAME << endl;
	}
	else
	{
		playerAlgoOne = new AutoPlayerAlgorithm();
	}
	if (isPlayerTwoUseFile)
	{
		playerAlgoTwo = new FilePlayerAlgorithm(PLAYER_TWO_POSITION_FILENAME, PLAYER_TWO_MOVE_FILENAME);
		cout << PLAYER_TWO_POSITION_FILENAME << endl;
	}
	else
	{
		cout << "isPlayerTwoUseFile == false" << endl;
		playerAlgoTwo = new AutoPlayerAlgorithm();
	}

	return true;
}

void RPSGame::PrintBoardToConsole()
{
	cout << "*******************PRINT THE BOARD:****************" << endl;
	for (int i = 1; i <= ROWS; i++)
	{
		for (int j = 1; j <= COLS; j++)
		{
			if (gameBoard.board[i][j].getIsJoker())
			{
				if (Cell::isPlayerOnePiece(gameBoard.board[i][j]))
				{
					cout << "J";
				}
				else
				{
					cout << "j";
				}
			}
			else if (gameBoard.board[i][j].getPiece() == 0)
			{
				cout << "-";
			}
			else
			{
				cout << gameBoard.board[i][j].getPiece();
			}
		}
		cout << endl;
	}
}
