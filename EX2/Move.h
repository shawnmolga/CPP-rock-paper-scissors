/*
 * Move.h
 *
 *  Created on: 24 באפר׳ 2018
 *      Author: OR
 */

#ifndef EX2_MOVE_H_
#define EX2_MOVE_H_
#include "Point.h"

class Move {
public:
	virtual const Point& getFrom() const;
	virtual const Point& getTo() const;
};



#endif /* EX2_MOVE_H_ */
