/*
 * RCPgameManager.cpp
 *
 *  Created on: 27 ���� 2018
 *      Author: OR
 */
#include "RCPgameManager.h"

RCPgameManager::RCPgameManager() :
		game (new RCPgame()) {
}

RCPgameManager::~RCPgameManager() {
	delete &game;
}
int RCPgameManager::getPieceFromLine(int start, const string &line) {
	int end = start;

	while (line[end] == ' ') {
		end++;
	}

	if ((size_t) end > line.length()) {
		return -1;
	}

	return end;
}

bool RCPgameManager::isAllGameFilesExists() {
	bool PositionP1Exists = false, PositioP2Exists = false,
			moveP1Exists = false, moveP2Exists = false;
	std::string currentFilename;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(".")) != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL) {
			currentFilename = ent->d_name;
			if (!PositionP1Exists
					&& currentFilename.compare("player1.rps_board") == 0) {
				PositionP1Exists = true;
			} else if (!PositioP2Exists
					&& currentFilename.compare("player2.rps_board") == 0) {
				PositioP2Exists = true;
			} else if (!moveP1Exists
					&& currentFilename.compare("player1.rps_moves") == 0) {
				moveP1Exists = true;
			} else if (!moveP2Exists
					&& currentFilename.compare("player2.rps_moves") == 0) {
				moveP2Exists = true;
			}
		} //while
		closedir(dir);
		if (!PositionP1Exists || !PositioP2Exists || !moveP1Exists
				|| !moveP2Exists) { //some problem with the getline funcion - exit
			std::cout
					<< "Error reading temp dirList file in working directory, Exit from Game."
					<< std::endl;
			return false;
		}
		return true;
	} else {
		std::cout
				<< "Unable to open temp dirList file in working directory, Exit from Game."
				<< std::endl;
		return false;
	}
	return false;
}

int RCPgameManager::getPositionFromLine(int start, const string &line, int &row,
		int &col) {
	while (line[start] == ' ') {
		start = start + 1;
	}
	int end = start + 1;
	while (line[end] != ' ') {
		end = end + 1;
	}
	string row_string = line.substr(start, end);
	row = std::stoi(row_string);
	start = end++;
	while (line[start] == ' ') {
		start = start + 1;
	}
	end = start + 1;
	while (line[end] != ' ') {
		end = end + 1;
	}
	const string col_string = line.substr(start, end);
	col = std::stoi(col_string);
	return end;
}

bool RCPgameManager::checkEmptyLine(int start, const string &line) {

	for (int i = start; i < (int) line.length(); ++i) {
		if (line[i] != ' ' && line[i] != '\n')
			return false;
	}

	return true;
}

