/*
 * FilePlayerAlgorithm.cpp
 *
 *  Created on: 24 ����� 2018
 *      Author: OR
 */

#include "FilePlayerAlgorithm.h"

FilePlayerAlgorithm::FilePlayerAlgorithm(const string &posFile,
										 const string &moveFile) : positionFile(posFile), movesFile(moveFile),movesFileLine(1),nextIndex(0)
{
	player1Move.open (moveFile, std::ofstream::in);

}

FilePlayerAlgorithm::~FilePlayerAlgorithm(){
	player1Move.close();

}

/**
    checks if line is empty.

    @params: start - from where we need to start to read from line
    line - line to read
    vectorToFill - vector of positions that will be filled
    @return: returns true if line is empty
			 returns false otherwise
 */
bool FilePlayerAlgorithm::checkEmptyLine(int start, const string &line)
{
	for (int i = start; i < (int)line.length(); ++i)
	{
		if (line[i] != ' ' && line[i] != '\n')
			return false;
	}

	return true;
}

/**
    extract piece from line.

    @params: start - from where we need to start to read from line
    line - line to read
    vectorToFill - vector of positions that will be filled
    @return: returns the index we read until it in line
 */
int FilePlayerAlgorithm::getPieceFromLine(int start, const string &line)
{
	int end = start;

	if ((size_t)end >= line.length())
	{
		cout << "Error - bad format - missing piece in line" << endl;
		return -1;
	}

	while (line[end] == ' ')
	{
		end++;
		if ((size_t)end >= line.length())
		{
			cout << "Error - bad format - missing piece in line" << endl;
			return -1;
		}
	}
	return end;
}

/**
    checks if a char read from line represents a digit betweeb 0-9.

    @params: c - character to check
    @return: true - if character is a digit betweeb 0-9
    else - false;
 */
bool FilePlayerAlgorithm::checkIfDigit(char c)
{
	int num = c - '0';
	if (num < 0 || num > 9)
	{
		return false;
	}
	return true;
}

/**
    extract row and column for line.

    @params: start position, line, row number and col number
    @return: -1 if there is an error otherwise the number of last char in
 */
int FilePlayerAlgorithm::getPositionFromLine(int start, const string &line,
											 int &row, int &col)
{
	int size = 1;
	if ((size_t)start >= line.length())
	{
		cout << "Error - bad format - missing position in line" << endl;
		return -1;
	}

	while (line[start] == ' ')
	{
		start = start + 1;
		if ((size_t)start >= line.length())
		{
			cout << "Error - bad format - missing position in line" << endl;
			return -1;
		}
	}
	if (!checkIfDigit(line[start]))
	{
		cout << "Error: Bad format - got letter instead of digit" << endl;
		return -1;
	}
	int end = start + 1;
	if ((size_t)end >= line.length())
	{
		cout << "Error - bad format - missing position in line" << endl;
		return -1;
	}

	while (line[end] != ' ')
	{
		if (!checkIfDigit(line[end]))
		{
			cout << "Error: Bad format - got letter instead of digit" << endl;
			return -1;
		}
		end = end + 1;
		if ((size_t)end >= line.length())
		{
			cout << "Error - bad format - missing position in line" << endl;
			return -1;
		}
		size++;
	}
	//get row - row format was legal
	string col_string = line.substr(start, size);
	col = std::stoi(col_string);

	//try to get col
	size = 1;
	if (line[end] != ' ')
	{
		cout << "Error: Bad format - missing space" << endl;
		return -1;
	}
	start = end++;
	if ((size_t)start >= line.length())
	{
		cout << "Error - bad format - missing position in line" << endl;
		return -1;
	}
	while (line[start] == ' ')
	{
		start = start + 1;
		if ((size_t)start >= line.length())
		{
			cout << "Error - bad format - missing position in line" << endl;
			return -1;
		}
	}
	if (!checkIfDigit(line[start]))
	{
		cout << "Error: Bad format - got letter instead of digit" << endl;
		return -1;
	}
	end = start + 1;
	while ((size_t)end != line.length() && line[end] != ' ')
	{

		if (!checkIfDigit(line[end]))
		{
			cout << "Error: Bad format - got letter instead of digit" << endl;
			return -1;
		}
		size++;
		end = end + 1;
	}
	//col is legal
	const string row_string = line.substr(start, size);
	row = std::stoi(row_string);

	return end;
}

/**
    get joker new representation from line if exist and legal.

    @params: line - line to read joker representation from.
    nextIndex - index to start reading from joker new piece representation.
    jokerRep - new representation of joker read from line.
    @return: return true - if succeed reading new joker piece.
    false - piece representation is not legal.
 */
