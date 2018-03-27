/*
 * RCPgameManager.h
 *
 *  Created on: 27 במרץ 2018
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
	RCPgameManager(string PositionFileP1, string PositionFileP2, string moveFilePlayer1, string moveFilePlayer2);
	~RCPgameManager();
	RCPgameManager(const RCPgameManager& manager) = delete;
	RCPgameManager& operator=(const RCPgameManager& manager) = delete;

	bool checkPositioningFormat(const string &line, int numOfPieces[], int &row, int &col, bool& isJoker, char& jokerPiece);
	bool checkPieceOverFlow(int numOfPieces[]);
	bool checkInsertPlayerPosition(int playerNum, ifstream & playerPositionFile);
	bool checkPositioningInputFiles(const string &player1PositionFile, const string &player2PositionFile);

private:
	RCPgame game;
	string posFileP1;
	string posFileP2;
	string moveFileP1;
	string moveFileP2;
};

#endif /* EX1_RCPGAMEMANAGER_H_ */
