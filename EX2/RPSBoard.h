/*
 * RPSBoard.h
 *
 *  Created on: 24 ����� 2018
 *      Author: OR
 */

#ifndef EX2_RPSBOARD_H_
#define EX2_RPSBOARD_H_
#include "Board.h"
#include "Constants.h"
#include <vector>
#include <sstream>
#include "Cell.h"
#include "Point.h"
#include "RPSpoint.h"
class RPSBoard : public Board
{
  public:
	RPSBoard();
	~RPSBoard();
	virtual int getPlayer(const Point &pos) const; // 1 for player 1�s piece, 2 for 2, 0 if empty
	vector<vector<Cell>> board;
	//private:
};

#endif /* EX2_RPSBOARD_H_ */
