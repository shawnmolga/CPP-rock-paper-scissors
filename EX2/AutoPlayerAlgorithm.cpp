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
	//position all flags on board
	for (int i = 0; i < FLAGS_NUM; ++i)
	{
		while (isCellTaken || hasOpponentNeighbor)
		{
			if (gameBoard.board[row][col].getPiece() == 0)
			{
				isCellTaken = false;
				if (!checkIsOpponentNeighbors(player, row, col))
				{
					hasOpponentNeighbor = false;
					AICell::updateCell(gameBoard.board[row][col], piece, false);
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

bool AutoPlayerAlgorithm::findEmptyNeigbor(int &row, int &col, int flagRow, int flagCol)
{
	
	char piece;
	row = flagRow;
	col = flagCol;
	//down neigbor
	if (isLegalPosition(row + 1, col))
	{
		piece = gameBoard.board[row + 1][col].getPiece();
		if (piece == 0){ //empty cell
			row = row + 1;
			return true;
		}
	}
	//right

	if (isLegalPosition(row, col + 1))
	{
		piece = gameBoard.board[row][col + 1].getPiece();
		if (piece == 0) {//empty cell
			col = col + 1;
			return true;
		}
	}
	//up neighbor

	if (isLegalPosition(row - 1, col))
	{
		piece = gameBoard.board[row - 1][col].getPiece();
		if (piece == 0) {//empty cell
			row = row - 1;
			return true;
		}
	}
	//left

	if (isLegalPosition(row, col - 1))
	{
		piece = gameBoard.board[row][col - 1].getPiece();
		if (piece == 0) {//empty cell
			col = col -1;
			return true;
		}
	}
	return false; //no empty cell
}

void AutoPlayerAlgorithm::positionBombs(int flagRow, int flagCol, int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill, int bombsToPosition, bool shouldPositionRandomly)
{
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
				vectorToFill.push_back(make_unique<RPSPiecePosition>(RPSpoint(row, col), piece, '#'));
				bombsPositioned++;
			}
			else
			{
				shouldPositionRandomly = true;
				break;
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

bool AutoPlayerAlgorithm::isOpponentPiece(int myPlayerNum, char piece)
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
	for (int i = 0; i < (int)fights.size(); ++i)
	{
		int x = fights[i]->getPosition().getX();
		int y = fights[i]->getPosition().getY();
		//char myPiece = toupper(fights[i]->getPiece(myPlayerNum)); //do we need this?
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
	return move;
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
	//bool isMyPiece = (myPlayerNum == 1);
	double score;
	int maxScore = INT_MIN;
	for (int i = 0; i < ROWS; ++i)
	{
		for (int j = 0; j < COLS; ++j)
		{
			if (gameBoard.board[i][j].getPiece() == 0 || gameBoard.board[i][j].getPiece() == '#')
				continue;
			bool isMyPiece = Cell::isPlayerOnePiece(gameBoard.board[i][j]) ? (myPlayerNum == 1) : (myPlayerNum == 2);
			if (!isMyPiece) continue;
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
	if (gameBoard.board[toRow][toCol].getPiece() == 0)
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
	AICell::cleanCell(gameBoard.board[fromRow][fromCol]);
}

double AutoPlayerAlgorithm::getBestMoveForPiece(double score, const int &fromRow, const int &fromCol, int &toRow, int &toCol)
{
	int row = toRow;
	int col = toCol;
	int currScore;

	//move right
	unique_ptr<Move> move = make_unique<RPSMove>(RPSpoint(fromRow, fromCol), RPSpoint(row, col + 1));
	if (isLegalMove(move, myPlayerNum == 1))
	{
		// Cell prevCell = gameBoard.board[fromRow][fromCol];
		currScore = tryMovePiece(move);
		if (currScore > score)
		{
			score = currScore;
			toRow = row;
			toCol = col + 1;
		}
	}

	//move left
	move = make_unique<RPSMove>(RPSpoint(fromRow, fromCol), RPSpoint(row, col - 1));
	if (isLegalMove(move, myPlayerNum == 1))
	{
		//Cell prevCell = gameBoard.board[fromRow][fromCol];
		currScore = tryMovePiece(move);
		if (currScore > score)
		{
			score = currScore;
			toRow = row;
			toCol = col - 1;
		}
	}

	//move up
	move = make_unique<RPSMove>(RPSpoint(fromRow, fromCol), RPSpoint(row - 1, col));
	if (isLegalMove(move, myPlayerNum == 1))
	{
		// Cell prevCell = gameBoard.board[fromRow][fromCol];
		currScore = tryMovePiece(move);
		if (currScore > score)
		{
			score = currScore;
			toRow = row - 1;
			toCol = col;
		}
	}

	//move down
    move = make_unique<RPSMove>(RPSpoint(fromRow, fromCol), RPSpoint(row + 1, col));
	if (isLegalMove(move, myPlayerNum == 1))
	{

		currScore = tryMovePiece(move);
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
double AutoPlayerAlgorithm::tryMovePiece(unique_ptr<Move> &move)
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
	char opponentPiece = gameBoard.board[to_x][to_y].getPiece();
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

//false if i lost and true if i win (tie is false)
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
	cout<<"fight false !"<<endl;
	return false; //shouldnt get here
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

void AutoPlayerAlgorithm::notifyFightResult(const FightInfo &fightInfo)
{
	int x = fightInfo.getPosition().getX();
	int y = fightInfo.getPosition().getY();
	char myPiece = toupper(fightInfo.getPiece(myPlayerNum));
	char opponentPiece = toupper(fightInfo.getPiece(myPlayerNum == 1 ? 2 : 1));
	int winner = fightInfo.getWinner();
	if (winner == 0)
	{
		if (opponentPiece == 'B')
			opponentBombsNumOnBoard--;
		else
			opponentMovingPieceNumOnBoard--;
		Cell::cleanCell(gameBoard.board[x][y]);
	}
	else if (winner != myPlayerNum)
	{   //opponent won
		//not a flag - game was not over
		gameBoard.board[x][y].flagProbability = 0;
		if (opponentPiece == 'B')
		{
			opponentBombsNumOnBoard--;
			Cell::cleanCell(gameBoard.board[x][y]);
		}
		//if it is not a bomb and not a flag - than it is moving piece
		gameBoard.board[x][y].isMovingPieceKnown = true;
		gameBoard.board[x][y].isMovingPiece = true;
		//check if piece was known before and changed.
		bool isJoker = (gameBoard.board[x][y].getPiece() != 0 && gameBoard.board[x][y].getPiece() != opponentPiece);
		if (isJoker)
			gameBoard.board[x][y].isJokerKnown = true;
		AICell::updateCell(gameBoard.board[x][y], opponentPiece, isJoker);
		AICell::updateCellKnowlage(gameBoard.board[x][y], opponentCell);
	}
	else
	{   //i won
		//if i won so opponent piece must not be a bomb or flag(since game was not over)
		opponentMovingPieceNumOnBoard--;
		if (toupper(myPiece) == 'B')
		{
			Cell::cleanCell(gameBoard.board[x][y]);
		}
		else
		{ // update win
			Cell::updateCell(gameBoard.board[x][y], myCell.getPiece(), myCell.getIsJoker());
		}
	}

	if (opponentMovingPieceNumOnBoard == 0)
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
				if (gameBoard.board[i][j].isMovingPieceKnown && !gameBoard.board[i][j].isMovingPiece)
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

double AutoPlayerAlgorithm::calcDistanceFromUnknownPiece()
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
				if (!gameBoard.board[i][j].isMovingPieceKnown)
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
	int totalEnemyPieces = 0;
	bool amIPlayerOne = myPlayerNum == 1;
	for (int i = 0; i < ROWS; ++i)
	{
		for (int j = 0; j < ROWS; ++j)
		{
			if ((!AICell::isPlayerOnePiece(gameBoard.board[i][j]) && amIPlayerOne) || (gameBoard.board[i][j].isPlayerOnePiece && !amIPlayerOne))
			{
				if (gameBoard.board[i][j].getPiece() != 0)
					totalEnemyPieces++;
				continue;
			}
			//else - this is my piece
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
	return (2 * protectingBombs + otherProtectingPieces) / 2 * bombs + movingPieces;
}
void AutoPlayerAlgorithm::countProtectingPieces(int i, int j, int &protectingBombs, int &otherProtectingPieces, int &enemyPieces, bool amIplayerOne)

{
	//searchRight
	bool isMyPiece;
	char piece = j + 1 > COLS - 1 ? 0 : gameBoard.board[i][j + 1].getPiece();
	if (piece != 0)
	{
		isMyPiece = AICell::isPlayerOnePiece(gameBoard.board[i][j + 1]) == amIplayerOne;

		if (toupper(piece) == 'B')
		{
			if (isMyPiece)
			{
				protectingBombs++;
			}
			else
				enemyPieces++; //enemies piece is next to flag
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
	piece = j - 1 < COLS - 1 ? 0 : gameBoard.board[i][j - 1].getPiece();
	if (piece != 0)
	{
		isMyPiece = AICell::isPlayerOnePiece(gameBoard.board[i][j - 1]) == amIplayerOne;

		if (toupper(piece) == 'B')
		{
			if (isMyPiece)
			{
				protectingBombs++;
			}
			else
				enemyPieces++; //enemies piece is next to flag
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
	piece = i - 1 < ROWS - 1 ? 0 : gameBoard.board[i - 1][j].getPiece();
	if (piece != 0)
	{
		isMyPiece = AICell::isPlayerOnePiece(gameBoard.board[i - 1][j]) == amIplayerOne;

		if (toupper(piece) == 'B')
		{
			if (isMyPiece)
			{
				protectingBombs++;
			}
			else
				enemyPieces++; //enemies piece is next to flag
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
	piece = i + 1 > ROWS ? 0 : gameBoard.board[i + 1][j].getPiece();
	if (piece != 0)
	{
		isMyPiece = AICell::isPlayerOnePiece(gameBoard.board[i + 1][j]) == amIplayerOne;

		if (toupper(piece) == 'B')
		{
			if (isMyPiece)
			{
				protectingBombs++;
			}
			else
				enemyPieces++; //enemies piece is next to flag
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
	piece = i - 1 < ROWS - 1 || j - 1 < COLS - 1 ? 0 : gameBoard.board[i - 1][j - 1].getPiece();
	if (piece != 0)
	{
		isMyPiece = AICell::isPlayerOnePiece(gameBoard.board[i - 1][j - 1]) == amIplayerOne;

		if (toupper(piece) == 'B')
		{
			if (isMyPiece)
			{
				protectingBombs++;
			}
			else
				enemyPieces++; //enemies piece is next to flag
		}
		else if (toupper(piece) != 'F')
		{
			if (isMyPiece)
				otherProtectingPieces++;
			else
				enemyPieces++; //enemies piece is next to flag
		}
	}

	piece = i + 1 > ROWS - 1 || j - 1 < COLS - 1 ? 0 : gameBoard.board[i + 1][j - 1].getPiece();
	if (piece != 0)
	{
		isMyPiece = AICell::isPlayerOnePiece(gameBoard.board[i + 1][j - 1]) == amIplayerOne;

		if (toupper(piece) == 'B')
		{
			if (isMyPiece)
			{
				protectingBombs++;
			}
			else
				enemyPieces++; //enemies piece is next to flag
		}
		else if (toupper(piece) != 'F')
		{
			if (isMyPiece)
				otherProtectingPieces++;
			else
				enemyPieces++; //enemies piece is next to flag
		}
	}

	piece = i - 1 < ROWS - 1 || j + 1 > COLS - 1 ? 0 : gameBoard.board[i - 1][j + 1].getPiece();
	if (piece != 0)
	{
		isMyPiece = AICell::isPlayerOnePiece(gameBoard.board[i - 1][j + 1]) == amIplayerOne;

		if (toupper(piece) == 'B')
		{
			if (isMyPiece)
			{
				protectingBombs++;
			}
			else
				enemyPieces++; //enemies piece is next to flag
		}
		else if (toupper(piece) != 'F')
		{
			if (isMyPiece)
				otherProtectingPieces++;
			else
				enemyPieces++; //enemies piece is next to flag
		}
	}

	piece = i + 1 > ROWS - 1 || j + 1 > COLS - 1 ? 0 : gameBoard.board[i + 1][j + 1].getPiece();
	if (piece != 0)
	{
		isMyPiece = AICell::isPlayerOnePiece(gameBoard.board[i + 1][j + 1]) == amIplayerOne;

		if (toupper(piece) == 'B')
		{
			if (isMyPiece)
			{
				protectingBombs++;
			}
			else
				enemyPieces++; //enemies piece is next to flag
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
		return 1 - (JOKER_SCORE / 100);
	}
	switch (toupper(cell.getPiece()))
	{
	case 'R':
		return 1 - (ROCK_SCORE / 100);
	case 'P':
		return 1 - (PAPER_SCORE / 100);
	case 'S':
		return 1 - (SCISSOR_SCORE / 100);
	}
	cout<<"AutoPlayerAlgorithm::calcMaterial errore"<<endl;
	return -1; //should never get here!

}

double AutoPlayerAlgorithm::calcDiscovery(AICell cell)
{
	int isFlagKnown = cell.flagProbability == 0 ? 0 : 1;
	int isJokerKnown = cell.isJokerKnown ? 0 : 1;
	int isMovingPiece = cell.isMovingPiece ? 0 : 1;

	return (isFlagKnown + isJokerKnown + isMovingPiece) / 3;
}

char AutoPlayerAlgorithm::shouldChangeJoker(double &score, int joker_x, int joker_y, bool amIPlayerOne)
{
	char oldRep = gameBoard.board[joker_x][joker_y].getPiece();
	char origRep = oldRep;
	char newRep;
	char bestRep = -1;
	int currScore;
	int material = calcMaterial(gameBoard.board[joker_x][joker_y]);;
	int discovery = 0;
	int reveal = 0;

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

		Cell::updateCell(gameBoard.board[joker_x][joker_y],
						 newRep, true);
		currScore = calcScore(material, discovery, reveal);

		if (currScore > score)
		{
			score = currScore;
			bestRep = newRep;
		}
		else
		{
			//change back to old rep
			Cell::updateCell(gameBoard.board[joker_x][joker_y],
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
			int piecePlayerNum = AICell::isPlayerOnePiece(gameBoard.board[i][j]) ? 1 : 2;
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
				Cell::updateCell(gameBoard.board[i][j], '#', false);
			}
		}
	}
}

void AutoPlayerAlgorithm::notifyOnOpponentMove(const Move &move)
{
	int from_x = move.getFrom().getX();
	int from_y = move.getFrom().getY();
	int to_x = move.getTo().getX();
	int to_y = move.getTo().getY();
	AICell fromCell = gameBoard.board[from_x][from_y];
	if (gameBoard.board[to_x][to_y].getPiece() == 0){
		AICell::updateCell(gameBoard.board[to_x][to_y], fromCell.getPiece(), fromCell.getIsJoker());
		AICell::updateCellKnowlage(gameBoard.board[to_x][to_y], fromCell);
	}
	else { //there's a fight
		opponentCell = gameBoard.board[from_x][from_y];
		myCell = gameBoard.board[to_x][to_y];
	}
	AICell::cleanCell(gameBoard.board[from_x][from_y]);
}


bool AutoPlayerAlgorithm::isLegalMove(unique_ptr<Move> &move, bool isPlayer1) {
	int from_x = move->getFrom().getX();
	int from_y = move->getFrom().getY();
	int to_x = move->getFrom().getX();
	int to_y = move->getFrom().getY();

	if ((move->getFrom().getX() < 1 || from_x > ROWS)
		|| (to_x < 1 || to_x > ROWS) || (from_y < 1 || from_y > COLS)
		|| (to_y < 1 || to_y > COLS)) {
		cout << "Error: illegal location on board" << endl;
		return false;
	}

	if (from_x == to_x && from_y == to_y) {
		cout << "Error: user MUST move one piece" << endl;
		return false;
	}

	if (gameBoard.board.at(from_x).at(from_y).getPiece() == 0) {
		cout << "Error: there is no piece in this position" << endl;
		return false;
	}
	else if ((isPlayer1 && islower(gameBoard.board.at(from_x).at(from_y).getPiece()))
		|| (!isPlayer1 && isupper(gameBoard.board[from_x][from_y].getPiece()))) {
		cout << "Error: trying to move the opponent piece" << endl;
		return false;
	}

	if (toupper(gameBoard.board.at(from_x).at(from_y).getPiece()) == BOMB
		|| toupper(gameBoard.board[from_x][from_y].getPiece()) == FLAG) {
		cout << "Error: flag/bomb piece is not allowed to move" << endl;
		return false;
	}

	if (to_x == from_x + 1 || to_x == from_x - 1) {
		if (to_y != from_y) {
			cout
				<< "Error: illegal move - can move only one cell up/down/left/right "
				<< endl;
			return false;
		}
	}
	else if (to_y == from_y + 1 || to_y == from_y - 1) {
		if (to_x != from_x) {
			cout
				<< "Error: illegal move - can move only one cell up/down/left/right"
				<< endl;
			return false;
		}
	}
	else {
		cout
			<< "Error: illegal move - can move only one cell up/down/left/right"
			<< endl;
		return false;
	}

	if (gameBoard.board.at(to_x).at(to_y).getPiece() != 0) {
		if (isPlayer1) {
			if (isupper(gameBoard.board.at(to_x).at(to_y).getPiece())) {
				cout
					<< "Error: you are trying to move to a cell taken by your own piece"
					<< endl;
				return false;
			}
		}
		else {
			if (islower(gameBoard.board.at(to_x).at(to_y).getPiece())) {
				cout
					<< "Error: you are trying to move to a cell taken by your own piece"
					<< endl;
				return false;
			}
		}
	}

	return true;
}