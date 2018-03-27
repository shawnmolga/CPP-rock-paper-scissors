/*
 * RCPgame.h
 *
 *  Created on: 27 במרץ 2018
 *      Author: OR
 */

#ifndef EX1_RCPGAME_H_
#define EX1_RCPGAME_H_

#include <iostream>
#include <fstream>
#include <string>
#include <fstream>
#include "Constants.h"
#include "Player.h"
#include "Cell.h"

using namespace std;

class RCPgame{

	RCPgame();
	~RCPgame();
	RCPgame(const RCPgame& game) = delete;
	RCPgame& operator=(const RCPgame& game) = delete;

	Player getPlayerOne()const;
	Player getPlayerTwo()const;

	bool fight(int row,int col,char piece, bool isJoker);
	bool checkGameOver();

private:
	//TODO : ask what we should do in case of wrong output file.
	enum GAME_OVER_TYPE {ALL_FLAGS_CAPTURED,ALL_PIECES_EATEN,WRONG_FILE_FORMAT,TIE};
	bool isGameOver;
	Cell** board;
	Player playerOne;
	Player playerTwo;
	GAME_OVER_TYPE gameOverReason;

};

#endif /* EX1_RCPGAME_H_ */
