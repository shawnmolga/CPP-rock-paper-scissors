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
//returns true if line is empty
//returns false otherwise
bool FilePlayerAlgorithm::checkEmptyLine(int start, const string &line)
{
	for (int i = start; i < (int)line.length(); ++i)
	{
		if (line[i] != ' ' && line[i] != '\n')
			return false;
	}

	return true;
}

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

bool FilePlayerAlgorithm::checkIfDigit(char c)
{
	int num = c - '0';
	if (num < 0 || num > 9)
	{
		return false;
	}
	return true;
}

/*
 Input- start position, line, row number and col number
 Output - -1 if there is an error otherwise the number of last char in
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
/*
 check if there is bad format in certain line.
 Input- line, player number, reference to row number and col number , joker representation and piece
 Output -  false if there is wrong format otherwise update piece, joker rep and row and col and return true
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
	if (nextIndex == -1)
		return false;
	//check if position is legal
	if ((row < 1 || row > ROWS) || (col < 1 || col > COLS))
	{
		cout << "Error: illegal location on board" << endl;
		return false;
	}
	if (piece == 'J'){

		if ((size_t)nextIndex >= line.length())
		{

			cout << "Error - bad format: missing joker rep piece" << endl;
			return false;
		}
		if (line[nextIndex] != ' ')
		{
			cout
					<< "Error - bad format: missing space after positions in joker position"
					<< endl;
			return false;
		}
		nextIndex = getPieceFromLine(nextIndex, line) + 1;
		if (nextIndex == 0)
		{
			cout << "Error: Bad format - no piece to position as joker" << endl;
			return false;
		}
		char jokerPiece = line[nextIndex - 1];
		if (jokerPiece != ROCK && jokerPiece != PAPER && jokerPiece != SCISSOR && jokerPiece != BOMB)
		{
			cout << "Error: Bad format - illegal piece for joker" << endl;
			return false;
		}

		jokerRep = jokerPiece;

		//check that after position line is empty
		if (!checkEmptyLine(nextIndex, line))
		{
			cout << "Error: Bad format - junk characters after position" << endl;
			jokerRep = -1;
			return false;
		}
	}
	return true;
}

void FilePlayerAlgorithm::getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>> &vectorToFill)
{
	(void)player;
	ifstream posFile(positionFile);
	string line;
	int row = -1;
	int col = -1;
	char piece = 0;
	char jokerRep = '#';
	int indexLine = 1; //Counting the lines;
	while (getline(posFile, line))
	{
		jokerRep = '#';
		//skip empty lines
		if (checkEmptyLine(0, line))
		{
			indexLine++;
			continue;
		}
		if (getPositionAndRepFromLine(line, row, col, jokerRep, piece) == false)
		{
			vectorToFill.push_back(
				make_unique<RPSPiecePosition>(RPSpoint(row, col), -1, jokerRep));
			posFile.close();
			return;
		}
		else
		{
			vectorToFill.push_back(
				make_unique<RPSPiecePosition>(RPSpoint(row, col), piece, jokerRep));
		}
		indexLine++;
		//position is illegal - tried to locate 2 pieces of same player in same position
	}
	//if there was a problem while reading we will put -2 in the piece
	if (posFile.bad())
	{
		cout << "Error while reading position file. Exiting game" << endl;
		vectorToFill.push_back(
			make_unique<RPSPiecePosition>(RPSpoint(row, col), -2, jokerRep));

		posFile.close();
		return;
	}
	posFile.close();
}

//compilation error - unused fightInfo variable !

void FilePlayerAlgorithm::notifyFightResult(const FightInfo& fightInfo){
    (void) fightInfo;
}

void FilePlayerAlgorithm::notifyOnInitialBoard(const Board &b,
											   const std::vector<unique_ptr<FightInfo>> &fights)
{
	(void)fights;
	(void)b;
}


//bad format - new_rep = "E"
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
		if (nextIndex == -1)
		{
			jc->setJokerNewRep('E');
			return make_unique<RPSJokerChange>(*jc);
		}
		point.setX(x_joker);
		point.setY(y_joker);
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


void FilePlayerAlgorithm::copyString(string & src, string toCopy){
	for (int i = 0; i < (int)toCopy.length(); i ++){
		src.push_back(toCopy[i]);
	}
}
/*
returns the next move of current player
empty line (0,0)
wrong format: (-1,0)
getline error: (-3,0)
eof: (-2,0)

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
		{
			//move file wrong format - Point from = (-1, 0)
			from.setX(0);
		}
		else
		{
			nextIndex = getPositionFromLine(0, local_line, from_x, from_y);
			from.setX(from_x);
			from.setY(from_y);

			bool isLackSpace = nextIndex != -1 ? (local_line[nextIndex] != ' ') : false;
			if (nextIndex == -1 || isLackSpace)
			{
				if (isLackSpace)
				{
					cout
						<< "Error: bad format - missing space between source to destination"
						<< endl;
				}
				from.setX(-1);
				//noy added 
				unique_ptr<RPSMove> move = make_unique<RPSMove>(from, to);
				return move;
			}
			else
			{
				nextIndex = getPositionFromLine(nextIndex, local_line, to_x, to_y);
				if (nextIndex == -1)
					from.setX(-1);
				else
				{
					to.setX(to_x);
					to.setY(to_y);
				}
				unique_ptr<RPSMove> move = make_unique<RPSMove>(from, to);
				//noy added
				return move;
			}
		}
	}
	//If we reach eod we will return -2
	else if(player1Move.eof()){
		from.setX(-2);
	}
	else if(player1Move.bad())
	{
		//if getLine error - Point from = (-2,0)
		from.setX(-3);
	}
	//make_unique < RPSPiecePosition
	//						> (RPSpoint(row, col), -2, jokerRep));
	unique_ptr<RPSMove> move = make_unique<RPSMove>(from, to);
	return move;
}

void FilePlayerAlgorithm::incrementMovesFileLine(){
	movesFileLine++;
}
void FilePlayerAlgorithm::notifyOnOpponentMove(const Move& move) { // noy implmenet only for compilation will succcedd
	cout<<move.getTo().getX()<<endl;
} // called only on opponent�s move