bool RCPgameManager::checkPositioningFormat(const string &line,
		int numOfPositionedPieces[], int playerNum, int &row, int &col,
		bool &isJoker, char &piece) {
	//TODO: ask in forum if we have space or other char in end of input line
	if (line.length() < 5) {
		cout << "Error: Bad format - line length is smaller than 5 characters"
				<< endl;
		return false;
	}
	int pieceIndex = getPieceFromLine(0, line);
	piece = line[pieceIndex];
	switch (piece) {
	case ROCK:
		numOfPositionedPieces[0]++;
		playerNum == 1 ?
				game.playerOne.setNumOfPieces(0,
						game.playerOne.numOfPieces[0] + 1) :
				game.playerTwo.setNumOfPieces(0,
						game.playerTwo.numOfPieces[0] + 1);
		break;
	case PAPER:
		numOfPositionedPieces[1]++;
		playerNum == 1 ?
				game.playerOne.setNumOfPieces(1,
						game.playerOne.numOfPieces[1] + 1) :
				game.playerTwo.setNumOfPieces(1,
						game.playerTwo.numOfPieces[1] + 1);
		break;
	case SCISSOR:
		numOfPositionedPieces[2]++;
		playerNum == 1 ?
				game.playerOne.setNumOfPieces(2,
						game.playerOne.numOfPieces[2] + 1) :
				game.playerTwo.setNumOfPieces(2,
						game.playerTwo.numOfPieces[2] + 1);
		break;
	case BOMB:
		numOfPositionedPieces[3]++;
		playerNum == 1 ?
				game.playerOne.setNumOfPieces(3,
						game.playerOne.numOfPieces[3] + 1) :
				game.playerTwo.setNumOfPieces(3,
						game.playerTwo.numOfPieces[3] + 1);
		break;
	case JOKER:
		numOfPositionedPieces[4]++;
		playerNum == 1 ?
				game.playerOne.setNumOfPieces(4,
						game.playerOne.numOfPieces[4] + 1) :
				game.playerTwo.setNumOfPieces(4,
						game.playerTwo.numOfPieces[4] + 1);
		isJoker = true;
		break;
	case FLAG:
		numOfPositionedPieces[5]++;
		playerNum == 1 ?
				game.playerOne.setNumOfPieces(5,
						game.playerOne.numOfPieces[5] + 1) :
				game.playerTwo.setNumOfPieces(5,
						game.playerTwo.numOfPieces[5] + 1);
		break;
	default:
		cout << "Error: Bad format - illegal piece" << endl;
		return false;
	}

	int nextIndex = getPositionFromLine(pieceIndex + 1, line, row, col) + 1;
	//check if position is legal
	if ((row < 1 || row > ROWS) || (col < 1 || col > COLS)) {
		cout << "Error: illegal location on board" << endl;
		return false;
	}
	if (isJoker) {
		nextIndex = getPieceFromLine(nextIndex, line) + 1;
		if (nextIndex == -1) {
			cout << "Error: Bad format - no piece to position as joker" << endl;
			return false;
		}
		char jokerPiece = line[nextIndex - 1];

		if (jokerPiece != ROCK || jokerPiece != PAPER || jokerPiece != SCISSOR
				|| jokerPiece != BOMB) {
			cout << "Error: Bad format - illegal piece for joker" << endl;
			return false;
		}

		piece = jokerPiece;
	}
	//check that after position line is empty
	if (!checkEmptyLine(nextIndex, line)) {
		cout << "Error: Bad format - junk characters after position" << endl;
		return false;
	}

	return true;
}

bool RCPgameManager::checkInsertPlayerPosition(int playerNum,
		ifstream &playerPositionFile) {
	//by order of pieces in constans file
	//for checking that there are no more than number of pieces allowed
	//cout << "in checkInertPlayerposition" << endl;
	int numOfPositionedPieces[6] = { 0 };
	string line;
	int row;
	int col;
	char piece = 0;
	bool isJoker = false;
	int flagCnt = 0;
	//initialize temporary board to check player's positions do not collide
	char board[ROWS][COLS] = { 0 };
	while (getline(playerPositionFile, line)) {
		//skip empty lines
		if (checkEmptyLine(0, line))
			continue;
		isJoker = false;
		if (!checkPositioningFormat(line, numOfPositionedPieces, playerNum, row,
				col, isJoker, piece)) {
			return false;
		}

		//position is illegal - tried to locate 2 pieces of same player in same position
		if (board[row - 1][col - 1] != 0) {
			cout
					<< "Error: two or more pieces are positioned on the same location"
					<< endl;
			return false;
		}

		//position on current player temp board
		if (playerNum == 1) {
			board[row - 1][col - 1] = piece;
		} else {
			board[row - 1][col - 1] = tolower(piece);
		}
		if (toupper(piece) == FLAG) {
			flagCnt++;
		}

		//check if there is a fight in game board
		if (game.board[row][col].getPiece() != 0) {
			if (playerNum == 1) //If it is player one - big letters
					{
				game.fight(true, row, col, piece, isJoker);
			} else //If it is player two -  small letters
			{
				game.fight(false, row, col, tolower(piece), isJoker);
			}
		} else //no fight
		{
			if (playerNum == 1) {
				Cell::updateCell(game.board[row][col], piece, isJoker);
			} else {
				Cell::updateCell(game.board[row][col], tolower(piece), isJoker);
			}
		}
	}
	//check if there are not enough flags positioned on board
	if (flagCnt < FLAGS_NUM) {
		cout << "Error: Missing flags - not all flags are positioned on board"
				<< endl;
		return false;
	}

	//check if there are too many pieces positioned on board
	if (checkPieceOverflow(numOfPositionedPieces))
		return false;

	//after checking no overflow of initial positions on board - add jokers moving pieces to each player's array
	updateJokerMovingPieces();
	return true;
}

