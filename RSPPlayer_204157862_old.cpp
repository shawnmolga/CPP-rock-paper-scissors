/*
 *  RSPPlayer_204157862.cpp
 *
 *  Created on: 2 ���� 2018
 *      Author: OR
 */

#include "RSPPlayer_204157862.h"
#include "AlgorithmRegistration.h"

extern "C" {
 RSPPlayer_204157862:: RSPPlayer_204157862()
{
	gameBoard = AIBoard(); //board for AI to learn about the game
	myPlayerNum = -1;
	opponentFlagsNumOnBoard = FLAGS_NUM;
	opponentBombsNumOnBoard = BOMBS_NUM + JOKERS_NUM;	//we dont know if joker is now bomb or not
	opponentMovingPieceNumOnBoard = SCISSORS_NUM + ROCKS_NUM + PAPERS_NUM + JOKERS_NUM; //we dont know if joker is moving piece now or not
	willBeFight = false;
}
}
REGISTER_ALGORITHM(204157862)

/**
    game manager will use this function to get positions of pieces to locate on board.

    @params: player - current player number
    vectorToFill - vector of positions that will be filled
 */
void  RSPPlayer_204157862::getInitialPositions(int player,
		std::vector<unique_ptr<PiecePosition>> &vectorToFill)
{
	myPlayerNum = player;
	positionUnmovingPieces(player, vectorToFill);
	positionMovingPieces(player, vectorToFill);
	positionJokers(player, vectorToFill);

}

/**
    fill vector in positions of all moving pieces of current player and position them on algorithm's board

    @params: player - current player number
    vectorToFill - vector of positions that will be filled
 */
void  RSPPlayer_204157862::positionMovingPieces(int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill)
{
	positionPiecesRandomly(ROCKS_NUM, player == 1 ? 'R' : 'r', false, '#', vectorToFill);
	positionPiecesRandomly(PAPERS_NUM, player == 1 ? 'P' : 'p', false,'#', vectorToFill);
	positionPiecesRandomly(SCISSORS_NUM, player == 1 ? 'S' : 's', false,'#', vectorToFill);
}

/**
    fill vector in positions of all jokers of current player and position them on algorithm's board

    @params: player - current player number
    vectorToFill - vector of positions that will be filled
 */
void  RSPPlayer_204157862::positionJokers(int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill)
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

/**
    position flags and bombs in algorithm's board and fill positions in vector

    @params: player - current player number
    vectorToFill - vector of positions that will be filled
 */
void  RSPPlayer_204157862::positionUnmovingPieces(int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill)
{
	int x = getRandomNumInRange(0, COLS - 1);
	int y = getRandomNumInRange(0, ROWS - 1);
	bool shouldPositionRandomly = true;
	int bombsPositioned = 0;							  //sum all bombs already positioned

	//position all flags on board
	positionFlagsOnBoard(player, vectorToFill, bombsPositioned);

	if (bombsPositioned < BOMBS_NUM)
	{
		positionBombs(x, y, player, vectorToFill, BOMBS_NUM - bombsPositioned, shouldPositionRandomly);
	}
}

/**
    position flags in algorithm's board and fill positions in vector

    @params: player - current player number
    vectorToFill - vector of positions that will be filled
    bombsPositioned - count bombs positioned around flags to know if we have left any bombs to position
 */
