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
	cout << "creating autoPlayerAlgorithm" << endl;
	gameBoard = AIBoard();
	myPlayerNum = -1;
	opponentFlagsNumOnBoard = FLAGS_NUM;
	opponentBombsNumOnBoard = BOMBS_NUM + JOKERS_NUM;	//we dont know if joker is now bomb or not
	opponentMovingPieceNumOnBoard = SCISSORS_NUM + ROCKS_NUM + PAPERS_NUM + JOKERS_NUM; //we dont know if joker is moving piece now or not
}

void AutoPlayerAlgorithm::getInitialPositions(int player,
		std::vector<unique_ptr<PiecePosition>> &vectorToFill)
{
	myPlayerNum = player;
	cout<<"BEFORE POSITIONING AI STUFF"<<endl;
	positionUnmovingPieces(player, vectorToFill);
	positionMovingPieces(player, vectorToFill);
	positionJokers(player, vectorToFill);
}

//TODO:delete after debug
void AutoPlayerAlgorithm::printVectorPositions(std::vector<unique_ptr<PiecePosition>> &vectorToFill){
	for (size_t i=0;i<vectorToFill.size(); ++i){
		cout<<"piece: ";
		cout<<(vectorToFill[i]->getPiece())<<endl;
		cout<<"position: x: ";
		cout<<(vectorToFill[i]->getPosition().getX());
		cout<<" y: ";
		cout<<(vectorToFill[i]->getPosition().getY())<<endl;
	}
}

void AutoPlayerAlgorithm::positionMovingPieces(int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill)
{
	positionPiecesRandomly(ROCKS_NUM, player == 1 ? 'R' : 'r', false, '#', vectorToFill);
	positionPiecesRandomly(PAPERS_NUM, player == 1 ? 'P' : 'p', false,'#', vectorToFill);
	positionPiecesRandomly(SCISSORS_NUM, player == 1 ? 'S' : 's', false,'#', vectorToFill);
}

void AutoPlayerAlgorithm::positionJokers(int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill)
{
	for (int i=0; i<JOKERS_NUM; ++i){
		int pieceNum = getRandomNumInRange(1, 4);
		switch (pieceNum)
		{
		case 1: //rock
			positionPiecesRandomly(1, player == 1 ? 'J' : 'j', true,player == 1 ? 'R' : 'r', vectorToFill);
			break;
		case 2: //paper
			positionPiecesRandomly(1, player == 1 ? 'J' : 'j', true,player == 1 ? 'P' : 'p', vectorToFill);
			break;
		case 3: //scissors
			positionPiecesRandomly(1, player == 1 ? 'J' : 'j', true,player == 1 ? 'S' : 's', vectorToFill);
			break;
		case 4: //bomb
			positionPiecesRandomly(1, player == 1 ? 'J' : 'j', true,player == 1 ? 'B' : 'b', vectorToFill);
			break;
		default:
			break;
		}
	}

}

void AutoPlayerAlgorithm::positionUnmovingPieces(int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill)
{
	bool isCellTaken = true;
	bool hasOpponentNeighbor = true;
	char piece = (player == 1 ? 'F' : 'f');
	int x = getRandomNumInRange(0, COLS - 1);
	int y = getRandomNumInRange(0, ROWS - 1);
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
			if (gameBoard.board[x][y].getPiece() == 0)
			{
				isCellTaken = false;
				if (!checkIsOpponentNeighbors(player, x, y))
				{
					hasOpponentNeighbor = false;
					AICell::updateCell(gameBoard.board[x][y], piece, false);
					vectorToFill.push_back(make_unique<RPSPiecePosition>(RPSpoint(x+1, y+1), toupper(piece), '#'));
					positionBombs(x, y, player, vectorToFill, protectingBombsNum, shouldPositionRandomly);
					bombsPositioned += protectingBombsNum;
				}
			}
			x = getRandomNumInRange(0, COLS - 1);
			y = getRandomNumInRange(0, ROWS - 1);
		}
		isCellTaken = true;
		hasOpponentNeighbor = true;
	}

	if (bombsPositioned < BOMBS_NUM)
	{
		shouldPositionRandomly = true;
		positionBombs(x, y, player, vectorToFill, BOMBS_NUM - bombsPositioned, shouldPositionRandomly);
	}
}

