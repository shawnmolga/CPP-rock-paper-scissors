/*
 * RCPgameManager.h
 *
 *  Created on: 27 ���� 2018
 *      Author: OR
 */

#ifndef EX1_RCPGAMEMANAGER_H_
#define EX1_RCPGAMEMANAGER_H_

#include <iostream>
#include <fstream>
#include <string>
#include <fstream>
#include "Constants.h"
#include "Player.h"
#include "RCPgame.h"
#include "dirent.h"

using namespace std;

class RCPgameManager{
	public:
		RCPgameManager();
		~RCPgameManager();
		RCPgameManager(const RCPgameManager& manager) = delete;
		RCPgameManager& operator=(const RCPgameManager& manager) = delete;
		RCPgame * game;
		bool checkInsertPlayerPosition(int playerNum, ifstream & playerPositionFile);
		bool checkInputFiles();
		void printOutputFile(const string &outputFile);
		void startGame();
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
		void updateJokerChange(char prevJokerRep, char newRep, bool isPlayerOne);
		void printBoardToCout();//todo  delete this

};

#endif /* EX1_RCPGAMEMANAGER_H_ */