void  RSPPlayer_204157862::positionFlagsOnBoard(int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill, int& bombsPositioned){
	bool isCellTaken = true;
	bool hasOpponentNeighbor = true;
	bool shouldPositionRandomly = false;
	char piece = (player == 1 ? 'F' : 'f');
	int x = getRandomNumInRange(0, COLS - 1);
	int y = getRandomNumInRange(0, ROWS - 1);
	int protectingBombsNum = BOMBS_NUM / (2 * FLAGS_NUM); //half will protect all flags and half will be spred randomly on board
	if (protectingBombsNum < 1 && BOMBS_NUM > 0)
	{ //prefer to protect flags
		protectingBombsNum = 1;
	}

	for (int i = 0; i < FLAGS_NUM; ++i)
		{
			while (isCellTaken || hasOpponentNeighbor)
			{
				if (gameBoard.board[x][y].getPiece() == 0)
				{
					isCellTaken = false;
					if (!checkIsOpponentNeighbors(x, y))
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
}

/**
    choose range to get a random number from.

    @params: start - range start
    end - range end
    @return random number from given range
 */
int  RSPPlayer_204157862::getRandomNumInRange(int start, int end)
{
	std::random_device rd;								  // obtain a random number from hardware
	std::mt19937 eng(rd());								  // seed the generator
	std::uniform_int_distribution<> yDistr(start, end); // define the range
	return yDistr(eng);
}

/**
    finding empty neighbor cell to flag position to locate a bomb in it

    @params: x,y - will be filled if an empty neighbor cell found as it's position.
    flag_x , flag_y - flag's position
    @return: true - empty neighbor cell was found, false - otherwise.
 */
bool  RSPPlayer_204157862::findEmptyNeigbor(int &x, int &y, int flag_x, int flag_y)
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

/**
    position bombs in algorithm's board and fill positions in vector

    @params: flag_x , flag_y - relevant if bombs should be positioned as flag's neighbors.
    If so, represents the location of flag.
    player - current player number.
    vectorToFill - vector of positions that will be filled
    bombsToPosition - number of bombs should be positioned
    shouldPositionRandomly - false - bombs should be positioned as flag's neighbor.
    true - otherwise.
 */
void  RSPPlayer_204157862::positionBombs(int flag_x, int flag_y, int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill, int bombsToPosition, bool shouldPositionRandomly)
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

/**
    position pieces randomly from same kind and by given number of pieces

    @params: pieceNum - number of pieces to position.
    piece - kind of piece to position.
    isJoker - is piece represents a joker.
    pieceRep - joker representation or '#' if this is not a joker
    player - current player number.
    vectorToFill - vector of positions that will be filled
 */
void  RSPPlayer_204157862::positionPiecesRandomly(int pieceNum, char piece, bool isJoker, char pieceRep, std::vector<unique_ptr<PiecePosition>> &vectorToFill)
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

/**
    check if this piece belongs to opponent

    @params: piece - piece to check
    @return: true - this is opponent piece, false - otherwise.
 */
bool  RSPPlayer_204157862::isOpponentPiece(char piece)
{
	if (piece == '#')
		return true;

	else if (myPlayerNum == 1)
	{
		return islower(piece);
	}
	else
	{
		return isupper(piece);
	}
}

/*
 * check if position is in board's limits
 *
@param: relevant y and x
@return: true if this is a legal position and  false otherwise
 */
bool  RSPPlayer_204157862::isLegalPosition(int x, int y)
{
	if (x > COLS - 1 || y > ROWS - 1 || x < 0 || y < 0)
	{
		return false;
	}
	return true;
}

/**
    checks if in neighbor cell there is an opponent piece

    @params: x,y - piece position to check its neighbors.
    @return: true if an opponent nieghbour is located there and otherwise false
 */
bool  RSPPlayer_204157862::checkIsOpponentNeighbors(int x, int y)
{
	char piece;

	//down neigbor
	if(isLegalPosition(x + 1, y)){
		piece = gameBoard.board[x + 1][y].getPiece();
		if (isOpponentPiece(piece))
		{
			return true;
		}
	}

	//right
	if (isLegalPosition(x, y + 1)){
		piece = gameBoard.board[x][y + 1].getPiece();
		if (isOpponentPiece(piece))
		{
			return true;
		}
	}

	//up neighbor
	if (isLegalPosition(x - 1, y)){
		piece = gameBoard.board[x - 1][y].getPiece();
		if (isOpponentPiece(piece))
		{
			return true;
		}
	}

	//left
	if (isLegalPosition(x, y - 1)){
		piece = gameBoard.board[x][y - 1].getPiece();
		if (isOpponentPiece(piece))
		{
			return true;
		}
	}

	return false;
}

/**
    game manager will use this function to update AI algorithm
    on board positions and fight results after locating all pieces on board.

    @params: b - Board instance to get board positions
    fights - vector of all fight results that occur after piece positioning.
 */
void  RSPPlayer_204157862::notifyOnInitialBoard(const Board &b,
		const std::vector<unique_ptr<FightInfo>> &fights)
{
	updateBoard(b);
	for (int i = 0; i < (int)fights.size(); ++i)
	{
		int x = fights[i]->getPosition().getX()-1; //col
		int y = fights[i]->getPosition().getY()-1; //row
		char myPiece = toupper(fights[i]->getPiece(myPlayerNum)); //do we need this?
		char opponentPiece = toupper(fights[i]->getPiece(myPlayerNum == 1 ? 2 : 1));
		int winner = fights[i]->getWinner();

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
}

/**
    AI algorithm update for each opponent piece its probability to represent a flag.
 */
void  RSPPlayer_204157862::updateFlagProbability()
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
				gameBoard.board[i][j].flagProbability = (double)FLAGS_NUM / (double)unkownPiecesNum;
			}
		}
	}
}

