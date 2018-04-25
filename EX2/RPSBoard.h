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
#include <sstream>

class RPSBoard : public Board{
public:
	RPSBoard();
	~RPSBoard();
	virtual int getPlayer(const Point& pos) const; // 1 for player 1�s piece, 2 for 2, 0 if empty
private:
vector<vector<char>> board;
};


#endif /* EX2_RPSBOARD_H_ */
