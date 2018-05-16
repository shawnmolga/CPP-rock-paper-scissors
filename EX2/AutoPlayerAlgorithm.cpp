/*
 * AutoPlayerAlgorithm.cpp
 *
 *  Created on: 2 ���� 2018
 *      Author: OR
 */

#include "AutoPlayerAlgorithm.h"

//constructor
AutoPlayerAlgorithm::AutoPlayerAlgorithm()
{
	gameBoard = AIBoard();
	myPlayerNum = -1;
	opponentBombsNumOnBoard = BOMBS_NUM + JOKERS_NUM;									//we dont know if joker is now bomb or not
	opponentMovingPieceNumOnBoard = SCISSORS_NUM + ROCKS_NUM + PAPERS_NUM + JOKERS_NUM; //we dont know if joker is moving piece now or not
}

void AutoPlayerAlgorithm::getInitialPositions(int player,
											  std::vector<unique_ptr<PiecePosition>> &vectorToFill)
{
	myPlayerNum = player;
	positionUnmovingPieces(player, vectorToFill);
	positionMovingPieces(player, vectorToFill);
	positionJokers(player, vectorToFill);
}

void AutoPlayerAlgorithm::positionMovingPieces(int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill)
{
	positionPiecesRandomly(ROCKS_NUM, player == 1 ? 'R' : 'r', false, vectorToFill);
	positionPiecesRandomly(PAPERS_NUM, player == 1 ? 'P' : 'p', false, vectorToFill);
	positionPiecesRandomly(SCISSORS_NUM, player == 1 ? 'S' : 's', false, vectorToFill);
}

void AutoPlayerAlgorithm::positionJokers(int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill)
{
	int pieceNum = getRandomNumInRange(1, 4);
	switch (pieceNum)
	{
	case 1: //rock
		positionPiecesRandomly(ROCKS_NUM, player == 1 ? 'R' : 'r', true, vectorToFill);
		break;
	case 2: //paper
		positionPiecesRandomly(PAPERS_NUM, player == 1 ? 'P' : 'p', false, vectorToFill);
		break;
	case 3: //scissors
		positionPiecesRandomly(SCISSORS_NUM, player == 1 ? 'S' : 's', false, vectorToFill);
		break;
	case 4: //bomb
		positionPiecesRandomly(SCISSORS_NUM, player == 1 ? 'B' : 'b', false, vectorToFill);
		break;
	default:
		break;
	}
}

void AutoPlayerAlgorithm::positionUnmovingPieces(int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill)
{
	bool isCellTaken = true;
	bool hasOpponentNeighbor = true;
	char piece = (player == 1 ? 'F' : 'f');
	int col = getRandomNumInRange(0, COLS - 1);
	int row = getRandomNumInRange(0, ROWS - 1);
	bool shouldPositionRandomly = false;
	int protectingBombsNum = BOMBS_NUM / (2 * FLAGS_NUM); //half will protect all flags and half will be spred randomly on board
	int bombsPositioned = 0;							  //sum all bombs already positioned
	if (protectingBombsNum < 1 && BOMBS_NUM > 0)
	{ //prefer to protect flags
		protectingBombsNum = 1;
	}

	for (int i = 0; i < FLAGS_NUM; ++i)
	{
		while (isCellTaken || hasOpponentNeighbor)
		{
			if (gameBoard.board[row][col].getPiece() == 0)
			{
				isCellTaken = false;
				if (!checkIsOpponentNeighbors)
				{
					hasOpponentNeighbor = false;
					Cell::updateCell(gameBoard.board[row][col], piece, false);
					vectorToFill.push_back(make_unique<RPSPiecePosition>(RPSpoint(row, col), piece, '#'));
					positionBombs(row, col, player, vectorToFill, protectingBombsNum, shouldPositionRandomly);
					bombsPositioned += protectingBombsNum;
				}
			}
			col = getRandomNumInRange(0, COLS - 1);
			row = getRandomNumInRange(0, ROWS - 1);
		}
		isCellTaken = true;
		hasOpponentNeighbor = true;
	}

	if (bombsPositioned < BOMBS_NUM)
	{
		shouldPositionRandomly = true;
		positionBombs(row, col, player, vectorToFill, BOMBS_NUM - bombsPositioned, shouldPositionRandomly);
	}
}

int AutoPlayerAlgorithm::getRandomNumInRange(int start, int end)
{
	std::random_device rd;								  // obtain a random number from hardware
	std::mt19937 eng(rd());								  // seed the generator
	std::uniform_int_distribution<> colDistr(start, end); // define the range
	return colDistr(eng);
}