/**
    game manager will use this function to get auto player's move

    @return: move - chosen move to play
 */
unique_ptr<Move>  RSPPlayer_204157862::getMove()
{
	int from_x = -3;
	int from_y = -3;
	int to_x = -3;
	int to_y = -3;
	getBestMove(from_x, from_y, to_x, to_y);
	unique_ptr<Move> move = make_unique<RPSMove>(RPSpoint(from_x+1, from_y+1), RPSpoint(to_x+1, to_y+1));

	return move;
}

/**
    game manager will use this function to get a joker change , if changed

    @return: nullptr - if joker was not changed.
    instance represents joker's change - otherwise.
 */
unique_ptr<JokerChange>  RSPPlayer_204157862::getJokerChange()
{
	int joker_x = -1;
	int joker_y = -1;
	char newRep = -1;
	char bestRep = newRep;
	double score = calcScore(1, 0, 0, -1, -1);
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

	AICell::updateCell(gameBoard.board[joker_x][joker_y],bestRep,true);
	unique_ptr<JokerChange> jokerChange = make_unique<RPSJokerChange>(toupper(bestRep), RPSpoint(joker_x+1, joker_y+1));
	return jokerChange;
}

/**
    get best move from AI algorithm to play. tries all moves of current player and decides
    which move is the best to play.

    @params: from_x,from_y - from where to move
    to_x,to_y - to where to move
 */
