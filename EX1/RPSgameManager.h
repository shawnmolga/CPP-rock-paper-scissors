/*
 * RPSgameManager.h
 *
 *  Created on: 27 ���� 2018
 *      Author: OR
 */

#ifndef EX1_RPSGAMEMANAGER_H_
#define EX1_RPSGAMEMANAGER_H_

#include <iostream>
#include <fstream>
#include <string>
#include <fstream>
#include "Constants.h"
#include "Player.h"
#include "dirent.h"
#include "RPSgame.h"

using namespace std;

class RPSgameManager{
	public:
		RPSgameManager();
		~RPSgameManager();
		RPSgameManager(const RPSgameManager& manager) = delete;
		RPSgameManager& operator=(const RPSgameManager& manager) = delete;
		RPSgame * game;
		bool checkInsertPlayerPosition(int playerNum, ifstream & playerPositionFile);
		bool checkInputFiles();
		void printOutputFile(const string &outputFile);
		int startGame();
		bool isLegalMove(int from_x, int from_y, int to_x, int to_y, bool isPlayer1);
		bool checkBadFormat();
		int getPieceFromLine(int start, const string &line);
		bool checkEmptyLine(int start, const string &line);
		bool checkPieceOverflow(int numOfPieces[]);
		bool checkPositioningFormat(const string &line, int numOfPositionedPieces[],int playerNum, int &row, int &col, bool &isJoker,
				char &piece);
		void updateJokerMovingPieces();
		void printBoardToFile(ofstream & output);
		int getPositionFromLine(int start, const string &line, int &row, int &col);
		bool makeMove(const string &s, bool isPlayer1);
		bool isAllGameFilesExists();
		bool checkIfDigit(char c);
		void updateJokerChange(char prevJokerRep, char newRep, bool isPlayerOne);

	private:
	int indexErrorPosOne;
	int indexErrorPosTwo;
	int indexErrorMoveOne;
	int indexErrorMoveTwo;

};

#endif /* EX1_RPSGAMEMANAGER_H_ */
