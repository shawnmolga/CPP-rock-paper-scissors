/*
 * RCPgameManager.cpp
 *
 *  Created on: 27 ���� 2018
 *      Author: OR
 */
#include "RCPgameManager.h"

RCPgameManager::RCPgameManager(string PositionFileP1, string PositionFileP2,
		string moveFilePlayer1, string moveFilePlayer2, string gameOutputFile) :
		game(RCPgame()), posFileP1(PositionFileP1), posFileP2(PositionFileP2), moveFileP1(
				moveFilePlayer1), moveFileP2(moveFilePlayer2), outputFile(
						gameOutputFile) {
}

RCPgameManager::~RCPgameManager() {
	delete &game;
}
int RCPgameManager::getPieceFromLine(int start, const string &line){

	int end = start;

	while (line[end] == ' '){
		end++;
	}

	if (end > line.length()){
		return -1;
	}

	return end;
}

int RCPgameManager::getPositionFromLine(int start, const string &line, int &row, int &col){

	int end = start;

	while (line[end] == ' '){
		end++;
	}

	row = line[end] - '0';
	end++;

	while (line[end] == ' '){
		end++;
	}
	col = line[end] - '0';

	return end;
}

bool RCPgameManager::checkEmptyLine(int start, const string &line){

	size_t length = line.length();

	for (int i=0; i<line.length(); ++i){
		if (line[i] != ' ' && line[i] != '\n') return false;
	}

	return true;
}


bool RCPgameManager::checkPositioningFormat(const string &line, int numOfPositionedPieces[],
		int playerNum, int &row, int &col, bool &isJoker,
		char &piece) {
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
		playerNum == 1 ? game.playerOne.numOfPieces[0]++ : game.playerTwo.numOfPieces[0]++;
		break;
	case PAPER:
		numOfPositionedPieces[1]++;
		playerNum == 1 ? game.playerOne.numOfPieces[1]++ : game.playerTwo.numOfPieces[1]++;
		break;
	case SCISSOR:
		numOfPositionedPieces[2]++;
		playerNum == 1 ? game.playerOne.numOfPieces[2]++ : game.playerTwo.numOfPieces[2]++;
		break;
	case BOMB:
		numOfPositionedPieces[3]++;
		playerNum == 1 ? game.playerOne.numOfPieces[3]++ : game.playerTwo.numOfPieces[3]++;
		break;
	case JOKER:
		numOfPositionedPieces[4]++;
		playerNum == 1 ? game.playerOne.numOfPieces[4]++ : game.playerTwo.numOfPieces[4]++;
		isJoker = true;
		break;
	case FLAG:
		numOfPositionedPieces[5]++;
		playerNum == 1 ? game.playerOne.numOfPieces[5]++ : game.playerTwo.numOfPieces[5]++;
		break;
	default:
		cout << "Error: Bad format - illegal piece" << endl;
		return false;
	}

	int nextIndex = getPositionFromLine(pieceIndex++, line, row, col)+1;
	//check if position is legal
	if ((row < 1 || row > ROWS-1) || (col < 1 || col > COLS-1)) {
		cout << "Error: illegal location on board" << endl;
		return false;
	}

	if (isJoker) {
		nextIndex = getPieceFromLine(nextIndex, line)+1;
		if(nextIndex == -1){
			cout << "Error: Bad format - no piece to position as joker" << endl;
			return false;
		}
		char jokerPiece = line[nextIndex-1];

		if (jokerPiece != ROCK || jokerPiece != PAPER || jokerPiece != SCISSOR
				|| jokerPiece != BOMB) {
			cout << "Error: Bad format - illegal piece for joker" << endl;
			return false;
		}

		piece = jokerPiece;

	}
	//check that after position line is empty
	if(!checkEmptyLine(nextIndex, line)){
		cout << "Error: Bad format - junk characters after position" << endl;
		return false;
	}
	return true;
}