void  RSPPlayer_204157862::getBestMove(int &from_x, int &from_y, int &to_x, int &to_y)
{
	cout<<"getBestMove"<<endl;
	double score;
	int maxScore = INT_MIN;
	cout<<"before for1"<<endl;
	for (int i = 0; i < COLS; ++i)
	{
		for (int j = 0; j < ROWS; ++j)
		{
			cout<<"inside for 2"<<endl;
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
	cout<<"myCell"<<endl;
	myCell = gameBoard.board[from_x][from_y];
	//make move
	cout<<"myCell"<<endl;
	if (gameBoard.board[to_x][to_y].getPiece() == 0)
	{
		cout<<"gameBoard.board[to_x][to_y]"<<endl;
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

/**
    get best move of a specific piece. try all legal moves for a piece and choose
    the best one.

    @params: score - previous best move score
    from_x,from_y - from where to move
    to_x,to_y - to where to move
 */
double  RSPPlayer_204157862::getBestMoveForPiece(double score, const int &from_x, const int &from_y, int &to_x, int &to_y)
{
	int x = from_x; //col
	int y = from_y; //row
	int currScore;

	//move right
	unique_ptr<Move> move = make_unique<RPSMove>(RPSpoint(from_x, from_y), RPSpoint(x, y + 1));
	if (isLegalMove(move, myPlayerNum == 1))
	{
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

/**
    move piece to get its score and then undo the move.

    @params: move - move to play
    @return: score - score of current move that was tried.
 */
double  RSPPlayer_204157862::tryMovePiece(unique_ptr<Move> &move)
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
	bool lostPieceInFight = false;
	bool isProbOne = false;
	double score;

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
		lostPieceInFight = tryToFight(to_x, to_y,
				gameBoard.board[from_x][from_y].getPiece(),
				gameBoard.board[from_x][from_y].getIsJoker(), isProbOne);
		Cell::cleanCell(gameBoard.board[from_x][from_y]);
	}
	if (!isProbOne) willBeFight = true;

	if (isProbOne && lostPieceInFight)
	{
		score = INT_MIN;
	}
	else{
		score = calcScore(material, discovery, reveal, to_x, to_y);
		willBeFight = false;
	}

	//return board to be as it was
	AICell::updateCell(gameBoard.board[from_x][from_y], myCell.getPiece(),
			myCell.getIsJoker());
	AICell::updateCell(gameBoard.board[to_x][to_y], opponentCell.getPiece(),
			opponentCell.getIsJoker());
	AICell::updateCellKnowlage(gameBoard.board[from_x][from_y], myCell);
	AICell::updateCellKnowlage(gameBoard.board[to_x][to_y], opponentCell);
	return score;
}

/**
    try to make a fight of move considered to be played by AI algorithm.
    we might not know who is the opponent we are fighting against -
    if we know we will perform the fight. otherwise, we will choose fight result randomly.

    @params: to_x,to_y - fight position
    myPiece - my piece to participate the fight at
    is joker - is my piece joker - true, else - false.
    isProbOne - function should set this referenced parameter to true if we know opponent's piece for sure.
    (and the fight result is real)
    @return: true - current player lost (my player)
    else - false.
 */
bool  RSPPlayer_204157862::tryToFight(int to_x, int to_y, char myPiece, bool isJoker, bool& isProbOne)
{
	char opponentPiece = gameBoard.board[to_x][to_y].getPiece();
	if (opponentPiece != '#')
	{ //we know the opponent piece so we can simulate fight normally
		isProbOne = true;
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

/**
    perform a fight with my moving piece against opponent's piece.

    @params: x,y - fight position
    myPiece - my piece to participate the fight at
    opponentPiece - opponent piece to participate the fight at
    isMyPiecejoker - is my piece joker - true, else - false.
    @return: true - current player lost a piece (my player)
    else - false.
 */
bool  RSPPlayer_204157862::fight(int x, int y, char myPiece, char opponentPiece, bool isMyPieceJoker)
{

	char myPieceRep = toupper(myPiece);
	char oppPieceRep = toupper(opponentPiece);
	//Case 1: 2 players in the same type - tie
	if (myPieceRep == oppPieceRep)
	{
		Cell::cleanCell(gameBoard.board[x][y]);
		return true;
	}

	//case 2: there is bomb and my player has another piece
	else if (oppPieceRep == BOMB)
	{
		Cell::cleanCell(gameBoard.board[x][y]);
		return true;
	}

	//Case 3: there is flag and current player has another piece
	else if (oppPieceRep == FLAG)
	{
		Cell::updateCell(gameBoard.board[x][y], myPiece, isMyPieceJoker);
		return false;
	}

	//case 4: there is PAPER and current player has another piece
	else if (oppPieceRep == PAPER)
	{
		if (myPieceRep == ROCK)
		{
			return true;
		}
		else if (myPieceRep == SCISSOR)
		{
			Cell::updateCell(gameBoard.board[x][y], myPiece,
					isMyPieceJoker);
			return false;
		}
	}
	//case 5: opponent piece is ROCK and i have another piece
	else if (oppPieceRep == ROCK)
	{
		if (myPieceRep == PAPER)
		{
			Cell::updateCell(gameBoard.board[x][y], myPiece,
					isMyPieceJoker);
			return false;
		}
		else if (myPieceRep == SCISSOR)
		{
			return true;
		}
	}

	else if (oppPieceRep == SCISSOR)
	{
		if (myPieceRep == PAPER)
		{
			return true;
		}
		else if (myPieceRep == ROCK)
		{
			Cell::updateCell(gameBoard.board[x][y], myPiece,
					isMyPieceJoker);
			return false;
		}
	}
	return false; //shouldnt get here
}

/**
    calculate a score for a given board state. Use different weights and parameters normalized to one to
    get to that score.

    @params: material - score for strength of piece that just moved.
    discovery - score for opponent's piece discovered in fight.
    reveal - score represents if i should reveal my piece by moving it.
    @return: total score of current move result.
 */
double  RSPPlayer_204157862::calcScore(double material, double discovery, double reveal, int to_x, int to_y)
{
	bool isJoker = false;
	if (to_x != -1 && to_y != -1){
		if (gameBoard.board[to_x][to_y].isMyPiece(myPlayerNum) && gameBoard.board[to_x][to_y].getIsJoker()){
			isJoker = true;
		}
	}
	double flagSaftey = calcFlagSaftey();
	double distanceFromBombOrFlag = to_x == -1 ? 0 : calcDistanceFromBombOrFlag(to_x,to_y);
	double distanceFromUnknownPiece = to_x == -1 ? 0 : calcDistanceFromUnknownPiece(to_x,to_y);

	//we dont want to defend opponent's flag with bombs !!
	if (distanceFromBombOrFlag > 0.9 || distanceFromUnknownPiece > 0.9){
		if (isJoker && !gameBoard.board[to_x][to_y].checkIsMovingPiece()){
			distanceFromBombOrFlag = 0;
			distanceFromUnknownPiece = 0;
		}

	}

	double score = MATERIAL_WEIGHT * material + DISCOVERY_WEIGHT * discovery + REVEAL_WEIGHT * reveal +
			FLAG_SAFTEY_WEUGHT * flagSaftey + DISTANCE_FROM_FLAG_WEIGHT * distanceFromBombOrFlag +
			DISTANCE_FROM_UNKNOWN_WEIGHT * distanceFromUnknownPiece;

	return score;
}

/**
    manager will use this function after both moves of opponent and mine - only if there
    was a fight - to notify about fight result.

    @params: fightInfo - includes pieces participated in fight, who won and in which location
    the fight was.
 */
void  RSPPlayer_204157862::notifyFightResult(const FightInfo &fightInfo)
{
	int x = fightInfo.getPosition().getX()-1;
	int y = fightInfo.getPosition().getY()-1;
	char myPiece = toupper(fightInfo.getPiece(myPlayerNum));
	char opponentPiece = toupper(fightInfo.getPiece(myPlayerNum == 1 ? 2 : 1));
	int winner = fightInfo.getWinner();

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

/**
    calculate the shortest path from a piece of mine to an opponent's flag or bombs
	@params: to_x,to_y: location of my moved piece (new location)
    @return: distance from closest piece of mine to a flag or bomb normalized to 1.
 */
double  RSPPlayer_204157862::calcDistanceFromBombOrFlag(int to_x, int to_y)
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
					distance = calcDistanceFromPiece(i, j, to_x, to_y);
					if (minimalDistance > distance)
						minimalDistance = distance;
				}
			}
		}
	}
	return (double)(ROWS + COLS - minimalDistance) / (double)(ROWS + COLS);
}

/**
    calculate the shortest path from a piece of mine to an opponent's piece
    that was not seen moving yet and suspected as a flag.
	@params: to_x,to_y: location of my moved piece (new location)
    @return: distance from closest piece of mine to an unkown piece.
 */
double  RSPPlayer_204157862::calcDistanceFromUnknownPiece(int to_x, int to_y)
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
					distance = calcDistanceFromPiece(i, j, to_x, to_y);
					if (minimalDistance > distance)
						minimalDistance = distance;
				}
			}
		}
	}
