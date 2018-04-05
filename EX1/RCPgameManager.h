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

using namespace std;

class RCPgameManager{
	public:
		RCPgameManager(string PositionFileP1, string PositionFileP2, string moveFilePlayer1, string moveFilePlayer2, string gameOutputFile);
		~RCPgameManager();
		RCPgameManager(const RCPgameManager& manager) = delete;
		RCPgameManager& operator=(const RCPgameManager& manager) = delete;
		RCPgame game;

		bool checkPositioningFormat(const string &line, int numOfPieces[], int &row, int &col, bool& isJoker, char& jokerPiece);
		bool checkInsertPlayerPosition(int playerNum, ifstream & playerPositionFile);
		bool checkInputFiles();
		void printOutputFile(const string &outputFile);
		void printBoardToFile(ofstream &output);
		void startGame();
		bool isLegalMove(int from_x, int from_y, int to_x, int to_y, bool isPlayer1);
		bool checkBadFormat();
		int getPieceFromLine(int start, const string &line);
		bool checkEmptyLine(int start, const string &line);
		bool checkPieceOverflow(int numOfPieces[]);
		bool checkPositioningFormat(const string &line, int numOfPositionedPieces[],int playerNum, int &row, int &col, bool &isJoker,
				char &piece);
		void updateJokerMovingPieces();
		void printBoardToFile();
		int getPositionFromLine(int start, const string &line, int &row, int &col);
		bool makeMove(const string &s, bool isPlayer1);


private:
	string posFileP1;
	string posFileP2;
	string moveFileP1;
	string moveFileP2;
	string outputFile;
};

#endif /* EX1_RCPGAMEMANAGER_H_ */
