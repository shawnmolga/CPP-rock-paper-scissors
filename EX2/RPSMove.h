/*
 * RPSMove.h
 *
 *  Created on: 28 Apr. 2018
 *      Author: shawn
 */


#ifndef EX2_RPSMOVE_H_
#define EX2_RPSMOVE_H_
#include "Move.h"
#include "Point.h"
#include "RPSpoint.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory> // for unique_ptr
#include "Constants.h"
#include "RPSBoard.h"

class RPSMove : public Move{
public:
	RPSMove(RPSpoint from, RPSpoint to);
	~RPSMove();
	virtual const Point& getFrom () const override;
	virtual const Point& getTo () const override;
	virtual void setFrom(RPSpoint point);
	virtual void setTo(RPSpoint point);
	bool static isLegalMove(RPSBoard gameBoard , unique_ptr<Move> &move, bool isPlayer1);

private:
	RPSpoint from;
	RPSpoint to;

};

#endif /* EX2_RPSMOVE_H_ */