return (double)(ROWS+COLS) - (double)minimalDistance;
}

/**
    calculate a the shortest path from my pieces to a given piece.
	@params: piece_x,piece_y - location of unknown piece
	my_x,my_y - location of my piece to calculate the distance from it to opponent's unkown piece
    @return: distance from my pieces to a piece.
 */
int  RSPPlayer_204157862::calcDistanceFromPiece(int piece_x, int piece_y, int my_x, int my_y)
{
	int distance;
	//there will be a fight with my piece vs unknown piece
	if (willBeFight && my_x == piece_x && my_y == piece_y){
		return 0;
	}
	distance = abs(my_x - piece_x) + abs(my_y - piece_y);
	return distance;
}

/**
    a parameter calculated for scoring function.
    represents how safe is my flag - if it has piece protectors of mine or
    if opponent's pieces are close to me

    @return: score of flag's safety between -1 to 1. can be negative if there is opponent neighbors
    to my flag.
 */
double  RSPPlayer_204157862::calcFlagSaftey()
{
	int protectingBombs = 0; //bombs neighbors to flag counter
	int otherProtectingPieces = 0; //moving pieces neighbors flag counter
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
			if (piece == 'F') //found a flag of mine - now check neighbors (protectors/enemies)
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
		return -1; //flag safety does not matter - no pieces left
	if (enemyPieces > 0)
		return ((double)(-1 * enemyPieces) / (double)totalEnemyPieces);
	return ((double)((2 * protectingBombs) + otherProtectingPieces) / (double)((2 * bombs) + movingPieces));
}

/**
    add to counter if a given neighbor to flag is a protecting or enemy piece.
    this is help-function to countProtectingPieces function.
	@params: piece - neighbor to flag
	x,y - neighbor location
	protecting bombs - function will update this reference to bombs number that are flag's neighbors.
	other protecting pieces - piece neighbors of flag that belong to same player
	enemy pieces - opponent piece neighbors of flag
 */