int AutoPlayerAlgorithm::getRandomNumInRange(int start, int end)
{
	std::random_device rd;								  // obtain a random number from hardware
	std::mt19937 eng(rd());								  // seed the generator
	std::uniform_int_distribution<> yDistr(start, end); // define the range
	return yDistr(eng);
}
// x-col y-row
bool AutoPlayerAlgorithm::findEmptyNeigbor(int &x, int &y, int flag_x, int flag_y)
{

	char piece;
	x = flag_x;
	y = flag_y;
	//down neigbor
	if (isLegalPosition(x + 1, y))
	{
		piece = gameBoard.board[x + 1][y].getPiece();
		if (piece == 0){ //empty cell
			x = x + 1;
			return true;
		}
	}
	//right

	if (isLegalPosition(x, y + 1))
	{
		piece = gameBoard.board[x][y + 1].getPiece();
		if (piece == 0) {//empty cell
			y = y + 1;
			return true;
		}
	}
	//up neighbor

	if (isLegalPosition(x - 1, y))
	{
		piece = gameBoard.board[x - 1][y].getPiece();
		if (piece == 0) {//empty cell
			x = x - 1;
			return true;
		}
	}
	//left

	if (isLegalPosition(x, y - 1))
	{
		piece = gameBoard.board[x][y - 1].getPiece();
		if (piece == 0) {//empty cell
			y = y -1;
			return true;
		}
	}
	return false; //no empty cell
}