bool AutoPlayerAlgorithm::findEmptyNeigbor(int &row, int &col, int flagRow, int FlagCol)
{
	//down neigbor
	char piece;
	if (isLegalPosition(row + 1, col))
	{
		piece = gameBoard.board[row + 1][col].getPiece();
		if (piece == 0) //empty cell
			return true;
	}
	//right

	if (isLegalPosition(row, col + 1))
	{
		piece = gameBoard.board[row][col + 1].getPiece();
		if (piece == 0) //empty cell
			return true;
	}
	//up neighbor

	if (isLegalPosition(row - 1, col))
	{
		piece = gameBoard.board[row - 1][col].getPiece();
		if (piece == 0) //empty cell
			return true;
	}
	//left

	if (isLegalPosition(row, col - 1))
	{
		piece = gameBoard.board[row][col - 1].getPiece();
		if (piece == 0) //empty cell
			return true;
	}
	return false; //no empty cell
}

void AutoPlayerAlgorithm::positionBombs(int flagRow, int flagCol, int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill, int bombsToPosition, bool shouldPositionRandomly)
{
	bool isCellTaken = true;
	bool hasOpponentNeighbor = true;
	char piece = (player == 1 ? 'B' : 'b');
	int row;
	int col;
	int bombsPositioned = 0;

	if (!shouldPositionRandomly)
	{
		for (int i = 0; i < bombsToPosition; ++i)
		{
			if (findEmptyNeigbor(row, col, flagRow, flagCol))
			{
				Cell::updateCell(gameBoard.board[row][col], piece, false);
				vectorToFill.push_back = (make_unique<RPSPiecePosition>(RPSpoint(row, col), piece, '#'));
				bombsPositioned++;
			}
			else
			{
				shouldPositionRandomly = true;
			}
		}
	}

	bombsToPosition = bombsToPosition - bombsPositioned;

	positionPiecesRandomly(bombsToPosition, piece, false, vectorToFill);
}

void AutoPlayerAlgorithm::positionPiecesRandomly(int pieceNum, char piece, bool isJoker, std::vector<unique_ptr<PiecePosition>> &vectorToFill)
{
	bool isCellTaken = true;
	int col = getRandomNumInRange(0, COLS - 1);
	int row = getRandomNumInRange(0, ROWS - 1);
	for (int i = 0; i < pieceNum; ++i)
	{
		while (isCellTaken)
		{
			if (gameBoard.board[row][col].getPiece() == 0)
			{
				isCellTaken = false;
				Cell::updateCell(gameBoard.board[row][col], piece, isJoker);
				vectorToFill.push_back(make_unique<RPSPiecePosition>(RPSpoint(row, col), piece, '#'));
				break;
			}
			col = getRandomNumInRange(0, COLS - 1);
			row = getRandomNumInRange(0, ROWS - 1);
		}
		isCellTaken = true;
	}
}

bool isOpponentPiece(int myPlayerNum, char piece)
{
	if (myPlayerNum == 1)
	{
		return islower(piece);
	}
	else
	{
		return isupper(piece);
	}
}

/*
Input  - relevant col and row
Output - true if this is a legal position and  false otherwise 
*/
// legal position on board starts from 0 to number of rows/cols - 1
bool AutoPlayerAlgorithm::isLegalPosition(int row, int col)
{
	if (row > ROWS - 1 || col > COLS - 1 || row < 0 || col < 0)
	{
		return false;
	}
	return true;
}
/*
Input - current player num, row and col 
Output - true if an opponent nieghbour is located there and otherwise false 
*/
bool AutoPlayerAlgorithm::checkIsOpponentNeighbors(int player, int row, int col)
{
	//down neigbor
	char piece = gameBoard.board[row + 1][col].getPiece();
	if (isLegalPosition(row + 1, col) && isOpponentPiece(player, piece))
	{
		return true;
	}
	//right
	piece = gameBoard.board[row][col + 1].getPiece();
	if (isLegalPosition(row, col + 1) && isOpponentPiece(player, piece))
	{
		return true;
	}
	//up neighbor
	piece = gameBoard.board[row - 1][col].getPiece();
	if (isLegalPosition(row - 1, col) && isOpponentPiece(player, piece))
	{
		return true;
	}
	//left
	piece = gameBoard.board[row][col - 1].getPiece();
	if (isLegalPosition(row, col - 1) && isOpponentPiece(player, piece))
	{
		return true;
	}
	return false;
}
/*
update algorithm about initial board and fight results
*/
void AutoPlayerAlgorithm::notifyOnInitialBoard(const Board &b,
											   const std::vector<unique_ptr<FightInfo>> &fights)
{
	updateBoard(b);
	for (int i = 0; i < fights.size(); ++i)
	{
		int x = fights[i]->getPosition().getX();
		int y = fights[i]->getPosition().getY();
		char myPiece = toupper(fights[i]->getPiece(myPlayerNum));
		char opponentPiece = toupper(fights[i]->getPiece(myPlayerNum == 1 ? 2 : 1));
		int winner = fights[i]->getWinner();
		if (winner == 0)
		{
			if (opponentPiece == 'B')
				opponentBombsNumOnBoard--;
			else
				opponentMovingPieceNumOnBoard--;
		}
		else if (winner != myPlayerNum)
		{ //opponent won
			//not a flag - game was not over
			gameBoard.board[x][y].flagProbability = 0;
			if (opponentPiece == 'B')
				opponentBombsNumOnBoard--;
			//if it is not a bomb and not a flag - than it is moving piece
			gameBoard.board[x][y].isMovingPiece = true;
			//check if piece was known before and changed.
			bool isJoker = (gameBoard.board[x][y].getPiece() != 0 && gameBoard.board[x][y].getPiece() != opponentPiece);
			if (isJoker)
				gameBoard.board[x][y].isJokerKnown = true;
			AICell::updateCell(gameBoard.board[x][y], opponentPiece, isJoker);
		}
		else
		{ //i won
			//if i won so opponent piece must not be a bomb or flag(since game was not over)
			opponentMovingPieceNumOnBoard--;
		}

		updateFlagProbability();
	}
}

