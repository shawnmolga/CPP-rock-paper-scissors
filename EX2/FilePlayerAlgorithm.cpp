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
	playerMoveFile.open (moveFile, std::ofstream::in);

}

FilePlayerAlgorithm::~FilePlayerAlgorithm(){
	playerMoveFile.close();

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
	for (int i = start; i < (int)line.length(); ++i) //skip space or enter
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

	if ((size_t)end >= line.length()) //start reading after line ended
	{
		cout << "Error - bad format - missing piece in line" << endl;
		return -1;
	}

	while (line[end] == ' ') //skip spaces until piece
	{
		end++;
		if ((size_t)end >= line.length())
		{
			cout << "Error - bad format - missing piece in line" << endl;
			return -1;
		}
	}
	return end; //return piece index
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
	if ((size_t)start >= line.length()) //try reading from index after line ended
	{
		cout << "Error - bad format - missing position in line" << endl;
		return -1;
	}

	while (line[start] == ' ') //skip spaces until column position
	{
		start = start + 1;
		if ((size_t)start >= line.length())
		{
			cout << "Error - bad format - missing position in line" << endl;
			return -1;
		}
	}
	if (!checkIfDigit(line[start])) //verify this is a digit
	{
		cout << "Error: Bad format - got letter instead of digit" << endl;
		return -1;
	}
	int end = start + 1;
	if ((size_t)end >= line.length()) //there is no more characters in line - missing row
	{
		cout << "Error - bad format - missing position in line" << endl;
		return -1;
	}

	while (line[end] != ' ') //skip spaces
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

	//try to get row
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
	while (line[start] == ' ') //skip spaces
	{
		start = start + 1;
		if ((size_t)start >= line.length())
		{
			cout << "Error - bad format - missing position in line" << endl;
			return -1;
		}
	}
	if (!checkIfDigit(line[start])) //check row is a number
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

	if ((size_t) nextIndex >= line.length()) { //no joker representation in line - we've reached end of line
		cout << "Error - bad format: missing joker rep piece" << endl;
		return false;
	}
	if (line[nextIndex] != ' ') { //no space between position to joker rep
		cout
				<< "Error - bad format: missing space after positions in joker position"
				<< endl;
		return false;
	}
	nextIndex = getPieceFromLine(nextIndex, line) + 1; //extract joker rep
	if (nextIndex == 0) {
		cout << "Error: Bad format - no piece to position as joker" << endl;
		return false;
	}
	char jokerPiece = line[nextIndex - 1];
	//chep rep is legal for joker
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
	int pieceIndex = getPieceFromLine(0, line); //extract piece from line 
	piece = line[pieceIndex];
	if ((size_t)pieceIndex + 1 >= line.length()) //no position in line
	{
		cout << "Error - bad format: missing position of piece" << endl;
		return false;
	}
	if (line[pieceIndex + 1] != ' ') //no space between piece to poition - must be
	{
		cout << "Error - bad format: missing space after piece" << endl;
		return false;
	}
	int nextIndex = getPositionFromLine(pieceIndex + 1, line, row, col);
	if (nextIndex == BAD_FORMAT_ERR) //bad position
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
	if (!checkEmptyLine(nextIndex, line)) //check no junk characters in line
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
	char jokerRep = '#'; //empty rep
	while (getline(posFile, line))
	{
		jokerRep = '#';
		//skip empty lines
		if (checkEmptyLine(0, line)) //skip empty lines
		{
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

		char new_rep = 'E'; //empty rep
		//skip all spaces until next char
		RPSpoint point(0, 0);
		nextIndex = getPieceFromLine(nextIndex, line1) + 1; //get joker piece
		if (nextIndex == 0)
		{
			return make_unique<RPSJokerChange>(new_rep,point);
		}
		if (line1[nextIndex - 1] != JOKER)
		{
			cout << "Error: Bad format - Junk characters in line" << endl;
			return make_unique<RPSJokerChange>(new_rep,point);
		}
		if (line1[nextIndex] != ':')
		{
			cout
					<< "Error: Bad format - Joker  hard coded information not placed correctly - need to be followed by colon"
					<< endl;
			return make_unique<RPSJokerChange>(new_rep,point);
		}

		nextIndex++;

		if (line1[nextIndex] != ' ') //missing space
		{
			cout << "Bad Format - missing space before joker change" << endl;
			return make_unique<RPSJokerChange>(new_rep,point);
		}
		int x_joker ;
		int y_joker ;
		nextIndex = getPositionFromLine(nextIndex, line1, x_joker, y_joker);
		if (nextIndex == BAD_FORMAT_ERR) //position error
		{
			return make_unique<RPSJokerChange>(new_rep,point);
		}
		
		nextIndex = 1 + getPieceFromLine(nextIndex, line1); //get joker new rep
		if (nextIndex == 0)
		{
			return make_unique<RPSJokerChange>(new_rep,point);
		}
		if (line1[nextIndex - 2] != ' ') //missing space
		{
			cout << "Bad Format - missing space before joker translation piece"
				 << endl;
			return make_unique<RPSJokerChange>(new_rep,point);
		}
		new_rep = line1[nextIndex - 1];
		if (!checkEmptyLine(nextIndex, line1))
		{
			cout << "Error: Bad format - rest of line is not empty" << endl;

			new_rep = 'E';
			return make_unique<RPSJokerChange>(new_rep,point);
		}
		point.setX(y_joker);
		point.setY(x_joker);
		return make_unique<RPSJokerChange>(new_rep,point);
	}
	else
		return nullptr; //if no jokerChange - return Null;
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

	//check if there was a problem while parsing
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
	if (getline(playerMoveFile, line1)) //read player's move from file
	{
		string local_line;
		copyString(local_line, line1);
		if (checkEmptyLine(0, local_line)) //if empty move
			from.setX(0);
		else
			//return move
			return handleNonEmptyLine(local_line, from_x, from_y, to_x, to_y, from, to);

	}
	//If we reach eof we will return nullptr
	else if(playerMoveFile.eof()){
		return nullptr;
	}
	else if(playerMoveFile.bad())
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