bool RCPgameManager::checkInsertPlayerPosition(int playerNum, ifstream &playerPositionFile)
{
	//by order of pieces in constans file
	//for checking that there are no more than number of pieces allowed
	int numOfPositionedPieces[6] = {0};
	string line;
	int row;
	int col;
	char piece = 0;
	bool isJoker = false;
	int flagCnt = 0;
	//initialize temporary board to check player's positions do not collide
	char board[ROWS][COLS] = {0};

	while (getline(playerPositionFile, line))
	{
		//skip empty lines
		if(checkEmptyLine) continue;

		isJoker = false;
		if (!checkPositioningFormat(line,numOfPositionedPieces, playerNum, row, col, isJoker,
				piece))
		{
			return false;
		}

		//position is illegal - tried to locate 2 pieces of same player in same position
		if (board[row][col] != 0){
			cout
			<< "Error: two or more pieces are positioned on the same location"
			<< endl;
			return false;
		}

		//position on current player temp board
		board[row][col] = piece;

		if (toupper(piece) == FLAG){
			flagCnt++;
		}

		//check if there is a fight in game board
		if (game.board[row][col].getPiece() != 0)
		{
			game.fight(row, col, piece, isJoker);
		}
		else //no fight
		{
			Cell::updateCell(game.board[row][col], piece, isJoker);
		}
	}

	//check if there are not enough flags positioned on board
	if (flagCnt < FLAGS_NUM){
		cout
		<< "Error: Missing flags - not all flags are positioned on board"
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

bool RCPgameManager::checkPieceOverflow(int numOfPieces[]){

	if (numOfPieces[0] > ROCKS_NUM || numOfPieces[1] > PAPERS_NUM || numOfPieces[2] > SCISSORS_NUM || numOfPieces[3] > BOMBS_NUM || numOfPieces[4] > JOKERS_NUM || numOfPieces[5] > FLAGS_NUM)
	{
		cout << "Error: a piece type appears in file more than its number"
				<< endl;
		return true;
	}
	return false;

}

void RCPgameManager::updateJokerMovingPieces(){
	for (int i=0; i<ROW; ++i){
		for(int j=0; j<COL; ++j){
			if (game.board[i][j].getIsJoker()){
				int pieceIndex;
				switch(toupper(game.board[i][j].getPiece())){
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
				isupper(game.board[i][j].getPiece()) ? game.getPlayerOne().numOfPieces[pieceIndex]++ : game.getPlayerTwo().numOfPieces[pieceIndex]++:
			}
		}
	}
}

bool RCPgameManager::checkInputFiles() {
	// Case 1: check if the file exist
	ifstream player1File(posFileP1);
	ifstream player2File(posFileP2);
	ifstream player1MovesFile(moveFileP1);
	ifstream player2MovesFile(moveFileP2);

	if (!player1File.is_open()) {
		cout << "Error: Player1 position file doesnt exist" << endl;
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
		cout << "Error: Player2 position file doesnt exist" << endl;
		return false;
	}
	// Case 2: the files do exists
	player1File.ifstream::close();
	player2File.ifstream::close();
	player1MovesFile.ifstream::close();
	player2MovesFile.ifstream::close();

	return true;
}

void RCPgameManager::printOutputFile(string &outputFile) {
	ofstream output;
	output.open(outputFile, ios::trunc);
	//player 1 is thw winner
	if (game.getPlayerOne().getIsWinner()) {
		output << "Winner : 1" << endl;
	}
	//player 2 is the winner
	else if (game.getPlayerTwo().getIsWinner()) {
		output << "Winner : 2" << endl;
	}
	//tie
	else {
		output << "Winner : 0" << endl;
	}
	output << endl; // the third line must be en empty line!
	output << game.ToString(game.getGameOverReason()) << endl;
	printBoardToFile(output);
	output.ofstream::close();
}

bool RCPgameManager::checkBadFormat() {
	ifstream positionFile1(posFile1);
	ifstream positionFile2(posFile2);
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
		game.setGameOver(1, WRONG_FILE_FORMAT_TWO);
		positionFile1.ifstream::close();
		positionFile2.ifstream::close();
		return true;
	}
	positionFile1.ifstream::close();
	positionFile2.ifstream::close();
	return false;
}

void RCPgameManager::printBoardToFile() {
	ofstream output(outputFile);

	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
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
		if (i != ROWS - 1) {
			output << endl;
		}
	}

	output.ofstream::close();
}

void RCPgameManager::startGame() {

	//check if game already over due to first positions
	if (game.checkGameOver()){
		return;
	}

	//start playing by move files
	ifstream player1Move(moveFileP1);
	ifstream player2Move(moveFileP2);

	//shouldnt happen - checked before game starts
	/*if (!player1Move.is_open()) {
		if (!player2Move.is_open())
			game.setGameOver(0, WRONG_FILE_FORMAT_BOTH);
	}*/

	string line1;
	string line2;

	while (getline(player1Move, line1)) {
		if (!checkEmptyLine(0, line1) && makeMove(line1, true))
			break;
		if (getline(player2Move, line2)) {
			if (!checkEmptyLine(0, line2) && makeMove(line2, false))
				break;
		} else
			break;
	}

	//one move file is over
	if (player1Move.eof()) {
		while (getline(player2Move, line2)) {
			if (!checkEmptyLine(0, line2) && makeMove(line2, false))
				break;
		}
	} else if (player2Move.eof()) {
		while (getline(player1Move, line1)) {
			if (!checkEmptyLine(0, line1) && makeMove(line1, true))
				break;
		}
	}

	player1Move.close();
	player2Move.close();
	return;
}

bool RCPgameManager::isLegalMove(int from_x, int from_y, int to_x, int to_y,
		bool isPlayer1) {

	if ((from_x < 1 || from_x > ROWS-1) || (to_x < 1 || to_x > ROWS-1)
			|| (from_y < 1 || from_y > COLS-1)
			|| (to_y < 1 || to_y > COLS-1)) {
		cout << "Error: illegal location on board" << endl;
		return false;
	}

	if (from_x == to_x && from_y == to_y) {
		cout << "Error: user MUST move one piece" << endl;
		return false;
	}

	if (game.board[to_x][to_y].getPiece() == 0){
		cout << "Error: there is no piece in this position" << endl;
		return false;
	}
	else if ((isPlayer1 && islower(game.board[to_x][to_y].getPiece())) ||
			(!isPlayer1 && isUpper(game.board[to_x][to_y].getPiece()))){
		cout << "Error: trying to move the opponent piece" << endl;
		return false;
	}

	if (game.board[from_x][from_y].getPiece() == BOMB
			|| game.board[from_x][from_y].getPiece() == FLAG) {
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
		} else if (!isPlayer1) {
			if (islower(game.board[to_x][to_y].getPiece())) {
				cout
				<< "Error: you are trying to move to a cell taken by your own piece"
				<< endl;
				return false;
			}
		}
	}

	return true;
}

