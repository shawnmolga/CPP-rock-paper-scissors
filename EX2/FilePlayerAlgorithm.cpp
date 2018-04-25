/*
 * FilePlayerAlgorithm.cpp
 *
 *  Created on: 24 ����� 2018
 *      Author: OR
 */

#include "FilePlayerAlgorithm.h"

FilePlayerAlgorithm::FilePlayerAlgorithm(const string &posFile, const string &moveFile) :
positionFile(posFile) , movesFile(moveFile){}

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
	//TODO: asked in forum if checks like that can be performed here or only by game manager
	//no answer yet... might be deleted

	if ((size_t)end >= line.length())
	{
		cout << "Error - bad format - missing piece in line" << endl;
		return -1;
	}

	while (line[end] == ' ')
	{
		end++;
		//TODO: asked in forum if checks like that can be performed here or only by game manager
		//no answer yet... might be deleted

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

int FilePlayerAlgorithm::getPositionFromLine(int start, const string &line, int &row, int &col)
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

void FilePlayerAlgorithm::getPositionAndRepFromLine(const string &line, int playerNum, int &row, int &col,
		char &jokerRep, char &piece)
{
	int pieceIndex = getPieceFromLine(0, line);
	piece = line[pieceIndex];

	//TODO: asked in forum if checks like that can be performed here or only by game manager
	//no answer yet... might be deleted
	if ((size_t)pieceIndex + 1 >= line.length())
	{
		cout << "Error - bad format: missing position of piece" << endl;
		return;
	}
	//TODO: asked in forum if checks like that can be performed here or only by game manager
	//no answer yet... might be deleted
	if (line[pieceIndex + 1] != ' ')
	{
		cout << "Error - bad format: missing space after piece" << endl;
		return;
	}
	int nextIndex = getPositionFromLine(pieceIndex + 1, line, row, col);
	if (nextIndex == -1)
		return;
	//check if position is legal
	if ((row < 1 || row > ROWS) || (col < 1 || col > COLS))
	{
		cout << "Error: illegal location on board" << endl;
		return;
	}
	if ((size_t)nextIndex >= line.length())
	{
		cout << "Error - bad format: missing joker rep piece" << endl;
		return;
	}

	if (line[nextIndex] != ' ')
	{
		cout
		<< "Error - bad format: missing space after positions in joker position"
		<< endl;
		return;
	}
	nextIndex = getPieceFromLine(nextIndex, line) + 1;
	if (nextIndex == 0)
	{
		cout << "Error: Bad format - no piece to position as joker" << endl;
		return;
	}
	char jokerPiece = line[nextIndex - 1];
	if (jokerPiece != ROCK && jokerPiece != PAPER && jokerPiece != SCISSOR && jokerPiece != BOMB)
	{
		cout << "Error: Bad format - illegal piece for joker" << endl;
		return;
	}

	jokerRep = jokerPiece;

	//check that after position line is empty
	if (!checkEmptyLine(nextIndex, line))
	{
		cout << "Error: Bad format - junk characters after position" << endl;
		//TODO: this will be the only way for game managar to know something is wrong....
		jokerRep = -1;
		return;
	}

}

virtual void FilePlayerAlgorithm::getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill){
	ifstream posFile(positionFile);
	string line;
	int row = -1;
	int col = -1;
	char piece = 0;
	char jokerRep = '#';
	int indexLine = 1; //Counting the lines;
	while (getline(posFile, line))
	{
		//skip empty lines
		if (checkEmptyLine(0, line))
		{
			indexLine++;
			continue;
		}
		getPositionAndRepFromLine(line, player, row, col, jokerRep, piece));

		//position is illegal - tried to locate 2 pieces of same player in same position
		if (board[row - 1][col - 1] != 0)
		{

			if (playerNum == 1)
			{
				indexErrorPosOne = indexLine;
			}
			else
			{
				indexErrorPosTwo = indexLine;
			}

			cout
			<< "Error: two or more pieces are positioned on the same location"
			<< endl;
			posFile.close();
			return;
		}

		auto p = make_unique<RPSPiecePosition>(RPSpoint(row,col),piece,jokerRep);
		vectorToFill.push_back(p);
		indexLine++;
	}
	if (playerPositionFile.bad())
	{
		cout << "Error while reading position file. Exiting game" << endl;
		posFile.close();
		return;
	}

	posFile.close();
}

//will not be used in file player method
virtual void FilePlayerAlgorithm::notifyOnInitialBoard(const Board& b, const std::vector<unique_ptr<FightInfo>>& fights){}
}