void  RSPPlayer_204157862::updateNeighborAsProtectorOrEnemy(char piece, int x, int y, int &protectingBombs, int &otherProtectingPieces, int &enemyPieces){
	bool isMyPiece;
	if (piece != 0)
	{
		isMyPiece = gameBoard.board[x][y].isMyPiece(myPlayerNum);

		if (toupper(piece) == 'B')
		{
			if (isMyPiece)
			{
				protectingBombs++;
			}
			else
				enemyPieces++; //enemies piece is next to flag
		}
		else if (toupper(piece) != 'F') //count only moving pieces protectors
		{
			if (isMyPiece)
				otherProtectingPieces++;
			else
				enemyPieces++; //enemies piece is next to flag
		}
	}
}


/**
    calculates protectors and enemies surrounding a piece (current player's flag)
    for flag safety calculation function.
	@params: i,j - flag's location.
	protecting bombs - function will update this reference to bombs number that are flag's neigbors.
	other protecting pieces - piece neighbors of flag that belong to same player
	enemy pieces - opponent piece neighbors of flag
 */
void  RSPPlayer_204157862::countProtectingPieces(int i, int j, int &protectingBombs, int &otherProtectingPieces, int &enemyPieces)

{
	//search down
	char piece = j + 1 > ROWS - 1 ? 0 : gameBoard.board[i][j + 1].getPiece();
	updateNeighborAsProtectorOrEnemy(piece, i , j+1, protectingBombs, otherProtectingPieces, enemyPieces);

	//search up
	piece = j - 1 < ROWS - 1 ? 0 : gameBoard.board[i][j - 1].getPiece();
	updateNeighborAsProtectorOrEnemy(piece, i , j-1, protectingBombs, otherProtectingPieces, enemyPieces);

	//search left
	piece = i - 1 < COLS - 1 ? 0 : gameBoard.board[i - 1][j].getPiece();
	updateNeighborAsProtectorOrEnemy(piece, i-1 , j, protectingBombs, otherProtectingPieces, enemyPieces);

	//search right
	piece = i + 1 > COLS ? 0 : gameBoard.board[i + 1][j].getPiece();
	updateNeighborAsProtectorOrEnemy(piece, i+1 , j, protectingBombs, otherProtectingPieces, enemyPieces);

	//SearchDiagonal
	piece = i - 1 < COLS - 1 || j - 1 < ROWS - 1 ? 0 : gameBoard.board[i - 1][j - 1].getPiece();
	updateNeighborAsProtectorOrEnemy(piece, i - 1, j - 1, protectingBombs, otherProtectingPieces, enemyPieces);
	//SearchDiagonal
	piece = i + 1 > COLS - 1 || j - 1 < ROWS - 1 ? 0 : gameBoard.board[i + 1][j - 1].getPiece();
	updateNeighborAsProtectorOrEnemy(piece, i + 1 , j - 1, protectingBombs, otherProtectingPieces, enemyPieces);
	//SearchDiagonal
	piece = i - 1 < COLS - 1 || j + 1 > ROWS - 1 ? 0 : gameBoard.board[i - 1][j + 1].getPiece();
	updateNeighborAsProtectorOrEnemy(piece, i - 1 , j + 1, protectingBombs, otherProtectingPieces, enemyPieces);
	//SearchDiagonal
	piece = i + 1 > COLS - 1 || j + 1 > ROWS - 1 ? 0 : gameBoard.board[i + 1][j + 1].getPiece();
	updateNeighborAsProtectorOrEnemy(piece, i + 1 , j + 1, protectingBombs, otherProtectingPieces, enemyPieces);

}

/**
    calculate a score of material parameter for scoring function.
    score is given by the kind of a piece that just tried to move.

    @return: score between 0 to 1.
 */
double  RSPPlayer_204157862::calcMaterial(Cell cell)
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
	return -1.0; //should never get here!

}

/**
    calculate a score of discovery parameter for scoring function.
    the less we know about a neighbor we can do a fight with in the next move -
    the higher the score could be.

    @return: discovery parameter score between 0 to 1.
 */