void AutoPlayerAlgorithm::updateFlagProbability()
{
	int unkownPiecesNum = 0;
	for (int i = 0; i < ROWS; ++i)
	{
		for (int j = 0; j < COLS; ++j)
		{
			if (gameBoard.board[i][j].flagProbability != 0)
			{
				unkownPiecesNum++; //count pieces that we dont know if there are flags or not
			}
		}
	}
	//update probability
	for (int i = 0; i < ROWS; ++i)
	{
		for (int j = 0; j < COLS; ++j)
		{
			if (gameBoard.board[i][j].flagProbability != 0)
			{
				gameBoard.board[i][j].flagProbability = FLAGS_NUM / unkownPiecesNum;
			}
		}
	}
}

unique_ptr<Move> AutoPlayerAlgorithm::getMove()
{
	int from_x;
	int from_y;
	int to_x;
	int to_y;
	getBestMove(from_x, from_y, to_x, to_y);
	unique_ptr<Move> move = make_unique<RPSMove>(RPSpoint(from_x, from_y), RPSpoint(to_x, to_y));
}

unique_ptr<JokerChange> AutoPlayerAlgorithm::getJokerChange()
{
	int joker_x;
	int joker_y;
	char newRep = -1;
	double score = INT_MIN;
	for (int i = 0; i < ROWS; ++i)
	{
		for (int j = 0; j < COLS; ++j)
		{
			if (gameBoard.board[i][j].getPiece() == 0)
				continue;
			bool isMyPiece = (AICell::isPlayerOnePiece(gameBoard.board[i][j]) && (myPlayerNum == 1)) || (!AICell::isPlayerOnePiece(gameBoard.board[i][j]) && (myPlayerNum != 1));
			if (gameBoard.board[i][j].getIsJoker() && isMyPiece)
			{
				newRep = shouldChangeJoker(score, i, j, myPlayerNum == 1);
				joker_x = i;
				joker_y = j;
			}
		}
	}
	//no joker change needed
	if (newRep == -1)
		return nullptr;

	unique_ptr<JokerChange> jokerChange = make_unique<RPSJokerChange>(newRep, RPSpoint(joker_x, joker_y));
	return jokerChange;
}

void AutoPlayerAlgorithm::getBestMove(int &fromRow, int &fromCol, int &toRow, int &toCol)
{
	bool isMyPiece = (myPlayerNum == 1);
	double score;
	int toRow;
	int toCol;
	int maxScore = INT_MIN;
	for (int i = 0; i < ROWS; ++i)
	{
		for (int j = 0; j < COLS; ++j)
		{
			if (gameBoard.board[i][j].getPiece() == 0)
				continue;
			isMyPiece = Cell::isPlayerOnePiece(gameBoard.board[i][j]) ? (myPlayerNum == 1) : (myPlayerNum == 2);
			score = getBestMoveForPiece(maxScore, i, j, toRow, toCol);
			if (maxScore < score)
			{
				maxScore = score;
				fromRow = i;
				fromCol = j;
			}
		}
	}

	//make move
	if (gameBoard.board[toRow][toCol].getPiece == 0)
	{
		Cell::updateCell(gameBoard.board[toRow][toCol], gameBoard.board[fromRow][fromCol].getPiece(),
						 gameBoard.board[fromRow][fromCol].getIsJoker());
		AICell::updateCellKnowlage(gameBoard.board[toRow][toCol], gameBoard.board[fromRow][fromCol]);
	}
	else
	{ //there is going to be a fight
		myCell = gameBoard.board[fromRow][fromCol];
		opponentCell = gameBoard.board[toCol][toCol];
	}
}

