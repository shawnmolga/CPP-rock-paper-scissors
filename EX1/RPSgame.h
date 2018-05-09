/*
 * RPSgame.h
 *
 *  Created on: 27 ���� 2018
 *      Author: OR
 */

#ifndef EX1_RPSGAME_H_
#define EX1_RPSGAME_H_

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
	WRONG_MOVE_FILE_FORMAT_ONE,
	WRONG_MOVE_FILE_FORMAT_TWO,
	TIE_NO_WINNER,
	TIE_ALL_FLAGS_EATEN
};

class RPSgame
{
public:
	RPSgame();
	~RPSgame();
	Cell **board;
	Player playerOne;
	Player playerTwo;
	RPSgame &operator=(RPSgame &game) = delete;

	Player getPlayerOne() const;
	Player getPlayerTwo() const;
	bool getIsGameOver() const;
	GAME_OVER_TYPE getGameOverReason() const;
	bool fight(bool isPlayerOneTurn,int row, int col, char piece, bool isJoker);
	bool checkGameOver(bool isBeforeMove, bool isPlayerOneTurn);
	void setGameOver(int winnerNumber, GAME_OVER_TYPE type);
	string ToString(GAME_OVER_TYPE typeGame,int indexErrorPosOne, int indexErrorPosTwo,int indexErrorMoveOne, int indexErrorMoveTwo);
	void resetGameResults();


		private:
	bool isGameOver;
	GAME_OVER_TYPE gameOverReason;
	void InitBoard();

};

#endif /* EX1_RPSGAME_H_ */