double  RSPPlayer_204157862::calcDiscovery(AICell cell)
{
	int isFlagKnown = cell.flagProbability == 0 ? 0 : 1;
	int isJokerKnown = cell.isJokerKnown ? 0 : 1;
	int isMovingPiece = cell.isMovingPiece ? 0 : 1;

	return (double)(isFlagKnown + isJokerKnown + isMovingPiece) / 3.0; //normalized to one
}

/**
    AI algorithm function trying to change a joker piece to new representation
    and check if it improves current player's game state.

    @params: score - this function will calculate and update best score of joker piece change
    joker_x,joker_y - joker's position
    amIPlayerOne - true - current player's number is one. else - false.
    @return: new representation of highest score board state after joker change.
    if it is better not to do a joker change - return -1.
 */
char  RSPPlayer_204157862::shouldChangeJoker(double &score, int joker_x, int joker_y, bool amIPlayerOne)
{
	char oldRep = gameBoard.board[joker_x][joker_y].getPiece();
	char origRep = oldRep;
	char newRep;
	char bestRep = -1;
	double currScore;
	double material = calcMaterial(gameBoard.board[joker_x][joker_y]);
	double discovery = 0;
	double reveal = 0;

	for (int i = 0; i < 4; ++i) //try all joker representations and
		//then decide by highest score if we want and which representation to give to joker
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
		currScore = calcScore(material, discovery, reveal, joker_x, joker_y);

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

/**
    when AI algorithm is sure - changes all opponent's unknown pieces to moving pieces.
 */
void  RSPPlayer_204157862::updateMovingPiece()
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
}

/**
	update AI board after manager notified on board's positions.
	@params: b - board given by manager
 */
void  RSPPlayer_204157862::updateBoard(const Board &b)
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

/**
	manager will ue this function to notify AI on opponent's move. and AI will
	use this data to update its board and learn about the game.
 */
void  RSPPlayer_204157862::notifyOnOpponentMove(const Move &move)
{
	int from_x = move.getFrom().getX()-1;
	int from_y = move.getFrom().getY()-1;
	int to_x = move.getTo().getX()-1;
	int to_y = move.getTo().getY()-1;
	AICell fromCell = gameBoard.board[from_x][from_y];

	//in case i am the only player plays - can happen only in auto-vs-file mode when move file is over before victory
	if (gameBoard.board[from_x][from_y].isMyPiece(myPlayerNum))
		return;
	//in case there is no fight we can update the AI board
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

/**
    check if the move AI tried to play is legal

    @return: true - move is legal. else - false.
 */
bool  RSPPlayer_204157862::isLegalMove(unique_ptr<Move> &move, bool isPlayer1) {
	//get move positions
	int from_x = move->getFrom().getX();
	int from_y = move->getFrom().getY();
	int to_x = move->getTo().getX();
	int to_y = move->getTo().getY();

	//check if position is in board limits
	if ((from_x < 0 || from_x > COLS-1)
			|| (to_x < 0 || to_x > COLS-1) || (from_y < 0 || from_y > ROWS-1)
			|| (to_y < 0 || to_y > ROWS-1)) {
		return false;
	}

	//check there was an actual move
	if (from_x == to_x && from_y == to_y) {
		return false;
	}

	//check if not trying to move empty piece
	if (gameBoard.board.at(from_x).at(from_y).getPiece() == 0) {
		return false;
	}//check if not trying to move opponent's piece
	else if ((isPlayer1 && islower(gameBoard.board.at(from_x).at(from_y).getPiece()))
			|| (!isPlayer1 && isupper(gameBoard.board[from_x][from_y].getPiece()))) {
		return false;
	}
	//check if not trying to move unmoving piece
	if (toupper(gameBoard.board.at(from_x).at(from_y).getPiece()) == BOMB
			|| toupper(gameBoard.board[from_x][from_y].getPiece()) == FLAG) {
		return false;
	}
	//check not trying to move to illegal cell
	if (to_x == from_x + 1 || to_x == from_x - 1) {
		if (to_y != from_y) {
			return false;
		}
	}
	else if (to_y == from_y + 1 || to_y == from_y - 1) {
		if (to_x != from_x) {
			return false;
		}
	}
	else {
		return false;
	}
	//check not trying to move to a cell taken by my own piece
	if (gameBoard.board.at(to_x).at(to_y).isMyPiece(myPlayerNum)) {
		return false;
	}

	return true;
}