bool FilePlayerAlgorithm::handleJokerPiece(string line,int & nextIndex, char & jokerRep) {

	if ((size_t) nextIndex >= line.length()) {
		cout << "Error - bad format: missing joker rep piece" << endl;
		return false;
	}
	if (line[nextIndex] != ' ') {
		cout
				<< "Error - bad format: missing space after positions in joker position"
				<< endl;
		return false;
	}
	nextIndex = getPieceFromLine(nextIndex, line) + 1;
	if (nextIndex == 0) {
		cout << "Error: Bad format - no piece to position as joker" << endl;
		return false;
	}
	char jokerPiece = line[nextIndex - 1];
	if (jokerPiece != ROCK && jokerPiece != PAPER && jokerPiece != SCISSOR && jokerPiece != BOMB) {
		cout << "Error: Bad format - illegal piece for joker" << endl;
		return false;
	}

	jokerRep = jokerPiece;
	return true;
}


/**
    check if there is bad format in certain line.

    @params: line, player number, reference to row number and col number , joker represe4ntation and piece.
    @return: false if there is wrong format otherwise update piece, joker rep and row and col and return true.

 */
bool FilePlayerAlgorithm::getPositionAndRepFromLine(const string &line, int &row, int &col, char &jokerRep, char &piece)
{
	int pieceIndex = getPieceFromLine(0, line);
	piece = line[pieceIndex];
	if ((size_t)pieceIndex + 1 >= line.length())
	{
		cout << "Error - bad format: missing position of piece" << endl;
		return false;
	}
	if (line[pieceIndex + 1] != ' ')
	{
		cout << "Error - bad format: missing space after piece" << endl;
		return false;
	}
	int nextIndex = getPositionFromLine(pieceIndex + 1, line, row, col);
	if (nextIndex == BAD_FORMAT_ERR)
		return false;
	//check if position is legal
	if ((row < 1 || row > ROWS) || (col < 1 || col > COLS))
	{
		cout << "Error: illegal location on board" << endl;
		return false;
	}
	if (piece == 'J') {
		if (!FilePlayerAlgorithm::handleJokerPiece(line, nextIndex, jokerRep))
			return false;
	}
	//check that after position line is empty
	if (!checkEmptyLine(nextIndex, line))
	{
		cout << "Error: Bad format - junk characters after position" << endl;
		jokerRep = JOKER_REP_ERROR;
		return false;
	}
	return true;
}

/**
    game manager will use this function to get positions of pieces to locate on board.

    @params: player - current player number
    vectorToFill - vector of positions that will be filled
 */
void FilePlayerAlgorithm::getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill)
{
	(void)player;
	ifstream posFile(positionFile);
	string line;
	int row = -1;
	int col = -1;
	char piece = 0;
	char jokerRep = '#';
	//int indexLine = 1; //Counting the lines;
	while (getline(posFile, line))
	{
		jokerRep = '#';
		//skip empty lines
		if (checkEmptyLine(0, line))
		{
			//indexLine++;
			continue;
		}
		if (getPositionAndRepFromLine(line, row, col, jokerRep, piece) == false)
		{
			vectorToFill.push_back(
					make_unique<RPSPiecePosition>(RPSpoint(col, row), BAD_FORMAT_POS_ERR, jokerRep));
			posFile.close();
			return;
		}
		else
		{
			vectorToFill.push_back(
					make_unique<RPSPiecePosition>(RPSpoint(col, row), piece, jokerRep));
		}
		//indexLine++;
		//position is illegal - tried to locate 2 pieces of same player in same position
	}
	//if there was a problem while reading we will put -2 in the piece
	if (posFile.bad())
	{
		cout << "Error while reading position file. Exiting game" << endl;
		vectorToFill.push_back(
				make_unique<RPSPiecePosition>(RPSpoint(col, row), READ_LINE_POS_ERR, jokerRep));

		posFile.close();
		return;
	}
	posFile.close();
}

/**
	does nothing to algorithm from file
 */
void FilePlayerAlgorithm::notifyFightResult(const FightInfo& fightInfo){
	(void) fightInfo;
}

/**
    does nothing to algorithm from file
 */
void FilePlayerAlgorithm::notifyOnInitialBoard(const Board &b,
											   const std::vector<unique_ptr<FightInfo>> &fights)
{
	(void)fights;
	(void)b;
}

/**
    game manager will use this function to get a joker change , if changed
	note: in this function bad format - new_rep = "E"

    @return: nullptr - if joker was not changed.
    instance represents joker's change - otherwise.
 */