double AutoPlayerAlgorithm::getBestMoveForPiece(double score, const int &fromRow, const int &fromCol, int &toRow, int &toCol)
{
	int row = toRow;
	int col = toCol;
	int currScore;

	//move right
	unique_ptr<Move> move = make_unique<RPSMove>(RPSpoint(fromRow, fromCol), RPSpoint(row, col + 1));
	if (RPSMove::isLegalMove(gameBoard, move, myPlayerNum == 1))
	{
		Cell prevCell = gameBoard.board[fromRow][fromCol];
		currScore = tryMovePiece(move, myPlayerNum == 1);
		if (currScore > score)
		{
			score = currScore;
			toRow = row;
			toCol = col + 1;
		}
	}

	//move left
	unique_ptr<Move> move = make_unique<RPSMove>(RPSpoint(fromRow, fromCol), RPSpoint(row, col - 1));
	if (RPSMove::isLegalMove(gameBoard, move, myPlayerNum == 1))
	{
		Cell prevCell = gameBoard.board[fromRow][fromCol];
		currScore = tryMovePiece(move, myPlayerNum == 1);
		if (currScore > score)
		{
			score = currScore;
			toRow = row;
			toCol = col - 1;
		}
	}

	//move up
	unique_ptr<Move> move = make_unique<RPSMove>(RPSpoint(fromRow, fromCol), RPSpoint(row - 1, col));
	if (RPSMove::isLegalMove(gameBoard, move, myPlayerNum == 1))
	{
		Cell prevCell = gameBoard.board[fromRow][fromCol];
		currScore = tryMovePiece(move, myPlayerNum == 1);
		if (currScore > score)
		{
			score = currScore;
			toRow = row - 1;
			toCol = col;
		}
	}

	//move down
	unique_ptr<Move> move = make_unique<RPSMove>(RPSpoint(fromRow, fromCol), RPSpoint(row + 1, col));
	if (RPSMove::isLegalMove(gameBoard, move, myPlayerNum == 1))
	{

		currScore = tryMovePiece(move, myPlayerNum == 1);
		if (currScore > score)
		{
			score = currScore;
			toRow = row + 1;
			toCol = col;
		}
	}

	return score;
}

//returns true if gameOver, false otherwise
double AutoPlayerAlgorithm::tryMovePiece(unique_ptr<Move> &move, bool isPlayerOneTurn)
{
	int from_x = move->getFrom().getX();
	int from_y = move->getFrom().getY();
	int to_x = move->getTo().getX();
	int to_y = move->getTo().getY();
	myCell = gameBoard.board[from_x][from_y];
	opponentCell = gameBoard.board[to_x][to_y];
	double discovery = 0;
	double material = calcMaterial(gameBoard.board[from_x][from_y]);
	double reveal = -1 / 2;

	if (gameBoard.board[to_x][to_y].getPiece() == 0)
	{
		Cell::updateCell(gameBoard.board[to_x][to_y], gameBoard.board[from_x][from_y].getPiece(),
						 gameBoard.board[from_x][from_y].getIsJoker());
		Cell::cleanCell(gameBoard.board[from_x][from_y]);
	}
	else
	{
		reveal = -1;
		discovery = calcDiscovery(gameBoard.board[to_x][to_y]);
		bool lostPieceInFight = tryToFight(to_x, to_y,
										   gameBoard.board[from_x][from_y].getPiece(),
										   gameBoard.board[from_x][from_y].getIsJoker());
		if (lostPieceInFight)
		{
			material *= -1;
		}
		Cell::cleanCell(gameBoard.board[from_x][from_y]);
	}
	double score = calcScore(material, discovery, reveal);
	//return board to be as it was
	AICell::updateCell(gameBoard.board[from_x][from_y], myCell.getPiece(),
					   myCell.getIsJoker());
	AICell::updateCell(gameBoard.board[to_x][to_y], opponentCell.getPiece(),
					   opponentCell.getIsJoker());
	AICell::updateCellKnowlage(gameBoard.board[from_x][from_y], myCell);
	AICell::updateCellKnowlage(gameBoard.board[to_x][to_y], opponentCell);
	return score;
}

