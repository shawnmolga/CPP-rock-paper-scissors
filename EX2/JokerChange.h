/*
 * JokerChange.h
 *
 *  Created on: 24 באפר׳ 2018
 *      Author: OR
 */

#ifndef EX2_JOKERCHANGE_H_
#define EX2_JOKERCHANGE_H_
#include "Point.h"

class JokerChange {
public:
	virtual const Point& getJokerChangePosition() const;
	virtual char getJokerNewRep() const; // R, P, S or B (but NOT J and NOT F)
};



#endif /* EX2_JOKERCHANGE_H_ */
