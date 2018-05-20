#include "RPSGame.h"
RPSGame::RPSGame() : isGameOver(false), playerOne(Player(1)), playerTwo(Player(2))
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

void RPSGame::updateJokerMovingPieces()
{
	PrintBoardToConsole();
	for (int i = 1; i <= COLS; ++i)
	{
		for (int j = 1; j <= ROWS; ++j)
		{
			if (gameBoard.board[i][j].getIsJoker())
			{
				int pieceIndex;
				//cout << "before switch" << endl;
				switch (toupper(gameBoard.board[i][j].getPiece()))
				{
				case ROCK:
					//cout << "in rock" << endl;
					pieceIndex = 0;
					break;
				case PAPER:
					//cout << "in paper" << endl;
					pieceIndex = 1;
					break;
				case SCISSOR:
					//cout << "in scissors" << endl;
					pieceIndex = 2;
					break;
				case BOMB:
				//	cout << "in bomb" << endl;
					pieceIndex = 3;
					break;
				}
				isupper(gameBoard.board[i][j].getPiece()) ? playerOne.setNumOfPieces(pieceIndex,
																					 playerOne.numOfPieces[pieceIndex] + 1)
														  : playerTwo.setNumOfPieces(pieceIndex,
																					 playerTwo.numOfPieces[pieceIndex] + 1);

				//cout << "check1: " << playerTwo.numOfPieces[pieceIndex] << endl;
			}
		}
	}
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
Input- move object, joker's change object and players turn
Output- returns true if gameOver, false otherwise
 */
bool RPSGame::movePiece(unique_ptr<Move> &move,
						bool isPlayerOneTurn, bool &isBadFormat)
{
	RPSFight fights = RPSFight();
	std::vector<unique_ptr<FightInfo>> initFights;
	bool gameOverIntenral = false;
	bool wasFight = false;
	isBadFormat = false;
	numOfMoves++;						 // we increment the numerator for each move
	int from_x = move->getFrom().getX(); //col
	int from_y = move->getFrom().getY(); //row
	int to_x = move->getTo().getX();	 //col
	int to_y = move->getTo().getY();	 //row
	cout << "From x: " << from_x << endl;
	cout << "From y: " << from_y << endl;
	cout << "to x: " << to_x << endl;
	cout << "to y: " << to_y << endl;
	if (!isLegalMove(move, isPlayerOneTurn)) //noy changed!
	{
		if (isPlayerOneTurn)
		{
			setGameOver(2, WRONG_MOVE_FILE_FORMAT_ONE);
		}
		else
			setGameOver(1, WRONG_MOVE_FILE_FORMAT_TWO);
		isBadFormat = true;
		return true;
	}
	//do move
	if (gameBoard.board[to_x][to_y].getPiece() == 0)
	{
		Cell::updateCell(gameBoard.board[to_x][to_y], gameBoard.board[from_x][from_y].getPiece(),
						 gameBoard.board[from_x][from_y].getIsJoker());
		Cell::cleanCell(gameBoard.board[from_x][from_y]);
	}
	else //fight!
	{
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
		gameOverIntenral = isGameOver;
		isBadFormat = false;
		Cell::cleanCell(gameBoard.board[from_x][from_y]);
		/*
		For the player who just moved:
		notifyFightResult(const FightInfo& fightInfo); // only if there was a fight
		getJokerChange();--->shawn do we have to add this??	
		 */
		wasFight = true;
		isPlayerOneTurn ? playerAlgoOne->notifyFightResult(fights) : playerAlgoTwo->notifyFightResult(fights);
		/*
		For the other player:
		void notifyOnOpponentMove(const Move& move);
		void notifyFightResult(const FightInfo& fightInfo); // only if there was a fight
		=> go back to [A TURN] for this player
		 */
	}

	unique_ptr<JokerChange> playerJokerChange = (isPlayerOneTurn ? playerAlgoOne->getJokerChange() : playerAlgoTwo->getJokerChange());

	//no joker change
	if (playerJokerChange != NULL)
	{
		cout << "inside if!" << endl;
		int x_joker = playerJokerChange->getJokerChangePosition().getX(); //row

		int y_joker = playerJokerChange->getJokerChangePosition().getY(); //col

		char new_rep = playerJokerChange->getJokerNewRep();
		if (new_rep == 'E') //means bad format
		{
			if (isPlayerOneTurn)
			{
				isBadFormat = true;
				return true;
			}
		}
		else if (!gameBoard.board[x_joker][y_joker].getIsJoker())
		{ //if the original peice is not a joker
			cout << "Error: Piece specified is not joker" << endl;
			isBadFormat = true;
			return true;
		}
		if (new_rep != ROCK && new_rep != PAPER && new_rep != SCISSOR && new_rep != BOMB)
		{
			cout << "Error: illegal NEW_REP " << endl;
			isBadFormat = true;
			return true;
		}
		updateJokerChange(toupper(gameBoard.board[x_joker][y_joker].getPiece()), toupper(new_rep), isPlayerOneTurn);
		Cell::updateCell(gameBoard.board[x_joker][y_joker],
						 isPlayerOneTurn ? new_rep : tolower(new_rep), true);
	}

	if (wasFight)
	{
		isPlayerOneTurn ? playerAlgoTwo->notifyOnOpponentMove(*move) : playerAlgoOne->notifyOnOpponentMove(*move);
		isPlayerOneTurn ? playerAlgoTwo->notifyFightResult(fights) : playerAlgoOne->notifyFightResult(fights);
	}

	return gameOverIntenral;
}

/*
 * Output - return -1 if there was an error  during move, or 0 if the move was successful.
 */
int RPSGame::makeMove()
{

	unique_ptr<Move> move = std::move(playerAlgoOne->getMove());
	//unique_ptr<Move> move2 = std::move(playerAlgoTwo->getMove());
	int xPiecePlayerOne = move->getFrom().getX();

	int xPiecePlayerTwo = 1;
	//int xPiecePlayerTwo = move2->getFrom().getX();
	bool isPlayerOneTurn = true;
	bool isGameOverInternal = false;
	bool isBadFormat = false;
	while (xPiecePlayerOne != EOF_MOVE_ERR && xPiecePlayerOne != READ_LINE_ERR && xPiecePlayerTwo != EOF_MOVE_ERR && xPiecePlayerTwo != READ_LINE_ERR && numOfMoves < MAX_NUM_OF_MOVES)
	{
		isBadFormat = false;
		//		unique_ptr<JokerChange> playerOneJokerChange =
		//				playerAlgoOne->getJokerChange();
		//		unique_ptr<JokerChange> playerTwoJokerChange =
		//				playerAlgoTwo->getJokerChange();

		// cout<<"GOT JOKER CHANGES!!!!"<<endl;
		// cout<<"Joker change ONE to "<<playerOneJokerChange->getJokerChangePosition().getX()<<", "<<playerOneJokerChange->getJokerChangePosition().getY()<<"new rep: "<<playerOneJokerChange->getJokerNewRep()<<endl;
		// cout<<"Joker change TWO to "<<playerTwoJokerChange->getJokerChangePosition().getX()<<", "<<playerTwoJokerChange->getJokerChangePosition().getY()<<"new rep: "<<playerTwoJokerChange->getJokerNewRep()<<endl;

		isGameOverInternal = false;
		if (xPiecePlayerOne != 0)
		{
			isPlayerOneTurn = true;
			isGameOverInternal = movePiece(move, isPlayerOneTurn, isBadFormat);
			cout << "isGameOverInternal: " << isGameOverInternal << endl;
			PrintBoardToConsole();
			if (isGameOverInternal)
			{
				if (isBadFormat)
				{
					cout << "enter isGameOverInternal" << endl;
					cout << "ERROR AI: ILLEGAL MOVE" << endl;
					setGameOver(2, WRONG_MOVE_FILE_FORMAT_ONE);
				}
				// cout<<"enter isGameOverInternal"<<endl;
				// cout<<"ERROR AI: ILLEGAL MOVE"<<endl;
				// setGameOver(2, WRONG_MOVE_FILE_FORMAT_ONE);
				break;
			}
		}
		move = std::move(playerAlgoTwo->getMove());
		xPiecePlayerTwo = move->getFrom().getX();

		if (xPiecePlayerTwo != EOF_MOVE_ERR && xPiecePlayerTwo != READ_LINE_ERR)
		{
			if (xPiecePlayerTwo != EMPTY_LINE)
			{
				isPlayerOneTurn = false;
				isGameOverInternal = movePiece(move, isPlayerOneTurn, isBadFormat);
				cout << "isGameOverInternal: " << isGameOverInternal << endl;
				PrintBoardToConsole();
				if (isGameOverInternal)
				{
					if (isBadFormat)
					{
						cout << "enter isGameOverInternal" << endl;
						cout << "ERROR AI: ILLEGAL MOVE" << endl;
						setGameOver(1, WRONG_MOVE_FILE_FORMAT_TWO);
					}
					// cout<<"enter isGameOverInternal"<<endl;
					// cout<<"ERROR AI: ILLEGAL MOVE"<<endl;
					// setGameOver(1, WRONG_MOVE_FILE_FORMAT_TWO);
					break;
				}
			}
		}
		else if (xPiecePlayerTwo == READ_LINE_ERR)
		{
			cout << "Error while reading move file. Exiting game" << endl;
			//we need to remeber to close the stream!!!
			//playerAlgoTwo->closeStream();
			//playerAlgoOne->closeStream();
			return  ERROR_DURING_MOVE;
		}
		else
		{
			break;
		}

		move = playerAlgoOne->getMove();
		//move2 = playerAlgoTwo->getMove();
		xPiecePlayerOne = move->getFrom().getX();
		//xPiecePlayerTwo = move2->getFrom().getX();

	} //while
	//game stopped
	if (numOfMoves >= 100)
	{
		setGameOver(0, TIE_NO_WINNER);
		gameOverReason = TOO_MANY_MOVES;
		//playerAlgoTwo->closeStream();
		//playerAlgoOne->closeStream();
		return MOVE_DONE_SUCC;
	}
	if (!isGameOverInternal)
	{
		if (xPiecePlayerOne == EOF_MOVE_ERR) //EOFcase player 1
		{
			move = playerAlgoTwo->getMove();
			xPiecePlayerTwo = move->getFrom().getX();
			//playerTwoJokerChange =playerAlgoTwo->getJokerChange();
			while (xPiecePlayerTwo != EOF_MOVE_ERR && xPiecePlayerTwo != READ_LINE_ERR)
			{
				isBadFormat = false;
				if (xPiecePlayerTwo != EMPTY_LINE) 
				{
					isPlayerOneTurn = false;
					isGameOverInternal = movePiece(move, isPlayerOneTurn, isBadFormat);
					PrintBoardToConsole();
					if (isGameOverInternal)
					{
						if (isBadFormat)
						{
							cout << "enter isGameOverInternal" << endl;
							setGameOver(1, WRONG_MOVE_FILE_FORMAT_TWO);
						}
						cout << "enter isGameOverInternal" << endl;
						return ERROR_DURING_MOVE;
					}
				}
				move = playerAlgoTwo->getMove();
				xPiecePlayerTwo = move->getFrom().getX();
			}
			if (xPiecePlayerTwo == READ_LINE_ERR)
			{
				cout << "Error while reading move file. Exiting game" << endl;
				//we need to remeber to close the stream!!!
				//playerAlgoTwo->closeStream();
				//playerAlgoOne->closeStream();
				return ERROR_DURING_MOVE;
			}
		}
		else if (xPiecePlayerTwo == EOF_MOVE_ERR) //player 2 eof
		{
			//  move1 = playerAlgoOne->getMove();
			//  xPiecePlayerOne = move1->getFrom().getX();
			//playerOneJokerChange =playerAlgoOne->getJokerChange();
			while (xPiecePlayerOne != EOF_MOVE_ERR && xPiecePlayerOne != READ_LINE_ERR)
			{
				isBadFormat = false;
				if (xPiecePlayerOne != EMPTY_LINE)
				{
					isPlayerOneTurn = true;
					isGameOverInternal = movePiece(move, isPlayerOneTurn, isBadFormat);
					PrintBoardToConsole();
					if (isGameOverInternal)
					{
						if (isBadFormat)
						{
							setGameOver(2, WRONG_MOVE_FILE_FORMAT_ONE);
						}
						return ERROR_DURING_MOVE; //game over
					}
				}
				move = playerAlgoOne->getMove();
				xPiecePlayerOne = move->getFrom().getX();
			}
			if (xPiecePlayerOne == READ_LINE_ERR)
			{
				cout << "Error while reading move file. Exiting game" << endl;
				return ERROR_DURING_MOVE;
			}
		}

		//TODO : close the stream
		isPlayerOneTurn = !isPlayerOneTurn;
		bool doesGameOver = checkGameOver(true, isPlayerOneTurn);
		if (!isGameOverInternal && !isGameOver && !doesGameOver)
		{
			if (isPlayerOneTurn)
			{
				if (!playerTwo.isLeftMovingPieces())
				{
					setGameOver(1, ALL_PIECES_EATEN);

					//playerAlgoTwo->closeStream();
					//playerAlgoOne->closeStream();
					return MOVE_DONE_SUCC;
				}
			}
			else
			{
				if (!playerOne.isLeftMovingPieces())
				{
					setGameOver(2, ALL_PIECES_EATEN);

					//playerAlgoTwo->closeStream();
					//playerAlgoOne->closeStream();
					return MOVE_DONE_SUCC;
				}
			}
			setGameOver(0, TIE_NO_WINNER);
		}
	}

	//playerAlgoTwo->closeStream();
	//playerAlgoOne->closeStream();
	return MOVE_DONE_SUCC;
}

/*
 Return BAD_FORMAT_POS_ERR if there is bad format error or -2 if there is getline error
 and GAME_OVER_SUCC (0) if everything is ok
 */

int RPSGame::startGame()
{
	// std::vector<unique_ptr<PiecePosition>> vectorToFill1;
	// std::vector<unique_ptr<PiecePosition>> vectorToFill2;
	//meand bad format err
	int initResult = checkBadFormat();
	if (initResult == BAD_FORMAT_POS_ERR)
	{
		return BAD_FORMAT_POS_ERR;
	}
	else if (initResult == READ_LINE_POS_ERR)
	{
		cout << "Error: could not read line in file. exiting..." << endl;
		return READ_LINE_POS_ERR;
	}
	bool isPlayerOneTurn = true;
	bool isAboutToMove = true;
	if (checkGameOver(isAboutToMove, isPlayerOneTurn))
	{

		return GAME_OVER_SUCC;
	}

	//If we got here the board is initialized! now we need to make a move.
	cout << "going to make a move" << endl;

	return makeMove();
}

bool RPSGame::isLegalMove(unique_ptr<Move> &move, bool isPlayer1)
{
	int from_x = move->getFrom().getX();
	int from_y = move->getFrom().getY();
	int to_x = move->getTo().getX();
	int to_y = move->getTo().getY();
	if ((from_x < 1 || from_x > COLS) || (to_x < 1 || to_x > COLS) || (from_y < 1 || from_y > ROWS) || (to_y < 1 || to_y > ROWS))
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
	if (gameBoard.board.at(from_x).at(from_y).getPiece() == 0)
	{
		cout << "Error: there is no piece in this position" << endl;
		return false;
	}
	else if ((isPlayer1 && islower(gameBoard.board.at(from_x).at(from_y).getPiece())) || (!isPlayer1 && isupper(gameBoard.board[from_x][from_y].getPiece())))
	{
		cout << "Error: trying to move the opponent piece" << endl;
		return false;
	}

	if (toupper(gameBoard.board.at(from_x).at(from_y).getPiece()) == BOMB || toupper(gameBoard.board[from_x][from_y].getPiece()) == FLAG)
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

	if (gameBoard.board.at(to_x).at(to_y).getPiece() != 0)
	{
		if (isPlayer1)
		{
			if (isupper(gameBoard.board.at(to_x).at(to_y).getPiece()))
			{
				cout
					<< "Error: you are trying to move to a cell taken by your own piece"
					<< endl;
				return false;
			}
		}
		else
		{
			if (islower(gameBoard.board.at(to_x).at(to_y).getPiece()))
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
 output- updated array with relevant piece , return true is piece is ok otherwise false!
 */
bool RPSGame::countNumOfPieces(const int playerNum, int numOfPositionedPieces[],
							   const int piece)
{
	switch (piece)
	{
	case ROCK:
		numOfPositionedPieces[0]++;
		playerNum == 1 ? playerOne.setNumOfPieces(0, playerOne.numOfPieces[0] + 1) : playerTwo.setNumOfPieces(0, playerTwo.numOfPieces[0] + 1);
		return true;
		break;
	case PAPER:
		numOfPositionedPieces[1]++;
		playerNum == 1 ? playerOne.setNumOfPieces(1, playerOne.numOfPieces[1] + 1) : playerTwo.setNumOfPieces(1, playerTwo.numOfPieces[1] + 1);
		return true;
		break;
	case SCISSOR:
		numOfPositionedPieces[2]++;
		playerNum == 1 ? playerOne.setNumOfPieces(2, playerOne.numOfPieces[2] + 1) : playerTwo.setNumOfPieces(2, playerTwo.numOfPieces[2] + 1);
		return true;
		break;
	case BOMB:
		numOfPositionedPieces[3]++;
		playerNum == 1 ? playerOne.setNumOfPieces(3, playerOne.numOfPieces[3] + 1) : playerTwo.setNumOfPieces(3, playerTwo.numOfPieces[3] + 1);
		return true;
		break;
	case JOKER:
		numOfPositionedPieces[4]++;
		playerNum == 1 ? playerOne.setNumOfPieces(4, playerOne.numOfPieces[4] + 1) : playerTwo.setNumOfPieces(4, playerTwo.numOfPieces[4] + 1);
		return true;
		break;
	case FLAG:
		numOfPositionedPieces[5]++;
		playerNum == 1 ? playerOne.setNumOfPieces(5, playerOne.numOfPieces[5] + 1) : playerTwo.setNumOfPieces(5, playerTwo.numOfPieces[5] + 1);
		return true;
		break;
	default:
		cout << "Error: Bad format - illegal piece" << endl;
		return false;
	}
}
/*
 Input - array of pieces
 Output - check piece overflow 
 */
bool RPSGame::checkPieceOverflow(int numOfPieces[])
{
	cout << "in checkPieceOverflow" << endl;
	cout << "numOfPieces[0] " << numOfPieces[0] << "numOfPieces[1] " << numOfPieces[1] << "numOfPieces[2] " << numOfPieces[1] << " numOfPieces[3] " << numOfPieces[3] << " numOfPieces[4] " << numOfPieces[4] << " numOfPieces[5] " << numOfPieces[5] << endl;

	if (numOfPieces[0] > ROCKS_NUM || numOfPieces[1] > PAPERS_NUM || numOfPieces[2] > SCISSORS_NUM || numOfPieces[3] > BOMBS_NUM || numOfPieces[4] > JOKERS_NUM || numOfPieces[5] > FLAGS_NUM)
	{
		cout << "Error: a piece type appears in file more than its number"
			 << endl;

		return true;
	}
	return false;
}

/*
 This function locate players on the board.
 Input - player number, vectorTofill (that vector contain all players pieces), isPlayerLegalFormat will indicate if there s bad format
 numOFPieces array in order to check if all the pieces are located in the board, fights and initFights vector (for notify function)
 Output  BAD_FORMAT_POS_ERR(-1) if there is bad format on player one or READ_LINE_POS_ERR (-2) if there is a problem with getline func
 */
int RPSGame::locateOnBoard(int playerNum, std::vector<unique_ptr<PiecePosition>> &vectorToFill, bool &isPlayerLegalFormat, int numOfPositionedPieces[], RPSFight &fights, std::vector<unique_ptr<FightInfo>> &initFights)
{
	cout << "in locateOnBoard" << endl;
	int vectorSize = vectorToFill.size();
	bool isJoker = false;
	int x;
	int y;
	//initialize temporary board to check player's positions do not collide
	char board[COLS][ROWS] = {0};
	int inputPiece;
	//inputPiece;
	int flagCnt = 0;
	bool isPieceOkPlayer1;
	bool isPieceOkPlayer2;
	for (int i = 0; i < vectorSize; i++)
	{
		isJoker = false;
		inputPiece = vectorToFill[i]->getPiece();
		// In case the line if bad forrmated
		if (inputPiece == BAD_FORMAT_POS_ERR)
		{
			isPlayerLegalFormat = false;
			return BAD_FORMAT_POS_ERR;
		}

		else if (inputPiece == READ_LINE_POS_ERR)
		{
			isPlayerLegalFormat = false;
			return READ_LINE_POS_ERR;
		}
		else
		{
			//check position was already done there! (maybe we need to trasform here)
			x = vectorToFill[i]->getPosition().getX();
			y = vectorToFill[i]->getPosition().getY();
			if (vectorToFill[i]->getJokerRep() != '#')
			{
				isJoker = true;
			}
			if (playerNum == 1)
			{
				isPieceOkPlayer1 = countNumOfPieces(1, numOfPositionedPieces, inputPiece);
				if (!isPieceOkPlayer1)
				{
					isPlayerLegalFormat = false;
					return BAD_FORMAT_POS_ERR;
				}
			}
			else
			{
				isPieceOkPlayer2 = countNumOfPieces(2, numOfPositionedPieces, inputPiece);
				if (!isPieceOkPlayer2)
				{
					isPlayerLegalFormat = false;
					return BAD_FORMAT_POS_ERR;
				}
			}
			//position is illegal - tried to locate 2 pieces of same player in same position
			if (board[x-1][y-1] != 0) //changes!!!
			{
				cout<<"error 1"<<endl;
				cout
					<< "Error: two or more pieces are positioned on the same location"
					<< endl;
				isPlayerLegalFormat = false;
				return BAD_FORMAT_POS_ERR;
			}
			//else
			board[x-1][y-1] = inputPiece;

			if (toupper(inputPiece) == FLAG)
			{
				flagCnt++;
			}
			if (playerNum == 1)
			{
				if (gameBoard.board.at(x).at(y).getPiece() != 0)
				{
					cout<<"error 2"<<endl;
					cout
						<< "Error: two or more pieces are positioned on the same location for player " << playerNum << endl;
					isPlayerLegalFormat = false;
					return BAD_FORMAT_POS_ERR;
				}
				else
				{
					if (isJoker)
						Cell::updateCell(gameBoard.board.at(x).at(y), vectorToFill[i]->getJokerRep(), isJoker);
					else
						Cell::updateCell(gameBoard.board.at(x).at(y), inputPiece, isJoker);
				}
			}
			else
			{
				if (gameBoard.board.at(x).at(y).getPiece() != 0)
				{
					if (gameBoard.board.at(x).at(y).getPiece() == tolower(gameBoard.board.at(x).at(y).getPiece()))
					{
						cout<<"error 3"<<endl;
						isPlayerLegalFormat = false;
						cout << "Error: two or more pieces are positioned on the same location for player " << playerNum << endl;
						return BAD_FORMAT_POS_ERR;
					}
					else
					{
						RPSpoint player1Pos(vectorToFill[i]->getPosition().getX(), vectorToFill[i]->getPosition().getY()); //noy changes!
						RPSpoint player2Pos(vectorToFill[i]->getPosition().getX(), vectorToFill[i]->getPosition().getY()); //noy changes
						if (isJoker)
						{
							fight(false, x, y, tolower(vectorToFill[i]->getJokerRep()), isJoker, fights, initFights,
								  player1Pos, player2Pos);
						}
						else
							fight(false, x, y, tolower(inputPiece), isJoker, fights, initFights,
								  player1Pos, player2Pos);
						//playerAlgoOne->notifyFightResult(fights);
						//playerAlgoTwo->notifyFightResult(fights);
					}
				}
				else
				{
					if (isJoker)
						Cell::updateCell(gameBoard.board.at(x).at(y), tolower(vectorToFill[i]->getJokerRep()), isJoker);
					else
						Cell::updateCell(gameBoard.board.at(x).at(y), tolower(inputPiece),
										 isJoker);
				}
			}
		}
	}

	//check if there are too many pieces positioned on board
	if (checkPieceOverflow(numOfPositionedPieces))
	{
		isPlayerLegalFormat = false;
		return BAD_FORMAT_POS_ERR;
	}
	//check if there are not enough flags positioned on board
	if (flagCnt < FLAGS_NUM)
	{
		isPlayerLegalFormat = false;
		cout << "Error: Missing flags - not all flags are positioned on board for player " << playerNum << endl;
		return BAD_FORMAT_POS_ERR;
	}
	return NO_BAD_FORMAT_ERR;
}

/*
 This function check initial position and locate players on the board.
 Input - two boolean fields that indicate if there was a problen in players position format
 Output -  
 BAD_FORMAT_POS_ERR(-1) if there is an error
 READ_LINE_POS_ERR(-2) if there is an read error
 NO_BAD_FORMAT_ERR (0) if there is no bad formats problem or errors.
 */
int RPSGame::checkPositionOnBoard(bool &isPlayerOneLegalFormat,
								  bool &isPlayerTwoLegalFormat,
								  RPSFight &fights, std::vector<unique_ptr<FightInfo>> &initFights)
{
	cout << "in checkPositionOnBoard" << endl;
	std::vector<unique_ptr<PiecePosition>> vectorToFillPlayerOne;
	std::vector<unique_ptr<PiecePosition>> vectorToFillPlayerTwo;

	playerAlgoOne->getInitialPositions(1, vectorToFillPlayerOne);

	playerAlgoTwo->getInitialPositions(2, vectorToFillPlayerTwo);

	//resetGameResults(); //where to put this?
	// <vector to fill> contains position(pos), piece(kind), jokerRep(rep)
	//Iterate player 1 vector and position on the board
	int numOfPositionedPieces[6] = {0};

	//bool isPlayerOneLegalFormat = true;
	//check player One Format
	int resultPlayerOne = locateOnBoard(1, vectorToFillPlayerOne, isPlayerOneLegalFormat, numOfPositionedPieces, fights, initFights);
	updateJokerMovingPieces();
	memset(numOfPositionedPieces, 0, sizeof(numOfPositionedPieces)); // for automatically-allocated arrays
	int resultPlayerTwo = locateOnBoard(2, vectorToFillPlayerTwo, isPlayerTwoLegalFormat, numOfPositionedPieces, fights, initFights);
	updateJokerMovingPieces();

	cout << "check2: " << playerTwo.numOfPieces[2] << endl
		 << endl;

	//todo deltee this
	cout << "player1" << endl;
	for (int i = 0; i < 6; i++)
	{
		cout << playerOne.numOfPieces[i] << endl;
	}
	cout << endl;
	cout << "player2" << endl;

	for (int i = 0; i < 6; i++)
	{
		cout << playerTwo.numOfPieces[i] << endl;
	}

	if (resultPlayerOne == BAD_FORMAT_POS_ERR || resultPlayerOne == READ_LINE_POS_ERR)
	{
		return BAD_FORMAT_POS_ERR; 			//-1
	}
	else if (resultPlayerTwo == READ_LINE_POS_ERR || resultPlayerTwo == BAD_FORMAT_POS_ERR)
	{
		cout<<"enter and return -2"<<endl;
		return READ_LINE_POS_ERR; 			//-2
	}
	return NO_BAD_FORMAT_ERR;

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
		playerTwo.setIsWinner(false);
		playerOne.setScore(playerOne.getScore() + 1);
		return;
	}
	else
	{
		playerTwo.setIsWinner(true);
		playerOne.setIsWinner(false);
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
	cout<<"After is legalFormat"<<endl;
	resetGameResults(); //reset the game result as we did in ex1!
	if (!isPlayerOneLegalFormat && !isPlayerTwoLegalFormat)
	{
		setGameOver(0, WRONG_FILE_FORMAT_BOTH);
		return BAD_FORMAT_POS_ERR;
	}
	else if (!isPlayerOneLegalFormat)
	{
		setGameOver(2, WRONG_FILE_FORMAT_ONE);
		return BAD_FORMAT_POS_ERR;
	}
	else if (!isPlayerTwoLegalFormat)
	{
		setGameOver(1, WRONG_FILE_FORMAT_TWO);
		return BAD_FORMAT_POS_ERR;
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
string RPSGame::ToString(GAME_OVER_TYPE typeGame)
{
	switch (typeGame)
	{
	case ALL_FLAGS_CAPTURED:
		return "All flags of the opponent are captured";
	case ALL_PIECES_EATEN:
		return "All moving PIECEs of the opponent are eaten";
	case WRONG_FILE_FORMAT_ONE:
		return "Bad Positioning input file for player 1 "; //TODO : line
	case WRONG_FILE_FORMAT_TWO:
		return "Bad Positioning input file for player 2 "; //TODO : line
	case WRONG_FILE_FORMAT_BOTH:
		return "Bad Positioning input file for both players - player 1 , player 2";
	case TIE_NO_WINNER:
		return "A tie - both Moves input files done without a winner";
	case TIE_ALL_FLAGS_EATEN:
		return "A tie - all flags are eaten by both players in the position files";
	case WRONG_MOVE_FILE_FORMAT_TWO:
		return "Bad Move File input file for player 2  ";
	case WRONG_MOVE_FILE_FORMAT_ONE:
		return "Bad Move File input file for player 1  ";
	case TOO_MANY_MOVES:
		return "Number of moves exceeds maximum";
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
	cout << "^^^^^^^^^^^^^^^^^ Inside checkGameOver ^^^^^^^^^^^^^^^^" << endl;
	cout << "isBeforeMove =  " << isBeforeMove << endl;
	Player *currPlayer = &playerOne;
	Player *nextPlayer = &playerTwo;
	if (!isPlayerOneTurn)
	{
		currPlayer = &playerTwo;
		nextPlayer = &playerOne;
	}
	//currPlayer->isLeftMovingPieces();
	cout << "isLeft" << currPlayer->isLeftMovingPieces() << endl;

	if (currPlayer->numOfPieces[5] == 0 && nextPlayer->numOfPieces[5] == 0)
	{
		cout << "1" << endl;
		isGameOver = true;
		currPlayer->setIsWinner(false);
		nextPlayer->setIsWinner(false);
		gameOverReason = TIE_ALL_FLAGS_EATEN;
		return true;
	}
	//check if all of player one's flags are taken
	if (currPlayer->numOfPieces[5] == 0)
	{
		cout << "2" << endl;
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
		cout << "3" << endl;
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
		cout << "4" << endl;
		nextPlayer->setIsWinner(true);
		currPlayer->setIsWinner(false);

		nextPlayer->setScore(nextPlayer->getScore() + 1);
		isGameOver = true;
		gameOverReason = ALL_PIECES_EATEN;
		return true;
	}
	if (isBeforeMove && !nextPlayer->isLeftMovingPieces())
	{
		cout << "5" << endl;
		currPlayer->setIsWinner(true);
		nextPlayer->setIsWinner(false);

		currPlayer->setScore(nextPlayer->getScore() + 1);
		isGameOver = true;
		gameOverReason = ALL_PIECES_EATEN;
		return true;
	}
	return false;
}

/*
 * Input - boolean that represents players turn, pieces row and col of next player ,
 * curr piece player,  if the player is joker or not, RPSFight object for updating fight result
 * and vector to update for initial fights, 2 points that represent players position.
 * Output - true if game if over, otherwise false
 */
bool RPSGame::fight(bool isPlayerOneTurn, int x, int y, char currPiece,
					bool isCurrPieceJoker, RPSFight &fights, std::vector<unique_ptr<FightInfo>> &initFights,
					RPSpoint posOne, RPSpoint posTwo)
{
	numOfMoves = 0; // we resent the numerator if there is a fight
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
	char nextPlayerPiece = toupper(gameBoard.board.at(x).at(y).getPiece());
	ptr->setPlayerOnePiece(currPlayerNum == 1 ? currPlayerPiece : nextPlayerPiece);
	ptr->setPlayerTwoPiece(nextPlayerNum == 1 ? currPlayerPiece : nextPlayerPiece);
	ptr->setPosition(*NextPlayerPos);
	cout<<"Fight begin "<< "currPlayerPiece: "<< currPlayerPiece<<endl;
	cout<< "nextPlayerPiece: "<< nextPlayerPiece<<endl;

	fights.setPosition(*NextPlayerPos);
	fights.setPlayerOnePiece(ptr->getPiece(1));
	fights.setPlayerTwoPiece(ptr->getPiece(2));
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
		//ptr->setPosition(*currPos);
		//ptr->setOpponentPiece(gameBoard.board.at(x).at(y).getPiece());
		ptr->setWinner(0);
		initFights.push_back(make_unique<RPSFight>(*ptr));

		Cell::updateCell(gameBoard.board.at(x).at(y), 0, false);
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
			Cell::updateCell(gameBoard.board.at(x).at(y), 0, false);
		}
		else
		{
			Cell::updateCell(gameBoard.board.at(x).at(y), currPiece,
							 isCurrPieceJoker);
		}
		//initFights.push_back(
		// 	make_unique<RPSFight>(currPos, gameBoard.board.at(row).at(col).getPiece(), currPlayerNum));

		//ptr->setPosition(*currPos);
		//ptr->setOpponentPiece(gameBoard.board.at(x).at(y).getPiece());
		fights.setWinner(currPlayerNum);
		ptr->setWinner(currPlayerNum);
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
			Cell::updateCell(gameBoard.board.at(x).at(y), 0, false);
		}
		fights.setWinner(nextPlayerNum);
		//initFights.push_back(
		// 	make_unique<RPSFight>(currPos, gameBoard.board.at(row).at(col).getPiece(), nextPlayerNum));

		//ptr->setPosition(*currPos);
		//ptr->setOpponentPiece(gameBoard.board.at(x).at(y).getPiece());
		fights.setWinner(nextPlayerNum);
		ptr->setWinner(nextPlayerNum);
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
		Cell::updateCell(gameBoard.board.at(x).at(y), 0, false);
		fights.setWinner(nextPlayerNum);
		//initFights.push_back(
		//	make_unique<RPSFight>(currPos, gameBoard.board.at(row).at(col).getPiece(), nextPlayerNum));

		//ptr->setPosition(*currPos);
		//ptr->setOpponentPiece(gameBoard.board.at(x).at(y).getPiece());
		ptr->setWinner(nextPlayerNum);
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
		Cell::updateCell(gameBoard.board.at(x).at(y), 0, false);
		fights.setWinner(currPlayerPiece);
		//initFights.push_back(
		//	make_unique<RPSFight>(currPos, gameBoard.board.at(row).at(col).getPiece(), nextPlayerNum));

		//ptr->setPosition(*currPos);
		//ptr->setOpponentPiece(gameBoard.board.at(x).at(y).getPiece());
		ptr->setWinner(currPlayerPiece);
		initFights.push_back(make_unique<RPSFight>(*ptr));
	}

	//case 6: there is PAPER and current player has another piece
	else if (nextPlayerPiece == PAPER)
	{
		cout << "case6" << endl;

		if (currPlayerPiece == ROCK)
		{
			//fights.setPosition(*currPos);
			fights.setWinner(nextPlayerNum);
			//initFights.push_back(
			//	make_unique<RPSFight>(currPos, gameBoard.board.at(row).at(col).getPiece(), nextPlayerNum));

			//ptr->setPosition(*currPos);
			//ptr->setOpponentPiece(gameBoard.board.at(x).at(y).getPiece());
			ptr->setWinner(nextPlayerNum);
			initFights.push_back(make_unique<RPSFight>(*ptr));

			currPlayer->numOfPieces[0]--;
		}
		else if (currPlayerPiece == SCISSOR)
		{
			nextPlayer->numOfPieces[1]--;
			fights.setWinner(currPlayerNum);
			//initFights.push_back(
			//make_unique<RPSFight>(currPos, gameBoard.board.at(row).at(col).getPiece(), currPlayerNum));

			//ptr->setPosition(*currPos);
			//ptr->setOpponentPiece(gameBoard.board.at(x).at(y).getPiece());
			ptr->setWinner(currPlayerNum);
			initFights.push_back(make_unique<RPSFight>(*ptr));

			Cell::updateCell(gameBoard.board.at(x).at(y), currPiece,
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

			//ptr->setPosition(*currPos);
			//ptr->setOpponentPiece(gameBoard.board.at(x).at(y).getPiece());
			ptr->setWinner(currPlayerNum);
			initFights.push_back(make_unique<RPSFight>(*ptr));

			Cell::updateCell(gameBoard.board.at(x).at(y), currPiece,
							 isCurrPieceJoker);
		}
		else if (currPlayerPiece == SCISSOR)
		{
			currPlayer->numOfPieces[2]--;
			//fights.setOpponentPiece(gameBoard.board.at(x).at(y).getPiece());
			fights.setWinner(nextPlayerNum);
			//initFights.push_back(
			// 	make_unique<RPSFight>(currPos, gameBoard.board.at(row).at(col).getPiece(), nextPlayerNum));

			//ptr->setPosition(*currPos);
			//ptr->setOpponentPiece(gameBoard.board.at(x).at(y).getPiece());
			ptr->setWinner(nextPlayerNum);
			initFights.push_back(make_unique<RPSFight>(*ptr));
		}
	}

	else if (nextPlayerPiece == SCISSOR)
	{
		cout << "case8" << endl;
		if (currPlayerPiece == PAPER)
		{
			//fights.setPosition(*currPos);
			fights.setWinner(nextPlayerNum);
			//initFights.push_back(
			// 	make_unique<RPSFight>(currPos, gameBoard.board.at(row).at(col).getPiece(), nextPlayerNum));
			//ptr->setPosition(*currPos);
			//ptr->setOpponentPiece(gameBoard.board.at(x).at(y).getPiece());
			ptr->setWinner(nextPlayerNum);
			initFights.push_back(make_unique<RPSFight>(*ptr));
			currPlayer->numOfPieces[1]--;
		}
		else if (currPlayerPiece == ROCK)
		{
			cout << "Inside case 8 rock!" << endl;
			fights.setWinner(currPlayerNum);
			//initFights.push_back(
			// 	make_unique<RPSFight>(currPos, gameBoard.board.at(row).at(col).getPiece(), currPlayerNum));

			//ptr->setPosition(*currPos);
			//ptr->setOpponentPiece(gameBoard.board.at(x).at(y).getPiece());
			ptr->setWinner(currPlayerNum);
			initFights.push_back(make_unique<RPSFight>(*ptr));

			nextPlayer->numOfPieces[2]--;
			cout << "gameBoard.board.at(x).at(y)" << gameBoard.board[x][y].getPiece() << endl;
			cout << "curr piece" << currPiece << endl;
			Cell::updateCell(gameBoard.board.at(x).at(y), currPiece,
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
			if (gameBoard.board[j][i].getIsJoker())
			{
				if (Cell::isPlayerOnePiece(gameBoard.board[j][i]))
				{
					output << "J";
				}
				else
				{
					output << "j";
				}
			}
			else if (gameBoard.board[j][i].getPiece() == 0)
			{
				output << " ";
			}
			else
			{
				output << gameBoard.board[j][i].getPiece();
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
		   << ToString(getGameOverReason())
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
	cout << "in initGameMethod" << endl;
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
	cout << "in initPlayerAlgo" << endl;
	if (isPlayerOneUseFile || isPlayerTwoUseFile)
	{
		if (!isAllGameFilesExists(isPlayerOneUseFile, isPlayerTwoUseFile))
			return false;
	}

	if (isPlayerOneUseFile)
	{
		playerAlgoOne = new FilePlayerAlgorithm(PLAYER_ONE_POSITION_FILENAME, PLAYER_ONE_MOVE_FILENAME);
	}
	else
	{
		cout << "creaating new AutoPlayerAlgorithm" << endl;
		playerAlgoOne = new AutoPlayerAlgorithm();
	}
	if (isPlayerTwoUseFile)
	{
		playerAlgoTwo = new FilePlayerAlgorithm(PLAYER_TWO_POSITION_FILENAME, PLAYER_TWO_MOVE_FILENAME);
	}
	else
	{
		playerAlgoTwo = new AutoPlayerAlgorithm();
	}

	return true;
}

//Need to erase this function - only for debug!
void RPSGame::PrintBoardToConsole()
{
	cout << "*******************PRINT THE BOARD:****************" << endl;
	for (int i = 1; i <= ROWS; i++)
	{
		for (int j = 1; j <= COLS; j++)
		{
			if (gameBoard.board[j][i].getIsJoker())
			{
				if (Cell::isPlayerOnePiece(gameBoard.board[j][i]))
				{
					cout << " J ";
				}
				else
				{
					cout << " j ";
				}
			}
			else if (gameBoard.board[j][i].getPiece() == 0)
			{
				cout << " - ";
			}
			else
			{
				cout << " " << gameBoard.board[j][i].getPiece() << " ";
			}
		}
		cout << endl;
	}
}