bool AutoPlayerAlgorithm::tryToFight(int to_x, int to_y, char myPiece, bool isJoker)
{
	char opponentPiece = gameBoard.board[to_x][to_y].getPiece;
	if (opponentPiece != '#')
	{ //we know the opponent piece so we can simulate fight normally
		return fight(to_x, to_y, myPiece, opponentPiece, isJoker);
	}
	else
	{
		int prob = getRandomNumInRange(1, 100);
		if (prob < 50)
		{ //guess i will lose in fight
			if (gameBoard.board[to_x][to_y].isMovingPieceKnown && !gameBoard.board[to_x][to_y].isMovingPiece)
			{
				AICell::cleanCell(gameBoard.board[to_x][to_y]);
			}
			return true; //lose
		}
		else
		{ //guess i won
			AICell::updateCell(gameBoard.board[to_x][to_y], myPiece,isJoker);
			return false; //win
		}
	}
}

bool AutoPlayerAlgorithm::fight(int row, int col, char myPiece, char opponentPiece, bool isMyPieceJoker)
{

	//Case 1: 2 players in the same type - tie
	if (myPiece == opponentPiece)
	{
		Cell::cleanCell(gameBoard.board[row][col]);
		return false;
	}

	//Case 2: there is flag and current player has another piece
	else if (opponentPiece == FLAG)
	{
		Cell::updateCell(gameBoard.board[row][col], myPiece, isMyPieceJoker);
		return true;
	}

	//case 3: there is bomb and current player has another piece
	else if (opponentPiece == BOMB || myPiece == BOMB)
	{
		Cell::cleanCell(gameBoard.board[row][col]);
		return false;
	}

	//case 4: there is PAPER and current player has another piece
	else if (opponentPiece == PAPER)
	{
		if (myPiece == ROCK)
		{
			return false;
		}
		else if (myPiece == SCISSOR)
		{
			Cell::updateCell(gameBoard.board[row][col], myPiece,
							 isMyPieceJoker);
			return true;
		}
	}
	//case 7: player 1 is ROCK and player 2 another piece
	else if (opponentPiece == ROCK)
	{
		if (myPiece == PAPER)
		{
			Cell::updateCell(gameBoard.board[row][col], myPiece,
							 isMyPieceJoker);
			return true;
		}
		else if (myPiece == SCISSOR)
		{
			return false;
		}
	}

	else if (opponentPiece == SCISSOR)
	{
		if (myPiece == PAPER)
		{
			return false;
		}
		else if (myPiece == ROCK)
		{
			Cell::updateCell(gameBoard.board[row][col], myPiece,
							 isMyPieceJoker);
			return true;
		}
	}
}

double AutoPlayerAlgorithm::calcScore(double material, double discovery, double reveal)
{
	double flagSaftey = calcFlagSaftey();
	double distanceFromBombOrFlag = calcDistanceFromBombOrFlag();
	double distanceFromUnknownPiece = calcDistanceFromUnknownPiece();
	return MATERIAL_WEIGHT * material + DISCOVERY_WEIGHT * discovery + REVEAL_WEIGHT * reveal +
		   FLAG_SAFTEY_WEUGHT * flagSaftey + DISTANCE_FROM_FLAG_WEIGHT * distanceFromBombOrFlag +
		   DISTANCE_FROM_UNKNOWN_WEIGHT * distanceFromUnknownPiece;
}

void notifyFightResult(const FightInfo &fightInfo)
{
	RPSpoint p = fightInfo.getPosition();
	char myPiece = toupper(fightInfo.getPiece(myPlayerNum));
	char opponentPiece = toupper(fightInfo.getPiece(myPlayerNum == 1 ? 2 : 1));
	int winner = fightInfo.getWinner();
	if (winner == 0)
	{
		if (opponentPiece == 'B')
			bombsNumOnBoard--;
		else
			movingPieceNumOnBoard--;
		Cell::cleanCell(gameBoard.board[p.getX()][p.getY()]);
	}
	else if (winner != myPlayerNum)
	{   //opponent won
		//not a flag - game was not over
		gameBoard.board[p.getX()][p.getY()].flagProbabilty = 0;
		if (opponentPiece == 'B')
		{
			bombsNumOnBoard--;
			Cell::cleanCell(gameBoard.board[p.getX()][p.getY()]);
		}
		//if it is not a bomb and not a flag - than it is moving piece
		gameBoard.board[p.getX()][p.getY()].isMovingPieceKnown = true;
		gameBoard.board[p.getX()][p.getY()].isMovingPiece = true;
		//check if piece was known before and changed.
		bool isJoker = (gameBoard.board[p.getX()][p.getY()].getPiece() != 0 && gameBoard.board[p.getX()][p.getY()].getPiece() != opponentPiece);
		if (isJoker)
			gameBoard.board[p.getX()][p.getY()].isJokerKnown = true;
		AICell::updateCell(gameBoard.board[p.getX()][p.getY()], opponentPiece, isJoker);
	}
	else
	{   //i won
		//if i won so opponent piece must not be a bomb or flag(since game was not over)
		movingPieceNumOnBoard--;
		if (toupper(myPiece) == 'B')
		{
			Cell::cleanCell(gameBoard.board[p.getX()][p.getY()]);
		}
		else
		{ // update win
			Cell::updateCell(gameBoard.board[p.getX()][p.getY()], myCell.getPiece(), myCell.getIsJoker());
			AICell::updateKnowladge(gameBoard.board[p.getX()][p.getY()], myCell);
		}
	}

	if (movingPieceNumOnBoard == 0)
	{ //we can mark all pieces as not moving pieces !!!
		updateMovingPiece();
	}

	updateFlagProbability();
}