bool RCPgameManager::checkPieceOverflow(int numOfPieces[]) {

	if (numOfPieces[0] > ROCKS_NUM || numOfPieces[1] > PAPERS_NUM
			|| numOfPieces[2] > SCISSORS_NUM || numOfPieces[3] > BOMBS_NUM
			|| numOfPieces[4] > JOKERS_NUM || numOfPieces[5] > FLAGS_NUM) {
		cout << "Error: a piece type appears in file more than its number"
				<< endl;
		return true;
	}
	return false;
}

void RCPgameManager::updateJokerMovingPieces() {
	for (int i = 1; i <= ROWS; ++i) {
		for (int j = 1; j <= COLS; ++j) {
			if (game.board[i][j].getIsJoker()) {
				int pieceIndex;
				switch (toupper(game.board[i][j].getPiece())) {
				case ROCK:
					pieceIndex = 0;
					break;
				case PAPER:
					pieceIndex = 1;
					break;
				case SCISSOR:
					pieceIndex = 2;
					break;
				case BOMB:
					pieceIndex = 3;
					break;
				}
				isupper(game.board[i][j].getPiece()) ?
						game.playerOne.setNumOfPieces(pieceIndex,
								game.playerOne.numOfPieces[pieceIndex] + 1) :
						game.playerTwo.setNumOfPieces(pieceIndex,
								game.playerTwo.numOfPieces[pieceIndex] + 1);
			}
		}
	}
}

bool RCPgameManager::checkInputFiles() {
	// Case 1: check if the file exist
	ifstream player1File(PLAYER_ONE_POSITION_FILENAME);
	ifstream player2File(PLAYER_TWO_POSITION_FILENAME);
	ifstream player1MovesFile(PLAYER_ONE_MOVE_FILENAME);
	ifstream player2MovesFile(PLAYER_TWO_MOVE_FILENAME);
	if (!player1File.is_open()) {
		cout << "Error: Player1 position file doesn't exist" << endl;
		player1File.ifstream::close();
		player2File.ifstream::close();
		player1MovesFile.ifstream::close();
		player2MovesFile.ifstream::close();
		return false;
	}
	if (!player2File.is_open()) {
		player1File.ifstream::close();
		player2File.ifstream::close();
		player1MovesFile.ifstream::close();
		player2MovesFile.ifstream::close();
		cout << "Error: Player2 position file doesn't exist" << endl;
		return false;
	}
	// Case 2: the files do exists
	player1File.ifstream::close();
	player2File.ifstream::close();
	player1MovesFile.ifstream::close();
	player2MovesFile.ifstream::close();

	return true;
}

void RCPgameManager::printOutputFile(const string &outputFile) {
	ofstream output;
	output.open(outputFile, ios::trunc);
	//player 1 is thw winner
	if (game.getPlayerOne().getIsWinner()) {
		output << "Winner : 1"<<endl;
	}
	//player 2 is the winner
	else if (game.getPlayerTwo().getIsWinner()) {
		output << "Winner : 2"<<endl;
	}
	//tie
	else {
		output << "Winner : 0"<<endl;
	}
	output << game.ToString(game.getGameOverReason()) << endl;
	output << endl; // the third line must be en empty line!
	printBoardToFile(output);
	output.close();
	return;
}

bool RCPgameManager::checkBadFormat() {
	ifstream positionFile1(PLAYER_ONE_POSITION_FILENAME);
	ifstream positionFile2(PLAYER_TWO_POSITION_FILENAME);
	bool isPlayerOneLegalFormat = checkInsertPlayerPosition(1, positionFile1);
	bool isPlayerTwoLegalFormat = checkInsertPlayerPosition(2, positionFile2);

	if (!isPlayerOneLegalFormat && !isPlayerTwoLegalFormat) {
		game.setGameOver(0, WRONG_FILE_FORMAT_BOTH);
		positionFile1.ifstream::close();
		positionFile2.ifstream::close();
		return true;
	} else if (!isPlayerOneLegalFormat) {
		game.setGameOver(2, WRONG_FILE_FORMAT_ONE);
		positionFile1.ifstream::close();
		positionFile2.ifstream::close();
		return true;
	}

	else if (!isPlayerTwoLegalFormat) {
		cout << "we think its wrong format" << endl;
		game.setGameOver(1, WRONG_FILE_FORMAT_TWO);
		positionFile1.ifstream::close();
		positionFile2.ifstream::close();
		return true;
	}
	positionFile1.ifstream::close();
	positionFile2.ifstream::close();
	return false;
}