bool RCPgameManager::makeMove(string s, bool isPlayer1) {
	bool isGameOver = false;

	char from_x = 0;
	char from_y = 0;
	char to_x = 0;
	char to_y = 0;
	bool isJokerChanged = false;

	int nextIndex = getPositionFromLine(0, s, &from_x, &from_y)+1;
	nextIndex = getPositionFromLine(nextIndex, s, &from_x, &from_y)+1;

	int from_x_int = from_x - '0';
	int from_y_int = from_y - '0';
	int to_x_int = to_x - '0';
	int to_y_int = to_y - '0';

	if (!isLegalMove(from_x_int, from_y_int, to_x_int, to_y_int, isPlayer1)) {
		isGameOver = true;
		if (isPlayer1)
			game.setGameOver(2, WRONG_MOVE_FILE_FORMAT_ONE);
		else
			game.setGameOver(1, WRONG_MOVE_FILE_FORMAT_TWO);
		return isGameOver;
	}

	//TODO: from what i understood joker change is optional..
	//no joker change
	if (!checkLineEmpty(nextIndex, s)){
		isJokerChanged = true;

		//skip all spaces until next char
		nextIndex = 1+getPieceFromLine(nextIndex, s);
		if (s[nextIndex-1] != JOKER){
			cout << "Error: Bad format - Joker information not placed correctly" << endl;
			isGameOver = true;
			if (isPlayer1)
				game.setGameOver(2, WRONG_MOVE_FILE_FORMAT_ONE);
			else
				game.setGameOver(1, WRONG_MOVE_FILE_FORMAT_TWO);
			return isGameOver;
		}

		nextIndex = 1+getPieceFromLine(nextIndex, s);
		if (s[nextIndex-1] != ':'){
			cout << "Error: Bad format - Joker information not placed correctly" << endl;
			isGameOver = true;
			if (isPlayer1)
				game.setGameOver(2, WRONG_MOVE_FILE_FORMAT_ONE);
			else
				game.setGameOver(1, WRONG_MOVE_FILE_FORMAT_TWO);
			return isGameOver;
		}

		char x_joker = 0;
		char y_joker = 0;

		nextIndex = getPositionFromLine(nextIndex, s, &x_joker, &y_joker)+1;

		int x_joker_int = x_joker - '0';
		int y_joker_int = y_joker - '0';

		nextIndex = 1+getPieceFromLine(nextIndex, s);
		char new_rep = s[nextIndex-1];
		if (!checkLineEmpty(nextIndex, s)){
			cout << "Error: Bad format - rest of line is not empty" << endl;
			isGameOver = true;
			if (isPlayer1)
				game.setGameOver(2, WRONG_MOVE_FILE_FORMAT_ONE);
			else
				game.setGameOver(1, WRONG_MOVE_FILE_FORMAT_TWO);
			return isGameOver;
		}

		if (!game.board[x_joker][y_joker].getIsJoker()) { //if the original peice is not a joker
			cout << "Error: Piece specified is not joker" << endl;
			isGameOver = true;
			if (isPlayer1)
				game.setGameOver(2, WRONG_MOVE_FILE_FORMAT_ONE);
			else
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
				game.setGameOver(1, WRONG_MOVE_FILE_FORMAT_TWO);
			return isGameOver;
		}

		//CHECK JOKER REPOSITION
		/*size_t pos = s.find("[J: ");
		if (pos == string::npos) {
			isGameOver = true;
			if (isPlayer1)
				game.setGameOver(2, WRONG_MOVE_FILE_FORMAT_ONE);
			else
				game.setGameOver(1, WRONG_MOVE_FILE_FORMAT_TWO);
			return isGameOver;
		}

		if (pos != 8) {
			cout << "Error: Bad format - Joker information not placed correctly"
					<< endl;

			isGameOver = true;
			if (isPlayer1)
				game.setGameOver(2, WRONG_FILE_FORMAT_ONE);
			else
				game.setGameOver(1, WRONG_FILE_FORMAT_TWO);
			return isGameOver;
		}

		size_t posJoker = s.find_first_not_of(" ", pos);

		if (pos == string::npos) {
			cout << "Error: Bad format - Joker information not placed correctly"
					<< endl;
			isGameOver = true;
			if (isPlayer1)
				game.setGameOver(2, WRONG_FILE_FORMAT_ONE);
			else
				game.setGameOver(1, WRONG_FILE_FORMAT_TWO);
			return isGameOver;
		}

		int x_joker = s[posJoker] - '0';
		int y_joker = s[posJoker + 2] - '0';
		if (s[posJoker + 1] != ' ' || s[posJoker + 3] != ' ') {
			cout << "Error: Bad format - should be space" << endl;
			isGameOver = true;
			if (isPlayer1)
				game.setGameOver(2, WRONG_FILE_FORMAT_ONE);
			else
				game.setGameOver(1, WRONG_FILE_FORMAT_TWO);
			return isGameOver;
		}*/

	}

	//i moved the check if move is legal to be before the joker check

	//do move
	if (game.board[to_x_int][to_y_int].getPiece() == 0) {
		Cell::updateCell(game.board[to_x_int][to_y_int],
				game.board[from_x_int][from_y_int].getPiece(),
				game.board[from_x_int][from_y_int].getIsJoker());
		Cell::updateCell(game.board[from_x_int][from_y_int], 0, false);
		isGameOver = game.checkGameOver();
	} else {
		isGameOver = game.fight(to_x_int, to_y_int,
				game.board[from_x_int][from_y_int].getPiece(),
				game.board[from_x_int][from_y_int].getIsJoker());
	}

	//change joker if needed
	if (isJokerChanged){
		Cell::updateCell(game.board[x_joker][y_joker], new_rep, true);
	}

	return isGameOver;
}