double AutoPlayerAlgorithm::calcDistanceFromBombOrFlag()
{
	bool amIPlayerOne = (myPlayerNum == 1);
	int distance;
	int minimalDistance = INT_MAX;
	//find bomb or flag
	for (int i = 0; i < ROWS; ++i)
	{
		for (int j = 0; j < COLS; ++j)
		{
			char piece = gameBoard.board[i][j].getPiece();
			if (piece == 0)
				continue;
			bool isMyPiece = (piece == '#') || (amIPlayerOne ? isupper(piece) : islower(piece));
			if (!isMyPiece)
			{
				if (gameBoard.board[i][j].isMovingPieceKnown() && !gameBoard.board[i][j].isMovingPiece())
				{
					distance = calcDistanceFromPiece(i, j, amIPlayerOne);
					if (minimalDistance > distance)
						minimalDistance = distance;
				}
			}
		}
	}

	return (1 - minimalDistance) / (ROWS + COLS);
}

double AutoPlayerAlgorithm::calcDistanceFromUnkownPiece()
{
	bool amIPlayerOne = (myPlayerNum == 1);
	int distance;
	int minimalDistance = INT_MAX;
	//find bomb or flag
	for (int i = 0; i < ROWS; ++i)
	{
		for (int j = 0; j < COLS; ++j)
		{
			char piece = gameBoard.board[i][j].getPiece();
			if (piece == 0)
				continue;
			bool isMyPiece = (piece == '#') || (amIPlayerOne ? isupper(piece) : islower(piece));
			if (!isMyPiece)
			{
				if (!gameBoard.board[i][j].isMovingPieceKnown())
				{
					distance = calcDistanceFromPiece(i, j, amIPlayerOne);
					if (minimalDistance > distance)
						minimalDistance = distance;
				}
			}
		}
	}

	return (1 - minimalDistance) / (ROWS + COLS);
}

int AutoPlayerAlgorithm::calcDistanceFromPiece(int piece_x, int piece_y, bool amIPlayerOne)
{
	int distance;
	int minimalDistance = INT_MAX;

	for (int i = 0; i < ROWS; ++i)
	{
		for (int j = 0; j < COLS; ++j)
		{
			char piece = gameBoard.board[i][j].getPiece();
			if (piece == 0 || piece == '#')
				continue;
			bool isMyPiece = (amIPlayerOne ? isupper(piece) : islower(piece));
			if (isMyPiece)
			{
				distance = abs(i - piece_x) + abs(j - piece_y);
				if (minimalDistance > distance)
					minimalDistance = distance;
			}
		}
	}

	return distance;
}

double AutoPlayerAlgorithm::calcFlagSaftey()
{
	int protectingBombs = 0;
	int otherProtectingPieces = 0;
	int enemyPieces = 0;
	int bombs = 0;
	int movingPieces = 0;
	int totalEnemyPieces;
	for (int i = 0; i < ROWS; ++i)
	{
		for (int j = 0; j < ROWS; ++j)
		{
			if ((!gameBoard.board[i][j].isPlayerOnePiece() && amIPlayerOne) || (gameBoard.board[i][j].isPlayerOnePiece() && !amIPlayerOne))
			{
				if (gameBoard.board[i][j].getPiece() != 0)
					enemyPieces++;
			}
			char piece = toupper(gameBoard.board[i][j].getPiece());
			if (piece == 'F')
			{
				countProtectingPieces(i, j, protectingBombs, otherProtectingPieces, enemyPieces, myPlayerNum == 1);
			}
			else if (piece == 'B')
			{
				bombs++;
			}
			else
			{
				movingPieces++;
			}
		}
	}

	if (enemyPieces > 0)
		return (-1 * enemyPieces / totalEnemyPieces);
	else
		return (2 * protectingBombs + otherProtectingPieces) / 2 * bombs + movingPieces;
}