void RCPgameManager::printBoardToFile(ofstream &output) {
	for (int i = 1; i <= ROWS; i++) {
		for (int j = 1; j <= COLS; j++) {
			if (game.board[i][j].getIsJoker()) {
				if (Cell::isPlayerOnePiece(game.board[i][j])) {
					output << "J";
				} else {
					output << "j";
				}
			} else if (game.board[i][j].getPiece() == 0) {
				output << " ";
			} else {
				output << game.board[i][j].getPiece();
			}
		}
		if (i != ROWS) {
			output << endl;
		}
	}
	output.ofstream::close();
	return;
}

void RCPgameManager::printBoardToCout() //todo  delete this
{
	for (int i = 1; i <= ROWS; i++) {
		for (int j = 1; j <= COLS; j++) {
			if (game.board[i][j].getIsJoker()) {
				if (Cell::isPlayerOnePiece(game.board[i][j])) {
					cout << "J ";
				} else {
					cout << "j ";
				}
			} else if (game.board[i][j].getPiece() == 0) {
				cout << "- ";
			} else {
				cout << game.board[i][j].getPiece() << " ";
			}
		}
		if (i != ROWS) {
			cout << endl;
		}
	}
	cout << endl;

}

void RCPgameManager::startGame() {
	//check if game already over due to first positions
	bool isGameOver = false;
	bool isPlayerOneTurn = true;
	if (game.checkGameOver(true)) {
		cout << game.getGameOverReason() << endl;
		return;
	}

	//start playing by move files
	ifstream player1Move(PLAYER_ONE_MOVE_FILENAME);
	ifstream player2Move(PLAYER_TWO_MOVE_FILENAME);

	string line1;
	string line2;
	printBoardToCout(); //todo delete this
	while (getline(player1Move, line1)) {
		if (!checkEmptyLine(0, line1)) {
			isPlayerOneTurn = true;
			isGameOver = makeMove(line1, true);
			if (isGameOver)
				break;
		}
		cout << "player1 move" << endl; // todo delete this
		printBoardToCout(); // todo delete this
		if (getline(player2Move, line2)) {
			if (!checkEmptyLine(0, line2)) {
				isPlayerOneTurn = false;
				isGameOver = makeMove(line2, false);
				if (isGameOver)
					break;
			}
			cout << "player2 move" << endl; //todo delete this
			printBoardToCout(); //todo delete this

		} else
			break;
	}
	//game stopped
	if (!isGameOver) {
		if (player1Move.eof()) {
			cout << "player 1 move file ended" << endl; //todo deletee this
			while (getline(player2Move, line2)) {
				if (!checkEmptyLine(0, line2)) {
					isPlayerOneTurn = false;
					isGameOver = makeMove(line2, false);
					if (isGameOver)
						break;
				}
			}
		} else if (player2Move.eof()) {
			cout << "player 2 move file ended" << endl; //todo deletee this
			while (getline(player1Move, line1)) {
				if (!checkEmptyLine(0, line1)) {
					isPlayerOneTurn = true;
					isGameOver = makeMove(line1, true);
					if (isGameOver)
						break;
				}
			}
		}
	}

	player1Move.close();
	player2Move.close();
	if (!game.getIsGameOver() && !game.checkGameOver(isPlayerOneTurn)) {
		game.setGameOver(0, TIE_NO_WINNER);
	}
	return;
}