void AutoPlayerAlgorithm::positionBombs(int flag_x, int flag_y, int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill, int bombsToPosition, bool shouldPositionRandomly)
{
	char piece = (player == 1 ? 'B' : 'b');
	int x;
	int y;
	int bombsPositioned = 0;

	if (!shouldPositionRandomly)
	{
		for (int i = 0; i < bombsToPosition; ++i)
		{
			if (findEmptyNeigbor(x, y, flag_x, flag_y))
			{
				Cell::updateCell(gameBoard.board[x][y], piece, false);
				vectorToFill.push_back(make_unique<RPSPiecePosition>(RPSpoint(x+1, y+1), toupper(piece), '#'));
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

	positionPiecesRandomly(bombsToPosition, piece, false,'#', vectorToFill);
}

void AutoPlayerAlgorithm::positionPiecesRandomly(int pieceNum, char piece, bool isJoker, char pieceRep, std::vector<unique_ptr<PiecePosition>> &vectorToFill)
{
	bool isCellTaken = true;
	int x = getRandomNumInRange(0, COLS - 1);
	int y = getRandomNumInRange(0, ROWS - 1);
	for (int i = 0; i < pieceNum; ++i)
	{
		while (isCellTaken)
		{
			if (gameBoard.board[x][y].getPiece() == 0)
			{
				isCellTaken = false;

				if (isJoker){
					Cell::updateCell(gameBoard.board[x][y], pieceRep, isJoker);
					vectorToFill.push_back(make_unique<RPSPiecePosition>(RPSpoint(x+1, y+1), toupper(piece) , toupper(pieceRep)));
				}
				else {//not a joker
					Cell::updateCell(gameBoard.board[x][y], piece, isJoker);
					vectorToFill.push_back(make_unique<RPSPiecePosition>(RPSpoint(x+1, y+1), toupper(piece), toupper(pieceRep)));
				}
				break;
			}
			x = getRandomNumInRange(0, COLS - 1);
			y = getRandomNumInRange(0, ROWS - 1);
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
Input  - relevant y and x
Output - true if this is a legal position and  false otherwise 
 */
// legal position on board starts from 0 to number of xs/ys - 1
//x-col y-row
bool AutoPlayerAlgorithm::isLegalPosition(int x, int y)
{
	if (x > COLS - 1 || y > ROWS - 1 || x < 0 || y < 0)
	{
		return false;
	}
	return true;
}
/*
Input - current player num, x and y
Output - true if an opponent nieghbour is located there and otherwise false 
 */
bool AutoPlayerAlgorithm::checkIsOpponentNeighbors(int player, int x, int y)
{
	//down neigbor
	char piece = gameBoard.board[x + 1][y].getPiece();
	if (isLegalPosition(x + 1, y) && isOpponentPiece(player, piece))
	{
		return true;
	}
	//right
	piece = gameBoard.board[x][y + 1].getPiece();
	if (isLegalPosition(x, y + 1) && isOpponentPiece(player, piece))
	{
		return true;
	}
	//up neighbor
	piece = gameBoard.board[x - 1][y].getPiece();
	if (isLegalPosition(x - 1, y) && isOpponentPiece(player, piece))
	{
		return true;
	}
	//left
	piece = gameBoard.board[x][y - 1].getPiece();
	if (isLegalPosition(x, y - 1) && isOpponentPiece(player, piece))
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
	//delete after debug
		cout<<"************AFTER NOTIFY-BEFORE UPDATE************"<<endl;
		PrintBoardToConsole();
	updateBoard(b);
	for (int i = 0; i < (int)fights.size(); ++i)
	{
		int x = fights[i]->getPosition().getX()-1; //col
		int y = fights[i]->getPosition().getY()-1; //row
		char myPiece = toupper(fights[i]->getPiece(myPlayerNum)); //do we need this?
		char opponentPiece = toupper(fights[i]->getPiece(myPlayerNum == 1 ? 2 : 1));
		int winner = fights[i]->getWinner();

		cout<<"###############FIGHTT RESULTS: position: "<<x<<" , "<<y<<" winner: "<<winner<<" my number: "<<myPlayerNum<<" my piece: "<<myPiece<<" opp piece: "<<opponentPiece<<endl;

		if (winner == 0)
		{
			AICell::cleanCell(gameBoard.board[x][y]);

			if (opponentPiece == 'B')
				opponentBombsNumOnBoard--;
			else if (opponentPiece == 'F')
				opponentFlagsNumOnBoard--;
			else
				opponentMovingPieceNumOnBoard--;
		}
		else if (winner != myPlayerNum)
		{ //opponent won
			//not a flag - cant win with flag
			gameBoard.board[x][y].flagProbability = 0;
			if (opponentPiece == 'B') {
				opponentBombsNumOnBoard--;
				AICell::cleanCell(gameBoard.board[x][y]);
			}
			else {
				AICell::updateCell(gameBoard.board[x][y], myPlayerNum == 1 ? tolower(opponentPiece) : opponentPiece, false);
				//if it is not a bomb and not a flag - than it is moving piece
				gameBoard.board[x][y].isMovingPieceKnown = true;
				gameBoard.board[x][y].isMovingPiece = true;
			}
		}
		else
		{ //i won
			if (myPiece == 'B')
				AICell::cleanCell(gameBoard.board[x][y]);
			//if i won so opponent piece must not be a bomb
			if (opponentPiece == 'F')
				opponentFlagsNumOnBoard--;
			else //must be moving piece
				opponentMovingPieceNumOnBoard--;
			AICell::updateCell(gameBoard.board[x][y], myPlayerNum == 1 ? myPiece : tolower(myPiece), gameBoard.board[x][y].getIsJoker());
		}


	}

	if (opponentMovingPieceNumOnBoard == 0)
	{ //we can mark all pieces as not moving pieces !!!
		updateMovingPiece();
	}

	updateFlagProbability();

	//delete after debug
	cout<<"************AFTER NOTIFY************"<<endl;
	PrintBoardToConsole();
}

void AutoPlayerAlgorithm::updateFlagProbability()
{
	int unkownPiecesNum = 0;
	for (int i = 0; i < COLS; ++i)
	{
		for (int j = 0; j < ROWS; ++j)
		{
			if (gameBoard.board[i][j].flagProbability != 0 && gameBoard.board[i][j].flagProbability != 1)
			{
				unkownPiecesNum++; //count pieces that we dont know if there are flags or not
			}
		}
	}

	if (unkownPiecesNum == 0)
		return; //probabilities are 1 or 0

	//update probability
	for (int i = 0; i < COLS; ++i)
	{
		for (int j = 0; j < ROWS; ++j)
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
	//cout<<"inside get move~~~~~~~~~~~~~~~"<<endl;
	int from_x;
	int from_y;
	int to_x;
	int to_y;
	getBestMove(from_x, from_y, to_x, to_y);
	unique_ptr<Move> move = make_unique<RPSMove>(RPSpoint(from_x+1, from_y+1), RPSpoint(to_x+1, to_y+1));

	///delete after debug
	cout<<"player number "<<myPlayerNum<<endl;
	cout<<"MY MOVE: "<<from_x<<" "<<from_y<<" to "<<to_x<<" "<<to_y<<" my piece: "<<myCell.getPiece()<<endl;
	cout<<"~~~~MY BOARD~~~~"<<endl;
	PrintBoardToConsole();

	return move;
}

unique_ptr<JokerChange> AutoPlayerAlgorithm::getJokerChange()
{
	int joker_x;
	int joker_y;
	char newRep = -1;
	char bestRep = newRep;
	double score = calcScore(1, 0, 0);
	for (int i = 0; i < COLS; ++i)
	{
		for (int j = 0; j < ROWS; ++j)
		{
			bool isMyPiece = gameBoard.board[i][j].isMyPiece(myPlayerNum);
			if (gameBoard.board[i][j].getIsJoker() && isMyPiece)
			{
				newRep = shouldChangeJoker(score, i, j, myPlayerNum == 1);
				if (newRep != -1){
					bestRep = newRep;
					joker_x = i;
					joker_y = j;
				}
			}
		}
	}
	//no joker change needed
	if (bestRep == -1)
		return nullptr;

	AICell::updateCell(gameBoard.board[joker_x][joker_y],newRep,true);
	cout<<"JOKER CHANGED: to "<<newRep<<endl;
	unique_ptr<JokerChange> jokerChange = make_unique<RPSJokerChange>(toupper(newRep), RPSpoint(joker_x+1, joker_y+1));
	return jokerChange;
}

void AutoPlayerAlgorithm::getBestMove(int &from_x, int &from_y, int &to_x, int &to_y)
{
	//cout<<"inside get best move~~~~~~~~~~~~"<<endl;
	//bool isMyPiece = (myPlayerNum == 1);
	double score;
	int maxScore = INT_MIN;
	for (int i = 0; i < COLS; ++i)
	{
		for (int j = 0; j < ROWS; ++j)
		{
			if (!gameBoard.board[i][j].isMyPiece(myPlayerNum)) continue;
			if (!gameBoard.board[i][j].checkIsMovingPiece()) continue;
			score = getBestMoveForPiece(maxScore, i, j, to_x, to_y);
			if (maxScore < score)
			{
				maxScore = score;
				from_x = i;
				from_y = j;
			}
		}
	}

	//make move
	if (gameBoard.board[to_x][to_y].getPiece() == 0)
	{
		Cell::updateCell(gameBoard.board[to_x][to_y], gameBoard.board[from_x][from_y].getPiece(),
				gameBoard.board[from_x][from_y].getIsJoker());
		AICell::updateCellKnowlage(gameBoard.board[to_x][to_y], gameBoard.board[from_x][from_y]);
	}
	else
	{ //there is going to be a fight
		myCell = gameBoard.board[from_x][from_y];
		opponentCell = gameBoard.board[to_x][to_y];
	}
	AICell::cleanCell(gameBoard.board[from_x][from_y]);
}

double AutoPlayerAlgorithm::getBestMoveForPiece(double score, const int &from_x, const int &from_y, int &to_x, int &to_y)
{
	//cout<<"INSIDE getBestMoveForPiece"<<endl;
	int x = from_x; //col
	int y = from_y; //row
	int currScore;

	//move right
	unique_ptr<Move> move = make_unique<RPSMove>(RPSpoint(from_x, from_y), RPSpoint(x, y + 1));
	if (isLegalMove(move, myPlayerNum == 1))
	{
		// Cell prevCell = gameBoard.board[from_x][from_y];
		currScore = tryMovePiece(move);
		if (currScore > score)
		{
			score = currScore;
			to_x = x;
			to_y = y + 1;
		}
	}

	//move left
	move = make_unique<RPSMove>(RPSpoint(from_x, from_y), RPSpoint(x, y - 1));
	if (isLegalMove(move, myPlayerNum == 1))
	{
		//Cell prevCell = gameBoard.board[from_x][from_y];
		currScore = tryMovePiece(move);
		if (currScore > score)
		{
			score = currScore;
			to_x = x;
			to_y = y - 1;
		}
	}

	//move up
	move = make_unique<RPSMove>(RPSpoint(from_x, from_y), RPSpoint(x - 1, y));
	if (isLegalMove(move, myPlayerNum == 1))
	{
		// Cell prevCell = gameBoard.board[from_x][from_y];
		currScore = tryMovePiece(move);
		if (currScore > score)
		{
			score = currScore;
			to_x = x - 1;
			to_y = y;
		}
	}

	//move down
	move = make_unique<RPSMove>(RPSpoint(from_x, from_y), RPSpoint(x + 1, y));
	if (isLegalMove(move, myPlayerNum == 1))
	{

		currScore = tryMovePiece(move);
		if (currScore > score)
		{
			score = currScore;
			to_x = x + 1;
			to_y = y;
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
	double reveal = -0.5;

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
			material *= -1.0;
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
bool AutoPlayerAlgorithm::fight(int x, int y, char myPiece, char opponentPiece, bool isMyPieceJoker)
{

	char myPieceRep = toupper(myPiece);
	char oppPieceRep = toupper(opponentPiece);
	//Case 1: 2 players in the same type - tie
	if (myPieceRep == oppPieceRep)
	{
		Cell::cleanCell(gameBoard.board[x][y]);
		return false;
	}

	//Case 2: there is flag and current player has another piece
	else if (oppPieceRep == FLAG)
	{
		Cell::updateCell(gameBoard.board[x][y], myPiece, isMyPieceJoker);
		return true;
	}

	//case 3: there is bomb and current player has another piece
	else if (oppPieceRep == BOMB || myPieceRep == BOMB)
	{
		Cell::cleanCell(gameBoard.board[x][y]);
		return false;
	}

	//case 4: there is PAPER and current player has another piece
	else if (oppPieceRep == PAPER)
	{
		if (myPieceRep == ROCK)
		{
			return false;
		}
		else if (myPieceRep == SCISSOR)
		{
			Cell::updateCell(gameBoard.board[x][y], myPiece,
					isMyPieceJoker);
			return true;
		}
	}
	//case 7: player 1 is ROCK and player 2 another piece
	else if (oppPieceRep == ROCK)
	{
		if (myPieceRep == PAPER)
		{
			Cell::updateCell(gameBoard.board[x][y], myPiece,
					isMyPieceJoker);
			return true;
		}
		else if (myPieceRep == SCISSOR)
		{
			return false;
		}
	}

	else if (oppPieceRep == SCISSOR)
	{
		if (myPieceRep == PAPER)
		{
			return false;
		}
		else if (myPieceRep == ROCK)
		{
			Cell::updateCell(gameBoard.board[x][y], myPiece,
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
	double score = MATERIAL_WEIGHT * material + DISCOVERY_WEIGHT * discovery + REVEAL_WEIGHT * reveal +
			FLAG_SAFTEY_WEUGHT * flagSaftey + DISTANCE_FROM_FLAG_WEIGHT * distanceFromBombOrFlag +
			DISTANCE_FROM_UNKNOWN_WEIGHT * distanceFromUnknownPiece;
	//	cout<<"material: "<<material<<endl;
	//	cout<<"discovery: "<<discovery<<endl;
	//	cout<<"reveal: "<<reveal<<endl;
	//	cout<<"flag safety: "<<flagSaftey<<endl;
	//	cout<<"distance 1: "<<distanceFromBombOrFlag<<endl;
	//	cout<<"distance 2: "<<distanceFromUnknownPiece<<endl;
	//	cout<<"in calc sore: "<<score<<endl;
	return score;
}

void AutoPlayerAlgorithm::notifyFightResult(const FightInfo &fightInfo)
{
	cout<<"FIGHT RESULT"<<endl;

	int x = fightInfo.getPosition().getX()-1;
	int y = fightInfo.getPosition().getY()-1;
	char myPiece = toupper(fightInfo.getPiece(myPlayerNum));
	char opponentPiece = toupper(fightInfo.getPiece(myPlayerNum == 1 ? 2 : 1));
	int winner = fightInfo.getWinner();

	cout<<"###############FIGHTT RESULTS: position: "<<x<<" , "<<y<<" winner: "<<winner<<" my number: "<<myPlayerNum<<" my piece: "<<myPiece<<" opp piece: "<<opponentPiece<<endl;


	//check if piece was known before and changed.
	bool isJoker = (toupper(opponentCell.getPiece()) != opponentPiece && opponentCell.getPiece() != 0 && opponentCell.getPiece() != '#');

	if (winner == 0)
	{
		if (opponentPiece == 'B') {
			opponentBombsNumOnBoard--;
			if (isJoker) //for removing extra pieces as jokers unkown
				opponentMovingPieceNumOnBoard--;
		}
		else if (opponentPiece == 'F')
			opponentFlagsNumOnBoard--;
		else {
			opponentMovingPieceNumOnBoard--;
			if (isJoker) //for removing extra pieces as jokers unkown
				opponentBombsNumOnBoard--;
		}
		Cell::cleanCell(gameBoard.board[x][y]);
	}
	else if (winner != myPlayerNum)
	{   //opponent won
		//not a flag - cant win
		gameBoard.board[x][y].flagProbability = 0;
		if (opponentPiece == 'B')
		{
			opponentBombsNumOnBoard--;
			if (isJoker) //for removing extra pieces as jokers unkown
				opponentMovingPieceNumOnBoard--;
			Cell::cleanCell(gameBoard.board[x][y]);
		}
		else {
			AICell::updateCell(gameBoard.board[x][y], myPlayerNum == 1 ? tolower(opponentPiece) : opponentPiece, isJoker);
			//if it is not a bomb and not a flag - than it is moving piece
			gameBoard.board[x][y].isMovingPieceKnown = true;
			gameBoard.board[x][y].isMovingPiece = true;
		}

		if (isJoker)
			gameBoard.board[x][y].isJokerKnown = true;

	}
	else
	{   //i won
		//if i won so opponent piece must not be a bomb
		if (opponentPiece == 'F')
			opponentFlagsNumOnBoard--;
		else {
			opponentMovingPieceNumOnBoard--;
			if (isJoker) //for removing extra pieces as jokers unkown
				opponentBombsNumOnBoard--;
		}
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
	int distance;
	int minimalDistance = ROWS + COLS;
	//find bomb or flag
	for (int i = 0; i < COLS; ++i)
	{
		for (int j = 0; j < ROWS; ++j)
		{
			char piece = gameBoard.board[i][j].getPiece();
			if (piece == 0)
				continue;
			bool isMyPiece = gameBoard.board[i][j].isMyPiece(myPlayerNum);
			if (!isMyPiece)
			{
				if (gameBoard.board[i][j].isMovingPieceKnown && !gameBoard.board[i][j].isMovingPiece)
				{
					distance = calcDistanceFromPiece(i, j);
					if (minimalDistance > distance)
						minimalDistance = distance;
				}
			}
		}
	}
	//cout<<"DISTANCE!:!:!!:!:!:"<<minimalDistance<<endl;
	return (double)(ROWS + COLS - minimalDistance) / (double)(ROWS + COLS);
}

double AutoPlayerAlgorithm::calcDistanceFromUnknownPiece()
{
	int distance;
	int minimalDistance = ROWS + COLS;
	//find bomb or flag
	for (int i = 0; i < COLS; ++i)
	{
		for (int j = 0; j < ROWS; ++j)
		{
			char piece = gameBoard.board[i][j].getPiece();
			if (piece == 0)
				continue;
			bool isMyPiece = gameBoard.board[i][j].isMyPiece(myPlayerNum);
			if (!isMyPiece)
			{
				if (!gameBoard.board[i][j].isMovingPieceKnown)
				{
					distance = calcDistanceFromPiece(i, j);
					if (minimalDistance > distance)
						minimalDistance = distance;
				}
			}
		}
	}

	return (double)(ROWS + COLS - minimalDistance) / (double)(ROWS + COLS);
}

int AutoPlayerAlgorithm::calcDistanceFromPiece(int piece_x, int piece_y)
{
	int distance;
	int minimalDistance = INT_MAX;

	for (int i = 0; i < COLS; ++i)
	{
		for (int j = 0; j < ROWS; ++j)
		{
			bool isMyPiece = gameBoard.board[i][j].isMyPiece(myPlayerNum);
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
	for (int i = 0; i < COLS; ++i)
	{
		for (int j = 0; j < ROWS; ++j)
		{
			bool isMyPiece = gameBoard.board[i][j].isMyPiece(myPlayerNum);
			if (!isMyPiece)
			{
				if (gameBoard.board[i][j].getPiece() != 0)
					totalEnemyPieces++;
				continue;
			}
			//else - this is my piece
			char piece = toupper(gameBoard.board[i][j].getPiece());
			if (piece == 'F')
			{
				countProtectingPieces(i, j, protectingBombs, otherProtectingPieces, enemyPieces);
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
	if (bombs == 0 && movingPieces == 0)
		return -1; //flag safety doesnt matter - no pieces left
	if (enemyPieces > 0)
		return ((double)(-1 * enemyPieces) / (double)totalEnemyPieces);
	return ((double)((2 * protectingBombs) + otherProtectingPieces) / (double)((2 * bombs) + movingPieces));
}
void AutoPlayerAlgorithm::countProtectingPieces(int i, int j, int &protectingBombs, int &otherProtectingPieces, int &enemyPieces)

{
	//searchRight
	bool isMyPiece;
	char piece = j + 1 > ROWS - 1 ? 0 : gameBoard.board[i][j + 1].getPiece();
	if (piece != 0)
	{
		isMyPiece = gameBoard.board[i][j + 1].isMyPiece(myPlayerNum);

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
	piece = j - 1 < ROWS - 1 ? 0 : gameBoard.board[i][j - 1].getPiece();
	if (piece != 0)
	{
		isMyPiece = gameBoard.board[i][j - 1].isMyPiece(myPlayerNum);

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
	piece = i - 1 < COLS - 1 ? 0 : gameBoard.board[i - 1][j].getPiece();
	if (piece != 0)
	{
		isMyPiece = gameBoard.board[i - 1][j].isMyPiece(myPlayerNum);

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
	piece = i + 1 > COLS ? 0 : gameBoard.board[i + 1][j].getPiece();
	if (piece != 0)
	{
		isMyPiece = gameBoard.board[i + 1][j].isMyPiece(myPlayerNum);

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
	piece = i - 1 < COLS - 1 || j - 1 < ROWS - 1 ? 0 : gameBoard.board[i - 1][j - 1].getPiece();
	if (piece != 0)
	{
		isMyPiece = gameBoard.board[i - 1][j - 1].isMyPiece(myPlayerNum);

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

	piece = i + 1 > COLS - 1 || j - 1 < ROWS - 1 ? 0 : gameBoard.board[i + 1][j - 1].getPiece();
	if (piece != 0)
	{
		isMyPiece = gameBoard.board[i + 1][j - 1].isMyPiece(myPlayerNum);

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

	piece = i - 1 < COLS - 1 || j + 1 > ROWS - 1 ? 0 : gameBoard.board[i - 1][j + 1].getPiece();
	if (piece != 0)
	{
		isMyPiece = gameBoard.board[i - 1][j + 1].isMyPiece(myPlayerNum);

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

	piece = i + 1 > COLS - 1 || j + 1 > ROWS - 1 ? 0 : gameBoard.board[i + 1][j + 1].getPiece();
	if (piece != 0)
	{
		isMyPiece = gameBoard.board[i + 1][j + 1].isMyPiece(myPlayerNum);

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
		return 1.0 - ((double)JOKER_SCORE / (double)10);
	}
	switch (toupper(cell.getPiece()))
	{
	case 'R':
		return 1.0 - ((double)ROCK_SCORE / (double)10);
	case 'P':
		return 1.0 - ((double)PAPER_SCORE / (double)10);
	case 'S':
		return 1.0 - ((double)SCISSOR_SCORE / (double)10);
	}
	return -1; //should never get here!

}

double AutoPlayerAlgorithm::calcDiscovery(AICell cell)
{
	int isFlagKnown = cell.flagProbability == 0 ? 0 : 1;
	int isJokerKnown = cell.isJokerKnown ? 0 : 1;
	int isMovingPiece = cell.isMovingPiece ? 0 : 1;

	return (double)(isFlagKnown + isJokerKnown + isMovingPiece) / 3.0;
}

char AutoPlayerAlgorithm::shouldChangeJoker(double &score, int joker_x, int joker_y, bool amIPlayerOne)
{
	char oldRep = gameBoard.board[joker_x][joker_y].getPiece();
	char origRep = oldRep;
	char newRep;
	char bestRep = -1;
	double currScore;
	double material = calcMaterial(gameBoard.board[joker_x][joker_y]);
	double discovery = 0;
	double reveal = 0;

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

		Cell::updateCell(gameBoard.board[joker_x][joker_y],newRep, true);
		currScore = calcScore(material, discovery, reveal);

		if (currScore > score)
		{
			score = currScore;
			bestRep = newRep;
		}
		//change back to old rep
		Cell::updateCell(gameBoard.board[joker_x][joker_y],oldRep, true);

	}

	return bestRep;
}

void AutoPlayerAlgorithm::updateMovingPiece()
{
	for (int i = 0; i < COLS; ++i)
	{
		for (int j = 0; j < ROWS; ++j)
		{
			if (gameBoard.board[i][j].getPiece() == 0) //empty cell
				continue;
			if (!gameBoard.board[i][j].isMyPiece(myPlayerNum))
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
	for (int i = 0; i < COLS; ++i)
	{
		for (int j = 0; j < ROWS; ++j)
		{
			RPSpoint p = RPSpoint(i+1, j+1);
			if (!gameBoard.board[i][j].isMyPiece(myPlayerNum) && b.getPlayer(p) != 0)
			{ //not empty - must be opponent piece
				Cell::updateCell(gameBoard.board[i][j], '#', false);
			}
		}
	}
}

void AutoPlayerAlgorithm::notifyOnOpponentMove(const Move &move)
{
	cout<<"MOVE RESULT"<<endl;
	int from_x = move.getFrom().getX()-1;
	int from_y = move.getFrom().getY()-1;
	int to_x = move.getTo().getX()-1;
	int to_y = move.getTo().getY()-1;
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
	int to_x = move->getTo().getX();
	int to_y = move->getTo().getY();

	if ((from_x < 0 || from_x > COLS-1)
			|| (to_x < 0 || to_x > COLS-1) || (from_y < 0 || from_y > ROWS-1)
			|| (to_y < 0 || to_y > ROWS-1)) {
		//		cout << "Error: illegal location on board" << endl;
		return false;
	}

	if (from_x == to_x && from_y == to_y) {
		//	cout << "Error: user MUST move one piece" << endl;
		return false;
	}

	if (gameBoard.board.at(from_x).at(from_y).getPiece() == 0) {
		//cout << "Error: there is no piece in this position" << endl;
		return false;
	}
	else if ((isPlayer1 && islower(gameBoard.board.at(from_x).at(from_y).getPiece()))
			|| (!isPlayer1 && isupper(gameBoard.board[from_x][from_y].getPiece()))) {
		//cout << "Error: trying to move the opponent piece" << endl;
		return false;
	}

	if (toupper(gameBoard.board.at(from_x).at(from_y).getPiece()) == BOMB
			|| toupper(gameBoard.board[from_x][from_y].getPiece()) == FLAG) {
		//cout << "Error: flag/bomb piece is not allowed to move" << endl;
		return false;
	}

	if (to_x == from_x + 1 || to_x == from_x - 1) {
		if (to_y != from_y) {
			//cout<< "Error: illegal move - can move only one cell up/down/left/right "<< endl;
			return false;
		}
	}
	else if (to_y == from_y + 1 || to_y == from_y - 1) {
		if (to_x != from_x) {
			//cout
			//<< "Error: illegal move - can move only one cell up/down/left/right"
			//<< endl;
			return false;
		}
	}
	else {
		//cout
		//<< "Error: illegal move - can move only one cell up/down/left/right"
		//<< endl;
		return false;
	}

	if (gameBoard.board.at(to_x).at(to_y).getPiece() != 0) {
		if (isPlayer1) {
			if (isupper(gameBoard.board.at(to_x).at(to_y).getPiece())) {
				//	cout
				//	<< "Error: you are trying to move to a cell taken by your own piece"
				//	<< endl;
				return false;
			}
		}
		else {
			if (islower(gameBoard.board.at(to_x).at(to_y).getPiece())) {
				//	cout
				//	<< "Error: you are trying to move to a cell taken by your own piece"
				//	<< endl;
				return false;
			}
		}
	}

	return true;
}

//Need to erase this function - only for debug!
void AutoPlayerAlgorithm::PrintBoardToConsole()
{
	cout << "*******************PRINT THE BOARD:****************" << endl;
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
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
				cout <<" "<< gameBoard.board[j][i].getPiece() << " ";
			}
		}
		cout << endl;
	}
}