void AutoPlayerAlgorithm::countProtectingPieces(i, j, protectingBombs, otherProtectingPieces, enemyPieces, amIplayerOne)
{
	//searchRight
	bool isMyPiece;
	char piece = j + 1 > COLS - 1 ? 0 : gameBoard.board[i][j + 1].getPiece();
	if (piece != 0)
	{
		isMyPiece = gameBoard.board[i][j + 1].isPlayerOnePiece() == amIplayerOne;

		if (toupper(piece) == 'B')
		{
			if (isMyPiece)
			{
				protectingBombs++;
			}
		}
		else if (toupper(piece) != 'F')
		{
			if (isMyPiece)
				otherProtectingPieces++;
			else
				enemyPieces++; //enemies piece is next to flag
		}
	}
	//searchLeft
	bool isMyPiece;
	char piece = j - 1 < COLS - 1 ? 0 : gameBoard.board[i][j - 1].getPiece();
	if (piece != 0)
	{
		isMyPiece = gameBoard.board[i][j - 1].isPlayerOnePiece() == amIplayerOne;

		if (toupper(piece) == 'B')
		{
			if (isMyPiece)
			{
				protectingBombs++;
			}
		}
		else if (toupper(piece) != 'F')
		{
			if (isMyPiece)
				otherProtectingPieces++;
			else
				enemyPieces++; //enemies piece is next to flag
		}
	}
	//searchUp
	bool isMyPiece;
	char piece = i - 1 < ROWS - 1 ? 0 : gameBoard.board[i - 1][j].getPiece();
	if (piece != 0)
	{
		isMyPiece = gameBoard.board[i - 1][j].isPlayerOnePiece() == amIplayerOne;

		if (toupper(piece) == 'B')
		{
			if (isMyPiece)
			{
				protectingBombs++;
			}
		}
		else if (toupper(piece) != 'F')
		{
			if (isMyPiece)
				otherProtectingPieces++;
			else
				enemyPieces++; //enemies piece is next to flag
		}
	}
	//searchDown
	bool isMyPiece;
	char piece = i + 1 > ROWS ? 0 : gameBoard.board[i + 1][j].getPiece();
	if (piece != 0)
	{
		isMyPiece = gameBoard.board[i + 1][j].isPlayerOnePiece() == amIplayerOne;

		if (toupper(piece) == 'B')
		{
			if (isMyPiece)
			{
				protectingBombs++;
			}
		}
		else if (toupper(piece) != 'F')
		{
			if (isMyPiece)
				otherProtectingPieces++;
			else
				enemyPieces++; //enemies piece is next to flag
		}
	}
	//SearchDiagonal
	bool isMyPiece;
	char piece = i - 1 < ROWS - 1 || j - 1 < COLS - 1 ? 0 : gameBoard.board[i - 1][j - 1].getPiece();
	if (piece != 0)
	{
		isMyPiece = gameBoard.board[i - 1][j - 1].isPlayerOnePiece() == amIplayerOne;

		if (toupper(piece) == 'B')
		{
			if (isMyPiece)
			{
				protectingBombs++;
			}
		}
		else if (toupper(piece) != 'F')
		{
			if (isMyPiece)
				otherProtectingPieces++;
			else
				enemyPieces++; //enemies piece is next to flag
		}
	}

	bool isMyPiece;
	char piece = i + 1 > ROWS - 1 || j - 1 < COLS - 1 ? 0 : gameBoard.board[i + 1][j - 1].getPiece();
	if (piece != 0)
	{
		isMyPiece = gameBoard.board[i + 1][j - 1].isPlayerOnePiece() == amIplayerOne;

		if (toupper(piece) == 'B')
		{
			if (isMyPiece)
			{
				protectingBombs++;
			}
		}
		else if (toupper(piece) != 'F')
		{
			if (isMyPiece)
				otherProtectingPieces++;
			else
				enemyPieces++; //enemies piece is next to flag
		}
	}

	bool isMyPiece;
	char piece = i - 1 < ROWS - 1 || j + 1 > COLS - 1 ? 0 : gameBoard.board[i - 1][j + 1].getPiece();
	if (piece != 0)
	{
		isMyPiece = gameBoard.board[i - 1][j + 1].isPlayerOnePiece() == amIplayerOne;

		if (toupper(piece) == 'B')
		{
			if (isMyPiece)
			{
				protectingBombs++;
			}
		}
		else if (toupper(piece) != 'F')
		{
			if (isMyPiece)
				otherProtectingPieces++;
			else
				enemyPieces++; //enemies piece is next to flag
		}
	}

	bool isMyPiece;
	char piece = i + 1 > ROWS - 1 || j + 1 > COLS - 1 ? 0 : gameBoard.board[i + 1][j + 1].getPiece();
	if (piece != 0)
	{
		isMyPiece = gameBoard.board[i + 1][j + 1].isPlayerOnePiece() == amIplayerOne;

		if (toupper(piece) == 'B')
		{
			if (isMyPiece)
			{
				protectingBombs++;
			}
		}
		else if (toupper(piece) != 'F')
		{
			if (isMyPiece)
				otherProtectingPieces++;
			else
				enemyPieces++; //enemies piece is next to flag
		}
	}
}

