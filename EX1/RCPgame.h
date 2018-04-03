/*
 * RCPgame.h
 *
 *  Created on: 27 ���� 2018
 *      Author: OR
 */

#ifndef EX1_RCPGAME_H_
#define EX1_RCPGAME_H_

#include <iostream>
#include <fstream>
#include <string>
#include <fstream>
#include "Player.h"
#include "Cell.h"

using namespace std;

enum GAME_OVER_TYPE
{
	ALL_FLAGS_CAPTURED,
	ALL_PIECES_EATEN,
	WRONG_FILE_FORMAT_ONE,
	WRONG_FILE_FORMAT_TWO,
	WRONG_FILE_FORMAT_BOTH,
	TIE_NO_WINNER,
	TIE_ALL_FLAGS_EATEN
};

class RCPgame
{
  public:
	RCPgame();
	~RCPgame();
	Cell **board;
	RCPgame(const RCPgame &game) = delete;
	RCPgame &operator=(const RCPgame &game) = delete;

	Player getPlayerOne() const;
	Player getPlayerTwo() const;

	GAME_OVER_TYPE getGameOverReason() const;
	bool fight(int row, int col, char piece, bool isJoker);
	bool checkGameOver();
	void setGameOver(Player winner, GAME_OVER_TYPE type);

	string ToString(GAME_OVER_TYPE typeGame);

  private:
	//TODO : ask what we should do in case of wrong output file.
	//enum GAME_OVER_TYPE {ALL_FLAGS_CAPTURED,ALL_PIECES_EATEN,WRONG_FILE_FORMAT,TIE};
	bool isGameOver;
	Player playerOne;
	Player playerTwo;
	GAME_OVER_TYPE gameOverReason;
};

#endif /* EX1_RCPGAME_H_ */