bool RCPgameManager::isLegalMove(int from_x, int from_y, int to_x, int to_y,
		bool isPlayer1) {

	if ((from_x < 1 || from_x > ROWS) || (to_x < 1 || to_x > ROWS)
			|| (from_y < 1 || from_y > COLS) || (to_y < 1 || to_y > COLS)) {
		cout << "Error: illegal location on board" << endl;
		return false;
	}

	if (from_x == to_x && from_y == to_y) {
		cout << "Error: user MUST move one piece" << endl;
		return false;
	}

	if (game.board[from_x][from_y].getPiece() == 0) {
		cout << "Error: there is no piece in this position" << endl;
		return false;
	} else if ((isPlayer1 && islower(game.board[from_x][from_y].getPiece() != 0))
			|| (!isPlayer1 && isupper(game.board[from_x][from_y].getPiece()))) {
		cout << "Error: trying to move the opponent piece" << endl;
		return false;
	}

	if (toupper(game.board[from_x][from_y].getPiece()) == BOMB
			|| toupper(game.board[from_x][from_y].getPiece()) == FLAG) {
		cout << "Error: flag/bomb piece is not allowed to move" << endl;
		return false;
	}

	if (to_x == from_x + 1 || to_x == from_x - 1) {
		if (to_y != from_y) {
			cout << "Error: illegal move " << endl;
			return false;
		}
	} else if (to_y == from_y + 1 || to_y == from_y - 1) {
		if (to_x != from_x) {
			cout << "Error: illegal move" << endl;
			return false;
		}
	} else {
		cout << "Error: illegal move" << endl;
		return false;
	}

	if (game.board[to_x][to_y].getPiece() != 0) {
		if (isPlayer1) {
			if (isupper(game.board[to_x][to_y].getPiece())) {
				cout
						<< "Error: you are trying to move to a cell taken by your own piece"
						<< endl;
				return false;
			}
		} else {
			if (islower(game.board[to_x][to_y].getPiece())) {
				cout << game.board[from_x][from_y].getPiece() << endl;
				cout << game.board[to_x][to_y].getPiece() << endl;
				cout
						<< "Error: you are trying to move to a cell taken by your own piece"
						<< endl;
				return false;
			}
		}
	}

	return true;
}

//returns true if game is over after move, false otherwise
bool RCPgameManager::makeMove(const string &s, bool isPlayer1) {
	bool isGameOver = false;
	int from_x;
	int from_y;
	int to_x = 0;
	int to_y = 0;
	bool isJokerChanged = false;
	int x_joker = 0;
	int y_joker = 0;
	char new_rep = '0';
	int nextIndex = getPositionFromLine(0, s, from_x, from_y) + 1;

	nextIndex = getPositionFromLine(nextIndex, s, to_x, to_y) + 1;
	if (!isLegalMove(from_x, from_y, to_x, to_y, isPlayer1)) {
		isGameOver = true;
		if (isPlayer1) {
			cout << "we think its wrong format" << endl;
			game.setGameOver(1, WRONG_MOVE_FILE_FORMAT_ONE);
		} else {
			game.setGameOver(2, WRONG_MOVE_FILE_FORMAT_TWO);
		}
		return isGameOver;
	}

	//TODO: from what i understood joker change is optional..
	//no joker change
	if (!checkEmptyLine(nextIndex, s)) {
		isJokerChanged = true;

		//skip all spaces until next char
		nextIndex = 1 + getPieceFromLine(nextIndex, s);
		if (s[nextIndex - 1] != JOKER) {
			cout << "Error: Bad format - Joker information not placed correctly"
					<< endl;
			isGameOver = true;
			if (isPlayer1)
				game.setGameOver(2, WRONG_MOVE_FILE_FORMAT_ONE);
			else
			cout << "we think its wrong format" << endl;
				game.setGameOver(1, WRONG_MOVE_FILE_FORMAT_TWO);
			return isGameOver;
		}

		nextIndex = 1 + getPieceFromLine(nextIndex, s);
		if (s[nextIndex - 1] != ':') {
			cout << "Error: Bad format - Joker information not placed correctly"
					<< endl;
			isGameOver = true;
			if (isPlayer1)
				game.setGameOver(2, WRONG_MOVE_FILE_FORMAT_ONE);
			else
			cout << "we think its wrong format" << endl;
				game.setGameOver(1, WRONG_MOVE_FILE_FORMAT_TWO);
			return isGameOver;
		}

		nextIndex = getPositionFromLine(nextIndex, s, x_joker, y_joker) + 1;

		nextIndex = 1 + getPieceFromLine(nextIndex, s);
		new_rep = s[nextIndex - 1];
		if (!checkEmptyLine(nextIndex, s)) {
			cout << "Error: Bad format - rest of line is not empty" << endl;
			isGameOver = true;
			if (isPlayer1)
				game.setGameOver(2, WRONG_MOVE_FILE_FORMAT_ONE);
			else
			cout << "we think its wrong format" << endl;
				game.setGameOver(1, WRONG_MOVE_FILE_FORMAT_TWO);
			return isGameOver;
		}

		if (!game.board[x_joker][y_joker].getIsJoker()) { //if the original peice is not a joker
			cout << "Error: Piece specified is not joker" << endl;
			isGameOver = true;
			if (isPlayer1)
				game.setGameOver(2, WRONG_MOVE_FILE_FORMAT_ONE);
			else
			cout << "we think its wrong format" << endl;
				game.setGameOver(1, WRONG_MOVE_FILE_FORMAT_TWO);
			return isGameOver;
		}

		if (new_rep != ROCK && new_rep != PAPER && new_rep != SCISSOR
				&& new_rep != BOMB) {
			cout << "Error: illegal NEW_REP " << endl;
			isGameOver = true;
			if (isPlayer1)
				game.setGameOver(2, WRONG_MOVE_FILE_FORMAT_ONE);
			else
			cout << "we think its wrong format" << endl;
				game.setGameOver(1, WRONG_MOVE_FILE_FORMAT_TWO);
			return isGameOver;
		}
	}

	//i moved the check if move is legal to be before the joker check

	//do move
	if (game.board[to_x][to_y].getPiece() == 0) {
		Cell::updateCell(game.board[to_x][to_y],
				game.board[from_x][from_y].getPiece(),
				game.board[from_x][from_y].getIsJoker());
		Cell::cleanCell(game.board[from_x][from_y]);
		isGameOver = game.checkGameOver(isPlayer1);
	} else {
		isGameOver = game.fight(isPlayer1, to_x, to_y,
				game.board[from_x][from_y].getPiece(),
				game.board[from_x][from_y].getIsJoker());

		//if the cell we are fighting is a bomb, the cell needs to get wiped from the board as well
		if (toupper(game.board[to_x][to_y].getPiece()) == 'B')
			Cell::cleanCell(game.board[to_x][to_y]);
		Cell::cleanCell(game.board[from_x][from_y]);

	}

	//change joker if needed
	if (isJokerChanged) {
		updateJokerChange(game.board[x_joker][y_joker].getPiece(), new_rep,
				isPlayer1);
		Cell::updateCell(game.board[x_joker][y_joker], new_rep, true);
	}
	return isGameOver;
}