double AutoPlayerAlgorithm::calcMaterial(Cell cell)
{
	if (cell.getIsJoker())
	{
		return 1 - (9 / 100);
	}
	switch (toupper(cell.getPiece()))
	{
	case 'R':
		return 1 - (ROCK_SCORE / 100);
	case 'P':
		return 1 - (ROCK_SCORE / 100);
	case 'S':
		return 1 - (ROCK_SCORE / 100);
	}
}

double AutoPlayerAlgorithm::calcDiscovery(AICell cell)
{
	int isFlagKnown = cell.flagProbabilty == 0 ? 0 : 1;
	int isJokerKnown = cell.isJokerKnown ? 0 : 1;
	int isMovingPiece = cell.isMovingPiece ? 0 : 1;

	return (isFlagKnown + isJokerKnown + isMovingPiece) / 3;
}

char AutoPlayerAlgorithm::shouldChangeJoker(double &score, int joker_x, int joker_y, bool amIPlayerOne)
{
	char oldRep = gameBoard.board[x_joker][y_joker].getPiece();
	char origRep = oldRep;
	char newRep;
	char bestRep = -1;
	int currScore;

	for (int i = 0; i < 4; ++i)
	{
		switch (i)
		{
		case 0:
			if (newRep != origRep)
				newRep = amIPlayerOne ? 'R' : 'r';
			break;
		case 1:
			if (newRep != origRep)
				newRep = amIPlayerOne ? 'P' : 'p';
			break;
		case 2:
			if (newRep != origRep)
				newRep = amIPlayerOne ? 'S' : 's';
			break;
		case 3:
			if (newRep != origRep)
				newRep = amIPlayerOne ? 'B' : 'b';
			break;
		}

		Cell::updateCell(gameBoard.board[x_joker][y_joker],
						 newRep, true);
		currScore = calcScore(x_joker, y_joker);

		if (currScore > score)
		{
			score = currScore;
			bestRep = newRep;
		}
		else
		{
			//change back to old rep
			Cell::updateCell(gameBoard.board[x_joker][y_joker],
							 oldRep, true);
		}
	}

	return bestRep;
}

void AutoPlayerAlgorithm::updateMovingPiece()
{
	for (int i = 0; i < ROWS; ++i)
	{
		for (int j = 0; j < COLS; ++j)
		{
			if (gameBoard.board[i][j].getPiece() == 0) //empty cell
				continue;
			int piecePlayerNum = gameBoard.board[i][j].isPlayerOnePiece() ? 1 : 2;
			if (gameBoard.board[i][j].getPiece() == '#' || myPlayerNum != piecePlayerNum)
			{ //this is opponent's piece
				if (gameBoard.board[i][j].isMovingPieceKnown && gameBoard.board[i][j].isMovingPiece)
				{
					continue;
				}
				gameBoard.board[i][j].isMovingPieceKnown = true;
				gameBoard.board[i][j].isMovingPiece = false;
			}
		}
	}
	//TODO: update flag probabilty
}

void AutoPlayerAlgorithm::updateBoard(const Board &b)
{
	for (int i = 0; i < ROWS; ++i)
	{
		for (int j = 0; j < COLS; ++j)
		{
			RPSpoint p = RPSpoint(i, j);
			if (b.getPlayer(p) != 0)
			{ //not empty - must be opponent piece
				Cell::updateCell(gameBoard.board[i][j], '?', false);
			}
		}
	}
}

void AutoPlayerAlgorithm::notifyOnOpponentMove(const Move &move)
{
	RPSpoint from = move.getFrom();
	RPSpoint to = move.getTo();
	AICell fromCell = gameBoard.board[from.getX()][from.getY()];
	AICell::updateCell(gameBoard.board[from.getX()][from.getY()], fromCell.getPiece(), fromCell.getIsJoker());
	AICell::updateCellKnowlage(gameBoard.board[from.getX()][from.getY()], fromCell);
	AICell::clearCell(gameBoard.board[from.getX()][from.getY()]);
}
