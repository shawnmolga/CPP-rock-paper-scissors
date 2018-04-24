/*
 * FightInfo.h
 *
 *  Created on: 24 באפר׳ 2018
 *      Author: OR
 */

#ifndef EX2_FIGHTINFO_H_
#define EX2_FIGHTINFO_H_
#include "Point.h"

class FightInfo {
public:
	virtual const Point& getPosition() const;
	virtual char getOpponentPiece() const; // R, P, S, B or F (but NOT J)
	virtual int getWinner() const; // 0 - both lost, 1 - player 1 won, 2 - player 2 won
};



#endif /* EX2_FIGHTINFO_H_ */
