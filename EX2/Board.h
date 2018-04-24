/*
 * Board .h
 *
 *  Created on: 24 באפר׳ 2018
 *      Author: OR
 */

#ifndef EX2_BOARD_H_
#define EX2_BOARD_H_
#include "Point.h"

class Board {
public:
	virtual int getPlayer(const Point& pos) const; // 1 for player 1’s piece, 2 for 2, 0 if empty

};


#endif /* EX2_BOARD_H_ */