unique_ptr<JokerChange> FilePlayerAlgorithm::getJokerChange()
{
	if (!checkEmptyLine(nextIndex, line1))
	{

		char new_rep = 'E';
		//skip all spaces until next char
		RPSpoint point(0, 0);
		RPSJokerChange* jc = new RPSJokerChange(new_rep, point);
		nextIndex = getPieceFromLine(nextIndex, line1) + 1;
		if (nextIndex == 0)
		{
			new_rep = 'E';
			jc->setJokerNewRep('E');
			//unique_ptr <RPSMove> move = make_unique <RPSMove> (from, to);
			return make_unique<RPSJokerChange>(*jc);
		}
		if (line1[nextIndex - 1] != JOKER)
		{
			cout << "Error: Bad format - Junk characters in line" << endl;
			jc->setJokerNewRep('E');
			return make_unique<RPSJokerChange>(*jc);
		}
		if (line1[nextIndex] != ':')
		{
			cout
					<< "Error: Bad format - Joker  hard coded information not placed correctly - need to be followed by colon"
					<< endl;
			new_rep = 'E';
			jc->setJokerNewRep('E');
			return make_unique<RPSJokerChange>(*jc);
		}

		nextIndex++;

		if (line1[nextIndex] != ' ')
		{
			cout << "Bad Format - missing space before joker change" << endl;
			jc->setJokerNewRep('E');
			return make_unique<RPSJokerChange>(*jc);
		}
		int x_joker ;
		int y_joker ;
		nextIndex = getPositionFromLine(nextIndex, line1, x_joker, y_joker);
		if (nextIndex == BAD_FORMAT_ERR)
		{
			jc->setJokerNewRep('E');
			return make_unique<RPSJokerChange>(*jc);
		}
		point.setX(y_joker); //noy changes due to bug!
		point.setY(x_joker);//noy changes due to bug!
		nextIndex = 1 + getPieceFromLine(nextIndex, line1);
		if (nextIndex == 0)
		{
			jc->setJokerNewRep('E');
			return make_unique<RPSJokerChange>(*jc);
		}
		if (line1[nextIndex - 2] != ' ')
		{
			cout << "Bad Format - missing space before joker translation piece"
				 << endl;
			jc->setJokerNewRep('E');
			return make_unique<RPSJokerChange>(*jc);
		}
		new_rep = line1[nextIndex - 1];
		if (!checkEmptyLine(nextIndex, line1))
		{
			cout << "Error: Bad format - rest of line is not empty" << endl;

			jc->setJokerNewRep('E');
			return make_unique<RPSJokerChange>(*jc);
		}
		jc->setJokerNewRep(new_rep);
		jc->setJokerChangePosition(point);
		return make_unique<RPSJokerChange>(*jc);
	}
	else
		return unique_ptr<RPSJokerChange>{}; //if no jokerChange - return Null;
}

/**
    copy string to string reference
 */
void FilePlayerAlgorithm::copyString(string & src, string toCopy){
	for (int i = 0; i < (int)toCopy.length(); i ++){
		src.push_back(toCopy[i]);
	}
}

/**
    parse line to get move from file. will use this function only if line is not empty.
    @params: local_line - line to read from.
    from_x, from_y - parse from where to move.
    to_x,to_y - parse to where to move.
	from,to - update reference with move parsed from line.
    @return: move parsed from line
 */
unique_ptr<Move> FilePlayerAlgorithm::handleNonEmptyLine(string local_line, int & from_x, int & from_y, int & to_x, int & to_y, RPSpoint& from, RPSpoint& to){
	nextIndex = getPositionFromLine(0, local_line, from_y, from_x);
	from.setX(from_x);
	from.setY(from_y);

	bool isLackSpace = nextIndex != BAD_FORMAT_ERR ? (local_line[nextIndex] != ' ') : false;
	if (nextIndex == BAD_FORMAT_ERR || isLackSpace)
	{
		if (isLackSpace)
			cout
					<< "Error: bad format - missing space between source to destination"
					<< endl;

		from.setX(BAD_FORMAT_ERR);
		unique_ptr<RPSMove> move = make_unique<RPSMove>(from, to);
		return move;
	}
	nextIndex = getPositionFromLine(nextIndex, local_line, to_y, to_x);
	if (nextIndex == BAD_FORMAT_ERR)
		from.setX(BAD_FORMAT_ERR);
	else
	{
		to.setX(to_x);
		to.setY(to_y);
	}
	unique_ptr<RPSMove> move = make_unique<RPSMove>(from, to);
	return move;
}

/**
    game manager will use this function to get player's move from file.
	if empty line return move as (0,0)
	if wrong format: (-1,0) =  BAD_FORMAT_ERR
	if getline error: (-3,0) = READ_LINE_ERR
	if eof: (-2,0) = EOF_MOVE_ERR
    @return: move - chosen move to play
 */
unique_ptr<Move> FilePlayerAlgorithm::getMove()
{

	int from_x = 0;
	int from_y = 0;
	int to_x = 0;
	int to_y = 0;

	RPSpoint from(from_x, from_y);
	RPSpoint to(to_x, to_y);
	if (getline(player1Move, line1))
	{
		string local_line;
		copyString(local_line, line1);
		if (checkEmptyLine(0, local_line))
			from.setX(0);
		else
			return handleNonEmptyLine(local_line, from_x, from_y, to_x, to_y, from, to);

	}
	//If we reach eod we will return -2
	else if(player1Move.eof()){
		from.setX(EOF_MOVE_ERR);
	}
	else if(player1Move.bad())
	{
		//if getLine error - Point from = (-2,0)
		from.setX(READ_LINE_ERR);
	}
	unique_ptr<RPSMove> move = make_unique<RPSMove>(from, to);
	return move;
}

/**
	does nothing to algorithm from file
 */
void FilePlayerAlgorithm::notifyOnOpponentMove(const Move& move) { // noy implmenet only for compilation will succcedd
	(void)move;
} // called only on opponent's move