void RCPgameManager::updateJokerChange(char prevJokerRep, char newRep,
		bool isPlayerOne) {
	//remove one piece from number of prev piece
	switch (prevJokerRep) {
	case ROCK:
		isPlayerOne ?
				game.playerOne.setNumOfPieces(0,
						game.playerOne.numOfPieces[0] - 1) :
				game.playerTwo.setNumOfPieces(0,
						game.playerTwo.numOfPieces[0] - 1);
		break;
	case PAPER:
		isPlayerOne ?
				game.playerOne.setNumOfPieces(1,
						game.playerOne.numOfPieces[1] - 1) :
				game.playerTwo.setNumOfPieces(1,
						game.playerTwo.numOfPieces[1] - 1);
		break;
	case SCISSOR:
		isPlayerOne ?
				game.playerOne.setNumOfPieces(2,
						game.playerOne.numOfPieces[2] - 1) :
				game.playerTwo.setNumOfPieces(2,
						game.playerTwo.numOfPieces[2] - 1);
		break;
	case BOMB:
		isPlayerOne ?
				game.playerOne.setNumOfPieces(3,
						game.playerOne.numOfPieces[3] - 1) :
				game.playerTwo.setNumOfPieces(3,
						game.playerTwo.numOfPieces[3] - 1);
		break;
		break;
	}
	//add new piece to number of curr joker piece
	switch (newRep) {
	case ROCK:
		isPlayerOne ?
				game.playerOne.setNumOfPieces(0,
						game.playerOne.numOfPieces[0] + 1) :
				game.playerTwo.setNumOfPieces(0,
						game.playerTwo.numOfPieces[0] + 1);
		;
		break;
	case PAPER:
		isPlayerOne ?
				game.playerOne.setNumOfPieces(1,
						game.playerOne.numOfPieces[1] + 1) :
				game.playerTwo.setNumOfPieces(1,
						game.playerTwo.numOfPieces[1] + 1);
		break;
	case SCISSOR:
		isPlayerOne ?
				game.playerOne.setNumOfPieces(2,
						game.playerOne.numOfPieces[2] + 1) :
				game.playerTwo.setNumOfPieces(2,
						game.playerTwo.numOfPieces[2] + 1);
		break;
	case BOMB:
		isPlayerOne ?
				game.playerOne.setNumOfPieces(3,
						game.playerOne.numOfPieces[3] + 1) :
				game.playerTwo.setNumOfPieces(3,
						game.playerTwo.numOfPieces[3] + 1);
		break;
		break;
	}
}
