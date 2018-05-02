/*
 * RPSpoint.h
 *
 *  Created on: 24 באפר׳ 2018
 *      Author: OR
 */

#ifndef EX2_RPSPOINT_H_
#define EX2_RPSPOINT_H_
#include "Point.h"

class RPSpoint : public Point{
public:
	RPSpoint(int x, int y);
	~RPSpoint();
	virtual int getX() const override;
	virtual int getY() const override;
	virtual void setX(int x);
	virtual void setY(int y);
private:
	int x;
	int y;
};



#endif /* EX2_RPSPOINT_H_ */
